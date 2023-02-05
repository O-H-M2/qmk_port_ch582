# -----------------------------------------------------------------------------

# Helper script used in the second edition of the xPack build 
# scripts. As the name implies, it should contain only functions and 
# should be included with 'source' by the build scripts (both native
# and container).

# -----------------------------------------------------------------------------

# Default empty definition, if XBB is available, it should
# redefine it.
function xbb_activate()
{
  :
}

# Add the freshly built binaries.
function xbb_activate_installed_bin()
{
  # Add the XBB bin to the PATH.
  PATH="${LIBS_INSTALL_FOLDER_PATH}/bin:${PATH}"

  # Add the XBB lib to the LD_LIBRARY_PATH.
  LD_LIBRARY_PATH="${LIBS_INSTALL_FOLDER_PATH}/lib:${LD_LIBRARY_PATH}"

  if [ -d "${LIBS_INSTALL_FOLDER_PATH}/lib64" ]
  then
    # On 64-bit systems, add lib64 to the LD_LIBRARY_PATH.
    LD_LIBRARY_PATH="${LIBS_INSTALL_FOLDER_PATH}/lib64:${LD_LIBRARY_PATH}"
  fi

  export PATH
  export LD_LIBRARY_PATH
}

# Add the freshly built headrs and libraries.
function xbb_activate_installed_dev()
{
  # Add XBB include in front of XBB_CPPFLAGS.
  XBB_CPPFLAGS="-I${LIBS_INSTALL_FOLDER_PATH}/include ${XBB_CPPFLAGS}"

  # Add XBB lib in front of XBB_LDFLAGS.
  XBB_LDFLAGS="-L${LIBS_INSTALL_FOLDER_PATH}/lib ${XBB_LDFLAGS}"
  XBB_LDFLAGS_LIB="-L${LIBS_INSTALL_FOLDER_PATH}/lib ${XBB_LDFLAGS_LIB}"
  XBB_LDFLAGS_APP="-L${LIBS_INSTALL_FOLDER_PATH}/lib ${XBB_LDFLAGS_APP}"
  XBB_LDFLAGS_APP_STATIC_GCC="-L${LIBS_INSTALL_FOLDER_PATH}/lib ${XBB_LDFLAGS_APP_STATIC_GCC}"

  # Add XBB lib in front of PKG_CONFIG_PATH.
  PKG_CONFIG_PATH="${LIBS_INSTALL_FOLDER_PATH}/lib/pkgconfig:${PKG_CONFIG_PATH}"

  LD_LIBRARY_PATH="${LIBS_INSTALL_FOLDER_PATH}/lib:${LD_LIBRARY_PATH}"

  if [ -d "${LIBS_INSTALL_FOLDER_PATH}/lib64" ]
  then
    # For 64-bit systems, add XBB lib64 in front of paths.
    XBB_LDFLAGS="-L${LIBS_INSTALL_FOLDER_PATH}/lib64 ${XBB_LDFLAGS_LIB}"
    XBB_LDFLAGS_LIB="-L${LIBS_INSTALL_FOLDER_PATH}/lib64 ${XBB_LDFLAGS_LIB}"
    XBB_LDFLAGS_APP="-L${LIBS_INSTALL_FOLDER_PATH}/lib64 ${XBB_LDFLAGS_APP}"
    XBB_LDFLAGS_APP_STATIC_GCC="-L${LIBS_INSTALL_FOLDER_PATH}/lib64 ${XBB_LDFLAGS_APP_STATIC_GCC}"

    PKG_CONFIG_PATH="${LIBS_INSTALL_FOLDER_PATH}/lib64/pkgconfig:${PKG_CONFIG_PATH}"

    LD_LIBRARY_PATH="${LIBS_INSTALL_FOLDER_PATH}/lib64:${LD_LIBRARY_PATH}"
  fi

  export XBB_CPPFLAGS

  export XBB_LDFLAGS
  export XBB_LDFLAGS_LIB
  export XBB_LDFLAGS_APP
  export XBB_LDFLAGS_APP_STATIC_GCC

  export PKG_CONFIG_PATH
  export LD_LIBRARY_PATH
}

# -----------------------------------------------------------------------------

function do_config_guess() 
{
  if [ -f "${XBB_FOLDER}/share/libtool/build-aux/config.guess" ]
  then
    BUILD="$(${XBB_FOLDER}/share/libtool/build-aux/config.guess)"
  elif [ -f "/usr/share/libtool/build-aux/config.guess" ]
  then
    BUILD="$(/usr/share/libtool/build-aux/config.guess)"
  elif [ -f "/usr/share/misc/config.guess" ]
  then
    BUILD="$(/usr/share/misc/config.guess)"
  else
    echo "Could not find config.guess."
    exit 1
  fi
}

function prepare_xbb_env() 
{
  # Defaults, to ensure the variables are defined.
  PATH="${PATH:-""}"
  LD_LIBRARY_PATH="${LD_LIBRARY_PATH:-""}"

  # Without this, on 32-bit the bootstrap perl fails to find libcrypt.
  # LD_LIBRARY_PATH="/lib:${LD_LIBRARY_PATH}"

  if [ -f "${HOME}/opt/xbb/xbb-source.sh" ]
  then
    echo
    echo "Sourcing ${HOME}/opt/xbb/xbb-source.sh..."
    source "${HOME}/opt/xbb/xbb-source.sh"
  elif [ -f "${HOME}/opt/homebrew/xbb/xbb-source.sh" ]
  then
    # Deprecated, on macOS it was moved to HOME/opt/xbb
    echo
    echo "Sourcing ${HOME}/opt/homebrew/xbb/xbb-source.sh..."
    source "${HOME}/opt/homebrew/xbb/xbb-source.sh"
  elif [ -f "/opt/xbb/xbb-source.sh" ]
  then
    echo
    echo "Sourcing /opt/xbb/xbb-source.sh..."
    source "/opt/xbb/xbb-source.sh"
  fi

  TARGET_FOLDER_NAME="${TARGET_PLATFORM}-${TARGET_ARCH}"

  # Compute the BUILD/HOST/TARGET for configure.
  CROSS_COMPILE_PREFIX=""
  if [ "${TARGET_PLATFORM}" == "win32" ]
  then

    # For Windows targets, decide which cross toolchain to use.
    if [ ${TARGET_ARCH} == "x32" ]
    then
      CROSS_COMPILE_PREFIX="i686-w64-mingw32"
    elif [ ${TARGET_ARCH} == "x64" ]
    then
      CROSS_COMPILE_PREFIX="x86_64-w64-mingw32"
    fi

    do_config_guess

    HOST="${CROSS_COMPILE_PREFIX}"
    TARGET="${HOST}"

  elif [ "${TARGET_PLATFORM}" == "darwin" ]
  then

    TARGET_BITS="64" # For now, only 64-bit macOS binaries

    do_config_guess

    HOST="${BUILD}"
    TARGET="${HOST}"

  elif [ "${TARGET_PLATFORM}" == "linux" ]
  then

    do_config_guess

    HOST="${BUILD}"
    TARGET="${HOST}"

  else
    echo "Unsupported target platform ${TARGET_PLATFORM}"
    exit 1
  fi

  if [ -f "/.dockerenv" ]
  then
    WORK_FOLDER_PATH="${CONTAINER_WORK_FOLDER_PATH}"
    DOWNLOAD_FOLDER_PATH="${CONTAINER_CACHE_FOLDER_PATH}"
  else
    WORK_FOLDER_PATH="${HOST_WORK_FOLDER_PATH}"
    DOWNLOAD_FOLDER_PATH="${HOST_CACHE_FOLDER_PATH}"
  fi

  # Develop builds use the host folder.
  BUILD_FOLDER_PATH="${WORK_FOLDER_PATH}/${TARGET_FOLDER_NAME}/build"
  if [ -f "/.dockerenv" ]
  then 
    if [ "${IS_DEVELOP}" != "y" ]
    then
      # Docker builds use a temporary folder.
      BUILD_FOLDER_PATH="/tmp/${TARGET_FOLDER_NAME}/build"
    fi
  fi

  LIBS_BUILD_FOLDER_PATH="${BUILD_FOLDER_PATH}/libs"
  mkdir -p "${LIBS_BUILD_FOLDER_PATH}"

  APP_BUILD_FOLDER_PATH="${BUILD_FOLDER_PATH}/${APP_LC_NAME}"
  # Do it later, only if needed.
  # mkdir -p "${APP_BUILD_FOLDER_PATH}"

  INSTALL_FOLDER_PATH="${WORK_FOLDER_PATH}/${TARGET_FOLDER_NAME}/install"

  LIBS_INSTALL_FOLDER_PATH="${INSTALL_FOLDER_PATH}/libs"
  mkdir -p "${LIBS_INSTALL_FOLDER_PATH}"

  APP_INSTALL_FOLDER_PATH="${INSTALL_FOLDER_PATH}/${APP_LC_NAME}"
  mkdir -p "${APP_INSTALL_FOLDER_PATH}"

  LOGS_FOLDER_NAME=${LOGS_FOLDER_NAME:-"logs"}
  LOGS_FOLDER_PATH="${WORK_FOLDER_PATH}/${TARGET_FOLDER_NAME}/${LOGS_FOLDER_NAME}"
  mkdir -p "${LOGS_FOLDER_PATH}"

  DEPLOY_FOLDER_NAME=${DEPLOY_FOLDER_NAME:-"deploy"}
  DEPLOY_FOLDER_PATH="${WORK_FOLDER_PATH}/${DEPLOY_FOLDER_NAME}"
  # Do it later, only if needed.
  # mkdir -p "${DEPLOY_FOLDER_PATH}"

  DISTRO_INFO_NAME=${DISTRO_INFO_NAME:-"distro-info"}
  
  BUILD_GIT_PATH="${WORK_FOLDER_PATH}/build.git"

  # ---------------------------------------------------------------------------

  APP_PREFIX="${INSTALL_FOLDER_PATH}/${APP_LC_NAME}"
  if [ "${TARGET_PLATFORM}" == "win32" ]
  then
    APP_PREFIX_DOC="${APP_PREFIX}/doc"
  else
    # For POSIX platforms, keep the tradition.
    APP_PREFIX_DOC="${APP_PREFIX}/share/doc"
  fi

  # ---------------------------------------------------------------------------

  SOURCES_FOLDER_PATH=${SOURCES_FOLDER_PATH:-"${WORK_FOLDER_PATH}/sources"}
  mkdir -p "${SOURCES_FOLDER_PATH}"

  # Empty defaults.
  IS_DEVELOP=${IS_DEVELOP:-""}
  IS_DEBUG=${IS_DEBUG:-""}
  WITH_PDF=${WITH_PDF:-""}
  WITH_HTML=${WITH_HTML:-""}

  # Redefine this to "y" to create files that include the architecture.
  HAS_NAME_ARCH=${HAS_NAME_ARCH:-""}

  # ---------------------------------------------------------------------------

  export PATH
  export LD_LIBRARY_PATH

  export SOURCES_FOLDER_PATH

  # libtool fails with the Ubuntu /bin/sh.
  export SHELL="/bin/bash"
  export CONFIG_SHELL="/bin/bash"
}

function prepare_xbb_extras()
{
  # ---------------------------------------------------------------------------

  XBB_CPPFLAGS=""

  XBB_CFLAGS="-ffunction-sections -fdata-sections -m${TARGET_BITS} -pipe"
  XBB_CXXFLAGS="-ffunction-sections -fdata-sections -m${TARGET_BITS} -pipe"

  XBB_LDFLAGS=""

  if [ "${IS_DEBUG}" == "y" ]
  then
    XBB_CFLAGS+=" -g -O0"
    XBB_CXXFLAGS+=" -g -O0"
    XBB_LDFLAGS+=" -g -O0"
  else
    XBB_CFLAGS+=" -O2"
    XBB_CXXFLAGS+=" -O2"
    XBB_LDFLAGS+=" -O2"
  fi

  if [ "${TARGET_PLATFORM}" == "linux" ]
  then
    local which_gcc_7="$(xbb_activate; which "g++-7")"
    if [ ! -z "${which_gcc_7}" ]
    then
      CC="gcc-7"
      CXX="g++-7"
    else
      CC="gcc"
      CXX="g++"
    fi
    # Do not add -static here, it fails.
    # Do not try to link pthread statically, it must match the system glibc.
    XBB_LDFLAGS_LIB="${XBB_LDFLAGS}"
    XBB_LDFLAGS_APP="${XBB_LDFLAGS} -Wl,--gc-sections"
    XBB_LDFLAGS_APP_STATIC_GCC="${XBB_LDFLAGS_APP} -static-libgcc -static-libstdc++"
  elif [ "${TARGET_PLATFORM}" == "darwin" ]
  then
    CC="gcc-7"
    CXX="g++-7"
    # Note: macOS linker ignores -static-libstdc++, so 
    # libstdc++.6.dylib should be handled.
    XBB_LDFLAGS+=" -Wl,-macosx_version_min,10.10"
    XBB_LDFLAGS_LIB="${XBB_LDFLAGS}"
    XBB_LDFLAGS_APP="${XBB_LDFLAGS} -Wl,-dead_strip"
    XBB_LDFLAGS_APP_STATIC_GCC="${XBB_LDFLAGS_APP}"
  elif [ "${TARGET_PLATFORM}" == "win32" ]
  then
    # CRT_glob is from ARM script
    # -static avoids libwinpthread-1.dll 
    # -static-libgcc avoids libgcc_s_sjlj-1.dll 
    XBB_LDFLAGS_LIB="${XBB_LDFLAGS}"
    XBB_LDFLAGS_APP="${XBB_LDFLAGS} -Wl,--gc-sections"
    XBB_LDFLAGS_APP_STATIC_GCC="${XBB_LDFLAGS_APP} -static-libgcc -static-libstdc++"
  fi

  set +u
  if [ ! -z "${XBB_FOLDER}" -a -x "${XBB_FOLDER}/bin/pkg-config-verbose" ]
  then
    PKG_CONFIG="${XBB_FOLDER}/bin/pkg-config-verbose"
  fi
  set -u

  PKG_CONFIG_PATH=${PKG_CONFIG_PATH:-":"}

  # Prevent pkg-config to search the system folders (configured in the
  # pkg-config at build time).
  PKG_CONFIG_LIBDIR=${PKG_CONFIG_LIBDIR:-":"}

  set +u
  echo
  echo "CC=${CC}"
  echo "CXX=${CXX}"
  echo "XBB_CPPFLAGS=${XBB_CPPFLAGS}"
  echo "XBB_CFLAGS=${XBB_CFLAGS}"
  echo "XBB_CXXFLAGS=${XBB_CXXFLAGS}"

  echo "XBB_LDFLAGS_LIB=${XBB_LDFLAGS_LIB}"
  echo "XBB_LDFLAGS_APP=${XBB_LDFLAGS_APP}"
  echo "XBB_LDFLAGS_APP_STATIC_GCC=${XBB_LDFLAGS_APP_STATIC_GCC}"

  echo "PKG_CONFIG=${PKG_CONFIG}"
  echo "PKG_CONFIG_PATH=${PKG_CONFIG_PATH}"
  echo "PKG_CONFIG_LIBDIR=${PKG_CONFIG_LIBDIR}"
  set -u

  (
    xbb_activate

    echo
    if [ "${TARGET_PLATFORM}" == "win32" ]
    then
      which ${CROSS_COMPILE_PREFIX}-gcc
      ${CROSS_COMPILE_PREFIX}-gcc --version
    else
      which ${CC}
      ${CC} --version
    fi

    which make
    make --version
  )

  set +u
  if [ "${TARGET_PLATFORM}" == "win32" -a ! -z "${CC}" -a ! -z  "${CXX}" ]
  then
    echo "CC and CXX must not be set for cross builds."
    exit 1
  fi
  set -u

  # ---------------------------------------------------------------------------

  export XBB_CPPFLAGS

  export XBB_CFLAGS
  export XBB_CXXFLAGS

  export XBB_LDFLAGS
  export XBB_LDFLAGS_LIB
  export XBB_LDFLAGS_APP
  export XBB_LDFLAGS_APP_STATIC_GCC

  export CC
  export CXX


  export PKG_CONFIG
  export PKG_CONFIG_PATH
  export PKG_CONFIG_LIBDIR

}

# -----------------------------------------------------------------------------

function do_actions()
{
  if [ "${ACTION}" == "clean" ]
  then
    echo

    if [ "${IS_NATIVE}" == "y" ]
    then
      echo "Removing the ${TARGET_FOLDER_NAME} build and install ${APP_LC_NAME} folders..."

      rm -rf "${HOST_WORK_FOLDER_PATH}/${TARGET_FOLDER_NAME}/build/${APP_LC_NAME}"
      rm -rf "${HOST_WORK_FOLDER_PATH}/${TARGET_FOLDER_NAME}/install/${APP_LC_NAME}"
    elif [ ! -z "${DO_BUILD_WIN32}${DO_BUILD_WIN64}${DO_BUILD_LINUX32}${DO_BUILD_LINUX64}${DO_BUILD_OSX}" ]
    then
      if [ "${DO_BUILD_WIN32}" == "y" ]
      then
        echo "Removing the win32-x32 build and install ${APP_LC_NAME} folders..."

        rm -rf "${HOST_WORK_FOLDER_PATH}/win32-x32/build/${APP_LC_NAME}"
        rm -rf "${HOST_WORK_FOLDER_PATH}/win32-x32/install/${APP_LC_NAME}"
      fi
      if [ "${DO_BUILD_WIN64}" == "y" ]
      then
        echo "Removing the win32-x64 build and install ${APP_LC_NAME} folders..."

        rm -rf "${HOST_WORK_FOLDER_PATH}/win32-x64/build/${APP_LC_NAME}"
        rm -rf "${HOST_WORK_FOLDER_PATH}/win32-x64/install/${APP_LC_NAME}"
      fi
      if [ "${DO_BUILD_LINUX32}" == "y" ]
      then
        echo "Removing the linux-x32 build and install ${APP_LC_NAME} folders..."

        rm -rf "${HOST_WORK_FOLDER_PATH}/linux-x32/build/${APP_LC_NAME}"
        rm -rf "${HOST_WORK_FOLDER_PATH}/linux-x32/install/${APP_LC_NAME}"
      fi
      if [ "${DO_BUILD_LINUX64}" == "y" ]
      then
        echo "Removing the linux-x64 build and install ${APP_LC_NAME} folders..."

        rm -rf "${HOST_WORK_FOLDER_PATH}/linux-x64/build/${APP_LC_NAME}"
        rm -rf "${HOST_WORK_FOLDER_PATH}/linux-x64/install/${APP_LC_NAME}"
      fi
      if [ "${DO_BUILD_OSX}" == "y" ]
      then
        echo "Removing the darwin-x64 build and install ${APP_LC_NAME} folders..."

        rm -rf "${HOST_WORK_FOLDER_PATH}/darwin-x64/build/${APP_LC_NAME}"
        rm -rf "${HOST_WORK_FOLDER_PATH}/darwin-x64/install/${APP_LC_NAME}"
      fi
    else
      echo "Removing the ${HOST_NODE_PLATFORM}-${HOST_NODE_ARCH} build and install ${APP_LC_NAME} folders..."

      rm -rf "${HOST_WORK_FOLDER_PATH}/${HOST_NODE_PLATFORM}-${HOST_NODE_ARCH}/build/${APP_LC_NAME}"
      rm -rf "${HOST_WORK_FOLDER_PATH}/${HOST_NODE_PLATFORM}-${HOST_NODE_ARCH}/install/${APP_LC_NAME}"
    fi
  fi

  if [ "${ACTION}" == "cleanlibs" ]
  then
    echo

    if [ "${IS_NATIVE}" == "y" ]
    then
      echo "Removing the ${TARGET_FOLDER_NAME} build and install libs folders..."

      rm -rf "${HOST_WORK_FOLDER_PATH}/${TARGET_FOLDER_NAME}/build/libs"
      rm -rf "${HOST_WORK_FOLDER_PATH}/${TARGET_FOLDER_NAME}/install/libs"
      rm -rf "${HOST_WORK_FOLDER_PATH}/${TARGET_FOLDER_NAME}/install"/stamp-*-installed
    elif [ ! -z "${DO_BUILD_WIN32}${DO_BUILD_WIN64}${DO_BUILD_LINUX32}${DO_BUILD_LINUX64}${DO_BUILD_OSX}" ]
    then
      if [ "${DO_BUILD_WIN32}" == "y" ]
      then
        echo "Removing the win32-x32 build and install libs folders..."

        rm -rf "${HOST_WORK_FOLDER_PATH}/win32-x32/build/libs"
        rm -rf "${HOST_WORK_FOLDER_PATH}/win32-x32/install/libs"
        rm -rf "${HOST_WORK_FOLDER_PATH}/win32-x32/install"/stamp-*-installed
      fi
      if [ "${DO_BUILD_WIN64}" == "y" ]
      then
        echo "Removing the win32-x64 build and install libs folders..."

        rm -rf "${HOST_WORK_FOLDER_PATH}/win32-x64/build/libs"
        rm -rf "${HOST_WORK_FOLDER_PATH}/win32-x64/install/libs"
        rm -rf "${HOST_WORK_FOLDER_PATH}/win32-x64/install"/stamp-*-installed
      fi
      if [ "${DO_BUILD_LINUX32}" == "y" ]
      then
        echo "Removing the linux-x32 build and install libs folders..."

        rm -rf "${HOST_WORK_FOLDER_PATH}/linux-x32/build/libs"
        rm -rf "${HOST_WORK_FOLDER_PATH}/linux-x32/install/libs"
        rm -rf "${HOST_WORK_FOLDER_PATH}/linux-x32/install"/stamp-*-installed
      fi
      if [ "${DO_BUILD_LINUX64}" == "y" ]
      then
        echo "Removing the linux-x64 build and install libs folders..."

        rm -rf "${HOST_WORK_FOLDER_PATH}/linux-x64/build/libs"
        rm -rf "${HOST_WORK_FOLDER_PATH}/linux-x64/install/libs"
        rm -rf "${HOST_WORK_FOLDER_PATH}/linux-x64/install"/stamp-*-installed
      fi
      if [ "${DO_BUILD_OSX}" == "y" ]
      then
        echo "Removing the darwin-x64 build and install libs folders..."

        rm -rf "${HOST_WORK_FOLDER_PATH}/darwin-x64/build/libs"
        rm -rf "${HOST_WORK_FOLDER_PATH}/darwin-x64/install/libs"
        rm -rf "${HOST_WORK_FOLDER_PATH}/darwin-x64/install"/stamp-*-installed
      fi
    else
      echo "Removing the ${HOST_NODE_PLATFORM}-${HOST_NODE_ARCH} build and install libs folders..."

      rm -rf "${HOST_WORK_FOLDER_PATH}/${HOST_NODE_PLATFORM}-${HOST_NODE_ARCH}/build/libs"
      rm -rf "${HOST_WORK_FOLDER_PATH}/${HOST_NODE_PLATFORM}-${HOST_NODE_ARCH}/install/libs"
      rm -rf "${HOST_WORK_FOLDER_PATH}/${HOST_NODE_PLATFORM}-${HOST_NODE_ARCH}/install"/stamp-*-installed
    fi
  fi

  if [ "${ACTION}" == "cleanall" ]
  then
    echo
    if [ "${IS_NATIVE}" == "y" ]
    then
      echo "Removing the ${TARGET_FOLDER_NAME} folder..."
  
      rm -rf "${HOST_WORK_FOLDER_PATH}/${TARGET_FOLDER_NAME}"
    elif [ ! -z "${DO_BUILD_WIN32}${DO_BUILD_WIN64}${DO_BUILD_LINUX32}${DO_BUILD_LINUX64}${DO_BUILD_OSX}" ]
    then
      if [ "${DO_BUILD_WIN32}" == "y" ]
      then
        echo "Removing the win32-x32 folder..."

        rm -rf "${HOST_WORK_FOLDER_PATH}/win32-x32"
      fi
      if [ "${DO_BUILD_WIN64}" == "y" ]
      then
        echo "Removing the win32-x64 folder..."

        rm -rf "${HOST_WORK_FOLDER_PATH}/win32-x64"
      fi
      if [ "${DO_BUILD_LINUX32}" == "y" ]
      then
        echo "Removing the linux-x32 folder..."

        rm -rf "${HOST_WORK_FOLDER_PATH}/linux-x32"
      fi
      if [ "${DO_BUILD_LINUX64}" == "y" ]
      then
        echo "Removing the linux-x64 folder..."

        rm -rf "${HOST_WORK_FOLDER_PATH}/linux-x64"
      fi
      if [ "${DO_BUILD_OSX}" == "y" ]
      then
        echo "Removing the darwin-x64 folder..."

        rm -rf "${HOST_WORK_FOLDER_PATH}/darwin-x64"
      fi
    else
      echo "Removing the ${HOST_NODE_PLATFORM}-${HOST_NODE_ARCH} folder..."

      rm -rf "${HOST_WORK_FOLDER_PATH}/${HOST_NODE_PLATFORM}-${HOST_NODE_ARCH}"
    fi
  fi

  if [ "${ACTION}" == "clean" -o "${ACTION}" == "cleanlibs" -o "${ACTION}" == "cleanall" ]
  then
    echo
    echo "Clean completed. Proceed with a regular build."

    exit 0
  fi

  # Not used for native buils. Otherwise the names of the docker images
  # must be set.
  if [ "${ACTION}" == "preload-images" ]
  then
    host_start_timer

    host_prepare_docker

    echo
    echo "Check/Preload Docker images..."

    echo
    docker run --interactive --tty "${docker_linux64_image}" \
      lsb_release --description --short

    echo
    docker run --interactive --tty "${docker_linux32_image}" \
      lsb_release --description --short

    echo
    docker images

    host_stop_timer

    exit 0
  fi
}

# -----------------------------------------------------------------------------

function run_app()
{
  # Does not include the .exe extension.
  local app_path=$1
  shift

  echo
  echo "${app_path} $@"
  if [ "${TARGET_PLATFORM}" == "linux" ]
  then
    "${app_path}" $@
  elif [ "${TARGET_PLATFORM}" == "darwin" ]
  then
    "${app_path}" $@
  elif [ "${TARGET_PLATFORM}" == "win32" ]
  then
    local wsl_path=$(which wsl.exe)
    if [ ! -z "${wsl_path}" ]
    then
      "${app_path}.exe" $@
    else 
      (
        xbb_activate
        
        local wine_path=$(which wine)
        if [ ! -z "${wine_path}" ]
        then
          wine "${app_path}.exe" $@
        else
          echo "Install wine if you want to run the .exe binaries on Linux."
        fi
      )
    fi
  fi

}

# -----------------------------------------------------------------------------

function do_patch()
{
  if [ ! -z "$1" ]
  then
    local patch_file_name="$1"
    local patch_path="${BUILD_GIT_PATH}/patches/${patch_file_name}"
    if [ -f "${patch_path}" ]
    then
      echo "Patching..."
      patch -p0 < "${patch_path}"
    fi
  fi
}

function extract()
{
  local archive_name="$1"
  local folder_name="$2"
  # local patch_file_name="$3"
  local pwd="$(pwd)"

  if [ ! -d "${folder_name}" ]
  then
    (
      xbb_activate

      echo
      echo "Extracting \"${archive_name}\"..."
      if [[ "${archive_name}" == *zip ]]
      then
        unzip "${archive_name}" 
      else
        if [ ! -z "${DEBUG}" ]
        then
          tar -x -v -f "${archive_name}"
        else
          tar -x -f "${archive_name}"
        fi
      fi

      if [ $# -gt 2 ]
      then
        cd "${folder_name}"
        do_patch "$3"
      fi
    )
  else
    echo "Folder \"${pwd}/${folder_name}\" already present."
  fi
}


function download()
{
  local url="$1"
  local archive_name="$2"

  if [ ! -f "${DOWNLOAD_FOLDER_PATH}/${archive_name}" ]
  then
    (
      xbb_activate

      echo
      echo "Downloading \"${archive_name}\" from \"${url}\"..."
      rm -f "${DOWNLOAD_FOLDER_PATH}/${archive_name}.download"
      mkdir -p "${DOWNLOAD_FOLDER_PATH}"
      curl --fail -L -o "${DOWNLOAD_FOLDER_PATH}/${archive_name}.download" "${url}"
      mv "${DOWNLOAD_FOLDER_PATH}/${archive_name}.download" "${DOWNLOAD_FOLDER_PATH}/${archive_name}"
    )
  else
    echo "File \"${DOWNLOAD_FOLDER_PATH}/${archive_name}\" already downloaded."
  fi
}

function download_and_extract()
{
  local url="$1"
  local archive_name="$2"
  local folder_name="$3"

  download "${url}" "${archive_name}"
  if [ $# -gt 3 ]
  then
    extract "${DOWNLOAD_FOLDER_PATH}/${archive_name}" "${folder_name}" "$4"
  else
    extract "${DOWNLOAD_FOLDER_PATH}/${archive_name}" "${folder_name}"
  fi
}

function git_clone()
{
  local url="$1"
  local branch="$2"
  local commit="$3"
  local folder_name="$4"

  (
    echo
    echo "Cloning \"${folder_name}\" from \"${url}\"..."
    git clone --branch="${branch}" "${url}" "${folder_name}"
    if [ -n "${commit}" ]
    then
      cd "${folder_name}"
      git checkout -qf "${commit}"
    fi
  )
}

# Copy the build files to the Work area, to make them easily available. 
function copy_build_git()
{
  rm -rf "${HOST_WORK_FOLDER_PATH}/build.git"
  mkdir -p "${HOST_WORK_FOLDER_PATH}/build.git"
  cp -r "$(dirname ${script_folder_path})"/* "${HOST_WORK_FOLDER_PATH}/build.git"
  rm -rf "${HOST_WORK_FOLDER_PATH}/build.git/scripts/helper/.git"
  rm -rf "${HOST_WORK_FOLDER_PATH}/build.git/scripts/helper/build-helper.sh"
}

# -----------------------------------------------------------------------------

function check_binary()
{
  local file_path="$1"

  if [ ! -x "${file_path}" ]
  then
    return 0
  fi

  if file --mime "${file_path}" | grep -q text
  then
    return 0
  fi

  check_binary_for_libraries "$1"
}

function check_binary_for_libraries()
{
  local file_path="$1"
  local file_name="$(basename ${file_path})"
  local folder_path="$(dirname ${file_path})"

  (
    xbb_activate

    if [ "${TARGET_PLATFORM}" == "win32" ]
    then
      echo
      echo "${file_name}"
      set +e
      ${CROSS_COMPILE_PREFIX}-objdump -x "${file_path}" | grep -i 'DLL Name'

      local dll_names=$(${CROSS_COMPILE_PREFIX}-objdump -x "${file_path}" \
        | grep -i 'DLL Name' \
        | sed -e 's/.*DLL Name: \(.*\)/\1/' \
      )

      local n
      for n in ${dll_names}
      do
        if [ ! -f "${folder_path}/${n}" ] 
        then
          if is_win_sys_dll "${n}"
          then
            :
          elif [ "${n}${HAS_WINPTHREAD}" == "libwinpthread-1.dlly" ]
          then
            :
          elif [[ ${n} == python*.dll ]] && [[ ${file_name} == *-gdb-py.exe ]]
          then
            :
          else
            echo "Unexpected |${n}|"
            exit 1
          fi
        fi
      done
      set -e
    elif [ "${TARGET_PLATFORM}" == "darwin" ]
    then
      echo
      (
        set +e
        cd ${folder_path}
        otool -L "${file_name}"
        set -e
      )

      set +e
      local unxp
      if [[ "${file_name}" == *\.dylib ]]
      then
        unxp=$(otool -L "${file_path}" | sed '1d' | sed '1d' | grep -v "${file_name}" | egrep -e "(macports|homebrew|opt|install)/")
      else
        unxp=$(otool -L "${file_path}" | sed '1d' | grep -v "${file_name}" | egrep -e "(macports|homebrew|opt|install)/")
      fi
      set -e

      # echo "|${unxp}|"
      if [ ! -z "$unxp" ]
      then
        echo "Unexpected |${unxp}|"
        exit 1
      fi
    elif [ "${TARGET_PLATFORM}" == "linux" ]
    then
      echo
      echo "${file_name}"
      set +e
      readelf -d "${file_path}" | egrep -i 'library|dynamic'

      local so_names=$(readelf -d "${file_path}" \
        | grep -i 'Shared library' \
        | sed -e 's/.*Shared library: \[\(.*\)\]/\1/' \
      )

      local n
      for n in ${so_names}
      do
        if [ ! -f "${folder_path}/${n}" ] 
        then
          if is_linux_sys_so "${n}"
          then
            :
          elif [[ ${n} == libpython* ]] && [[ ${file_name} == *-gdb-py ]]
          then
            :
          else
            echo "Unexpected |${n}|"
            exit 1
          fi
        fi
      done
      set -e
    fi
  )
}

function is_win_sys_dll() 
{
  local dll_name="$1"

  # DLLs that are expected to be present on any Windows.
  local sys_dlls=(ADVAPI32.dll \
    KERNEL32.dll \
    msvcrt.dll \
    MSVCR90.dll \
    SHELL32.dll \
    USER32.dll \
    WINMM.dll \
    WINMM.DLL \
    WS2_32.dll \
    ole32.dll \
    DNSAPI.dll \
    IPHLPAPI.dll \
    GDI32.dll \
    IMM32.dll \
    IMM32.DLL \
    OLEAUT32.dll \
    IPHLPAPI.DLL \
    VERSION.dll \
    SETUPAPI.dll \
    CFGMGR32.dll \
    PSAPI.DLL \
    USERENV.dll \
  )

  local dll
  for dll in "${sys_dlls[@]}"
  do
    if [ "${dll}" == "${dll_name}" ]
    then
        return 0 # True
    fi
  done
  return 1 # False
}

function is_linux_sys_so() 
{
  local lib_name="$1"

  # Do not add these two, they are present if the toolchain is installed, 
  # but this is not guaranteed, so better copy them from the xbb toolchain.
  # libstdc++.so.6 
  # libgcc_s.so.1 

  # Shared libraries that are expected to be present on any Linux.
  # Note the X11 libraries.
  local sys_libs=(\
    librt.so.1 \
    libm.so.6 \
    libc.so.6 \
    libutil.so.1 \
    libpthread.so.0 \
    libdl.so.2 \
    ld-linux-x86-64.so.2 \
    ld-linux.so.2 \
    libX11.so.6 \
    libXau.so.6 \
    libxcb.so.1 \
  )

  local lib
  for lib in "${sys_libs[@]}"
  do
    if [ "${lib}" == "${lib_name}" ]
    then
      return 0 # True
    fi
  done
  return 1 # False
}

function is_darwin_sys_dylib() 
{
  local lib_name="$1"

  if [[ ${lib_name} == /usr/lib* ]]
  then
    return 0
  fi
  if [[ ${lib_name} == /System/Library* ]]
  then
    return 0
  fi

  return 1
}

# -----------------------------------------------------------------------------

function has_origin()
{
  if [ $# -lt 1 ]
  then
    warning "has_origin: Missing file argument"
    exit 1
  fi

  local bin="$1"
  if [ "${TARGET_PLATFORM}" == "linux" ]
  then
    local origin=$(readelf -d ${bin} | grep 'Library runpath: \[$ORIGIN\]')
    if [ ! -z "${origin}" ]
    then
      return 0 # true
    fi
  fi
  return 1 # false
}

# Strip binary files as in "strip binary" form, for both native
# (linux/mac) and mingw.
function strip_binary2() 
{
    set +e
    if [ $# -lt 2 ]
    then
        warning "strip_binary: Missing arguments"
        exit 1
    fi

    local strip="$1"
    local bin="$2"

    if is_elf "${bin}"
    then
      if has_origin "${bin}"
      then
        echo "${strip} ${bin} skipped (patched)"
      else
        echo "${strip} ${bin}"
        # ${strip} ${bin} 2>/dev/null || true
        "${strip}" -S "${bin}" || true
      fi
    else
      echo $(file "${bin}")
    fi

    set -e
}

function strip_binary() 
{
  if [ $# -lt 1 ]
  then
    warning "strip_binary: Missing file argument"
    exit 1
  fi

  local bin="$1"

  local strip="strip"
  if [ "${TARGET_PLATFORM}" == "win32" ]
  then
    strip="${CROSS_COMPILE_PREFIX}-strip"
    if [[ "${bin}" != *.exe ]]
    then
      bin="${bin}.exe"
    fi
  fi

  if has_origin "${bin}"
  then
    # If the file was patched, skip strip, otherwise
    # we may damage the binary due to a bug in strip.
    echo "${strip} ${bin} skipped (patched)"
    return
  fi

  echo "${strip} ${bin}"
  "${strip}" -S "${bin}" || true
}

function is_elf()
{
  if [ $# -lt 1 ]
  then
    warning "is_elf: Missing arguments"
    exit 1
  fi
  local bin="$1"

  # Symlinks do not match.
  if [ -L "${bin}" ]
  then
    return 1
  fi

  if [ -f "${bin}" -a -x "${bin}" ]
  then
    # Return 0 (true) if found.
    file ${bin} | egrep -q "( ELF )|( PE )|( PE32 )|( PE32\+ )|( Mach-O )"
  else
    return 1
  fi
}

# -----------------------------------------------------------------------------

# Deprecated, use copy_dependencies_recursive().
function copy_win_gcc_dll() 
{
  local dll_name="$1"

  # Identify the current cross gcc version, to locate the specific dll folder.
  local cross_gcc_version=$(${CROSS_COMPILE_PREFIX}-gcc --version | grep 'gcc' | sed -e 's/.*\s\([0-9]*\)[.]\([0-9]*\)[.]\([0-9]*\).*/\1.\2.\3/')
  local cross_gcc_version_short=$(echo ${cross_gcc_version} | sed -e 's/\([0-9]*\)[.]\([0-9]*\)[.]\([0-9]*\).*/\1.\2/')
  local SUBLOCATION="-win32"

  # First try Ubuntu specific locations,
  # then do a long full search.

  if [ -f "${XBB_FOLDER}/${CROSS_COMPILE_PREFIX}/lib/${dll_name}" ]
  then
    cp -v "${XBB_FOLDER}/${CROSS_COMPILE_PREFIX}/lib/${dll_name}" \
      "${APP_PREFIX}/bin"
  elif [ -f "/usr/lib/gcc/${CROSS_COMPILE_PREFIX}/${cross_gcc_version}/${dll_name}" ]
  then
    cp -v "/usr/lib/gcc/${CROSS_COMPILE_PREFIX}/${cross_gcc_version}/${dll_name}" \
      "${APP_PREFIX}/bin"
  elif [ -f "/usr/lib/gcc/${CROSS_COMPILE_PREFIX}/${cross_gcc_version_short}/${dll_name}" ]
  then
    cp -v "/usr/lib/gcc/${CROSS_COMPILE_PREFIX}/${cross_gcc_version_short}/${dll_name}" \
      "${APP_PREFIX}/bin"
  elif [ -f "/usr/lib/gcc/${CROSS_COMPILE_PREFIX}/${cross_gcc_version_short}${SUBLOCATION}/${dll_name}" ]
  then
    cp -v "/usr/lib/gcc/${CROSS_COMPILE_PREFIX}/${cross_gcc_version_short}${SUBLOCATION}/${dll_name}" \
      "${APP_PREFIX}/bin"
  else
    echo "Searching /usr for ${dll_name}..."
    SJLJ_PATH=$(find "${XBB_FOLDER}/${CROSS_COMPILE_PREFIX}" /usr \! -readable -prune -o -name ${dll_name} -print | grep ${CROSS_COMPILE_PREFIX})
    cp -v ${SJLJ_PATH} "${APP_PREFIX}/bin"
  fi
}

# Deprecated, use copy_dependencies_recursive().
function copy_win_libwinpthread_dll() 
{
  if [ -f "${XBB_FOLDER}/${CROSS_COMPILE_PREFIX}/bin/libwinpthread-1.dll" ]
  then
    cp "${XBB_FOLDER}/${CROSS_COMPILE_PREFIX}/bin/libwinpthread-1.dll" \
      "${APP_PREFIX}/bin"
  else
    echo "No libwinpthread-1.dll"
    exit 1
  fi
}

# -----------------------------------------------------------------------------

function change_dylib()
{
  local dylib_name="$1"
  local file_path="$2"

  local dylib_path="$(otool -L "${file_path}" | grep "${dylib_name}" | sed -e 's|[[:space:]]*\(.*\)[[:space:]][(].*[)]|\1|')"

  if [ -z "${dylib_path}" ]
  then
    echo "Dylib ${dylib_name} not used in binary ${file_path}..."
    exit 1
  fi

  # Hack to bring Python library name in line with the other libraries.
  if [ "${dylib_name}" == "Python" ]
  then
    local version="$(otool -L "${file_path}" | grep "${dylib_name}" | sed -e 's|.*current version \([0-9][0-9]*\.[0-9][0-9]*\).*|\1|')"
    dylib_name="libpython${version}.dylib"
    rm -rf "$(dirname ${file_path})/Python"
  fi

  chmod +w "${file_path}"
  install_name_tool \
    -change "${dylib_path}" \
    "@executable_path/${dylib_name}" \
    "${file_path}"

  if [ ! -f "$(dirname ${file_path})/${dylib_name}" ]
  then
    install -v -c -m 644 "${dylib_path}" "$(dirname ${file_path})/${dylib_name}"
  fi
}

# Workaround to Docker error on 32-bit image:
# stat: Value too large for defined data type (requires -D_FILE_OFFSET_BITS=64)
function patch_linux_elf_origin()
{
  local file_path="$1"

  local tmp_path=$(mktemp)
  rm -rf "${tmp_path}"
  cp "${file_path}" "${tmp_path}"
  patchelf --set-rpath '$ORIGIN' "${tmp_path}"
  cp "${tmp_path}" "${file_path}"
  rm -rf "${tmp_path}"
}

# $1 - absolute path to executable
function prepare_app_libraries()
{
  local app_path="$1"
  shift

  (
    xbb_activate

    local app_folder_path="$(dirname "${app_path}")"

    if [ "${WITH_STRIP}" == "y" ]
    then
      strip_binary "${app_path}"
    fi

    if [ "${TARGET_PLATFORM}" == "linux" ]
    then
      echo
      echo "Shared libraries:"
      echo "${app_path}"
      readelf -d "${app_path}" | grep 'Shared library:'

      echo
      echo "Preparing libraries..."
      patch_linux_elf_origin "${app_path}"

      echo
      copy_dependencies_recursive "${app_path}" "${app_folder_path}"
    elif [ "${TARGET_PLATFORM}" == "darwin" ]
    then
      echo
      echo "Initial dynamic libraries:"
      otool -L "${app_path}"

      echo
      echo "Preparing libraries..."
      copy_dependencies_recursive "${app_path}" "${app_folder_path}"

      echo
      echo "Updated dynamic libraries:"
      otool -L "${app_path}"
    elif [ "${TARGET_PLATFORM}" == "win32" ]
    then
      echo
      echo "Dynamic libraries:"
      echo "${app_path}.exe"
      ${CROSS_COMPILE_PREFIX}-objdump -x "${app_path}.exe" | grep -i 'DLL Name'

      echo
      echo "Preparing libraries..."
      copy_dependencies_recursive "${app_path}.exe" "${app_folder_path}"
    fi
  )
}

function prepare_app_folder_libraries()
{
  local folder_path="${APP_PREFIX}"
  if [ $# -ge 1 ]
  then
    folder_path="$1"
  fi

  (
    xbb_activate

    echo
    echo "Preparing ${folder_path} libraries..."

    local binaries
    if [ "${TARGET_PLATFORM}" == "win32" ]
    then

      binaries=$(find "${folder_path}" -name \*.exe)
      for bin in ${binaries} 
      do
        echo "Preparing ${bin} libraries..."
        copy_dependencies_recursive "${bin}" "$(dirname "${bin}")"
      done

    elif [ "${TARGET_PLATFORM}" == "darwin" ]
    then

      binaries=$(find "${folder_path}" -name \* -perm +111 -and ! -type d)
      for bin in ${binaries} 
      do
        if is_elf "${bin}"
        then
          echo "Preparing ${bin} libraries..."
          copy_dependencies_recursive "${bin}" "$(dirname "${bin}")"
        fi
      done

    elif [ "${TARGET_PLATFORM}" == "linux" ]
    then

      binaries=$(find "${folder_path}" -name \* -perm /111 -and ! -type d)
      for bin in ${binaries} 
      do
        if is_elf "${bin}"
        then
          echo "Preparing ${bin} libraries..."
          copy_dependencies_recursive "${bin}" "$(dirname "${bin}")"
        fi
      done

    fi
  )
}

function copy_dependencies_recursive()
{
  if [ $# -ne 2 ]
  then 
    echo "copy_dependencies_recursive requires 2 args." 
    exit 1
  fi
  local file_path="$1"
  local dest_path="$2"

  local file_name="$(basename "${file_path}")"
  local folder_path="$(dirname "${file_path}")"

  if is_elf "${file_path}"
  then
    if [ "${folder_path}" != "${dest_path}" ]
    then
      if [ ! -f "${dest_path}/${file_name}" ]
      then
        install -v -c -m 644 "${file_path}" "${dest_path}"
        if [ "${WITH_STRIP}" == "y" ]
        then
          strip_binary "${dest_path}/${file_name}"
        fi
      fi
    fi
    if [ "${TARGET_PLATFORM}" == "linux" ]
    then
      patch_linux_elf_origin "${dest_path}/${file_name}"
    fi
  else
    if [ "${TARGET_PLATFORM}" == "win32" ]
    then
      # On Windows don't bother with links, simply copy the file.
      local link_path="$(readlink -f "${file_path}")"
      if [ ! -f "${dest_path}/${file_name}" ]
      then
        install -v -c -m 644 "${link_path}" "${dest_path}"
        if [ "${WITH_STRIP}" == "y" ]
        then
          strip_binary "${dest_path}/${file_name}"
        fi
      fi
    else
      # On POSIX preserve symbolic links, since shared libraries can be
      # referred with different names.
      if [ -L "${file_path}" ]
      then
        (
          local which_realpath="$(which "realpath")"
          if [ ! -z "${which_realpath}" ]
          then
            # Resolve only one link level. If there are more, they
            # are resolved recursively.
            local link_path="$(readlink "${file_path}")"
            cd "${folder_path}"
            # Compute the absolute path of the link.
            local real_path="$(realpath "${link_path}")"
            copy_dependencies_recursive "${real_path}" "${dest_path}"

            cd "${dest_path}"
            if [ \( "$(basename "${link_path}")" != "${file_name}" \) -a \( ! -L "${file_name}" \) ]
            then
              rm -rf "${file_name}"
              ln -sv "$(basename "${link_path}")" "${file_name}" 
            fi
          else
            # Compute the final absolute path of the link, regardless
            # how many links there are on the way.
            local link_path="$(readlink -f "${file_path}")"
            copy_dependencies_recursive "${link_path}" "${dest_path}"

            cd "${dest_path}"
            if [ \( "$(basename "${link_path}")" != "${file_name}" \) -a \( ! -L "${file_name}" \) ]
            then
              rm -rf "${file_name}"
              ln -sv "$(basename "${link_path}")" "${file_name}" 
            fi
          fi
        )
        return
      else
        if [ ! -f "${dest_path}/${file_name}" ]
        then
          install -v -c -m 644 "${file_path}" "${dest_path}"
          if [ "${WITH_STRIP}" == "y" ]
          then
            strip_binary "${dest_path}/${file_name}"
          fi
        fi
        if [ "${TARGET_PLATFORM}" == "linux" ]
        then
          patch_linux_elf_origin "${dest_path}/${file_name}"
        fi
      fi
    fi
  fi

  if [ "${TARGET_PLATFORM}" == "linux" ]
  then
    local libs=$(readelf -d "${dest_path}/${file_name}" \
          | grep -i 'Shared library' \
          | sed -e 's/.*Shared library: \[\(.*\)\]/\1/' \
        )
    local lib
    for lib in ${libs}
    do
      if is_linux_sys_so "${lib}"
      then
        : # System library, no need to copy it.
      else
        if [ -f "${LIBS_INSTALL_FOLDER_PATH}/lib64/${lib}" ]
        then
          copy_dependencies_recursive "${LIBS_INSTALL_FOLDER_PATH}/lib64/${lib}" "${dest_path}"
        elif [ -f "${LIBS_INSTALL_FOLDER_PATH}/lib/${lib}" ]
        then
          copy_dependencies_recursive "${LIBS_INSTALL_FOLDER_PATH}/lib/${lib}" "${dest_path}"
        else
          local full_path=$(${CC} -print-file-name=${lib})
          # -print-file-name outputs back the requested name if not found.
          if [ "${full_path}" != "${lib}" ]
          then
            copy_dependencies_recursive "${full_path}" "${dest_path}"
          else
            if [ -f "${XBB_FOLDER}/lib64/${lib}" ]
            then
              copy_dependencies_recursive "${XBB_FOLDER}/lib64/${lib}" "${dest_path}"
            elif [ -f "${XBB_FOLDER}/lib/${lib}" ]
            then
              copy_dependencies_recursive "${XBB_FOLDER}/lib/${lib}" "${dest_path}"
            else
              echo "${lib} not found"
              exit 1
            fi
          fi
        fi
      fi
    done
  elif [ "${TARGET_PLATFORM}" == "darwin" ]
  then
    echo
    otool -L "${dest_path}/${file_name}"
    local libs=$(otool -L "${dest_path}/${file_name}" \
          | sed '1d' \
          | sed -e 's|[[:space:]]*\(.*\) (.*)|\1|' \
        )
    local exec_prefix="@executable_path/"
    local loader_path="@loader_path/"
    local lib
    for lib in ${libs}
    do
      if [ "${lib}" == "${exec_prefix}${file_name}" ]
      then
        :
      elif [ "${lib}" == "${loader_path}${file_name}" ]
      then
        :
      elif [ "$(basename $(readlink -f ${lib}))" == "${file_name}" ]
      then
        : # Libraries return a line with their own name.
      else
        if is_darwin_sys_dylib "${lib}"
        then
          : # System library, no need to copy it.
        else
          # The libs can be relative to @executable_path or absolute.
          if [ "${lib:0:${#exec_prefix}}" == "${exec_prefix}" ]
          then
            : 
          else
            if [ -f "${lib}" ]
            then
              copy_dependencies_recursive "${lib}" "${dest_path}"
            elif [ -f "${LIBS_INSTALL_FOLDER_PATH}/lib/${lib}" ]
            then
              copy_dependencies_recursive "${LIBS_INSTALL_FOLDER_PATH}/lib/${lib}" "${dest_path}"
            else
              echo "${lib} not found"
              exit 1
            fi
            # Change library path to '@executable_path' inside the lib or app.
            change_dylib "$(basename "${lib}")" "${dest_path}/${file_name}"
          fi
        fi
      fi
    done
  elif [ "${TARGET_PLATFORM}" == "win32" ]
  then
    local libs=$(${CROSS_COMPILE_PREFIX}-objdump -x "${dest_path}/${file_name}" \
          | grep -i 'DLL Name' \
          | sed -e 's/.*DLL Name: \(.*\)/\1/' \
        )
    local lib
    for lib in ${libs}
    do
      if is_win_sys_dll "${lib}"
      then
        : # System DLL, no need to copy it.
      else
        if [ -f "${LIBS_INSTALL_FOLDER_PATH}/bin/${lib}" ]
        then
          copy_dependencies_recursive "${LIBS_INSTALL_FOLDER_PATH}/bin/${lib}" "${dest_path}"
        elif [ -f "${XBB_FOLDER}/${CROSS_COMPILE_PREFIX}/bin/${lib}" ]
        then
          copy_dependencies_recursive "${XBB_FOLDER}/${CROSS_COMPILE_PREFIX}/bin/${lib}" "${dest_path}"
        else
          local full_path=$(${CROSS_COMPILE_PREFIX}-gcc -print-file-name=${lib})
          # -print-file-name outputs back the requested name if not found.
          if [ "${full_path}" != "${lib}" ]
          then
            copy_dependencies_recursive "${full_path}" "${dest_path}"
          else
            echo "${lib} required by ${file_name}, not found"
            exit 1
          fi
        fi
      fi
    done
  fi
}

function check_binaries()
{
  local folder_path="${APP_PREFIX}"
  if [ $# -ge 1 ]
  then
    folder_path="$1"
  fi

  (
    xbb_activate
    
    echo
    echo "Checking binaries for unwanted libraries..."

    local binaries
    if [ "${TARGET_PLATFORM}" == "win32" ]
    then

      binaries=$(find "${folder_path}" -name \*.exe)
      for bin in ${binaries} 
      do
        check_binary "${bin}"
      done

    elif [ "${TARGET_PLATFORM}" == "darwin" ]
    then

      binaries=$(find "${folder_path}" -name \* -perm +111 -and ! -type d)
      for bin in ${binaries} 
      do
        if is_elf "${bin}"
        then
          check_binary "${bin}"
        fi
      done

    elif [ "${TARGET_PLATFORM}" == "linux" ]
    then

      binaries=$(find "${folder_path}" -name \* -perm /111 -and ! -type d)
      for bin in ${binaries} 
      do
        if is_elf "${bin}"
        then
          check_binary "${bin}"
        fi
      done

    fi
  )
}

# -----------------------------------------------------------------------------

# $1 - absolute path to input folder
# $2 - name of output folder below INSTALL_FOLDER
function copy_license() 
{
  # Iterate all files in a folder and install some of them in the
  # destination folder
  echo
  echo "$2"
  (
    cd "$1"
    local f
    for f in *
    do
      if [ -f "$f" ]
      then
        if [[ "$f" =~ AUTHORS.*|NEWS.*|COPYING.*|README.*|LICENSE.*|FAQ.*|DEPENDENCIES.*|THANKS.* ]]
        then
          install -d -m 0755 \
            "${APP_PREFIX}/${DISTRO_INFO_NAME}/licenses/$2"
          install -v -c -m 644 "$f" \
            "${APP_PREFIX}/${DISTRO_INFO_NAME}/licenses/$2"
        fi
      fi
    done
  )
  (
    xbb_activate

    if [ "${TARGET_PLATFORM}" == "win32" ]
    then
      find "${APP_PREFIX}/${DISTRO_INFO_NAME}/licenses" \
        -type f \
        -exec unix2dos '{}' ';'
    fi
  )
}

function copy_build_files()
{
  echo
  echo "Copying build files..."

  (
    cd "${BUILD_GIT_PATH}"

    mkdir -p patches

    find scripts patches -type d \
      -exec install -d -m 0755 \
        "${APP_PREFIX}/${DISTRO_INFO_NAME}"/'{}' ';'

    find scripts patches -type f \
      -exec install -v -c -m 644 \
        '{}' "${APP_PREFIX}/${DISTRO_INFO_NAME}"/'{}' ';'

    if [ -f CHANGELOG.txt ]
    then
      install -v -c -m 644 \
          CHANGELOG.txt "${APP_PREFIX}/${DISTRO_INFO_NAME}"
    fi
    if [ -f CHANGELOG.md ]
    then
      install -v -c -m 644 \
          CHANGELOG.md "${APP_PREFIX}/${DISTRO_INFO_NAME}"
    fi
  )
}

# -----------------------------------------------------------------------------

# Copy one folder to another
function copy_dir() 
{
  local from_path="$1"
  local to_path="$2"

  set +u
  # rm -rf "${to_path}"
  mkdir -p "${to_path}"

  (
    cd "${from_path}"
    if [ "${TARGET_PLATFORM}" == "darwin" ]
    then
      find . -xdev -print0 | cpio -oa0 | (cd "${to_path}" && cpio -im)
    else
      find . -xdev -print0 | cpio -oa0V | (cd "${to_path}" && cpio -imuV)
    fi
  )

  set -u
}

# -----------------------------------------------------------------------------

function create_archive()
{
  (
    xbb_activate

    local distribution_file_version="${RELEASE_VERSION}"

    local target_folder_name=${TARGET_FOLDER_NAME}

    if [ "${HAS_NAME_ARCH}" != "y" ]
    then
      # Pre xPack distributions use the old file name convention.
      distribution_file_version="${RELEASE_VERSION}-${DISTRIBUTION_FILE_DATE}"

      if [ "${TARGET_PLATFORM}" == "win32" ]
      then
        target_folder_name="win${TARGET_BITS}"
      elif [ "${TARGET_PLATFORM}" == "darwin" ]
      then
        target_folder_name="macos"
      elif [ "${TARGET_PLATFORM}" == "linux" ]
      then
        target_folder_name="${CONTAINER_DISTRO_LC_NAME}${TARGET_BITS}"
      fi
    fi

    local distribution_file="${DEPLOY_FOLDER_PATH}/${DISTRO_LC_NAME}-${APP_LC_NAME}-${distribution_file_version}-${target_folder_name}"

    cd "${APP_PREFIX}"
    find . -name '.DS_Store' -exec rm '{}' ';'

    echo
    echo "Creating distribution..."

    mkdir -p "${DEPLOY_FOLDER_PATH}"

    # The folder is temprarily moved into a a more elaborate hierarchy like
    # xPacks/<app-name>/<version>.
    # After the archive is created, the folders are moved back.
    # The atempt to transform the tar path failes, since symlinks were
    # also transformed, which is bad.
    if [ "${TARGET_PLATFORM}" == "win32" ]
    then

      local distribution_file="${distribution_file}.zip"
      local archive_version_path="${INSTALL_FOLDER_PATH}/archive/${DISTRO_UC_NAME}/${APP_UC_NAME}/${distribution_file_version}"

      echo
      echo "ZIP file: \"${distribution_file}\"."

      rm -rf "${INSTALL_FOLDER_PATH}/archive"
      mkdir -p "${archive_version_path}"
      mv "${APP_PREFIX}"/* "${archive_version_path}"

      cd "${INSTALL_FOLDER_PATH}/archive"
      zip -r9 -q "${distribution_file}" *

      # Put folders back.
      mv "${archive_version_path}"/* "${APP_PREFIX}"

    else

      # Unfortunately on node.js, xz & bz2 require native modules, which
      # proved unsafe, some xz versions failed to compile on node.js v9.x,
      # so use the good old .tgz.
      local distribution_file="${distribution_file}.tgz"
      local archive_version_path="${INSTALL_FOLDER_PATH}/archive/${DISTRO_TOP_FOLDER}/${APP_LC_NAME}/${distribution_file_version}"

      echo "Compressed tarball: \"${distribution_file}\"."

      rm -rf "${INSTALL_FOLDER_PATH}/archive"
      mkdir -p "${archive_version_path}"
      mv -v "${APP_PREFIX}"/* "${archive_version_path}"

      # Without --hard-dereference the hard links may be turned into
      # broken soft links on macOS.
      cd "${INSTALL_FOLDER_PATH}"/archive
      # -J uses xz for compression; best compression ratio.
      # -j uses bz2 for compression; good compression ratio.
      # -z uses gzip for compression; fair compression ratio.
      tar -c -z -f "${distribution_file}" \
        --owner=0 \
        --group=0 \
        --format=posix \
        --hard-dereference \
        *

      # Put folders back.
      mv -v "${archive_version_path}"/* "${APP_PREFIX}"

    fi

    cd "${DEPLOY_FOLDER_PATH}"
    compute_sha shasum -a 256 "$(basename ${distribution_file})"
  )
}

# -----------------------------------------------------------------------------

# $1 = application name
function check_application()
{
  local app_name=$1

  if [ "${TARGET_PLATFORM}" == "linux" ]
  then

    echo
    echo "Checking binaries for unwanted shared libraries..."

    check_binary "${APP_PREFIX}/bin/${app_name}"

    local libs=$(find "${APP_PREFIX}/bin" -name \*.so.\* -type f)
    local lib
    for lib in ${libs} 
    do
      check_binary_for_libraries "${lib}"
    done

  elif [ "${TARGET_PLATFORM}" == "darwin" ]
  then

    echo
    echo "Checking binaries for unwanted dynamic libraries..."

    check_binary "${APP_PREFIX}/bin/${app_name}"

    local libs=$(find "${APP_PREFIX}/bin" -name \*.dylib -type f)
    local lib
    for lib in ${libs} 
    do
      check_binary_for_libraries "${lib}"
    done

  elif [ "${TARGET_PLATFORM}" == "win32" ]
  then

    echo
    echo "Checking binaries for unwanted DLLs..."

    check_binary "${APP_PREFIX}/bin/${app_name}.exe"

    local libs=$(find "${APP_PREFIX}/bin" -name \*.dll -type f)
    local lib
    for lib in ${libs} 
    do
      check_binary_for_libraries "${lib}"
    done

  else

    echo "Unsupported TARGET_PLATFORM ${TARGET_PLATFORM}"
    exit 1

  fi

  echo
}

# -----------------------------------------------------------------------------

function compute_sha() 
{
  # $1 shasum program
  # $2.. options
  # ${!#} file

  file=${!#}
  sha_file="${file}.sha"
  "$@" >"${sha_file}"
  echo "SHA: $(cat ${sha_file})"
}

# -----------------------------------------------------------------------------
