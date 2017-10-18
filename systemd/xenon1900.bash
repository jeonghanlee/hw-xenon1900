#!/bin/bash
#
#  Copyright (c) 2017 - Present European Spallation Source ERIC
#
#  The program is free software: you can redistribute
#  it and/or modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation, either version 2 of the
#  License, or any newer version.
#
#  This program is distributed in the hope that it will be useful, but WITHOUT
#  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
#  more details.
#
#  You should have received a copy of the GNU General Public License along with
#  this program. If not, see https://www.gnu.org/licenses/gpl-2.0.txt
#
# Author : Jeong Han Lee
# email  : jeonghan.lee@gmail.com
# Date   : Wednesday, October 18 14:34:03 CEST 2017
# version : 0.0.1
#

ROOT_UID=0 
E_NOTROOT=101
EXIST=1
NON_EXIST=0

function pushd() { builtin pushd "$@" > /dev/null; }
function popd() { builtin popd "$@" > /dev/null;}


SUDO_CMD="sudo"

		  
declare -gr SC_SCRIPT="$(realpath "$0")"
declare -gr SC_SCRIPTNAME=${0##*/}
declare -gr SC_TOP="$(dirname "$SC_SCRIPT")"
declare -gr SUDO_CMD="sudo";
declare -gr SD_UNIT_PATH01=/usr/lib/systemd/system

declare -gr SYSTEMD_UNIT="inventory.service"
declare -gr SYSTEMD_UNIT_M4="${SYSTEMD_UNIT}.m4"
declare -gr SYSTEMD_CONF_M4="${SYSTEMD_UNIT}.conf.m4"

${SUDO_CMD} -v

declare -gr TMP="tmp"


pushd ${SC_TOP}
echo ${SC_TOP}

# echo ""
mkdir -p ${TMP}

cat > ${TMP}/${SYSTEMD_UNIT_M4} <<EOF
include(\`${SYSTEMD_CONF_M4}')
SYSTEMD_UNIT(\`${EPICS_APPS}/xenon1900')
EOF

m4 ${TMP}/${SYSTEMD_UNIT_M4}  > ${TMP}/${SYSTEMD_UNIT}

${SUDO_CMD} install -m 644 ${TMP}/${SYSTEMD_UNIT} ${SD_UNIT_PATH01}

#rm -rf ${TMP}

popd

${SUDO_CMD} systemctl daemon-reload;
#${SUDO_CMD} systemctl enable ${SYSTEMD_UNIT};
# #${SUDO_CMD} systemctl start  ${SYSTEMD_UNIT};

exit 0
