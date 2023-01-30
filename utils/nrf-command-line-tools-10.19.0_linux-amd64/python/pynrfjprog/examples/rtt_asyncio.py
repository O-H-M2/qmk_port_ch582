""" 

    This file contains example code meant to be used in order to showcase the 
    pynrfjprog AsyncIO RTT functionality.
     
    Sample program: rtt_asyncio.py
    
    Run from command line:  
        python rtt_asyncio.py  
    or if imported using "from pynrfjprog import examples"
        asyncio.run(examples.rtt_asyncio.run(682000044))
    
    Program flow:
        0. An API object is instantiated with AUTO family to read the device family.
        1. A connection to the debug probe is established.
        2. The detected family is loaded using select_family()
        3. An exapmle rtt application is programmed
        4. An asyncio RTT channel is created
        5. "Hello world!" is printed to channel 1
        6. The echoed data is read from channel 1 and printed to terminal

"""

from __future__ import print_function
import argparse
import asyncio

# Import pynrfjprog API module
try:
    from .. import LowLevel
except ImportError:
    from pynrfjprog import LowLevel

from . import hex_files


async def test_rtt(api, channel_index):
    while not api.rtt_is_control_block_found():
        asyncio.sleep(0.5)

    # Test RTT echo functionality on channel 1
    reader, writer = await api.rtt_asyncio_open_connection(channel_index)

    print(f"# Send Hello World!")

    writer.write("Hello World!")

    data = await reader.read(n=100)
    print(f"# Received {data.decode()}")

    writer.close()

    await writer.wait_closed()


def run(snr=None):
    """
    Run example script.

    @param int snr: Specify serial number of DK to run example on.
    """
    print("# AsyncIO RTT example using pynrfjprog started...")

    with LowLevel.API() as api:
        # Connect to and identify device
        if snr is None:
            api.connect_to_emu_without_snr()
        else:
            api.connect_to_emu_with_snr(snr)

        family = api.read_device_family()
        device_version = api.read_device_version()

        # Program and start RTT firmware
        hex_file_path = hex_files.find_rtt_hex(family, device_version)

        print(f"# Programming {hex_file_path}")

        api.erase_all()
        api.program_file(hex_file_path)

        api.sys_reset()
        api.go()

        api.rtt_start()

        asyncio.run(test_rtt(api, channel_index=1))

    print("# Example done...")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-s", "--serial", type=int, help="Serial number to test.")
    args = parser.parse_args()

    run(args.serial)
