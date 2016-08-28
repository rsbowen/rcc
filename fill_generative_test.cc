#include "fill.h"
#include "word_finder.h"
#include "puzzle.h"
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>

using std::string;
using std::vector;

// TODO: add #'s and negative examples.

string RandomGrid(const int size) {
  string completed_puzzle;
  completed_puzzle.reserve(size*size);
  for(int row = 0; row < size; ++row) {
    for(int col = 0; col < size; ++col) {
      int c = rand() % 26;
      completed_puzzle.push_back('a'+c);
    }
  }
  return completed_puzzle;
}

string RandomlyAddBlanks(const string& completed_puzzle) {
  string incomplete_puzzle = completed_puzzle;
  for(int i = 0; i < incomplete_puzzle.size(); ++i) {
    if (incomplete_puzzle[i] != '#' && (rand() % 2 == 0)) incomplete_puzzle[i] = ' ';
  }
  return incomplete_puzzle;
}

int main(int argc, char* argv[]) {
  srand(time(nullptr));
  int test_size = 4;
  for(int i = 0; i < 1000; ++i) {
    // TODO: it might be inefficient to be reallocating the strings every time
    string grid = RandomGrid(test_size);
    Puzzle completed_puzzle(grid, test_size);
    Puzzle incomplete_puzzle(RandomlyAddBlanks(grid), test_size);
    vector<string> dictionary = completed_puzzle.AllWords();
    const vector<string> patterns = incomplete_puzzle.AllWords();
    for(const string& pattern : patterns) {
      // Ensure there are some other words which match this pattern, to cause backtracking. Ignore any patterns with no spaces (TODO: inelegant)
      for(int i = 0; i<10; ++i) {
        int num_blanks = 0;
        string new_word = pattern;
	for(int pattern_idx = 0; pattern_idx < pattern.size(); ++pattern_idx) {
	  if(new_word[pattern_idx] == ' ') {
	    new_word[pattern_idx] = 'a' + (rand() % 26);
	    num_blanks++;
	  }
	}
	if(num_blanks==0) break;
	dictionary.push_back(new_word);
      }
    }
    VectorWordFinder finder(dictionary);
    string filled = Fill(incomplete_puzzle, &finder);
    if(filled == "") {
      std::cout << "generative failed with: " << completed_puzzle.Data() << "," << incomplete_puzzle.Data() << " and dictionary ";
      for(const auto& word : dictionary) {
        std::cout << word << ",";
      }
      return 1;
    }
  }
}
