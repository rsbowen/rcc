#include <algorithm>
#include <limits>
#include <stack>
#include <string>
#include <vector>

#include "word_finder.h"
#include "fill.h"
#include "puzzle.h"

#include <iostream> // temp

using std::string;
using std::vector;

// TODO: better naming: work out which one is smallest, better doc
// is_done: this configuration is already filled in all the way
// row, col: the place the word under consideration will start
// matches: if empty, there is an unmatchable thing or a bad word here
// TODO: can probably at each depth level in the stack just only look at the words that touch the cell we just changed, which would save a lot of time.
// TODO(don't call both 'configuration' and 'puzzle'. Probably will be solved by a wrapper class instead of passing strings and size around everywhere.
// TODO: maybe test individually?
// TODO: maybe this should return is_down
DoConfigResult DoConfig(Puzzle configuration, const WordFinder* word_finder, std::pair<int, int>* word_start, Direction* word_direction, vector<string>* matches)
{
  matches->clear();
  int min_num_matches_seen = std::numeric_limits<int>::max();
  bool puzzle_is_complete = true;
  string pattern_for_min_matches;
  for(int row = 0; row < configuration.Size(); ++row) {
    for(int col = 0; col < configuration.Size(); ++col) {
      // Only non-'#' squares can possibly be word starts.
      if(configuration.At({row, col}) == '#') continue;
      for(const Direction& direction : {Direction::DOWN, Direction::ACROSS}) {
        // Skip if not a word start.
        if (configuration.WordStart({row, col}, direction) != std::pair<int, int>(row, col)) continue;
        const string pattern = configuration.WordAt({row, col}, direction);
        // If this is a complete word then nothing to do. 
	if(std::find(pattern.begin(), pattern.end(), ' ') == pattern.end()) continue;
        int num_matches = word_finder->LazyNumberOfMatches(min_num_matches_seen, pattern);
	//std::cout << "pattern " << pattern << " had " << num_matches << " matches." << std::endl;
        if(num_matches == 0) {
          // Bad! Matches is already empty so just return.
          return DoConfigResult::IMPOSSIBLE;
        }
	//If this is not the new shortest then nothing to do. 
	if(num_matches >= min_num_matches_seen) continue;
        min_num_matches_seen = num_matches;
	pattern_for_min_matches = pattern;
	puzzle_is_complete = false;
	*word_direction = direction;
        *word_start = {row, col};
      }
    } 
  }
  if(puzzle_is_complete) return DoConfigResult::COMPLETE;
  // Fill in *matches.
  word_finder->FillMatches(matches, pattern_for_min_matches);
  return DoConfigResult::MATCHES_FOUND;
}

// Extremely naive approach to filling: a depth first search with pruning.
// Maintain a stack of partially-filled puzzles and indices.
// For each puzzle, we will search over all the possibilities for the one with the smallest number of possibilities. Recompute that every time (yikes, maybe can save memory here).
string Fill(const Puzzle& puzzle, const WordFinder* word_finder) {
  std::stack<std::pair<Puzzle, int>> depth_first_stack;
  depth_first_stack.push({puzzle, 0});
  while(!depth_first_stack.empty()) {
    //std::cout << "entering dfs, stack size is " << depth_first_stack.size() << std::endl;
    std::pair<Puzzle, int> configuration = depth_first_stack.top();
    depth_first_stack.pop();
    std::pair<int, int> next_word_coords;
    Direction direction;
    vector<string> matches;
    DoConfigResult result = DoConfig(configuration.first, word_finder, &next_word_coords, &direction, &matches);
    //std::cout << "in dfs, there are " << matches.size() << " matches " << std::endl;
    if(result == DoConfigResult::COMPLETE) return configuration.first.Data();
    // Before we push the next search, push on this one.
    if(configuration.second + 1< matches.size()) {
      //std::cout << "pushing back next-in-same-config" << std::endl;
      depth_first_stack.push({configuration.first, configuration.second+1});
    }
    if(result == DoConfigResult::IMPOSSIBLE) {
      // Got into a configuration that's impossible; we're done; don't push back children
      continue;
    }
    const string& match = matches[configuration.second];
    configuration.first.SetWord(next_word_coords, direction, match);
    // Depth-first our way down here.

    //std::cout << "pushing back depth config" << std::endl;
    depth_first_stack.push({configuration.first, 0});
  }
  return ""; // No solution found.
}
