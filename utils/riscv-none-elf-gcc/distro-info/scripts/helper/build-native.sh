#!/usr/bin/env bash
# -----------------------------------------------------------------------------
# This file is part of the xPack distribution.
#   (https://xpack.github.io)
# Copyright (c) 2019 Liviu Ionescu.
#
# Permission to use, copy, modify, and/or distribute this software
# for any purpose is hereby granted, under the terms of the MIT license.
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# Safety settings (see https://gist.github.com/ilg-ul/383869cbb01f61a51c4d).

if [[ ! -z ${DEBUG} ]]
then
  set ${DEBUG} # Activate the expand mode if DEBUG is anything but empty.
else
  DEBUG=""
fi

set -o errexit # Exit if command failed.
set -o pipefail # Exit if pipe failed.
set -o nounset # Exit if variable not set.

# Remove the initial space and instead use '\n'.
IFS=$'\n\t'

# -----------------------------------------------------------------------------
# Identify the script location, to reach, for example, the helper scripts.

build_script_path="$0"
if [[ "${build_script_path}" != /* ]]
then
  # Make relative path absolute.
  build_script_path="$(pwd)/$0"
fi

script_folder_path="$(dirname "${build_script_path}")"
script_folder_name="$(basename "${script_folder_path}")"

# =============================================================================

scripts_folder_path="$(dirname $(dirname "${script_folder_path}"))/scripts"
helper_folder_path="${scripts_folder_path}/helper"

# -----------------------------------------------------------------------------

# Script to run a native build, which uses the
# tools and libraries available on the host machine. It is generally
# intended for development and creating customised versions (as opposed
# to the build intended for creating distribution packages).
#
# Developed on Ubuntu 18 LTS x64 and macOS 10.13.

# -----------------------------------------------------------------------------

source "${scripts_folder_path}/defs-source.sh"

echo
echo "${APP_DESCRIPTION} native build script."

# Helper functions.
source "${helper_folder_path}/common-functions-source.sh"
source "${helper_folder_path}/host-functions-source.sh"
source "${helper_folder_path}/common-libs-functions-source.sh"
source "${helper_folder_path}/common-apps-functions-source.sh"

# The order is important, it may override helper defs.
if [ -f "${scripts_folder_path}/common-functions-source.sh" ]
then
  source "${scripts_folder_path}/common-functions-source.sh"
fi

if [ -f "${scripts_folder_path}/common-libs-functions-source.sh" ]
then
  source "${scripts_folder_path}/common-libs-functions-source.sh"
fi

if [ -f "${scripts_folder_path}/common-apps-functions-source.sh" ]
then
source "${scripts_folder_path}/common-apps-functions-source.sh"
fi

source "${scripts_folder_path}/common-versions-source.sh"

host_detect

# -----------------------------------------------------------------------------

# Array where the remaining args will be stored.
declare -a rest

help_message="    bash $0 [--win] [--debug] [--develop] [--jobs N] [--help] [clean|cleanlibs|cleanall]"
host_native_options "${help_message}" $@

host_common

# -----------------------------------------------------------------------------

set_xbb_env

if [ "${TARGET_PLATFORM}" == "darwin" ]
then
  set_compiler_env
else
  prepare_gcc_env "" ""
fi

set_xbb_extras

# -----------------------------------------------------------------------------

tests_initialize

build_versions

# -----------------------------------------------------------------------------

host_stop_timer

host_notify_completed

# Completed successfully.
exit 0

# -----------------------------------------------------------------------------
