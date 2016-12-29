#TODO: dependencies are probably wrong, just make clean/make all
CC=g++
COMPILE_FLAGS=--std=c++11 -c -Wall -pg -O2
LINK_FLAGS=--std=c++11 -pg -O2

all: trie.o word_finder.o puzzle.o fill.o trie_test word_finder_test 

#Object files

trie.o: trie.cc trie.h
	$(CC) $(COMPILE_FLAGS) trie.cc -o trie.o

# TODO: really Trie should inherit from this
word_finder.o: word_finder.cc word_finder.h
	$(CC) $(COMPILE_FLAGS) word_finder.cc -o word_finder.o

fill.o: fill.cc fill.h
	$(CC) $(COMPILE_FLAGS) fill.cc

puzzle.o: puzzle.cc puzzle.h
	$(CC) $(COMPILE_FLAGS) puzzle.cc

graph.o: graph.cc graph.h puzzle.o
	$(CC) $(COMPILE_FLAGS) graph.cc

#TODO: I think by using the right compiler instead of g++, we can use object files here instead of compiling every time.
fill.js: bindings.cc word_finder.cc puzzle.cc fill.cc
	emcc --bind -o fill.js bindings.cc word_finder.cc puzzle.cc fill.cc

#Tests

trie_test: trie.o trie_test.cc
	$(CC) $(LINK_FLAGS) trie_test.cc trie.o -o trie_test

word_finder_test: word_finder.o word_finder_test.cc trie.o
	$(CC) $(LINK_FLAGS) word_finder_test.cc word_finder.o trie.o -o word_finder_test

fill_test: word_finder.o puzzle.o fill.o fill_test.cc graph.o
	$(CC) $(LINK_FLAGS) fill_test.cc fill.o word_finder.o puzzle.o graph.o -o fill_test

fill_generative_test: word_finder.o puzzle.o fill.o graph.o trie.o fill_generative_test.cc
	$(CC) $(LINK_FLAGS) fill_generative_test.cc fill.o word_finder.o puzzle.o graph.o trie.o -o fill_generative_test

puzzle_test: puzzle.o puzzle_test.cc word_finder.o
	$(CC) $(LINK_FLAGS) puzzle_test.cc puzzle.o word_finder.o -o puzzle_test

perf_test : perf_test.cc puzzle.o word_finder.o fill.o graph.o trie.o
	$(CC) $(LINK_FLAGS) perf_test.cc puzzle.o word_finder.o fill.o graph.o trie.o -o perf_test

graph_test: graph_test.cc graph.o puzzle.o
	$(CC) $(LINK_FLAGS) graph_test.cc puzzle.o graph.o -o graph_test

trie_perf_test : trie_perf_test.cc word_finder.o trie.o
	$(CC) $(LINK_FLAGS) trie_perf_test.cc word_finder.o trie.o -o trie_perf_test

# Other binaries

fill_cli: fill_cli.cc puzzle.o word_finder.o fill.o graph.o trie.o
	$(CC) $(LINK_FLAGS) fill_cli.cc puzzle.o word_finder.o fill.o graph.o trie.o -o fill_cli

clean:
	rm *.o trie_test word_finder_test fill_test puzzle_test fill.js perf_test fill_generative_test
