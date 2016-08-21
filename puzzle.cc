#include "puzzle.h"
// tmp
#include <iostream>

using std::string;

int* Coord(Direction direction, std::pair<int, int>* coords) {
  switch(direction) {
    case Direction::DOWN: {
      return &(coords->first);
    }
    case Direction::ACROSS: {
      return &(coords->second);
    }
  }
}

// TODO: sanity check size
Puzzle::Puzzle(const string& puzzle, int puzzle_size) : data_(puzzle), puzzle_size_(puzzle_size) {}

char Puzzle::At(std::pair<int, int> coords) {
  // TODO: sanity check size
  return data_[coords.first*puzzle_size_ + coords.second];
}

void Puzzle::Set(std::pair<int, int> coords, char c) {
  data_[coords.first*puzzle_size_ + coords.second] = c;
}

std::pair<int, int> Puzzle::WordStart(std::pair<int, int> coords, Direction direction) {
  while( *Coord(direction, &coords) > 0 && At(coords) != '#' ) {
   (*Coord(direction, &coords))--;
  }
  if(At(coords) == '#') {
    (*Coord(direction, &coords))++; //TODO: total hack
  }
  return coords;
}

std::string Puzzle::WordAt(std::pair<int, int> coords, Direction direction) {
  string out = "";
  out.reserve(puzzle_size_);
  while( *Coord(direction, &coords) < puzzle_size_ && At(coords) != '#'){
    out.push_back(At(coords));
    (*Coord(direction, &coords))++;
  }
  return out;
}

std::string Puzzle::PrettyString() {
  std::string output;
  output.reserve(1+puzzle_size_*(puzzle_size_+1));

  output.push_back('\n');
  for(int row = 0; row < puzzle_size_; ++row) {
    for(int col = 0; col < puzzle_size_; ++col) {
      output.push_back(At(std::pair<int, int>(row, col)));
    }
    output.push_back('\n');
  }
  return output;
}

void Puzzle::SetWord(std::pair<int, int> coords, Direction direction, string word) {
  for (const char c : word) {
    Set(coords, c);
    (*Coord(direction, &coords))++;
  }
}
  
int Puzzle::Size() {return puzzle_size_;}
string Puzzle::Data() {return data_;}
