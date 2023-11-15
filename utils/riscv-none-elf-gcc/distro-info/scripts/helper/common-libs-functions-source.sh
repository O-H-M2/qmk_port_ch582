# -----------------------------------------------------------------------------
# This file is part of the xPack distribution.
#   (https://xpack.github.io)
# Copyright (c) 2020 Liviu Ionescu.
#
# Permission to use, copy, modify, and/or distribute this software
# for any purpose is hereby granted, under the terms of the MIT license.
# -----------------------------------------------------------------------------

# Helper script used in the xPack build scripts. As the name implies,
# it should contain only functions and should be included with 'source'
# by the build scripts (both native and container).

# -----------------------------------------------------------------------------

function build_zlib()
{
  # http://zlib.net
  # http://zlib.net/fossils/

  # https://archlinuxarm.org/packages/aarch64/zlib/files/PKGBUILD
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=zlib-static
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=zlib-git
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=mingw-w64-zlib

  # 2013-04-28 "1.2.8"
  # 2017-01-15 "1.2.11"
  # 2022-03-27, "1.2.12"

  local zlib_version="$1"

  # The folder name as resulted after being extracted from the archive.
  local zlib_src_folder_name="zlib-${zlib_version}"

  local zlib_archive="${zlib_src_folder_name}.tar.gz"
  local zlib_url="http://zlib.net/fossils/${zlib_archive}"

  # The folder name for build, licenses, etc.
  local zlib_folder_name="${zlib_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${zlib_folder_name}"

  local zlib_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${zlib_folder_name}-installed"
  if [ ! -f "${zlib_stamp_file_path}" ]
  then

    echo
    echo "zlib in-source building"

    if [ ! -d "${LIBS_BUILD_FOLDER_PATH}/${zlib_folder_name}" ]
    then
      cd "${LIBS_BUILD_FOLDER_PATH}"

      download_and_extract "${zlib_url}" "${zlib_archive}" \
        "${zlib_src_folder_name}"

      if [ "${zlib_src_folder_name}" != "${zlib_folder_name}" ]
      then
        mv -v "${zlib_src_folder_name}" "${zlib_folder_name}"
      fi
    fi

    (
      cd "${LIBS_BUILD_FOLDER_PATH}/${zlib_folder_name}"

      xbb_activate_installed_dev

      if [ "${TARGET_PLATFORM}" == "win32" ]
      then
        (
          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          echo
          echo "Running zlib make..."

          # Build.
          run_verbose make -f win32/Makefile.gcc \
            PREFIX=${CROSS_COMPILE_PREFIX}- \
            prefix="${LIBS_INSTALL_FOLDER_PATH}" \
            CFLAGS="${XBB_CFLAGS_NO_W} -Wp,-D_FORTIFY_SOURCE=2 -fexceptions --param=ssp-buffer-size=4"

          run_verbose make -f win32/Makefile.gcc install \
            DESTDIR="${LIBS_INSTALL_FOLDER_PATH}/" \
            INCLUDE_PATH="include" \
            LIBRARY_PATH="lib" \
            BINARY_PATH="bin"

        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${zlib_folder_name}/make-output-$(ndate).txt"
      else

        CPPFLAGS="${XBB_CPPFLAGS}"
        CFLAGS="${XBB_CFLAGS_NO_W}"
        CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

        LDFLAGS="${XBB_LDFLAGS_LIB}"
        if [ "${TARGET_PLATFORM}" == "linux" ]
        then
          LDFLAGS+=" -Wl,-rpath,${LD_LIBRARY_PATH}"
        fi

        export CPPFLAGS
        export CFLAGS
        export CXXFLAGS
        export LDFLAGS

        # No config.status left, use the library.
        if [ ! -f "libz.a" ]
        then
          (
            if [ "${IS_DEVELOP}" == "y" ]
            then
              env | sort
            fi

            echo
            echo "Running zlib configure..."

            if [ "${IS_DEVELOP}" == "y" ]
            then
              run_verbose bash "configure" --help
            fi

            # Hack needed for 1.2.12 on macOS
            export cc="${CC}"
            run_verbose bash ${DEBUG} "configure" \
              --prefix="${LIBS_INSTALL_FOLDER_PATH}"

            cp "configure.log" "${LOGS_FOLDER_PATH}/${zlib_folder_name}/configure-log-$(ndate).txt"
          ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${zlib_folder_name}/configure-output-$(ndate).txt"
        fi

        (
          echo
          echo "Running zlib make..."

          # Build.
          run_verbose make -j ${JOBS}

          if [ "${WITH_TESTS}" == "y" ]
          then
            run_verbose make -j1 test
          fi

          run_verbose make install

        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${zlib_folder_name}/make-output-$(ndate).txt"
      fi

      copy_license \
        "${LIBS_BUILD_FOLDER_PATH}/${zlib_folder_name}" \
        "${zlib_folder_name}"

    )

    (
      test_zlib
    ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${zlib_folder_name}/test-output-$(ndate).txt"

    touch "${zlib_stamp_file_path}"

  else
    echo "Library zlib already installed."
  fi
}

function test_zlib()
{
  (
    xbb_activate

    echo
    echo "Checking the zlib shared libraries..."

    show_libs "${LIBS_INSTALL_FOLDER_PATH}/lib/libz.${SHLIB_EXT}"
  )
}

# -----------------------------------------------------------------------------

function build_gmp()
{
  # https://gmplib.org
  # https://gmplib.org/download/gmp/

  # https://archlinuxarm.org/packages/aarch64/gmp/files/PKGBUILD
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=gmp-hg

  # https://github.com/Homebrew/homebrew-core/blob/master/Formula/gmp.rb

  # 01-Nov-2015 "6.1.0"
  # 16-Dec-2016 "6.1.2"
  # 17-Jan-2020 "6.2.0"
  # 14-Nov-2020, "6.2.1"

  local gmp_version="$1"
  local name_suffix=${2-''}

  # The folder name as resulted after being extracted from the archive.
  local gmp_src_folder_name="gmp-${gmp_version}"

  local gmp_archive="${gmp_src_folder_name}.tar.xz"
  local gmp_url="https://gmplib.org/download/gmp/${gmp_archive}"

  # The folder name for build, licenses, etc.
  local gmp_folder_name="${gmp_src_folder_name}${name_suffix}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${gmp_folder_name}"

  local gmp_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${gmp_folder_name}-installed"
  if [ ! -f "${gmp_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${gmp_url}" "${gmp_archive}" \
      "${gmp_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${gmp_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${gmp_folder_name}"

      if [ -n "${name_suffix}" ]
      then

        CPPFLAGS="${XBB_CPPFLAGS}"
        CFLAGS="${XBB_CFLAGS_NO_W}"
        CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

        LDFLAGS="${XBB_LDFLAGS_LIB}"

      else

        xbb_activate_installed_dev

        # Exceptions used by Arm GCC script and by mingw-w64.
        CPPFLAGS="${XBB_CPPFLAGS} -fexceptions"
        # Test fail with -Ofast, revert to -O2
        CFLAGS="${XBB_CFLAGS_NO_W}"
        CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

        LDFLAGS="${XBB_LDFLAGS_LIB}"
        if [ "${TARGET_PLATFORM}" == "linux" ]
        then
          LDFLAGS+=" -Wl,-rpath,${LD_LIBRARY_PATH}"
        fi

        if [ "${TARGET_PLATFORM}" == "win32" ]
        then
          export CC_FOR_BUILD="${NATIVE_CC}"
        fi

      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      # ABI is mandatory, otherwise configure fails on 32-bit.
      # (see https://gmplib.org/manual/ABI-and-ISA.html)
      if [ "${TARGET_ARCH}" == "x64" -o "${TARGET_ARCH}" == "x32" -o "${TARGET_ARCH}" == "ia32" ]
      then
        export ABI="${TARGET_BITS}"
      fi

      if [ ! -f "config.status" ]
      then
        (
          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          echo
          echo "Running gmp${name_suffix} configure..."

          # ABI is mandatory, otherwise configure fails on 32-bit.
          # (see https://gmplib.org/manual/ABI-and-ISA.html)

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${gmp_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}${name_suffix}")

          if [ -n "${name_suffix}" ]
          then

            config_options+=("--build=${BUILD}")
            config_options+=("--host=${BUILD}")
            config_options+=("--target=${BUILD}")

          else

            config_options+=("--build=${BUILD}")
            config_options+=("--host=${HOST}")
            config_options+=("--target=${TARGET}")

            config_options+=("--enable-cxx")
            # From Arm.
            config_options+=("--enable-fft")

            if [ "${TARGET_PLATFORM}" == "win32" ]
            then
              # mpfr asks for this explicitly during configure.
              # (although the message is confusing)
              config_options+=("--enable-shared")
              config_options+=("--disable-static")
            elif [ "${TARGET_PLATFORM}" == "darwin" ]
            then
              # Enable --with-pic to avoid linking issues with the static library
              config_options+=("--with-pic")
            fi

            if [ "${TARGET_ARCH}" == "ia32" -o "${TARGET_ARCH}" == "arm" ]
            then
              config_options+=("ABI=32")
            fi

          fi

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${gmp_src_folder_name}/configure" \
            "${config_options[@]}"

          if false # [ "${TARGET_PLATFORM}" == "darwin" ] # and clang
          then
            # Disable failing `t-sqrlo` test.
            run_verbose sed -i.bak \
              -e 's| t-sqrlo$(EXEEXT) | |' \
              "tests/mpn/Makefile"
          fi

          cp "config.log" "${LOGS_FOLDER_PATH}/${gmp_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${gmp_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running gmp${name_suffix} make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_TESTS}" == "y" ]
        then
          if [ "${TARGET_PLATFORM}" == "darwin" -a "${TARGET_ARCH}" == "arm64" ]
          then
            # FAIL: t-rand
            :
          else
            run_verbose make -j1 check
          fi
        fi

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${gmp_folder_name}/make-output-$(ndate).txt"

      if [ -z "${name_suffix}" ]
      then
        copy_license \
          "${SOURCES_FOLDER_PATH}/${gmp_src_folder_name}" \
          "${gmp_folder_name}"
      fi

    )

    touch "${gmp_stamp_file_path}"

  else
    echo "Library gmp${name_suffix} already installed."
  fi
}

# Depends on gmp.
function build_mpfr()
{
  # http://www.mpfr.org
  # http://www.mpfr.org/history.html

  # https://archlinuxarm.org/packages/aarch64/mpfr/files/PKGBUILD
  # https://git.archlinux.org/svntogit/packages.git/tree/trunk/PKGBUILD?h=packages/mpfr

  # 6 March 2016 "3.1.4"
  # 7 September 2017 "3.1.6"
  # 31 January 2019 "4.0.2"
  # 10 July 2020 "4.1.0"

  local mpfr_version="$1"
  local name_suffix=${2-''}

  # The folder name as resulted after being extracted from the archive.
  local mpfr_src_folder_name="mpfr-${mpfr_version}"

  local mpfr_archive="${mpfr_src_folder_name}.tar.xz"
  local mpfr_url="http://www.mpfr.org/${mpfr_src_folder_name}/${mpfr_archive}"

  # The folder name for build, licenses, etc.
  local mpfr_folder_name="${mpfr_src_folder_name}${name_suffix}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${mpfr_folder_name}"

  local mpfr_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${mpfr_folder_name}-installed"
  if [ ! -f "${mpfr_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${mpfr_url}" "${mpfr_archive}" \
      "${mpfr_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${mpfr_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${mpfr_folder_name}"

      if [ -n "${name_suffix}" ]
      then

        CPPFLAGS="${XBB_CPPFLAGS}"
        CFLAGS="${XBB_CFLAGS_NO_W}"
        CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

        LDFLAGS="${XBB_LDFLAGS_LIB}"

      else

        xbb_activate_installed_dev

        CPPFLAGS="${XBB_CPPFLAGS}"
        CFLAGS="${XBB_CFLAGS_NO_W}"
        CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

        LDFLAGS="${XBB_LDFLAGS_LIB}"
        if [ "${TARGET_PLATFORM}" == "linux" ]
        then
          LDFLAGS+=" -Wl,-rpath,${LD_LIBRARY_PATH}"
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
          echo "Running mpfr${name_suffix} configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${mpfr_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}${name_suffix}")

          if [ -n "${name_suffix}" ]
          then

            config_options+=("--build=${BUILD}")
            config_options+=("--host=${BUILD}")
            config_options+=("--target=${BUILD}")

          else

            config_options+=("--build=${BUILD}")
            config_options+=("--host=${HOST}")
            config_options+=("--target=${TARGET}")

          fi

          config_options+=("--with-gmp=${LIBS_INSTALL_FOLDER_PATH}${name_suffix}")

          config_options+=("--disable-maintainer-mode")
          config_options+=("--disable-warnings")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${mpfr_src_folder_name}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${mpfr_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${mpfr_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running mpfr${name_suffix} make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose make -j1 check
        fi

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${mpfr_folder_name}/make-output-$(ndate).txt"

      if [ -z "${name_suffix}" ]
      then
        copy_license \
          "${SOURCES_FOLDER_PATH}/${mpfr_src_folder_name}" \
          "${mpfr_folder_name}"
      fi

    )
    touch "${mpfr_stamp_file_path}"

  else
    echo "Library mpfr${name_suffix} already installed."
  fi
}

# Depends on gmp, mpfr.
function build_mpc()
{
  # http://www.multiprecision.org/
  # ftp://ftp.gnu.org/gnu/mpc

  # https://archlinuxarm.org/packages/aarch64/mpc/files/PKGBUILD
  # https://git.archlinux.org/svntogit/packages.git/tree/trunk/PKGBUILD?h=packages/libmpc

  # 20 Feb 2015 "1.0.3"
  # 2018-01-11 "1.1.0"
  # 2020-08 "1.2.0"
  # 2020-10 "1.2.1"

  local mpc_version="$1"
  local name_suffix=${2-''}

  # The folder name as resulted after being extracted from the archive.
  local mpc_src_folder_name="mpc-${mpc_version}"

  local mpc_archive="${mpc_src_folder_name}.tar.gz"
  local mpc_url="ftp://ftp.gnu.org/gnu/mpc/${mpc_archive}"
  if [[ ${mpc_version} =~ 0\.* ]]
  then
    mpc_url="http://www.multiprecision.org/downloads/${mpc_archive}"
  fi

  # The folder name for build, licenses, etc.
  local mpc_folder_name="${mpc_src_folder_name}${name_suffix}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${mpc_folder_name}"

  local mpc_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${mpc_folder_name}-installed"
  if [ ! -f "${mpc_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${mpc_url}" "${mpc_archive}" \
      "${mpc_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${mpc_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${mpc_folder_name}"

      if [ -n "${name_suffix}" ]
      then

        CPPFLAGS="${XBB_CPPFLAGS}"
        CFLAGS="${XBB_CFLAGS_NO_W}"
        CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

        LDFLAGS="${XBB_LDFLAGS_LIB}"

      else

        xbb_activate_installed_dev

        CPPFLAGS="${XBB_CPPFLAGS}"
        CFLAGS="${XBB_CFLAGS_NO_W}"
        CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

        LDFLAGS="${XBB_LDFLAGS_LIB}"
        if [ "${TARGET_PLATFORM}" == "linux" ]
        then
          LDFLAGS+=" -Wl,-rpath,${LD_LIBRARY_PATH}"
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
          echo "Running mpc${name_suffix} configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${mpc_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}${name_suffix}")

          if [ -n "${name_suffix}" ]
          then

            config_options+=("--build=${BUILD}")
            config_options+=("--host=${BUILD}")
            config_options+=("--target=${BUILD}")

          else

            config_options+=("--build=${BUILD}")
            config_options+=("--host=${HOST}")
            config_options+=("--target=${TARGET}")

          fi

          config_options+=("--disable-maintainer-mode")

          config_options+=("--with-gmp=${LIBS_INSTALL_FOLDER_PATH}${name_suffix}")
          config_options+=("--with-mpfr=${LIBS_INSTALL_FOLDER_PATH}${name_suffix}")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${mpc_src_folder_name}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${mpc_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${mpc_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running mpc${name_suffix} make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose make -j1 check
        fi

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${mpc_folder_name}/make-output-$(ndate).txt"

      if [ -z "${name_suffix}" ]
      then
        copy_license \
          "${SOURCES_FOLDER_PATH}/${mpc_src_folder_name}" \
          "${mpc_folder_name}"
      fi

    )
    touch "${mpc_stamp_file_path}"

  else
    echo "Library mpc${name_suffix} already installed."
  fi
}

# Depends on gmp.
function build_isl()
{
  # http://isl.gforge.inria.fr - deprecated
  # https://sourceforge.net/projects/libisl/files/

  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=isl

  # 2015-06-12 "0.15"
  # 2016-01-15 "0.16.1"
  # 2016-12-20 "0.18"
  # 2019-03-26 "0.21"
  # 2020-01-16 "0.22"
  # 2020-11-11 "0.23"
  # 2021-05-01 "0.24"

  local isl_version="$1"
  local name_suffix=${2-''}

  # The folder name as resulted after being extracted from the archive.
  local isl_src_folder_name="isl-${isl_version}"

  local isl_archive="${isl_src_folder_name}.tar.xz"
  if [[ ${isl_version} =~ 0\.1[24]\.* ]]
  then
    isl_archive="${isl_src_folder_name}.tar.gz"
  fi

  local isl_url="https://sourceforge.net/projects/libisl/files/${isl_archive}"

  # The folder name for build, licenses, etc.
  local isl_folder_name="${isl_src_folder_name}${name_suffix}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${isl_folder_name}"

  local isl_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${isl_folder_name}-installed"
  if [ ! -f "${isl_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${isl_url}" "${isl_archive}" \
      "${isl_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${isl_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${isl_folder_name}"

      if [ -n "${name_suffix}" ]
      then

        # Otherwise `configure: error: gmp.h header not found`.`
        CPPFLAGS="${XBB_CPPFLAGS} -I${LIBS_INSTALL_FOLDER_PATH}${name_suffix}/include"
        CFLAGS="${XBB_CFLAGS_NO_W}"
        CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

        LDFLAGS="${XBB_LDFLAGS_LIB}"

      else

        xbb_activate_installed_dev

        CPPFLAGS="${XBB_CPPFLAGS}"
        CFLAGS="${XBB_CFLAGS_NO_W}"
        CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

        LDFLAGS="${XBB_LDFLAGS_LIB}"
        if [ "${TARGET_PLATFORM}" == "linux" ]
        then
          LDFLAGS+=" -Wl,-rpath,${LD_LIBRARY_PATH}"
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
          echo "Running isl${name_suffix} configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${isl_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}${name_suffix}")

          if [ -n "${name_suffix}" ]
          then

            config_options+=("--build=${BUILD}")
            config_options+=("--host=${BUILD}")
            config_options+=("--target=${BUILD}")

          else

            config_options+=("--build=${BUILD}")
            config_options+=("--host=${HOST}")
            config_options+=("--target=${TARGET}")

          fi

          config_options+=("--with-gmp=${LIBS_INSTALL_FOLDER_PATH}${name_suffix}")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${isl_src_folder_name}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${isl_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${isl_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running isl${name_suffix} make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_TESTS}" == "y" ]
        then
          if [ "${TARGET_PLATFORM}" == "linux" -a \
            \( "${TARGET_ARCH}" == "x64" -o "${TARGET_ARCH}" == "x32" -o "${TARGET_ARCH}" == "ia32" \) ] 
          then
            # /Host/Users/ilg/Work/gcc-8.4.0-1/linux-x64/build/libs/isl-0.22/.libs/lt-isl_test_cpp: relocation error: /Host/Users/ilg/Work/gcc-8.4.0-1/linux-x64/build/libs/isl-0.22/.libs/lt-isl_test_cpp: symbol _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm, version GLIBCXX_3.4.21 not defined in file libstdc++.so.6 with link time reference
            # FAIL isl_test_cpp (exit status: 127)
            # /Host/Users/ilg/Work/gcc-8.4.0-1/linux-x32/build/libs/isl-0.22/.libs/lt-isl_test_cpp: relocation error: /Host/Users/ilg/Work/gcc-8.4.0-1/linux-x32/build/libs/isl-0.22/.libs/lt-isl_test_cpp: symbol _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj, version GLIBCXX_3.4.21 not defined in file libstdc++.so.6 with link time reference
            # FAIL isl_test_cpp (exit status: 127)
            # Similar for aarch64
            # FAIL: isl_test_cpp

            run_verbose make -j1 check || true
          else
            run_verbose make -j1 check
          fi
        fi

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${isl_folder_name}/make-output-$(ndate).txt"

      if [ -z "${name_suffix}" ]
      then
        copy_license \
          "${SOURCES_FOLDER_PATH}/${isl_src_folder_name}" \
          "${isl_folder_name}"
      fi

    )
    touch "${isl_stamp_file_path}"

  else
    echo "Library isl${name_suffix} already installed."
  fi
}

function build_zstd()
{
  # Zstandard is a real-time compression algorithm
  # https://facebook.github.io/zstd/
  # https://github.com/facebook/zstd/releases
  # https://github.com/facebook/zstd/archive/v1.4.4.tar.gz
  # https://github.com/facebook/zstd/releases/download/v1.5.0/zstd-1.5.0.tar.gz

  # https://github.com/archlinux/svntogit-packages/blob/packages/zstd/trunk/PKGBUILD
  # https://archlinuxarm.org/packages/aarch64/zstd/files/PKGBUILD

  # https://github.com/Homebrew/homebrew-core/blob/master/Formula/zstd.rb

  # 5 Nov 2019 "1.4.4"
  # 3 Mar 2021 "1.4.9"
  # 14 May 2021 "1.5.0"
  # 20 Jan 2022 "1.5.2"

  local zstd_version="$1"

  # The folder name as resulted after being extracted from the archive.
  local zstd_src_folder_name="zstd-${zstd_version}"

  local zstd_archive="${zstd_src_folder_name}.tar.gz"
  # GitHub release archive.
  local zstd_github_archive="v${zstd_version}.tar.gz"
  local zstd_github_url="https://github.com/facebook/zstd/archive/${zstd_github_archive}"

  # The folder name for build, licenses, etc.
  local zstd_folder_name="${zstd_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${zstd_folder_name}"

  local zstd_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${zstd_folder_name}-installed"
  if [ ! -f "${zstd_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${zstd_github_url}" "${zstd_archive}" \
      "${zstd_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${zstd_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${zstd_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
      if [ "${TARGET_PLATFORM}" == "linux" ]
      then
        LDFLAGS+=" -Wl,-rpath,${LD_LIBRARY_PATH}"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      local build_type
      if [ "${IS_DEBUG}" == "y" ]
      then
        build_type=Debug
      else
        build_type=Release
      fi

      if [ ! -f "CMakeCache.txt" ]
      then
        (
          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          echo
          echo "Running zstd cmake..."

          config_options=()

          config_options+=("-LH")
          config_options+=("-G" "Ninja")

          config_options+=("-DCMAKE_INSTALL_PREFIX=${LIBS_INSTALL_FOLDER_PATH}")
          config_options+=("-DZSTD_BUILD_PROGRAMS=OFF")

          if [ "${WITH_TESTS}" == "y" ]
          then
            config_options+=("-DZSTD_BUILD_TESTS=ON")
          fi

          run_verbose cmake \
            "${config_options[@]}" \
            \
            "${SOURCES_FOLDER_PATH}/${zstd_src_folder_name}/build/cmake"

        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${zstd_folder_name}/cmake-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running zstd build..."

        run_verbose cmake \
          --build . \
          --parallel ${JOBS} \
          --config "${build_type}" \

        # It takes too long.
        if false # [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose ctest \
            -V \

        fi

        (
          # The install procedure runs some resulted executables, which require
          # the libssl and libcrypt libraries from XBB.
          # xbb_activate_libs

          echo
          echo "Running zstd install..."

          run_verbose cmake \
            --build . \
            --config "${build_type}" \
            -- \
            install

        )
      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${zstd_folder_name}/build-output-$(ndate).txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${zstd_src_folder_name}" \
        "${zstd_folder_name}"

      (
        cd "${LIBS_BUILD_FOLDER_PATH}"

        copy_cmake_logs "${zstd_folder_name}"
      )

    )
    touch "${zstd_stamp_file_path}"

  else
    echo "Library zstd already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_libiconv()
{
  # https://www.gnu.org/software/libiconv/
  # https://ftp.gnu.org/pub/gnu/libiconv/

  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=libiconv

  # https://github.com/Homebrew/homebrew-core/blob/master/Formula/libiconv.rb

  # 2011-08-07 1.14"
  # 2017-02-02 "1.15"
  # 2019-04-26 "1.16"
  # 2022-05-15, "1.17"

  local libiconv_version="$1"

  local libiconv_src_folder_name="libiconv-${libiconv_version}"

  local libiconv_archive="${libiconv_src_folder_name}.tar.gz"
  local libiconv_url="https://ftp.gnu.org/pub/gnu/libiconv/${libiconv_archive}"

  local libiconv_folder_name="${libiconv_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${libiconv_folder_name}"

  local libiconv_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${libiconv_folder_name}-installed"
  if [ ! -f "${libiconv_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${libiconv_url}" "${libiconv_archive}" \
      "${libiconv_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${libiconv_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${libiconv_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      # -fgnu89-inline fixes "undefined reference to `aliases2_lookup'"
      #  https://savannah.gnu.org/bugs/?47953
      CFLAGS="${XBB_CFLAGS_NO_W} -fgnu89-inline"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
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
          echo "Running libiconv configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${libiconv_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${libiconv_src_folder_name}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${libiconv_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libiconv_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running libiconv make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose make -j1 check
        fi

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libiconv_folder_name}/make-output-$(ndate).txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${libiconv_src_folder_name}" \
        "${libiconv_folder_name}"

    )

    touch "${libiconv_stamp_file_path}"

  else
    echo "Library libiconv already installed."
  fi
}

# -----------------------------------------------------------------------------

# Could not make it work on Windows.
function build_ncurses()
{
  # https://ftp.gnu.org/gnu/ncurses/
  # https://ftp.gnu.org/gnu/ncurses/ncurses-6.3.tar.gz

  # https://invisible-island.net/ncurses/
  # https://invisible-mirror.net/archives/ncurses/
  # https://invisible-mirror.net/archives/ncurses/ncurses-6.2.tar.gz

  # depends=(glibc gcc-libs)
  # https://archlinuxarm.org/packages/aarch64/ncurses/files/PKGBUILD
  # http://deb.debian.org/debian/pool/main/n/ncurses/ncurses_6.1+20181013.orig.tar.gz.asc

  # https://github.com/msys2/MINGW-packages/blob/master/mingw-w64-ncurses/PKGBUILD
  # https://github.com/msys2/MINGW-packages/blob/master/mingw-w64-ncurses/001-use-libsystre.patch
  # https://github.com/msys2/MSYS2-packages/blob/master/ncurses/PKGBUILD

  # https://github.com/Homebrew/homebrew-core/blob/master/Formula/ncurses.rb

  # _4421.c:1364:15: error: expected ‘)’ before ‘int’
  # ../include/curses.h:1906:56: note: in definition of macro ‘mouse_trafo’
  # 1906 | #define mouse_trafo(y,x,to_screen) wmouse_trafo(stdscr,y,x,to_screen)

  # 26 Feb 2011, "5.8" # build fails
  # 27 Jan 2018, "5.9" # build fails
  # 27 Jan 2018, "6.1"
  # 12 Feb 2020, "6.2"
  # 2021-11-08, "6.3"

  local ncurses_version="$1"
  local ncurses_version_major="$(echo ${ncurses_version} | sed -e 's|\([0-9][0-9]*\)\.\([0-9][0-9]*\)|\1|')"
  local ncurses_version_minor="$(echo ${ncurses_version} | sed -e 's|\([0-9][0-9]*\)\.\([0-9][0-9]*\)|\2|')"

  # The folder name as resulted after being extracted from the archive.
  local ncurses_src_folder_name="ncurses-${ncurses_version}"

  local ncurses_archive="${ncurses_src_folder_name}.tar.gz"
  # local ncurses_url="https://invisible-mirror.net/archives/ncurses/${ncurses_archive}"
  local ncurses_url="https://ftp.gnu.org/gnu/ncurses/${ncurses_archive}"

  # The folder name  for build, licenses, etc.
  local ncurses_folder_name="${ncurses_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${ncurses_folder_name}"

  local ncurses_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${ncurses_folder_name}-installed"
  if [ ! -f "${ncurses_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${ncurses_url}" "${ncurses_archive}" \
      "${ncurses_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${ncurses_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${ncurses_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
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
          # 6.3 fails with
          # configure: error: expected a pathname, not ""
          export PKG_CONFIG_LIBDIR="no"

          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          echo
          echo "Running ncurses configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${ncurses_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          # Not yet functional on windows.
          if [ "${TARGET_PLATFORM}" == "win32" ]
          then

            # The build passes, but generally it is not expected to be
            # used on Windows.

            # export PATH_SEPARATOR=";"

            # --with-libtool \
            # /opt/xbb/bin/libtool: line 10548: gcc-8bs: command not found

            # Without --with-pkg-config-libdir= it'll try to write the .pc files in the
            # xbb folder, probbaly by using the dirname of pkg-config.

            config_options+=("--with-build-cc=${NATIVE_CC}")
            config_options+=("--with-build-cflags=${CFLAGS}")
            config_options+=("--with-build-cppflags=${CPPFLAGS}")
            config_options+=("--with-build-ldflags=${LDFLAGS}")

            config_options+=("--without-progs")

            # Only for the MinGW port, it provides a way to substitute
            # the low-level terminfo library with different terminal drivers.
            config_options+=("--enable-term-driver")

            config_options+=("--disable-termcap")
            config_options+=("--disable-home-terminfo")
            config_options+=("--disable-db-install")

          else

            # Without --with-pkg-config-libdir= it'll try to write the .pc files in the
            # xbb folder, probbaly by using the dirname of pkg-config.

            config_options+=("--with-terminfo-dirs=/etc/terminfo")
            config_options+=("--with-default-terminfo-dir=/etc/terminfo:/lib/terminfo:/usr/share/terminfo")
            config_options+=("--with-gpm")
            config_options+=("--with-versioned-syms")
            config_options+=("--with-xterm-kbs=del")

            config_options+=("--enable-termcap")
            config_options+=("--enable-const")
            config_options+=("--enable-symlinks")

            # HomeBrew
            config_options+=("--enable-sigwinch")

          fi

          config_options+=("--with-shared")
          config_options+=("--with-normal")
          config_options+=("--with-cxx")
          config_options+=("--with-cxx-binding")
          config_options+=("--with-cxx-shared")
          config_options+=("--with-pkg-config-libdir=${LIBS_INSTALL_FOLDER_PATH}/lib/pkgconfig")

          # Fails on Linux, with missing _nc_cur_term, which is there.
          config_options+=("--without-pthread")

          config_options+=("--without-ada")
          config_options+=("--without-debug")
          config_options+=("--without-manpages")
          config_options+=("--without-tack")
          config_options+=("--without-tests")

          config_options+=("--enable-pc-files")
          config_options+=("--enable-sp-funcs")
          config_options+=("--enable-ext-colors")
          config_options+=("--enable-interop")

          # Do not use, it disables the wide libraries.
          # config_options+=("--disable-lib-suffixes")

          # config_options+=("--disable-overwrite")

          NCURSES_DISABLE_WIDEC=${NCURSES_DISABLE_WIDEC:-""}

          if [ "${NCURSES_DISABLE_WIDEC}" == "y" ]
          then
            config_options+=("--disable-widec")
          else
            config_options+=("--enable-widec")
          fi

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${ncurses_src_folder_name}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${ncurses_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${ncurses_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running ncurses make..."

        # Build.
        run_verbose make -j ${JOBS}

        # The test-programs are interactive

        # Has no install-strip
        run_verbose make install

        # Expose the library to pkg_config also as `curses`.
        if [ -f "${LIBS_INSTALL_FOLDER_PATH}/lib/pkgconfig/ncurses.pc" ]
        then
          cat "${LIBS_INSTALL_FOLDER_PATH}/lib/pkgconfig/ncurses.pc" | \
            sed -e 's|Name: ncurses|Name: curses|' \
            > "${LIBS_INSTALL_FOLDER_PATH}/lib/pkgconfig/curses.pc"
        fi

        if [ -f "${LIBS_INSTALL_FOLDER_PATH}/lib/pkgconfig/ncurses++.pc" ]
        then
          cat "${LIBS_INSTALL_FOLDER_PATH}/lib/pkgconfig/ncurses++.pc" | \
            sed -e 's|Name: ncurses++|Name: curses++|' \
            > "${LIBS_INSTALL_FOLDER_PATH}/lib/pkgconfig/curses++.pc"
        fi

        if [ -f "${LIBS_INSTALL_FOLDER_PATH}/lib/libncurses.${SHLIB_EXT}" ]
        then
          ln -sfv libncurses.${SHLIB_EXT} "${LIBS_INSTALL_FOLDER_PATH}/lib/libcurses.${SHLIB_EXT}"
        fi

        if [ -f "${LIBS_INSTALL_FOLDER_PATH}/lib/libncurses.a" ]
        then
          ln -sfv libncurses.a "${LIBS_INSTALL_FOLDER_PATH}/lib/libcurses.a"
        fi

        if [ -f "${LIBS_INSTALL_FOLDER_PATH}/lib/libncurses++.${SHLIB_EXT}" ]
        then
          ln -sfv libncurses++.${SHLIB_EXT} "${LIBS_INSTALL_FOLDER_PATH}/lib/libcurses++.${SHLIB_EXT}"
        fi

        if [ -f "${LIBS_INSTALL_FOLDER_PATH}/lib/libncurses++.a" ]
        then
          ln -sfv libncurses++.a "${LIBS_INSTALL_FOLDER_PATH}/lib/libcurses++.a"
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${ncurses_folder_name}/make-output-$(ndate).txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${ncurses_src_folder_name}" \
        "${ncurses_folder_name}"

    )

    touch "${ncurses_stamp_file_path}"

  else
    echo "Library ncurses already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_libffi()
{
  # http://www.sourceware.org/libffi/
  # ftp://sourceware.org/pub/libffi/
  # https://github.com/libffi/libffi
  # https://github.com/libffi/libffi/releases
  # https://github.com/libffi/libffi/archive/v3.2.1.tar.gz

  # https://archlinuxarm.org/packages/aarch64/libffi/files/PKGBUILD
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=libffi-git
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=mingw-w64-libffi

  # https://github.com/Homebrew/homebrew-core/blob/master/Formula/libffi.rb

  # 12-Nov-2014, "3.2.1"
  # 23 Nov 2019, "3.3"
  # 29 Jun 2021, "3.4.2"

  local libffi_version="$1"

  local libffi_src_folder_name="libffi-${libffi_version}"

  local libffi_archive="${libffi_src_folder_name}.tar.gz"
  # GitHub release archive.
  local libffi_github_archive="v${libffi_version}.tar.gz"
  local libffi_github_url="https://github.com/libffi/libffi/archive/${libffi_github_archive}"

  local libffi_folder_name="${libffi_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${libffi_folder_name}"

  local libffi_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-${libffi_folder_name}-installed"
  if [ ! -f "${libffi_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${libffi_github_url}" "${libffi_archive}" \
      "${libffi_src_folder_name}"

    (
      if [ ! -x "${SOURCES_FOLDER_PATH}/${libffi_src_folder_name}/configure" ]
      then

        cd "${SOURCES_FOLDER_PATH}/${libffi_src_folder_name}"

        xbb_activate_installed_dev

        run_verbose bash ${DEBUG} "autogen.sh"

      fi
    ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libffi_folder_name}/autogen-output-$(ndate).txt"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${libffi_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${libffi_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
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
          echo "Running libffi configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${libffi_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          config_options+=("--enable-pax_emutramp")

          # --enable-pax_emutramp is inspired by AUR
          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${libffi_src_folder_name}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${libffi_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libffi_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running libffi make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose make -j1 check
        fi

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libffi_folder_name}/make-output-$(ndate).txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${libffi_src_folder_name}" \
        "${libffi_folder_name}"

    )

    touch "${libffi_stamp_file_path}"

  else
    echo "Library libffi already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_gettext()
{
  # https://www.gnu.org/software/gettext/
  # http://ftp.gnu.org/pub/gnu/gettext/

  # https://archlinuxarm.org/packages/aarch64/gettext/files/PKGBUILD
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=gettext-git
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=mingw-w64-gettext

  # https://github.com/Homebrew/homebrew-core/blob/master/Formula/gettext.rb

  # 2015-07-14 "0.19.5.1"
  # 2016-06-11 "0.19.8.1"
  # 2020-04-14 "0.20.2"
  # 2020-07-26 "0.21"

  local gettext_version="$1"

  local gettext_src_folder_name="gettext-${gettext_version}"

  local gettext_archive="${gettext_src_folder_name}.tar.gz"
  local gettext_url="http://ftp.gnu.org/pub/gnu/gettext/${gettext_archive}"

  local gettext_folder_name="${gettext_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${gettext_folder_name}"

  local gettext_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-${gettext_folder_name}-installed"
  if [ ! -f "${gettext_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${gettext_url}" "${gettext_archive}" \
      "${gettext_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${gettext_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${gettext_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
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
          echo "Running gettext configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${gettext_src_folder_name}/gettext-runtime/configure" --help
          fi

          # Build only the /gettext-runtime folder, attempts to build
          # the full package fail with a CXX='no' problem.
          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          if [ "${TARGET_PLATFORM}" == "win32" ]
          then
            config_options+=("--enable-threads=windows")
            config_options+=("--with-gnu-ld")
          elif [ "${TARGET_PLATFORM}" == "linux" ]
          then
            config_options+=("--enable-threads=posix")
            config_options+=("--with-gnu-ld")
          elif [ "${TARGET_PLATFORM}" == "darwin" ]
          then
            config_options+=("--enable-threads=posix")
          fi

          config_options+=("--disable-installed-tests")
          config_options+=("--disable-always-build-tests")
          # config_options+=("--enable-nls")
          config_options+=("--disable-java")
          config_options+=("--disable-native-java")
          config_options+=("--disable-c++")
          config_options+=("--disable-libasprintf")

          #  --enable-nls needed to include libintl
          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${gettext_src_folder_name}/gettext-runtime/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${gettext_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${gettext_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running gettext make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_TESTS}" == "y" ]
        then
          make -j1 check # || true
        fi

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${gettext_folder_name}/make-output-$(ndate).txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${gettext_src_folder_name}" \
        "${gettext_folder_name}"

    )

    touch "${gettext_stamp_file_path}"

  else
    echo "Library gettext already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_libelf()
{
  # https://sourceware.org/elfutils/
  # ftp://sourceware.org/pub/elfutils/
  # ftp://sourceware.org/pub/elfutils//0.178/elfutils-0.178.tar.bz2

  # https://archlinuxarm.org/packages/aarch64/libelf/files/PKGBUILD

  # libelf_version="0.8.13" (deprecated)
  # 26 Nov 2019, 0.178
  # 2020-03-30, 0.179
  # 2020-06-11, 0.180
  # 2020-09-08, 0.181
  # 2020-10-31, 0.182
  # 2021-02-07, "0.183"
  # 2021-05-10, "0.184"

  local libelf_version="$1"

  local libelf_src_folder_name="libelf-${libelf_version}"
  local libelf_archive="${libelf_src_folder_name}.tar.gz"

  # local libelf_url="http://www.mr511.de/software/${libelf_archive}"
  # The original site seems unavailable, use a mirror.
  local libelf_url="https://fossies.org/linux/misc/old/${libelf_archive}"

  local libelf_folder_name="${libelf_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${libelf_folder_name}"

  local libelf_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-${libelf_folder_name}-installed"
  if [ ! -f "${libelf_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${libelf_url}" "${libelf_archive}" \
      "${libelf_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${libelf_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${libelf_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
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
          echo "Running libelf configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${libelf_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          # config_options+=("--disable-nls")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${libelf_src_folder_name}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${libelf_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libelf_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running libelf make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose make -j1 check
        fi

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libelf_folder_name}/make-output-$(ndate).txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${libelf_src_folder_name}" \
        "${libelf_folder_name}"

    )

    touch "${libelf_stamp_file_path}"

  else
    echo "Library libelf already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_expat()
{
  # https://libexpat.github.io
  # https://github.com/libexpat/libexpat/releases

  # https://archlinuxarm.org/packages/aarch64/expat/files/PKGBUILD
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=expat-git

  # Oct 21, 2017 "2.1.1"
  # Nov 1, 2017 "2.2.5"
  # 26 Sep 2019 "2.2.9"
  # 3 Oct 2020, "2.2.10"
  # 25 Mar 2021 "2.3.0"
  # 23 May 2021, "2.4.1"
  # 29 Mar 2022, "2.4.8"

  local expat_version="$1"

  local expat_src_folder_name="expat-${expat_version}"
  local expat_archive="${expat_src_folder_name}.tar.bz2"
  if [[ ${expat_version} =~ 2\.0\.* ]]
  then
    expat_archive="${expat_src_folder_name}.tar.gz"
  fi

  local expat_release="R_$(echo ${expat_version} | sed -e 's|[.]|_|g')"
  local expat_url="https://github.com/libexpat/libexpat/releases/download/${expat_release}/${expat_archive}"

  local expat_folder_name="${expat_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${expat_folder_name}"

  local expat_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-${expat_folder_name}-installed"
  if [ ! -f "${expat_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${expat_url}" "${expat_archive}" \
      "${expat_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${expat_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${expat_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
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
          echo "Running expat configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${expat_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          config_options+=("--without-docbook")
          config_options+=("--without-xmlwf")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${expat_src_folder_name}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${expat_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${expat_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running expat make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose make -j1 check
        fi

        # Has no install-strip
        run_verbose make install

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${expat_folder_name}/make-output-$(ndate).txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${expat_src_folder_name}" \
        "${expat_folder_name}"

    )

    touch "${expat_stamp_file_path}"

  else
    echo "Library expat already installed."
  fi
}


# -----------------------------------------------------------------------------

function build_xz()
{
  # https://tukaani.org/xz/
  # https://sourceforge.net/projects/lzmautils/files/

  # https://archlinuxarm.org/packages/aarch64/xz/files/PKGBUILD
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=xz-git

  # 2016-12-30 "5.2.3"
  # 2018-04-29 "5.2.4"
  # 2020-03-17 "5.2.5"
  # 2022-08-12 "5.2.6"

  local xz_version="$1"

  local xz_src_folder_name="xz-${xz_version}"
  local xz_archive="${xz_src_folder_name}.tar.xz"
  local xz_url="https://sourceforge.net/projects/lzmautils/files/${xz_archive}"

  local xz_folder_name="${xz_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${xz_folder_name}"

  local xz_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-${xz_folder_name}-installed"
  if [ ! -f "${xz_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${xz_url}" "${xz_archive}" \
      "${xz_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${xz_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${xz_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
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
          echo "Running xz configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${xz_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          # config_options+=("--disable-nls")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${xz_src_folder_name}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${xz_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${xz_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running xz make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose make -j1 check
        fi

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${xz_folder_name}/make-output-$(ndate).txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${xz_src_folder_name}" \
        "${xz_folder_name}"

    )

    touch "${xz_stamp_file_path}"

  else
    echo "Library xz already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_gpm()
{
  # General purpose mouse. Used by ncurses.
  # https://www.nico.schottelius.org/software/gpm/
  # https://github.com/telmich/gpm
  # https://github.com/telmich/gpm/tags
  # https://github.com/telmich/gpm/releases/tag/1.20.7
  # https://github.com/telmich/gpm/archive/1.20.7.tar.gz
  # https://github.com/xpack-dev-tools/gpm/archive/refs/tags/v1.20.7-1.tar.gz

  # https://archlinuxarm.org/packages/aarch64/gpm/files/PKGBUILD

  # 27 Oct 2012 "1.20.7"
  # 25 Apr 2022 "1.20.7-1" xPack

  local gpm_version="$1"

  local gpm_src_folder_name="gpm-${gpm_version}"

  local gpm_archive="${gpm_src_folder_name}.tar.gz"
  # GitHub release archive.
  local gpm_github_archive="${gpm_version}.tar.gz"
  # local gpm_github_url="https://github.com/telmich/gpm/archive/${gpm_github_archive}"
  local gpm_github_url="https://github.com/xpack-dev-tools/gpm/archive/refs/tags/v${gpm_github_archive}"

  local gpm_folder_name="${gpm_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${gpm_folder_name}"

  local gpm_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-${gpm_folder_name}-installed"
  if [ ! -f "${gpm_stamp_file_path}" ]
  then

    echo
    echo "gmp in-source building"

    cd "${LIBS_BUILD_FOLDER_PATH}"

    if [ ! -d "${LIBS_BUILD_FOLDER_PATH}/${gpm_folder_name}" ]
    then
      cd "${LIBS_BUILD_FOLDER_PATH}"

      download_and_extract "${gpm_github_url}" "${gpm_archive}" \
        "${gpm_src_folder_name}"

      if [ "${gpm_src_folder_name}" != "${gpm_folder_name}" ]
      then
        mv -v "${gpm_src_folder_name}" "${gpm_folder_name}"
      fi
    fi

    (
      cd "${LIBS_BUILD_FOLDER_PATH}/${gpm_folder_name}"
      if [ ! -f "stamp-autogen" ]
      then

        run_verbose bash ${DEBUG} "autogen.sh"

        touch "stamp-autogen"
      fi
    ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${gpm_folder_name}/autogen-output-$(ndate).txt"

    (
      cd "${LIBS_BUILD_FOLDER_PATH}/${gpm_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS} -I${LIBS_BUILD_FOLDER_PATH}/${gpm_folder_name}/src/headers"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
      if [ "${TARGET_PLATFORM}" == "linux" ]
      then
        LDFLAGS+=" -Wl,-rpath,${LD_LIBRARY_PATH}"
        LDFLAGS+=" -Wl,--allow-multiple-definition"
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
          echo "Running gpm configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          # config_options+=("--with-pic")

          run_verbose bash ${DEBUG} "configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${gpm_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${gpm_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running gpm make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose make -j1 check
        fi

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

        if [ "${TARGET_PLATFORM}" == "linux" ]
        then
          (
            mkdir -pv "${APP_PREFIX}/bin"
            cd "${APP_PREFIX}/bin"

            # Manual copy, since it is not refered in the elf.
            cp -v "${LIBS_INSTALL_FOLDER_PATH}/lib/libgpm.so.2.1.0" .
            rm -f "libgpm.so.2"
            ln -s -v "libgpm.so.2.1.0" "libgpm.so.2"
          )
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${gpm_folder_name}/make-output-$(ndate).txt"

      copy_license \
        "${LIBS_BUILD_FOLDER_PATH}/${gpm_folder_name}" \
        "${gpm_folder_name}"
    )

    touch "${gpm_stamp_file_path}"

  else
    echo "Library gpm already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_libmpdec()
{
  # http://www.bytereef.org/mpdecimal/index.html
  # https://www.bytereef.org/mpdecimal/download.html
  # https://www.bytereef.org/software/mpdecimal/releases/mpdecimal-2.4.2.tar.gz

  # https://archlinuxarm.org/packages/aarch64/mpdecimal/files/PKGBUILD

  # 2016-02-28, "2.4.2"
  # 2020-06-28, "2.5.0"
  # 2021-01-28, "2.5.1"

  local libmpdec_version="$1"

  local libmpdec_src_folder_name="mpdecimal-${libmpdec_version}"

  local libmpdec_archive="${libmpdec_src_folder_name}.tar.gz"
  local libmpdec_url="https://www.bytereef.org/software/mpdecimal/releases/${libmpdec_archive}"

  local libmpdec_folder_name="${libmpdec_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${libmpdec_folder_name}"

  local libmpdec_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${libmpdec_folder_name}-installed"
  if [ ! -f "${libmpdec_stamp_file_path}" ]
  then

    echo
    echo "libmpdec in-source building"

    if [ ! -d "${LIBS_BUILD_FOLDER_PATH}/${libmpdec_folder_name}" ]
    then
      cd "${LIBS_BUILD_FOLDER_PATH}"

      download_and_extract "${libmpdec_url}" "${libmpdec_archive}" \
        "${libmpdec_src_folder_name}"

      if [ "${libmpdec_src_folder_name}" != "${libmpdec_folder_name}" ]
      then
        mv -v "${libmpdec_src_folder_name}" "${libmpdec_folder_name}"
      fi
    fi

    (
      cd "${LIBS_BUILD_FOLDER_PATH}/${libmpdec_src_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
      if [ "${TARGET_PLATFORM}" == "linux" ]
      then
        LDFLAGS+=" -Wl,-rpath,${LD_LIBRARY_PATH}"
      fi

      LD=${CC}

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS
      export LD

      if [ ! -f "config.status" ]
      then
        (
          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          echo
          echo "Running libmpdec configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          # C++ tests fail on Linux.
          # config_options+=("--enable-cxx")
          config_options+=("--disable-cxx")

          run_verbose bash ${DEBUG} "configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${libmpdec_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libmpdec_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running libmpdec make..."

        # Build.
        run_verbose make -j ${JOBS}

        # Has no install-strip
        run_verbose make install

        if [ "${WITH_TESTS}" == "y" ]
        then
          if [ "${TARGET_PLATFORM}" == "linux" ]
          then
            # TODO
            # Fails shared on darwin
            run_verbose make -j1 check
          fi
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libmpdec_folder_name}/make-output-$(ndate).txt"

      copy_license \
        "${LIBS_BUILD_FOLDER_PATH}/${libmpdec_folder_name}" \
        "${libmpdec_folder_name}"

    )

    touch "${libmpdec_stamp_file_path}"

  else
    echo "Library libmpdec already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_libxcrypt()
{
  # Replacement for the old libcrypt.so.1.

  # https://github.com/besser82/libxcrypt
  # https://github.com/besser82/libxcrypt/archive/v4.4.15.tar.gz

  # 26 Jul 2018, "4.1.1"
  # 26 Oct 2018, "4.2.3"
  # 14 Nov 2018, "4.3.4"
  # Requires new autotools.
  # m4/ax_valgrind_check.m4:80: warning: macro `AM_EXTRA_RECURSIVE_TARGETS' not found in library
  # Feb 25 2020, "4.4.15"
  # 23 Aug 2020, "4.4.17"
  # 1 May 2021, "4.4.20"
  # 18 Sep 2021, "4.4.26"
  # 02 Feb 2022, "4.4.28"

  local libxcrypt_version="$1"

  local libxcrypt_src_folder_name="libxcrypt-${libxcrypt_version}"

  local libxcrypt_archive="${libxcrypt_src_folder_name}.tar.gz"
  # GitHub release archive.
  local libxcrypt_github_archive="v${libxcrypt_version}.tar.gz"
  local libxcrypt_github_url="https://github.com/besser82/libxcrypt/archive/${libxcrypt_github_archive}"

  local libxcrypt_folder_name="${libxcrypt_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${libxcrypt_folder_name}"

  local libxcrypt_patch_file_path="${libxcrypt_folder_name}.patch"
  local libxcrypt_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${libxcrypt_folder_name}-installed"
  if [ ! -f "${libxcrypt_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    set +e
    download_and_extract "${libxcrypt_github_url}" "${libxcrypt_archive}" \
      "${libxcrypt_src_folder_name}" "${libxcrypt_patch_file_path}"
    set -e

    if [ ! -x "${SOURCES_FOLDER_PATH}/${libxcrypt_src_folder_name}/configure" ]
    then
      (
        cd "${SOURCES_FOLDER_PATH}/${libxcrypt_src_folder_name}"

        xbb_activate_installed_dev

        if [ -f "autogen.sh" ]
        then
          run_verbose bash ${DEBUG} autogen.sh
        elif [ -f "bootstrap" ]
        then
          run_verbose bash ${DEBUG} bootstrap
        else
          #
          run_verbose autoreconf -fiv
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libxcrypt_folder_name}/autogen-output-$(ndate).txt"

    fi

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${libxcrypt_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${libxcrypt_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
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
          echo "Running libxcrypt configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${libxcrypt_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${libxcrypt_src_folder_name}/configure" \
            "${config_options[@]}"

          # patch_all_libtool_rpath

          cp "config.log" "${LOGS_FOLDER_PATH}/${libxcrypt_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libxcrypt_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running libxcrypt make..."

        # Build.
        run_verbose make -j ${JOBS}

        # install is not able to rewrite them.
        rm -rfv "${LIBS_INSTALL_FOLDER_PATH}"/lib*/libxcrypt.*
        rm -rfv "${LIBS_INSTALL_FOLDER_PATH}"/lib*/libowcrypt.*
        rm -rfv "${LIBS_INSTALL_FOLDER_PATH}"/lib/pkgconfig/libcrypt.pc

        # make install-strip
        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

        if [ "${WITH_TESTS}" == "y" ]
        then
          if [ "${TARGET_PLATFORM}" == "darwin" ]
          then
            # macOS FAIL: test/symbols-static.sh
            # macOS FAIL: test/symbols-renames.sh
            run_verbose make -j1 check || true
          else
            run_verbose make -j1 check
          fi
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libxcrypt_folder_name}/make-output-$(ndate).txt"
    )

    (
      test_libxcrypt
    ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libxcrypt_folder_name}/test-output-$(ndate).txt"

    touch "${libxcrypt_stamp_file_path}"

  else
    echo "Library libxcrypt already installed."
  fi
}

function test_libxcrypt()
{
  (
    xbb_activate

    echo
    echo "Checking the libxcrypt shared libraries..."

    show_libs "${LIBS_INSTALL_FOLDER_PATH}/lib/libcrypt.${SHLIB_EXT}"
  )
}

# -----------------------------------------------------------------------------

function build_openssl()
{
  # https://www.openssl.org
  # https://www.openssl.org/source/

  # https://www.openssl.org/source/openssl-1.1.1n.tar.gz

  # https://archlinuxarm.org/packages/aarch64/openssl/files/PKGBUILD
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=openssl-static
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=openssl-git

  # 2017-Nov-02
  # XBB_OPENSSL_VERSION="1.1.0g"
  # The new version deprecated CRYPTO_set_locking_callback, and yum fails with
  # /usr/lib64/python2.6/site-packages/pycurl.so: undefined symbol: CRYPTO_set_locking_callback

  # 2017-Dec-07, "1.0.2n"
  # 2019-Feb-26, "1.0.2r"
  # 2019-Feb-26, "1.1.1b"
  # 2019-Sep-10, "1.1.1d"
  # 2019-Dec-20, "1.0.2u"
  # 2020-Sep-22, "1.1.1h"
  # 2021-Mar-25, "1.1.1k"
  # 2021-Aug-24, "1.1.1l"
  # 2022-Mar-15, "1.1.1n"
  # "1.1.1q"

  local openssl_version="$1"
  # Numbers
  local openssl_version_major=$(echo ${openssl_version} | sed -e 's|\([0-9][0-9]*\)\.\([0-9][0-9]*\)\..*|\1|')
  local openssl_version_minor=$(echo ${openssl_version} | sed -e 's|\([0-9][0-9]*\)\.\([0-9][0-9]*\)\..*|\2|')

  local openssl_src_folder_name="openssl-${openssl_version}"

  local openssl_archive="${openssl_src_folder_name}.tar.gz"
  local openssl_url="https://www.openssl.org/source/${openssl_archive}"

  local openssl_folder_name="${openssl_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${openssl_folder_name}"

  local openssl_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${openssl_folder_name}-installed"
  if [ ! -f "${openssl_stamp_file_path}" ]
  then

    echo
    echo "openssl in-source building"

    if [ ! -d "${LIBS_BUILD_FOLDER_PATH}/${openssl_folder_name}" ]
    then
      cd "${LIBS_BUILD_FOLDER_PATH}"

      download_and_extract "${openssl_url}" "${openssl_archive}" \
        "${openssl_src_folder_name}"

      if [ "${openssl_src_folder_name}" != "${openssl_folder_name}" ]
      then
        mv -v "${openssl_src_folder_name}" "${openssl_folder_name}"
      fi
    fi

    (
      cd "${LIBS_BUILD_FOLDER_PATH}/${openssl_folder_name}"

      xbb_activate_installed_dev

      #  -Wno-unused-command-line-argument

      # export CPPFLAGS="${XBB_CPPFLAGS} -I${LIBS_BUILD_FOLDER_PATH}/${openssl_folder_name}/include"
      CPPFLAGS="${XBB_CPPFLAGS}"
      if [ "${TARGET_PLATFORM}" == "darwin" ]
      then
        # /usr/include/CommonCrypto/CommonRandom.h:35:9: error: unknown type name 'CCCryptorStatus'
        # typedef CCCryptorStatus CCRNGStatus;
        CPPFLAGS+=" -I/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include"
      fi
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

      if [ ! -f config.stamp ]
      then
        (
          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          echo
          echo "Running openssl configure..."

          echo
          if [ "${TARGET_PLATFORM}" == "darwin" ]
          then

            # Older versions do not support the KERNEL_BITS trick and require
            # the separate configurator.

            if [ ${openssl_version_minor} -eq 0 ]
            then

              # This config does not use the standard GNU environment definitions.
              # `Configure` is a Perl script.
              if [ "${IS_DEVELOP}" == "y" ]
              then
                run_verbose "./Configure" --help || true
              fi

              run_verbose "./Configure" "darwin64-x86_64-cc" \
                --prefix="${LIBS_INSTALL_FOLDER_PATH}" \
                \
                --openssldir="${LIBS_INSTALL_FOLDER_PATH}/openssl" \
                shared \
                enable-md2 \
                enable-rc5 \
                enable-tls \
                enable-tls1_3 \
                enable-tls1_2 \
                enable-tls1_1 \
                zlib \
                "${CPPFLAGS} ${CFLAGS} ${LDFLAGS}"

              run_verbose make depend

            else

              if [ "${IS_DEVELOP}" == "y" ]
              then
                run_verbose "./config" --help
              fi

              # From HomeBrew
              # SSLv2 died with 1.1.0, so no-ssl2 no longer required.
              # SSLv3 & zlib are off by default with 1.1.0 but this may not
              # be obvious to everyone, so explicitly state it for now to
              # help debug inevitable breakage.

              export KERNEL_BITS=64
              run_verbose "./config" \
                --prefix="${LIBS_INSTALL_FOLDER_PATH}" \
                \
                --openssldir="${LIBS_INSTALL_FOLDER_PATH}/openssl" \
                shared \
                enable-md2 \
                enable-rc5 \
                enable-tls \
                enable-tls1_3 \
                enable-tls1_2 \
                enable-tls1_1 \
                no-ssl3 \
                no-ssl3-method \
                zlib \
                "${CPPFLAGS} ${CFLAGS} ${LDFLAGS}"

            fi

          elif [ "${TARGET_PLATFORM}" == "linux" ]
          then

            config_options=()
            if [ "${TARGET_ARCH}" == "x64" ]
            then
              config_options+=("enable-ec_nistp_64_gcc_128")
            elif [ "${TARGET_ARCH}" == "arm64" ]
            then
              config_options+=("no-afalgeng")
            fi

            set +u

            # undefined reference to EVP_md2
            #  enable-md2

            # perl, do not start with bash.
            run_verbose "./config" \
              --prefix="${LIBS_INSTALL_FOLDER_PATH}" \
              \
              --openssldir="${LIBS_INSTALL_FOLDER_PATH}/openssl" \
              shared \
              enable-md2 \
              enable-rc5 \
              enable-tls \
              enable-tls1_3 \
              enable-tls1_2 \
              enable-tls1_1 \
              no-ssl3 \
              no-ssl3-method \
              zlib \
              "${config_options[@]}" \
              "-Wa,--noexecstack ${CPPFLAGS} ${CFLAGS} ${LDFLAGS}"

            set -u

            if [ ${openssl_version_minor} -eq 0 ]
            then
              run_verbose make depend
            fi

          elif [ "${TARGET_PLATFORM}" == "win32" ]
          then

            run_verbose "./Configure" --help || true

            config_options=()

            if [ "${TARGET_ARCH}" == "x64" ]
            then
              config_options+=("mingw64")
            elif [ "${TARGET_ARCH}" == "ia32" ]
            then
              config_options+=("mingw")
            else
              echo "Unsupported TARGET_ARCH ${TARGET_ARCH}"
              exit 1
            fi

            config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

            # Not needed, the CC/CXX macros already define the target.
            # config_options+=("--cross-compile-prefix=${TARGET}")

            config_options+=("--openssldir=${LIBS_INSTALL_FOLDER_PATH}/openssl")

            config_options+=("shared")
            config_options+=("zlib-dynamic")
            config_options+=("enable-camellia")
            config_options+=("enable-capieng")
            config_options+=("enable-idea")
            config_options+=("enable-mdc2")
            config_options+=("enable-rc5")
            config_options+=("enable-rfc3779")
            config_options+=("-D__MINGW_USE_VC2005_COMPAT")

            run_verbose "./Configure" \
              "${config_options[@]}"

            run_verbose make -j ${JOBS}

          else
            echo "Unsupported TARGET_PLATFORM ${TARGET_PLATFORM}"
            exit 1
          fi

          touch config.stamp

          # cp "configure.log" "${LOGS_FOLDER_PATH}/configure-openssl-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${openssl_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running openssl make..."

        # Build.
        run_verbose make -j ${JOBS}

        run_verbose make install_sw

        mkdir -pv "${LIBS_INSTALL_FOLDER_PATH}/openssl"

        if [ -f "${XBB_FOLDER_PATH}/openssl/cert.pem" ]
        then
          install -v -c -m 644 "${XBB_FOLDER_PATH}/openssl/ca-bundle.crt" "${LIBS_INSTALL_FOLDER_PATH}/openssl"
          install -v -c -m 644 "${XBB_FOLDER_PATH}/openssl/cert.pem" "${LIBS_INSTALL_FOLDER_PATH}/openssl"
        elif [ -f "/private/etc/ssl/cert.pem" ]
        then
          install -v -c -m 644 "/private/etc/ssl/cert.pem" "${LIBS_INSTALL_FOLDER_PATH}/openssl"
        fi

        curl --location http://curl.haxx.se/ca/cacert.pem -o cacert.pem
        install -v -c -m 644 cacert.pem "${LIBS_INSTALL_FOLDER_PATH}/openssl"

        if [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose make -j1 test
        fi

        if [ -f "${LIBS_INSTALL_FOLDER_PATH}/lib64/libcrypto.${SHLIB_EXT}" ]
        then
          show_libs "${LIBS_INSTALL_FOLDER_PATH}/lib64/libcrypto.${SHLIB_EXT}"
        else
          show_libs "${LIBS_INSTALL_FOLDER_PATH}/lib/libcrypto.${SHLIB_EXT}"
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${openssl_folder_name}/make-output-$(ndate).txt"

      (
        test_openssl
      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${openssl_folder_name}/test-output-$(ndate).txt"
    )

    touch "${openssl_stamp_file_path}"

  else
    echo "Component openssl already installed."
  fi
}

function test_openssl()
{
  (
    xbb_activate_installed_bin

    echo
    echo "Testing if the openssl binaries start properly..."

    run_app "${LIBS_INSTALL_FOLDER_PATH}/bin/openssl" version

    echo
    echo "Checking the openssl shared libraries..."

    show_libs "${LIBS_INSTALL_FOLDER_PATH}/bin/openssl"

    if [ -f "${LIBS_INSTALL_FOLDER_PATH}/lib64/libcrypto.${SHLIB_EXT}" ]
    then
      show_libs "${LIBS_INSTALL_FOLDER_PATH}/lib64/libcrypto.${SHLIB_EXT}"
    else
      show_libs "${LIBS_INSTALL_FOLDER_PATH}/lib/libcrypto.${SHLIB_EXT}"
    fi

    if [ -f "${LIBS_INSTALL_FOLDER_PATH}/lib64/libssl.${SHLIB_EXT}" ]
    then
      show_libs "${LIBS_INSTALL_FOLDER_PATH}/lib64/libssl.${SHLIB_EXT}"
    else
      show_libs "${LIBS_INSTALL_FOLDER_PATH}/lib/libssl.${SHLIB_EXT}"
    fi
  )
}

# -----------------------------------------------------------------------------

function build_sqlite()
{
  # https://www.sqlite.org/
  # https://sqlite.org/chronology.html
  # https://www.sqlite.org/download.html
  # https://www.sqlite.org/2020/sqlite-src-3330000.zip
  # https://www.sqlite.org/2021/sqlite-src-3360000.zip
  # https://www.sqlite.org/2022/sqlite-src-3380200.zip
  # https://www.sqlite.org/src/tarball/7ebdfa80/SQLite-7ebdfa80.tar.gz

  # https://archlinuxarm.org/packages/aarch64/sqlite/files/PKGBUILD

  # 2020-06-18 "3.32.3" 7ebdfa80
  # 2021-06-18 "3360000"
  # 2022 "3380200"
  # "3390200"

  local sqlite_version="$1"

  local sqlite_src_folder_name="sqlite-src-${sqlite_version}"
  local sqlite_archive="${sqlite_src_folder_name}.zip"
  local sqlite_url

  if [ "${sqlite_version}" == "3380200" ]
  then
    # 2022
    sqlite_url="https://www.sqlite.org/2022/${sqlite_archive}"
  elif [ "${sqlite_version}" == "3360000" ]
  then
    # 2021
    sqlite_url="https://www.sqlite.org/2021/${sqlite_archive}"
  elif [ "${sqlite_version}" == "3.32.3" ]
  then
    sqlite_commit="7ebdfa80"
    sqlite_src_folder_name="SQLite-${sqlite_commit}"
    sqlite_archive="${sqlite_src_folder_name}.tar.gz"
    sqlite_url="https://www.sqlite.org/src/tarball/${sqlite_commit}/${sqlite_archive}"
  else
    echo "Unsupported version ${sqlite_version}. Quit."
    exit 1
  fi

  local sqlite_folder_name="sqlite-${sqlite_version}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${sqlite_folder_name}"

  local sqlite_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-${sqlite_folder_name}-installed"
  if [ ! -f "${sqlite_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${sqlite_url}" "${sqlite_archive}" \
      "${sqlite_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${sqlite_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${sqlite_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
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
          echo "Running sqlite configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${sqlite_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          config_options+=("--enable-tcl=no")
          # Fails on macOS & Linux.
          # config_options+=("--disable-tcl")
          # Fail on macOS.
          # config_options+=("--disable-readline")
          # config_options+=("--disable-amalgamation")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${sqlite_src_folder_name}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${sqlite_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${sqlite_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running sqlite make..."

        # Build.
        run_verbose make -j ${JOBS}

        # Fails on Linux. And takes way too long.
        # 2 errors out of 249249 tests on docker Linux 64-bit little-endian
        # !Failures on these tests: oserror-1.4.1 oserror-1.4.2
        if false # [ "${WITH_TESTS}" == "y" ]
        then
          (
            # To access the /opt/xbb/lib/libtcl8.6.so
            xbb_activate_libs

            run_verbose make -j1 quicktest
          )
        fi

        # Has no install-strip
        run_verbose make install

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${sqlite_folder_name}/make-output-$(ndate).txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${sqlite_src_folder_name}" \
        "${sqlite_folder_name}"

    )

    touch "${sqlite_stamp_file_path}"

  else
    echo "Library sqlite already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_readline()
{
  # https://tiswww.case.edu/php/chet/readline/rltop.html
  # https://ftp.gnu.org/gnu/readline/
  # https://ftp.gnu.org/gnu/readline/readline-8.0.tar.gz

  # depends=(glibc gcc-libs)
  # https://archlinuxarm.org/packages/aarch64/readline/files/PKGBUILD

  # 2019-01-07, "8.0"
  # 2020-12-06, "8.1"
  # 2022-01-05, "8.1.2"

  local readline_version="$1"
  local readline_version_major="$(echo ${readline_version} | sed -e 's|\([0-9][0-9]*\)\.\([0-9][0-9]*\)|\1|')"
  local readline_version_minor="$(echo ${readline_version} | sed -e 's|\([0-9][0-9]*\)\.\([0-9][0-9]*\)|\2|')"

  # The folder name as resulted after being extracted from the archive.
  local readline_src_folder_name="readline-${readline_version}"

  local readline_archive="${readline_src_folder_name}.tar.gz"
  local readline_url="https://ftp.gnu.org/gnu/readline/${readline_archive}"

  # The folder name  for build, licenses, etc.
  local readline_folder_name="${readline_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${readline_folder_name}"

  local readline_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${readline_folder_name}-installed"
  if [ ! -f "${readline_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${readline_url}" "${readline_archive}" \
      "${readline_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${readline_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${readline_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
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
          echo "Running readline configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${readline_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${readline_src_folder_name}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${readline_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${readline_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running readline make..."

        # Build.
        run_verbose make -j ${JOBS}

        # Has no install-strip
        run_verbose make install

        if [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose make -j1 check
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${readline_folder_name}/make-output-$(ndate).txt"
    )

    (
      test_readline
    ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${readline_folder_name}/test-output-$(ndate).txt"

    touch "${readline_stamp_file_path}"

  else
    echo "Library readline already installed."
  fi
}

function test_readline()
{
  (
    xbb_activate

    echo
    echo "Checking the readline shared libraries..."

    show_libs "${LIBS_INSTALL_FOLDER_PATH}/lib/libreadline.${SHLIB_EXT}"
    show_libs "${LIBS_INSTALL_FOLDER_PATH}/lib/libhistory.${SHLIB_EXT}"
  )
}

# -----------------------------------------------------------------------------

# TODO: add support for dylib
function build_bzip2()
{
  # https://sourceware.org/bzip2/
  # https://sourceware.org/pub/bzip2/
  # https://sourceware.org/pub/bzip2/bzip2-1.0.8.tar.gz

  # https://archlinuxarm.org/packages/aarch64/bzip2/files/PKGBUILD

  # https://github.com/msys2/MINGW-packages/blob/master/mingw-w64-bzip2/PKGBUILD

  # https://github.com/Homebrew/homebrew-core/blob/master/Formula/bzip2.rb

  # 2019-07-13 "1.0.8"

  local bzip2_version="$1"

  local bzip2_src_folder_name="bzip2-${bzip2_version}"

  local bzip2_archive="${bzip2_src_folder_name}.tar.gz"
  local bzip2_url="https://sourceware.org/pub/bzip2/${bzip2_archive}"

  local bzip2_folder_name="${bzip2_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${bzip2_folder_name}"

  local bzip2_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-${bzip2_folder_name}-installed"
  if [ ! -f "${bzip2_stamp_file_path}" ]
  then

    echo
    echo "bzip2 in-source building"

    if [ ! -d "${LIBS_BUILD_FOLDER_PATH}/${bzip2_folder_name}" ]
    then
      cd "${LIBS_BUILD_FOLDER_PATH}"

      download_and_extract "${bzip2_url}" "${bzip2_archive}" \
        "${bzip2_src_folder_name}"

      if [ "${bzip2_src_folder_name}" != "${bzip2_folder_name}" ]
      then
        mv -v "${bzip2_src_folder_name}" "${bzip2_folder_name}"
      fi
    fi

    (
      cd "${LIBS_BUILD_FOLDER_PATH}/${bzip2_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      # libbz2.a(bzlib.o): relocation R_X86_64_PC32 against symbol `BZ2_crc32Table' can not be used when making a shared object; recompile with -fPIC
      CFLAGS="${XBB_CFLAGS_NO_W} -fPIC"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
      if [ "${TARGET_PLATFORM}" == "linux" ]
      then
        LDFLAGS+=" -Wl,-rpath,${LD_LIBRARY_PATH}"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      (
        if [ "${IS_DEVELOP}" == "y" ]
        then
          env | sort
        fi

        echo
        echo "Running bzip2 make..."

        if [ "${TARGET_PLATFORM}" == "linux" ]
        then
          # Build.
          run_verbose make all -j ${JOBS} \
            PREFIX=${LIBS_INSTALL_FOLDER_PATH} \
            CC=${CC} \
            AR=${AR} \
            RANLIB=${RANLIB} \
            LDFLAGS=${LDFLAGS} \

          run_verbose make install PREFIX=${LIBS_INSTALL_FOLDER_PATH}

          run_verbose make clean

          # Build the shared library.
          run_verbose make all -f Makefile-libbz2_so -j ${JOBS} \
            PREFIX=${LIBS_INSTALL_FOLDER_PATH} \
            CC=${CC} \
            AR=${AR} \
            RANLIB=${RANLIB} \
            LDFLAGS=${LDFLAGS} \

          mkdir -pv "${LIBS_INSTALL_FOLDER_PATH}/lib/"
          install -v -c -m 644 "libbz2.so.${bzip2_version}" "${LIBS_INSTALL_FOLDER_PATH}/lib/"

          (
            cd "${LIBS_INSTALL_FOLDER_PATH}/lib/"

            rm -rfv libbz2.so*
            ln -sv "libbz2.so.${bzip2_version}" libbz2.so.1.0
            ln -sv "libbz2.so.${bzip2_version}" libbz2.so.1
            ln -sv "libbz2.so.${bzip2_version}" libbz2.so
          )
        elif [ "${TARGET_PLATFORM}" == "darwin" ]
        then

          # Build.
          run_verbose make all -j ${JOBS} \
            PREFIX=${LIBS_INSTALL_FOLDER_PATH} \
            CC=${CC} \
            AR=${AR} \
            RANLIB=${RANLIB} \
            LDFLAGS=${LDFLAGS} \

          run_verbose make install PREFIX=${LIBS_INSTALL_FOLDER_PATH}

          # TODO: add support for creating macOS dylib.

        elif [ "${TARGET_PLATFORM}" == "windows" ]
        then

          # Not yet functional.
          run_verbose make libbz2.a bzip2 bzip2recover -j ${JOBS} \
            PREFIX=${LIBS_INSTALL_FOLDER_PATH} \
            CC=${CC} \
            AR=${AR} \
            RANLIB=${RANLIB} \
            LDFLAGS=${LDFLAGS} \

          run_verbose make install PREFIX=${LIBS_INSTALL_FOLDER_PATH}

        fi
      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${bzip2_folder_name}/make-output-$(ndate).txt"

      copy_license \
        "${LIBS_BUILD_FOLDER_PATH}/${bzip2_folder_name}" \
        "${bzip2_folder_name}"

    )

    touch "${bzip2_stamp_file_path}"

  else
    echo "Library bzip2 already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_lzo()
{
  # Real-time data compression library
  # https://www.oberhumer.com/opensource/lzo/
  # https://www.oberhumer.com/opensource/lzo/download/lzo-2.10.tar.gz

  # https://github.com/archlinux/svntogit-packages/blob/packages/lzo/trunk/PKGBUILD

  # https://github.com/Homebrew/homebrew-core/blob/master/Formula/lzo.rb

  # 01 Mar 2017 "2.10"

  local lzo_version="$1"

  local lzo_src_folder_name="lzo-${lzo_version}"

  local lzo_archive="${lzo_src_folder_name}.tar.gz"
  local lzo_url="https://www.oberhumer.com/opensource/lzo/download/${lzo_archive}"

  local lzo_folder_name="${lzo_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${lzo_folder_name}"

  local lzo_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${lzo_folder_name}-installed"
  if [ ! -f "${lzo_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${lzo_url}" "${lzo_archive}" \
      "${lzo_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${lzo_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${lzo_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
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
          echo "Running lzo configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${lzo_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          config_options+=("--disable-dependency-tracking")
          config_options+=("--enable-shared")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${lzo_src_folder_name}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${lzo_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${lzo_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running lzo make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose make -j1 check
        fi

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${lzo_folder_name}/make-output-$(ndate).txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${lzo_src_folder_name}" \
        "${lzo_folder_name}"

    )

    touch "${lzo_stamp_file_path}"

  else
    echo "Library lzo already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_python2()
{
  # https://www.python.org
  # https://www.python.org/downloads/source/
  # https://www.python.org/ftp/python/
  # https://www.python.org/ftp/python/2.7.18/Python-2.7.18.tar.xz

  # https://archlinuxarm.org/packages/aarch64/python/files/PKGBUILD
  # https://git.archlinux.org/svntogit/packages.git/tree/trunk/PKGBUILD?h=packages/python
  # https://git.archlinux.org/svntogit/packages.git/tree/trunk/PKGBUILD?h=packages/python-pip

  # 19-Apr-2020, "2.7.18"

  local python2_version="$1"

  export PYTHON2_VERSION_MAJOR=$(echo ${python2_version} | sed -e 's|\([0-9]\)\..*|\1|')
  export PYTHON2_VERSION_MINOR=$(echo ${python2_version} | sed -e 's|\([0-9]\)\.\([0-9][0-9]*\)\..*|\2|')
  export PYTHON2_VERSION_MAJOR_MINOR=${PYTHON2_VERSION_MAJOR}${PYTHON2_VERSION_MINOR}

  # Used in python27-config.sh.
  export PYTHON2_SRC_FOLDER_NAME="Python-${python2_version}"

  local python2_archive="${PYTHON2_SRC_FOLDER_NAME}.tar.xz"
  local python2_url="https://www.python.org/ftp/python/${python2_version}/${python2_archive}"

  local python2_folder_name="python-${python2_version}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${python2_folder_name}"

  local python2_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${python2_folder_name}-installed"
  if [ ! -f "${python2_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${python2_url}" "${python2_archive}" \
      "${PYTHON2_SRC_FOLDER_NAME}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${python2_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${python2_folder_name}"

      # To pick the new libraries
      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS} -I${LIBS_INSTALL_FOLDER_PATH}/include/ncurses"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_APP_STATIC_GCC}"
      if [ "${TARGET_PLATFORM}" == "linux" ]
      then
        LDFLAGS+=" -Wl,-rpath,${LD_LIBRARY_PATH}"
      fi

      if [[ ${CC} =~ .*gcc.* ]]
      then
        # Inspired from Arch; not supported by clang.
        CFLAGS+=" -fno-semantic-interposition"
        CXXFLAGS+=" -fno-semantic-interposition"
        LDFLAGS+=" -fno-semantic-interposition"
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
          echo "Running python2 configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${PYTHON2_SRC_FOLDER_NAME}/configure" --help
          fi

          # Fail on macOS:
          # --enable-universalsdk
          # --with-lto

          # "... you should not skip tests when using --enable-optimizations as
          # the data required for profiling is generated by running tests".

          # --enable-optimizations takes too long

          config_options=()
          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--with-dbmliborder=gdbm:ndbm")

          config_options+=("--without-ensurepip")
          config_options+=("--without-lto")

          # Create the PythonX.Y.so.
          config_options+=("--enable-shared")

          if [ "${TARGET_PLATFORM}" == "darwin" ]
          then
            config_options+=("--enable-unicode=ucs2")
          else
            config_options+=("--enable-unicode=ucs4")
          fi

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${PYTHON2_SRC_FOLDER_NAME}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${python2_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${python2_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running python2 make..."

        # export LD_RUN_PATH="${LIBS_INSTALL_FOLDER_PATH}/lib"

        # Build.
        run_verbose make -j ${JOBS} # build_all

        run_verbose make altinstall

        # Hundreds of tests, take a lot of time.
        # Many failures.
        if false # [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose make -j1 quicktest
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${python2_folder_name}/make-output-$(ndate).txt"
    )

    (
      test_python2
    ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${python2_folder_name}/test-output-$(ndate).txt"

    copy_license \
      "${SOURCES_FOLDER_PATH}/${PYTHON2_SRC_FOLDER_NAME}" \
      "${python2_folder_name}"

    touch "${python2_stamp_file_path}"

  else
    echo "Component python2 already installed."
  fi
}


function test_python2()
{
  (
    # xbb_activate_installed_bin

    echo
    echo "Checking the python2 binary shared libraries..."

    show_libs "${LIBS_INSTALL_FOLDER_PATH}/bin/python2.${PYTHON2_VERSION_MINOR}"
    show_libs "${LIBS_INSTALL_FOLDER_PATH}/lib/libpython${PYTHON2_VERSION_MAJOR}.${PYTHON2_VERSION_MINOR}.${SHLIB_EXT}"

    echo
    echo "Testing if the python2 binary starts properly..."

    export LD_LIBRARY_PATH="${LIBS_INSTALL_FOLDER_PATH}/lib"
    run_app "${LIBS_INSTALL_FOLDER_PATH}/bin/python2.${PYTHON2_VERSION_MINOR}" --version

    run_app "${LIBS_INSTALL_FOLDER_PATH}/bin/python2.${PYTHON2_VERSION_MINOR}" -c 'import sys; print(sys.path)'
    run_app "${LIBS_INSTALL_FOLDER_PATH}/bin/python2.${PYTHON2_VERSION_MINOR}" -c 'import sys; print(sys.prefix)'
  )
}


# -----------------------------------------------------------------------------

# Download the Windows Python 2 libraries and headers.
function download_python2_win()
{
  # https://www.python.org/downloads/release/python-2714/
  # https://www.python.org/ftp/python/2.7.14/python-2.7.14.msi
  # https://www.python.org/ftp/python/2.7.14/python-2.7.14.amd64.msi

  local python2_win_version="$1"

  export PYTHON2_VERSION_MAJOR=$(echo ${python2_win_version} | sed -e 's|\([0-9]\)\..*|\1|')
  export PYTHON2_VERSION_MINOR=$(echo ${python2_win_version} | sed -e 's|\([0-9]\)\.\([0-9][0-9]*\)\..*|\2|')
  export PYTHON2_VERSION_MAJOR_MINOR=${PYTHON2_VERSION_MAJOR}${PYTHON2_VERSION_MINOR}

  local python2_win_pack

  if [ "${TARGET_BITS}" == "32" ]
  then
    PYTHON2_WIN_SRC_FOLDER_NAME="python-${python2_win_version}-embed-win32"
    python2_win_pack="python-${python2_win_version}.msi"
  else
    PYTHON2_WIN_SRC_FOLDER_NAME="python-${python2_win_version}-embed-amd64"
    python2_win_pack="python-${python2_win_version}.amd64.msi"
  fi

  # Used in python27-config.sh.
  export PYTHON2_WIN_SRC_FOLDER_NAME

  local python2_win_url="https://www.python.org/ftp/python/${python2_win_version}/${python2_win_pack}"

  cd "${SOURCES_FOLDER_PATH}"

  download "${python2_win_url}" "${python2_win_pack}"

  (
    if [ ! -d "${PYTHON2_WIN_SRC_FOLDER_NAME}" ]
    then
      cd "${SOURCES_FOLDER_PATH}"

      # Include only the headers and the python library and executable.
      local tmp_path="/tmp/included$$"
      echo '*.h' >"${tmp_path}"
      echo 'python*.dll' >>"${tmp_path}"
      echo 'python*.lib' >>"${tmp_path}"
      7za x -o"${PYTHON2_WIN_SRC_FOLDER_NAME}" "${DOWNLOAD_FOLDER_PATH}/${python2_win_pack}" -i@"${tmp_path}"

      # Patch to disable the macro that renames hypot.
      local patch_path="${PYTHON2_WIN_SRC_FOLDER_NAME}.patch"
      if [ -f "${patch_path}" ]
      then
        (
          cd "${PYTHON2_WIN_SRC_FOLDER_NAME}"
          patch -p0 <"${patch_path}"
        )
      fi
    else
      echo "Folder ${PYTHON2_WIN_SRC_FOLDER_NAME} already present."
    fi

    echo "Copying python${PYTHON2_VERSION_MAJOR_MINOR}.dll..."
    # From here it'll be copied as dependency.
    mkdir -pv "${LIBS_INSTALL_FOLDER_PATH}/bin/"
    install -v -c -m 644 "${PYTHON2_WIN_SRC_FOLDER_NAME}/python${PYTHON2_VERSION_MAJOR_MINOR}.dll" \
      "${LIBS_INSTALL_FOLDER_PATH}/bin/"

    mkdir -pv "${LIBS_INSTALL_FOLDER_PATH}/lib/"
    install -v -c -m 644 "${PYTHON2_WIN_SRC_FOLDER_NAME}/python${PYTHON2_VERSION_MAJOR_MINOR}.lib" \
      "${LIBS_INSTALL_FOLDER_PATH}/lib/"
  )
}

# -----------------------------------------------------------------------------

# https://stackoverflow.com/questions/44150871/embeded-python3-6-with-mingw-in-c-fail-on-linking

function build_python3()
{
  # https://www.python.org
  # https://www.python.org/downloads/source/
  # https://www.python.org/ftp/python/
  # https://www.python.org/ftp/python/3.7.3/Python-3.7.3.tar.xz

  # https://github.com/Homebrew/homebrew-core/blob/master/Formula/python@3.10.rb

  # https://archlinuxarm.org/packages/aarch64/python/files/PKGBUILD
  # https://git.archlinux.org/svntogit/packages.git/tree/trunk/PKGBUILD?h=packages/python
  # https://git.archlinux.org/svntogit/packages.git/tree/trunk/PKGBUILD?h=packages/python-pip

  # 2018-12-24, "3.7.2"
  # March 25, 2019, "3.7.3"
  # Dec. 18, 2019, "3.8.1"
  # 17-Aug-2020, "3.7.9"
  # 23-Sep-2020, "3.8.6"
  # May 3, 2021 "3.8.10"
  # May 3, 2021 "3.9.5"
  # Aug. 30, 2021, "3.8.12"
  # Aug. 30, 2021, "3.9.7"
  # Sept. 4, 2021, "3.7.12"
  # 24-Mar-2022, "3.9.12"
  # 23-Mar-2022, "3.10.4"

  local python3_version="$1"

  local python3_version_major=$(echo ${python3_version} | sed -e 's|\([0-9]\)\..*|\1|')
  local python3_version_minor=$(echo ${python3_version} | sed -e 's|\([0-9]\)\.\([0-9][0-9]*\)\..*|\2|')

  PYTHON3_VERSION_MAJOR=$(echo ${python3_version} | sed -e 's|\([0-9]\)\..*|\1|')
  PYTHON3_VERSION_MINOR=$(echo ${python3_version} | sed -e 's|\([0-9]\)\.\([0-9][0-9]*\)\..*|\2|')
  export PYTHON3_VERSION_MAJOR_MINOR=${PYTHON3_VERSION_MAJOR}${PYTHON3_VERSION_MINOR}

  local python3_src_folder_name="Python-${python3_version}"

  # Used in add_python3_syslibs()
  PYTHON3_SRC_FOLDER_NAME=${python3_src_folder_name}

  local python3_archive="${python3_src_folder_name}.tar.xz"
  local python3_url="https://www.python.org/ftp/python/${python3_version}/${python3_archive}"

  local python3_folder_name="python-${python3_version}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${python3_folder_name}"

  local python3_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${python3_folder_name}-installed"
  if [ ! -f "${python3_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${python3_url}" "${python3_archive}" \
      "${python3_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${python3_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${python3_folder_name}"

      # To pick the new libraries
      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS} -I${LIBS_INSTALL_FOLDER_PATH}/include/ncurses"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      if [ "${TARGET_PLATFORM}" == "darwin" ] && [[ ${CC} =~ .*gcc.* ]]
      then
        # HACK! GCC chokes on dynamic sizes:
        # error: variably modified ‘bytes’ at file scope
        # char bytes[kAuthorizationExternalFormLength];
        # -DkAuthorizationExternalFormLength=32 not working
        prepare_clang_env ""
      fi

      LDFLAGS="${XBB_LDFLAGS_APP_STATIC_GCC}"
      if [ "${TARGET_PLATFORM}" == "linux" ]
      then
        LDFLAGS+=" -Wl,-rpath,${LD_LIBRARY_PATH}"
      fi

      if [[ ${CC} =~ .*gcc.* ]]
      then
        # Inspired from Arch; not supported by clang.
        CFLAGS+=" -fno-semantic-interposition"
        CXXFLAGS+=" -fno-semantic-interposition"
        LDFLAGS+=" -fno-semantic-interposition"
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
          echo "Running python3 configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${python3_src_folder_name}/configure" --help
          fi

          # Fail on macOS:
          # --enable-universalsdk
          # --with-lto

          # "... you should not skip tests when using --enable-optimizations as
          # the data required for profiling is generated by running tests".

          # --enable-optimizations takes too long

          config_options=()
          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--with-universal-archs=${TARGET_BITS}-bit")
          config_options+=("--with-computed-gotos")
          config_options+=("--with-dbmliborder=gdbm:ndbm")

          # From Brew, but better not, allow configure to choose.
          # config_options+=("--with-system-expat")
          # config_options+=("--with-system-ffi")
          # config_options+=("--with-system-libmpdec")

          # config_options+=("--with-openssl=${INSTALL_FOLDER_PATH}")
          config_options+=("--without-ensurepip")
          config_options+=("--without-lto")

          # Create the PythonX.Y.so.
          config_options+=("--enable-shared")

          # config_options+=("--enable-loadable-sqlite-extensions")
          config_options+=("--disable-loadable-sqlite-extensions")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${python3_src_folder_name}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${python3_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${python3_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running python3 make..."

        # export LD_RUN_PATH="${LIBS_INSTALL_FOLDER_PATH}/lib"

        # Build.
        run_verbose make -j ${JOBS} # build_all

        run_verbose make altinstall

        # Hundreds of tests, take a lot of time.
        # Many failures.
        if false # [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose make -j1 quicktest
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${python3_folder_name}/make-output-$(ndate).txt"
    )

    (
      test_python3
    ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${python3_folder_name}/test-output-$(ndate).txt"

    copy_license \
      "${SOURCES_FOLDER_PATH}/${python3_src_folder_name}" \
      "${python3_folder_name}"

    touch "${python3_stamp_file_path}"

  else
    echo "Component python3 already installed."
  fi
}

function test_python3()
{
  (
    # xbb_activate_installed_bin

    echo
    echo "Checking the python3 binary shared libraries..."

    show_libs "${LIBS_INSTALL_FOLDER_PATH}/bin/python3.${PYTHON3_VERSION_MINOR}"
    if [ -f "${LIBS_INSTALL_FOLDER_PATH}/lib/libpython${PYTHON3_VERSION_MAJOR}.${PYTHON3_VERSION_MINOR}m.${SHLIB_EXT}" ]
    then
      show_libs "${LIBS_INSTALL_FOLDER_PATH}/lib/libpython${PYTHON3_VERSION_MAJOR}.${PYTHON3_VERSION_MINOR}m.${SHLIB_EXT}"
    elif [ -f "${LIBS_INSTALL_FOLDER_PATH}/lib/libpython${PYTHON3_VERSION_MAJOR}.${PYTHON3_VERSION_MINOR}.${SHLIB_EXT}" ]
    then
      show_libs "${LIBS_INSTALL_FOLDER_PATH}/lib/libpython${PYTHON3_VERSION_MAJOR}.${PYTHON3_VERSION_MINOR}.${SHLIB_EXT}"
    fi

    echo
    echo "Testing if the python3 binary starts properly..."

    export LD_LIBRARY_PATH="${LIBS_INSTALL_FOLDER_PATH}/lib"
    run_app "${LIBS_INSTALL_FOLDER_PATH}/bin/python3.${PYTHON3_VERSION_MINOR}" --version

    run_app "${LIBS_INSTALL_FOLDER_PATH}/bin/python3.${PYTHON3_VERSION_MINOR}" -c 'import sys; print(sys.path)'
    run_app "${LIBS_INSTALL_FOLDER_PATH}/bin/python3.${PYTHON3_VERSION_MINOR}" -c 'import sys; print(sys.prefix)'
  )
}

# -----------------------------------------------------------------------------

# Download the Windows Python 3 libraries and headers.
function download_python3_win()
{
  # https://www.python.org/downloads/windows/
  # https://www.python.org/downloads/release/python-372/
  # https://www.python.org/ftp/python/3.7.2/python-3.7.2.post1-embed-win32.zip
  # https://www.python.org/ftp/python/3.7.2/python-3.7.2.post1-embed-amd64.zip
  # https://www.python.org/ftp/python/3.7.2/python-3.7.2.exe
  # https://www.python.org/ftp/python/3.7.2/python-3.7.2-amd64.exe
  # https://www.python.org/ftp/python/3.7.2/Python-3.7.2.tar.xz
  # https://www.python.org/ftp/python/3.7.6/
  # https://www.python.org/ftp/python/3.7.6/python-3.7.6-embed-amd64.zip
  # https://www.python.org/ftp/python/3.7.6/python-3.7.6-embed-win32.zip

  local python3_win_version="$1"

  PYTHON3_VERSION_MAJOR=$(echo ${python3_win_version} | sed -e 's|\([0-9]\)\..*|\1|')
  PYTHON3_VERSION_MINOR=$(echo ${python3_win_version} | sed -e 's|\([0-9]\)\.\([0-9][0-9]*\)\..*|\2|')
  export PYTHON3_VERSION_MAJOR_MINOR=${PYTHON3_VERSION_MAJOR}${PYTHON3_VERSION_MINOR}

  # Version 3.7.2 uses a longer name, like python-3.7.2.post1-embed-amd64.zip.
  if [ "${TARGET_BITS}" == "32" ]
  then
    PYTHON3_WIN_SRC_FOLDER_NAME="python-${python3_win_version}-embed-win32"
  else
    PYTHON3_WIN_SRC_FOLDER_NAME="python-${python3_win_version}-embed-amd64"
  fi

  # Used in python3-config.sh
  export PYTHON3_WIN_SRC_FOLDER_NAME

  local python3_win_embed_pack="${PYTHON3_WIN_SRC_FOLDER_NAME}.zip"
  local python3_win_embed_url="https://www.python.org/ftp/python/${python3_win_version}/${python3_win_embed_pack}"

  (
    if [ ! -d "${SOURCES_FOLDER_PATH}/${PYTHON3_WIN_SRC_FOLDER_NAME}" ]
    then
      download "${python3_win_embed_url}" "${python3_win_embed_pack}"

      # The archive has no folders, so extract it manually.
      mkdir -p "${SOURCES_FOLDER_PATH}/${PYTHON3_WIN_SRC_FOLDER_NAME}"
      cd "${SOURCES_FOLDER_PATH}/${PYTHON3_WIN_SRC_FOLDER_NAME}"
      run_verbose_develop unzip "${DOWNLOAD_FOLDER_PATH}/${python3_win_embed_pack}"
    else
      echo "Folder ${PYTHON3_WIN_SRC_FOLDER_NAME} already present."
    fi

    cd "${SOURCES_FOLDER_PATH}/${PYTHON3_WIN_SRC_FOLDER_NAME}"
    echo "Copying python${PYTHON3_VERSION_MAJOR}${PYTHON3_VERSION_MINOR}.dll..."
    # From here it'll be copied as dependency.
    mkdir -pv "${LIBS_INSTALL_FOLDER_PATH}/bin/"
    install -v -c -m 644 "python${PYTHON3_VERSION_MAJOR}.dll" \
      "${LIBS_INSTALL_FOLDER_PATH}/bin/"
    install -v -c -m 644 "python${PYTHON3_VERSION_MAJOR}${PYTHON3_VERSION_MINOR}.dll" \
      "${LIBS_INSTALL_FOLDER_PATH}/bin/"
  )

  export PYTHON3_SRC_FOLDER_NAME="Python-${python3_win_version}"

  local python3_archive="${PYTHON3_SRC_FOLDER_NAME}.tar.xz"
  local python3_url="https://www.python.org/ftp/python/${python3_win_version}/${python3_archive}"

  # The full source is needed for the headers.
  if [ ! -d "${SOURCES_FOLDER_PATH}/${PYTHON3_SRC_FOLDER_NAME}" ]
  then
    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${python3_url}" "${python3_archive}" \
      "${PYTHON3_SRC_FOLDER_NAME}"
  fi
}

# -----------------------------------------------------------------------------

# Used by gdb-py3 on Windows. The default paths on Windows are different
# from POSIX.
function add_python3_win_syslibs()
{
  if [ "${TARGET_PLATFORM}" == "win32" ]
  then
    echo
    echo "Copying .pyd & .dll files from the embedded Python distribution..."
    mkdir -pv "${APP_PREFIX}/bin"
    cp -v "${SOURCES_FOLDER_PATH}/${PYTHON3_WIN_SRC_FOLDER_NAME}/python${PYTHON3_VERSION_MAJOR_MINOR}.zip"\
      "${APP_PREFIX}/bin"

    mkdir -pv "${APP_PREFIX}/bin/DLLs"
    cp -v "${SOURCES_FOLDER_PATH}/${PYTHON3_WIN_SRC_FOLDER_NAME}"/*.pyd \
      "${APP_PREFIX}/bin/DLLs"
    cp -v "${SOURCES_FOLDER_PATH}/${PYTHON3_WIN_SRC_FOLDER_NAME}"/*.dll \
      "${APP_PREFIX}/bin/DLLs"
  fi
}

# Used by gdb-py3 on POSIX and by packages with full
# control over path (like meson) on all platforms.
function add_python3_syslibs()
{
  local python_with_version="python${PYTHON3_VERSION_MAJOR}.${PYTHON3_VERSION_MINOR}"
  if [ ! -d "${APP_PREFIX}/lib/${python_with_version}/" ]
  then
    (
      mkdir -pv "${APP_PREFIX}/lib/${python_with_version}/"

      (
        echo
        echo "Copying .py files from the standard Python library..."

        # Copy all .py from the original source package.
        cp -r "${SOURCES_FOLDER_PATH}/${PYTHON3_SRC_FOLDER_NAME}"/Lib/* \
          "${APP_PREFIX}/lib/${python_with_version}/"

        echo "Compiling all python sources..."
        if [ "${TARGET_PLATFORM}" == "win32" ]
        then
          run_verbose "${WORK_FOLDER_PATH}/${LINUX_INSTALL_RELATIVE_PATH}/libs/bin/python3.${PYTHON3_VERSION_MINOR}" \
            -m compileall \
            -j "${JOBS}" \
            -f "${APP_PREFIX}/lib/${python_with_version}/" \
            || true
        else
          # Compiling tests fails, ignore the errors.
          run_verbose "${LIBS_INSTALL_FOLDER_PATH}/bin/python3.${PYTHON3_VERSION_MINOR}" \
            -m compileall \
            -j "${JOBS}" \
            -f "${APP_PREFIX}/lib/${python_with_version}/" \
            || true
        fi

        # For just in case.
        find "${APP_PREFIX}/lib/${python_with_version}/" \
          \( -name '*.opt-1.pyc' -o -name '*.opt-2.pyc' \) \
          -exec rm -v {} \;
      )

      echo "Replacing .py files with .pyc files..."
      move_pyc "${APP_PREFIX}/lib/${python_with_version}"

      mkdir -pv "${APP_PREFIX}/lib/${python_with_version}/lib-dynload/"

      echo
      echo "Copying Python shared libraries..."

      if [ "${TARGET_PLATFORM}" == "win32" ]
      then
        # Copy the Windows specific DLLs (.pyd) to the separate folder;
        # they are dynamically loaded by Python.
        cp -v "${SOURCES_FOLDER_PATH}/${PYTHON3_WIN_SRC_FOLDER_NAME}"/*.pyd \
          "${APP_PREFIX}/lib/${python_with_version}/lib-dynload/"
        # Copy the usual DLLs too; the python*.dll are used, do not remove them.
        cp -v "${SOURCES_FOLDER_PATH}/${PYTHON3_WIN_SRC_FOLDER_NAME}"/*.dll \
          "${APP_PREFIX}/lib/${python_with_version}/lib-dynload/"
      else
        # Copy dynamically loaded modules and rename folder.
        cp -rv "${LIBS_INSTALL_FOLDER_PATH}/lib/python${PYTHON3_VERSION_MAJOR}.${PYTHON3_VERSION_MINOR}"/lib-dynload/* \
          "${APP_PREFIX}/lib/${python_with_version}/lib-dynload/"
      fi
    )
  fi
}

function process_pyc()
{
  local file_path="$1"

  # echo bbb "${file_path}"

  local file_full_name="$(basename "${file_path}")"
  local file_name="$(echo "${file_full_name}" | sed -e 's|\.cpython-[0-9]*\.pyc||')"
  local folder_path="$(dirname $(dirname "${file_path}"))"

  # echo "${folder_path}" "${file_name}"

  if [ -f "${folder_path}/${file_name}.py" ]
  then
    mv "${file_path}" "${folder_path}/${file_name}.pyc"
    rm "${folder_path}/${file_name}.py"
  fi
}

export -f process_pyc

function process_pycache()
{
  local folder_path="$1"

  find ${folder_path} -name '*.pyc' -type f -print0 | xargs -0 -L 1 -I {} bash -c 'process_pyc "{}"'

  if [ $(ls -1 "${folder_path}" | wc -l) -eq 0 ]
  then
    rm -rf "${folder_path}"
  fi
}

export -f process_pycache

function move_pyc()
{
  local folder_path="$1"

  find ${folder_path} -name '__pycache__' -type d -print0 | xargs -0 -L 1 -I {} bash -c 'process_pycache "{}"'
}

# -----------------------------------------------------------------------------

function build_libpng()
{
  # To ensure builds stability, use slightly older releases.
  # https://sourceforge.net/projects/libpng/files/libpng16/
  # https://sourceforge.net/projects/libpng/files/libpng16/older-releases/

  # https://github.com/archlinux/svntogit-packages/blob/packages/libpng/trunk/PKGBUILD
  # https://archlinuxarm.org/packages/aarch64/libpng/files/PKGBUILD
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=libpng-git
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=mingw-w64-libpng

  # https://github.com/Homebrew/homebrew-core/blob/master/Formula/libpng.rb

  # libpng_version="1.2.53"
  # libpng_version="1.6.17"
  # libpng_version="1.6.23" # 2016-06-09
  # libpng_version="1.6.36" # 2018-12-01
  # 2019-04-15, "1.6.37"
  # libpng_SFOLDER="libpng12"
  # libpng_SFOLDER="libpng16"

  local libpng_version="$1"
  local libpng_major_minor_version="$(echo ${libpng_version} | sed -e 's|\([0-9][0-9]*\)\.\([0-9][0-9]*\)\.[0-9].*|\1\2|')"

  local libpng_src_folder_name="libpng-${libpng_version}"

  local libpng_archive="${libpng_src_folder_name}.tar.xz"
  # local libpng_url="https://sourceforge.net/projects/libpng/files/${libpng_SFOLDER}/older-releases/${libpng_version}/${libpng_archive}"
  local libpng_url="https://sourceforge.net/projects/libpng/files/libpng${libpng_major_minor_version}/${libpng_version}/${libpng_archive}"

  local libpng_folder_name="${libpng_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${libpng_folder_name}"

  local libpng_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-libpng-${libpng_version}-installed"
  if [ ! -f "${libpng_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${libpng_url}" "${libpng_archive}" \
      "${libpng_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${libpng_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${libpng_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
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
          echo "Running libpng configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${libpng_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          # config_options+=("--disable-static")
          # From Arm Arch.
          config_options+=("--enable-arm-neon=no")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${libpng_src_folder_name}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${libpng_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libpng_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running libpng make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libpng_folder_name}/make-output-$(ndate).txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${libpng_src_folder_name}" \
        "${libpng_folder_name}"

    )

    touch "${libpng_stamp_file_path}"

  else
    echo "Library libpng already installed."
  fi
}

# See also
# https://archlinuxarm.org/packages/aarch64/libjpeg-turbo/files/PKGBUILD

function build_jpeg()
{
  # http://www.ijg.org
  # http://www.ijg.org/files/

  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=libjpeg9

  # https://github.com/Homebrew/homebrew-core/blob/master/Formula/jpeg.rb

  # Jan 19 10:26 2014 "9a"
  # Jan 17 10:46 2016 "9b"
  # Jan 14 10:10 2018 "9c"
  # Jan 12 10:07 2020 "9d"
  # Jan 16 10:12 2022 "9e"

  local jpeg_version="$1"

  local jpeg_src_folder_name="jpeg-${jpeg_version}"

  local jpeg_archive="jpegsrc.v${jpeg_version}.tar.gz"
  local jpeg_url="http://www.ijg.org/files/${jpeg_archive}"

  local jpeg_folder_name="${jpeg_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${jpeg_folder_name}"

  local jpeg_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-jpeg-${jpeg_version}-installed"
  if [ ! -f "${jpeg_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${jpeg_url}" "${jpeg_archive}" \
        "${jpeg_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${jpeg_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${jpeg_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
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
          echo "Running jpeg configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${jpeg_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          # --enable-shared needed by sdl2_image on CentOS 64-bit and Ubuntu.
          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${jpeg_src_folder_name}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${jpeg_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${jpeg_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running jpeg make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${jpeg_folder_name}/make-output-$(ndate).txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${jpeg_src_folder_name}" \
        "${jpeg_folder_name}"

    )

    touch "${jpeg_stamp_file_path}"

  else
    echo "Library jpeg already installed."
  fi
}

function build_pixman()
{
  # http://www.pixman.org
  # http://cairographics.org/releases/

  # https://archlinuxarm.org/packages/aarch64/pixman/files/PKGBUILD
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=pixman-git
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=mingw-w64-pixman

  # https://github.com/Homebrew/homebrew-core/blob/master/Formula/pixman.rb

  # pixman_version="0.32.6"
  # pixman_version="0.34.0" # 2016-01-31
  # pixman_version="0.38.0" # 2019-02-11
  # 2019-04-10, "0.38.4"
  # 2020-04-19, "0.40.0"

  local pixman_version="$1"

  local pixman_src_folder_name="pixman-${pixman_version}"

  local pixman_archive="${pixman_src_folder_name}.tar.gz"
  local pixman_url="http://cairographics.org/releases/${pixman_archive}"

  local pixman_folder_name="${pixman_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${pixman_folder_name}"

  local pixman_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-pixman-${pixman_version}-installed"
  if [ ! -f "${pixman_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${pixman_url}" "${pixman_archive}" \
      "${pixman_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${pixman_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${pixman_folder_name}"

      # Windows libtool chaks for it.
      mkdir -pv test/lib

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
      if [ "${TARGET_PLATFORM}" == "linux" ]
      then
        LDFLAGS+=" -Wl,-rpath,${LD_LIBRARY_PATH}"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      if [ "${TARGET_PLATFORM}" == "darwin" ] && [[ ${CC} =~ .*gcc.* ]]
      then
        # TODO: check again on Apple Silicon.
        prepare_clang_env ""
      fi

      if [ ! -f "config.status" ]
      then
        (
          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          echo
          echo "Running pixman configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${pixman_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          # config_options+=("--with-gnu-ld")

          # The numerous disables were inspired from Arch, after the initial
          # failed on armhf.
          config_options+=("--disable-static-testprogs")
          config_options+=("--disable-loongson-mmi")
          config_options+=("--disable-vmx")
          config_options+=("--disable-arm-simd")
          config_options+=("--disable-arm-neon")
          config_options+=("--disable-arm-iwmmxt")
          config_options+=("--disable-mmx")
          config_options+=("--disable-sse2")
          config_options+=("--disable-ssse3")
          config_options+=("--disable-mips-dspr2")
          config_options+=("--disable-gtk")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${pixman_src_folder_name}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${pixman_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${pixman_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running pixman make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${pixman_folder_name}/make-output-$(ndate).txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${pixman_src_folder_name}" \
        "${pixman_folder_name}"

    )

    touch "${pixman_stamp_file_path}"

  else
    echo "Library pixman already installed."
  fi
}

# -----------------------------------------------------------------------------


function build_glib()
{
  # http://ftp.gnome.org/pub/GNOME/sources/glib
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=glib2-git
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=mingw-w64-glib2

  # https://github.com/Homebrew/homebrew-core/blob/master/Formula/glib.rb

  # 2015-May-13, "2.44.1"
  # 2017-Mar-13, "2.51.5"
  # 2018-Sep-21, "2.56.3"
  # 2018-Dec-18, "2.56.4"
  # --- Starting with 2.57, the build was migrated to meson (TODO) ---
  # 2018-Aug-23, "2.57.3"
  # 2019-Sep-02, "2.60.7"
  # 2021-May-11, "2.68.4"
  # 2021-Sep-17, "2.70.0"
  # 2021-Dec-03, "2.70.2"
  # 2022-Apr-14, "2.72.1"

  local glib_version="$1"
  local glib_major_version=$(echo ${glib_version} | sed -e 's|\([0-9][0-9]*\)\.\([0-9][0-9]*\)\.[0-9].*|\1|')
  local glib_minor_version=$(echo ${glib_version} | sed -e 's|\([0-9][0-9]*\)\.\([0-9][0-9]*\)\.[0-9].*|\2|')
  local glib_major_minor_version="$(echo ${glib_version} | sed -e 's|\([0-9][0-9]*\)\.\([0-9][0-9]*\)\.[0-9].*|\1.\2|')"

  local glib_src_folder_name="glib-${glib_version}"

  local glib_archive="${glib_src_folder_name}.tar.xz"

  local glib_url="http://ftp.gnome.org/pub/GNOME/sources/glib/${glib_major_minor_version}/${glib_archive}"

  local glib_folder_name="${glib_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${glib_folder_name}"

  local glib_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-glib-${glib_version}-installed"
  if [ ! -f "${glib_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${glib_url}" "${glib_archive}" \
      "${glib_src_folder_name}"

    (
      # Hack, /gio/lib added because libtool needs it on Win32.
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${glib_folder_name}"/gio/lib
      cd "${LIBS_BUILD_FOLDER_PATH}/${glib_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
      if [ "${TARGET_PLATFORM}" == "linux" ]
      then
        LDFLAGS+=" -Wl,-rpath,${LD_LIBRARY_PATH}"
      elif [ "${TARGET_PLATFORM}" == "win32" ]
      then
        LDFLAGS+=" -Wl,--allow-multiple-definition"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      if [ "${TARGET_PLATFORM}" == "darwin" ] && [[ ${CC} =~ .*gcc.* ]]
      then
        # GCC fails with
        # error: unknown type name ‘dispatch_block_t
        prepare_clang_env ""
      fi

      if [ ${glib_major_version} -eq 2 -a ${glib_minor_version} -le 56 ]
      then
        # Up to 2.56 use the old configure.
        if [ ! -f "config.status" ]
        then
          (
            if [ "${IS_DEVELOP}" == "y" ]
            then
              env | sort
            fi

            echo
            echo "Running glib configure..."

            if [ "${IS_DEVELOP}" == "y" ]
            then
              run_verbose bash "${SOURCES_FOLDER_PATH}/${glib_src_folder_name}/configure" --help
            fi

            config_options=()

            config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

            config_options+=("--build=${BUILD}")
            config_options+=("--host=${HOST}")
            config_options+=("--target=${TARGET}")

            # --with-libiconv=gnu required on Linux
            config_options+=("--with-libiconv=gnu")
            config_options+=("--without-pcre")

            config_options+=("--disable-selinux")
            config_options+=("--disable-fam")
            config_options+=("--disable-xattr")
            config_options+=("--disable-libelf")
            config_options+=("--disable-libmount")
            config_options+=("--disable-dtrace")
            config_options+=("--disable-systemtap")
            config_options+=("--disable-coverage")
            config_options+=("--disable-Bsymbolic")
            config_options+=("--disable-znodelete")
            config_options+=("--disable-compile-warnings")
            config_options+=("--disable-installed-tests")
            config_options+=("--disable-always-build-tests")

            run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${glib_src_folder_name}/configure" \
              "${config_options[@]}"

            # Disable SPLICE, it fails on CentOS.
            local gsed_path=$(which gsed)
            if [ ! -z "${gsed_path}" ]
            then
              run_verbose gsed -i -e '/#define HAVE_SPLICE 1/d' config.h
            else
              run_verbose sed -i -e '/#define HAVE_SPLICE 1/d' config.h
            fi

            cp "config.log" "${LOGS_FOLDER_PATH}/${glib_folder_name}/config-log-$(ndate).txt"
          ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${glib_folder_name}/configure-output-$(ndate).txt"
        fi

        (
          echo
          echo "Running glib make..."

          # Build.
          run_verbose make -j ${JOBS}

          if [ "${WITH_STRIP}" == "y" ]
          then
            run_verbose make install-strip
          else
            run_verbose make install
          fi

        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${glib_folder_name}/make-output-$(ndate).txt"
      else
        echo "glib >2.56 not yet implemented"
        exit 1
      fi

      copy_license \
        "${SOURCES_FOLDER_PATH}/${glib_src_folder_name}" \
        "${glib_folder_name}"

    )

    touch "${glib_stamp_file_path}"

  else
    echo "Library glib already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_libxml2()
{
  # http://www.xmlsoft.org
  # ftp://xmlsoft.org/libxml2/
  # https://download.gnome.org/sources/libxml2
  # https://download.gnome.org/sources/libxml2/2.9/libxml2-2.9.14.tar.xz

  # https://gitlab.gnome.org/GNOME/libxml2/-/releases
  # https://gitlab.gnome.org/GNOME/libxml2/-/archive/v2.9.14/libxml2-v2.9.14.tar.bz2

  # https://archlinuxarm.org/packages/aarch64/libxml2/files/PKGBUILD
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=libxml2-git
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=mingw-w64-libxml2

  # Mar 05 2018, "2.9.8"
  # Jan 03 2019, "2.9.9"
  # Oct 30 2019, "2.9.10"
  # May 13 2021, "2.9.11"
  # May 2, 2022, "2.9.14"
  # 2022-Aug-17, "2.10.0"

  local libxml2_version="$1"
  local libxml2_version_major_minor="$(echo ${libxml2_version} | sed -e 's|\([0-9][0-9]*\)\.\([0-9][0-9]*\)\.[0-9].*|\1.\2|')"


  local libxml2_src_folder_name="libxml2-${libxml2_version}"

  local libxml2_archive="${libxml2_src_folder_name}.tar.xz"
  # local libxml2_url="ftp://xmlsoft.org/libxml2/${libxml2_archive}"
  local libxml2_url="https://download.gnome.org/sources/libxml2/${libxml2_version_major_minor}/${libxml2_archive}"

  local libxml2_folder_name="${libxml2_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${libxml2_folder_name}"

  local libxml2_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-libxml2-${libxml2_version}-installed"
  if [ ! -f "${libxml2_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${libxml2_url}" "${libxml2_archive}" \
      "${libxml2_src_folder_name}"

    # Fails if not built in place.
    if [ ! -d "${LIBS_BUILD_FOLDER_PATH}/${libxml2_folder_name}" ]
    then
      (
        cp -r "${libxml2_src_folder_name}" \
          "${LIBS_BUILD_FOLDER_PATH}/${libxml2_folder_name}"

        cd "${LIBS_BUILD_FOLDER_PATH}/${libxml2_folder_name}"

        xbb_activate_installed_dev

        autoreconf -vfi
      )
    fi

    (
      # /lib added due to wrong -Llib used during make.
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${libxml2_folder_name}/lib"
      cd "${LIBS_BUILD_FOLDER_PATH}/${libxml2_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
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
          echo "Running libxml2 configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          config_options+=("--without-python")
          # Not functional on Windows anyway
          if [ "${TARGET_PLATFORM}" == "win32" ]
          then
            config_options+=("--without-catalog")
          fi

          run_verbose bash ${DEBUG} "configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${libxml2_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libxml2_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running libxml2 make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libxml2_folder_name}/make-output-$(ndate).txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${libxml2_src_folder_name}" \
        "${libxml2_folder_name}"

    )

    touch "${libxml2_stamp_file_path}"

  else
    echo "Library libxml2 already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_libedit()
{
  # https://www.thrysoee.dk/editline/
  # https://www.thrysoee.dk/editline/libedit-20210522-3.1.tar.gz
  # https://www.thrysoee.dk/editline/libedit-20210910-3.1.tar.gz

  # https://archlinuxarm.org/packages/aarch64/libedit/files/PKGBUILD

  # 2021-05-22, "20210522-3.1"
  # 2021-09-1-, "20210910-3.1"

  local libedit_version="$1"
  local libedit_version_short="$(echo ${libedit_version} | sed -e 's|[0-9]*-||')"

  local libedit_src_folder_name="libedit-${libedit_version}"
  local libedit_archive="${libedit_src_folder_name}.tar.gz"

  local libedit_url="https://www.thrysoee.dk/editline/${libedit_archive}"

  local libedit_folder_name="libedit-${libedit_version_short}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${libedit_folder_name}"

  local libedit_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-${libedit_folder_name}-installed"
  if [ ! -f "${libedit_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${libedit_url}" "${libedit_archive}" \
      "${libedit_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${libedit_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${libedit_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
      if [ "${TARGET_PLATFORM}" == "linux" ]
      then
        if [ -d "${LIBS_INSTALL_FOLDER_PATH}/include/ncursesw" ]
        then
          CPPFLAGS+=" -I${LIBS_INSTALL_FOLDER_PATH}/include/ncursesw"
        elif [ -d "${LIBS_INSTALL_FOLDER_PATH}/include/ncurses" ]
        then
          CPPFLAGS+=" -I${LIBS_INSTALL_FOLDER_PATH}/include/ncurses"
        else
          echo "No include/ncurses folder."
          exit 1
        fi
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
          echo "Running libedit configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${libedit_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          # config_options+=("--disable-nls")
          # config_options+=("--disable-shared")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${libedit_src_folder_name}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${libedit_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libedit_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running libedit make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose make -j1 check
        fi

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libedit_folder_name}/make-output-$(ndate).txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${libedit_src_folder_name}" \
        "${libedit_folder_name}"

    )

    touch "${libedit_stamp_file_path}"

  else
    echo "Library libedit already installed."
  fi
}

# -----------------------------------------------------------------------------

# Not yet functional.
function build_xar()
{
  # https://github.com/mackyle/xar
  # https://github.com/mackyle/xar/archive/refs/tags/xar-1.6.1.tar.gz

  # 18 Sep 2012, "1.6.1"

  local xar_version="$1"

  local xar_src_folder_name="xar-xar-${xar_version}"

  local xar_archive="xar-${xar_version}.tar.gz"
  # GitHub release archive.
  local xar_github_archive="xar-${xar_version}.tar.gz"
  local xar_github_url="https://github.com/mackyle/xar/archive/refs/tags/${xar_github_archive}"

  local xar_folder_name="xar-${xar_version}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${xar_folder_name}"

  local xar_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-${xar_folder_name}-installed"
  if [ ! -f "${xar_stamp_file_path}" ]
  then

    echo
    echo "xar in-source building"

    cd "${LIBS_BUILD_FOLDER_PATH}"

    if [ ! -d "${LIBS_BUILD_FOLDER_PATH}/${xar_folder_name}" ]
    then
      cd "${LIBS_BUILD_FOLDER_PATH}"

      download_and_extract "${xar_github_url}" "${xar_archive}" \
        "${xar_src_folder_name}"

      if [ "${xar_src_folder_name}" != "${xar_folder_name}" ]
      then
        mv -v "${xar_src_folder_name}" "${xar_folder_name}"
      fi
    fi

    (
      cd "${LIBS_BUILD_FOLDER_PATH}/${xar_folder_name}/xar/"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
      if [ "${TARGET_PLATFORM}" == "linux" ]
      then
        LDFLAGS+=" -Wl,-rpath,${LD_LIBRARY_PATH}"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      if [ ! -x "configure" ]
      then

        echo
        echo "Running xar autogen..."
        run_verbose bash ${DEBUG} "autogen.sh"

      fi

      if [ ! -f "config.status" ]
      then
        (
          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          echo
          echo "Running xar configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "./configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          run_verbose bash ${DEBUG} "./configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${xar_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${xar_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running xar make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose make -j1 check
        fi

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${xar_folder_name}/make-output-$(ndate).txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${xar_src_folder_name}" \
        "${xar_folder_name}"

    )

    touch "${xar_stamp_file_path}"

  else
    echo "Library xar already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_libgpg_error()
{
  # https://gnupg.org/ftp/gcrypt/libgpg-error

  # https://archlinuxarm.org/packages/aarch64/libgpg-error/files/PKGBUILD

  # https://github.com/Homebrew/homebrew-core/blob/master/Formula/libgpg-error.rb

  # 2020-02-07, "1.37"
  # 2021-03-22, "1.42"
  # 2021-11-03, "1.43"

  local libgpg_error_version="$1"

  local libgpg_error_src_folder_name="libgpg-error-${libgpg_error_version}"

  local libgpg_error_archive="${libgpg_error_src_folder_name}.tar.bz2"
  local libgpg_error_url="https://gnupg.org/ftp/gcrypt/libgpg-error/${libgpg_error_archive}"

  local libgpg_error_folder_name="${libgpg_error_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${libgpg_error_folder_name}"

  local libgpg_error_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${libgpg_error_folder_name}-installed"
  if [ ! -f "${libgpg_error_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${libgpg_error_url}" "${libgpg_error_archive}" \
      "${libgpg_error_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${libgpg_error_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${libgpg_error_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
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
          echo "Running libgpg-error configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${libgpg_error_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          config_options+=("--enable-static")

          config_options+=("--disable-dependency-tracking")
          config_options+=("--disable-silent-rules")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${libgpg_error_src_folder_name}/configure" \
            "${config_options[@]}"

          if [ "${TARGET_PLATFORM}" == "darwin" ]
          then
            run_verbose sed -i.bak \
              -e 's|t-lock-single-posix$(EXEEXT)||' \
              "tests/Makefile"
          fi

          # WARN-TEST
          # FAIL: t-syserror (disabled)
          # Interestingly enough, initially (before dismissing install-strip)
          # it passed.
          echo run_verbose sed -i.bak \
            -e 's|t-syserror$(EXEEXT)||' \
            "tests/Makefile"

          cp "config.log" "${LOGS_FOLDER_PATH}/${libgpg_error_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libgpg_error_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running libgpg-error make..."

        # Build.
        run_verbose make -j ${JOBS}

        # make install-strip
        run_verbose make install

        if [ "${WITH_TESTS}" == "y" ]
        then
          # WARN-TEST
          run_verbose make -j1 check
        fi

        run_app "${LIBS_INSTALL_FOLDER_PATH}/bin/gpg-error-config" --libs

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libgpg_error_folder_name}/make-output-$(ndate).txt"
    )

    touch "${libgpg_error_stamp_file_path}"

  else
    echo "Library libgpg-error already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_libgcrypt()
{
  # https://gnupg.org/ftp/gcrypt/libgcrypt
  # https://gnupg.org/ftp/gcrypt/libgcrypt/libgcrypt-1.8.5.tar.bz2

  # https://archlinuxarm.org/packages/aarch64/libgcrypt/files/PKGBUILD

  # https://github.com/Homebrew/homebrew-core/blob/master/Formula/libgcrypt.rb

  # 2019-08-29, "1.8.5"
  # 2021-06-02, "1.8.8"
  # 2021-04-19, "1.9.3" Fails many tests on macOS 10.13
  # 2021-08-22, "1.9.4"

  local libgcrypt_version="$1"

  local libgcrypt_src_folder_name="libgcrypt-${libgcrypt_version}"

  local libgcrypt_archive="${libgcrypt_src_folder_name}.tar.bz2"
  local libgcrypt_url="https://gnupg.org/ftp/gcrypt/libgcrypt/${libgcrypt_archive}"

  local libgcrypt_folder_name="${libgcrypt_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${libgcrypt_folder_name}"

  local libgcrypt_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${libgcrypt_folder_name}-installed"
  if [ ! -f "${libgcrypt_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${libgcrypt_url}" "${libgcrypt_archive}" \
      "${libgcrypt_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${libgcrypt_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${libgcrypt_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB} -v"
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
          echo "Running libgcrypt configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${libgcrypt_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          config_options+=("--with-libgpg-error-prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--enable-static")

          if [ "${TARGET_ARCH}" == "arm64" ]
          then
            config_options+=("--disable-neon-support")
            config_options+=("--disable-arm-crypto-support")
          fi

          config_options+=("--disable-doc")
          config_options+=("--disable-large-data-tests")

          # For Darwin, there are problems with the assembly code.
          config_options+=("--disable-asm")
          config_options+=("--disable-amd64-as-feature-detection")

          config_options+=("--disable-dependency-tracking")
          config_options+=("--disable-silent-rules")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${libgcrypt_src_folder_name}/configure" \
            "${config_options[@]}"

          if false # [ "${TARGET_ARCH}" == "arm64" ]
          then
            # fix screwed up capability detection
            sed -i.bak -e '/HAVE_GCC_INLINE_ASM_AARCH32_CRYPTO 1/d' "config.h"
            sed -i.bak -e '/HAVE_GCC_INLINE_ASM_NEON 1/d' "config.h"
          fi

          cp "config.log" "${LOGS_FOLDER_PATH}/${libgcrypt_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libgcrypt_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running libgcrypt make..."

        # Build.
        run_verbose make -j ${JOBS}

        # make install-strip
        run_verbose make install

        # Check after install, otherwise mac test fails:
        # dyld: Library not loaded: /Users/ilg/opt/xbb/lib/libgcrypt.20.dylib
        # Referenced from: /Users/ilg/Work/xbb-3.1-macosx-10.15.3-x86_64/build/libs/libgcrypt-1.8.5/tests/.libs/random

        run_verbose make -j1 check

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libgcrypt_folder_name}/make-output-$(ndate).txt"
    )

    touch "${libgcrypt_stamp_file_path}"

  else
    echo "Library libgcrypt already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_libassuan()
{
  # https://gnupg.org/ftp/gcrypt/libassuan
  # https://gnupg.org/ftp/gcrypt/libassuan/libassuan-2.5.3.tar.bz2

  # https://archlinuxarm.org/packages/aarch64/libassuan/files/PKGBUILD

  # 2019-02-11, "2.5.3"
  # 2021-03-22, "2.5.5"

  local libassuan_version="$1"

  local libassuan_src_folder_name="libassuan-${libassuan_version}"

  local libassuan_archive="${libassuan_src_folder_name}.tar.bz2"
  local libassuan_url="https://gnupg.org/ftp/gcrypt/libassuan/${libassuan_archive}"

  local libassuan_folder_name="${libassuan_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${libassuan_folder_name}"

  local libassuan_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${libassuan_folder_name}-installed"
  if [ ! -f "${libassuan_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${libassuan_url}" "${libassuan_archive}" \
      "${libassuan_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${libassuan_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${libassuan_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
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
          echo "Running libassuan configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${libassuan_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          config_options+=("--with-libgpg-error-prefix=${LIBS_INSTALL_FOLDER_PATH}")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${libassuan_src_folder_name}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${libassuan_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libassuan_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running libassuan make..."

        # Build.
        run_verbose make -j ${JOBS}

        # make install-strip
        run_verbose make install

        if [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose make -j1 check
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libassuan_folder_name}/make-output-$(ndate).txt"
    )

    touch "${libassuan_stamp_file_path}"

  else
    echo "Library libassuan already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_libksba()
{
  # https://gnupg.org/ftp/gcrypt/libksba
  # https://gnupg.org/ftp/gcrypt/libksba/libksba-1.3.5.tar.bz2

  # https://archlinuxarm.org/packages/aarch64/libksba/files/PKGBUILD

  # https://github.com/Homebrew/homebrew-core/blob/master/Formula/libksba.rb

  # 2016-08-22, "1.3.5"
  # 2021-06-10, "1.6.0"

  local libksba_version="$1"

  local libksba_src_folder_name="libksba-${libksba_version}"

  local libksba_archive="${libksba_src_folder_name}.tar.bz2"
  local libksba_url="https://gnupg.org/ftp/gcrypt/libksba/${libksba_archive}"

  local libksba_folder_name="${libksba_src_folder_name}"

  local libksba_patch_file_path="${libksba_folder_name}.patch"

  mkdir -pv "${LOGS_FOLDER_PATH}/${libksba_folder_name}"

  local libksba_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${libksba_folder_name}-installed"
  if [ ! -f "${libksba_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${libksba_url}" "${libksba_archive}" \
      "${libksba_src_folder_name}" "${libksba_patch_file_path}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${libksba_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${libksba_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
      if [ "${TARGET_PLATFORM}" == "linux" ]
      then
        LDFLAGS+=" -Wl,-rpath,${LD_LIBRARY_PATH}"
      fi

      export CC_FOR_BUILD="${CC}"

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
          echo "Running libksba configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${libksba_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          config_options+=("--with-libgpg-error-prefix=${LIBS_INSTALL_FOLDER_PATH}")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${libksba_src_folder_name}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${libksba_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libksba_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running libksba make..."

        # Build.
        run_verbose make -j ${JOBS}

        # make install-strip
        run_verbose make install

        if [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose make -j1 check
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libksba_folder_name}/make-output-$(ndate).txt"
    )

    touch "${libksba_stamp_file_path}"

  else
    echo "Library libksba already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_npth()
{
  # https://gnupg.org/ftp/gcrypt/npth
  # https://gnupg.org/ftp/gcrypt/npth/npth-1.6.tar.bz2

  # https://archlinuxarm.org/packages/aarch64/npth/files/PKGBUILD

  # https://github.com/Homebrew/homebrew-core/blob/master/Formula/npth.rb

  # 2018-07-16, "1.6"

  local npth_version="$1"

  local npth_src_folder_name="npth-${npth_version}"

  local npth_archive="${npth_src_folder_name}.tar.bz2"
  local npth_url="https://gnupg.org/ftp/gcrypt/npth/${npth_archive}"

  local npth_folder_name="${npth_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${npth_folder_name}"

  local npth_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${npth_folder_name}-installed"
  if [ ! -f "${npth_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${npth_url}" "${npth_archive}" \
      "${npth_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${npth_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${npth_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
      if [ "${TARGET_PLATFORM}" == "linux" ]
      then
        LDFLAGS+=" -Wl,-rpath,${LD_LIBRARY_PATH}"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      if [ "${TARGET_PLATFORM}" == "darwin" ] && [[ ${CC} =~ .*gcc.* ]]
      then
        # /usr/include/os/base.h:113:20: error: missing binary operator before token "("
        # #if __has_extension(attribute_overloadable)
        prepare_clang_env ""
      fi

      if [ ! -f "config.status" ]
      then
        (
          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          echo
          echo "Running npth configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${npth_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${npth_src_folder_name}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${npth_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${npth_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running npth make..."

        # Build.
        run_verbose make -j ${JOBS}

        # make install-strip
        run_verbose make install

        if [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose make -j1 check
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${npth_folder_name}/make-output-$(ndate).txt"
    )

    touch "${npth_stamp_file_path}"

  else
    echo "Library npth already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_nettle()
{
  # https://www.lysator.liu.se/~nisse/nettle/
  # https://ftp.gnu.org/gnu/nettle/

  # https://archlinuxarm.org/packages/aarch64/nettle/files/PKGBUILD
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=nettle-git

  # https://github.com/Homebrew/homebrew-core/blob/master/Formula/nettle.rb

  # 2017-11-19, "3.4"
  # 2018-12-04, "3.4.1"
  # 2019-06-27, "3.5.1"
  # 2021-06-07, "3.7.3"

  local nettle_version="$1"

  local nettle_src_folder_name="nettle-${nettle_version}"

  local nettle_archive="${nettle_src_folder_name}.tar.gz"
  local nettle_url="ftp://ftp.gnu.org/gnu/nettle/${nettle_archive}"

  local nettle_folder_name="${nettle_src_folder_name}"

  local nettle_patch_file_path="${nettle_folder_name}.patch"

  mkdir -pv "${LOGS_FOLDER_PATH}/${nettle_folder_name}"

  local nettle_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${nettle_folder_name}-installed"
  if [ ! -f "${nettle_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${nettle_url}" "${nettle_archive}" \
      "${nettle_src_folder_name}" "${nettle_patch_file_path}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${nettle_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${nettle_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
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
          echo "Running nettle configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${nettle_src_folder_name}/configure" --help
          fi

          # -disable-static

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          # config_options+=("--enable-mini-gmp")
          config_options+=("--enable-shared")

          config_options+=("--disable-documentation")
          config_options+=("--disable-arm-neon")
          config_options+=("--disable-assembler")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${nettle_src_folder_name}/configure" \
            "${config_options[@]}"

          if false # is_darwin # && [ "${XBB_LAYER}" == "xbb-bootstrap" ]
          then
            # dlopen failed: dlopen(../libnettle.so, 2): image not found
            # /Users/ilg/Work/xbb-3.1-macosx-x86_64/sources/nettle-3.5.1/run-tests: line 57: 46731 Abort trap: 6           "$1" $testflags
            # darwin: FAIL: dlopen
            run_verbose sed -i.bak \
              -e 's| dlopen-test$(EXEEXT)||' \
              "testsuite/Makefile"
          fi

          cp "config.log" "${LOGS_FOLDER_PATH}/${nettle_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${nettle_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running nettle make..."

        # Build.
        run_verbose make -j ${JOBS}

        # make install-strip
        # For unknown reasons, on 32-bits make install-info fails
        # (`install-info --info-dir="/opt/xbb/share/info" nettle.info` returns 1)
        # Make the other install targets.
        run_verbose make install-headers install-static install-pkgconfig install-shared-nettle install-shared-hogweed

        if [ "${WITH_TESTS}" == "y" ]
        then
          if false # is_darwin
          then
            # dlopen failed: dlopen(../libnettle.so, 2): image not found
            # /Users/ilg/Work/xbb-3.1-macosx-x86_64/sources/nettle-3.5.1/run-tests: line 57: 46731 Abort trap: 6           "$1" $testflags
            # darwin: FAIL: dlopen
            # WARN-TEST
            run_verbose make -j1 -k check
          else
            # Takes very long on armhf.
            run_verbose make -j1 -k check
          fi
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${nettle_folder_name}/make-output-$(ndate).txt"
    )

    touch "${nettle_stamp_file_path}"

  else
    echo "Library nettle already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_libusb()
{
  # https://libusb.info/
  # https://github.com/libusb/libusb/releases/
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=mingw-w64-libusb
  # https://github.com/libusb/libusb/releases/download/v1.0.24/libusb-1.0.24.tar.bz2

  # https://github.com/Homebrew/homebrew-core/blob/master/Formula/libusb.rb

  # 2015-09-14, 1.0.20
  # 2018-03-25, 1.0.22
  # 2020-12-11, 1.0.24
  # 2022-04-10, "1.0.26"

  local libusb_version="$1"

  local libusb_src_folder_name="libusb-${libusb_version}"

  local libusb_archive="${libusb_src_folder_name}.tar.bz2"
  local libusb_url="https://github.com/libusb/libusb/releases/download/v${libusb_version}/${libusb_archive}"

  local libusb_folder_name="${libusb_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${libusb_folder_name}"

  local libusb_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-libusb-${libusb_version}-installed"
  if [ ! -f "${libusb_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${libusb_url}" "${libusb_archive}" \
      "${libusb_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${libusb_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${libusb_folder_name}"

      xbb_activate_installed_dev

      if [ "${TARGET_PLATFORM}" == "darwin" ] && [[ ${CC} =~ .*gcc.* ]]
      then
        # /Users/ilg/Work/qemu-arm-6.2.0-1/darwin-x64/sources/libusb-1.0.24/libusb/os/darwin_usb.c: In function 'darwin_handle_transfer_completion':
        # /Users/ilg/Work/qemu-arm-6.2.0-1/darwin-x64/sources/libusb-1.0.24/libusb/os/darwin_usb.c:2151:3: error: variable-sized object may not be initialized
        # 2151 |   const char *transfer_types[max_transfer_type + 1] = {"control", "isoc", "bulk", "interrupt", "bulk-stream"};
        prepare_clang_env ""
      fi

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
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
          echo "Running libusb configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${libusb_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          config_options+=("--disable-dependency-tracking")
          if [ "${TARGET_PLATFORM}" == "linux" ]
          then
            # On 32-bit Arm `/lib/arm-linux-gnueabihf/libudev.so.1` has
            # a dependency on the system `libgcc_s.so.1` and makes
            # life very difficult.
            config_options+=("--disable-udev")
          fi

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${libusb_src_folder_name}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${libusb_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libusb_folder_name}/configure-output-$(ndate).txt"

      fi

      (
        echo
        echo "Running libusb make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libusb_folder_name}/make-output-$(ndate).txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${libusb_src_folder_name}" \
        "${libusb_folder_name}"
    )

    touch "${libusb_stamp_file_path}"

  else
    echo "Library libusb already installed."
  fi
}


# Required by Windows.
function build_libusb_w32()
{
  # https://sourceforge.net/projects/libusb-win32/files/libusb-win32-releases/
  # 2012-01-17, 1.2.6.0
  # libusb_w32_version="1.2.6.0" # +PATCH!

  local libusb_w32_version="$1"

  local libusb_w32_prefix="libusb-win32"
  local libusb_w32_prefix_version="${libusb_w32_prefix}-${libusb_w32_version}"

  local libusb_w32_src_folder_name="${libusb_w32_prefix}-src-${libusb_w32_version}"

  local libusb_w32_archive="${libusb_w32_src_folder_name}.zip"
  local libusb_w32_url="http://sourceforge.net/projects/libusb-win32/files/libusb-win32-releases/${libusb_w32_version}/${libusb_w32_archive}"

  local libusb_w32_folder_name="${libusb_w32_prefix}-${libusb_w32_version}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${libusb_w32_folder_name}"

  local libusb_w32_patch="libusb-win32-${libusb_w32_version}-mingw-w64.patch"

  local libusb_w32_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-{libusb_w32_folder_name}-installed"
  if [ ! -f "${libusb_w32_stamp_file_path}" ]
  then

    echo
    echo "libusb_w32 in-source building"

    if [ ! -d "${LIBS_BUILD_FOLDER_PATH}/${libusb_w32_folder_name}" ]
    then
      cd "${LIBS_BUILD_FOLDER_PATH}"

      # Do not add the patch here, it must be done after dos2unix.
      download_and_extract "${libusb_w32_url}" "${libusb_w32_archive}" \
        "${libusb_w32_src_folder_name}"

      if [ "${libusb_w32_src_folder_name}" != "${libusb_w32_folder_name}" ]
      then
        mv -v "${libusb_w32_src_folder_name}" "${libusb_w32_folder_name}"
      fi

      cd "${LIBS_BUILD_FOLDER_PATH}/${libusb_w32_folder_name}"

      # Patch from:
      # https://gitorious.org/jtag-tools/openocd-mingw-build-scripts

      # The conversions are needed to avoid errors like:
      # 'Hunk #1 FAILED at 31 (different line endings).'
      run_verbose dos2unix src/install.c
      run_verbose dos2unix src/install_filter_win.c
      run_verbose dos2unix src/registry.c

      if [ -f "${BUILD_GIT_PATH}/patches/${libusb_w32_patch}" ]
      then
        run_verbose patch -p0 < "${BUILD_GIT_PATH}/patches/${libusb_w32_patch}"
      fi
    fi

    (
      echo
      echo "Running libusb-win32 make..."

      cd "${LIBS_BUILD_FOLDER_PATH}/${libusb_w32_folder_name}"

      xbb_activate_installed_dev

      # Build.
      (
          CPPFLAGS="${XBB_CPPFLAGS}"
          CFLAGS="${XBB_CFLAGS_NO_W}"
          CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

          LDFLAGS="${XBB_LDFLAGS_LIB}"

          export CPPFLAGS
          export CFLAGS
          export CXXFLAGS
          export LDFLAGS

          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          run_verbose make \
            host_prefix=${CROSS_COMPILE_PREFIX} \
            host_prefix_x86=i686-w64-mingw32 \
            dll

          # Manually install, could not find a make target.
          mkdir -pv "${LIBS_INSTALL_FOLDER_PATH}/bin"

          # Skipping it does not remove the reference from openocd, so for the
          # moment it is preserved.
          cp -v "${LIBS_BUILD_FOLDER_PATH}/${libusb_w32_folder_name}/libusb0.dll" \
            "${LIBS_INSTALL_FOLDER_PATH}/bin"

          mkdir -pv "${LIBS_INSTALL_FOLDER_PATH}/lib"
          cp -v "${LIBS_BUILD_FOLDER_PATH}/${libusb_w32_folder_name}/libusb.a" \
            "${LIBS_INSTALL_FOLDER_PATH}/lib"

          mkdir -pv "${LIBS_INSTALL_FOLDER_PATH}/lib/pkgconfig"
          sed -e "s|XXX|${LIBS_INSTALL_FOLDER_PATH}|" \
            "${helper_folder_path}/pkgconfig/${libusb_w32_prefix_version}.pc" \
            > "${LIBS_INSTALL_FOLDER_PATH}/lib/pkgconfig/libusb.pc"

          mkdir -pv "${LIBS_INSTALL_FOLDER_PATH}/include/libusb"
          cp -v "${LIBS_BUILD_FOLDER_PATH}/${libusb_w32_folder_name}/src/lusb0_usb.h" \
            "${LIBS_INSTALL_FOLDER_PATH}/include/libusb/usb.h"

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libusb_w32_folder_name}/make-output-$(ndate).txt"

      copy_license \
        "${LIBS_BUILD_FOLDER_PATH}/${libusb_w32_folder_name}" \
        "${libusb_w32_folder_name}"
    )

    touch "${libusb_w32_stamp_file_path}"

  else
    echo "Library libusb-w32 already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_vde()
{
  # Ethernet compliant virtual network
  # https://github.com/virtualsquare/vde-2
  # https://vde.sourceforge.io/
  # https://sourceforge.net/projects/vde/files/vde2/
  # https://downloads.sourceforge.net/project/vde/vde2/2.3.2/vde2-2.3.2.tar.gz

  # https://github.com/archlinux/svntogit-packages/blob/packages/vde2/trunk/PKGBUILD

  # https://github.com/Homebrew/homebrew-core/blob/master/Formula/vde.rb

  # 2011-11-23 "2.3.2"

  local vde_version="$1"

  local vde_src_folder_name="vde2-${vde_version}"

  local vde_archive="${vde_src_folder_name}.tar.gz"
  local vde_url="https://downloads.sourceforge.net/project/vde/vde2/${vde_version}/${vde_archive}"

  local vde_folder_name="${vde_src_folder_name}"
  local vde_patch_file_patch="${vde_folder_name}.patch.diff"

  mkdir -pv "${LOGS_FOLDER_PATH}/${vde_folder_name}"

  local vde_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${vde_folder_name}-installed"
  if [ ! -f "${vde_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${vde_url}" "${vde_archive}" \
      "${vde_src_folder_name}" "${vde_patch_file_patch}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${vde_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${vde_folder_name}"

      xbb_activate_installed_dev

      # On debug, -O[01] fail with:
      # Undefined symbols for architecture x86_64:
      #   "_ltonstring", referenced from:
      #       _fst_in_bpdu in fstp.o
      #   "_nstringtol", referenced from:
      #       _fst_in_bpdu in fstp.o
      #       _fstprintactive in fstp.o

      CPPFLAGS="$(echo "${XBB_CPPFLAGS}" | sed -e 's|-O0|-O2|')"
      CFLAGS="$(echo "${XBB_CFLAGS_NO_W}" | sed -e 's|-O0|-O2|')"
      CXXFLAGS="$(echo "${XBB_CXXFLAGS_NO_W}" | sed -e 's|-O0|-O2|')"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
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
          echo "Running vde configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${vde_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          config_options+=("--disable-python")
          # src/vde_cryptcab/cryptcab.c:25:23: error: tentative definition has type 'EVP_CIPHER_CTX' (aka 'struct evp_cipher_ctx_st') that is never completed
          config_options+=("--disable-cryptcab")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${vde_src_folder_name}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${vde_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${vde_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running vde make..."

        # Build.
        # *** No rule to make target '../../src/lib/libvdemgmt.la', needed by 'libvdesnmp.la'.  Stop.
        run_verbose make # -j ${JOBS}

        if [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose make -j1 check
        fi

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${vde_folder_name}/make-output-$(ndate).txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${vde_src_folder_name}" \
        "${vde_folder_name}"

    )

    touch "${vde_stamp_file_path}"

  else
    echo "Library vde already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_libpcap()
{
  # Portable library for network traffic capture
  # https://www.tcpdump.org/
  # https://www.tcpdump.org/release/
  # https://www.tcpdump.org/release/libpcap-1.10.1.tar.gz

  # https://github.com/archlinux/svntogit-packages/blob/packages/libpcap/trunk/PKGBUILD

  # https://github.com/Homebrew/homebrew-core/blob/master/Formula/libpcap.rb

  # June 9, 2021 "1.10.1"

  local libpcap_version="$1"

  local libpcap_src_folder_name="libpcap-${libpcap_version}"

  local libpcap_archive="${libpcap_src_folder_name}.tar.gz"
  local libpcap_url="https://www.tcpdump.org/release/${libpcap_archive}"

  local libpcap_folder_name="${libpcap_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${libpcap_folder_name}"

  local libpcap_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${libpcap_folder_name}-installed"
  if [ ! -f "${libpcap_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${libpcap_url}" "${libpcap_archive}" \
      "${libpcap_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${libpcap_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${libpcap_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
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
          echo "Running libpcap configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${libpcap_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          # HomeBrew
          config_options+=("--disable-universal")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${libpcap_src_folder_name}/configure" \
            "${config_options[@]}"

          cp "config.log" "${LOGS_FOLDER_PATH}/${libpcap_folder_name}/config-log-$(ndate).txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libpcap_folder_name}/configure-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running libpcap make..."

        # Build.
        run_verbose make -j ${JOBS}

        run_verbose make install

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libpcap_folder_name}/make-output-$(ndate).txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${libpcap_src_folder_name}" \
        "${libpcap_folder_name}"

    )

    touch "${libpcap_stamp_file_path}"

  else
    echo "Library libpcap already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_libssh()
{
  # C library SSHv1/SSHv2 client and server protocols
  # https://www.libssh.org/
  # https://www.libssh.org/files/
  # https://www.libssh.org/files/0.9/libssh-0.9.6.tar.xz

  # https://github.com/archlinux/svntogit-packages/blob/packages/libssh/trunk/PKGBUILD
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=mingw-w64-libssh

  # https://github.com/msys2/MINGW-packages/blob/master/mingw-w64-libssh/PKGBUILD

  # https://github.com/Homebrew/homebrew-core/blob/master/Formula/libssh.rb

  # 2021-08-26 "0.9.6"

  local libssh_version="$1"
  local libssh_major_minor_version="$(echo ${libssh_version} | sed -e 's|\([0-9][0-9]*\)\.\([0-9][0-9]*\)\.[0-9].*|\1.\2|')"

  local libssh_src_folder_name="libssh-${libssh_version}"

  local libssh_archive="${libssh_src_folder_name}.tar.xz"
  local libssh_url="https://www.libssh.org/files/${libssh_major_minor_version}/${libssh_archive}"

  local libssh_folder_name="${libssh_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${libssh_folder_name}"

  local libssh_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${libssh_folder_name}-installed"
  if [ ! -f "${libssh_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${libssh_url}" "${libssh_archive}" \
      "${libssh_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${libssh_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${libssh_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
      if [ "${TARGET_PLATFORM}" == "linux" ]
      then
        LDFLAGS+=" -Wl,-rpath,${LD_LIBRARY_PATH}"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      local build_type
      if [ "${IS_DEBUG}" == "y" ]
      then
        build_type=Debug
      else
        build_type=Release
      fi

      if [ ! -f "CMakeCache.txt" ]
      then
        (
          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          echo
          echo "Running libssh cmake..."

          config_options=()

          config_options+=("-DCMAKE_INSTALL_PREFIX=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("-DBUILD_STATIC_LIB=ON")
          config_options+=("-DWITH_SYMBOL_VERSIONING=OFF")

          # From Arch.
          config_options+=("-DWITH_GSSAPI=OFF")

          # Since CMake insists on picking the system one.
          config_options+=("-DWITH_ZLIB=OFF")

          if [ "${TARGET_PLATFORM}" == "linux" ]
          then
            # On Linux
            # undefined reference to `__stack_chk_guard'
            config_options+=("-DWITH_STACK_PROTECTOR=OFF")
            config_options+=("-DWITH_STACK_PROTECTOR_STRONG=OFF")
            # config_options+=("-DWITH_STACK_CLASH_PROTECTION=OFF")
          fi

          run_verbose cmake \
            "${config_options[@]}" \
            \
            "${SOURCES_FOLDER_PATH}/${libssh_src_folder_name}"

        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libssh_folder_name}/cmake-output-$(ndate).txt"
      fi

      (
        echo
        echo "Running libssh make..."

        # Build.
        run_verbose make -j ${JOBS}

        run_verbose make install

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libssh_folder_name}/make-output-$(ndate).txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${libssh_src_folder_name}" \
        "${libssh_folder_name}"

    )

    touch "${libssh_stamp_file_path}"

  else
    echo "Library libssh already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_sdl2()
{
  # https://www.libsdl.org/
  # https://www.libsdl.org/release

  # https://archlinuxarm.org/packages/aarch64/sdl2/files/PKGBUILD
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=sdl2-hg
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=mingw-w64-sdl2

  # https://github.com/Homebrew/homebrew-core/blob/master/Formula/sdl2.rb

  # sdl2_version="2.0.3" # 2014-03-16
  # sdl2_version="2.0.5" # 2016-10-20
  # sdl2_version="2.0.9" # 2018-10-31
  # 2021-11-30, "2.0.18"
  # 2022-04-25, "2.0.22"

  local sdl2_version="$1"

  local sdl2_src_folder_name="SDL2-${sdl2_version}"

  local sdl2_archive="${sdl2_src_folder_name}.tar.gz"
  local sdl2_url="https://www.libsdl.org/release/${sdl2_archive}"

  local sdl2_folder_name="${sdl2_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${sdl2_folder_name}"

  local sdl2_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-sdl2-${sdl2_version}-installed"
  if [ ! -f "${sdl2_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${sdl2_url}" "${sdl2_archive}" \
      "${sdl2_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${sdl2_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${sdl2_folder_name}"

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
      if [ "${TARGET_PLATFORM}" == "linux" ]
      then
        LDFLAGS+=" -Wl,-rpath,${LD_LIBRARY_PATH}"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      if [ "${TARGET_PLATFORM}" == "darwin" ] && [[ ${CC} =~ .*gcc.* ]]
      then
        # GNU GCC fails with
        #  CC     build/SDL_syspower.lo
        # In file included from //System/Library/Frameworks/CoreFoundation.framework/Headers/CFPropertyList.h:13,
        #                 from //System/Library/Frameworks/CoreFoundation.framework/Headers/CoreFoundation.h:55,
        #                 from /Users/ilg/Work/qemu-riscv-2.8.0-9/sources/SDL2-2.0.9/src/power/macosx/SDL_syspower.c:26:
        # //System/Library/Frameworks/CoreFoundation.framework/Headers/CFStream.h:249:59: error: unknown type name ‘dispatch_queue_t’
        prepare_clang_env ""
      fi

      if [ ! -f "config.status" ]
      then

        (
          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          echo
          echo "Running sdl2 configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${sdl2_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          config_options+=("--enable-video")
          config_options+=("--disable-audio")
          config_options+=("--disable-joystick")
          config_options+=("--disable-haptic")

          if [ "${TARGET_PLATFORM}" == "win32" ]
          then
            :
          elif [ "${TARGET_PLATFORM}" == "linux" ]
          then
            config_options+=("--enable-video-opengl")
            config_options+=("--enable-video-x11")
          elif [ "${TARGET_PLATFORM}" == "darwin" ]
          then
            config_options+=("--without-x")
          fi

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${sdl2_src_folder_name}/configure" \
            ${config_options[@]}

          cp "config.log" "${LOGS_FOLDER_PATH}/${sdl2_folder_name}/config-log.txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${sdl2_folder_name}/configure-output.txt"

      fi

      (
        echo
        echo "Running sdl2 make..."

        # Build.
        run_verbose make -j ${JOBS}

        run_verbose make install

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${sdl2_folder_name}/make-output.txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${sdl2_src_folder_name}" \
        "${sdl2_folder_name}"

    )

    touch "${sdl2_stamp_file_path}"

  else
    echo "Library sdl2 already installed."
  fi
}

function build_sdl2_image()
{
  # https://www.libsdl.org/projects/SDL_image/
  # https://www.libsdl.org/projects/SDL_image/release

  # https://archlinuxarm.org/packages/aarch64/sdl2_image/files
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=mingw-w64-sdl2_image

  # https://github.com/Homebrew/homebrew-core/blob/master/Formula/sdl2_image.rb

  # sdl2_image_version="1.1"
  # sdl2_image_version="2.0.1" # 2016-01-03
  # sdl2_image_version="2.0.3" # 2018-03-01
  # sdl2_image_version="2.0.4" # 2018-10-31
  # 2019-07-01, "2.0.5"

  local sdl2_image_version="$1"

  local sdl2_image_src_folder_name="SDL2_image-${sdl2_image_version}"

  local sdl2_image_archive="${sdl2_image_src_folder_name}.tar.gz"
  local sdl2_image_url="https://www.libsdl.org/projects/SDL_image/release/${sdl2_image_archive}"

  local sdl2_image_folder_name="${sdl2_image_src_folder_name}"

  mkdir -pv "${LOGS_FOLDER_PATH}/${sdl2_image_folder_name}"

  local sdl2_image_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-sdl2-image-${sdl2_image_version}-installed"
  if [ ! -f "${sdl2_image_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${sdl2_image_url}" "${sdl2_image_archive}" \
      "${sdl2_image_src_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${sdl2_image_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${sdl2_image_folder_name}"

      # The windows build checks this.
      mkdir -pv lib

      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"
      OBJCFLAGS="${XBB_CFLAGS_NO_W}"

      LDFLAGS="${XBB_LDFLAGS_LIB}"
      if [ "${TARGET_PLATFORM}" == "linux" ]
      then
        LDFLAGS+=" -Wl,-rpath,${LD_LIBRARY_PATH}"
      fi
      if [ "${IS_DEVELOP}" == "y" ]
      then
        LDFLAGS+=" -v"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export OBJCFLAGS
      export LDFLAGS

      if [ "${TARGET_PLATFORM}" == "darwin" ]
      then
        export OBJC=clang
      fi

      # export LIBS="-lpng16 -ljpeg"

      env | sort

      if [ ! -f "config.status" ]
      then

        (
          if [ "${IS_DEVELOP}" == "y" ]
          then
            env | sort
          fi

          echo
          echo "Running sdl2-image configure..."

          if [ "${IS_DEVELOP}" == "y" ]
          then
            run_verbose bash "${SOURCES_FOLDER_PATH}/${sdl2_image_src_folder_name}/configure" --help
          fi

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")

          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          config_options+=("--enable-jpg")
          config_options+=("--enable-png")

          config_options+=("--disable-sdltest")
          config_options+=("--disable-jpg-shared")
          config_options+=("--disable-png-shared")
          config_options+=("--disable-bmp")
          config_options+=("--disable-gif")
          config_options+=("--disable-lbm")
          config_options+=("--disable-pcx")
          config_options+=("--disable-pnm")
          config_options+=("--disable-tga")
          config_options+=("--disable-tif")
          config_options+=("--disable-tif-shared")
          config_options+=("--disable-xcf")
          config_options+=("--disable-xpm")
          config_options+=("--disable-xv")
          config_options+=("--disable-webp")
          config_options+=("--disable-webp-shared")

          if [ "${TARGET_PLATFORM}" == "darwin" ]
          then
            config_options+=("--enable-imageio")
          fi

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${sdl2_image_src_folder_name}/configure" \
            ${config_options[@]}

          cp "config.log" "${LOGS_FOLDER_PATH}/${sdl2_image_folder_name}/config-log.txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${sdl2_image_folder_name}/configure-output.txt"

      fi

      (
        echo
        echo "Running sdl2-image make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${sdl2_image_folder_name}/make-output.txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${sdl2_image_src_folder_name}" \
        "${sdl2_image_folder_name}"

    )

    touch "${sdl2_image_stamp_file_path}"

  else
    echo "Library sdl2-image already installed."
  fi
}

# -----------------------------------------------------------------------------
