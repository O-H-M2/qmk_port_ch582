""" 

    This file contains example code meant to be used in order to test the 
    pynrfjprog API and Hex modules. If multiple devices are connected, a 
    pop-up will appear for device selection.
    
    Sample program: program_hex.py
    Requires an nRF Development Kit for visual confirmation (LEDs).
    
    Run from command line:  
        python program_hex.py  
    or if imported as "from pynrfjprog import examples"
        examples.program_hex.run()
    
    Program flow:
        0. An API object is instantiated with AUTO family to read the device family.
        1. An API object is instantiated with the read family. A connection to the debug probe is established.
        2. Flash memory is erased.
        3. A hex file is parsed and programmed into memory.
        4. The device is reset and application is run.

"""

from __future__ import print_function
import os

# Import pynrfjprog API module and HEX parser module
try:
    from .. import LowLevel, Hex
except ImportError:
    from pynrfjprog import LowLevel, Hex

from . import hex_files


def run(snr=None):
    """
    Run example script.

    @param (optional) int snr: Specify serial number of DK to run example on.
    """
    print("# Hex file programming example using pynrfjprog started...")

    # We will detect the device family of your device. Initialize an API object with AUTO family.
    print("# Opening API with device family AUTO, reading the device family.")

    with LowLevel.API() as api:
        if snr is not None:
            api.connect_to_emu_with_snr(snr)
        else:
            api.connect_to_emu_without_snr()

        # Read the device's family.
        # This step is performed so this example can be run in all devices without customer input.
        device_family = api.read_device_family()
        device_version = api.read_device_version()

        hex_file_path = hex_files.find_blinky_hex(device_family, device_version)

        # Erase all the flash of the device.
        print("# Erase only flash sectors overlaping image.")
        api.erase_file(file_path=hex_file_path, chip_erase_mode=LowLevel.EraseAction.ERASE_SECTOR)

        print(f"# Programming {hex_file_path}")
        api.program_file(file_path=hex_file_path)

        # Reset the device and run.
        api.sys_reset()
        api.go()
        print("# Application running. Your board should be blinking.")

        # Close the loaded DLL to free resources.
        api.close()

        print("# Example done...")


if __name__ == "__main__":
    run()
