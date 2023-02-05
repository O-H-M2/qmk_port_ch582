"""
This module provides some utility functions to locate a SEGGER JLINK dll. 

It is no longer used in LowLevel.API or MultiAPI.API, but maintained and tested in case it is still in use with third parties.
The internal JLinkARM install search algorithm that is currently used internally in nRF shared libraries is exposed by
the functions HighLevel.API.find_jlink() and LowLevel.API.find_jlink().

These functions expect a standard OS installation and a default installation path for the SEGGER shared library. If these 
conditions are not met the absolute path of the SEGGER JLinkARM shared library must be provided when instantiating an
API or MultiAPI object.
"""

try:
    from . import HighLevel
except Exception:
    import HighLevel

import warnings
warnings.warn("JLink module will be deprecated in version 11, use HighLevel or LowLevel module instead.", PendingDeprecationWarning)


def find_latest_dll():
    return HighLevel.API().find_jlink_path()
