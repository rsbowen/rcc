#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>
#include <map>
#include <memory>

struct TrieNode {
  // TODO: maybe a fixed-size array
  std::map<char, std::unique_ptr<TrieNode>> children;
  bool is_word_end;
};

class Trie {
  public:
    Trie();
    void AddWord(const std::string& s);
    bool CheckWord(const std::string& s) const; 
    void Matches(const std::string& pattern, std::vector<std::string>* matches) const;
    int LazyNumberOfMatches(int k, const std::string& pattern) const;

  protected:
  std::unique_ptr<TrieNode> root_;
};

#endif
