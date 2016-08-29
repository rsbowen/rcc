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
          //std::cout << "Impossible on '" << pattern << "' row " << row << " col " << col; 
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

struct FillSearchState {
  Puzzle puzzle_;
  std::vector<std::string> matches_;
  std::pair<int, int> next_word_coords_;
  Direction direction_;
  int which_; // TODO: better name
  DoConfigResult result_;
  FillSearchState(const Puzzle& puzzle) : puzzle_(puzzle), which_(0) {}
};

// Extremely naive approach to filling: a depth first search with pruning.
// Maintain a stack of partially-filled puzzles and indices.
// For each puzzle, we will search over all the possibilities for the one with the smallest number of possibilities. Recompute that every time (yikes, maybe can save memory here).
string Fill(const Puzzle& puzzle, const WordFinder* word_finder) {
  std::stack<FillSearchState> depth_first_stack;
  // Save the words which were user-provided in the sense that they are complete words in the puzzle.
  std::vector<std::string> input_words = puzzle.AllWords();
  std::vector<std::string> user_words = puzzle.AllWords();
  for(const string& word : input_words) {
    if(std::find(word.begin(), word.end(), ' ') != word.end()) continue;
    user_words.push_back(word);
  }

  depth_first_stack.push({puzzle});
  while(!depth_first_stack.empty()) {
    //std::cout << "entering dfs, stack size is " << depth_first_stack.size() << std::endl;
    FillSearchState search_state = depth_first_stack.top();
    depth_first_stack.pop();
    if(search_state.which_ == 0) {
      // This is the first time we've seen this state so we need to populate the rest of the state.
      // TODO: just make this take a search state.
      search_state.result_ = DoConfig(search_state.puzzle_, word_finder, &search_state.next_word_coords_, &search_state.direction_, &search_state.matches_);
      //std::cout << "in dfs, there are " << matches.size() << " matches " << std::endl;
      if(search_state.result_ == DoConfigResult::COMPLETE){
        // The very last fill will not have had its cross-words checked by a subsequent call to DoConfig, so check all words.
	// TODO: this is inelegant.
	bool all_words_good = true;
	for(const auto& word : search_state.puzzle_.AllWords()) {
	  // Check that it's valid, either by virtue of being a user word, or in the dictionary
	  if(std::find(user_words.begin(), user_words.end(), word) != user_words.end()) continue;
	  if(word_finder->LazyNumberOfMatches(1, word) > 0) continue;
	  all_words_good = false;
	  break;
	}
	if(all_words_good) return search_state.puzzle_.Data();
	// Some word was not good, which is the same case as IMPOSSIBLE.
	search_state.result_ = DoConfigResult::IMPOSSIBLE;
      }
      // Before we push the next search, push on this one.
    }
    if(search_state.which_ + 1 < search_state.matches_.size()) {
      //std::cout << "pushing back next-in-same-config" << std::endl;
      auto new_search_state = search_state;
      ++new_search_state.which_;
      depth_first_stack.push(new_search_state);
    }
    if(search_state.result_ == DoConfigResult::IMPOSSIBLE) {
      // Got into a configuration that's impossible; we're done; don't push back children
      continue;
    }
    const string& match = search_state.matches_[search_state.which_];
    search_state.puzzle_.SetWord(search_state.next_word_coords_, search_state.direction_, match);
    // Depth-first our way down now.

    //std::cout << "pushing back depth config" << std::endl;
    depth_first_stack.push({search_state.puzzle_});
  }
  return ""; // No solution found.
}
