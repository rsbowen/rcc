#ifndef PUZZLE_H
#define PUZZLE_H

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

#include "word_finder.h"

enum class Direction {ACROSS, DOWN};

// TODO: make all the functions in Puzzle take this.
// TODO: this name sucks
// TODO: make sure consistent throughout codebase whether using 'coord' or 'coords'
struct Word {
  Word(const std::pair<int, int>& coords, const Direction& direction) : coords_(coords), direction_(direction) {}
  Word(int row, int col, const Direction& direction) : coords_({row, col}), direction_(direction) {}
  Word() : coords_(-1, -1), direction_(Direction::DOWN) {}
  friend bool operator<(const Word& l, const Word& r) {
    if(l.coords_.first < r.coords_.first) return true;
    if(l.coords_.first > r.coords_.first) return false;
    if(l.coords_.second < r.coords_.second) return true;
    if(l.coords_.second > r.coords_.second) return false;
    if(l.direction_ == Direction::ACROSS && r.direction_ == Direction::DOWN) return true;
    return false;
}
  std::pair<int, int> coords_;
  Direction direction_;
};
bool operator==(const Word& lhs, const Word& rhs);
bool operator!=(const Word& lhs, const Word& rhs);
std::ostream& operator<<(std::ostream& stream, const Word& obj);

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
  int BlanksInWordAt(std::pair<int, int> coords, Direction direction) const;
  void SetWord(std::pair<int, int> coords, Direction direction, std::string word);
  std::vector<std::string> Matches(std::pair<int, int> coords, Direction direction, const WordFinder* word_finder, const WordFinder* user_word_finder);
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


// Not quite a standard c++ iterator because a crossword isn't quite a collection of Words (though that would be cool)
// TODO: convert all iterations over all words in the codebase to using this.
class PuzzleIterator {
 public:
  PuzzleIterator(Puzzle& puzzle);
  Word Get();
  bool Valid();
  // TODO: make sure this matches sort order.
  void Increment();

 private:
  void IncrementOnce();

  Puzzle& puzzle_;
  std::pair<int, int> coord_;
  Direction direction_;
};

#endif
