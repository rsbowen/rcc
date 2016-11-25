#ifndef GRAPH_H
#define GRAPH_H

#include "puzzle.h"
#include <unordered_set>
#include <unordered_map>

// TODO: move puzzlegraph definition here.

std::vector<std::vector<Word>> ConnectedComponents(const PuzzleGraph& graph);

// Strategy:
//  1) If the smallest connected component has 6 or fewer words, fill it (all at once)
//  2) If filling any word would increase the number of connected components, fill it.
//  3) Fill the word with the most blanks
//
// Output: list (in order) of sets (not in order) of words to be filled.
std::vector<std::unordered_set<Word>> FillOrder(Puzzle puzzle);

// Helper for components. All the words in the component, as filled in the puzzle, in order.
std::string DumpComponent(const Puzzle& puzzle, const std::vector<Word> component);

#endif
