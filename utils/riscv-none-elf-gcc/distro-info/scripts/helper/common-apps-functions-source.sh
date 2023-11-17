# -----------------------------------------------------------------------------
# This file is part of the xPack distribution.
#   (https://xpack.github.io)
# Copyright (c) 2020 Liviu Ionescu.
#
# Permission to use, copy, modify, and/or distribute this software
# for any purpose is hereby granted, under the terms of the MIT license.
# -----------------------------------------------------------------------------

# Helper script used in xPack Developer Tools build scripts.
# As the name implies, it should contain only functions and
# should be included with 'source' by the build scripts (both native
# and container).

# -----------------------------------------------------------------------------

function build_patchelf()
{
  # https://nixos.org/patchelf.html
  # https://github.com/NixOS/patchelf
  # https://github.com/NixOS/patchelf/releases/
  # https://github.com/NixOS/patchelf/releases/download/0.12/patchelf-0.12.tar.bz2
  # https://github.com/NixOS/patchelf/archive/0.12.tar.gz

  # 2016-02-29, "0.9"
  # 2019-03-28, "0.10"
  # 2020-06-09, "0.11"
  # 2020-08-27, "0.12"

  local patchelf_version="$1"

  local patchelf_src_folder_name="patchelf-${patchelf_version}"

  # local patchelf_archive="${patchelf_src_folder_name}.tar.bz2"
  # GitHub release archive.
  local patchelf_github_archive="${patchelf_version}.tar.gz"
  local patchelf_github_url="https://github.com/NixOS/patchelf/archive/${patchelf_github_archive}"

  local patchelf_folder_name="${patchelf_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${patchelf_folder_name}"

  local patchelf_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${patchelf_folder_name}-installed"
  if [ ! -f "${patchelf_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${patchelf_github_url}" "${patchelf_github_archive}" \
      "${patchelf_src_folder_name}"

    (
      if [ ! -x "${SOURCES_FOLDER_PATH}/${patchelf_src_folder_name}/configure" ]
      then

        cd "${SOURCES_FOLDER_PATH}/${patchelf_src_folder_name}"

        xbb_activate_installed_dev

        run_verbose bash ${DEBUG} "bootstrap.sh"

      fi
    ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${patchelf_folder_name}/autogen-output-$(ndate).txt"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${patchelf_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${patchelf_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_APP_STATIC_GCC}"

      if [ "${TARGET_PLATFORM}" == "linux" ]
      then
        LDFLAGS+=" -Wl,-rpath,${LD_LIBRARY_PATH}"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      if [ ! -f "config.status" ]
      then
        (
          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          echo
          echo "Running patchelf configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${patchelf_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${patchelf_src_folder_name}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${patchelf_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${patchelf_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running patchelf make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

        show_libs "${LIBS_INSTALL_FOLDER_PATH}/bin/patchelf"

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${patchelf_folder_name}/make-output-$(ndate).txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${patchelf_src_folder_name}" \
        "${patchelf_folder_name}"

    )

    (
      test_patchelf
    ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${patchelf_folder_name}/test-output-$(ndate).txt"

    touch "${patchelf_stamp_file_path}"

  else
    echo "Component patchelf already installed."
  fi
}

function test_patchelf()
{
  (
    xbb_activate

    echo
    echo "Checking the patchelf shared libraries..."

    show_libs "${LIBS_INSTALL_FOLDER_PATH}/bin/patchelf"

    echo
    echo "Checking if patchelf starts..."
    "${LIBS_INSTALL_FOLDER_PATH}/bin/patchelf" --version
    "${LIBS_INSTALL_FOLDER_PATH}/bin/patchelf" --help
  )
}

# -----------------------------------------------------------------------------

function build_automake()
{
  # https://www.gnu.org/software/automake/
  # https://ftp.gnu.org/gnu/automake/

  # https://archlinuxarm.org/packages/any/automake/files/PKGBUILD
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=automake-git

  # 2015-01-05, "1.15"
  # 2018-02-25, "1.16"
  # 2020-03-21, "1.16.2"
  # 2020-11-18, "1.16.3"

  local automake_version="$1"

  local automake_src_folder_name="automake-${automake_version}"

  local automake_archive="${automake_src_folder_name}.tar.xz"
  local automake_url="https://ftp.gnu.org/gnu/automake/${automake_archive}"

  local automake_folder_name="${automake_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${automake_folder_name}"

  # help2man: can't get `--help' info from automake-1.16
  # Try `--no-discard-stderr' if option outputs to stderr

  local automake_patch_file_path="${automake_folder_name}.patch"
  local automake_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${automake_folder_name}-installed"
  if [ ! -f "${automake_stamp_file_path}" -o ! -d "${BUILD_FOLDER_PATH}/${automake_folder_name}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${automake_url}" "${automake_archive}" \
      "${automake_src_folder_name}" \
      "${automake_patch_file_path}"

    (
      mkdir -pv "${BUILD_FOLDER_PATH}/${automake_folder_name}"
      cd "${BUILD_FOLDER_PATH}/${automake_folder_name}"

      xbb_activate_installed_dev

      export CPPFLAGS="${XBB_CPPFLAGS}"
      export CFLAGS="${XBB_CFLAGS_NO_W}"
      export CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      export LDFLAGS="${XBB_LDFLAGS_APP_STATIC_GCC}"

      if [ ! -f "config.status" ]
      then
        (
          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          echo
          echo "Running automake configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${automake_src_folder_name}/configure" --help
          fi

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${automake_src_folder_name}/configure" \
            --prefix="${LIBS_INSTALL_FOLDER_PATH}" \
            \
            --build="${BUILD}" \

          cp "config.log" "${LOGS_FOLDER_PATH}/${automake_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${automake_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running automake make..."

        # Build.
        run_verbose make -j ${JOBS}

        # make install-strip
        run_verbose make install

        # Takes too long and some tests fail.
        # XFAIL: t/pm/Cond2.pl
        # XFAIL: t/pm/Cond3.pl
        # ...
        if false
        then
          run_verbose make -j1 check
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${automake_folder_name}/make-output-$(ndate).txt"
    )

    (
      test_automake
    ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${automake_folder_name}/test-output-$(ndate).txt"

    hash -r

    touch "${automake_stamp_file_path}"

  else
    echo "Component automake already installed."
  fi

  test_functions+=("test_automake")
}

function test_automake()
{
  (
    xbb_activate_installed_bin

    echo
    echo "Testing if automake binaries start properly..."

    run_verbose "${LIBS_INSTALL_FOLDER_PATH}/bin/automake" --version
  )
}

# -----------------------------------------------------------------------------


function build_findutils()
{
  # https://www.gnu.org/software/findutils/
  # https://ftp.gnu.org/gnu/findutils/
  # https://ftp.gnu.org/gnu/findutils/findutils-4.8.0.tar.xz

  # 2021-01-09, "4.8.0"

  local findutils_version="$1"

  local findutils_src_folder_name="findutils-${findutils_version}"

  local findutils_archive="${findutils_src_folder_name}.tar.xz"
  local findutils_url="https://ftp.gnu.org/gnu/findutils/${findutils_archive}"

  local findutils_folder_name="${findutils_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${findutils_folder_name}"

  local findutils_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${findutils_folder_name}-installed"
  if [ ! -f "${findutils_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${findutils_url}" "${findutils_archive}" \
      "${findutils_src_folder_name}"

    (
      if [ ! -x "${SOURCES_FOLDER_PATH}/${findutils_src_folder_name}/configure" ]
      then

        cd "${SOURCES_FOLDER_PATH}/${findutils_src_folder_name}"

        xbb_activate_installed_dev

        run_verbose bash ${DEBUG} "bootstrap.sh"

      fi
    ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${findutils_folder_name}/autogen-output-$(ndate).txt"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${findutils_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${findutils_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_APP_STATIC_GCC}"

      if [ "${TARGET_PLATFORM}" == "linux" ]
      then
        LDFLAGS+=" -Wl,-rpath,${LD_LIBRARY_PATH}"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      if [ ! -f "config.status" ]
      then
        (
          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          echo
          echo "Running findutils configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${findutils_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          # config_options+=("--host=${HOST}")
          # config_options+=("--target=${TARGET}")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${findutils_src_folder_name}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${findutils_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${findutils_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running findutils make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

        show_libs "${LIBS_INSTALL_FOLDER_PATH}/bin/find"

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${findutils_folder_name}/make-output-$(ndate).txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${findutils_src_folder_name}" \
        "${findutils_folder_name}"

    )

    (
      test_findutils
    ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${findutils_folder_name}/test-output-$(ndate).txt"

    touch "${findutils_stamp_file_path}"

  else
    echo "Component findutils already installed."
  fi
}

function test_findutils()
{
  (
    xbb_activate

    echo
    echo "Checking the findutils shared libraries..."

    show_libs "${LIBS_INSTALL_FOLDER_PATH}/bin/find"

    echo
    echo "Checking if findutils starts..."
    "${LIBS_INSTALL_FOLDER_PATH}/bin/find" || true
  )
}

# =============================================================================

# Used to initialise options in all mingw builds:
# `config_options=("${config_options_common[@]}")`

function prepare_mingw_config_options_common()
{
  # ---------------------------------------------------------------------------
  # Used in multiple configurations.

  config_options_common=()

  local prefix=${APP_PREFIX}
  if [ $# -ge 1 ]
  then
    config_options_common+=("--prefix=$1")
  else
    echo "prepare_mingw_config_options_common requires a prefix path"
    exit 1
  fi

  config_options_common+=("--disable-multilib")

  # https://docs.microsoft.com/en-us/cpp/porting/modifying-winver-and-win32-winnt?view=msvc-160
  # Windows 7
  config_options_common+=("--with-default-win32-winnt=0x601")

  # `ucrt` is the new Microsoft C runtime.
  # https://support.microsoft.com/en-us/topic/update-for-universal-c-runtime-in-windows-c0514201-7fe6-95a3-b0a5-287930f3560c
  config_options_common+=("--with-default-msvcrt=${MINGW_MSVCRT:-ucrt}")
  # config_options_common+=("--with-default-msvcrt=${MINGW_MSVCRT:-msvcrt}")

  config_options_common+=("--enable-wildcard")
  config_options_common+=("--enable-warnings=0")
}

function prepare_mingw_env()
{
  export MINGW_VERSION="$1"
  export MINGW_NAME_SUFFIX=${2-''}

  export MINGW_VERSION_MAJOR=$(echo ${MINGW_VERSION} | sed -e 's|\([0-9][0-9]*\)\..*|\1|')

  # The original SourceForge location.
  export MINGW_SRC_FOLDER_NAME="mingw-w64-v${MINGW_VERSION}"
  export MINGW_FOLDER_NAME="${MINGW_SRC_FOLDER_NAME}${MINGW_NAME_SUFFIX}"
}

function build_mingw_headers()
{
  # http://mingw-w64.org/doku.php/start
  # https://sourceforge.net/projects/mingw-w64/files/mingw-w64/mingw-w64-release/

  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=mingw-w64-headers
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=mingw-w64-crt
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=mingw-w64-winpthreads
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=mingw-w64-binutils
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=mingw-w64-gcc

  # https://github.com/msys2/MINGW-packages/blob/master/mingw-w64-headers-git/PKGBUILD
  # https://github.com/msys2/MINGW-packages/blob/master/mingw-w64-crt-git/PKGBUILD
  # https://github.com/msys2/MINGW-packages/blob/master/mingw-w64-winpthreads-git/PKGBUILD
  # https://github.com/msys2/MINGW-packages/blob/master/mingw-w64-binutils/PKGBUILD
  # https://github.com/msys2/MINGW-packages/blob/master/mingw-w64-gcc/PKGBUILD

  # https://github.com/msys2/MSYS2-packages/blob/master/gcc/PKGBUILD

  # https://github.com/StephanTLavavej/mingw-distro

  # 2018-06-03, "5.0.4"
  # 2018-09-16, "6.0.0"
  # 2019-11-11, "7.0.0"
  # 2020-09-18, "8.0.0"
  # 2021-05-09, "8.0.2"
  # 2021-05-22, "9.0.0"
  # 2022-04-04, "10.0.0"

  local mingw_archive="${MINGW_SRC_FOLDER_NAME}.tar.bz2"
  local mingw_url="https://sourceforge.net/projects/mingw-w64/files/mingw-w64/mingw-w64-release/${mingw_archive}"

  # If SourceForge is down, there is also a GitHub mirror.
  # https://github.com/mirror/mingw-w64
  # MINGW_FOLDER_NAME="mingw-w64-${MINGW_VERSION}"
  # mingw_archive="v${MINGW_VERSION}.tar.gz"
  # mingw_url="https://github.com/mirror/mingw-w64/archive/${mingw_archive}"

  # https://sourceforge.net/p/mingw-w64/wiki2/Cross%20Win32%20and%20Win64%20compiler/
  # https://sourceforge.net/p/mingw-w64/mingw-w64/ci/master/tree/configure

  # For binutils/GCC, the official method to build the mingw-w64 toolchain
  # is to set --prefix and --with-sysroot to the same directory to allow
  # the toolchain to be relocatable.

  # Recommended GCC configuration:
  # (to disable multilib, add `--enable-targets="${TARGET}"`)
  #
  # $ ../gcc-trunk/configure --{host,build}=<build triplet> \
	# --target=x86_64-w64-mingw32 --enable-multilib --enable-64bit \
	# --{prefix,with-sysroot}=<prefix> --enable-version-specific-runtime-libs \
	# --enable-shared --with-dwarf --enable-fully-dynamic-string \
	# --enable-languages=c,ada,c++,fortran,objc,obj-c++ --enable-libgomp \
	# --enable-libssp --with-host-libstdcxx="-lstdc++ -lsupc++" \
	# --with-{gmp,mpfr,mpc,cloog,ppl}=<host dir> --enable-lto
  #
  # $ make all-gcc && make install-gcc
  #
  # build mingw-w64-crt (headers, crt, tools)
  #
  # $ make all-target-libgcc && make install-target-libgcc
  #
  # build mingw-libraries (winpthreads)
  #
  # Continue the GCC build (C++)
  # $ make && make install

  # ---------------------------------------------------------------------------

  # The 'headers' step creates the 'include' folder.

  local mingw_headers_folder_name="mingw-${MINGW_VERSION}-headers${MINGW_NAME_SUFFIX}"

  cd "${SOURCES_FOLDER_PATH}"

  download_and_extract "${mingw_url}" "${mingw_archive}" \
    "${MINGW_SRC_FOLDER_NAME}"

  # The docs recommend to add several links, but for non-multilib
  # configurations there are no target or lib32/lib64 specific folders.

  mkdir -pv "${LOGS_FOLDER_PATH}/${MINGW_FOLDER_NAME}"

  # ---------------------------------------------------------------------------

  # https://github.com/archlinux/svntogit-community/blob/packages/mingw-w64-headers/trunk/PKGBUILD

  local mingw_headers_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${mingw_headers_folder_name}-installed"
  if [ ! -f "${mingw_headers_stamp_file_path}" ]
  then
    (
      mkdir -p "${BUILD_FOLDER_PATH}/${mingw_headers_folder_name}"
      cd "${BUILD_FOLDER_PATH}/${mingw_headers_folder_name}"

      if [ ! -f "config.status" ]
      then
        (
          echo
          echo "Running mingw-w64-headers${MINGW_NAME_SUFFIX} configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${MINGW_SRC_FOLDER_NAME}/mingw-w64-headers/configure" --help
          fi

          if [ -n "${MINGW_NAME_SUFFIX}" ]
          then
            prepare_mingw_config_options_common "${APP_PREFIX}${MINGW_NAME_SUFFIX}"
            config_options=("${config_options_common[@]}")

            config_options+=("--build=${BUILD}")
            # The bootstrap binaries will run on the build machine.
            config_options+=("--host=${TARGET}")
            config_options+=("--target=${TARGET}")
          else
            prepare_mingw_config_options_common "${APP_PREFIX}${MINGW_NAME_SUFFIX}/${CROSS_COMPILE_PREFIX}"
            config_options=("${config_options_common[@]}")

            config_options+=("--build=${BUILD}")
            config_options+=("--host=${HOST}")
            config_options+=("--target=${TARGET}")
          fi

          config_options+=("--with-tune=generic")

          config_options+=("--enable-sdk=all")
          config_options+=("--enable-idl")
          config_options+=("--without-widl")

          # From Arch, but not recognised.
          # config_options+=("--enable-secure-api")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${MINGW_SRC_FOLDER_NAME}/mingw-w64-headers/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${MINGW_FOLDER_NAME}/config-headers-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${MINGW_FOLDER_NAME}/configure-headers-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running mingw-w64-headers${MINGW_NAME_SUFFIX} make..."

        # Build.
        run_verbose make -j ${JOBS}

        run_verbose make install-strip

        if [ -n "${MINGW_NAME_SUFFIX}" ]
        then
          mkdir -pv "${APP_PREFIX}${MINGW_NAME_SUFFIX}/${CROSS_COMPILE_PREFIX}"
          (
            cd "${APP_PREFIX}${MINGW_NAME_SUFFIX}/${CROSS_COMPILE_PREFIX}"
            run_verbose ln -sv ../include include
          )

          # This is this needed by the GCC bootstrap; otherwise:
          # The directory that should contain system headers does not exist:
          # /Host/home/ilg/Work/gcc-11.1.0-1/win32-x64/install/gcc-bootstrap/mingw/include

          rm -rf "${APP_PREFIX}${MINGW_NAME_SUFFIX}/mingw"
          (
            cd "${APP_PREFIX}${MINGW_NAME_SUFFIX}"
            run_verbose ln -sv "${CROSS_COMPILE_PREFIX}" "mingw"
          )
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${MINGW_FOLDER_NAME}/make-headers-output-$(ndate).txt"

      # No need to do it again for each component.
      if [ -z "${MINGW_NAME_SUFFIX}" ]
      then
        copy_license \
          "${SOURCES_FOLDER_PATH}/${MINGW_SRC_FOLDER_NAME}" \
          "${MINGW_FOLDER_NAME}"
      fi

    )

    touch "${mingw_headers_stamp_file_path}"

  else
    echo "Component mingw-w64-headers${MINGW_NAME_SUFFIX} already installed."
  fi
}

function build_mingw_crt()
{
  # ---------------------------------------------------------------------------

  # The 'crt' step creates the C run-time in the 'lib' folder.

  # https://github.com/archlinux/svntogit-community/blob/packages/mingw-w64-crt/trunk/PKGBUILD

  local mingw_crt_folder_name="mingw-${MINGW_VERSION}-crt${MINGW_NAME_SUFFIX}"

  local mingw_crt_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${mingw_crt_folder_name}-installed"
  if [ ! -f "${mingw_crt_stamp_file_path}" ]
  then
    (
      mkdir -p "${BUILD_FOLDER_PATH}/${mingw_crt_folder_name}"
      cd "${BUILD_FOLDER_PATH}/${mingw_crt_folder_name}"

      # -ffunction-sections -fdata-sections fail with:
      # Error: .seh_endproc used in segment '.text' instead of expected '.text$WinMainCRTStartup'
      CPPFLAGS=""
      CFLAGS="-O2 -pipe -w"
      CXXFLAGS="-O2 -pipe -w"

      LDFLAGS=""

      if [ "${IS_DEVELOP}" == "y" ]
      then
        LDFLAGS+=" -v"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      # Without it, apparently a bug in autoconf/c.m4, function AC_PROG_CC, results in:
      # checking for _mingw_mac.h... no
      # configure: error: Please check if the mingw-w64 header set and the build/host option are set properly.
      # (https://github.com/henry0312/build_gcc/issues/1)
      # export CC=""
      # Alternately it is possible to define CC to the mingw-gcc.

      if [ ! -f "config.status" ]
      then
        (
          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          echo
          echo "Running mingw-w64-crt${MINGW_NAME_SUFFIX} configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${MINGW_SRC_FOLDER_NAME}/mingw-w64-crt/configure" --help
          fi

          prepare_mingw_config_options_common "${APP_PREFIX}${MINGW_NAME_SUFFIX}/${CROSS_COMPILE_PREFIX}"
          config_options=("${config_options_common[@]}")
          config_options+=("--with-sysroot=${APP_PREFIX}${MINGW_NAME_SUFFIX}")

          if [ -n "${MINGW_NAME_SUFFIX}" ]
          then
            config_options+=("--build=${BUILD}")
            # The bootstrap binaries will run on the build machine.
            config_options+=("--host=${TARGET}")
            config_options+=("--target=${TARGET}")
          else
            config_options+=("--build=${BUILD}")
            config_options+=("--host=${HOST}")
            config_options+=("--target=${TARGET}")
          fi

          if [ "${TARGET_ARCH}" == "x64" ]
          then
            config_options+=("--disable-lib32")
            config_options+=("--enable-lib64")
          elif [ "${TARGET_ARCH}" == "x32" -o "${TARGET_ARCH}" == "ia32" ]
          then
            config_options+=("--enable-lib32")
            config_options+=("--disable-lib64")
          else
            echo "Oops! Unsupported TARGET_ARCH=${TARGET_ARCH}."
            exit 1
          fi

          config_options+=("--enable-wildcard")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${MINGW_SRC_FOLDER_NAME}/mingw-w64-crt/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${MINGW_FOLDER_NAME}/config-crt-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${MINGW_FOLDER_NAME}/configure-crt-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running mingw-w64-crt${MINGW_NAME_SUFFIX} make..."

        # Build.
        run_verbose make -j ${JOBS}

        run_verbose make install-strip

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${MINGW_FOLDER_NAME}/make-crt-output-$(ndate).txt"
    )

    touch "${mingw_crt_stamp_file_path}"

  else
    echo "Component mingw-w64-crt${MINGW_NAME_SUFFIX} already installed."
  fi
}


function build_mingw_winpthreads()
{
  # https://github.com/archlinux/svntogit-community/blob/packages/mingw-w64-winpthreads/trunk/PKGBUILD

  local mingw_winpthreads_folder_name="mingw-${MINGW_VERSION}-winpthreads${MINGW_NAME_SUFFIX}"

  local mingw_winpthreads_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${mingw_winpthreads_folder_name}-installed"
  if [ ! -f "${mingw_winpthreads_stamp_file_path}" ]
  then
    (
      mkdir -p "${BUILD_FOLDER_PATH}/${mingw_winpthreads_folder_name}"
      cd "${BUILD_FOLDER_PATH}/${mingw_winpthreads_folder_name}"

      # -ffunction-sections -fdata-sections fail with:
      # Error: .seh_endproc used in segment '.text' instead of expected '.text$WinMainCRTStartup'
      CPPFLAGS=""
      CFLAGS="-O2 -pipe -w"
      CXXFLAGS="-O2 -pipe -w"

      LDFLAGS=""

      if [ "${IS_DEVELOP}" == "y" ]
      then
        LDFLAGS+=" -v"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      if [ ! -f "config.status" ]
      then
        (
          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          echo
          echo "Running mingw-w64-winpthreads${MINGW_NAME_SUFFIX} configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${MINGW_SRC_FOLDER_NAME}/mingw-w64-libraries/winpthreads/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${APP_PREFIX}${MINGW_NAME_SUFFIX}/${CROSS_COMPILE_PREFIX}")
          config_options+=("--with-sysroot=${APP_PREFIX}${MINGW_NAME_SUFFIX}")

          config_options+=("--libdir=${APP_PREFIX}${MINGW_NAME_SUFFIX}/${CROSS_COMPILE_PREFIX}/lib")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          # This prevents references to libwinpthread-1.dll, which is
          # particularly useful with -static-libstdc++, otherwise the
          # result is not exactly static.
          # This also requires disabling shared in the GCC configuration.
          config_options+=("--disable-shared")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${MINGW_SRC_FOLDER_NAME}/mingw-w64-libraries/winpthreads/configure" \
            "${config_options[@]}"

         cp "config.log" "${LOGS_FOLDER_PATH}/${MINGW_FOLDER_NAME}/config-winpthreads-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${MINGW_FOLDER_NAME}/configure-winpthreads-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running mingw-w64-winpthreads${MINGW_NAME_SUFFIX} make..."

        # Build.
        run_verbose make -j ${JOBS}

        run_verbose make install-strip

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${MINGW_FOLDER_NAME}/make-winpthreads-output-$(ndate).txt"
    )

    touch "${mingw_winpthreads_stamp_file_path}"

  else
    echo "Component mingw-w64-winpthreads${MINGW_NAME_SUFFIX} already installed."
  fi
}

function build_mingw_winstorecompat()
{
  local mingw_winstorecompat_folder_name="mingw-${MINGW_VERSION}-winstorecompat${MINGW_NAME_SUFFIX}"

  local mingw_winstorecompat_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${mingw_winstorecompat_folder_name}-installed"
  if [ ! -f "${mingw_winstorecompat_stamp_file_path}" ]
  then

    (
      mkdir -p "${BUILD_FOLDER_PATH}/${mingw_winstorecompat_folder_name}"
      cd "${BUILD_FOLDER_PATH}/${mingw_winstorecompat_folder_name}"

      CPPFLAGS=""
      CFLAGS="-O2 -pipe -w"
      CXXFLAGS="-O2 -pipe -w"

      LDFLAGS=""

      if [ "${IS_DEVELOP}" == "y" ]
      then
        LDFLAGS+=" -v"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      if [ ! -f "config.status" ]
      then
        (
          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          echo
          echo "Running mingw-w64-winstorecompat${MINGW_NAME_SUFFIX} configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${MINGW_SRC_FOLDER_NAME}/mingw-w64-libraries/winstorecompat/configure" --help
          fi

          config_options=()
          # Note: native library.

          config_options+=("--prefix=${APP_PREFIX}${MINGW_NAME_SUFFIX}/${CROSS_COMPILE_PREFIX}")
          config_options+=("--libdir=${APP_PREFIX}${MINGW_NAME_SUFFIX}/${CROSS_COMPILE_PREFIX}/lib")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${MINGW_SRC_FOLDER_NAME}/mingw-w64-libraries/winstorecompat/configure" \
            "${config_options[@]}"

         cp "config.log" "${LOGS_FOLDER_PATH}/${MINGW_FOLDER_NAME}/config-winstorecompat-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${MINGW_FOLDER_NAME}/configure-winstorecompat-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running mingw-w64-winstorecompat${MINGW_NAME_SUFFIX} make..."

        # Build.
        run_verbose make -j ${JOBS}

        run_verbose make install-strip

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${MINGW_FOLDER_NAME}/make-winstorecompat-output-$(ndate).txt"
    )

    touch "${mingw_winstorecompat_stamp_file_path}"

  else
    echo "Component mingw-w64-winstorecompat${MINGW_NAME_SUFFIX} already installed."
  fi
}

function build_mingw_libmangle()
{
  local mingw_libmangle_folder_name="mingw-${MINGW_VERSION}-libmangle${MINGW_NAME_SUFFIX}"

  local mingw_libmangle_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${mingw_libmangle_folder_name}-installed"
  if [ ! -f "${mingw_libmangle_stamp_file_path}" ]
  then

    (
      mkdir -p "${BUILD_FOLDER_PATH}/${mingw_libmangle_folder_name}"
      cd "${BUILD_FOLDER_PATH}/${mingw_libmangle_folder_name}"

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS}"

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      if [ ! -f "config.status" ]
      then
        (
          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          echo
          echo "Running mingw-w64-libmangle${MINGW_NAME_SUFFIX} configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${MINGW_SRC_FOLDER_NAME}/mingw-w64-libraries/libmangle/configure" --help
          fi

          config_options=()
          # Note: native library.
          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}${MINGW_NAME_SUFFIX}")

          if [ -n "${MINGW_NAME_SUFFIX}" ]
          then
            config_options+=("--build=${BUILD}")
            config_options+=("--host=${BUILD}")
            config_options+=("--target=${BUILD}")
          else
            config_options+=("--build=${BUILD}")
            config_options+=("--host=${HOST}")
            config_options+=("--target=${TARGET}")
          fi

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${MINGW_SRC_FOLDER_NAME}/mingw-w64-libraries/libmangle/configure" \
            "${config_options[@]}"

         cp "config.log" "${LOGS_FOLDER_PATH}/${MINGW_FOLDER_NAME}/config-libmangle-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${MINGW_FOLDER_NAME}/configure-libmangle-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running mingw-w64-libmangle${MINGW_NAME_SUFFIX} make..."

        # Build.
        run_verbose make -j ${JOBS}

        run_verbose make install-strip

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${MINGW_FOLDER_NAME}/make-libmangle-output-$(ndate).txt"
    )

    touch "${mingw_libmangle_stamp_file_path}"

  else
    echo "Component mingw-w64-libmangle${MINGW_NAME_SUFFIX} already installed."
  fi
}


function build_mingw_gendef()
{
  local mingw_gendef_folder_name="mingw-${MINGW_VERSION}-gendef${MINGW_NAME_SUFFIX}"

  local mingw_gendef_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${mingw_gendef_folder_name}-installed"
  if [ ! -f "${mingw_gendef_stamp_file_path}" ]
  then

    (
      mkdir -p "${BUILD_FOLDER_PATH}/${mingw_gendef_folder_name}"
      cd "${BUILD_FOLDER_PATH}/${mingw_gendef_folder_name}"

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_APP_STATIC_GCC}"

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      if [ ! -f "config.status" ]
      then
        (
          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          echo
          echo "Running mingw-w64-gendef${MINGW_NAME_SUFFIX} configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${MINGW_SRC_FOLDER_NAME}/mingw-w64-tools/gendef/configure" --help
          fi

          config_options+=("--prefix=${APP_PREFIX}${MINGW_NAME_SUFFIX}")

          if [ -n "${MINGW_NAME_SUFFIX}" ]
          then
            config_options+=("--build=${BUILD}")
            config_options+=("--host=${BUILD}")
            config_options+=("--target=${BUILD}")
          else
            config_options+=("--build=${BUILD}")
            config_options+=("--host=${HOST}")
            config_options+=("--target=${TARGET}")
          fi

          config_options+=("--with-mangle=${LIBS_INSTALL_FOLDER_PATH}${MINGW_NAME_SUFFIX}")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${MINGW_SRC_FOLDER_NAME}/mingw-w64-tools/gendef/configure" \
            "${config_options[@]}"

         cp "config.log" "${LOGS_FOLDER_PATH}/${MINGW_FOLDER_NAME}/config-gendef-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${MINGW_FOLDER_NAME}/configure-gendef-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running mingw-w64-gendef${MINGW_NAME_SUFFIX} make..."

        # Build.
        run_verbose make -j ${JOBS}

        run_verbose make install-strip

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${MINGW_FOLDER_NAME}/make-gendef-output-$(ndate).txt"
    )

    touch "${mingw_gendef_stamp_file_path}"

  else
    echo "Component mingw-w64-gendef${MINGW_NAME_SUFFIX} already installed."
  fi
}


function build_mingw_widl()
{
  local mingw_widl_folder_name="mingw-${MINGW_VERSION}-widl${MINGW_NAME_SUFFIX}"

  local mingw_widl_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${mingw_widl_folder_name}-installed"
  if [ ! -f "${mingw_widl_stamp_file_path}" ]
  then

    (
      mkdir -p "${BUILD_FOLDER_PATH}/${mingw_widl_folder_name}"
      cd "${BUILD_FOLDER_PATH}/${mingw_widl_folder_name}"

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_APP_STATIC_GCC}"

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      if [ ! -f "config.status" ]
      then
        (
          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          echo
          echo "Running mingw-w64-widl${MINGW_NAME_SUFFIX} configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${MINGW_SRC_FOLDER_NAME}/mingw-w64-tools/widl/configure" --help
          fi

          config_options=()
          config_options+=("--prefix=${APP_PREFIX}${MINGW_NAME_SUFFIX}")

          if [ -n "${MINGW_NAME_SUFFIX}" ]
          then
            config_options+=("--build=${BUILD}")
            config_options+=("--host=${BUILD}") # Native!
            config_options+=("--target=${TARGET}")

          else
            config_options+=("--build=${BUILD}")
            config_options+=("--host=${HOST}")
            config_options+=("--target=${TARGET}")

            # To remove any target specific prefix and leave only widl.exe.
            config_options+=("--program-prefix=")
          fi

          config_options+=("--with-widl-includedir=${APP_PREFIX}${MINGW_NAME_SUFFIX}/${CROSS_COMPILE_PREFIX}/include")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${MINGW_SRC_FOLDER_NAME}/mingw-w64-tools/widl/configure" \
            "${config_options[@]}"

         cp "config.log" "${LOGS_FOLDER_PATH}/${MINGW_FOLDER_NAME}/config-widl-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${MINGW_FOLDER_NAME}/configure-widl-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running mingw-w64-widl${MINGW_NAME_SUFFIX} make..."

        # Build.
        run_verbose make -j ${JOBS}

        run_verbose make install-strip

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${MINGW_FOLDER_NAME}/make-widl-output-$(ndate).txt"
    )

    touch "${mingw_widl_stamp_file_path}"

  else
    echo "Component mingw-w64-widl${MINGW_NAME_SUFFIX} already installed."
  fi
}

# -----------------------------------------------------------------------------

# Environment variables:
# QEMU_GIT_URL
# QEMU_GIT_BRANCH
# QEMU_GIT_COMMIT

function build_qemu()
{
  # Stick to upstream as long as possible.
  # https://github.com/qemu/qemu/tags

  # The second choice is the xPack fork.
  # https://github.com/xpack-dev-tools/qemu

  # https://github.com/archlinux/svntogit-packages/blob/packages/qemu/trunk/PKGBUILD
  # https://github.com/archlinux/svntogit-community/blob/packages/libvirt/trunk/PKGBUILD

  # https://github.com/Homebrew/homebrew-core/blob/master/Formula/qemu.rb

  # https://github.com/msys2/MINGW-packages/blob/master/mingw-w64-qemu/PKGBUILD

  local qemu_version="$1"
  local qemu_target="$2" # arm, riscv, tools

  qemu_src_folder_name="${QEMU_SRC_FOLDER_NAME:-qemu-${qemu_version}.git}"

  local qemu_folder_name="qemu-${qemu_version}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${qemu_folder_name}/"

  local qemu_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-${qemu_folder_name}-installed"
  if [ ! -f "${qemu_stamp_file_path}" ] || [ "${IS_DEBUG}" == "y" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    if [ ! -d "${SOURCES_FOLDER_PATH}/${qemu_src_folder_name}" ]
    then
      git_clone "${QEMU_GIT_URL}" "${QEMU_GIT_BRANCH}" \
          "${QEMU_GIT_COMMIT}" "${SOURCES_FOLDER_PATH}/${qemu_src_folder_name}"

      # Simple way to customise the greeting message, instead of
      # managing a patch, or a fork.
      run_verbose sed -i.bak \
        -e 's|printf("QEMU emulator version "|printf("xPack QEMU emulator version "|' \
        "${SOURCES_FOLDER_PATH}/${qemu_src_folder_name}/softmmu/vl.c"
    fi
    # exit 1

    (
      mkdir -p "${BUILD_FOLDER_PATH}/${qemu_folder_name}"
      cd "${BUILD_FOLDER_PATH}/${qemu_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"

      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_APP}"
      if [ "${TARGET_PLATFORM}" == "linux" ]
      then
        LDFLAGS+=" -Wl,-rpath,${LD_LIBRARY_PATH}"
      elif [ "${TARGET_PLATFORM}" == "win32" ]
      then
        LDFLAGS+=" -fstack-protector"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS

      export LDFLAGS

      (
        if [ ! -f "config.status" ]
        then

          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          echo
          echo "Running qemu ${qemu_target} configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            # Although it shouldn't, the script checks python before --help.
            run_verbose bash "${SOURCES_FOLDER_PATH}/${qemu_src_folder_name}/configure" \
              --help
          fi

          config_options=()

          config_options+=("--prefix=${APP_PREFIX}")

          config_options+=("--bindir=${APP_PREFIX}/bin")

          # This seems redundant, but without it the greeting
          # string is suffixed by -dirty.
          config_options+=("--with-pkgversion=${QEMU_GIT_COMMIT}")

          if [ "${TARGET_PLATFORM}" == "win32" ]
          then
            config_options+=("--cross-prefix=${CROSS_COMPILE_PREFIX}-")
          fi

          config_options+=("--cc=${CC}")
          config_options+=("--cxx=${CXX}")

          # CFLAGS, CXXFLAGS and LDFLAGS are used directly.
          config_options+=("--extra-cflags=${CPPFLAGS}")
          config_options+=("--extra-cxxflags=${CPPFLAGS}")

          if [ "${qemu_target}" == "arm" ]
          then
            config_options+=("--target-list=arm-softmmu,aarch64-softmmu")
            config_options+=("--disable-tools")
          elif [ "${qemu_target}" == "riscv" ]
          then
            config_options+=("--target-list=riscv32-softmmu,riscv64-softmmu")
            config_options+=("--disable-tools")
          elif [ "${qemu_target}" == "tools" ]
          then
            config_options+=("--target-list=") # None
            config_options+=("--enable-tools")
          else
            echo "Unsupported qemu_target ${qemu_target}"
            exit 1
          fi

          if [ "${IS_DEBUG}" == "y" ]
          then
            config_options+=("--enable-debug")
          fi

          config_options+=("--enable-nettle")
          config_options+=("--enable-lzo")

          # Not toghether with nettle.
          # config_options+=("--enable-gcrypt")

          if [ "${TARGET_PLATFORM}" != "win32" ]
          then
            config_options+=("--enable-libssh")
            config_options+=("--enable-curses")
            config_options+=("--enable-vde")
          fi

          if [ "${TARGET_PLATFORM}" == "darwin" ]
          then
            # For now, Cocoa builds fail on macOS 10.13.
            if [ "${ENABLE_QEMU_SDL:-"n"}" == "y" ]
            then
              # In the first Arm release.
              config_options+=("--disable-cocoa")
              config_options+=("--enable-sdl")
            else
              config_options+=("--enable-cocoa")
              config_options+=("--disable-sdl")
            fi
            # Prevent codesign issues caused by including the Hypervisor.
            config_options+=("--disable-hvf")
          else
            config_options+=("--enable-sdl")
          fi

          config_options+=("--disable-bsd-user")
          config_options+=("--disable-guest-agent")
          config_options+=("--disable-gtk")

          if [ "${WITH_STRIP}" != "y" ]
          then
            config_options+=("--disable-strip")
          fi

          config_options+=("--disable-werror")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${qemu_src_folder_name}/configure" \
            ${config_options[@]}

        fi
        cp "config.log" "${LOGS_FOLDER_PATH}/${qemu_folder_name}/configure-log-$(ndate).txt"
      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${qemu_folder_name}/configure-output-$(ndate).txt"

      (
        echo
        echo "Running qemu ${qemu_target} make..."

        # Build.
        run_verbose make -j ${JOBS} # V=1

        run_verbose make install

        if [ "${qemu_target}" == "arm" ]
        then
          show_libs "${APP_PREFIX}/bin/qemu-system-aarch64"
        elif [ "${qemu_target}" == "riscv" ]
        then
          show_libs "${APP_PREFIX}/bin/qemu-system-riscv64"
        elif [ "${qemu_target}" == "tools" ]
        then
          show_libs "${APP_PREFIX}/bin/qemu-img"
        else
          echo "Unsupported qemu_target ${qemu_target}"
          exit 1
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${qemu_folder_name}/make-output-$(ndate).txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${qemu_src_folder_name}" \
        "qemu-${QEMU_VERSION}"
    )

    touch "${qemu_stamp_file_path}"
  else
    echo "Component qemu ${qemu_target} already installed."
  fi

  # Define this function at package level.
  tests_add "test_qemu_${qemu_target}"
}

# -----------------------------------------------------------------------------

# binutils should not be used on Darwin, the build is ok, but
# there are functional issues, due to the different ld/as/etc.

function build_native_binutils()
{
  # https://www.gnu.org/software/binutils/
  # https://ftp.gnu.org/gnu/binutils/

  # https://archlinuxarm.org/packages/aarch64/binutils/files/PKGBUILD
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=gdb-git

  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=mingw-w64-binutils
  # https://github.com/msys2/MINGW-packages/blob/master/mingw-w64-binutils/PKGBUILD

  # mingw-w64
  # https://github.com/archlinux/svntogit-community/blob/packages/mingw-w64-binutils/trunk/PKGBUILD


  # 2017-07-24, "2.29"
  # 2018-01-28, "2.30"
  # 2018-07-18, "2.31.1"
  # 2019-02-02, "2.32"
  # 2019-10-12, "2.33.1"
  # 2020-02-01, "2.34"
  # 2020-07-24, "2.35"
  # 2020-09-19, "2.35.1"
  # 2021-01-24, "2.36"
  # 2021-01-30, "2.35.2"
  # 2021-02-06, "2.36.1"
  # 2021-07-18, "2.37"
  # 2022-02-09, "2.38"
  # 2022-08-05, "2.39"

  local binutils_version="$1"
  local name_suffix=${2-''}

  local binutils_src_folder_name="binutils-${binutils_version}"
  local binutils_folder_name="${binutils_src_folder_name}${name_suffix}"

  local binutils_archive="${binutils_src_folder_name}.tar.xz"
  local binutils_url="https://ftp.gnu.org/gnu/binutils/${binutils_archive}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${binutils_folder_name}"

  local binutils_patch_file_name="binutils-${binutils_version}.patch"
  local binutils_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-${binutils_folder_name}-installed"
  if [ ! -f "${binutils_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${binutils_url}" "${binutils_archive}" \
      "${binutils_src_folder_name}" "${binutils_patch_file_name}"

    local binutils_prerequisites_download_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${binutils_folder_name}-prerequisites-downloaded"
    if false # [ ! -f "${binutils_prerequisites_download_stamp_file_path}" ]
    then
      (
        cd "${SOURCES_FOLDER_PATH}/${binutils_src_folder_name}"

        # Fool the script to think it is in the gcc folder.
        mkdir -p gcc
        touch gcc/BASE-VER

        run_verbose bash "${SOURCES_FOLDER_PATH}/${GCC_SRC_FOLDER_NAME}/contrib/download_prerequisites"  --no-verify

        rm -rf gcc

        touch "${binutils_prerequisites_download_stamp_file_path}"
      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${binutils_folder_name}/prerequisites-download-output-$(ndate).txt"
    fi

    (
      mkdir -p "${BUILD_FOLDER_PATH}/${binutils_folder_name}"
      cd "${BUILD_FOLDER_PATH}/${binutils_folder_name}"

      if [ -n "${name_suffix}" ]
      then

        CPPFLAGS="${XBB_CPPFLAGS} -I${LIBS_INSTALL_FOLDER_PATH}${name_suffix}/include"
        CFLAGS="${XBB_CFLAGS_NO_W}"
        CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

        LDFLAGS="${XBB_LDFLAGS_APP_STATIC_GCC} -Wl,-rpath,${XBB_FOLDER_PATH}/lib"

      else
        # To access the newly compiled libraries.
        xbb_activate_installed_dev

        CPPFLAGS="${XBB_CPPFLAGS}"
        CFLAGS="${XBB_CFLAGS_NO_W}"
        CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

        LDFLAGS="${XBB_LDFLAGS_APP_STATIC_GCC}"

        if [ "${TARGET_PLATFORM}" == "win32" ]
        then
          if [ "${TARGET_ARCH}" == "x32" -o "${TARGET_ARCH}" == "ia32" ]
          then
            # From MSYS2 MINGW
            LDFLAGS+=" -Wl,--large-address-aware"
          fi

          # Used to enable wildcard; inspired from arm-none-eabi-gcc.
          LDFLAGS+=" -Wl,${XBB_FOLDER_PATH}/usr/${CROSS_COMPILE_PREFIX}/lib/CRT_glob.o"
        elif [ "${TARGET_PLATFORM}" == "linux" ]
        then
          LDFLAGS+=" -Wl,-rpath,${LD_LIBRARY_PATH:-${LIBS_INSTALL_FOLDER_PATH}/lib}"
        fi
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      if [ ! -f "config.status" ]
      then
        (
          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          echo
          echo "Running binutils${name_suffix} configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${binutils_src_folder_name}/configure" --help

            run_verbose bash "${SOURCES_FOLDER_PATH}/${binutils_src_folder_name}/binutils/configure" --help
            run_verbose bash "${SOURCES_FOLDER_PATH}/${binutils_src_folder_name}/bfd/configure" --help
            run_verbose bash "${SOURCES_FOLDER_PATH}/${binutils_src_folder_name}/gas/configure" --help
            run_verbose bash "${SOURCES_FOLDER_PATH}/${binutils_src_folder_name}/ld/configure" --help
          fi

          # ? --without-python --without-curses, --with-expat
          config_options=()

          if [ -n "${name_suffix}" ]
          then

            config_options+=("--prefix=${APP_PREFIX}${name_suffix}")
            config_options+=("--with-sysroot=${APP_PREFIX}${name_suffix}")

            config_options+=("--build=${BUILD}")
            # The bootstrap binaries will run on the build machine.
            config_options+=("--host=${BUILD}")
            config_options+=("--target=${TARGET}")

            config_options+=("--with-pkgversion=${GCC_BOOTSTRAP_BRANDING}")

            config_options+=("--with-libiconv-prefix=${XBB_FOLDER_PATH}")

            config_options+=("--disable-multilib")
            config_options+=("--disable-werror")
            config_options+=("--disable-shared")
            config_options+=("--disable-nls")

            config_options+=("--enable-static")
            config_options+=("--enable-build-warnings=no")
            config_options+=("--enable-lto")
            config_options+=("--enable-plugins")
            config_options+=("--enable-deterministic-archives")
            config_options+=("--enable-libssp")

          else

            config_options+=("--prefix=${APP_PREFIX}")
            config_options+=("--with-sysroot=${APP_PREFIX}")
            # config_options+=("--with-lib-path=/usr/lib:/usr/local/lib")
            config_options+=("--program-suffix=")

            config_options+=("--infodir=${APP_PREFIX_DOC}/info")
            config_options+=("--mandir=${APP_PREFIX_DOC}/man")
            config_options+=("--htmldir=${APP_PREFIX_DOC}/html")
            config_options+=("--pdfdir=${APP_PREFIX_DOC}/pdf")

            config_options+=("--build=${BUILD}")
            config_options+=("--host=${HOST}")
            config_options+=("--target=${TARGET}")

            config_options+=("--with-pkgversion=${BINUTILS_BRANDING}")

            if [ "${TARGET_PLATFORM}" != "linux" ]
            then
              config_options+=("--with-libiconv-prefix=${LIBS_INSTALL_FOLDER_PATH}")
            fi

            config_options+=("--without-system-zlib")

            config_options+=("--with-pic")

            # error: debuginfod is missing or unusable
            config_options+=("--without-debuginfod")

            if [ "${TARGET_PLATFORM}" == "win32" ]
            then

              config_options+=("--enable-ld")

              if [ "${TARGET_ARCH}" == "x64" ]
              then
                # From MSYS2 MINGW
                config_options+=("--enable-64-bit-bfd")
              fi

              config_options+=("--enable-shared")
              config_options+=("--enable-shared-libgcc")

            elif [ "${TARGET_PLATFORM}" == "linux" ]
            then

              config_options+=("--enable-ld")

              config_options+=("--disable-shared")
              config_options+=("--disable-shared-libgcc")

            elif [ "${TARGET_PLATFORM}" == "darwin" ]
            then
              echo
              echo "binutils not supported on macOS"
              exit 1
            else
              echo "Oops! Unsupported ${TARGET_PLATFORM}."
              exit 1
            fi

            config_options+=("--enable-static")

            config_options+=("--enable-gold")
            config_options+=("--enable-lto")
            config_options+=("--enable-libssp")
            config_options+=("--enable-relro")
            config_options+=("--enable-threads")
            config_options+=("--enable-interwork")
            config_options+=("--enable-plugins")
            config_options+=("--enable-build-warnings=no")
            config_options+=("--enable-deterministic-archives")

            # TODO
            # config_options+=("--enable-nls")
            config_options+=("--disable-nls")

            config_options+=("--disable-new-dtags")

            config_options+=("--disable-multilib")
            config_options+=("--disable-werror")
            config_options+=("--disable-sim")

          fi

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${binutils_src_folder_name}/configure" \
            ${config_options[@]}

          cp "config.log" "${LOGS_FOLDER_PATH}/${binutils_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${binutils_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running binutils${name_suffix} make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_TESTS}" == "y" ]
        then
          : # run_verbose make check
        fi

        # Avoid strip here, it may interfere with patchelf.
        # make install-strip
        run_verbose make install

        if [ -n "${name_suffix}" ]
        then

          show_native_libs "${APP_PREFIX}${name_suffix}/bin/${CROSS_COMPILE_PREFIX}-ar"
          show_native_libs "${APP_PREFIX}${name_suffix}/bin/${CROSS_COMPILE_PREFIX}-as"
          show_native_libs "${APP_PREFIX}${name_suffix}/bin/${CROSS_COMPILE_PREFIX}-ld"
          show_native_libs "${APP_PREFIX}${name_suffix}/bin/${CROSS_COMPILE_PREFIX}-strip"
          show_native_libs "${APP_PREFIX}${name_suffix}/bin/${CROSS_COMPILE_PREFIX}-nm"
          show_native_libs "${APP_PREFIX}${name_suffix}/bin/${CROSS_COMPILE_PREFIX}-objcopy"
          show_native_libs "${APP_PREFIX}${name_suffix}/bin/${CROSS_COMPILE_PREFIX}-objdump"
          show_native_libs "${APP_PREFIX}${name_suffix}/bin/${CROSS_COMPILE_PREFIX}-ranlib"
          show_native_libs "${APP_PREFIX}${name_suffix}/bin/${CROSS_COMPILE_PREFIX}-size"
          show_native_libs "${APP_PREFIX}${name_suffix}/bin/${CROSS_COMPILE_PREFIX}-strings"

        else

          if [ "${TARGET_PLATFORM}" == "darwin" ]
          then
            : # rm -rv "${APP_PREFIX}/bin/strip"
          fi

          (
            xbb_activate_tex

            if [ "${WITH_PDF}" == "y" ]
            then
              run_verbose make pdf
              run_verbose make install-pdf
            fi

            if [ "${WITH_HTML}" == "y" ]
            then
              run_verbose make html
              run_verbose make install-html
            fi
          )

          show_libs "${APP_PREFIX}/bin/ar"
          show_libs "${APP_PREFIX}/bin/as"
          show_libs "${APP_PREFIX}/bin/ld"
          show_libs "${APP_PREFIX}/bin/strip"
          show_libs "${APP_PREFIX}/bin/nm"
          show_libs "${APP_PREFIX}/bin/objcopy"
          show_libs "${APP_PREFIX}/bin/objdump"
          show_libs "${APP_PREFIX}/bin/ranlib"
          show_libs "${APP_PREFIX}/bin/size"
          show_libs "${APP_PREFIX}/bin/strings"

        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${binutils_folder_name}/make-output-$(ndate).txt"

      if [ -z "${name_suffix}" ]
      then
        copy_license \
          "${SOURCES_FOLDER_PATH}/${binutils_src_folder_name}" \
          "${binutils_folder_name}"
      fi

    )

    touch "${binutils_stamp_file_path}"
  else
    echo "Component binutils${name_suffix} already installed."
  fi

  if [ -n "${name_suffix}" ]
  then
    :
  else
    tests_add "test_native_binutils"
  fi
}

function test_native_binutils()
{
  (
    if [ -d "xpacks/.bin" ]
    then
      TEST_BIN_PATH="$(pwd)/xpacks/.bin"
    elif [ -d "${APP_PREFIX}/bin" ]
    then
      TEST_BIN_PATH="${APP_PREFIX}/bin"
    else
      echo "Wrong folder."
      exit 1
    fi

    show_libs "${TEST_BIN_PATH}/ar"
    show_libs "${TEST_BIN_PATH}/as"
    show_libs "${TEST_BIN_PATH}/elfedit"
    show_libs "${TEST_BIN_PATH}/gprof"
    show_libs "${TEST_BIN_PATH}/ld"
    show_libs "${TEST_BIN_PATH}/ld.gold"
    show_libs "${TEST_BIN_PATH}/strip"
    show_libs "${TEST_BIN_PATH}/nm"
    show_libs "${TEST_BIN_PATH}/objcopy"
    show_libs "${TEST_BIN_PATH}/objdump"
    show_libs "${TEST_BIN_PATH}/ranlib"
    show_libs "${TEST_BIN_PATH}/readelf"
    show_libs "${TEST_BIN_PATH}/size"
    show_libs "${TEST_BIN_PATH}/strings"
    show_libs "${TEST_BIN_PATH}/strip"

    echo
    echo "Testing if binutils starts properly..."

    run_app "${TEST_BIN_PATH}/ar" --version
    run_app "${TEST_BIN_PATH}/as" --version
    run_app "${TEST_BIN_PATH}/elfedit" --version
    run_app "${TEST_BIN_PATH}/gprof" --version
    run_app "${TEST_BIN_PATH}/ld" --version
    if [ -f  "${TEST_BIN_PATH}/ld.gold${DOT_EXE}" ]
    then
      # No ld.gold on Windows.
      run_app "${TEST_BIN_PATH}/ld.gold" --version
    fi
    run_app "${TEST_BIN_PATH}/strip" --version
    run_app "${TEST_BIN_PATH}/nm" --version
    run_app "${TEST_BIN_PATH}/objcopy" --version
    run_app "${TEST_BIN_PATH}/objdump" --version
    run_app "${TEST_BIN_PATH}/ranlib" --version
    run_app "${TEST_BIN_PATH}/readelf" --version
    run_app "${TEST_BIN_PATH}/size" --version
    run_app "${TEST_BIN_PATH}/strings" --version
    run_app "${TEST_BIN_PATH}/strip" --version
  )

  echo
  echo "Local binutils tests completed successfuly."
}

# -----------------------------------------------------------------------------

# Environment variables:
# BINUTILS_VERSION
# BINUTILS_SRC_FOLDER_NAME
# BINUTILS_ARCHIVE_NAME
# BINUTILS_URL

# https://github.com/archlinux/svntogit-community/blob/packages/arm-none-eabi-binutils/trunk/PKGBUILD
# https://github.com/archlinux/svntogit-community/blob/packages/riscv32-elf-binutils/trunk/PKGBUILD

function build_cross_binutils()
{
  # https://ftp.gnu.org/gnu/binutils/
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=binutils-git
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=gdb-git

  local binutils_folder_name="binutils-${BINUTILS_VERSION}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${binutils_folder_name}"

  local binutils_patch="${binutils_folder_name}.patch"
  local binutils_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-${binutils_folder_name}-installed"
  if [ ! -f "${binutils_stamp_file_path}" ]
  then

    # Download binutils.
    cd "${SOURCES_FOLDER_PATH}"
    download_and_extract "${BINUTILS_ARCHIVE_URL}" "${BINUTILS_ARCHIVE_NAME}" \
        "${BINUTILS_SRC_FOLDER_NAME}" "${binutils_patch}"

    (
      mkdir -pv "${BUILD_FOLDER_PATH}/${binutils_folder_name}"
      cd "${BUILD_FOLDER_PATH}/${binutils_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS} -UFORTIFY_SOURCE" # ABE
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_APP}"
      if [ "${TARGET_PLATFORM}" == "win32" ]
      then
        LDFLAGS+=" -Wl,${XBB_FOLDER_PATH}/mingw/lib/CRT_glob.o"
      elif [ "${TARGET_PLATFORM}" == "linux" ]
      then
        LDFLAGS+=" -Wl,-rpath,${LD_LIBRARY_PATH}"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      if [ ! -f "config.status" ]
      then
        (
          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          echo
          echo "Running cross binutils configure..."

          bash "${SOURCES_FOLDER_PATH}/${BINUTILS_SRC_FOLDER_NAME}/configure" --help

          # 11.2-2022.02-darwin-x86_64-arm-none-eabi-manifest.txt:
          # binutils_configure='--enable-initfini-array --disable-nls
          # --without-x --disable-gdbtk --without-tcl --without-tk
          # --enable-plugins --disable-gdb --without-gdb --target=arm-none-eabi
          # --prefix=/'

          # 11.2-2022.02-darwin-x86_64-aarch64-none-elf-manifest.txt
          # binutils_configure='--enable-64-bit-bfd
          # --enable-targets=arm-none-eabi,aarch64-none-linux-gnu,aarch64-none-elf
          # --enable-initfini-array --disable-nls --without-x --disable-gdbtk
          # --without-tcl --without-tk --enable-plugins --disable-gdb
          # --without-gdb --target=aarch64-none-elf --prefix=/'

          # ? --without-python --without-curses, --with-expat

          config_options=()

          config_options+=("--prefix=${APP_PREFIX}")
          config_options+=("--infodir=${APP_PREFIX_DOC}/info")
          config_options+=("--mandir=${APP_PREFIX_DOC}/man")
          config_options+=("--htmldir=${APP_PREFIX_DOC}/html")
          config_options+=("--pdfdir=${APP_PREFIX_DOC}/pdf")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${GCC_TARGET}")

          config_options+=("--disable-nls") # Arm, AArch64
          config_options+=("--disable-gdb") # Arm, AArch64
          config_options+=("--disable-gdbtk") # Arm, AArch64

          config_options+=("--disable-sim")
          config_options+=("--disable-werror")

          config_options+=("--enable-initfini-array") # Arm, AArch64
          config_options+=("--enable-lto")
          config_options+=("--enable-plugins") # Arm, AArch64
          config_options+=("--enable-build-warnings=no")
          config_options+=("--with-isa-spec=2.2")
          if [ "${GCC_TARGET}" == "aarch64-none-elf" ]
          then
            config_options+=("--enable-64-bit-bfd") # AArch64
            config_options+=("--enable-targets=arm-none-eabi,aarch64-none-linux-gnu,aarch64-none-elf") # AArch64
          fi

          config_options+=("--without-gdb") # Arm, AArch64
          config_options+=("--without-x") # Arm, AArch64
          config_options+=("--without-tcl") # Arm, AArch64
          config_options+=("--without-tk") # Arm, AArch64

          config_options+=("--with-pkgversion=${BRANDING}")
          config_options+=("--with-system-zlib")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${BINUTILS_SRC_FOLDER_NAME}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${binutils_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${binutils_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running cross binutils make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_TESTS}" == "y" ]
        then
          if [ "${TARGET_PLATFORM}" == "darwin" ]
          then
            # /bin/bash: DSYMUTIL@: command not found
            :
          else
            run_verbose make check
          fi
        fi

        # Avoid strip here, it may interfere with patchelf.
        # make install-strip
        run_verbose make install

        (
          xbb_activate_tex

          if [ "${WITH_PDF}" == "y" ]
          then
            run_verbose make pdf
            run_verbose make install-pdf
          fi

          if [ "${WITH_HTML}" == "y" ]
          then
            run_verbose make html
            run_verbose make install-html
          fi
        )

        if [ -n "${APP_PREFIX_NANO:-}" ]
        then
          # Without this copy, the build for the nano version of the GCC second
          # step fails with unexpected errors, like "cannot compute suffix of
          # object files: cannot compile".
          copy_dir "${APP_PREFIX}" "${APP_PREFIX_NANO}"
        fi

        show_libs "${APP_PREFIX}/bin/${GCC_TARGET}-ar"
        show_libs "${APP_PREFIX}/bin/${GCC_TARGET}-as"
        show_libs "${APP_PREFIX}/bin/${GCC_TARGET}-ld"
        show_libs "${APP_PREFIX}/bin/${GCC_TARGET}-nm"
        show_libs "${APP_PREFIX}/bin/${GCC_TARGET}-objcopy"
        show_libs "${APP_PREFIX}/bin/${GCC_TARGET}-objdump"
        show_libs "${APP_PREFIX}/bin/${GCC_TARGET}-ranlib"
        show_libs "${APP_PREFIX}/bin/${GCC_TARGET}-size"
        show_libs "${APP_PREFIX}/bin/${GCC_TARGET}-strings"
        show_libs "${APP_PREFIX}/bin/${GCC_TARGET}-strip"

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${binutils_folder_name}/make-output-$(ndate).txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${BINUTILS_SRC_FOLDER_NAME}" \
        "${binutils_folder_name}"
    )

    touch "${binutils_stamp_file_path}"

  else
    echo "Component cross binutils already installed."
  fi

  tests_add "test_cross_binutils"
}

function test_cross_binutils()
{
  (
    if [ -d "xpacks/.bin" ]
    then
      TEST_BIN_PATH="$(pwd)/xpacks/.bin"
    elif [ -d "${APP_PREFIX}/bin" ]
    then
      TEST_BIN_PATH="${APP_PREFIX}/bin"
    else
      echo "Wrong folder."
      exit 1
    fi

    show_libs "${TEST_BIN_PATH}/${GCC_TARGET}-ar"
    show_libs "${TEST_BIN_PATH}/${GCC_TARGET}-as"
    show_libs "${TEST_BIN_PATH}/${GCC_TARGET}-ld"
    show_libs "${TEST_BIN_PATH}/${GCC_TARGET}-nm"
    show_libs "${TEST_BIN_PATH}/${GCC_TARGET}-objcopy"
    show_libs "${TEST_BIN_PATH}/${GCC_TARGET}-objdump"
    show_libs "${TEST_BIN_PATH}/${GCC_TARGET}-ranlib"
    show_libs "${TEST_BIN_PATH}/${GCC_TARGET}-size"
    show_libs "${TEST_BIN_PATH}/${GCC_TARGET}-strings"
    show_libs "${TEST_BIN_PATH}/${GCC_TARGET}-strip"

    run_app "${TEST_BIN_PATH}/${GCC_TARGET}-ar" --version
    run_app "${TEST_BIN_PATH}/${GCC_TARGET}-as" --version
    run_app "${TEST_BIN_PATH}/${GCC_TARGET}-ld" --version
    run_app "${TEST_BIN_PATH}/${GCC_TARGET}-nm" --version
    run_app "${TEST_BIN_PATH}/${GCC_TARGET}-objcopy" --version
    run_app "${TEST_BIN_PATH}/${GCC_TARGET}-objdump" --version
    run_app "${TEST_BIN_PATH}/${GCC_TARGET}-ranlib" --version
    run_app "${TEST_BIN_PATH}/${GCC_TARGET}-size" --version
    run_app "${TEST_BIN_PATH}/${GCC_TARGET}-strings" --version
    run_app "${TEST_BIN_PATH}/${GCC_TARGET}-strip" --version
  )
}

# -----------------------------------------------------------------------------

function define_flags_for_target()
{
  local optimize="${CFLAGS_OPTIMIZATIONS_FOR_TARGET}"
  if [ "$1" == "" ]
  then
    # For newlib, optimize for speed.
    optimize="$(echo ${optimize} | sed -e 's/-O[123]/-O2/g')"
    # DO NOT make this explicit, since exceptions references will always be
    # inserted in the `extab` section.
    # optimize+=" -fexceptions"
  elif [ "$1" == "-nano" ]
  then
    # For newlib-nano optimize for size and disable exceptions.
    optimize="$(echo ${optimize} | sed -e 's/-O[123]/-Os/g')"
    optimize="$(echo ${optimize} | sed -e 's/-Ofast/-Os/p')"
    optimize+=" -fno-exceptions"
  fi

  CFLAGS_FOR_TARGET="${optimize}"
  CXXFLAGS_FOR_TARGET="${optimize}"
  if [ "${IS_DEBUG}" == "y" ]
  then
    # Avoid `-g`, many local symbols cannot be removed by strip.
    CFLAGS_FOR_TARGET+=" -g"
    CXXFLAGS_FOR_TARGET+=" -g"
  fi

  if [ "${WITH_LIBS_LTO:-}" == "y" ]
  then
    CFLAGS_FOR_TARGET+=" -flto -ffat-lto-objects"
    CXXFLAGS_FOR_TARGET+=" -flto -ffat-lto-objects"
  fi

  LDFLAGS_FOR_TARGET="--specs=nosys.specs"
}

# -----------------------------------------------------------------------------

function download_cross_gcc()
{
  if [ ! -d "${SOURCES_FOLDER_PATH}/${GCC_SRC_FOLDER_NAME}" ]
  then
    (
      cd "${SOURCES_FOLDER_PATH}"

      download_and_extract "${GCC_ARCHIVE_URL}" \
        "${GCC_ARCHIVE_NAME}" "${GCC_SRC_FOLDER_NAME}" \
        "${GCC_PATCH_FILE_NAME}"
    )
  fi
}

# Environment variables:
# GCC_VERSION
# GCC_SRC_FOLDER_NAME
# GCC_ARCHIVE_URL
# GCC_ARCHIVE_NAME
# GCC_PATCH_FILE_NAME

# https://github.com/archlinux/svntogit-community/blob/packages/arm-none-eabi-gcc/trunk/PKGBUILD
# https://github.com/archlinux/svntogit-community/blob/packages/riscv64-elf-gcc/trunk/PKGBUILD

function build_cross_gcc_first()
{
  local gcc_first_folder_name="gcc-${GCC_VERSION}-first"

  mkdir -pv "${LOGS_FOLDER_PATH}/${gcc_first_folder_name}"

  local gcc_first_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-${gcc_first_folder_name}-installed"
  if [ ! -f "${gcc_first_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_cross_gcc

    (
      mkdir -pv "${BUILD_FOLDER_PATH}/${gcc_first_folder_name}"
      cd "${BUILD_FOLDER_PATH}/${gcc_first_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"
      if [ "${TARGET_PLATFORM}" == "win32" ]
      then
        # The CFLAGS are set in XBB_CFLAGS, but for C++ it must be selective.
        # Without it gcc cannot identify cc1 and other binaries
        CXXFLAGS+=" -D__USE_MINGW_ACCESS"
      fi
      LDFLAGS="${XBB_LDFLAGS_APP}"
      if [ "${TARGET_PLATFORM}" == "linux" ]
      then
        LDFLAGS+=" -Wl,-rpath,${LD_LIBRARY_PATH}"
      fi

      define_flags_for_target ""

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      export CFLAGS_FOR_TARGET
      export CXXFLAGS_FOR_TARGET
      export LDFLAGS_FOR_TARGET

      if [ ! -f "config.status" ]
      then
        (
          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          echo
          echo "Running cross gcc first stage configure..."

          bash "${SOURCES_FOLDER_PATH}/${GCC_SRC_FOLDER_NAME}/configure" --help

          # 11.2-2022.02-darwin-x86_64-arm-none-eabi-manifest.txt:
          # gcc1_configure='--target=arm-none-eabi
          # --prefix=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-arm-none-eabi/install//
          # --with-gmp=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-arm-none-eabi/host-tools
          # --with-mpfr=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-arm-none-eabi/host-tools
          # --with-mpc=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-arm-none-eabi/host-tools
          # --with-isl=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-arm-none-eabi/host-tools
          # --disable-shared --disable-nls --disable-threads --disable-tls
          # --enable-checking=release --enable-languages=c --without-cloog
          # --without-isl --with-newlib --without-headers
          # --with-multilib-list=aprofile,rmprofile'

          # 11.2-2022.02-darwin-x86_64-aarch64-none-elf-manifest.txt
          # gcc1_configure='--target=aarch64-none-elf
          # --prefix=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-aarch64-none-elf/install//
          # --with-gmp=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-aarch64-none-elf/host-tools
          # --with-mpfr=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-aarch64-none-elf/host-tools
          # --with-mpc=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-aarch64-none-elf/host-tools
          # --with-isl=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-aarch64-none-elf/host-tools
          # --disable-shared --disable-nls --disable-threads --disable-tls
          # --enable-checking=release --enable-languages=c --without-cloog
          # --without-isl --with-newlib --without-headers'

          # From: https://gcc.gnu.org/install/configure.html
          # --enable-shared[=package[,…]] build shared versions of libraries
          # --enable-tls specify that the target supports TLS (Thread Local Storage).
          # --enable-nls enables Native Language Support (NLS)
          # --enable-checking=list the compiler is built to perform internal consistency checks of the requested complexity. ‘yes’ (most common checks)
          # --with-headers=dir specify that target headers are available when building a cross compiler
          # --with-newlib Specifies that ‘newlib’ is being used as the target C library. This causes `__eprintf`` to be omitted from `libgcc.a`` on the assumption that it will be provided by newlib.
          # --enable-languages=c newlib does not use C++, so C should be enough

          # --enable-checking=no ???

          # --enable-lto make it explicit, Arm uses the default.

          # Prefer an explicit libexec folder.
          # --libexecdir="${APP_PREFIX}/lib"

          config_options=()

          config_options+=("--prefix=${APP_PREFIX}")
          config_options+=("--infodir=${APP_PREFIX_DOC}/info")
          config_options+=("--mandir=${APP_PREFIX_DOC}/man")
          config_options+=("--htmldir=${APP_PREFIX_DOC}/html")
          config_options+=("--pdfdir=${APP_PREFIX_DOC}/pdf")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${GCC_TARGET}")

          config_options+=("--disable-libgomp") # ABE
          config_options+=("--disable-libmudflap") # ABE
          config_options+=("--disable-libquadmath") # ABE
          config_options+=("--disable-libsanitizer") # ABE
          config_options+=("--disable-libssp") # ABE

          config_options+=("--disable-nls") # Arm, AArch64
          config_options+=("--disable-shared") # Arm, AArch64
          config_options+=("--disable-threads") # Arm, AArch64
          config_options+=("--disable-tls") # Arm, AArch64

          config_options+=("--enable-checking=release") # Arm, AArch64
          config_options+=("--enable-languages=c") # Arm, AArch64
          # config_options+=("--enable-lto") # ABE

          config_options+=("--without-cloog") # Arm, AArch64
          config_options+=("--without-headers") # Arm, AArch64
          config_options+=("--without-isl") # Arm, AArch64

          config_options+=("--with-gnu-as") # Arm, ABE
          config_options+=("--with-gnu-ld") # Arm, ABE

          config_options+=("--with-gmp=${LIBS_INSTALL_FOLDER_PATH}") # AArch64
          config_options+=("--with-pkgversion=${BRANDING}")
          config_options+=("--with-newlib") # Arm, AArch64

          config_options+=("--with-system-zlib")

          if [ "${GCC_TARGET}" == "arm-none-eabi" ]
          then
            config_options+=("--disable-libatomic") # ABE

            if [ "${WITHOUT_MULTILIB}" == "y" ]
            then
              config_options+=("--disable-multilib")
            else
              config_options+=("--enable-multilib") # Arm
              config_options+=("--with-multilib-list=${GCC_MULTILIB_LIST}")  # Arm
            fi
          elif [ "${GCC_TARGET}" == "riscv-none-elf" ]
          then
            config_options+=("--with-abi=${GCC_ABI}")
            config_options+=("--with-arch=${GCC_ARCH}")
           config_options+=("--with-isa-spec=2.2")

            if [ "${WITHOUT_MULTILIB}" == "y" ]
            then
              config_options+=("--disable-multilib")
            else
              config_options+=("--enable-multilib")
            fi
          fi

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${GCC_SRC_FOLDER_NAME}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${gcc_first_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${gcc_first_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        # Partial build, without documentation.
        echo
        echo "Running cross gcc first stage make..."

        # No need to make 'all', 'all-gcc' is enough to compile the libraries.
        # Parallel builds may fail.
        run_verbose make -j ${JOBS} all-gcc
        # make all-gcc

        # No -strip available here.
        run_verbose make install-gcc

        # Strip?

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${gcc_first_folder_name}/make-output-$(ndate).txt"
    )

    touch "${gcc_first_stamp_file_path}"

  else
    echo "Component cross gcc first stage already installed."
  fi
}

# -----------------------------------------------------------------------------

# Environment variables:
# NEWLIB_VERSION
# NEWLIB_SRC_FOLDER_NAME
# NEWLIB_ARCHIVE_URL
# NEWLIB_ARCHIVE_NAME

# https://github.com/archlinux/svntogit-community/blob/packages/arm-none-eabi-newlib/trunk/PKGBUILD
# https://github.com/archlinux/svntogit-community/blob/packages/riscv32-elf-newlib/trunk/PKGBUILD

# For the nano build, call it with "-nano".
# $1="" or $1="-nano"
function build_cross_newlib()
{
  local name_suffix=${1-''}
  local newlib_folder_name="newlib-${NEWLIB_VERSION}${name_suffix}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${newlib_folder_name}"

  local newlib_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-${newlib_folder_name}-installed"
  if [ ! -f "${newlib_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    if [ ! -d "${NEWLIB_SRC_FOLDER_NAME}" ]
    then
      download_and_extract "${NEWLIB_ARCHIVE_URL}" "${NEWLIB_ARCHIVE_NAME}" \
      "${NEWLIB_SRC_FOLDER_NAME}"

      if [ "${ENABLE_NEWLIB_RISCV_NANO_CXX_PATCH:-""}" == "y" ]
      then
        echo
        echo "Patching nano.specs..."

        local nano_specs_file_path="${NEWLIB_SRC_FOLDER_NAME}/libgloss/riscv/nano.specs"
        if grep "%(nano_link)" "${nano_specs_file_path}" | grep -q "%:replace-outfile(-lstdc++ -lstdc++_nano)"
        then
          echo "-lstdc++_nano already in"
        else
          run_verbose sed -i.bak \
            -e 's|^\(%(nano_link) .*\)$|\1 %:replace-outfile(-lstdc++ -lstdc++_nano)|' \
            "${nano_specs_file_path}"
        fi
        if grep "%(nano_link)" "${nano_specs_file_path}" | grep -q "%:replace-outfile(-lsupc++ -lsupc++_nano)"
        then
          echo "-lsupc++_nano already in"
        else
          run_verbose sed -i.bak \
            -e 's|^\(%(nano_link) .*\)$|\1 %:replace-outfile(-lsupc++ -lsupc++_nano)|' \
            "${nano_specs_file_path}"
        fi
      fi
      # exit 1
    fi

    (
      mkdir -pv "${BUILD_FOLDER_PATH}/${newlib_folder_name}"
      cd "${BUILD_FOLDER_PATH}/${newlib_folder_name}"

      xbb_activate_installed_dev

      # Add the gcc first stage binaries to the path.
      PATH="${APP_PREFIX}/bin:${PATH}"

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      define_flags_for_target "${name_suffix}"

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS

      export CFLAGS_FOR_TARGET
      export CXXFLAGS_FOR_TARGET
      export LDFLAGS_FOR_TARGET

      if [ ! -f "config.status" ]
      then
        (
          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          # --disable-nls do not use Native Language Support
          # --enable-newlib-io-long-double   enable long double type support in IO functions printf/scanf
          # --enable-newlib-io-long-long   enable long long type support in IO functions like printf/scanf
          # --enable-newlib-io-c99-formats   enable C99 support in IO functions like printf/scanf
          # --enable-newlib-register-fini   enable finalization function registration using atexit
          # --disable-newlib-supplied-syscalls disable newlib from supplying syscalls (__NO_SYSCALLS__)

          # --disable-newlib-fvwrite-in-streamio    disable iov in streamio
          # --disable-newlib-fseek-optimization    disable fseek optimization
          # --disable-newlib-wide-orient    Turn off wide orientation in streamio
          # --disable-newlib-unbuf-stream-opt    disable unbuffered stream optimization in streamio
          # --enable-newlib-nano-malloc    use small-footprint nano-malloc implementation
          # --enable-lite-exit	enable light weight exit
          # --enable-newlib-global-atexit	enable atexit data structure as global
          # --enable-newlib-nano-formatted-io    Use nano version formatted IO
          # --enable-newlib-reent-small

          # --enable-newlib-retargetable-locking ???

          echo
          echo "Running cross newlib${name_suffix} configure..."

          bash "${SOURCES_FOLDER_PATH}/${NEWLIB_SRC_FOLDER_NAME}/configure" --help

          config_options=()

          if [ "${name_suffix}" == "" ]
          then

            # 11.2-2022.02-darwin-x86_64-arm-none-eabi-manifest.txt:
            # newlib_configure=' --disable-newlib-supplied-syscalls
            # --enable-newlib-io-long-long --enable-newlib-io-c99-formats
            # --enable-newlib-mb --enable-newlib-reent-check-verify
            # --target=arm-none-eabi --prefix=/'

            # 11.2-2022.02-darwin-x86_64-aarch64-none-elf-manifest.txt
            # newlib_configure=' --disable-newlib-supplied-syscalls
            # --enable-newlib-io-long-long --enable-newlib-io-c99-formats
            # --enable-newlib-mb --enable-newlib-reent-check-verify
            # --target=aarch64-none-elf --prefix=/'

            config_options+=("--prefix=${APP_PREFIX}")
            config_options+=("--infodir=${APP_PREFIX_DOC}/info")
            config_options+=("--mandir=${APP_PREFIX_DOC}/man")
            config_options+=("--htmldir=${APP_PREFIX_DOC}/html")
            config_options+=("--pdfdir=${APP_PREFIX_DOC}/pdf")

            config_options+=("--build=${BUILD}")
            config_options+=("--host=${HOST}")
            config_options+=("--target=${GCC_TARGET}")

            config_options+=("--disable-newlib-supplied-syscalls") # Arm, AArch64

            config_options+=("--enable-newlib-io-c99-formats") # Arm, AArch64

            config_options+=("--enable-newlib-io-long-long") # Arm, AArch64
            config_options+=("--enable-newlib-mb") # Arm, AArch64
            config_options+=("--enable-newlib-reent-check-verify") # Arm, AArch64

            config_options+=("--enable-newlib-register-fini") # Arm

            config_options+=("--enable-newlib-retargetable-locking") # Arm

            run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${NEWLIB_SRC_FOLDER_NAME}/configure" \
              "${config_options[@]}"

          elif [ "${name_suffix}" == "-nano" ]
          then

            # 11.2-2022.02-darwin-x86_64-arm-none-eabi-manifest.txt:
            # newlib_nano_configure=' --disable-newlib-supplied-syscalls
            # --enable-newlib-nano-malloc --disable-newlib-unbuf-stream-opt
            # --enable-newlib-reent-small --disable-newlib-fseek-optimization
            # --enable-newlib-nano-formatted-io
            # --disable-newlib-fvwrite-in-streamio --disable-newlib-wide-orient
            # --enable-lite-exit --enable-newlib-global-atexit
            # --enable-newlib-reent-check-verify
            # --target=arm-none-eabi --prefix=/'

            # --enable-newlib-io-long-long and --enable-newlib-io-c99-formats
            # are currently ignored if --enable-newlib-nano-formatted-io.
            # --enable-newlib-register-fini is debatable, was removed.

            config_options+=("--prefix=${APP_PREFIX_NANO}")

            config_options+=("--build=${BUILD}")
            config_options+=("--host=${HOST}")
            config_options+=("--target=${GCC_TARGET}")

            config_options+=("--disable-newlib-fseek-optimization") # Arm
            config_options+=("--disable-newlib-fvwrite-in-streamio") # Arm

            config_options+=("--disable-newlib-supplied-syscalls") # Arm
            config_options+=("--disable-newlib-unbuf-stream-opt") # Arm
            config_options+=("--disable-newlib-wide-orient") # Arm

            config_options+=("--enable-lite-exit") # Arm
            config_options+=("--enable-newlib-global-atexit") # Arm
            config_options+=("--enable-newlib-nano-formatted-io") # Arm
            config_options+=("--enable-newlib-nano-malloc") # Arm
            config_options+=("--enable-newlib-reent-check-verify") # Arm
            config_options+=("--enable-newlib-reent-small") # Arm

            config_options+=("--enable-newlib-retargetable-locking") # Arm

            run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${NEWLIB_SRC_FOLDER_NAME}/configure" \
              "${config_options[@]}"

          else
            echo "Unsupported build_cross_newlib name_suffix '${name_suffix}'"
            exit 1
          fi

          cp "config.log" "${LOGS_FOLDER_PATH}/${newlib_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${newlib_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        # Partial build, without documentation.
        echo
        echo "Running cross newlib${name_suffix} make..."

        # Parallel builds may fail.
        run_verbose make -j ${JOBS}
        # make

        # Top make fails with install-strip due to libgloss make.
        run_verbose make install

        if [ "${name_suffix}" == "" ]
        then

          if [ "${WITH_PDF}" == "y" ]
          then

            xbb_activate_tex

            # Warning, parallel build failed on Debian 32-bit.
            run_verbose make pdf

            install -v -d "${APP_PREFIX_DOC}/pdf"

            install -v -c -m 644 \
              "${GCC_TARGET}/libgloss/doc/porting.pdf" "${APP_PREFIX_DOC}/pdf"
            install -v -c -m 644 \
              "${GCC_TARGET}/newlib/libc/libc.pdf" "${APP_PREFIX_DOC}/pdf"
            install -v -c -m 644 \
              "${GCC_TARGET}/newlib/libm/libm.pdf" "${APP_PREFIX_DOC}/pdf"

          fi

          if [ "${WITH_HTML}" == "y" ]
          then

            run_verbose make html

            install -v -d "${APP_PREFIX_DOC}/html"

            copy_dir "${GCC_TARGET}/newlib/libc/libc.html" "${APP_PREFIX_DOC}/html/libc"
            copy_dir "${GCC_TARGET}/newlib/libm/libm.html" "${APP_PREFIX_DOC}/html/libm"

          fi

        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${newlib_folder_name}/make-output-$(ndate).txt"

      if [ "${name_suffix}" == "" ]
      then
        copy_license \
          "${SOURCES_FOLDER_PATH}/${NEWLIB_SRC_FOLDER_NAME}" \
          "${newlib_folder_name}"
      fi

    )

    touch "${newlib_stamp_file_path}"
  else
    echo "Component cross newlib$1 already installed."
  fi
}

# -----------------------------------------------------------------------------

function copy_cross_nano_libs()
{
  local src_folder="$1"
  local dst_folder="$2"

  # Copy the nano variants with a distinct name, as used in nano.specs.
  cp -v -f "${src_folder}/libc.a" "${dst_folder}/libc_nano.a"
  cp -v -f "${src_folder}/libg.a" "${dst_folder}/libg_nano.a"
  cp -v -f "${src_folder}/libm.a" "${dst_folder}/libm_nano.a"


  cp -v -f "${src_folder}/libstdc++.a" "${dst_folder}/libstdc++_nano.a"
  cp -v -f "${src_folder}/libsupc++.a" "${dst_folder}/libsupc++_nano.a"

  if [ -f "${src_folder}/libgloss.a" ]
  then
    cp -v -f "${src_folder}/libgloss.a" "${dst_folder}/libgloss_nano.a"
  fi

  if [ -f "${src_folder}/librdimon.a" ]
  then
    cp -v -f "${src_folder}/librdimon.a" "${dst_folder}/librdimon_nano.a"
  fi

  if [ -f "${src_folder}/librdimon-v2m.a" ]
  then
    cp -v -f "${src_folder}/librdimon-v2m.a" "${dst_folder}/lrdimon-v2m_nano.a"
  fi
}

# Copy target libraries from each multilib folders.
# $1=source
# $2=destination
# $3=target gcc
function copy_cross_multi_libs()
{
  local -a multilibs
  local multilib
  local multi_folder
  local src_folder="$1"
  local dst_folder="$2"
  local gcc_target="$3"

  echo ${gcc_target}
  multilibs=( $("${gcc_target}" -print-multi-lib 2>/dev/null) )
  if [ ${#multilibs[@]} -gt 0 ]
  then
    for multilib in "${multilibs[@]}"
    do
      multi_folder="${multilib%%;*}"
      copy_cross_nano_libs "${src_folder}/${multi_folder}" \
        "${dst_folder}/${multi_folder}"
    done
  else
    copy_cross_nano_libs "${src_folder}" "${dst_folder}"
  fi
}

# -----------------------------------------------------------------------------

function copy_cross_linux_libs()
{
  local copy_linux_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-copy-linux-completed"
  if [ ! -f "${copy_linux_stamp_file_path}" ]
  then

    local linux_path="${LINUX_INSTALL_RELATIVE_PATH}/${APP_LC_NAME}"

    (
      cd "${WORK_FOLDER_PATH}"

      copy_dir "${linux_path}/${GCC_TARGET}/lib" "${APP_PREFIX}/${GCC_TARGET}/lib"
      copy_dir "${linux_path}/${GCC_TARGET}/include" "${APP_PREFIX}/${GCC_TARGET}/include"
      copy_dir "${linux_path}/include" "${APP_PREFIX}/include"
      copy_dir "${linux_path}/lib" "${APP_PREFIX}/lib"
      copy_dir "${linux_path}/share" "${APP_PREFIX}/share"
    )

    (
      cd "${APP_PREFIX}"
      find "${GCC_TARGET}/lib" "${GCC_TARGET}/include" "include" "lib" "share" \
        -perm /111 -and ! -type d \
        -exec rm '{}' ';'
    )
    touch "${copy_linux_stamp_file_path}"

  else
    echo "Component copy-linux-libs already processed."
  fi
}

# -----------------------------------------------------------------------------

function add_cross_linux_install_path()
{
  # Verify that the compiler is there.
  "${WORK_FOLDER_PATH}/${LINUX_INSTALL_RELATIVE_PATH}/${APP_LC_NAME}/bin/${GCC_TARGET}-gcc" --version

  export PATH="${WORK_FOLDER_PATH}/${LINUX_INSTALL_RELATIVE_PATH}/${APP_LC_NAME}/bin:${PATH}"
  echo ${PATH}
}

# Environment variables:
# GCC_VERSION
# GCC_SRC_FOLDER_NAME
# GCC_ARCHIVE_URL
# GCC_ARCHIVE_NAME
# GCC_PATCH_FILE_NAME

# For the nano build, call it with "-nano".
# $1="" or $1="-nano"
function build_cross_gcc_final()
{
  local name_suffix=${1-''}

  local gcc_final_folder_name="gcc-${GCC_VERSION}-final${name_suffix}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${gcc_final_folder_name}"

  local gcc_final_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-${gcc_final_folder_name}-installed"
  if [ ! -f "${gcc_final_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_cross_gcc

    (
      mkdir -pv "${BUILD_FOLDER_PATH}/${gcc_final_folder_name}"
      cd "${BUILD_FOLDER_PATH}/${gcc_final_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      # if [ "${TARGET_PLATFORM}" == "darwin" ]
      # then
      #   # Hack to avoid spurious errors like:
      #   # fatal error: bits/nested_exception.h: No such file or directory
      #   CPPFLAGS+=" -I${BUILD_FOLDER_PATH}/${gcc_final_folder_name}/${GCC_TARGET}/libstdc++-v3/include"
      # fi
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"
      if [ "${TARGET_PLATFORM}" == "win32" ]
      then
        # The CFLAGS are set in XBB_CFLAGS, but for C++ it must be selective.
        # Without it gcc cannot identify cc1 and other binaries
        CXXFLAGS+=" -D__USE_MINGW_ACCESS"

        # Hack to prevent "too many sections", "File too big" etc in insn-emit.c
        CXXFLAGS=$(echo ${CXXFLAGS} | sed -e 's|-ffunction-sections -fdata-sections||')
      fi

      LDFLAGS="${XBB_LDFLAGS_APP}"
      if [ "${TARGET_PLATFORM}" == "linux" ]
      then
        LDFLAGS+=" -Wl,-rpath,${LD_LIBRARY_PATH}"
      fi
      # Do not add CRT_glob.o here, it will fail with already defined,
      # since it is already handled by --enable-mingw-wildcard.

      define_flags_for_target "${name_suffix}"

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      export CFLAGS_FOR_TARGET
      export CXXFLAGS_FOR_TARGET
      export LDFLAGS_FOR_TARGET

      if [ "${TARGET_PLATFORM}" == "win32" ]
      then
        add_cross_linux_install_path

        export AR_FOR_TARGET=${GCC_TARGET}-ar
        export NM_FOR_TARGET=${GCC_TARGET}-nm
        export OBJDUMP_FOR_TARET=${GCC_TARGET}-objdump
        export STRIP_FOR_TARGET=${GCC_TARGET}-strip
        export CC_FOR_TARGET=${GCC_TARGET}-gcc
        export GCC_FOR_TARGET=${GCC_TARGET}-gcc
        export CXX_FOR_TARGET=${GCC_TARGET}-g++
      fi

      if [ ! -f "config.status" ]
      then
        (
          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          echo
          echo "Running cross gcc${name_suffix} final stage configure..."

          bash "${SOURCES_FOLDER_PATH}/${GCC_SRC_FOLDER_NAME}/configure" --help

          # https://gcc.gnu.org/install/configure.html
          # --enable-shared[=package[,…]] build shared versions of libraries
          # --enable-tls specify that the target supports TLS (Thread Local Storage).
          # --enable-nls enables Native Language Support (NLS)
          # --enable-checking=list the compiler is built to perform internal consistency checks of the requested complexity. ‘yes’ (most common checks)
          # --with-headers=dir specify that target headers are available when building a cross compiler
          # --with-newlib Specifies that ‘newlib’ is being used as the target C library. This causes `__eprintf`` to be omitted from `libgcc.a`` on the assumption that it will be provided by newlib.
          # --enable-languages=c,c++ Support only C/C++, ignore all other.

          # Prefer an explicit libexec folder.
          # --libexecdir="${APP_PREFIX}/lib" \

          # --enable-lto make it explicit, Arm uses the default.
          # --with-native-system-header-dir is needed to locate stdio.h, to
          # prevent -Dinhibit_libc, which will skip some functionality,
          # like libgcov.

          config_options=()

          config_options+=("--prefix=${APP_PREFIX}${name_suffix}")
          if [ "${name_suffix}" == "" ]
          then
            config_options+=("--prefix=${APP_PREFIX}")
            config_options+=("--infodir=${APP_PREFIX_DOC}/info")
            config_options+=("--mandir=${APP_PREFIX_DOC}/man")
            config_options+=("--htmldir=${APP_PREFIX_DOC}/html")
            config_options+=("--pdfdir=${APP_PREFIX_DOC}/pdf")
          elif [ "${name_suffix}" == "-nano" ]
          then
            config_options+=("--prefix=${APP_PREFIX_NANO}")
          else
            echo "Unsupported name_suffix '${name_suffix}'"
            exit 1
          fi

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${GCC_TARGET}")

          config_options+=("--disable-libgomp") # ABE
          config_options+=("--disable-libmudflap") # ABE
          config_options+=("--disable-libquadmath") # ABE
          config_options+=("--disable-libsanitizer") # ABE
          config_options+=("--disable-libssp") # ABE

          config_options+=("--disable-nls") # Arm, AArch64
          config_options+=("--disable-shared") # Arm, AArch64
          config_options+=("--disable-threads") # Arm, AArch64
          config_options+=("--disable-tls") # Arm, AArch64

          config_options+=("--enable-checking=release") # Arm, AArch64
          config_options+=("--enable-languages=c,c++,fortran") # Arm, AArch64

          if [ "${TARGET_PLATFORM}" == "win32" ]
          then
            config_options+=("--enable-mingw-wildcard")
          fi

          config_options+=("--with-gmp=${LIBS_INSTALL_FOLDER_PATH}") # AArch64

          config_options+=("--with-newlib") # Arm, AArch64
          config_options+=("--with-pkgversion=${BRANDING}")

          config_options+=("--with-gnu-as") # Arm ABE
          config_options+=("--with-gnu-ld") # Arm ABE

          config_options+=("--with-system-zlib")

          # `${with_sysroot}${native_system_header_dir}/stdio.h`
          # is checked for presence; if not present `inhibit_libc=true` and
          # libgcov.a is compiled with empty functions.
          # https://github.com/xpack-dev-tools/arm-none-eabi-gcc-xpack/issues/1
          config_options+=("--with-sysroot=${APP_PREFIX}/${GCC_TARGET}")
          config_options+=("--with-native-system-header-dir=/include")

          if [ "${GCC_TARGET}" == "arm-none-eabi" ]
          then
            config_options+=("--disable-libatomic") # ABE

            if [ "${WITHOUT_MULTILIB}" == "y" ]
            then
              config_options+=("--disable-multilib")
            else
              config_options+=("--enable-multilib") # Arm
              config_options+=("--with-multilib-list=${GCC_MULTILIB_LIST}")  # Arm
            fi
          elif [ "${GCC_TARGET}" == "riscv-none-elf" ]
          then
            config_options+=("--with-abi=${GCC_ABI}")
            config_options+=("--with-arch=${GCC_ARCH}")
           config_options+=("--with-isa-spec=2.2")

            if [ "${WITHOUT_MULTILIB}" == "y" ]
            then
              config_options+=("--disable-multilib")
            else
              config_options+=("--enable-multilib")
            fi
          fi

          # Practically the same.
          if [ "${name_suffix}" == "" ]
          then

            # 11.2-2022.02-darwin-x86_64-arm-none-eabi-manifest.txt:
            # gcc2_configure='--target=arm-none-eabi
            # --prefix=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-arm-none-eabi/install//
            # --with-gmp=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-arm-none-eabi/host-tools
            # --with-mpfr=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-arm-none-eabi/host-tools
            # --with-mpc=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-arm-none-eabi/host-tools
            # --with-isl=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-arm-none-eabi/host-tools
            # --disable-shared --disable-nls --disable-threads --disable-tls
            # --enable-checking=release --enable-languages=c,c++,fortran
            # --with-newlib --with-multilib-list=aprofile,rmprofile'

            # 11.2-2022.02-darwin-x86_64-aarch64-none-elf-manifest.txt
            # gcc2_configure='--target=aarch64-none-elf
            # --prefix=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-aarch64-none-elf/install//
            # --with-gmp=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-aarch64-none-elf/host-tools
            # --with-mpfr=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-aarch64-none-elf/host-tools
            # --with-mpc=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-aarch64-none-elf/host-tools
            # --with-isl=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-aarch64-none-elf/host-tools
            # --disable-shared --disable-nls --disable-threads --disable-tls
            # --enable-checking=release --enable-languages=c,c++,fortran
            # --with-newlib 			 			 			'

            run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${GCC_SRC_FOLDER_NAME}/configure" \
              "${config_options[@]}"

          elif [ "${name_suffix}" == "-nano" ]
          then

            # 11.2-2022.02-darwin-x86_64-arm-none-eabi-manifest.txt:
            # gcc2_nano_configure='--target=arm-none-eabi
            # --prefix=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-arm-none-eabi/nano_install//
            # --with-gmp=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-arm-none-eabi/host-tools
            # --with-mpfr=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-arm-none-eabi/host-tools
            # --with-mpc=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-arm-none-eabi/host-tools
            # --with-isl=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-arm-none-eabi/host-tools
            # --disable-shared --disable-nls --disable-threads --disable-tls
            # --enable-checking=release --enable-languages=c,c++,fortran
            # --with-newlib --with-multilib-list=aprofile,rmprofile'

            run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${GCC_SRC_FOLDER_NAME}/configure" \
              "${config_options[@]}"

          fi
          cp "config.log" "${LOGS_FOLDER_PATH}/${gcc_final_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${gcc_final_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        # Partial build, without documentation.
        echo
        echo "Running cross gcc${name_suffix} final stage make..."

        if [ "${TARGET_PLATFORM}" != "win32" ]
        then

          # Passing USE_TM_CLONE_REGISTRY=0 via INHIBIT_LIBC_CFLAGS to disable
          # transactional memory related code in crtbegin.o.
          # This is a workaround. Better approach is have a t-* to set this flag via
          # CRTSTUFF_T_CFLAGS

          if [ "${TARGET_PLATFORM}" == "darwin" ]
          then
            if [ "${IS_DEVELOP}" == "y" ]
            then
              run_verbose make -j ${JOBS} INHIBIT_LIBC_CFLAGS="-DUSE_TM_CLONE_REGISTRY=0"
            else
              # Retry, parallel builds do fail, headers are probably
              # used before being installed. For example:
              # fatal error: bits/string_view.tcc: No such file or directory
              run_verbose make -j ${JOBS} INHIBIT_LIBC_CFLAGS="-DUSE_TM_CLONE_REGISTRY=0" \
              || run_verbose make -j ${JOBS} INHIBIT_LIBC_CFLAGS="-DUSE_TM_CLONE_REGISTRY=0" \
              || run_verbose make -j ${JOBS} INHIBIT_LIBC_CFLAGS="-DUSE_TM_CLONE_REGISTRY=0" \
              || run_verbose make -j ${JOBS} INHIBIT_LIBC_CFLAGS="-DUSE_TM_CLONE_REGISTRY=0"
            fi
          else
            run_verbose make -j ${JOBS} INHIBIT_LIBC_CFLAGS="-DUSE_TM_CLONE_REGISTRY=0"
          fi

          # Avoid strip here, it may interfere with patchelf.
          # make install-strip
          run_verbose make install

          if [ "${name_suffix}" == "-nano" ]
          then

            local target_gcc=""
            if [ "${TARGET_PLATFORM}" == "win32" ]
            then
              target_gcc="${GCC_TARGET}-gcc"
            else
              if [ -x "${APP_PREFIX_NANO}/bin/${GCC_TARGET}-gcc" ]
              then
                target_gcc="${APP_PREFIX_NANO}/bin/${GCC_TARGET}-gcc"
              # elif [ -x "${APP_PREFIX}/bin/${GCC_TARGET}-gcc" ]
              # then
              #   target_gcc="${APP_PREFIX}/bin/${GCC_TARGET}-gcc"
              else
                echo "No ${GCC_TARGET}-gcc --print-multi-lib"
                exit 1
              fi
            fi

            # Copy the libraries after appending the `_nano` suffix.
            # Iterate through all multilib names.
            copy_cross_multi_libs \
              "${APP_PREFIX_NANO}/${GCC_TARGET}/lib" \
              "${APP_PREFIX}/${GCC_TARGET}/lib" \
              "${target_gcc}"

            # Copy the nano configured newlib.h file into the location that nano.specs
            # expects it to be.
            mkdir -pv "${APP_PREFIX}/${GCC_TARGET}/include/newlib-nano"
            cp -v -f "${APP_PREFIX_NANO}/${GCC_TARGET}/include/newlib.h" \
              "${APP_PREFIX}/${GCC_TARGET}/include/newlib-nano/newlib.h"

          fi

        else

          # For Windows build only the GCC binaries, the libraries were copied
          # from the Linux build.
          # Parallel builds may fail.
          run_verbose make -j ${JOBS} all-gcc
          # make all-gcc

          # No -strip here.
          run_verbose make install-gcc

          # Strip?

        fi

        if [ "${name_suffix}" == "" ]
        then
          (
            xbb_activate_tex

            # Full build, with documentation.
            if [ "${WITH_PDF}" == "y" ]
            then
              run_verbose make pdf
              run_verbose make install-pdf
            fi

            if [ "${WITH_HTML}" == "y" ]
            then
              run_verbose make html
              run_verbose make install-html
            fi

            if [ "${WITH_PDF}" != "y" -a "${WITH_HTML}" != "y" ]
            then
              run_verbose rm -rf "${APP_PREFIX}/shrare/doc"
            fi
          )
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${gcc_final_folder_name}/make-output-$(ndate).txt"

      if [ "${name_suffix}" == "" ]
      then
        copy_license \
          "${SOURCES_FOLDER_PATH}/${GCC_SRC_FOLDER_NAME}" \
          "gcc-${GCC_VERSION}"
      fi

    )

    touch "${gcc_final_stamp_file_path}"

  else
    echo "Component cross gcc${name_suffix} final stage already installed."
  fi

  if [ "${name_suffix}" == "" ]
  then
    tests_add "test_cross_gcc"
  fi
}

function test_cross_gcc()
{
  (
    if [ -d "xpacks/.bin" ]
    then
      TEST_BIN_PATH="$(pwd)/xpacks/.bin"
    elif [ -d "${APP_PREFIX}/bin" ]
    then
      TEST_BIN_PATH="${APP_PREFIX}/bin"
    else
      echo "Wrong folder."
      exit 1
    fi

    show_libs "${TEST_BIN_PATH}/${GCC_TARGET}-gcc"
    show_libs "${TEST_BIN_PATH}/${GCC_TARGET}-g++"

    if [ "${TARGET_PLATFORM}" != "win32" ]
    then
      show_libs "$(${TEST_BIN_PATH}/${GCC_TARGET}-gcc -print-prog-name=cc1)"
      show_libs "$(${TEST_BIN_PATH}/${GCC_TARGET}-gcc -print-prog-name=cc1plus)"
      show_libs "$(${TEST_BIN_PATH}/${GCC_TARGET}-gcc -print-prog-name=collect2)"
      show_libs "$(${TEST_BIN_PATH}/${GCC_TARGET}-gcc -print-prog-name=lto-wrapper)"
      show_libs "$(${TEST_BIN_PATH}/${GCC_TARGET}-gcc -print-prog-name=lto1)"
    fi

    run_app "${TEST_BIN_PATH}/${GCC_TARGET}-gcc" --help
    run_app "${TEST_BIN_PATH}/${GCC_TARGET}-gcc" -dumpversion
    run_app "${TEST_BIN_PATH}/${GCC_TARGET}-gcc" -dumpmachine
    run_app "${TEST_BIN_PATH}/${GCC_TARGET}-gcc" -print-multi-lib
    run_app "${TEST_BIN_PATH}/${GCC_TARGET}-gcc" -print-search-dirs
    # run_app "${TEST_BIN_PATH}/${GCC_TARGET}-gcc" -dumpspecs | wc -l

    local tmp=$(mktemp /tmp/gcc-test.XXXXX)
    rm -rf "${tmp}"

    mkdir -pv "${tmp}"
    cd "${tmp}"

    if false # [ "${TARGET_PLATFORM}" == "win32" ] && [ -z ${IS_NATIVE_TEST+x} ]
    then
      : # Skip Windows when non native (running on Wine).
    else

      if [ "${GCC_TARGET}" == "arm-none-eabi" ]
      then
        specs="-specs=rdimon.specs"
      elif [ "${GCC_TARGET}" == "aarch64-none-elf" ]
      then
        specs="-specs=rdimon.specs"
      elif [ "${GCC_TARGET}" == "riscv-none-elf" ]
      then
        specs="-specs=semihost.specs"
      else
        specs="-specs=nosys.specs"
      fi

      # Note: __EOF__ is quoted to prevent substitutions here.
      cat <<'__EOF__' > hello.c
#include <stdio.h>

int
main(int argc, char* argv[])
{
  printf("Hello World\n");
}
__EOF__

      run_app "${TEST_BIN_PATH}/${GCC_TARGET}-gcc" -pipe -o hello-c.elf "${specs}" hello.c -v

      run_app "${TEST_BIN_PATH}/${GCC_TARGET}-gcc" -pipe -o hello.c.o -c -flto hello.c
      run_app "${TEST_BIN_PATH}/${GCC_TARGET}-gcc" -pipe -o hello-c-lto.elf "${specs}" -flto -v hello.c.o

      # Note: __EOF__ is quoted to prevent substitutions here.
      cat <<'__EOF__' > hello.cpp
#include <iostream>

int
main(int argc, char* argv[])
{
  std::cout << "Hello World" << std::endl;
}

extern "C" void __sync_synchronize();

void
__sync_synchronize()
{
}
__EOF__

      run_app "${TEST_BIN_PATH}/${GCC_TARGET}-g++" -pipe -o hello-cpp.elf "${specs}" hello.cpp

      run_app "${TEST_BIN_PATH}/${GCC_TARGET}-g++" -pipe -o hello.cpp.o -c -flto hello.cpp
      run_app "${TEST_BIN_PATH}/${GCC_TARGET}-g++" -pipe -o hello-cpp-lto.elf "${specs}" -flto -v hello.cpp.o

      run_app "${TEST_BIN_PATH}/${GCC_TARGET}-g++" -pipe -o hello-cpp-gcov.elf "${specs}" -fprofile-arcs -ftest-coverage -lgcov hello.cpp
    fi

    cd ..
    rm -rf "${tmp}"
  )
}

# -----------------------------------------------------------------------------

# Environment variables:
# GDB_VERSION
# GDB_SRC_FOLDER_NAME
# GDB_ARCHIVE_URL
# GDB_ARCHIVE_NAME
# GDB_PATCH_FILE_NAME

# https://github.com/archlinux/svntogit-community/blob/packages/arm-none-eabi-gdb/trunk/PKGBUILD
# https://github.com/archlinux/svntogit-community/blob/packages/riscv32-elf-gdb/trunk/PKGBUILD

# Called multile times, with and without python support.
# $1="" or $1="-py" or $1="-py3"
function build_cross_gdb()
{
  local name_suffix=${1-''}

  # GDB Text User Interface
  # https://ftp.gnu.org/old-gnu/Manuals/gdb/html_chapter/gdb_19.html#SEC197

  local gdb_folder_name="gdb-${GDB_VERSION}${name_suffix}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${gdb_folder_name}"

  local gdb_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-${gdb_folder_name}-installed"

  if [ ! -f "${gdb_stamp_file_path}" ]
  then

    # Download gdb
    if [ ! -d "${SOURCES_FOLDER_PATH}/${GDB_SRC_FOLDER_NAME}" ]
    then
      cd "${SOURCES_FOLDER_PATH}"

      download_and_extract "${GDB_ARCHIVE_URL}" "${GDB_ARCHIVE_NAME}" \
          "${GDB_SRC_FOLDER_NAME}" "${GDB_PATCH_FILE_NAME}"
    fi
    # exit 1

    (
      mkdir -pv "${BUILD_FOLDER_PATH}/${gdb_folder_name}"
      cd "${BUILD_FOLDER_PATH}/${gdb_folder_name}"

      # To pick up the python lib from XBB
      # xbb_activate_dev
      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_APP}"
      LIBS=""

      # libiconv is used by Python3.
      # export LIBS="-liconv"
      if [ "${TARGET_PLATFORM}" == "win32" ]
      then
        # https://stackoverflow.com/questions/44150871/embeded-python3-6-with-mingw-in-c-fail-on-linking
        # ???
        CPPFLAGS+=" -DPy_BUILD_CORE_BUILTIN=1"

        if [ "${name_suffix}" == "-py" ]
        then
          # Definition required by python-config.sh.
          export GNURM_PYTHON_WIN_DIR="${SOURCES_FOLDER_PATH}/${PYTHON2_SRC_FOLDER_NAME}"
        fi

        # Hack to place the bcrypt library at the end of the list of libraries,
        # to avoid 'undefined reference to BCryptGenRandom'.
        # Using LIBS does not work, the order is important.
        export DEBUGINFOD_LIBS="-lbcrypt"

        # From Arm script.
        LDFLAGS+=" -v -Wl,${XBB_FOLDER_PATH}/mingw/lib/CRT_glob.o"
        # Workaround for undefined reference to `__strcpy_chk' in GCC 9.
        # https://sourceforge.net/p/mingw-w64/bugs/818/
        LIBS="-lssp -liconv"
      elif [ "${TARGET_PLATFORM}" == "darwin" ]
      then
        : # LIBS="-liconv -lncurses"
      elif [ "${TARGET_PLATFORM}" == "linux" ]
      then
        LDFLAGS+=" -Wl,-rpath,${LD_LIBRARY_PATH}"
      fi

      CONFIG_PYTHON_PREFIX=""

      if [ "${name_suffix}" == "-py3" ]
      then
        if [ "${TARGET_PLATFORM}" == "win32" ]
        then
          # The source archive includes only the pyconfig.h.in, which needs
          # to be configured, which is not an easy task. Thus add the file copied
          # from a Windows install.
          cp -v "${helper_folder_path}/extras/python/pyconfig-win-${PYTHON3_VERSION}.h" \
            "${LIBS_INSTALL_FOLDER_PATH}/include/pyconfig.h"
        else
          CONFIG_PYTHON_PREFIX="${APP_PREFIX}"
        fi
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS

      export LDFLAGS
      export LIBS

      export CONFIG_PYTHON_PREFIX

      # python -c 'from distutils import sysconfig;print(sysconfig.PREFIX)'
      # python -c 'from distutils import sysconfig;print(sysconfig.EXEC_PREFIX)'

      # The patch to `gdb/python/python-config.py` uses CONFIG_PYTHON_PREFIX,
      # otherwise the resulting python is not relocatable:
      # Fatal Python error: init_fs_encoding: failed to get the Python codec of the filesystem encoding
      # Python runtime state: core initialized
      # ModuleNotFoundError: No module named 'encodings'

      if [ ! -f "config.status" ]
      then
        (
          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          echo
          echo "Running cross gdb${name_suffix} configure..."

          bash "${SOURCES_FOLDER_PATH}/${GDB_SRC_FOLDER_NAME}/gdb/configure" --help

          # 11.2-2022.02-darwin-x86_64-arm-none-eabi-manifest.txt:
          # gdb_configure='--enable-initfini-array --disable-nls --without-x
          # --disable-gdbtk --without-tcl --without-tk --disable-werror
          # --without-expat --without-libunwind-ia64 --without-lzma
          # --without-babeltrace --without-intel-pt --without-xxhash
          # --without-debuginfod --without-guile --disable-source-highlight
          # --disable-objc-gc --with-python=no --disable-binutils
          # --disable-sim --disable-as --disable-ld --enable-plugins
          # --target=arm-none-eabi --prefix=/ --with-mpfr
          # --with-libmpfr-prefix=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-arm-none-eabi/host-tools
          # --with-libmpfr-type=static
          # --with-libgmp-prefix=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-arm-none-eabi/host-tools
          # --with-libgmp-type=static'

          # 11.2-2022.02-darwin-x86_64-aarch64-none-elf-manifest.txt
          # gdb_configure='--enable-64-bit-bfd
          # --enable-targets=arm-none-eabi,aarch64-none-linux-gnu,aarch64-none-elf
          # --enable-initfini-array --disable-nls --without-x --disable-gdbtk
          # --without-tcl --without-tk --disable-werror --without-expat
          # --without-libunwind-ia64 --without-lzma --without-babeltrace
          # --without-intel-pt --without-xxhash  --without-debuginfod
          # --without-guile --disable-source-highlight --disable-objc-gc
          # --with-python=no --disable-binutils --disable-sim --disable-as
          # --disable-ld --enable-plugins --target=aarch64-none-elf --prefix=/
          # --with-mpfr
          # --with-libmpfr-prefix=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-aarch64-none-elf/host-tools
          # --with-libmpfr-type=static
          # --with-libgmp-prefix=/Volumes/data/jenkins/workspace/GNU-toolchain/arm-11/build-aarch64-none-elf/host-tools
          # --with-libgmp-type=static'

          config_options=()

          config_options+=("--prefix=${APP_PREFIX}")
          config_options+=("--infodir=${APP_PREFIX_DOC}/info")
          config_options+=("--mandir=${APP_PREFIX_DOC}/man")
          config_options+=("--htmldir=${APP_PREFIX_DOC}/html")
          config_options+=("--pdfdir=${APP_PREFIX_DOC}/pdf")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${GCC_TARGET}")

          config_options+=("--program-prefix=${GCC_TARGET}-")
          config_options+=("--program-suffix=${name_suffix}")

          config_options+=("--disable-binutils") # Arm, AArch64
          config_options+=("--disable-as") # Arm, AArch64
          config_options+=("--disable-gdbtk") # Arm, AArch64
          # config_options+=("--disable-gprof")
          config_options+=("--disable-ld") # Arm, AArch64
          config_options+=("--disable-nls") # Arm, AArch64
          config_options+=("--disable-objc-gc") # Arm, AArch64
          config_options+=("--disable-sim") # Arm, AArch64
          config_options+=("--disable-source-highlight") # Arm, AArch64
          config_options+=("--disable-werror") # Arm, AArch64

          config_options+=("--enable-gdb")
          config_options+=("--enable-initfini-array") # Arm, AArch64
          config_options+=("--enable-build-warnings=no")
          config_options+=("--enable-plugins") # Arm, AArch64

          if [ "${GCC_TARGET}" == "aarch64-none-elf" ]
          then
            config_options+=("--enable-64-bit-bfd") # AArch64
            config_options+=("--enable-targets=arm-none-eabi,aarch64-none-linux-gnu,aarch64-none-elf") # AArch64
          fi

          config_options+=("--without-babeltrace") # Arm, AArch64
          config_options+=("--without-debuginfod") # Arm, AArch64
          config_options+=("--without-guile") # Arm, AArch64
          config_options+=("--without-intel-pt") # Arm, AArch64
          config_options+=("--without-libunwind-ia64") # Arm, AArch64
          config_options+=("--without-lzma") # Arm, AArch64
          config_options+=("--without-tcl") # Arm, AArch64
          config_options+=("--without-tk") # Arm, AArch64
          config_options+=("--without-x") # Arm, AArch64
          config_options+=("--without-xxhash") # Arm, AArch64

          config_options+=("--with-expat") # Arm
          config_options+=("--with-gdb-datadir=${APP_PREFIX}/${GCC_TARGET}/share/gdb")

          # No need to, we keep track of paths to shared libraries.
          # Plus that if fails the build:
          # /opt/xbb/bin/ld: /usr/lib/x86_64-linux-gnu/libm-2.27.a(e_log.o): warning: relocation against `_dl_x86_cpu_features' in read-only section `.text'
          # /opt/xbb/bin/ld: /usr/lib/x86_64-linux-gnu/libm-2.27.a(e_pow.o): in function `__ieee754_pow_ifunc':
          # (.text+0x12b2): undefined reference to `_dl_x86_cpu_features'
          # /opt/xbb/bin/ld: /usr/lib/x86_64-linux-gnu/libm-2.27.a(e_exp.o): in function `__ieee754_exp_ifunc':
          # (.text+0x5d2): undefined reference to `_dl_x86_cpu_features'
          # /opt/xbb/bin/ld: /usr/lib/x86_64-linux-gnu/libm-2.27.a(e_log.o): in function `__ieee754_log_ifunc':
          # (.text+0x1602): undefined reference to `_dl_x86_cpu_features'
          # /opt/xbb/bin/ld: warning: creating DT_TEXTREL in a PIE

          # config_options+=("--with-libexpat-type=static") # Arm
          # config_options+=("--with-libgmp-type=static") # Arm, AArch64
          # config_options+=("--with-libmpfr-type=static") # Arm, AArch64

          config_options+=("--with-pkgversion=${BRANDING}")
          config_options+=("--with-system-gdbinit=${APP_PREFIX}/${GCC_TARGET}/lib/gdbinit")
          config_options+=("--with-system-zlib")

          if [ "${name_suffix}" == "-py3" ]
          then
            if [ "${TARGET_PLATFORM}" == "win32" ]
            then
              config_options+=("--with-python=${helper_folder_path}/extras/python/python${PYTHON3_VERSION_MAJOR}-config-win.sh")
            else
              config_options+=("--with-python=${LIBS_INSTALL_FOLDER_PATH}/bin/python3.${PYTHON3_VERSION_MINOR}")
            fi
          else
             config_options+=("--with-python=no")
          fi

          if [ "${TARGET_PLATFORM}" == "win32" ]
          then
            config_options+=("--disable-tui")
          else
            config_options+=("--enable-tui")
          fi

          # Note that all components are disabled, except GDB.
          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${GDB_SRC_FOLDER_NAME}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${gdb_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${gdb_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running cross gdb${name_suffix} make..."

        # Build.
        run_verbose make -j ${JOBS} all-gdb

        # install-strip fails, not only because of readline has no install-strip
        # but even after patching it tries to strip a non elf file
        # strip:.../install/riscv-none-gcc/bin/_inst.672_: file format not recognized

        # The explicit `-gdb` fixes a bug noticed with gdb 12, that builds
        # a defective `as.exe` even if instructed not to do so.
        run_verbose make install-gdb

        if [ "${name_suffix}" == "" ]
        then
          (
            xbb_activate_tex

            if [ "${WITH_PDF}" == "y" ]
            then
              run_verbose make pdf-gdb
              run_verbose make install-pdf-gdb
            fi

            if [ "${WITH_HTML}" == "y" ]
            then
              run_verbose make html-gdb
              run_verbose make install-html-gdb
            fi
          )
        fi

        rm -rfv "${LIBS_INSTALL_FOLDER_PATH}/include/pyconfig.h"

        show_libs "${APP_PREFIX}/bin/${GCC_TARGET}-gdb${name_suffix}"

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${gdb_folder_name}/make-output-$(ndate).txt"

      if [ "${name_suffix}" == "" ]
      then
        copy_license \
          "${SOURCES_FOLDER_PATH}/${GDB_SRC_FOLDER_NAME}" \
          "${gdb_folder_name}"
      fi
    )

    touch "${gdb_stamp_file_path}"
  else
    echo "Component cross gdb${name_suffix} already installed."
  fi

  tests_add "test_cross_gdb${name_suffix}"
}

function test_cross_gdb_py()
{
  test_cross_gdb "-py"
}

function test_cross_gdb_py3()
{
  test_cross_gdb "-py3"
}

function test_cross_gdb()
{
  local suffix=""
  if [ $# -ge 1 ]
  then
    suffix="$1"
  fi

  (
    if [ -d "xpacks/.bin" ]
    then
      TEST_BIN_PATH="$(pwd)/xpacks/.bin"
    elif [ -d "${APP_PREFIX}/bin" ]
    then
      TEST_BIN_PATH="${APP_PREFIX}/bin"
    else
      echo "Wrong folder."
      exit 1
    fi

    show_libs "${TEST_BIN_PATH}/${GCC_TARGET}-gdb${suffix}"

    run_app "${TEST_BIN_PATH}/${GCC_TARGET}-gdb${suffix}" --version
    run_app "${TEST_BIN_PATH}/${GCC_TARGET}-gdb${suffix}" --config

    # This command is known to fail with 'Abort trap: 6' (SIGABRT)
    run_app "${TEST_BIN_PATH}/${GCC_TARGET}-gdb${suffix}" \
      --nh \
      --nx \
      -ex='show language' \
      -ex='set language auto' \
      -ex='quit'

    if [ "${suffix}" == "-py3" ]
    then
      # Show Python paths.
      run_app "${TEST_BIN_PATH}/${GCC_TARGET}-gdb${suffix}" \
        --nh \
        --nx \
        -ex='set pagination off' \
        -ex='python import sys; print(sys.prefix)' \
        -ex='python import sys; import os; print(os.pathsep.join(sys.path))' \
        -ex='quit'
    fi
  )
}

function tidy_up()
{
  (
    xbb_activate

    echo
    echo "# Tidying up..."

    # find: pred.c:1932: launch: Assertion `starting_desc >= 0' failed.
    cd "${APP_PREFIX}"

    find "${APP_PREFIX}" -name "libiberty.a" -exec rm -v '{}' ';'
    find "${APP_PREFIX}" -name '*.la' -exec rm -v '{}' ';'

    if [ "${TARGET_PLATFORM}" == "win32" ]
    then
      find "${APP_PREFIX}" -name "liblto_plugin.a" -exec rm -v '{}' ';'
      find "${APP_PREFIX}" -name "liblto_plugin.dll.a" -exec rm -v '{}' ';'
    fi
  )
}

function strip_libs()
{
  if [ "${WITH_STRIP}" == "y" ]
  then
    (
      xbb_activate

      PATH="${APP_PREFIX}/bin:${PATH}"

      echo
      echo "Stripping libraries..."

      cd "${WORK_FOLDER_PATH}"

      # which "${GCC_TARGET}-objcopy"

      local libs=$(find "${APP_PREFIX}" -name '*.[ao]')
      for lib in ${libs}
      do
        if false
        then
          echo "${GCC_TARGET}-objcopy -R ... ${lib}"
          "${APP_PREFIX}/bin/${GCC_TARGET}-objcopy" -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_pubtypes -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc "${lib}" || true
        else
          echo "[${APP_PREFIX}/bin/${GCC_TARGET}-strip --strip-debug ${lib}]"
          "${APP_PREFIX}/bin/${GCC_TARGET}-strip" --strip-debug "${lib}"
        fi
      done
    )
  fi
}

function final_tunings()
{
  # Create the missing LTO plugin links.
  # For `ar` to work with LTO objects, it needs the plugin in lib/bfd-plugins,
  # but the build leaves it where `ld` needs it. On POSIX, make a soft link.
  if [ "${FIX_LTO_PLUGIN:-}" == "y" ]
  then
    (
      cd "${APP_PREFIX}"

      echo
      if [ "${TARGET_PLATFORM}" == "win32" ]
      then
        echo
        echo "Copying ${LTO_PLUGIN_ORIGINAL_NAME}..."

        mkdir -pv "$(dirname ${LTO_PLUGIN_BFD_PATH})"

        if [ ! -f "${LTO_PLUGIN_BFD_PATH}" ]
        then
          local plugin_path="$(find * -type f -name ${LTO_PLUGIN_ORIGINAL_NAME})"
          if [ ! -z "${plugin_path}" ]
          then
            cp -v "${plugin_path}" "${LTO_PLUGIN_BFD_PATH}"
          else
            echo "${LTO_PLUGIN_ORIGINAL_NAME} not found."
            exit 1
          fi
        fi
      else
        echo
        echo "Creating ${LTO_PLUGIN_ORIGINAL_NAME} link..."

        mkdir -pv "$(dirname ${LTO_PLUGIN_BFD_PATH})"
        if [ ! -f "${LTO_PLUGIN_BFD_PATH}" ]
        then
          local plugin_path="$(find * -type f -name ${LTO_PLUGIN_ORIGINAL_NAME})"
          if [ ! -z "${plugin_path}" ]
          then
            ln -s -v "../../${plugin_path}" "${LTO_PLUGIN_BFD_PATH}"
          else
            echo "${LTO_PLUGIN_ORIGINAL_NAME} not found."
            exit 1
          fi
        fi
      fi
    )
  fi
}

# -----------------------------------------------------------------------------
