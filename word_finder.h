#ifndef WORD_FINDER_H
#define WORD_FINDER_H

#include <string>
#include <vector>

class WordFinder {
 public:
  // If the number of matches to the pattern is less than k, return the number of matches. Otherwise, return k.
  virtual int LazyNumberOfMatches(int k, const std::string& pattern) const = 0;

  // Return a vector of all matches.
  virtual void FillMatches(std::vector<std::string>* matches, const std::string& pattern) const = 0;
};

// Backed by a set of vectors.
class VectorWordFinder : public WordFinder {
 public:
  VectorWordFinder(const std::vector<std::string>& dictionary); 
  int LazyNumberOfMatches(int k, const std::string& pattern) const override;
  void FillMatches(std::vector<std::string>* matches, const std::string& pattern) const override;
 private:
  std::vector<std::vector<std::string>> dictionaries_;
};

#endif

