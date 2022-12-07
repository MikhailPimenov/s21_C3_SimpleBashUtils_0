#!/bin/bash
SUCCESS=0
FAIL=0
COUNTER=0
DIFF=""

s21_command=(
    "./grep/s21_grep"
    )
sys_command=(
    "grep"
    )

flags=(
    "v"
    "c"
    "l"
    "n"
    "h"
    "o"
    "i"
    "s"
)

tests=(
"s grep/test_files/test_0_grep.txt FLAGS"
"for grep/s21_grep.c grep/s21_grep.h grep/Makefile FLAGS"
"for grep/s21_grep.c FLAGS"
"-e for -e ^int grep/s21_grep.c grep/s21_grep.h grep/Makefile FLAGS"
"-e for -e ^int grep/s21_grep.c FLAGS"
"FLAGS grep/no_file.txt"
"FLAGS -f grep/no_pattern.txt grep/no_file.txt"
"-e regex -e ^print grep/s21_grep.c FLAGS -f grep/test_files/test_ptrn_grep.txt"
"-e while -e void grep/s21_grep.c grep/Makefile FLAGS -f grep/test_files/test_ptrn_grep.txt"
"-e intel -e int FLAGS grep/test_files/test_7_grep.txt"
"-e int -e intel FLAGS grep/test_files/test_7_grep.txt"
)

manual=(
"-n for grep/test_files/test_1_grep.txt grep/test_files/test_2_grep.txt"
"-n for grep/test_files/test_1_grep.txt"
"-n -e ^\} grep/test_files/test_1_grep.txt"
"-c -e /\ grep/test_files/test_1_grep.txt"
"-ce ^int grep/test_files/test_1_grep.txt grep/test_files/test_2_grep.txt"
"-e ^int grep/test_files/test_1_grep.txt"
"-nivh = grep/test_files/test_1_grep.txt grep/test_files/test_2_grep.txt"
"-e"
"-ie INT grep/test_files/test_5_grep.txt"
"-echar grep/test_files/test_1_grep.txt grep/test_files/test_2_grep.txt"
"-ne = -e out grep/test_files/test_5_grep.txt"
"-iv int grep/test_files/test_5_grep.txt"
"-in int grep/test_files/test_5_grep.txt"
"-c -l aboba grep/test_files/test_1_grep.txt grep/test_files/test_5_grep.txt"
"-v grep/test_files/test_1_grep.txt -e ank"
"-noe ) grep/test_files/test_5_grep.txt"
"-l for grep/test_files/test_1_grep.txt grep/test_files/test_2_grep.txt"
"-o -e int grep/test_files/test_4_grep.txt"
"-e = -e out grep/test_files/test_5_grep.txt"
"-noe ing -e as -e the -e not -e is grep/test_files/test_6_grep.txt"
"-e ing -e as -e the -e not -e is grep/test_files/test_6_grep.txt"
"-c -e . grep/test_files/test_1_grep.txt -e '.'"
"-l for grep/no_file.txt grep/test_files/test_2_grep.txt"
"-e int -si grep/no_file.txt grep/s21_grep.c grep/no_file2.txt grep/s21_grep.h"
"-si grep/s21_grep.c -f grep/no_pattern.txt"
"-f grep/test_files/test_3_grep.txt grep/test_files/test_5_grep.txt"
)

run_test() {
    param=$(echo "$@" | sed "s/FLAGS/$var/")
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose -q --log-file="${s21_command[@]}".log ./"${s21_command[@]}" $param > /dev/null
    leak=$(grep -ic -A10000 "LEAK SUMMARY:" "${s21_command[@]}".log || true)
    leak2=$(grep -ic -A10000 "ERROR SUMMARY: [^0]" "${s21_command[@]}".log || true)
    let "COUNTER++"
    if [ "$leak" -eq "0" ] && [ "$leak2" -eq "0" ]
    then
        let "SUCCESS++"
        echo "$COUNTER - Success $param"
    else
        let "FAIL++"
        echo "$COUNTER - Fail $param"
        exit 1
    fi
    rm -f "${s21_command[@]}".log "${sys_command[@]}".log
}

echo "^^^^^^^^^^^^^^^^^^^^^^^"
echo "TESTS WITH NORMAL FLAGS"
echo "^^^^^^^^^^^^^^^^^^^^^^^"
printf "\n"
echo "#######################"
echo "MANUAL TESTS"
echo "#######################"
printf "\n"

for i in "${manual[@]}"
do
    var="-"
    run_test "$i"
done

printf "\n"
echo "#######################"
echo "AUTOTESTS"
echo "#######################"
printf "\n"
echo "======================="
echo "1 PARAMETER"
echo "======================="
printf "\n"

for var1 in "${flags[@]}"
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        run_test "$i"
    done
done
printf "\n"
echo "======================="
echo "2 PARAMETERS"
echo "======================="
printf "\n"

for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                run_test "$i"
            done
        fi
    done
done
printf "\n"
echo "======================="
echo "3 PARAMETERS"
echo "======================="
printf "\n"
for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        for var3 in "${flags[@]}"
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1 -$var2 -$var3"
                    run_test "$i"
                done
            fi
        done
    done
done
printf "\n"
echo "======================="
echo "4 PARAMETERS"
echo "======================="
printf "\n"
for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        for var3 in "${flags[@]}"
        do
            for var4 in "${flags[@]}"
            do
                if [ $var1 != $var2 ] && [ $var2 != $var3 ] \
                && [ $var1 != $var3 ] && [ $var1 != $var4 ] \
                && [ $var2 != $var4 ] && [ $var3 != $var4 ]
                then
                    for i in "${tests[@]}"
                    do
                        var="-$var1 -$var2 -$var3 -$var4"
                        run_test "$i"
                    done
                fi
            done
        done
    done
done
printf "\n"
echo "#######################"
echo "AUTOTESTS"
echo "#######################"
printf "\n"
echo "======================="
echo "DOUBLE PARAMETER"
echo "======================="
printf "\n"
for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1$var2"
                run_test "$i"
            done
        fi
    done
done

printf "\n"
echo "#######################"
echo "AUTOTESTS"
echo "#######################"
printf "\n"
echo "======================="
echo "TRIPLE PARAMETER"
echo "======================="
printf "\n"
for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        for var3 in "${flags[@]}"
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1$var2$var3"
                    run_test "$i"
                done
            fi
        done
    done
done
printf "\n"
echo "FAILED: $FAIL"
echo "SUCCESSFUL: $SUCCESS"
echo "ALL: $COUNTER"
printf "\n"
##############################
