#include "fill.h"
#include "puzzle.h"
#include <iostream>
#include <vector>

using std::string;
using std::vector;

int main(int argc, char* argv[]) {
  // Simplest DoConfig test: only one thing left to fill.
  // as  
  //  #
  Puzzle do_config_test_puzzle("as #", 2);
  VectorWordFinder do_config_word_finder({"as", "s", "r", "t", "q"}); // Additional 1-letter 'words' are to make sure that the 1-letter across at (1,0) won't cause us to return IMPOSSIBLE.
  std::pair<int, int> word_start;
  Direction direction;
  bool is_done;
  vector<string> matches;
  DoConfigResult result = DoConfig(do_config_test_puzzle, &do_config_word_finder, &word_start, &direction, &matches);
  if(result != DoConfigResult::MATCHES_FOUND) std::cout << "result was wrong" << std::endl;
  if(word_start != std::pair<int, int>(0,0)) std::cout << " word_start was " << word_start.first << "," << word_start.second << std::endl;
  if(direction != Direction::DOWN) std::cout << " direction was " << (direction == Direction::DOWN? "Down":"Across") << std::endl;
  if(is_done) std::cout << "is_done was true" << std::endl;
  if(matches.size() != 1 || matches[0] != "as") std::cout << "matches was wrong" << std::endl;

  Puzzle puzzle("a   ", 2);
  // Extremely simple fill: 2x2 like this
  // AS
  // TO
  // starting from
  // A?
  // ??

  VectorWordFinder word_finder({"as", "to", "at", "so"});
  string filled = Fill(puzzle, &word_finder);
  std::cout << filled << std::endl;

  // Example which forces backtracking
  // AS
  // TO
  //
  // starting from
  // A?
  // ??
  // But with "AB" in there to force backtracking
  VectorWordFinder bigger_word_finder({"ab", "as", "to", "at", "so"});
  filled = Fill(puzzle, &bigger_word_finder);
  std::cout << filled << std::endl;
  return 0;
}


