#ifndef PUZZLE_H
#define PUZZLE_H

#include <string>
#include <vector>

enum class Direction {ACROSS, DOWN};

class Puzzle {
 public:
  Puzzle(const std::string& puzzle, int puzzle_size);

  // TODO: not really good that direction comes second here but first in the implemenation helpers like Coord()
  char At(std::pair<int, int> coords);
  void Set(std::pair<int, int> coords, char c);
  std::pair<int, int> WordStart(std::pair<int, int> coords, Direction direction);
  std::string WordAt(std::pair<int, int> coords, Direction direction);
  void SetWord(std::pair<int, int> coords, Direction direction, std::string word);
  int Size();
  std::vector<std::string> AllWords();
  std::string Data();
  std::string PrettyString();
  
 private:
  int puzzle_size_;
  std::string data_;
};

#endif
