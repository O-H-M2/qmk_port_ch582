""" 

    This file contains example code meant to be used in order to test the 
    pynrfjprog API and Hex modules. If multiple devices are connected, a 
    pop-up will appear for device selection.
    
    Sample program: program_hex.py
    Requires nRF Development Kit for visual confirmation (LEDs).
    
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
import argparse

# Import pynrfjprog API module and HEX parser module
try:
    from .. import HighLevel
except ImportError:
    from pynrfjprog import HighLevel

from . import hex_files


def run(snr):
    """
    Run example script.

    @param int snr: Specify serial number of DK to run example on.
    """
    print("# Hex file programming example using pynrfjprog started...")

    print("# Opening High Level API instance and initializing a probe handle.")
    # Initialize an API object.
    # Open the loaded DLL and connect to an emulator probe.
    with HighLevel.API() as api:
        # Initialize the probe connection. The device family is automatically detected, and the correct sub dll is loaded.
        with HighLevel.DebugProbe(api, snr) as probe:

            print("# Reading out device information.")
            # Read out device information to find out which hex file to program
            device_info = probe.get_device_info()
            hex_file_path = hex_files.find_blinky_hex(device_info.device_family.name, device_info.device_type.name)

            # Erase all the flash of the device.
            print("# Programming %s to device with ERASE_ALL and SYSTEM_RESET." % hex_file_path)

            # Make a program option struct to modify programming sequence.
            # If ommitted, the default programoptions struct specifies ERASE_ALL, SYSTEM_RESET, and VERIFY_NONE.
            program_options = HighLevel.ProgramOptions(
                erase_action=HighLevel.EraseAction.ERASE_ALL,
                reset=HighLevel.ResetAction.RESET_SYSTEM,
                verify=HighLevel.VerifyAction.VERIFY_READ,
            )

            probe.program(hex_file_path, program_options=program_options)

            print("# Application running. Your board should be blinking.")

    print("# Example done...")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-s", "--serial", type=int, help="Serial number to test.")
    args = parser.parse_args()

    run(args.serial)
