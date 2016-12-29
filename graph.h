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
// Output: list (in order) of list of places to be filled.
// TODO: this is awful naming.
struct FillOrderEntry {
  std::vector<Word> words;
  // Is this a complete connected component?
  bool is_cc;
};

std::vector<FillOrderEntry> FillOrder(Puzzle puzzle);

// Helper for components. All the words in the component, as filled in the puzzle, in order. In the "WithCrosswords" variant, all cross words not in that component but crossing it also are appended.
std::string DumpComponent(const Puzzle& puzzle, const std::vector<Word> component);
std::string DumpComponentWithCrosswords(const Puzzle& puzzle, const PuzzleGraph& puzzle_graph, const std::vector<Word> component);

// The inverse of DumpComponent
void FillComponent(Puzzle* puzzle, const std::vector<Word> component, const std::string& dumped_component);

#endif
