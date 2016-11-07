#include "puzzle.h"

using std::string;

bool operator==(const Word& lhs, const Word& rhs) {
  return lhs.coords_.first == rhs.coords_.first && lhs.coords_.second == rhs.coords_.second && lhs.direction_ == rhs.direction_;
}
bool operator!=(const Word& lhs, const Word& rhs) { return !(lhs==rhs);}

std::ostream& operator<<(std::ostream& stream, const Word& obj) {
  stream << "(" << obj.coords_.first << "," << obj.coords_.second << "," << (obj.direction_ == Direction::DOWN ? "down)" : "across)");
  return stream;
}

int* Coord(Direction direction, std::pair<int, int>* coords) {
  if(direction == Direction::DOWN)
  {
    return &(coords->first);
  }
  return &(coords->second);
}

// TODO: sanity check size
Puzzle::Puzzle(const string& puzzle, int puzzle_size) : puzzle_size_(puzzle_size), data_(puzzle) {}

char Puzzle::At(std::pair<int, int> coords) const {
  // TODO: sanity check size
  return data_[coords.first*puzzle_size_ + coords.second];
}

void Puzzle::Set(std::pair<int, int> coords, char c) {
  data_[coords.first*puzzle_size_ + coords.second] = c;
}

std::pair<int, int> Puzzle::WordStart(std::pair<int, int> coords, Direction direction) const {
  while( *Coord(direction, &coords) > 0 && At(coords) != '#' ) {
   (*Coord(direction, &coords))--;
  }
  if(At(coords) == '#') {
    (*Coord(direction, &coords))++; //TODO: total hack
  }
  return coords;
}

std::string Puzzle::WordAt(std::pair<int, int> coords, Direction direction) const {
  string out = "";
  out.reserve(puzzle_size_);
  while( *Coord(direction, &coords) < puzzle_size_ && At(coords) != '#'){
    out.push_back(At(coords));
    (*Coord(direction, &coords))++;
  }
  return out;
}

int Puzzle::BlanksInWordAt(std::pair<int, int> coords, Direction direction) const {
  int total = 0;
  while(*Coord(direction, &coords) < puzzle_size_ && At(coords) != '#'){
    if(At(coords) == ' ') ++total;
    (*Coord(direction, &coords))++;
  }
  return total;
}

std::vector<std::string> Puzzle::Matches(std::pair<int, int> coords, Direction direction, const WordFinder* word_finder, const WordFinder* user_word_finder) {
  const string pattern = WordAt(coords, direction);
  std::vector<std::string> matches, user_matches;
  word_finder->FillMatches(&matches, pattern);
  user_word_finder->FillMatches(&user_matches, pattern);
  std::copy(user_matches.begin(), user_matches.end(), std::back_inserter(matches));
  std::vector<std::string> filtered_matches;
  for(const string& match : matches) {
    SetWord(coords, direction, match);
    auto coords_in_word = coords;
    auto cross_direction = (direction==Direction::ACROSS?Direction::DOWN:Direction::ACROSS);
    bool this_match_ok = true;
    for(;*Coord(direction, &coords_in_word) < puzzle_size_ && At(coords_in_word) != '#'; (*Coord(direction, &coords_in_word))++){
      auto cross_word_start_coords = WordStart(coords_in_word, cross_direction);
      //skip this check if the cross word is size 1.
      const string cross_pattern = WordAt(cross_word_start_coords, cross_direction);
      if(cross_pattern.size() == 1) continue;
      if(user_word_finder->LazyNumberOfMatches(1, cross_pattern) == 0 && word_finder->LazyNumberOfMatches(1, cross_pattern) == 0) {
        this_match_ok = false;
	break;
      }
    }
    if(this_match_ok) filtered_matches.push_back(match);
  }
  // Restore puzzle to before-for-loop state.
  SetWord(coords, direction, pattern);
  return filtered_matches;
}

std::string Puzzle::PrettyString() const {
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
  
int Puzzle::Size() const {return puzzle_size_;}

std::vector<std::string> Puzzle::AllWords() const {
  std::vector<std::string> all_words;
  for(int i = 0; i < puzzle_size_; ++i) {
    for(int j = 0; j < puzzle_size_; ++j) {
      for(const Direction& direction : {Direction::DOWN, Direction::ACROSS}) {
        if(WordStart({i,j}, direction) != std::pair<int, int>(i,j)) continue;
	all_words.push_back(WordAt({i,j}, direction));
      }
    }
  }
  return all_words;
}

PuzzleGraph Puzzle::AsGraph() const {
  PuzzleGraph graph;
  for(int i = 0; i<puzzle_size_; ++i) {
    for(int j = 0; j<puzzle_size_; ++j) {
      for(const Direction& direction : {Direction::DOWN, Direction::ACROSS}) {
        // TODO: this is not the clearest code ever written.
        Word coord_and_direction({{i,j}, direction});
        if(WordStart({i,j}, direction) != std::pair<int, int>(i,j)) continue;
	const string word = WordAt({i,j}, direction);
	if(word.find(" ") == std::string::npos) continue;
        const Direction other_direction = (direction==Direction::DOWN ? Direction::ACROSS : Direction::DOWN);
	std::pair<int, int> coords(i,j);
	std::vector<Word> neighbors;
	for(const char c : word) {
	  const string crossword = WordAt(coords, other_direction);
	  if(crossword.find(" ") != std::string::npos) {
	    neighbors.push_back({WordStart(coords, other_direction), other_direction});
	  }
	  (*Coord(direction, &coords))++;
	}
	graph[coord_and_direction] = neighbors;
      }
    }
  }
  return graph;
}

string Puzzle::Data() const {return data_;}

PuzzleIterator::PuzzleIterator(Puzzle& puzzle) : puzzle_(puzzle), coord_({-1,0}), direction_(Direction::DOWN) {
  IncrementOnce();
}

Word PuzzleIterator::Get() {
  return {coord_, direction_};
}

bool PuzzleIterator::Valid() {
  return coord_.first != -1;
}

void PuzzleIterator::Increment() {
  do {
    IncrementOnce();
  } while (coord_.first != -1 && puzzle_.WordStart(coord_, direction_) != coord_);
}

void PuzzleIterator::IncrementOnce() {
  if(direction_ == Direction::ACROSS) {
    direction_ = Direction::DOWN;
    return;
  }
  direction_ = Direction::ACROSS;
  if(coord_.first + 1 < puzzle_.Size()) {
    ++coord_.first;
    return;
  }
  if(coord_.second + 1 < puzzle_.Size()) {
    ++coord_.second;
    coord_.first = 0;
    return;
  }
  coord_.first = -1;
  coord_.second = -1;
}
