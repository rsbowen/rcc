#include "word_finder.h"

using std::vector;
using std::string;

// TODO: sort the dictionary by word length
VectorWordFinder::VectorWordFinder(const vector<string>& dictionary) : dictionary_(dictionary) {}

int VectorWordFinder::LazyNumberOfMatches(int k, const string& pattern) const {
  int matches_so_far = 0;
  for(const string& word : dictionary_) {
    if (matches_so_far==k) return k;
    if(word.size() != pattern.size()) continue;
    bool match = true;
    for(int i = 0; i<word.size(); ++i) {
      if(word[i] != pattern[i] && pattern[i] != ' ') {
        match = false;
	break;
      }
    }
    if(match) ++matches_so_far;
  }
  return matches_so_far;
}

void VectorWordFinder::FillMatches(vector<string>* matches, const string& pattern) const {
  matches->clear();
  for(const string& word : dictionary_) {
    if(word.size() != pattern.size()) continue;
    bool match = true;
    for(int i = 0; i<word.size(); ++i) {
      if(word[i] != pattern[i] && pattern[i] != ' ') {
        match = false;
	break;
      }
    }
    if(match) matches->push_back(word);
  }
}

