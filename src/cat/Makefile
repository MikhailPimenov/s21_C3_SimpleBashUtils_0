CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11 -pedantic -c #-fsanitize=address
LFLAGS=#-fsanitize=address
DEBUGFLAGS=-g

all: s21_cat

s21_cat: s21_cat.o ../common/my_getline.o
		$(CC) $(LFLAGS) $^ -o $@ $(DEBUGFLAGS)

s21_cat.o: s21_cat.c
		$(CC) $(CFLAGS) $< -o $@ $(DEBUGFLAGS)

../common/my_getline.o: ../common/my_getline.c ../common/my_getline.h
		$(CC) $(CFLAGS) $< -o $@ $(DEBUGFLAGS)

clean:
		rm -rf *.o s21_cat ../common/*.o
