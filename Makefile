CC=clang
CFLAGS=-g -std=gnu99 -Wall -Wextra -Wpedantic

test: test.c src/*
	$(CC) test.c src/*.c $(CFLAGS)

words: words.c src/*
	$(CC) words.c src/*.c $(CFLAGS)
