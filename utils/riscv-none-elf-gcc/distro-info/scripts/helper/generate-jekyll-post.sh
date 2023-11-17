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

# Script to generate the Jekyll post markdown page.

# -----------------------------------------------------------------------------

source "${scripts_folder_path}/defs-source.sh"

echo
echo "Generating the ${APP_DESCRIPTION} release Jekyll post page..."

# Helper functions.
source "${helper_folder_path}/common-functions-source.sh"

# -----------------------------------------------------------------------------

version=${RELEASE_VERSION:-"$(get_current_version)"}

dest_folder_path="${HOME}/Downloads/xpack-binaries/${APP_LC_NAME}"
rm -rf "${dest_folder_path}"
mkdir -p "${dest_folder_path}"

echo
if [ "${SKIP_XBBLI:-""}" == "y" ]
then
  echo "Skiping xbbli files..."
else
  scp -p xbbli:"Work/${APP_LC_NAME}-${version}/deploy/*" "${dest_folder_path}"
fi

echo
if [ "${SKIP_XBBLA64:-""}" == "y" ]
then
  echo "Skiping xbbla64 files..."
else
  scp -p xbbla64:"Work/${APP_LC_NAME}-${version}/deploy/*" "${dest_folder_path}"
fi

echo
if [ "${SKIP_XBBLA32:-""}" == "y" ]
then
  echo "Skiping xbbla32 files..."
else
  scp -p xbbla32:"Work/${APP_LC_NAME}-${version}/deploy/*" "${dest_folder_path}"
fi

echo
if [ "${SKIP_XBBMI:-""}" == "y" ]
then
  echo "Skiping xbbmi files..."
else
  scp -p ${XBBMI_HOSTNAME:-xbbmi}:"Work/${APP_LC_NAME}-${version}/deploy/*" "${dest_folder_path}"
fi

echo
if [ "${SKIP_XBBMA:-""}" == "y" ]
then
  echo "Skiping xbbma files..."
else
  scp -p xbbma:"Work/${APP_LC_NAME}-${version}/deploy/*" "${dest_folder_path}"
fi

echo
ls -lL "${dest_folder_path}"

echo
cat ${dest_folder_path}/*.sha

release_date="$(date '+%Y-%m-%d %H:%M:%S %z')"
post_file_path="${HOME}/Desktop/$(date -u '+%Y-%m-%d')-${APP_LC_NAME}-v$(echo ${version} | tr '.' '-')-released.md"
echo
echo "Move '${post_file_path}' to the Jekyll _posts/releases/${APP_LC_NAME} folder."

rm -rf "${post_file_path}"
touch "${post_file_path}"

cat scripts/templates/body-jekyll-release-post-part-1-liquid.md | liquidjs "{ \"RELEASE_VERSION\": \"${version}\", \"RELEASE_DATE\": \"${release_date}\" }" >> "${post_file_path}"

echo >> "${post_file_path}"
echo '```console'  >> "${post_file_path}"
cat ${dest_folder_path}/*.sha \
  | sed -e 's|$|\n|' \
  | sed -e 's|  |\n|' \
  >> "${post_file_path}"
echo '```'  >> "${post_file_path}"

cat scripts/templates/body-jekyll-release-post-part-2-liquid.md | liquidjs "{ \"RELEASE_VERSION\": \"${version}\", \"RELEASE_DATE\": \"${release_date}\" }" >> "${post_file_path}"

echo "Don't forget to manually solve the two TODO action points."

echo
echo "Done."

# Completed successfully.
exit 0

# -----------------------------------------------------------------------------
