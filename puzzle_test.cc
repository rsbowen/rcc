#include "puzzle.h"

#include <iostream>
#include <algorithm>

int main(int argc, char* argv[]) {
  // Test the At function
  // Puzzle is
  //  a b c
  //  d e f
  //  g h i
  Puzzle puzzle("abcdefghi", 3);
  if(puzzle.At({0,0}) != 'a') std::cout << "At((0,0))" << std::endl;
  if(puzzle.At({0,1}) != 'b') std::cout << "At((0,1))" << std::endl;
  if(puzzle.At({0,2}) != 'c') std::cout << "At((0,2))" << std::endl;
  if(puzzle.At({1,0}) != 'd') std::cout << "At((1,0))" << std::endl;
  if(puzzle.At({1,1}) != 'e') std::cout << "At((1,1))" << std::endl;
  if(puzzle.At({1,2}) != 'f') std::cout << "At((1,2))" << std::endl;
  if(puzzle.At({2,0}) != 'g') std::cout << "At((2,0))" << std::endl;
  if(puzzle.At({2,1}) != 'h') std::cout << "At((2,1))" << std::endl;
  if(puzzle.At({2,2}) != 'i') std::cout << "At((2,2))" << std::endl;

  // Test the Set function
  puzzle.Set({1,2}, 'q');
  if(puzzle.At({1,2}) != 'q') std::cout << "Set((1,2))" << std::endl; 
  // Test the WordStart function
  // Puzzle is
  // a b c d
  // e # f g 
  // h   j k
  // l   n o
  Puzzle puzzle2("abcde#fgh jkl no", 4);
  if(puzzle2.WordStart({0,0}, Direction::DOWN) != std::pair<int, int>({0,0})) std::cout << "WordStart(0,0)" << std::endl;
  if(puzzle2.WordStart({1,0}, Direction::DOWN) != std::pair<int, int>({0,0})) std::cout << "WordStart(1,0)" << std::endl;
  if(puzzle2.WordStart({1,3}, Direction::ACROSS) != std::pair<int, int>({1,2})) std::cout << "WordStart(1,3)" << std::endl;

  // Test the WordAt function.
  if(puzzle2.WordAt({0,0}, Direction::ACROSS) != "abcd") std::cout << "WordAt(0,0,across)" << std::endl;
  if(puzzle2.WordAt({0,0}, Direction::DOWN) != "aehl") std::cout << "WordAt(0,0,down)" << std::endl;
  if(puzzle2.WordAt({1,2}, Direction::ACROSS) != "fg") std::cout << "WordAt(1,2,across)" << std::endl;
  if(puzzle2.WordAt({2,1}, Direction::DOWN) != "  ") std::cout << "WordAt(2,1,down)" << std::endl;

  if(puzzle2.BlanksInWordAt({0,0}, Direction::ACROSS) != 0) std::cout << "BIWA(0,0,across)" << std::endl;
  if(puzzle2.BlanksInWordAt({2,0}, Direction::ACROSS) != 1) std::cout << "BIWA(2,0,across)" << std::endl;
  if(puzzle2.BlanksInWordAt({2,1}, Direction::DOWN) != 2) std::cout << "BIWA(2,1,across)" << std::endl;

  // Test WordAt with spaces.
  Puzzle puzzle3("a  c", 2);
  if(puzzle3.WordAt({0,0}, Direction::ACROSS) != "a ") std::cout << "with spaces, WordAt(0,0,across)" << std::endl;
  if(puzzle3.WordAt({1,0}, Direction::ACROSS) != " c") std::cout << "with spaces, WordAt(1,0,across)" << std::endl;

  // Test the SetWord function
  puzzle2.SetWord({3,0}, Direction::ACROSS, "test");
  if(puzzle2.WordAt({3,0}, Direction::ACROSS) != "test") std::cout << "SetWord(3,0,across)" << puzzle2.Data() << std::endl;

  // Test the AllWords function
  Puzzle all_words_puzzle("ab#cdefghijk#lmn", 4);
  auto all_words = all_words_puzzle.AllWords();
  std::sort(all_words.begin(), all_words.end());
  std::vector<std::string> expected_all_words = {"ab", "adh", "beil", "c", "cgkn", "defg", "fjm", "hijk", "lmn"};
  if(all_words.size() != expected_all_words.size()) std::cout << "all_words size wrong";
  for(int i = 0; i<all_words.size(); ++i) {
    if(all_words[i] != expected_all_words[i]) std::cout << "i is " << i << "all_words[i] is " << all_words[i] << " but expected_all_words[i] is " << expected_all_words[i];
  }

  // Test Puzzle::Matches with a 3x3 puzzle:
  // # ? #
  // ? ? ? 
  // # # #
  // And the dictionary: [abc, def, xxx, ab, bb, be, a, g]
  // Vary the singleton letter at the top and query the triple-long one
  // * singleton:empty, expect abc, def
  // * singleton:a, expect abc
  // * singleton:g, expect nothing

  // Some of the words are put into the "user" vector word finder
  VectorWordFinder vwf({"abc","xxx","bb","be","a","g"});
  VectorWordFinder uvwf({"def", "ab", "g"});
  Puzzle matches_puzzle_1("# #   ###", 3);
  Puzzle matches_puzzle_2("#a#   ###", 3);
  Puzzle matches_puzzle_3("#g#   ###", 3);
  Word word({1,0}, Direction::ACROSS);
  const auto matches_1 = matches_puzzle_1.Matches(word.coords_, word.direction_, &vwf, &uvwf);
  const auto matches_2 = matches_puzzle_2.Matches(word.coords_, word.direction_, &vwf, &uvwf);
  const auto matches_3 = matches_puzzle_3.Matches(word.coords_, word.direction_, &vwf, &uvwf);
  if(matches_1.size() != 2) {
    std::cout << "matches_1 has bad size " << matches_1.size() << std::endl;
    for(const auto& match : matches_1) std::cout << match << std::endl;
  } else if(!((matches_1[0] == "abc" && matches_1[1] == "def") || (matches_1[0]=="def" && matches_1[1] == "abc"))) {
      std::cout << "matches_1 has right size but bad elements " << matches_1[0] << "," << matches_1[1];
  }

  if (matches_2.size() != 1) {
    std::cout << "matches_2 has bad size " << matches_2.size();
  } else if (matches_2[0] != "abc") {
    std::cout << "matches_2 has bad element " << matches_2[0];
  }

  if (matches_3.size() != 0) {
    std::cout << "matches_3 has bad size " << matches_3.size();
  }

  // Test As Graph: 3x3 donut with things on the side like this
  //   a   x
  // b # c x
  //   d   x
  // x x x x

  Puzzle graph_puzzle(" a xb#cx d xxxxx", 4);
  PuzzleGraph graph = graph_puzzle.AsGraph();


  // TODO: this makes assumptions about the order, which is an implementation detail. fix with real testing framework that can do unordered container content compares or something.
  bool good_graph = true;
  if(graph.size() != 4) good_graph = false;
  Word one_across({0,0}, Direction::ACROSS);
  Word one_down({0,0}, Direction::DOWN);
  Word two_down({0,2}, Direction::DOWN);
  Word three_across({2,0}, Direction::ACROSS);
  if(graph[one_across].size() != 2 || graph[one_down].size() != 2 || graph[two_down].size() != 2 || graph [three_across].size() != 2) {
    good_graph = false;
  }

  if(good_graph && graph[one_across][0] != one_down) good_graph = false;
  if(good_graph && graph[one_across][1] != two_down) good_graph = false;
  if(good_graph && graph[one_down][0] != one_across) good_graph = false;
  if(good_graph && graph[one_down][1] != three_across) good_graph = false;
  if(good_graph && graph[two_down][0] != one_across) good_graph = false;
  if(good_graph && graph[two_down][1] != three_across) good_graph = false;
  if(good_graph && graph[three_across][0] != one_down) good_graph = false;
  if(good_graph && graph[three_across][1] != two_down) good_graph = false;

  if(!good_graph) { std::cout << "Bad graph!" << std::endl;}

  Puzzle it_puzzle("ab#c#defg", 3);
  auto iter = PuzzleIterator(it_puzzle);
  std::vector<std::string> words_from_iterator;
  for(auto iter = PuzzleIterator(it_puzzle); iter.Valid(); iter.Increment()){
    Word location = iter.Get();
    words_from_iterator.push_back(it_puzzle.WordAt(location.coords_, location.direction_));
  }
  std::sort(words_from_iterator.begin(), words_from_iterator.end());
  if(words_from_iterator.size() != 8) { std::cout << "Bad # of words from iterator: " << words_from_iterator.size() << std::endl; return 1;};
  std::vector<std::string> words_expected = {"ab", "ace", "b", "c", "d", "dg", "efg", "f"};
  for(int i = 0; i < 8; ++i) {
    if(words_expected[i] != words_from_iterator[i]) {std::cout << "mismatch, got " << words_from_iterator[i] << " wanted " << words_expected[i] << std::endl;};
  }

  return 0;
}
