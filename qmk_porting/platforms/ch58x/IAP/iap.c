#include "iap.h"

#undef pSetupReqPak /* 解决和外设库头文件冲突  */
#define pSetupReqPak ((PUSB_SETUP_REQ)EP0_Databuf)

#define DevEP0SIZE 64

// device descriptor
const uint8_t MyDevDescr[] = {
    0x12,       /* bLength */
    0x01,       /* bDescriptorType */
    0x10, 0x01, /* bcdUSB */
    0xFF,       /* bDeviceClass */
    0x80,       /* bDeviceSubClass */
    0x55,       /* bDeviceProtocol */
    DevEP0SIZE, /* bMaxPacketSize */
    0x48, 0x43, /* idVendor */
    0xe0, 0x55, /* idProduct */
    0x00, 0x01, /* bcdDevice */
    0x01,       /* iManufacturer */
    0x02,       /* iProduct */
    0x00,       /* iSerial */
    0x01        /* bNumConfigurations */
};
// configuration descriptor
const uint8_t MyCfgDescr[] = {
    0x09,       /* bLength */
    0x02,       /* bDescriptorType */
    0x20, 0x00, /* wTotalLength */
    0x01,       /* bNumInterfaces */
    0x01,       /* bConfigurationValue */
    0x00,       /* iConfiguration */
    0x80,       /* bmAttributes */
    0x39,       /* bMaxPower */

    0x09, /* bLength */
    0x04, /* bDescriptorType */
    0x00, /* bInterfaceNumber */
    0x00, /* bAlternateSetting */
    0x02, /* bNumEndpoints */
    0xFF, /* bInterfaceClass */
    0x80, /* bInterfaceSubClass */
    0x55, /* nInterfaceProtocol */
    0x00, /* iInterface: Index of string descriptor */

    0x07,       /* bLength */
    0x05,       /* bDescriptorType */
    0x82,       /* bEndpointAddress */
    0x02,       /* bmAttributes */
    0x40, 0x00, /* wMaxPacketSize */
    0x00,       /* bInterval */

    0x07,       /* bLength */
    0x05,       /* bDescriptorType */
    0x02,       /* bEndpointAddress */
    0x02,       /* bmAttributes */
    0x40, 0x00, /* wMaxPacketSize */
    0x00        /* bInterval */
};
const uint8_t MyLangDescr[] = { 0x04, 0x03, 0x09, 0x04 };
const uint8_t MyManuInfo[] = { 0x10, 0x03, 'H', 0, 'u', 0, 'c', 0, 'k', 0, 'i', 0, 'e', 0, 's', 0 };
const uint8_t MyProdInfo[] = { 0x12, 0x03, 'D', 0, 'F', 0, 'U', 0, ' ', 0, 'M', 0, 'o', 0, 'd', 0, 'e', 0 };

uint8_t DevConfig;
uint8_t SetupReqCode;
uint16_t SetupReqLen;
const uint8_t *pDescr;

__attribute__((aligned(4))) uint8_t EP0_Databuf[64 + 64 + 64]; //ep0(64)+ep4_out(64)+ep4_in(64)
__attribute__((aligned(4))) uint8_t EP1_Databuf[64 + 64];      //ep1_out(64)+ep1_in(64)
__attribute__((aligned(4))) uint8_t EP2_Databuf[64 + 64];      //ep2_out(64)+ep2_in(64)
__attribute__((aligned(4))) uint8_t EP3_Databuf[64 + 64];      //ep3_out(64)+ep3_in(64)

__attribute__((aligned(4))) uint8_t g_write_buf[256 + 64]; //每次满256字节再写flash，提升速度

volatile uint32_t g_buf_write_ptr = 0;
volatile uint32_t g_flash_write_ptr = 0;
uint32_t g_tcnt;
__attribute__((aligned(4))) iap_cmd_t g_iap_cmd;

/*********************************************************************
 * @fn      USB_DevTransProcess
 *
 * @brief   IAP USB主循环,程序放ram中运行，提升速度.
 *
 * @param   None.
 *
 * @return  None.
 */
__HIGH_CODE void USB_DevTransProcess(void)
{
    uint8_t len, chtype;
    uint8_t intflag, errflag = 0;

    intflag = R8_USB_INT_FG;
    if (intflag & RB_UIF_TRANSFER) {
        g_tcnt = 0; //USB有数据，清空超时计数
        if (intflag & RB_U_IS_NAK) {
        } else {
            // 分析操作令牌和端点号
            switch (R8_USB_INT_ST & (MASK_UIS_TOKEN | MASK_UIS_ENDP)) {
                case UIS_TOKEN_IN | 2:
                    R8_UEP2_CTRL = (R8_UEP2_CTRL & ~MASK_UEP_T_RES) | UEP_T_RES_NAK;
                    break;
                case UIS_TOKEN_OUT | 2: {
                    if (R8_USB_INT_ST & RB_UIS_TOG_OK) {
                        // 不同步的数据包将丢弃
                        len = R8_USB_RX_LEN;
                        my_memcpy(g_iap_cmd.other.buf, EP2_Databuf, len);
                        myDevEP2_OUT_Deal(len);
                    }
                } break;

                case UIS_TOKEN_IN | 1:
                    R8_UEP1_CTRL = (R8_UEP1_CTRL & ~MASK_UEP_T_RES) | UEP_T_RES_NAK;
                    break;

                case UIS_TOKEN_IN: {
                    switch (SetupReqCode) {
                        case USB_GET_DESCRIPTOR:
                            len = SetupReqLen >= DevEP0SIZE ? DevEP0SIZE : SetupReqLen; // 本次传输长度
                            my_memcpy(EP0_Databuf, pDescr, len);                        /* 加载上传数据 */
                            SetupReqLen -= len;
                            pDescr += len;
                            R8_UEP0_T_LEN = len;
                            R8_UEP0_CTRL ^= RB_UEP_T_TOG; // 翻转
                            break;
                        case USB_SET_ADDRESS:
                            R8_USB_DEV_AD = (R8_USB_DEV_AD & RB_UDA_GP_BIT) | SetupReqLen;
                            R8_UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
                            break;
                        default:
                            R8_UEP0_T_LEN = 0; // 状态阶段完成中断或者是强制上传0长度数据包结束控制传输
                            R8_UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
                            break;
                    }
                } break;

                case UIS_TOKEN_OUT:
                    //len = R8_USB_RX_LEN;
                    R8_UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
                    break;
                default:
                    break;
            }
            R8_USB_INT_FG = RB_UIF_TRANSFER;
        }
        if (R8_USB_INT_ST & RB_UIS_SETUP_ACT) // Setup包处理
        {
            R8_UEP0_CTRL = RB_UEP_R_TOG | RB_UEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_NAK;
            SetupReqLen = pSetupReqPak->wLength;
            SetupReqCode = pSetupReqPak->bRequest;
            chtype = pSetupReqPak->bRequestType;

            len = 0;
            errflag = 0;
            if ((pSetupReqPak->bRequestType & USB_REQ_TYP_MASK) != USB_REQ_TYP_STANDARD) {
                errflag = 0xFF; /* 非标准请求 */
            } else              /* 标准请求 */
            {
                switch (SetupReqCode) {
                    case USB_GET_DESCRIPTOR: {
                        switch (((pSetupReqPak->wValue) >> 8)) {
                            case USB_DESCR_TYP_DEVICE: {
                                pDescr = MyDevDescr;
                                len = MyDevDescr[0];
                            } break;

                            case USB_DESCR_TYP_CONFIG: {
                                pDescr = MyCfgDescr;
                                len = MyCfgDescr[2];
                            } break;

                            case USB_DESCR_TYP_STRING: {
                                switch ((pSetupReqPak->wValue) & 0xff) {
                                    case 1:
                                        pDescr = MyManuInfo;
                                        len = MyManuInfo[0];
                                        break;
                                    case 2:
                                        pDescr = MyProdInfo;
                                        len = MyProdInfo[0];
                                        break;
                                    case 0:
                                        pDescr = MyLangDescr;
                                        len = MyLangDescr[0];
                                        break;
                                    default:
                                        errflag = 0xFF; // 不支持的字符串描述符
                                        break;
                                }
                            } break;

                            default:
                                errflag = 0xff;
                                break;
                        }
                        if (SetupReqLen > len) {
                            SetupReqLen = len; //实际需上传总长度
                        }
                        len = (SetupReqLen >= DevEP0SIZE) ? DevEP0SIZE : SetupReqLen;
                        my_memcpy(EP0_Databuf, pDescr, len);
                        pDescr += len;
                    } break;

                    case USB_SET_ADDRESS:
                        SetupReqLen = (pSetupReqPak->wValue) & 0xff;
                        break;

                    case USB_GET_CONFIGURATION:
                        EP0_Databuf[0] = DevConfig;
                        if (SetupReqLen > 1) {
                            SetupReqLen = 1;
                        }
                        break;

                    case USB_SET_CONFIGURATION:
                        DevConfig = (pSetupReqPak->wValue) & 0xff;
                        break;

                    case USB_CLEAR_FEATURE: {
                        if ((pSetupReqPak->bRequestType & USB_REQ_RECIP_MASK) == USB_REQ_RECIP_ENDP) // 端点
                        {
                            switch ((pSetupReqPak->wIndex) & 0xff) {
                                case 0x82:
                                    R8_UEP2_CTRL = (R8_UEP2_CTRL & ~(RB_UEP_T_TOG | MASK_UEP_T_RES)) | UEP_T_RES_NAK;
                                    break;
                                case 0x02:
                                    R8_UEP2_CTRL = (R8_UEP2_CTRL & ~(RB_UEP_R_TOG | MASK_UEP_R_RES)) | UEP_R_RES_ACK;
                                    break;
                                case 0x81:
                                    R8_UEP1_CTRL = (R8_UEP1_CTRL & ~(RB_UEP_T_TOG | MASK_UEP_T_RES)) | UEP_T_RES_NAK;
                                    break;
                                case 0x01:
                                    R8_UEP1_CTRL = (R8_UEP1_CTRL & ~(RB_UEP_R_TOG | MASK_UEP_R_RES)) | UEP_R_RES_ACK;
                                    break;
                                default:
                                    errflag = 0xFF; // 不支持的端点
                                    break;
                            }
                        } else {
                            errflag = 0xFF;
                        }
                    } break;

                    case USB_GET_INTERFACE:
                        EP0_Databuf[0] = 0x00;
                        if (SetupReqLen > 1) {
                            SetupReqLen = 1;
                        }
                        break;

                    case USB_GET_STATUS:
                        EP0_Databuf[0] = 0x00;
                        EP0_Databuf[1] = 0x00;
                        if (SetupReqLen > 2) {
                            SetupReqLen = 2;
                        }
                        break;

                    default:
                        errflag = 0xff;
                        break;
                }
            }
            if (errflag == 0xff) // 错误或不支持
            {
                //                  SetupReqCode = 0xFF;
                R8_UEP0_CTRL = RB_UEP_R_TOG | RB_UEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL; // STALL
            } else {
                if (chtype & 0x80) // 上传
                {
                    len = (SetupReqLen > DevEP0SIZE) ? DevEP0SIZE : SetupReqLen;
                    SetupReqLen -= len;
                } else {
                    len = 0; // 下传
                }
                R8_UEP0_T_LEN = len;
                R8_UEP0_CTRL = RB_UEP_R_TOG | RB_UEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK; // 默认数据包是DATA1
            }
        }
        R8_USB_INT_FG = RB_UIF_TRANSFER;
    } else if (intflag & RB_UIF_BUS_RST) {
        R8_USB_DEV_AD = 0;
        R8_UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
        R8_UEP1_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK | RB_UEP_AUTO_TOG;
        R8_UEP2_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK | RB_UEP_AUTO_TOG;
        R8_UEP3_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK | RB_UEP_AUTO_TOG;
        R8_USB_INT_FG = RB_UIF_BUS_RST;
    } else if (intflag & RB_UIF_SUSPEND) {
        // 唤醒
        R8_USB_INT_FG = RB_UIF_SUSPEND;
    } else {
        R8_USB_INT_FG = intflag;
    }
}

/*********************************************************************
 * @fn      myDevEP2_OUT_Deal
 *
 * @brief   IAP USB数据处理函数，放入ram中运行提升速度.
 *
 * @param   None.
 *
 * @return  None.
 */
__HIGH_CODE void myDevEP2_OUT_Deal(uint8_t l)
{
    /* 用户可自定义 */
    uint8_t s = 0;
    uint32_t addr;

    switch (g_iap_cmd.other.buf[0]) {
        case CMD_IAP_PROM:
            if (g_iap_cmd.program.len == 0) {
                if (g_buf_write_ptr != 0) {
                    g_buf_write_ptr = ((g_buf_write_ptr + 3) & (~3)); //四字节对齐
                    s = FLASH_ROM_WRITE(g_flash_write_ptr, (PUINT32)g_write_buf, g_buf_write_ptr);
                    g_buf_write_ptr = 0;
                }
            } else {
                my_memcpy(g_write_buf + g_buf_write_ptr, g_iap_cmd.program.buf, g_iap_cmd.program.len);
                g_buf_write_ptr += g_iap_cmd.program.len;
                if (g_buf_write_ptr >= 256) {
                    s = FLASH_ROM_WRITE(g_flash_write_ptr, (PUINT32)g_write_buf, 256);
                    g_flash_write_ptr += 256;
                    g_buf_write_ptr = g_buf_write_ptr - 256;                    //超出的长度
                    my_memcpy(g_write_buf, g_write_buf + 256, g_buf_write_ptr); //保存剩下的iap_cmd.program.buf + g_iap_cmd.program.len - g_buf_write_ptr
                }
            }
            myDevEP2_IN_Deal(s);
            break;
        case CMD_IAP_ERASE:
            //这里可以添加地址判断，也可以直接擦除指定位置
            addr = (g_iap_cmd.erase.addr[0] | (uint32_t)g_iap_cmd.erase.addr[1] << 8 | (uint32_t)g_iap_cmd.erase.addr[2] << 16 | (uint32_t)g_iap_cmd.erase.addr[3] << 24);
            if (addr == APP_CODE_START_ADDR) {
                s = FLASH_ROM_ERASE(APP_CODE_START_ADDR, APP_CODE_END_ADDR - APP_CODE_START_ADDR);
                g_buf_write_ptr = 0; //计数清零
                g_flash_write_ptr = APP_CODE_START_ADDR;
            } else {
                s = 0xfe;
            }
            myDevEP2_IN_Deal(s);
            break;
        case CMD_IAP_VERIFY:
            my_memcpy(g_write_buf, g_iap_cmd.verify.buf, g_iap_cmd.verify.len);
            addr = (g_iap_cmd.verify.addr[0] | (uint32_t)g_iap_cmd.verify.addr[1] << 8 | (uint32_t)g_iap_cmd.verify.addr[2] << 16 | (uint32_t)g_iap_cmd.verify.addr[3] << 24);
            s = FLASH_ROM_VERIFY(addr, g_write_buf, g_iap_cmd.verify.len);
            myDevEP2_IN_Deal(s);
            break;
        case CMD_IAP_END:
            /*结束升级，复位USB，跳转到app*/
            jumpPre;
            jumpApp();
            break;
        default:
            myDevEP2_IN_Deal(0xfe);
            break;
    }
}

/*******************************************************************************
 * Function Name  : myDevEP2_IN_Deal
 * Description    : 端点2数据上传
 * Input          : l: 上传数据长度(<64B)
 * Return         : None
 *******************************************************************************/
__HIGH_CODE void myDevEP2_IN_Deal(uint8_t s)
{
    EP2_Databuf[64] = s;
    EP2_Databuf[65] = 0;
    R8_UEP2_T_LEN = 2;
    R8_UEP2_CTRL = (R8_UEP2_CTRL & ~MASK_UEP_T_RES) | UEP_T_RES_ACK; //enable send
}

/*********************************************************************
 * @fn      my_memcpy
 *
 * @brief   数据拷贝函数,程序放ram中运行，提升速度
 *
 * @param   None.
 *
 * @return  None.
 */
__HIGH_CODE void my_memcpy(void *dst, const void *src, uint32_t l)
{
    uint32_t len = l;
    PUINT8 pdst = (PUINT8)dst;
    PUINT8 psrc = (PUINT8)src;
    while (len) {
        *pdst++ = *psrc++;
        len--;
    }
}
