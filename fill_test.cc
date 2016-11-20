#include "fill.h"
#include "puzzle.h"
#include <iostream>
#include <vector>

using std::string;
using std::vector;

int main(int argc, char* argv[]) {
/*
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
  */

  Puzzle puzzle("a   ", 2);
  // Extremely simple fill: 2x2 like this
  // AS
  // TO
  // starting from
  // A?
  // ??

  VectorWordFinder word_finder({"as", "to", "at", "so"});
  string filled = Fill(puzzle, &word_finder);
  if(filled.size() == 0) { std::cout << "Couldn't fill simple test "; }

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
  if(filled.size() == 0) { std::cout << "Couldn't fill backtracking test"; }

  // Weird special-case test found by generative testing: the last fill wasn't having its cross-words checked. This is hard to test well. This particular test depends on implementation details: that the word with the fewest matches is filled first. In this test there is only one square left to fill. Its across has 1 possibilities and its down has 2, so the across will be filled first. But that across does not produce a valid puzzle.
  // A B
  // C ?
  Puzzle final_fill_cross_puzzle("abc ", 2);
  VectorWordFinder final_fill_cross_word_finder({"ab", "ac", "cx", "by", "bz"});
  filled = Fill(final_fill_cross_puzzle, &final_fill_cross_word_finder);
  if(filled.size() != 0) { std::cout << "Final-fill-cross test failed, producing the fill '" << filled << "'" << std::endl;}

  // Check that, if some initial word is not in the dictionary, it's still okay.
  // A B
  // C ?
  Puzzle user_word_puzzle("abc ", 2);
  VectorWordFinder user_word_finder({"cx", "bx"});
  filled = Fill(user_word_puzzle, &user_word_finder);
  if(filled.size() == 0) { std::cout << "User Word Puzzle failure"; }

  // 
  return 0;
}


