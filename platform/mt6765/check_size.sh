# L2 Cache Share SRAM contains:
# 4 KB GFH            : 0x0020_0000 ~ 0x0020_1000
# 252 KB Preloader      : 0x0020_1000 ~ 0x0023_FFFF
# 20 KB Preloader BSS2  : 0x0010_8C00 ~ 0x0010_DC00
PL_START_ADDR=0x00201000
PL_END_ADDR=0x0023FFFF

# Preloader BSS is splitted into bss1 and bss2. bss1 would be overwritten
# by ATF. bss2 keeps the variables which will be used after ATF is loaded.
# Since normal + security + UFS features are around 10 KB, thus we evaluate
# bss2 size up to 20 KB.
PL_MAX_RAM_SIZE=20480

SIG_SIZE=$(cat "${PRELOADER_OUT}/bin/sig_size.txt")
if [ -z "$SIG_SIZE" ]
then
	SIG_SIZE=292
fi
PL_MAX_ROM_SIZE=$((PL_END_ADDR - PL_START_ADDR - SIG_SIZE))

ROM_RPT_SIZE=500
RAM_RPT_SIZE=500

# check image rom size
PL_ROM_SIZE=$(stat -c%s "${PL_IMAGE}")

if [ $PL_ROM_SIZE -gt $PL_MAX_ROM_SIZE ]; then
    echo "===================== Building Fail ==========================="
    echo " IMG ROM Size: ${PL_ROM_SIZE} bytes > ${PL_MAX_ROM_SIZE} bytes!! "
    echo " WARNING: Reduce your code size first then compile again!! "
    echo "==============================================================="
    echo "                    ROM SIZE REPORT (>${ROM_RPT_SIZE} bytes)   "
    echo "---------------------------------------------------------------"
    cat ${PRELOADER_OUT}/report-codesize.txt | awk -F' ' '{if ($3=="FUNC" && $2>sz) print $0}' sz=${ROM_RPT_SIZE}
    echo "---------------------------------------------------------------"
    rm -f ${PL_IMAGE}
    echo "BUILD FAIL."
    exit 1;
fi

# check image ram size
PL_RAM_SIZE=$(awk '{if($3=="_bss2_start") {BSS_START=("0x" $1)+0} ;
             if($3=="_bss2_end") {BSS_END=("0x" $1)+0}}
             END{printf("%d\n",BSS_END-BSS_START)}' ${PL_FUN_MAP})
if [ $PL_RAM_SIZE -gt $PL_MAX_RAM_SIZE ]; then
    echo "===================== Building Fail ==========================="
    echo " IMG RAM Size: ${PL_RAM_SIZE} bytes > ${PL_MAX_RAM_SIZE} bytes!!"
    echo " WARNING: Reduce your code size first then compile again!! "
    echo "==============================================================="
    echo "                    RAM SIZE REPORT (>${RAM_RPT_SIZE} bytes)   "
    echo "---------------------------------------------------------------"
    cat ${PRELOADER_OUT}/report-codesize.txt | awk -F' ' '{if ($3=="OBJECT" && $2>sz) print $0}' sz=${RAM_RPT_SIZE}
    echo "---------------------------------------------------------------"
    rm -f ${PL_IMAGE}
    echo "BUILD FAIL."
    exit 1;
fi

# check image dram size
PL_DRAM_SIZE=$(awk '{if($3=="_dram_start") {DRAM_START=("0x" $1)+0} ;
             if($3=="_dram_end") {DRAM_END=("0x" $1)+0}}
             END{printf("%d\n",DRAM_END-DRAM_START)}' ${PL_FUN_MAP})

PL_SIZE=`expr $PL_RAM_SIZE + $PL_ROM_SIZE`
echo "===================== Building Success ==========================="
echo " PL MAX ROM size : ${PL_MAX_ROM_SIZE} bytes"
echo " PL ROM size     : ${PL_ROM_SIZE} bytes"
echo " PL RAM size     : ${PL_RAM_SIZE} bytes"
echo " PL DRAM size    : ${PL_DRAM_SIZE} bytes"
echo " PL ROM+RAM size : ${PL_SIZE} bytes"
echo "=================================================================="
