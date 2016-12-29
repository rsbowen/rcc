#ifndef FILL_H
#define FILL_H

#include<vector>
#include<string>
#include "word_finder.h"
#include "puzzle.h"

// Exposed for testing.

enum class DoConfigResult {MATCHES_FOUND, IMPOSSIBLE, CC_COMPLETE};
DoConfigResult DoConfig(Puzzle configuration, const WordFinder* word_finder, std::pair<int, int>* word_start, Direction* word_direction, std::vector<std::string>* matches);

// TODO: change this to a bool, with either biggest_fill or the solution filled in to an out-parameter string pointer.
std::string Fill(const Puzzle& puzzle, const WordFinder* word_finder, std::string* biggest_fill);

#endif //FILL_H
