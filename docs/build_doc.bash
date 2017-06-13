#!/bin/bash
#
#  Copyright (c) 2017 Jeong Han Lee
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

declare -gr SC_SCRIPT="$(realpath "$0")"
declare -gr SC_TOP="$(dirname "$SC_SCRIPT")"
declare -gr SC_DATE="$(date +%Y%m%d-%H%M)"
declare -gr ZINT_TOP=${SC_TOP}/zint


set -a
. ${ZINT_TOP}/env.conf
set +a


. ${ZINT_TOP}/zint/functions


pushd ${ZINT_TOP}
bash bcGenerator.bash all
popd

pushd ${SC_TOP}
latex ics_inv.tex
latex ics_inv.tex
dvipdf ics_inv.dvi
popd


