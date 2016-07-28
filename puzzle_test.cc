#include "puzzle.h"

#include <iostream>

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
  // h i j k
  // l m n o
  Puzzle puzzle2("abcde#fghijklmno", 4);
  if(puzzle2.WordStart({0,0}, Direction::DOWN) != std::pair<int, int>({0,0})) std::cout << "WordStart(0,0)" << std::endl;
  if(puzzle2.WordStart({1,0}, Direction::DOWN) != std::pair<int, int>({0,0})) std::cout << "WordStart(1,0)" << std::endl;
  if(puzzle2.WordStart({1,3}, Direction::ACROSS) != std::pair<int, int>({1,2})) std::cout << "WordStart(1,3)" << std::endl;

  // Test the WordAt function.
  if(puzzle2.WordAt({0,0}, Direction::ACROSS) != "abcd") std::cout << "WordAt(0,0,across)" << std::endl;
  if(puzzle2.WordAt({0,0}, Direction::DOWN) != "aehl") std::cout << "WordAt(0,0,down)" << std::endl;
  if(puzzle2.WordAt({1,2}, Direction::ACROSS) != "fg") std::cout << "WordAt(1,2,across)" << std::endl;
  if(puzzle2.WordAt({2,1}, Direction::DOWN) != "im") std::cout << "WordAt(2,1,down)" << std::endl;

  // Test the SetWord function
  puzzle2.SetWord({3,0}, Direction::ACROSS, "test");
  if(puzzle2.WordAt({3,0}, Direction::ACROSS) != "test") std::cout << "SetWord(3,0,across)" << puzzle2.Data() << std::endl;

  return 0;
}
