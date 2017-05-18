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
# bash 4 is needed
# 

declare -gr SC_SCRIPT="$(realpath "$0")"
declare -gr SC_TOP="$(dirname "$SC_SCRIPT")"
declare -gr SC_DATE="$(date +%Y%m%d-%H%M)"



set -a
. ${SC_TOP}/env.conf
set +a

. ${SC_TOP}/functions

function zint_ft() {
    local tgt_name=$1
    local tgt_data=$2
    zint --whitesp=${WHITE_SP} --barcode=${DEFAULT_BC_TYPE} --output="${tgt_name}" --data="${tgt_data}";
}

function build_barcodes() {
    local func_name=${FUNCNAME[*]}; __ini_func ${func_name};

    local out_format="$1" && shift
    local array=($@)

    local tgt_pre=${array[0]}
    local csv_name=${array[1]}
    local tgt_path=${array[2]}
    local out_path=${SC_TOP}/${tgt_path}
    local out_tex="${tgt_pre,,}.tex"


    
    # create a directory where barcodes will be generated
    mkdir -p ${out_path}
    
    # remove the existent tex file, and create an empty one

    pushd ${out_path} 
    rm -f ${out_tex}
    touch ${out_tex}
    popd
    
    while IFS=, read id name
    do
	target_name=${out_path}/${name}.${out_format}
	target_data="${tgt_pre},${id},${name}"

	zint_ft ${target_name} ${target_data};
	m4 -DIMAGE_PATH=${target_name} -DCAPTION="${name^^}" ${SC_TOP}/template/figure.m4 >> ${out_path}/${out_tex}

    done < ${SC_TOP}/${csv_name}
    
    __end_func ${func_name};
}


arg2=$2


isVar=$(checkIfVar ${arg2})
if [[ $isVar -eq "$NON_EXIST" ]]; then
    out_format=${DEFAULT_OUTPUT_FORMAT}
fi

case "$1" in
    vd)
    ##	build_vendors
	build_barcodes "${out_format}" ${VD_ARRAY[@]}
	;;
    ff)
	build_barcodes "${out_format}" ${FF_ARRAY[@]}
	;;
    lo)
	build_barcodes "${out_format}" ${LO_ARRAY[@]}
	;;
    st)
	build_barcodes "${out_format}" ${ST_ARRAY[@]}
	;;
    mo)
	build_barcodes "${out_format}" ${MO_ARRAY[@]}
	;;
    all)
	build_barcodes "${out_format}" ${VD_ARRAY[@]}
	build_barcodes "${out_format}" ${FF_ARRAY[@]}
	build_barcodes "${out_format}" ${LO_ARRAY[@]}
	build_barcodes "${out_format}" ${ST_ARRAY[@]}
	build_barcodes "${out_format}" ${MO_ARRAY[@]}
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
	echo "          eps    : eps file  (default)         << ">&2
	echo "          png    : png file                    << ">&2
	echo "">&2 	
	exit 0
esac


