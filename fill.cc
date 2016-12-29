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
#include <map>
#include <set>

using std::string;
using std::vector;

struct ComponentMemoization {
  int hits; // Number of times we used this entry.
  std::vector<string> solutions;
};

// A fill order is a list of sets of Words. Each set is a connected component.
// To fill a connected component, we just do a DFS. So if we were writing this recursively it would be something like:
// f(puzzle, fill_order, fill_order_index):
//  Figure out what word is next: fill order (can remember this), then a heuristic based on fewest letters or something
//  for each match:
//    puzzle_prime = fill next_word with that match
//    f(puzzle_prime, fill_order)

// TODO: unify whether these field names have an underscore at the end (they should)
struct FillSearchState {
  Puzzle puzzle_;
  int fill_order_index_;
  bool first_in_c;
  string component_start;
  FillSearchState(const Puzzle& puzzle) : puzzle_(puzzle), fill_order_index_(0), first_in_c(true) {}
};

// TODO: naming
// TODO: maybe this should be a member function of FillSearchState
// return value is true if it's done.
string SingleThing(const std::vector<FillOrderEntry>& fill_order, const PuzzleGraph& graph, std::stack<FillSearchState>* stack, const WordFinder* word_finder, const WordFinder* user_word_finder, std::map<std::pair<int, std::string>, ComponentMemoization>* component_starts_seen, std::string* biggest_fill) {
  FillSearchState search_state = stack->top();
  //std::cout << "---------------------" << std::endl;
  //std::cout << "SingleThing on puzzle" << std::endl << search_state.puzzle_.PrettyString();
  stack->pop();
  if(search_state.first_in_c && fill_order[search_state.fill_order_index_].is_cc) {
    //std::cout << "First in component" << std::endl;
    string dumped_component = DumpComponentWithCrosswords(search_state.puzzle_, graph, fill_order[search_state.fill_order_index_].words);
    ComponentMemoization& component_memo = (*component_starts_seen)[{search_state.fill_order_index_, dumped_component}];
    component_memo.hits++;
    if( component_memo.hits > 1) {
      //std::cout << "Found a dumped component here. Key is " << dumped_component << std::endl;
      // TODO: alias (*components_starts_seen)[]... to make this much more readable.
      for(const auto& solved_component : component_memo.solutions) {
        FillSearchState new_state = search_state;
	++new_state.fill_order_index_;
	FillComponent(&new_state.puzzle_, fill_order[search_state.fill_order_index_].words, solved_component);
	stack->push(new_state);
      }
      return "";
    }
    //std::cout << "dumped component for this search state is " << dumped_component << std::endl;
    search_state.component_start = dumped_component;
  }
  // Determine whether there are any words left in this.
  Word word_with_fewest_to_fill;
  //std::cout << "Checking for words in component." << std::endl;
  int num_to_fill = std::numeric_limits<int>::max();
  for(const Word& word : fill_order[search_state.fill_order_index_].words) {
    int num_to_fill_here = search_state.puzzle_.BlanksInWordAt(word.coords_, word.direction_);
    //std::cout << " word " << word << " has num to fill here = " << num_to_fill_here;
    if(num_to_fill_here > 0 && num_to_fill_here < num_to_fill) {
      word_with_fewest_to_fill = word;
      num_to_fill = num_to_fill_here;
    }
  }
  if(num_to_fill == std::numeric_limits<int>::max()) {
    // We are done with this component. Are we done overall?
    //std::cout << "num to fill is max, so incrementing fill order index, which was " << search_state.fill_order_index_ << std::endl;
    ++search_state.fill_order_index_;
    if(search_state.fill_order_index_ == fill_order.size()) {
      return search_state.puzzle_.Data();
    }
    // Not done with all components; new component. Record this solution if this is a cc.
    // TODO: supah ugly.
    if(fill_order[search_state.fill_order_index_-1].is_cc) {
      std::vector<string>& component_solutions = (*component_starts_seen)[{search_state.fill_order_index_-1,search_state.component_start}].solutions;
      //std::cout << "Adding component with key " << search_state.component_start << std::endl;
      string dumped_solution = DumpComponent(search_state.puzzle_, fill_order[search_state.fill_order_index_-1].words);
      if(std::find(component_solutions.begin(), component_solutions.end(), dumped_solution) == component_solutions.end()) {
        component_solutions.push_back(dumped_solution);
      }
    }
    search_state.first_in_c = true;
    stack->push(search_state);
    return "";
  }
  const vector<string> matches = search_state.puzzle_.Matches(word_with_fewest_to_fill.coords_, word_with_fewest_to_fill.direction_, word_finder, user_word_finder);
  for(const auto& match : matches) {
    FillSearchState new_state = search_state;
    new_state.puzzle_.SetWord(word_with_fewest_to_fill.coords_, word_with_fewest_to_fill.direction_, match);
    new_state.first_in_c = false;
    stack->push(new_state);
  }
  if(matches.empty()) {
    std::string fill_here = search_state.puzzle_.Data();
    if(std::count(fill_here.begin(), fill_here.end(), ' ') < std::count(biggest_fill->begin(), biggest_fill->end(), ' ')) {
      *biggest_fill = fill_here;
    }
  }
  return "";
}

// Extremely naive approach to filling: a depth first search with pruning.
// Maintain a stack of partially-filled puzzles and indices.
// Uses a predetermined order.
// TODO: memoize the small CC's, which is where I actually expect getting a speedup here.
string Fill(const Puzzle& puzzle, const WordFinder* word_finder, string* biggest_fill) {
  *biggest_fill = puzzle.Data();
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
  const auto puzzle_graph = puzzle.AsGraph(); // TODO: This computes this twice.
  const auto fill_order = FillOrder(puzzle);
  // TODO: make this type more friendly using using-aliasing or a struct.
  // TODO: rename this
  // Key is: component index and dumped string, incl. crosswords. value is: number of times seen (TODO: drop it) and set of words which finish.
  // TODO: perhaps replace this with a set, but want to retain deterministic word order.
  std::map<std::pair<int, string>, ComponentMemoization> component_starts_seen;

  /*
  int i = 0;
  for(const auto& fill: fill_order) {
    std::cout << "fill number " << i << " has " << fill.size() << " Words one of which is " << fill[0] << std::endl;
    ++i;
  }
  */
  
  while(!depth_first_stack.empty()) {
    string result = SingleThing(fill_order, puzzle_graph, &depth_first_stack, word_finder, &user_word_finder, &component_starts_seen, biggest_fill);
    if(!result.empty()) return result;
  }
  for(const auto& component_memo_pair : component_starts_seen) {
    
    std::cout << component_memo_pair.first.first << "," << component_memo_pair.first.second << "," << component_memo_pair.second.hits << "," << component_memo_pair.second.solutions.size() << std::endl;
  }
  return ""; // No solution found.
}
