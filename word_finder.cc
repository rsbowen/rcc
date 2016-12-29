#include "word_finder.h"

using std::vector;
using std::string;

VectorWordFinder::VectorWordFinder(const vector<string>& dictionary) {
  for(const auto& word : dictionary) {
    if(word.size() >= dictionaries_.size()) {
      dictionaries_.resize(word.size() + 1);
    }
    dictionaries_[word.size()].push_back(word);
  }
}

int VectorWordFinder::LazyNumberOfMatches(int k, const string& pattern) const {
  int matches_so_far = 0;
  if(pattern.size() >= dictionaries_.size()) return 0;
  for(const string& word : dictionaries_[pattern.size()]) {
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
  if(pattern.size() >= dictionaries_.size()) return;
  for(const string& word : dictionaries_[pattern.size()]) {
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

bool VectorWordFinder::LookUp(const std::string& word) const {
  // TODO: This could be faster.
  if(word.size() >= dictionaries_.size()) return false;
  for(const string& dictionary_word : dictionaries_[word.size()]) {
    if(dictionary_word == word) return true;
  }
  return false;
}

TrieWordFinder::TrieWordFinder(const std::vector<std::string>& dictionary) {
  for(const auto& word : dictionary) {
    trie_.AddWord(word);
  }
}

int TrieWordFinder::LazyNumberOfMatches(int k, const std::string& pattern) const {
  return trie_.LazyNumberOfMatches(k, pattern);
}

void TrieWordFinder::FillMatches(std::vector<std::string>* matches, const std::string& pattern) const {
  trie_.Matches(pattern, matches);
}

bool TrieWordFinder::LookUp(const std::string& word) const {
  return trie_.CheckWord(word);
}
