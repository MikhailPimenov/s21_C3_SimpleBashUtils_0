#!/bin/bash

COUNTER_SUCCESS=0
COUNTER_FAIL=0
DIFF_RES=""
TEST_FILE=""
echo "" > log.txt

for TEST_FILE1 in  "../../texts/test1.txt" "../../texts/test2.txt"
do
    for TEST_FILE2 in "../../texts/test3.txt" "../../texts/test4.txt" "../../texts/test5.txt"
    do
    if [ $TEST_FILE1 != $TEST_FILE2 ]
    then
        TEST_FILE="$TEST_FILE1 $TEST_FILE2"
        for var in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
        do
                  TEST1="$var $TEST_FILE"
                  echo "$TEST1"
                  ./s21_cat $TEST1 > s21_cat.txt
                  cat $TEST1 > cat.txt
                  DIFF_RES="$(diff -s s21_cat.txt cat.txt)"
                  if [ "$DIFF_RES" == "Files s21_cat.txt and cat.txt are identical" ]
                    then
                      (( COUNTER_SUCCESS++ ))
                    else
                      echo "$TEST1" >> log.txt
                      (( COUNTER_FAIL++ ))
                  fi
                  rm s21_cat.txt cat.txt
        done

        for var in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
        do
          for var2 in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
          do
                if [ $var != $var2 ]
                then
                  TEST1="$var $var2 $TEST_FILE"
                  echo "$TEST1"
                  ./s21_cat $TEST1 > s21_cat.txt
                  cat $TEST1 > cat.txt
                  DIFF_RES="$(diff -s s21_cat.txt cat.txt)"
                  if [ "$DIFF_RES" == "Files s21_cat.txt and cat.txt are identical" ]
                    then
                      (( COUNTER_SUCCESS++ ))
                    else
                      echo "$TEST1" >> log.txt
                      (( COUNTER_FAIL++ ))
                  fi
                  rm s21_cat.txt cat.txt
                fi
          done
        done

        for var in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
        do
          for var2 in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
          do
              for var3 in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
              do
                if [ $var != $var2 ] && [ $var2 != $var3 ] && [ $var != $var3 ]
                then
                  TEST1="$var $var2 $var3 $TEST_FILE"
                  echo "$TEST1"
                  ./s21_cat $TEST1 > s21_cat.txt
                  cat $TEST1 > cat.txt
                  DIFF_RES="$(diff -s s21_cat.txt cat.txt)"
                  if [ "$DIFF_RES" == "Files s21_cat.txt and cat.txt are identical" ]
                    then
                      (( COUNTER_SUCCESS++ ))
                    else
                      echo "$TEST1" >> log.txt
                      (( COUNTER_FAIL++ ))
                  fi
                  rm s21_cat.txt cat.txt

                fi
              done
          done
        done

        for var in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
        do
          for var2 in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
          do
              for var3 in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
              do
                  for var4 in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
                  do
                    if [ $var != $var2 ] && [ $var2 != $var3 ] && [ $var != $var3 ] && [ $var != $var4 ] && [ $var2 != $var4 ] && [ $var3 != $var4 ]
                    then
                      TEST1="$var $var2 $var3 $var4 $TEST_FILE"
                      echo "$TEST1"
                      ./s21_cat $TEST1 > s21_cat.txt
                      cat $TEST1 > cat.txt
                      DIFF_RES="$(diff -s s21_cat.txt cat.txt)"
                      if [ "$DIFF_RES" == "Files s21_cat.txt and cat.txt are identical" ]
                        then
                          (( COUNTER_SUCCESS++ ))
                        else
                          echo "$TEST1" >> log.txt
                          (( COUNTER_FAIL++ ))
                      fi
                      rm s21_cat.txt cat.txt

                    fi
                  done
              done
          done
        done
        fi
    done
done

./s21_cat
(( COUNTER_SUCCESS++ ))

./s21_cat --abir $TEST_FILE
(( COUNTER_SUCCESS++ ))

TEST1="-j"
echo $TEST1
./s21_cat $TEST1 > s21_cat.txt
cat $TEST1 > cat.txt
DIFF_RES="$(diff -s s21_cat.txt cat.txt)"
if [ "$DIFF_RES" == "Files s21_cat.txt and cat.txt are identical" ]
then
  (( COUNTER_SUCCESS++ ))
else
  echo "$TEST1" >> log.txt
  (( COUNTER_FAIL++ ))
fi

TEST1="-e nofile"
echo $TEST1
./s21_cat $TEST1 > s21_cat.txt
cat $TEST1 > cat.txt
DIFF_RES="$(diff -s s21_cat.txt cat.txt)"
if [ "$DIFF_RES" == "Files s21_cat.txt and cat.txt are identical" ]
then
  (( COUNTER_SUCCESS++ ))
else
  echo "$TEST1" >> log.txt
  (( COUNTER_FAIL++ ))
fi

echo "SUCCESS: $COUNTER_SUCCESS"
echo "FAIL: $COUNTER_FAIL"
