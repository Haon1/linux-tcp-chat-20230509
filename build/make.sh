#!/bin/bash

declare -r NONE="\033[m"
declare -r LIGHT_RED="\033[1;31m"
declare -r LIGHT_GREEN="\033[1;32m"
declare -r YELLOW="\033[1;33m"
declare -r LIGHT_BLUE="\033[1;34m"
declare -r PURPLE="\033[1;35m"
declare -r WHITE="\033[1;37m"
declare -r RED="\033[2;31m"

DIR=$(dirname $0)
export FULL_DIR=${DIR/\./$(pwd)}
echo "full_dir [${FULL_DIR}]"
TOP=${FULL_DIR%%/build}
echo "top [${TOP}]"
OUT=${FULL_DIR%%/build}
echo "out [${OUT}]"

###  需要创建的文件夹
export OUT_DIR=${OUT}/out
export OBJ_DIR=${OUT_DIR}/obj
export BIN_DIR=${OUT_DIR}/bin
export TMP_DIR=${OUT_DIR}/tmp
export LIB_DIR=${OUT_DIR}/lib
CLIENT=${TMP_DIR}/CLIENT
SERVER=${TMP_DIR}/SERVER
COMMON=${TMP_DIR}/COMMON

###  编译工具
CC=gcc


function white_msg()
{
    echo -e "$WHITE $@ $NONE"
}

function red_msg()
{
    echo -e "$LIGHT_RED $@ $NONE"
}

function red_error_msg()
{
    echo -e "$RED $@ $NONE"
}

function green_msg()
{
    echo -e "$LIGHT_GREEN $@ $NONE"
}

function yellow_msg()
{
    echo -e "$YELLOW $@ $NONE"
}

function blue_msg()
{
    echo -e "$LIGHT_BLUE $@ $NONE"
}

function purple_msg()
{
    echo -e "$PURPLE $@ $NONE"
}


#Create Output Directory
function create_out_dir()
{
    mkdir -p ${OUT_DIR}
    mkdir -p ${OBJ_DIR}
    mkdir -p ${BIN_DIR}
    mkdir -p ${LIB_DIR}
    mkdir -p ${TMP_DIR}
    mkdir -p ${CLIENT}
    mkdir -p ${SERVER}
    mkdir -p ${COMMON}

    for dir in `ls ${OUT}`
    do
        if [ ${#dir} == 6 ];then
            #echo ${dir}
            mkdir ${OBJ_DIR}/${dir}
        fi
    done
}

function copy_project_file()
{
	blue_msg "### target CLIENT ###"
	purple_msg "---------------------"
	red_msg "Copy client's Files"
	cp -arf ${OUT}/client/* ${TMP_DIR}/CLIENT
    echo -e "\n"

    blue_msg "### target SERVER ###"
    purple_msg "---------------------"
    red_msg "Copy server's Files"
    cp -arf ${OUT}/server/* ${TMP_DIR}/SERVER
    echo -e "\n"

    blue_msg "### target COMMON ###"
    purple_msg "---------------------"
    red_msg "Copy common's Files"
    cp -arf ${OUT}/common/* ${TMP_DIR}/COMMON
    echo -e "\n"
}

function compile_common_lib()
{
    blue_msg "### compile common lib ###"
	purple_msg "---------------------"
    ${CC} -fpic ${COMMON}/src/encode.c -c -lm -I${COMMON}/include -o ${OBJ_DIR}/common/encode.o
    ${CC} -shared -fpic -o ${LIB_DIR}/libencode.so ${OBJ_DIR}/common/encode.o
    export LD_LIBRARY_PATH=${LIB_DIR}
    echo -e "\n"
}

function compile_client()
{
    blue_msg "### compile client ###"
	purple_msg "---------------------"
    ${CC} ${CLIENT}/src/client.c -c -o -I${CLIENT}/include -o ${OBJ_DIR}/client/client.o
    ${CC} ${OBJ_DIR}/client/client.o -o ${BIN_DIR}/client  -L ${LIB_DIR} -lencode  -lpthread -lc -l${CC}
    echo -e "\n"
}

function compile_server()
{
    blue_msg "### compile server ###"
	purple_msg "---------------------"
    ${CC} ${SERVER}/src/server.c -c -o -I${SERVER}/include -o ${OBJ_DIR}/server/server.o
    ${CC} ${OBJ_DIR}/server/server.o -o ${BIN_DIR}/server  -L ${LIB_DIR} -lencode  -lpthread -lc -l${CC}
    echo -e "\n"
}


#Create Output Directory
if [ ! -e ${OUT_DIR} ]; then
    green_msg "### Create Output Directory ###"
    create_out_dir
fi

    copy_project_file
    purple_msg "### Start Compile ###"
    compile_common_lib
    compile_client
    compile_server
    rm -rf ${TMP_DIR}
    green_msg "### End ###"