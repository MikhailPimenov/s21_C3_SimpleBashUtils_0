CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11 -pedantic -c -fsanitize=address
LFLAGS=-fsanitize=address
DEBUGFLAGS=#-g

BUILDDIRCAT=cat
BUILDDIRGREP=grep
REPORT_DIRECTORY=valgrind_report


all: s21_cat s21_grep


s21_cat: $(BUILDDIRCAT)/s21_cat

s21_grep: $(BUILDDIRGREP)/s21_grep


s21_cat_test: s21_cat
		bash $(BUILDDIRCAT)/test_cat.sh


s21_grep_test: s21_grep
		bash $(BUILDDIRGREP)/grep_base.sh

s21_grep_test_extended: s21_grep
		bash $(BUILDDIRGREP)/grep.sh

s21_grep_test_super_extended: s21_grep
		bash $(BUILDDIRGREP)/test_grep_extended2.sh


$(BUILDDIRCAT)/s21_cat: $(BUILDDIRCAT)/s21_cat.o
		$(CC) $(LFLAGS) $< -o $@ 

$(BUILDDIRCAT)/s21_cat.o: $(BUILDDIRCAT)/s21_cat.c
		$(CC) $(CFLAGS) $< -o $@ $(DEBUGFLAGS)


$(BUILDDIRGREP)/s21_grep: $(BUILDDIRGREP)/s21_grep.o
		$(CC) $(LFLAGS) $< -o $@

$(BUILDDIRGREP)/s21_grep.o: $(BUILDDIRGREP)/s21_grep.c
		$(CC) $(CFLAGS) $< -o $@ $(DEBUGFLAGS)


clean_s21_cat:
		rm -rf $(BUILDDIRCAT)/*.o $(BUILDDIRCAT)/s21_cat $(BUILDDIRCAT)/log.txt $(BUILDDIRCAT)/cat.txt $(BUILDDIRCAT)/s21_cat.txt $(BUILDDIRCAT)/result21.txt

clean_s21_grep:
		rm -rf $(BUILDDIRGREP)/*.o $(BUILDDIRGREP)/s21_grep $(BUILDDIRGREP)/log.txt $(BUILDDIRGREP)/_valgrind-out.txt


clean: clean_s21_cat clean_s21_grep
		rm -rf valgrind_report


leaks: s21_grep s21_cat
		leaks -atExit -- $(BUILDDIRCAT)/s21_cat
		leaks -atExit -- $(BUILDDIRCAT)/s21_cat -n -s $(BUILDDIRCAT)/s21_cat.c
		leaks -atExit -- $(BUILDDIRGREP)/s21_grep -emain -nieReTuRn $(BUILDDIRGREP)/s21_grep.c $(BUILDDIRGREP)/s21_grep.c -f$(BUILDDIRGREP)/regex_patterns.txt -eFor -f $(BUILDDIRGREP)/regex_patterns2.txt $(BUILDDIRGREP)/test2.txt -neheLlo -e while
		leaks -atExit -- $(BUILDDIRGREP)/s21_grep -emain -nieReTuRn $(BUILDDIRGREP)/s21_grep.c $(BUILDDIRGREP)/s21_grep.c -f$(BUILDDIRGREP)/regex_patterns.txt -eFor -f $(BUILDDIRGREP)/regex_patterns2.txt $(BUILDDIRGREP)/test2.txt -neheLlo -o

valgrind: s21_grep s21_cat
		mkdir -p $(REPORT_DIRECTORY)
		-valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose $(BUILDDIRCAT)/s21_cat $(BUILDDIRCAT)/s21_cat > $(REPORT_DIRECTORY)/valgrind_report.txt
		-valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose $(BUILDDIRGREP)/s21_grep -emain -nieReTuRn $(BUILDDIRGREP)/s21_grep.c $(BUILDDIRGREP)/s21_grep.c -f$(BUILDDIRGREP)/regex_patterns.txt -eFor -f $(BUILDDIRGREP)/regex_patterns2.txt $(BUILDDIRGREP)/test2.txt -neheLlo -e while > $(REPORT_DIRECTORY)/valgrind_report.txt
		-valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose $(BUILDDIRGREP)/s21_grep -emain -nieReTuRn $(BUILDDIRGREP)/s21_grep.c $(BUILDDIRGREP)/s21_grep.c -f$(BUILDDIRGREP)/regex_patterns.txt -eFor -f $(BUILDDIRGREP)/regex_patterns2.txt $(BUILDDIRGREP)/test2.txt -neheLlo -o > $(REPORT_DIRECTORY)/valgrind_report.txt