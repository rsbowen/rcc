CC=g++
COMPILE_FLAGS=--std=c++11 -c
LINK_FLAGS=--std=c++11

all: trie.o word_finder.o trie_test word_finder_test fill.o

#Object files

trie.o: trie.cc trie.h
	$(CC) $(COMPILE_FLAGS) trie.cc -o trie.o

word_finder.o: word_finder.cc word_finder.h trie.o
	$(CC) $(COMPILE_FLAGS) word_finder.cc -o word_finder.o

fill.o: fill.cc fill.h
	$(CC) $(COMPILE_FLAGS) fill.cc

#Tests

trie_test: trie.o trie_test.cc
	$(CC) $(LINK_FLAGS) trie_test.cc trie.o -o trie_test

word_finder_test: word_finder.o word_finder_test.cc
	$(CC) $(LINK_FLAGS) word_finder_test.cc word_finder.o -o word_finder_test

fill_test: word_finder.o fill.o fill_test.cc
	$(CC) $(LINK_FLAGS) fill_test.cc fill.o word_finder.o -o fill_test

clean:
	rm *.o trie_test word_finder_test
