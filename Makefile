CC = gcc
FLAGS = -Wall -g -std=c99

all: main_test

main_test: main_test.c graph_structures.c
	$(CC) -o $@ $^

clean: 
	@rm main_test
