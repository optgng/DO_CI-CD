#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""

declare -a tests=(
"VAR int test_files/test_1_grep.txt"
"VAR int test_files/test_2_grep.txt"
"VAR int test_files/test_3_grep.txt"
"VAR int test_files/test_4_grep.txt"
"VAR int test_files/test_5_grep.txt"
)

red="\033[31m"
green="\033[32m"
no_color="\033[0m"

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_grep $t > test_s21_grep.log
    grep $t > test_sys_grep.log
    DIFF_RES="$(diff -s test_s21_grep.log test_sys_grep.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_grep.log and test_sys_grep.log are identical" ]
    then
      (( SUCCESS++ ))
      echo -e "$red$FAIL$no_color/$green$SUCCESS$no_color/$COUNTER $greensuccess$no_color grep $t"
    else
      (( FAIL++ ))
      echo -e "$red$FAIL$no_color/$green$SUCCESS$no_color/$COUNTER $redfail$no_color grep $t"
    fi
    rm test_s21_grep.log test_sys_grep.log
}

# 1 parameter
for var1 in e i v c l n h s o
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

echo -e "${red}FAIL: $FAIL"
echo -e "${green}SUCCESS: $SUCCESS"
echo -e "${no_color}ALL: $COUNTER"
