"""

    This file contains an example of how to use pynrfjprog for doing a modem firmware upgrade on Thingy:91
    over a serial port. The Thingy:91 does not feature an on-board debugger and can't directly use the existing modem
    DFU tool (IPCDFUProbe). The upgrade is done in two steps. First, the nrf9160 is programmed with an application
    that accepts modem firmware data over UART. Next, the modem firmware is uploaded by communicating with
    the application.

    The Thingy:91 has an USB<->UART bridge that exposes two of nrf9160's UART pairs. MCUBoot (the bootloader) uses
    one of the UART pairs at 115200 baudrate, and is used to flash our application. The other pair is configured
    to 1Mbaud, and is used to upload the modem firmware. Note that the nrf52840 UART bridge might need a firmware
    upgrade for this example to function properly. The Thingy:91 board documentation has instructions for how
    to do this.

    There are three prerequisites for this example:
    1) The nRF9160 device must be in serial recovery mode (ready for application flash) when starting
    the example. For Thingy:91, this is done by holding the reset button while toggling the power switch.

    2) You must know the two serial ports used by the nRF9160 device. The "nRF Connect --> Programmer" desktop
     application might be of aid here. Keep in mind the enumeration of the serial ports on your computer does not
     necessarily correlate with UART enumeration, i.e. UART0 can show up as a higher serial port number than UART1.

    3) The path to a zip file containing the modem firmware files must be provided. The zip folder can obtained from
    Nordic Semiconductors official websites. Note that your current version of the modem firmware might have downgrade
    protection, hence, a programming attempt can sometimes fail due to uploading outdated files.

    Sample program: nrf9160_pca20035_modem_upgrade_over_serial.py

    Run from command line:
        python nrf9160_pca20035_modem_upgrade_over_serial.py \
                <serial_device_name_uart0> \
                <serial_device_name_uart1> \
                <nrf9160_modem_firmware_zip_location>
    or if imported as "from pynrfjprog import examples"
        examples.nrf9160_pca20035_modem_upgrade_over_serial.run(
                <serial_device_name_uart0>,
                <serial_device_name_uart1>,
                <nrf9160_modem_firmware_zip_location>)

    Program flow:
        0. Configure logging to see the output of HighLevel API operations.
        1. Create a HighLevel API instance.
        2. Program nRF9160 with 'nrf9160_pca20035_firmware_upgrade_app' using an MCUBootDFUProbe probe.
           This application will receive modem firmware data over serial communication and apply it to the device.
        3. Upload the modem firmware pointed to by 'nrf9160_modem_firmware_zip_location' using a
           ModemUARTDFUProbe probe.
        4. Print the time elapsed during modem firmware upload.

"""

from __future__ import print_function
import sys
import argparse
import logging
import time
import os

# Import pynrfjprog HighLevel API module
try:
    from .. import HighLevel
except ImportError:
    from pynrfjprog import HighLevel

from . import hex_files


def run(serial_device_name_uart0, serial_device_name_uart1, nrf9160_modem_firmware_zip_location):
    """
    Run example script.

    @param string serial_device_name_uart0:               Specify Thingy:91 UART0 serial port name.
    @param string serial_device_name_uart1:               Specify Thingy:91 UART1 serial port name.
    @param string nrf9160_modem_firmware_zip_location:    Specify the location of the modem firmware zip file
    """
    # Verify that the firmware zip exists
    if not os.path.exists(nrf9160_modem_firmware_zip_location):
        raise FileNotFoundError(
            "Invalid path to modem firmware zip file provided. Skipping nrf9160 modem device firmware upgrade example."
        )

    print("# nrf9160 modem firmware upgrade over serial port example started...")

    # Configure logging to see HighLevel API output
    logging.basicConfig(level=logging.INFO, format="%(message)s")

    hex_file_path = hex_files.find_thingy91_modem_update_hex()

    # Create our API instance
    with HighLevel.API() as api:
        # Program the nrf9160 device with the DFU server application using MCUBoot
        # This assumes the device is in serial recovery mode and is ready to be programmed
        print("Flashing nrf9160 modem DFU application")
        with HighLevel.MCUBootDFUProbe(api, serial_device_name_uart0) as mcuboot_probe:
            mcuboot_probe.program(hex_file_path)

        # Upload the modem firmware to the nrf9160 device
        start_time = time.time()
        with HighLevel.ModemUARTDFUProbe(api, serial_device_name_uart1) as modem_dfu_probe:
            modem_dfu_probe.program(nrf9160_modem_firmware_zip_location)
            modem_dfu_probe.verify(nrf9160_modem_firmware_zip_location)

        # Print upload time
        print("------------------------------------------------------------")
        print("The operation took {0:.2f} seconds ".format(time.time() - start_time))

    print("# Example done...")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("uart0", default=None, help="Serial device name for Thingy:91 UART0 (mcuboot@115200 baud)")
    parser.add_argument("uart1", default=None, help="Serial device name for Thingy:91 UART1")
    parser.add_argument("firmware", default=None, help="Path to nrf9160 modem firmware zip folder")
    args = parser.parse_args()
    run(args.uart0, args.uart1, args.firmware)
