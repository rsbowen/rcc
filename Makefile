CC=g++
COMPILE_FLAGS=--std=c++11 -c
LINK_FLAGS=--std=c++11

all: trie.o word_finder.o trie_test word_finder_test	

#Object files

trie.o: trie.cc trie.h
	$(CC) $(COMPILE_FLAGS) trie.cc -o trie.o

word_finder.o: word_finder.cc word_finder.h
	$(CC) $(COMPILE_FLAGS) word_finder.cc -o word_finder.o

#Tests

trie_test: trie.o trie_test.cc
	$(CC) $(LINK_FLAGS) trie_test.cc trie.o -o trie_test

word_finder_test: word_finder.o word_finder_test.cc
	$(CC) $(LINK_FLAGS) word_finder_test.cc word_finder.o -o word_finder_test

clean:
	rm *.o
