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
declare -gr SC_DATE="$(date +%Y%m%d-%H%M)"



set -a
. ${SC_TOP}/env.conf
set +a

. ${SC_TOP}/functions



# arg3 : output directory
# arg1 : prefix
# arg2 : csv filename

function build_barcodes() {
    local func_name=${FUNCNAME[*]}; __ini_func ${func_name};

    local prefix="$1"
    local input_filename="$2"
    local output_target="$3"
    local output_format="$4"
    local output_dir=${SC_TOP}/${output_target}
    local image_path=""
    local output_tex="${prefix,,}.tex"
    mkdir -p ${output_dir}
    rm -f ${output_dir}/${output_tex}
    touch ${output_dir}/${output_tex}
    
    while IFS=, read id name
    do
	image_path=${output_dir}/${name}.${output_format}
	zint --whitesp=${WHITE_SP} --barcode=${QR} --output=${output_dir}/${name}.${output_format} --data="${prefix},${id},${name}"
	m4 -DIMAGE_PATH=${image_path} -DCAPTION="${prefix},${id},${name^^}" ${SC_TOP}/template/figure.m4 >> ${output_dir}/${output_tex}
    done < ${input_filename}
    
    __end_func ${func_name};
}

function vd() {

    local output_format="$1"
    local isVar=$(checkIfVar ${output_format})
    if [[ $isVar -eq "$NON_EXIST" ]]; then
	output_format=${DEFAULT_OUTPUT_FORMAT}
    fi
    
    build_barcodes "${VD_PRE}" "${VD_CSV}" "${VD_TGT}" "$output_format"
    
}

function ff() {
    local output_format="$1"
    local isVar=$(checkIfVar ${output_format})
    if [[ $isVar -eq "$NON_EXIST" ]]; then
	output_format=${DEFAULT_OUTPUT_FORMAT}
    fi
    build_barcodes "${FF_PRE}" "${FF_CSV}" "${FF_TGT}" "$output_format"
}

arg2=$2

case "$1" in
    vd)
    ##	build_vendors
	vd  ${arg2}
	;;
    ff)
	ff ${arg2}
	;;
    all)
	vd ${arg2}
	ff ${arg2}
	;;
    
    *)

	echo "">&2
        echo " Barcode Generator ">&2
	echo ""
	echo " Usage: $0 <ar1> <arg2>">&2 
	echo ""
        echo "          <arg1> : info">&2 
	echo ""
	echo "          vd     : build vendors'     BarCodes << ">&2
	echo "          ff     :       formfactors' Barcodes << ">&2
	echo "          all    :        all         BarCodes << ">&2
	echo ""
	echo "          <arg2> : info">&2
	echo ""
	echo "          png    : png file (default) << ">&2
	echo "          eps    : eps file           << ">&2
	echo "">&2 	
	exit 0
esac


