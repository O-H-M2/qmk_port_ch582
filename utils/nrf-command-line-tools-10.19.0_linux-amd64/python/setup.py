from setuptools import setup
from setuptools.command.install import install
from distutils import log
import os
import stat
import pynrfjprog 

class PostInstall(install):

    def run(self):
        # Perform normal install
        install.run(self)
        
        # Post-install scripts
        try:
            root_path = os.path.join(self.install_lib,"pynrfjprog")
            for relative_lib_path in [x for x in os.listdir(root_path) if x.startswith("lib")]:
                lib_path = os.path.join(root_path,relative_lib_path)
                for file in os.listdir(lib_path):
                    file_abs_path = os.path.join(lib_path,file)
                    if file.startswith("jlinkarm_nrf_worker"):
                        log.info(f"Mark {relative_lib_path}/{file} as executable")
                        st = os.stat(file_abs_path)
                        os.chmod(file_abs_path, st.st_mode | stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH)
        except Exception:
            log.warn("Unable to set the executable bit on jlinkarm_nrf_worker")

setup(
    name='pynrfjprog',
    version=pynrfjprog.__version__,
    package_data={
        'pynrfjprog.lib_x86': ['*.dll', '*.so*', '*.dylib*', 'jlinkarm_nrf_worker*'],
        'pynrfjprog.lib_armhf': ['*.dll', '*.so*', '*.dylib*', 'jlinkarm_nrf_worker*'],
        'pynrfjprog.lib_arm64': ['*.dll', '*.so*', '*.dylib*', 'jlinkarm_nrf_worker*'],
        'pynrfjprog.lib_x64': ['*.dll', '*.so*', '*.dylib*', 'jlinkarm_nrf_worker*'],
        'pynrfjprog.docs': ['*.h', 'nrfjprog_release_notes*.txt'],
        'pynrfjprog.examples.hex_files': ['*.hex']
    },
    cmdclass={'install': PostInstall}
)
