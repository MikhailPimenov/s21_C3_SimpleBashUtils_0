CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11 -pedantic -c #-fsanitize=address
LFLAGS=#-fsanitize=address
DEBUGFLAGS=-g

BUILDDIRCAT=cat
BUILDDIRGREP=grep
REPORT_DIRECTORY=valgrind_report


all: s21_cat s21_grep

s21_grep_debug: $(BUILDDIRGREP)/s21_grep_debug

s21_cat: $(BUILDDIRCAT)/s21_cat

s21_grep: $(BUILDDIRGREP)/s21_grep


s21_cat_test: s21_cat
		bash $(BUILDDIRCAT)/cat_test.sh


s21_grep_test1: s21_grep
		bash $(BUILDDIRGREP)/grep_test1.sh

s21_grep_test2: s21_grep
		bash $(BUILDDIRGREP)/grep_test2.sh

s21_grep_test3: s21_grep
		bash $(BUILDDIRGREP)/grep_test3.sh

s21_grep_test_leaks: s21_grep
		bash $(BUILDDIRGREP)/grep_test_leaks.sh



$(BUILDDIRCAT)/s21_cat: $(BUILDDIRCAT)/s21_cat.o common/my_getline.o
		$(CC) $(LFLAGS) $^ -o $@ 

$(BUILDDIRCAT)/s21_cat.o: $(BUILDDIRCAT)/s21_cat.c
		$(CC) $(CFLAGS) $< -o $@ $(DEBUGFLAGS)




$(BUILDDIRGREP)/s21_grep: $(BUILDDIRGREP)/s21_grep.o common/my_getline.o
		$(CC) $(LFLAGS) $^ -o $@

$(BUILDDIRGREP)/s21_grep.o: $(BUILDDIRGREP)/s21_grep.c
		$(CC) $(CFLAGS) $< -o $@ $(DEBUGFLAGS)


$(BUILDDIRGREP)/s21_grep_debug: $(BUILDDIRGREP)/s21_grep_debug.o common/my_getline.o
		$(CC) $(LFLAGS) $^ -o $@

$(BUILDDIRGREP)/s21_grep_debug.o: $(BUILDDIRGREP)/s21_grep.c
		$(CC) $(CFLAGS) $< -o $@ $(DEBUGFLAGS) -D _DEBUG


common/my_getline.o: common/my_getline.c common/my_getline.h
		$(CC) $(CFLAGS) $< -o $@ $(DEBUGFLAGS)


clean_s21_cat:
		rm -rf $(BUILDDIRCAT)/*.o $(BUILDDIRCAT)/s21_cat $(BUILDDIRCAT)/log.txt $(BUILDDIRCAT)/cat.txt $(BUILDDIRCAT)/s21_cat.txt $(BUILDDIRCAT)/result21.txt

clean_s21_grep:
		rm -rf $(BUILDDIRGREP)/*.o $(BUILDDIRGREP)/s21_grep $(BUILDDIRGREP)/log.txt $(BUILDDIRGREP)/_valgrind-out.txt


clean: clean_s21_cat clean_s21_grep
		rm -rf valgrind_report
		rm -rf common/*.o
		rm -rf failed_tests.txt
		rm -rf log.txt
		rm -rf grep.log
		rm -rf grep.txt
		rm -rf s21_grep.txt


leaks: s21_grep s21_cat
		leaks -atExit -- $(BUILDDIRCAT)/s21_cat
		leaks -atExit -- $(BUILDDIRCAT)/s21_cat -n -s $(BUILDDIRCAT)/s21_cat.c
		leaks -atExit -- $(BUILDDIRGREP)/s21_grep -emain -nieReTuRn $(BUILDDIRGREP)/s21_grep.c $(BUILDDIRGREP)/s21_grep.c -f../texts/regex_patterns.txt -eFor -f ../texts/regex_patterns2.txt ../texts/test2.txt -f $(BUILDDIRGREP)/test_files/test_ptrn_grep.txt -e rege -neheLlo -e while
		leaks -atExit -- $(BUILDDIRGREP)/s21_grep -emain -nieReTuRn $(BUILDDIRGREP)/s21_grep.c $(BUILDDIRGREP)/s21_grep.c -f../texts/regex_patterns.txt -eFor -f ../texts/regex_patterns2.txt ../texts/test2.txt -f $(BUILDDIRGREP)/test_files/test_ptrn_grep.txt -e rege -neheLlo -o

valgrind: s21_grep s21_cat
		mkdir -p $(REPORT_DIRECTORY)
# -valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s $(BUILDDIRCAT)/s21_cat $(BUILDDIRCAT)/s21_cat > $(REPORT_DIRECTORY)/valgrind_report.txt
# -valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s $(BUILDDIRGREP)/s21_grep -emain -nieReTuRn $(BUILDDIRGREP)/s21_grep.c $(BUILDDIRGREP)/s21_grep.c -f../texts/regex_patterns.txt -eFor -f ../texts/regex_patterns2.txt ../texts/test2.txt $(BUILDDIRGREP)/test_files/test_ptrn_grep.txt -e rege -neheLlo -e while > $(REPORT_DIRECTORY)/valgrind_report.txt
# -valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s $(BUILDDIRGREP)/s21_grep -emain -nieReTuRn $(BUILDDIRGREP)/s21_grep.c $(BUILDDIRGREP)/s21_grep.c -f../texts/regex_patterns.txt -eFor -f ../texts/regex_patterns2.txt ../texts/test2.txt $(BUILDDIRGREP)/test_files/test_ptrn_grep.txt -e rege -neheLlo -o -c > $(REPORT_DIRECTORY)/valgrind_report.txt
# -valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s $(BUILDDIRGREP)/s21_grep -emain -nieReTuRn $(BUILDDIRGREP)/s21_grep.c $(BUILDDIRGREP)/s21_grep.c -f../texts/regex_patterns.txt -eFor -f ../texts/regex_patterns2.txt ../texts/test2.txt $(BUILDDIRGREP)/test_files/test_ptrn_grep.txt -e rege -neheLlo -o -f > $(REPORT_DIRECTORY)/valgrind_report.txt
# -valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s $(BUILDDIRGREP)/s21_grep -emain -nieReTuRn $(BUILDDIRGREP)/s21_grep.c $(BUILDDIRGREP)/s21_grep.c -f../texts/regex_patterns.txt -eFor -f ../texts/regex_patterns2.txt ../texts/test2.txt $(BUILDDIRGREP)/test_files/test_ptrn_grep.txt -e rege -neheLlo -o -e > $(REPORT_DIRECTORY)/valgrind_report.txt
# -valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s $(BUILDDIRGREP)/s21_grep -emain -nieReTuRn $(BUILDDIRGREP)/s21_grep.c $(BUILDDIRGREP)/s21_grep.c -f../texts/regex_patterns.txt -eFor -f ../texts/regex_patterns2.txt ../texts/test2.txt $(BUILDDIRGREP)/test_files/test_ptrn_grep.txt -e rege -neheLlo -o -e q > $(REPORT_DIRECTORY)/valgrind_report.txt
# -valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s $(BUILDDIRGREP)/s21_grep -emain -nieReTuRn $(BUILDDIRGREP)/s21_grep.c $(BUILDDIRGREP)/s21_grep.c -f../texts/regex_patterns.txt -eFor -f ../texts/regex_patterns2.txt ../texts/test2.txt $(BUILDDIRGREP)/test_files/test_ptrn_grep.txt -e rege -neheLlo -o non-exsist-file.txt > $(REPORT_DIRECTORY)/valgrind_report.txt -c
# -valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s $(BUILDDIRGREP)/s21_grep -emain -nieReTuRn $(BUILDDIRGREP)/s21_grep.c $(BUILDDIRGREP)/s21_grep.c -f../texts/regex_patterns.txt -eFor -f ../texts/regex_patterns2.txt ../texts/test2.txt $(BUILDDIRGREP)/test_files/test_ptrn_grep.txt -e rege -neheLlo -o non-exsist-file.txt > $(REPORT_DIRECTORY)/valgrind_report.txt -f
# -valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s $(BUILDDIRGREP)/s21_grep > $(REPORT_DIRECTORY)/valgrind_report.txt 
# -valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s $(BUILDDIRGREP)/s21_grep emain $(BUILDDIRGREP)/s21_grep.c ../texts/test2.txt $(BUILDDIRGREP)/test_files/test_ptrn_grep.txt -e rege > $(REPORT_DIRECTORY)/valgrind_report.txt 
# -valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s $(BUILDDIRGREP)/s21_grep emain $(BUILDDIRGREP)/s21_grep.c ../texts/test2.txt $(BUILDDIRGREP)/test_files/test_ptrn_grep.txt -e rege -o > $(REPORT_DIRECTORY)/valgrind_report.txt 
# -valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s $(BUILDDIRGREP)/s21_grep $(BUILDDIRGREP)/s21_grep.c $(BUILDDIRGREP)/s21_grep.c -f$(BUILDDIRGREP)/regex_patterns.txt -eFor -f $(BUILDDIRGREP)/regex_patterns2.txt ../texts/test2.txt $(BUILDDIRGREP)/test_files/test_ptrn_grep.txt -e rege -neheLlo > $(REPORT_DIRECTORY)/valgrind_report.txt 
		-valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s $(BUILDDIRGREP)/s21_grep $(BUILDDIRGREP)/s21_grep.c $(BUILDDIRGREP)/s21_grep.c -f$(BUILDDIRGREP)/regex_patterns.txt -eFor -f $(BUILDDIRGREP)/regex_patterns2.txt ../texts/test2.txt $(BUILDDIRGREP)/test_files/test_ptrn_grep.txt -e rege -neheLlo -o > $(REPORT_DIRECTORY)/valgrind_report.txt 

lint:
		python3 ../materials/linters/cpplint.py --extensions=c */*.c;
		python3 ../materials/linters/cpplint.py --extensions=c */*.h;

cppcheck:
		cppcheck --enable=all --suppress=missingIncludeSystem */*.c
