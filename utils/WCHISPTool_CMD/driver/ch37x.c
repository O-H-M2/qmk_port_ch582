/*
 * USB ISP device driver
 *
 * Copyright (C) 2022 Nanjing Qinheng Microelectronics Co., Ltd.
 * Web: http://wch.cn
 * Author: WCH@TECH39 <tech@wch.cn>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Update Log:
 * V1.0 - initial version
 */

#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kref.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/poll.h>
#include <linux/slab.h>
#include <linux/usb.h>
#include <linux/version.h>

#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 10, 0))
#include <asm/uaccess.h>
#else
#include <linux/uaccess.h>
#endif

/* table of devices that work with this driver */
static struct usb_device_id ch37x_table[] = {
    {USB_DEVICE(0x1a86, 0x55e0)}, {USB_DEVICE(0x4348, 0x55e0)}, {} /* Terminating entry */
};
MODULE_DEVICE_TABLE(usb, ch37x_table);

/* Get a minor range for your devices from the usb maintainer */
#define USB_ch37x_MINOR_BASE 200
#define COMMAND_TIMEOUT      (2 * HZ) /* seconds timeout for a command */
#define MAX_NUM              4

/* Structure to hold all of our device specific stuff */
struct usb_ch37x {
    struct usb_device *udev;         /* the usb device for this device */
    struct usb_interface *interface; /* the interface for this device */
    struct semaphore limit_sem;      /* limiting the number of writes in progress */
    struct usb_anchor submitted;     /* in case we need to retract our submissions */
    __u16 *ch37x_id;

    size_t bulk_in_num;  /* the number of bulk in endpoint */
    size_t bulk_out_num; /* the number of bulk in endpoint */
	u8 bulk_in_endpointAddr[MAX_NUM];
	u8 bulk_out_endpointAddr[MAX_NUM];

    int errors; /* the last request tanked */
    struct kref kref;
    struct mutex io_mutex;
    spinlock_t buflock;
    int readtimeout;
    int writetimeout;
};

#define to_ch37x_dev(d) container_of(d, struct usb_ch37x, kref)

#define MIN(x, y)   ((x) < (y) ? (x) : (y))
#define DRV_VERSION "WCH USB ISP Driver Version 1.0"

#define GetVersion   0x00000001
#define GetDeviceID  0x00000003
#define GetDeviceDes 0x00000006

#define CH37X_INT3_DOWN 0x8000000f

#define CH37X_SET_TIMEOUT 0x80000010
#define CH37X_BULK_READ   0x80000011
#define CH37X_BULK_WRITE  0x80000012

static struct usb_driver ch37x_driver;

static void ch37x_draw_down(struct usb_ch37x *dev);

static void ch37x_delete(struct kref *kref)
{
    struct usb_ch37x *dev = to_ch37x_dev(kref);
 
    usb_put_dev(dev->udev);
    kfree(dev->ch37x_id);
    kfree(dev);
}

static int ch37x_open(struct inode *inode, struct file *file)
{
    struct usb_ch37x *dev;
    struct usb_interface *interface;
    int subminor;
    int retval = 0;

    subminor = iminor(inode);

    interface = usb_find_interface(&ch37x_driver, subminor);
    if (!interface) {
        printk(KERN_ERR "%s - error, can't find device for minor %d\n", __FUNCTION__, subminor);
        retval = -ENODEV;
        goto exit;
    }

    dev = usb_get_intfdata(interface);
    if (!dev) {
        retval = -ENODEV;
        goto exit;
    }

    retval = usb_autopm_get_interface(interface);
    if (retval) goto exit;

    /* increment our usage count for the device */
    kref_get(&dev->kref);

    /* save our object in the file's private structure */
    file->private_data = dev;

exit:
    return retval;
}

static int ch37x_release(struct inode *inode, struct file *file)
{
    struct usb_ch37x *dev;
    int retval = 0;

    if (file == NULL) {
        return -ENODEV;
        goto exit;
    }

    dev = (struct usb_ch37x *)file->private_data;
    if (dev == NULL) {
        retval = -ENODEV;
        goto exit;
    }

    mutex_lock(&dev->io_mutex);
    if (dev->interface) usb_autopm_put_interface(dev->interface);

    mutex_unlock(&dev->io_mutex);

    /* decrement the count on our device */
    kref_put(&dev->kref, ch37x_delete);

    return 0;

exit:
    return retval;
}

static int ch37x_flush(struct file *file, fl_owner_t id)
{
    struct usb_ch37x *dev;
    int res;

    dev = file->private_data;
    if (dev == NULL) return -ENODEV;

    /* wait for io to stop */
    mutex_lock(&dev->io_mutex);
    ch37x_draw_down(dev);

    /* read out errors, leave subsequent opens a clean slate */
    spin_lock_irq(&dev->buflock);
    res = dev->errors ? (dev->errors == -EPIPE ? -EPIPE : -EIO) : 0;
    dev->errors = 0;
    spin_unlock_irq(&dev->buflock);

    mutex_unlock(&dev->io_mutex);

    return res;
}

/********************************************************************************************/
static ssize_t ch37x_bulk_read(struct usb_ch37x *dev, u8 num, void *buf, u32 count)
{
    int rv;
    int bytes_read = 0;
    int bytes_to_read = count;
    char *buffer;

    buffer = kmalloc(bytes_to_read, GFP_KERNEL);
    if (!buffer) return -ENOMEM;

    rv = usb_bulk_msg(dev->udev, usb_rcvbulkpipe(dev->udev, dev->bulk_in_endpointAddr[num]), buffer, bytes_to_read,
                      &bytes_read, dev->readtimeout);
    if (rv == 0) {
        if (copy_to_user((char __user *)buf, buffer, bytes_read)) {
            rv = -EFAULT;
			goto out;
        }
		rv = bytes_read;
    }

out:
    kfree(buffer);
    return rv;
}

static ssize_t ch37x_bulk_write(struct usb_ch37x *dev, int num, void *buf, unsigned long count)
{
    int rv;
    int bytes_write = 0;
    int bytes_to_write = count;
    char *buffer;

    buffer = kmalloc(bytes_to_write, GFP_KERNEL);
    if (!buffer) return -ENOMEM;

    rv = copy_from_user(buffer, (char __user *)buf, bytes_to_write);
    if (rv) goto out;

    rv = usb_bulk_msg(dev->udev, usb_sndbulkpipe(dev->udev, dev->bulk_out_endpointAddr[num]), buffer, bytes_to_write,
                      &bytes_write, dev->writetimeout);
	if (rv == 0) {
		rv = bytes_write;
    }

out:
    kfree(buffer);
    return rv;
}

long ch37x_ioctl(struct file *ch37x_file, unsigned int cmd, unsigned long arg)
{
    int retval = 0;
    u32 bytes_write = 0;
    u32 bytes_read = 0;
    struct usb_ch37x *dev;
    char *ch37x_version_tmp = DRV_VERSION;
    int readtimeout = 0;
    int writetimeout = 0;
    u8 epnum = 0;
    unsigned long arg1;

    dev = (struct usb_ch37x *)ch37x_file->private_data;

    if (mutex_lock_interruptible(&dev->io_mutex)) return -ERESTARTSYS;

    /* verify that the device wasn't unplugged */
    if (dev->udev == NULL) {
        retval = -ENODEV;
        goto exit;
    }

    switch (cmd) {
    case GetVersion:
        retval = copy_to_user((char __user *)arg, ch37x_version_tmp, strlen(DRV_VERSION));
        break;
    case GetDeviceID:
        retval = copy_to_user((__u16 __user *)arg, dev->ch37x_id, sizeof(__u16) * 2);
        break;
    case GetDeviceDes:
        retval =
            usb_get_descriptor(dev->udev, USB_DT_DEVICE, 0x00, &dev->udev->descriptor, sizeof(dev->udev->descriptor));
        if (retval) retval = copy_to_user((u8 __user *)arg, &dev->udev->descriptor, sizeof(dev->udev->descriptor));
        break;
    case CH37X_SET_TIMEOUT:
        get_user(readtimeout, (u32 __user *)arg);
        get_user(writetimeout, ((u32 __user *)arg + 1));
        if (!retval) {
            dev->readtimeout = readtimeout;
            dev->writetimeout = writetimeout;
        }
        break;
    case CH37X_BULK_READ:
        get_user(epnum, (u8 __user *)arg);
        get_user(bytes_read, (u32 __user *)((u8 *)arg + 1));
        arg1 = (unsigned long)((u8 *)arg + 5);
        retval = ch37x_bulk_read(dev, epnum, (char __user *)arg1, bytes_read);
        break;
    case CH37X_BULK_WRITE:
        get_user(epnum, (u8 __user *)arg);
        get_user(bytes_write, (u32 __user *)((u8 *)arg + 1));
        arg1 = (unsigned long)((u8 *)arg + 5);
        retval = ch37x_bulk_write(dev, epnum, (char __user *)arg1, bytes_write);
        break;

    default:
        retval = -ENOTTY;
        break;
    }
exit:
    /* unlock the device */
    mutex_unlock(&dev->io_mutex);

    return retval;
}

static struct file_operations ch37x_fops = {
    .owner = THIS_MODULE,
    .open = ch37x_open,
    .unlocked_ioctl = ch37x_ioctl,
    .release = ch37x_release,
    .flush = ch37x_flush,
};

/*
 * usb class driver info in order to get a minor number from the usb core,
 * and to have the device registered with devfs and the driver core
 */
static struct usb_class_driver ch37x_class = {
    .name = "usb/ch37x%d",
    .fops = &ch37x_fops,
    .minor_base = USB_ch37x_MINOR_BASE,
};

static int ch37x_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    struct usb_ch37x *dev;
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint;
    int i;
    int retval = -ENOMEM;

    /* allocate memory for our device state and initialize it */
    dev = kmalloc(sizeof(*dev), GFP_KERNEL);
    if (dev == NULL) {
        goto error;
    }
    memset(dev, 0x00, sizeof(*dev));
    kref_init(&dev->kref);

    mutex_init(&dev->io_mutex);
    init_usb_anchor(&dev->submitted);
    dev->ch37x_id = kmalloc(sizeof(char) * 4, GFP_KERNEL);

    dev->udev = usb_get_dev(interface_to_usbdev(interface));
    dev->ch37x_id[0] = dev->udev->descriptor.idVendor;
    dev->ch37x_id[1] = dev->udev->descriptor.idProduct;

    dev->interface = interface;

    /* set up the endpoint information */
    /* use only the first bulk-in and bulk-out endpoints */
    iface_desc = interface->cur_altsetting;
    for (i = 0; i < iface_desc->desc.bNumEndpoints; ++i) {
        endpoint = &iface_desc->endpoint[i].desc;
        /* USB DIR IN && BULK */
        if ((endpoint->bEndpointAddress & 0x80) && ((endpoint->bmAttributes & 3) == 0x02)) {
			dev->bulk_in_endpointAddr[dev->bulk_in_num] = endpoint->bEndpointAddress; 
            dev->bulk_in_num++;
        }
        /* USB DIR OUT && BULK */
        if (!(endpoint->bEndpointAddress & 0x80) && ((endpoint->bmAttributes & 3) == 0x02)) {
			dev->bulk_out_endpointAddr[dev->bulk_out_num] = endpoint->bEndpointAddress; 
            dev->bulk_out_num++;
        }
    }

    /* save our data pointer in this interface device */
    usb_set_intfdata(interface, dev);

    /* we can register the device now, as it is ready */
    retval = usb_register_dev(interface, &ch37x_class);
    if (retval) {
        /* something prevented us from registering this driver */
        printk(KERN_ERR "Not able to get a minor for this device.\n");
        usb_set_intfdata(interface, NULL);
        goto error;
    }

    /* let the user know what node this device is now attached to */
    dev_info(&interface->dev, "USB ch37x device now attached to ch37x-%d", interface->minor);
    return 0;

error:
    if (dev) kref_put(&dev->kref, ch37x_delete);
    return retval;
}

static void ch37x_disconnect(struct usb_interface *interface)
{
    struct usb_ch37x *dev;
    int minor = interface->minor;

    dev = usb_get_intfdata(interface);
    usb_set_intfdata(interface, NULL);

    /* give back our minor */
    usb_deregister_dev(interface, &ch37x_class);

    /* prevent more I/O from starting */
    mutex_lock(&dev->io_mutex);
    dev->interface = NULL;
    mutex_unlock(&dev->io_mutex);

    usb_kill_anchored_urbs(&dev->submitted);

    /* decrement our usage count */
    kref_put(&dev->kref, ch37x_delete);

    dev_info(&interface->dev, "USB ch37x #%d now disconnected", minor);
}

static void ch37x_draw_down(struct usb_ch37x *dev)
{
    int time;

    time = usb_wait_anchor_empty_timeout(&dev->submitted, 1000);
    if (!time) usb_kill_anchored_urbs(&dev->submitted);
}

static int ch37x_suspend(struct usb_interface *intf, pm_message_t message)
{
    struct usb_ch37x *dev = usb_get_intfdata(intf);

    if (!dev) return 0;
    ch37x_draw_down(dev);
    return 0;
}

static int ch37x_resume(struct usb_interface *intf)
{
    return 0;
}

static int ch37x_pre_reset(struct usb_interface *intf)
{
    struct usb_ch37x *dev = usb_get_intfdata(intf);

    mutex_lock(&dev->io_mutex);
    ch37x_draw_down(dev);

    return 0;
}

static int ch37x_post_reset(struct usb_interface *intf)
{
    struct usb_ch37x *dev = usb_get_intfdata(intf);

    /* we are sure no URBs are active - no locking needed */
    dev->errors = -EPIPE;
    mutex_unlock(&dev->io_mutex);

    return 0;
}

static struct usb_driver ch37x_driver = {
    .name = "ch37x",
    .probe = ch37x_probe,
    .disconnect = ch37x_disconnect,
    .suspend = ch37x_suspend,
    .resume = ch37x_resume,
    .pre_reset = ch37x_pre_reset,
    .post_reset = ch37x_post_reset,
    .id_table = ch37x_table,
    .supports_autosuspend = 1,
};

static int __init usb_ch37x_init(void)
{
    int ret;
    ret = usb_register(&ch37x_driver);
    if (ret) printk(KERN_ERR "usb_register failed. Error number: %d\n", ret);
    return ret;
}

static void __exit usb_ch37x_exit(void) { usb_deregister(&ch37x_driver); }

module_init(usb_ch37x_init);
module_exit(usb_ch37x_exit);
MODULE_LICENSE("GPL");
