#include "fill.h"
#include "word_finder.h"
#include "puzzle.h"
#include <time.h>
#include <algorithm>
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
  // Constants which control the generative test
  int test_size = 2;  // Side length of crossword
  int extra_words_per_pattern = 2;
  int random_attempts = 10000;
  for(int i = 0; i < random_attempts; ++i) {
    // TODO: it might be inefficient to be reallocating the strings every time
    string grid = RandomGrid(test_size);
    Puzzle completed_puzzle(grid, test_size);
    Puzzle incomplete_puzzle(RandomlyAddBlanks(grid), test_size);
    vector<string> dictionary = completed_puzzle.AllWords();
    const vector<string> patterns = incomplete_puzzle.AllWords();
    for(const string& pattern : patterns) {
      // Ensure there are some other words which match this pattern, to cause backtracking. Ignore any patterns with no spaces (TODO: inelegant)
      for(int i = 0; i<extra_words_per_pattern; ++i) {
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
    std::random_shuffle(dictionary.begin(), dictionary.end());
    VectorWordFinder finder(dictionary);
    string filled = Fill(incomplete_puzzle, &finder);
    if(filled == "") {
      std::cout << "generative failed with:" << std::endl << completed_puzzle.PrettyString() << "=" << incomplete_puzzle.PrettyString() << " and dictionary " << std::endl;
      for(const auto& word : dictionary) {
        std::cout << word << std::endl;
      }
      return 1;
    } else {
      Puzzle filled_puzzle(filled, test_size);
      for(const auto word : filled_puzzle.AllWords()) {
        if(std::find(dictionary.begin(), dictionary.end(), word) == dictionary.end()) {
	  std::cout << "Found not-in-dict word " << word << std::endl << filled_puzzle.PrettyString() << "=" << incomplete_puzzle.PrettyString() << incomplete_puzzle.PrettyString() << " and dictionary " << std::endl;
          for(const auto& word : dictionary) {
            std::cout << word << std::endl;
          }
        return 1;
	}
      }
    }
  }
}
