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

int main(int argc, char* argv[]) {
  srand(time(nullptr));
  int test_size = 4;
  for(int i = 0; i < 1000; ++i) {
    string completed_puzzle = "";
    string incomplete_puzzle = "";
    completed_puzzle.reserve(test_size*test_size);
    incomplete_puzzle.reserve(test_size*test_size);
    for(int row = 0; row < test_size; ++row) {
      for(int col = 0; col<test_size; ++col) {
        int c = rand() % 26;
	completed_puzzle.push_back('a'+c);
	if(rand()%2) incomplete_puzzle.push_back('a'+c);
	else incomplete_puzzle.push_back(' ');
       }
     }
     // TODO(rsbowen): these names suck!
     Puzzle completed_Puzzle(completed_puzzle, test_size);
     Puzzle incomplete_Puzzle(incomplete_puzzle, test_size);
     vector<string> words;
     for(int i = 0; i < test_size; ++i) {
       words.push_back(completed_Puzzle.WordAt({0,i}, Direction::DOWN));
       words.push_back(completed_Puzzle.WordAt({i,0}, Direction::ACROSS));
     }
     VectorWordFinder finder(words);
     string filled = Fill(incomplete_Puzzle, &finder);
     if(filled == "") {
       std::cout << "generative failed with: " << completed_puzzle << "," << incomplete_puzzle;
       return 1;
     }
     std::cout incomplete_puzzle << "//" << filled << "//" << completed_Puzzle.Data() << std::endl;
   }
}
