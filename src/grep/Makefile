CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11 -pedantic -c #-fsanitize=address
LFLAGS=#-fsanitize=address
DEBUGFLAGS=-g

all: s21_grep

s21_grep: s21_grep.o ../common/my_getline.o
		$(CC) $(LFLAGS) $^ -o $@

s21_grep.o: s21_grep.c
		$(CC) $(CFLAGS) $< -o $@ $(DEBUGFLAGS)

../common/my_getline.o: ../common/my_getline.c ../common/my_getline.h
		$(CC) $(CFLAGS) $< -o $@ $(DEBUGFLAGS)

clean:
		rm -rf *.o s21_grep ../common/*.o



