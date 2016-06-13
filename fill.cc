#include <algorithm>
#include <limits>
#include <stack>
#include <string>
#include <vector>

#include "word_finder.h"

using std::string;
using std::vector;
// Helpers, probably will be moved out at some point.

// TODO: combine these with a flag for across vs. down
std::pair<int, int> AcrossWordBounds(const string& puzzle, int r, int c, int puzzle_size) {
  // TODO Sanity checking here: word at (r,c) is not a '#', r and c are inside bounds.
  int start_col = c;
  int end_col = c;
  while( start_col > 0 && puzzle[r*puzzle_size + start_col] != '#' ) --start_col;
  while( end_col <= puzzle_size && puzzle[r*puzzle_size + end_col] != '#' ) ++end_col;
  return {start_col, end_col};
}

std::pair<int, int> DownWordBounds(const string& puzzle, int r, int c, int puzzle_size) {
  // TODO Sanity checking here: word at (r,c) is not a '#', r and c are inside bounds.
  int start_row = c;
  int end_row = c;
  while( start_row > 0 && puzzle[start_row*puzzle_size + c] != '#' ) --start_row;
  while( end_row <= puzzle_size && puzzle[end_row*puzzle_size + c] != '#' ) ++end_row;
  return {start_row, end_row};
}

// TODO: change all these bool params to an enum
string WordAt(const string& puzzle, int puzzle_size, int row, int col, int length, bool is_down) {
  string out = "";
  out.resize(length, ' ');
  for(int i = 0; i<length; ++i) {
    int row_here = row + is_down?length:0;
    int col_here = col + is_down?0:length;
    // TODO: probably put a dumb wrapper on puzzle which stores as a string but does this function and also the indexing below
    out[i] = puzzle[row_here*puzzle_size + col_here];
  }
  return out;
}

// TODO: better naming: work out which one is smallest, better doc
// is_done: this configuration is already filled in all the way
// row, col: the place the word under consideration will start
// matches: if empty, there is an unmatchable thing or a bad word here
// TODO: can probably at each depth level in the stack just only look at the words that touch the cell we just changed, which would save a lot of time.
// TODO(don't call both 'configuration' and 'puzzle'. Probably will be solved by a wrapper class instead of passing strings and size around everywhere.
void DoConfig(string configuration, const int puzzle_size, const WordFinder* word_finder, int* next_word_row, int* next_word_col, bool* pattern_is_down, bool* is_done, vector<string>* matches)
{
  matches->clear();
  int min_num_matches_seen = std::numeric_limits<int>::max();
  *is_done = true;
  string pattern_for_min_matches;
  for(int row = 0; row < puzzle_size; ++row) {
    for(int col = 0; col < puzzle_size; ++col) {
      if(configuration[(row)*puzzle_size + col] == '#') continue;
      // TODO: more elegant here
      for(int down_or_across = 0; down_or_across < 2; ++down_or_across) {
        bool is_down = (down_or_across == 0);
	bool is_word_start = false;
	if (is_down && row == 0) is_word_start = true;
	if (!is_down && col == 0) is_word_start = true;
	if (is_down && configuration[(row-1)*puzzle_size+col] == '#') is_word_start = true;
	if (!is_down && configuration[row*puzzle_size+col - 1] == '#') is_word_start = true;
	if(!is_word_start) continue;
        // Check the down or across word here. TODO: rationalize WordBounds w.r.t. a bool is_down param
        std::pair<int, int> indices;
	if(is_down) indices = DownWordBounds(configuration, row, col, puzzle_size);
	else indices = AcrossWordBounds(configuration, row, col, puzzle_size);
        int length = indices.second-indices.first+1;
        string pattern = WordAt(configuration, puzzle_size, row, col, length, is_down);
        int num_matches = word_finder->LazyNumberOfMatches(min_num_matches_seen, pattern);
        if(num_matches == 0) {
          // Bad! Matches is already empty so just return.
          return;
        }
        // If this is a complete word then nothing to do. 
	if(std::find(pattern.begin(), pattern.end(), ' ') == pattern.end()) continue;
	//If this is not the new shortest then nothing to do. 
	if(num_matches >= min_num_matches_seen) continue;
	pattern_for_min_matches = pattern;
	*pattern_is_down = is_down;
	*next_word_row = row;
	*next_word_col = col;
      }
    } 
  }
  if(*is_done) return;
  // Fill in *matches.
  word_finder->FillMatches(matches, pattern_for_min_matches);
}

// Extremely naive approach to filling: a depth first search with pruning.
// Maintain a stack of partially-filled puzzles and indices.
// For each puzzle, we will search over all the possibilities for the one with the smallest number of possibilities. Recompute that every time (yikes, maybe can save memory here).

string Fill(const string& puzzle, const WordFinder* word_finder, const int puzzle_size) {
  std::stack<std::pair<std::string, int>> depth_first_stack;
  depth_first_stack.push({puzzle, 0});
  while(!depth_first_stack.empty()) {
    std::pair<std::string, int> configuration = depth_first_stack.top();
    depth_first_stack.pop();
    int next_word_row, next_word_col;
    bool pattern_is_down, is_done;
    vector<string> matches;
    DoConfig(configuration.first, puzzle_size, word_finder, &next_word_row, &next_word_col, &pattern_is_down, &is_done, &matches);
    if(is_done) return configuration.first;
    if(matches.empty()) {
      // Got into a configuration that's impossible; we're done.
      continue;
    }
    // Before we push the next search, push on this one.
    if(matches.size()-1 < configuration.second) {
      depth_first_stack.push({configuration.first, configuration.second+1});
    }
    const string& match = matches[configuration.second];
    for(int i = 0; i<match.length(); ++i) {
      int row_to_fill = next_word_row+(pattern_is_down?i:0);
      int col_to_fill = next_word_col+(pattern_is_down?0:i);
      configuration.first[row_to_fill*puzzle_size + col_to_fill] = match[i];
    }
    // Depth-first our way down here.
    depth_first_stack.push({configuration.first, 0});
  }
  return ""; // No solution found.
}
