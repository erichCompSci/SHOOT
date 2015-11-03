CC = gcc
FLAGS = -Wall -g -std=c99

all: main_test

main_test: main_test.c graph_structures.c
	$(CC) $(FLAGS) -o $@ $^


dot: the_graph.dot
	@dot the_graph.dot -Tpdf -o the_graph.pdf

clean: 
	@rm main_test the_graph.*
