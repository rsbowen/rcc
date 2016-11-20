#include <algorithm>
#include <limits>
#include <stack>
#include <string>
#include <vector>
#include <unordered_set>

#include "word_finder.h"
#include "fill.h"
#include "puzzle.h"
#include "graph.h"

#include <iostream> // temp

using std::string;
using std::vector;

// A fill order is a list of sets of Words. Each set is a connected component.
// To fill a connected component, we just do a DFS. So if we were writing this recursively it would be something like:
// f(puzzle, fill_order, fill_order_index):
//  Figure out what word is next: fill order (can remember this), then a heuristic based on fewest letters or something
//  for each match:
//    puzzle_prime = fill next_word with that match
//    f(puzzle_prime, fill_order)

struct FillSearchState {
  Puzzle puzzle_;
  int fill_order_index_;
  FillSearchState(const Puzzle& puzzle) : puzzle_(puzzle), fill_order_index_(0) {}
};

// TODO: naming
// TODO: maybe this should be a member function of FillSearchState
// return value is true if it's done.
string SingleThing(const std::vector<std::unordered_set<Word>>& fill_order,std::stack<FillSearchState>* stack, const WordFinder* word_finder, const WordFinder* user_word_finder) {
  FillSearchState search_state = stack->top();
  stack->pop();
  // Determine whether there are any words left in this.
  Word word_with_fewest_to_fill;
  int num_to_fill = std::numeric_limits<int>::max();
  for(const Word& word : fill_order[search_state.fill_order_index_]) {
    int num_to_fill_here = search_state.puzzle_.BlanksInWordAt(word.coords_, word.direction_);
    if(num_to_fill_here > 0 && num_to_fill_here < num_to_fill) {
      word_with_fewest_to_fill = word;
      num_to_fill = num_to_fill_here;
    }
  }
  if(num_to_fill == std::numeric_limits<int>::max()) {
    // We are done with this component. Are we done overall?
    ++search_state.fill_order_index_;
    if(search_state.fill_order_index_ == fill_order.size()) {
      return search_state.puzzle_.Data();
    } else {
      stack->push(search_state);
      return "";
    }
  }
  const vector<string> matches = search_state.puzzle_.Matches(word_with_fewest_to_fill.coords_, word_with_fewest_to_fill.direction_, word_finder, user_word_finder);
  for(const auto& match : matches) {
    FillSearchState new_state = search_state;
    new_state.puzzle_.SetWord(word_with_fewest_to_fill.coords_, word_with_fewest_to_fill.direction_, match);
    stack->push(new_state);
  }
  return "";
}

// Extremely naive approach to filling: a depth first search with pruning.
// Maintain a stack of partially-filled puzzles and indices.
// Uses a predetermined order.
// TODO: memoize the small CC's, which is where I actually expect getting a speedup here.
string Fill(const Puzzle& puzzle, const WordFinder* word_finder) {
  std::stack<FillSearchState> depth_first_stack;
  // Save the words which were user-provided in the sense that they are complete words in the puzzle.
  std::vector<std::string> input_words = puzzle.AllWords();
  std::vector<std::string> user_words = {};
  for(const string& word : input_words) {
    if(std::find(word.begin(), word.end(), ' ') != word.end()) continue;
    user_words.push_back(word);
  }

  VectorWordFinder user_word_finder(user_words);

  depth_first_stack.push({puzzle});
  const auto fill_order = FillOrder(puzzle);
  
  while(!depth_first_stack.empty()) {
    string result = SingleThing(fill_order, &depth_first_stack, word_finder, &user_word_finder);
    if(!result.empty()) return result;
  }
  return ""; // No solution found.
}
