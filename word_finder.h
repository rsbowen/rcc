#ifndef WORD_FINDER_H
#define WORD_FINDER_H

#include <string>
#include <vector>

#include "trie.h"

class WordFinder {
 public:
  // If the number of matches to the pattern is less than k, return the number of matches. Otherwise, return k.
  virtual int LazyNumberOfMatches(int k, const std::string& pattern) const = 0;

  // Return a vector of all matches. TODO: swap argument order.
  virtual void FillMatches(std::vector<std::string>* matches, const std::string& pattern) const = 0;

  // Check a particular word.
  virtual bool LookUp (const std::string& word) const = 0; 
};

// Backed by a set of vectors.
class VectorWordFinder : public WordFinder {
 public:
  VectorWordFinder(const std::vector<std::string>& dictionary); 
  int LazyNumberOfMatches(int k, const std::string& pattern) const override;
  void FillMatches(std::vector<std::string>* matches, const std::string& pattern) const override;
  bool LookUp (const std::string& word) const override; 
 private:
  std::vector<std::vector<std::string>> dictionaries_;
};

// Backed by custom trie implementation.
class TrieWordFinder : public WordFinder {
 public:
  TrieWordFinder(const std::vector<std::string>& dictionary); 
  int LazyNumberOfMatches(int k, const std::string& pattern) const override;
  void FillMatches(std::vector<std::string>* matches, const std::string& pattern) const override;
  bool LookUp (const std::string& word) const override; 
 private:
  Trie trie_;
};

#endif

