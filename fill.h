#ifndef FILL_H
#define FILL_H

#include<vector>
#include<string>
#include "word_finder.h"
#include "puzzle.h"

// Exposed for testing.

enum class DoConfigResult {MATCHES_FOUND, IMPOSSIBLE, CC_COMPLETE};
DoConfigResult DoConfig(Puzzle configuration, const WordFinder* word_finder, std::pair<int, int>* word_start, Direction* word_direction, std::vector<std::string>* matches);

std::string Fill(const Puzzle& puzzle, const WordFinder* word_finder);

#endif //FILL_H
