import os


def find_device_hex(device_family, device_version, suffix):
    """Find the appropriate hex file to program"""

    module_dir, _ = os.path.split(__file__)
    device_version_short = device_version.split("_")[0]
    hex_file_path = os.path.join(os.path.abspath(module_dir), device_version_short.lower() + suffix + ".hex")

    if os.path.exists(hex_file_path):
        return hex_file_path

    hex_file_path = os.path.join(os.path.abspath(module_dir), device_family.lower() + suffix + ".hex")

    if os.path.exists(hex_file_path):
        return hex_file_path

    raise FileNotFoundError(
        f"Could not find example binary for device {device_version.lower()}.\n\
        This example may not support your device yet."
    )


def find_blinky_hex(device_family, device_version):
    """Find the blinky program for the specified device"""
    return find_device_hex(device_family, device_version, "_dk_blinky")


def find_rtt_hex(device_family, device_version):
    """Find the RTT program for the specified device"""
    return find_device_hex(device_family, device_version, "_dk_rtt")


def find_thingy91_modem_update_hex():
    """Find the modem update fw for the thingy91"""
    module_dir, _ = os.path.split(__file__)
    return os.path.join(os.path.abspath(module_dir), "nrf9160_pca20035_firmware_upgrade_app_0.1.1.hex")
