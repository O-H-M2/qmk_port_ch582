""" 

    This file contains example code meant to be used in order to showcase the 
    pynrfjprog callback-based asynchronous RTT functionality.
     
    Sample program: rtt_callback.py
    
    Run from command line:  
        python rtt_callback.py  
    or if imported using "from pynrfjprog import examples"
        examples.rtt_callback.run(682000044)
    
    Program flow:
        0. An API object is instantiated with AUTO family to read the device family.
        1. A connection to the debug probe is established.
        2. The detected family is loaded using select_family()
        3. An exapmle rtt application is programmed
        4. RTT is started
        5. "Hello world!" is printed to channel 1
        6. The echoed data is read from channel 1 and printed to terminal

"""

from __future__ import print_function
import argparse
import threading
import time

# Import pynrfjprog API module
try:
    from .. import LowLevel
except ImportError:
    from pynrfjprog import LowLevel

from . import hex_files

read_event = threading.Event()


def read_callback(channel_index, data, _):
    print(f"# Received {data.decode('utf-8')}")
    read_event.set()


def run(snr=None):
    """
    Run example script.

    @param int snr: Specify serial number of DK to run example on.
    """
    print("# Callback-based asynchronous RTT example using pynrfjprog started...")

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

        while not api.rtt_is_control_block_found():
            time.sleep(0.5)

        api.rtt_async_callback_start(channel_index=1, max_read_data_len=100, read_callback_func=read_callback)

        print(f"# Send Hello World!")
        api.rtt_async_write(channel_index=1, msg="Hello World!")

        # Wait for read callback to happen
        read_event.wait(2)

        api.rtt_async_callback_stop(channel_index=1)

    print("# Example done...")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-s", "--serial", type=int, help="Serial number to test.")
    args = parser.parse_args()

    run(args.serial)
