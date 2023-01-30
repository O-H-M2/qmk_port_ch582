""" 

    This file contains example code meant to be used in order to test the 
    pynrfjprog API.
    
    Sample program: python_help.py
    
    Run from command line:  
        python python_help.py  
    or if imported as "from pynrfjprog import examples"
        examples.python_help.run()
    
    Program flow:
        0. Python help() is used to list the available API functions

"""

from __future__ import print_function

# Import pynrfjprog API module
try:
    from .. import LowLevel
except ImportError:
    from pynrfjprog import LowLevel


def run():
    print("# Python help() example started...")

    # Use the python help() to list the available information regarding the API module
    help(LowLevel)

    print("# Example done...")


if __name__ == "__main__":
    run()
