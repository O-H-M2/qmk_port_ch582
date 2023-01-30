""" Examples for pynrfjprog. 
    Each individual example can be used by importing examples as:
    from pynrfjprog import examples;  then calling:
    examples.python_help.run() """

try:
    from . import python_help
    from . import program_hex
    from . import memory_read_write
    from . import highlevel_program_hex
    from . import highlevel_memory_read_write
    from . import nrf9160_pca20035_modem_upgrade_over_serial
    from . import rtt_asyncio
    from . import rtt_callback
    from . import rtt_synchronous

    from .hex_files import *

except Exception:
    import python_help
    import program_hex
    import memory_read_write
    import highlevel_program_hex
    import highlevel_memory_read_write
    import nrf9160_pca20035_modem_upgrade_over_serial
    import rtt_asyncio
    import rtt_callback
    import rtt_synchronous

    from .hex_files import *
