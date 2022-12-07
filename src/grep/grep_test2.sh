COUNTER_SUCCESS=0
COUNTER_FAIL=0
DIFF_RES=""
PATTERN="Hello"
FILENAME1="../texts/1.txt"
FILENAME2="../texts/2.txt"
FILENAME3="../texts/3.txt"
FILENAME4="../texts/4.txt"
FILENAME5="../texts/5.txt"

cat /dev/null > grep/log.txt

for var in -v -c -l -n -i -o -h
do
  for var2 in -v -c -l -n -i -o -h
  do
      for var3 in -v -c -l -n -i -o -h
      do
        for var4 in -v -c -l -n -i -o -h
        do
          if [ $var != $var2 ] && [ $var != $var2 ] && [ $var != $var3 ] && [ $var != $var4 ]
          then
            TEST1="$var $var2 $var3 $var4 $PATTERN $FILENAME1 $FILENAME2 $FILENAME3 $FILENAME4 $FILENAME5"
            echo "$TEST1"
            ./grep/s21_grep $TEST1 > grep/s21_grep.txt
            grep $TEST1 > grep/grep.txt
            DIFF_RES="$(diff -s grep/s21_grep.txt grep/grep.txt)"
            if [ "$DIFF_RES" == "Files grep/s21_grep.txt and grep/grep.txt are identical" ]
              then
                (( COUNTER_SUCCESS++ ))
              else
                echo "$TEST1" >> grep/log.txt
                (( COUNTER_FAIL++ ))
            fi
            rm grep/s21_grep.txt grep/grep.txt

            TEST2="$var $var2 $var3 $var4 $PATTERN $FILENAME1 $FILENAME2 $FILENAME3 $FILENAME4 $FILENAME5"
            # echo "$TEST2"
            ./grep/s21_grep $TEST2 > grep/s21_grep.txt
            grep $TEST2 > grep/grep.txt
            DIFF_RES="$(diff -s grep/s21_grep.txt grep/grep.txt)"
            if [ "$DIFF_RES" == "Files grep/s21_grep.txt and grep/grep.txt are identical" ]
              then
                (( COUNTER_SUCCESS++ ))
              else
                echo "$TEST2" >> grep/log.txt
                (( COUNTER_FAIL++ ))
            fi
            rm grep/s21_grep.txt grep/grep.txt

            TEST3="$var $var2 $var3 $var4 $PATTERN $FILENAME1 $FILENAME2 $FILENAME3 $FILENAME4 $FILENAME5"
            # echo "$TEST3"
            ./grep/s21_grep $TEST3 > grep/s21_grep.txt
            grep $TEST3 > grep/grep.txt
            DIFF_RES="$(diff -s grep/s21_grep.txt grep/grep.txt)"
            if [ "$DIFF_RES" == "Files grep/s21_grep.txt and grep/grep.txt are identical" ]
              then
                (( COUNTER_SUCCESS++ ))
              else
                echo "$TEST3" >> grep/log.txt
                (( COUNTER_FAIL++ ))
            fi
            rm grep/s21_grep.txt grep/grep.txt

            TEST4="$var $var2 $var3 $var4 $PATTERN $FILENAME1 $FILENAME2 $FILENAME3 $FILENAME4 $FILENAME5"
            # echo "$TEST4"
            ./grep/s21_grep $TEST4 > grep/s21_grep.txt
            grep $TEST4 > grep/grep.txt
            DIFF_RES="$(diff -s grep/s21_grep.txt grep/grep.txt)"
            if [ "$DIFF_RES" == "Files grep/s21_grep.txt and grep/grep.txt are identical" ]
              then
                (( COUNTER_SUCCESS++ ))
              else
                echo "$TEST4" >> grep/log.txt
                (( COUNTER_FAIL++ ))
            fi
            rm grep/s21_grep.txt grep/grep.txt

            TEST5="$var $var2 $var3 $var4 $PATTERN $FILENAME1 $FILENAME2 $FILENAME3 $FILENAME4 $FILENAME5"
            # echo "$TEST5"
            ./grep/s21_grep $TEST5 > grep/s21_grep.txt
            grep $TEST5 > grep/grep.txt
            DIFF_RES="$(diff -s grep/s21_grep.txt grep/grep.txt)"
            if [ "$DIFF_RES" == "Files grep/s21_grep.txt and grep/grep.txt are identical" ]
              then
                (( COUNTER_SUCCESS++ ))
              else
                echo "$TEST5" >> grep/log.txt
                (( COUNTER_FAIL++ ))
            fi
            rm grep/s21_grep.txt grep/grep.txt

            TEST6="$var $var2 $var3 $var4 $PATTERN $FILENAME1 $FILENAME2 $FILENAME3 $FILENAME4 $FILENAME5"
            # echo "$TEST6"
            ./grep/s21_grep $TEST6 > grep/s21_grep.txt
            grep $TEST6 > grep/grep.txt
            DIFF_RES="$(diff -s grep/s21_grep.txt grep/grep.txt)"
            if [ "$DIFF_RES" == "Files grep/s21_grep.txt and grep/grep.txt are identical" ]
              then
                (( COUNTER_SUCCESS++ ))
              else
                echo "$TEST6" >> grep/log.txt
                (( COUNTER_FAIL++ ))
            fi
            rm grep/s21_grep.txt grep/grep.txt

          fi
        done  
      done
  done
done

echo "SUCCESS: $COUNTER_SUCCESS"
echo "FAIL: $COUNTER_FAIL"
