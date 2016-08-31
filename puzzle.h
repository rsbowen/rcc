#ifndef PUZZLE_H
#define PUZZLE_H

#include <string>
#include <vector>
#include <unordered_map>

enum class Direction {ACROSS, DOWN};

// TODO: make all the functions in Puzzle take this.
// TODO: this name sucks
struct Word {
  Word(const std::pair<int, int>& coords, const Direction& direction) : coords_(coords), direction_(direction) {}
  std::pair<int, int> coords_;
  Direction direction_;
};
bool operator==(const Word& lhs, const Word& rhs);
bool operator!=(const Word& lhs, const Word& rhs);

// TODO: maybe these live in a cc file
namespace std {
template <> struct hash<Word> {
  typedef Word argument_type;
  typedef std::size_t result_type;
  // TODO: maybe this hash function sucks. maybe find a hash combine function somewhere. boost?
  size_t operator() (const Word& word) const {
    return std::hash<std::string>()(std::to_string(word.coords_.first)+"x"+std::to_string(word.coords_.second)+"x"+std::to_string(static_cast<int>(word.direction_)));
  }
};
} //namespace std

// Adjacency Lists
using PuzzleGraph = std::unordered_map<Word, std::vector<Word>>; 

class Puzzle {
 public:
  Puzzle(const std::string& puzzle, int puzzle_size);

  // TODO: not really good that direction comes second here but first in the implemenation helpers like Coord()
  char At(std::pair<int, int> coords) const;
  void Set(std::pair<int, int> coords, char c);
  std::pair<int, int> WordStart(std::pair<int, int> coords, Direction direction) const;
  std::string WordAt(std::pair<int, int> coords, Direction direction) const;
  void SetWord(std::pair<int, int> coords, Direction direction, std::string word);
  int Size() const;
  std::vector<std::string> AllWords() const;
  std::string Data() const;
  std::string PrettyString() const;
  // Vertices are just not-yet-complete words. Edges are whether they meet.
  PuzzleGraph AsGraph() const;
  
 private:
  int puzzle_size_;
  std::string data_;
};

#endif
