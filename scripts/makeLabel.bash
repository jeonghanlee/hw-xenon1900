#!/bin/bash
#
#  Copyright (c) 2017 - Present Jeong Han Lee
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
# Date   : 
# version : 0.0.1
#

declare -gr SC_SCRIPT="$(realpath "$0")"
declare -gr SC_TOP="$(dirname "$SC_SCRIPT")"

declare -gr QR="58"
declare -gr DM="71"
declare -gr PRINTER="DYMO"

function pushd() { builtin pushd "$@" > /dev/null; }
function popd()  { builtin popd  "$@" > /dev/null; }

function create_code() {
    
    local hash_id=$1
    local issue_no=$2
    local code=$3
    
    local t_filename="${hash_id}.png"
    local o_filename="${hash_id}_${issue_no}.png"
    
    pushd ${SC_TOP}
    zint -o ${t_filename} --height=48 -w 4 --border=2  -b ${code} -d ${hash_id}
    convert ${t_filename} -gravity South -background white -splice 0x16 -annotate +0+2 "${issue_no}" ${o_filename}
    # We only care the final result in order to print it out
    rm -f ${t_filename}
    # CUPS printer
    # https://www.cups.org/doc/options.html
#    lpr -P ${PRINTER} ${o_filename};
    popd
}

hash_id_no="$1"
jira_issue_no="$2"

create_code "$hash_id_no" "$jira_issue_no" ${QR}

exit 0;
