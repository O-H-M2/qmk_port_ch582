""" 

    This file contains example code meant to be used in order to test the 
    pynrfjprog High Level API.
     
    Sample program: memory_read_write.py
    
    Run from command line:  
        python memory_read_write.py  
    or if imported using "from pynrfjprog import examples"
        examples.memory_read_write.run(682000044)
    
    Program flow:
        0. An API object is instantiated.
        1. A probe object is instantiated.
        2. Flash memory is erased.
        3. 0xDEADBEEF is written to address 0x0.
        4. 0xBAADF00D is written to address 0x10.
        5. Memory 0x0 and 0x10 is read and printed to console.
        6. The probe object is destroyed.
        7. The API is closed.

"""

from __future__ import print_function
import argparse

# Import pynrfjprog API module
try:
    from .. import HighLevel
except ImportError:
    from pynrfjprog import HighLevel


def run(snr):
    """
    Run example script.

    @param int snr: Specify serial number of DK to run example on.
    """
    print("# Memory read and write example using pynrfjprog started...")

    print("# Opening High Level API instance and initializing a probe handle.")
    # Initialize an API object.
    # Open the loaded DLL and connect to an emulator probe.
    with HighLevel.API() as api:
        # Initialize the probe connection. The device family is automatically detected, and the correct sub dll is loaded.
        with HighLevel.DebugProbe(api, snr) as probe:
            # Erase all the flash of the device.
            print("# Erasing all flash in the microcontroller.")
            # api.erase supports several erase actions, including sector erase, erase all, and qspi erase options.
            probe.erase(HighLevel.EraseAction.ERASE_ALL)

            # Write to addresses 0x0 and 0x10. NVMC is used automatically whenever a write to flash is requested.
            print("# Writing 0xDEADBEEF to memory address 0x0.")
            probe.write(0x0, 0xDEADBEEF)
            print("# Writing 0xBAADF00D to memory address 0x10.")
            probe.write(0x10, 0xBAADF00D)

            # Read from addresses 0x0 and 0x10 and print the result.
            print("# Reading memory address 0x0 and 0x10, and print to console")
            print("Address 0x0 contains: ", hex(probe.read(0x0)))
            print("Address 0x10 contains: ", hex(probe.read(0x10)))

    print("# Example done...")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-s", "--serial", type=int, help="Serial number to test.")
    args = parser.parse_args()

    run(args.serial)
