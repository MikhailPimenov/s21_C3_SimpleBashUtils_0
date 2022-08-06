CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11 -pedantic -c -g


BUILDDIRCAT=cat
BUILDDIRGREP=grep


all: s21_cat s21_grep


s21_cat: $(BUILDDIRCAT)/s21_cat

s21_grep: $(BUILDDIRGREP)/s21_grep



s21_cat_test: s21_cat
		bash $(BUILDDIRCAT)/test_cat.sh


s21_grep_test: s21_grep
		bash $(BUILDDIRGREP)/grep_base.sh

s21_grep_test_extended: s21_grep
		bash $(BUILDDIRGREP)/grep.sh


$(BUILDDIRCAT)/s21_cat: $(BUILDDIRCAT)/s21_cat.o
		$(CC) $< -o $@

$(BUILDDIRCAT)/s21_cat.o: $(BUILDDIRCAT)/s21_cat.c
		$(CC) $(CFLAGS) $< -o $@


	
$(BUILDDIRGREP)/s21_grep: $(BUILDDIRGREP)/s21_grep.o
		$(CC) $< -o $@

$(BUILDDIRGREP)/s21_grep.o: $(BUILDDIRGREP)/s21_grep.c
		$(CC) $(CFLAGS) $< -o $@


clean_s21_cat:
		rm -rf $(BUILDDIRCAT)/*.o $(BUILDDIRCAT)/s21_cat $(BUILDDIRCAT)/log.txt $(BUILDDIRCAT)/cat.txt $(BUILDDIRCAT)/s21_cat.txt $(BUILDDIRCAT)/result21.txt

clean_s21_grep:
		rm -rf $(BUILDDIRGREP)/*.o $(BUILDDIRGREP)/s21_grep $(BUILDDIRGREP)/log.txt $(BUILDDIRGREP)/_valgrind-out.txt


clean: clean_s21_cat clean_s21_grep
