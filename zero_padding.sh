#!/bin/bash

FILE_PATH=$1
ALIGNMENT=$2
PADDING_SIZE=0

FILE_SIZE=$(($(wc -c < "${FILE_PATH}")))
REMAINDER=$((${FILE_SIZE} % ${ALIGNMENT}))
FILE_DIR=$(dirname "${FILE_PATH}")
if [ ${REMAINDER} -ne 0 ]; then
    PADDING_SIZE=$((${ALIGNMENT} - ${REMAINDER}))
    dd if=/dev/zero of=${FILE_DIR}/padding.txt bs=$PADDING_SIZE count=1
    cat ${FILE_DIR}/padding.txt>>${FILE_PATH}
#    rm ${FILE_DIR}/padding.txt
fi
