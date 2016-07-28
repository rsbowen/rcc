#TODO: dependencies are probably wrong, just make clean/make all

CC=g++
COMPILE_FLAGS=--std=c++11 -c
LINK_FLAGS=--std=c++11

all: trie.o word_finder.o puzzle.o fill.o trie_test word_finder_test 

#Object files

trie.o: trie.cc trie.h
	$(CC) $(COMPILE_FLAGS) trie.cc -o trie.o

word_finder.o: word_finder.cc word_finder.h trie.o
	$(CC) $(COMPILE_FLAGS) word_finder.cc -o word_finder.o

fill.o: fill.cc fill.h
	$(CC) $(COMPILE_FLAGS) fill.cc

puzzle.o: puzzle.cc puzzle.h
	$(CC) $(COMPILE_FLAGS) puzzle.cc

#Tests

trie_test: trie.o trie_test.cc
	$(CC) $(LINK_FLAGS) trie_test.cc trie.o -o trie_test

word_finder_test: word_finder.o word_finder_test.cc
	$(CC) $(LINK_FLAGS) word_finder_test.cc word_finder.o -o word_finder_test

fill_test: word_finder.o puzzle.o fill.o fill_test.cc
	$(CC) $(LINK_FLAGS) fill_test.cc fill.o word_finder.o puzzle.o -o fill_test

puzzle_test: puzzle.o puzzle_test.cc
	$(CC) $(LINK_FLAGS) puzzle_test.cc puzzle.o -o puzzle_test

clean:
	rm *.o trie_test word_finder_test fill_test puzzle_test
