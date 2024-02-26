INPUT_FILE=$1
OUTPUT_FILE=$2

AWK_ARG=""
if [[ $(awk --version) = GNU* ]]; then
  AWK_ARG="--non-decimal-data"
fi

echo "Symbol,Address,Size,Object" > ${OUTPUT_FILE}.tmp
awk ${AWK_ARG} '/^ .text/ {
    if ($2 != "" && $2 >= "0x00200000") {
        $4 = substr($4, match($4, /[^\/]+$/));
        printf("%s,%s,%d,%s\n", $1, $2, $3, $4);
    }
}' ${INPUT_FILE} >> ${OUTPUT_FILE}.tmp

awk ${AWK_ARG} '/^ .text/ {
    if ($2 == "") {
        A = $1;
        getline;
        $3 = substr($3, match($3, /[^\/]+$/));
        A = A "," sprintf("%s,%d,%s", $1, $2, $3);
        if ($1 >= "0x00200000")
            print A;
        A = "";
    }
}' ${INPUT_FILE} >> ${OUTPUT_FILE}.tmp

awk ${AWK_ARG} '/^ .data/ {
    if ($2 != "" && $2 >= "0x00200000") {
        $4 = substr($4, match($4, /[^\/]+$/));
        printf("%s,%s,%d,%s\n", $1, $2, $3, $4);
    }
}' ${INPUT_FILE} >> ${OUTPUT_FILE}.tmp

awk ${AWK_ARG} '/^ .data/ {
    if ($2 == "") {
        A = $1;
        getline;
        $3 = substr($3, match($3, /[^\/]+$/));
        A = A "," sprintf("%s,%d,%s", $1, $2, $3);
        if ($1 >= "0x00200000")
            print A;
        A = "";
    }
}' ${INPUT_FILE} >> ${OUTPUT_FILE}.tmp

awk ${AWK_ARG} '/^ .rodata/ {
    if ($2 != "" && $2 >= "0x00200000") {
        $4 = substr($4, match($4, /[^\/]+$/));
        printf("%s,%s,%d,%s\n", $1, $2, $3, $4);
    }
}' ${INPUT_FILE} >> ${OUTPUT_FILE}.tmp

awk ${AWK_ARG} '/^ .rodata/ {
    if ($2 == "") {
        A = $1;
        getline;
        $3 = substr($3, match($3, /[^\/]+$/));
        A = A "," sprintf("%s,%d,%s", $1, $2, $3);
        if ($1 >= "0x00200000")
            print A;
        A = "";
    }
}' ${INPUT_FILE} >> ${OUTPUT_FILE}.tmp

awk ${AWK_ARG} '/^ .bss/ {
    if ($2 != "" && ($2 >= "0x00200000" || $2 >= "0x00100000")) {
        $4 = substr($4, match($4, /[^\/]+$/));
        printf("%s,%s,%d,%s\n", $1, $2, $3, $4);
    }
}' ${INPUT_FILE} >> ${OUTPUT_FILE}.tmp

awk ${AWK_ARG} '/^ .bss/ {
    if ($2 == "") {
        A = $1;
        getline;
        $3 = substr($3, match($3, /[^\/]+$/));
        A = A "," sprintf("%s,%d,%s", $1, $2, $3);
        if (($1 >= "0x00200000") || ($1 >= "0x00100000"))
            print A;
        A = "";
    }
}' ${INPUT_FILE} >> ${OUTPUT_FILE}.tmp

awk ${AWK_ARG} '/^ \*\* merge strings/ {
    A = $1" "$2" "$3;
    getline;
    A = A "," sprintf("%s,%d", $1, $2);
    if ($1 >= "0x00200000")
        print A;
    A = "";
}' ${INPUT_FILE} >> ${OUTPUT_FILE}.tmp

awk ${AWK_ARG} '/^.got/ {
    printf("%s,%s,%d\n", $1, $2, $3, $4);
}' ${INPUT_FILE} >> ${OUTPUT_FILE}.tmp

awk '
    BEGIN {
        FS = ",";
        print "Object,Symbol,Size";
    }

    {
        if (NR > 1) {
            obj_sym = $4 " " $1;    # use object + symbol as key
            object[obj_sym] = $4;
            size[obj_sym] = $3;
            total += $3;
        }
    }

    END {
            for (obj_sym in object) {
                sum[object[obj_sym]] += size[obj_sym];
            }

            PROCINFO["sorted_in"] = "@val_num_desc"
            for (filename in sum) {
                print filename",," sum[filename];

                for (obj_sym in object) {
                    if (object[obj_sym] == filename) {
                        symbol = substr(obj_sym, match(obj_sym, / /) + 1);
                        print "," symbol "," size[obj_sym]
                    }
                }
            }

            print "Total,," total;
    }
' ${OUTPUT_FILE}.tmp > ${OUTPUT_FILE}

rm ${OUTPUT_FILE}.tmp
echo "End of Profiling..."
