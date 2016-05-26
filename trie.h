#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>


class TrieNode {
 public:
  TrieNode();
  TrieNode& Child(char x);
  void AddWord(const std::string& s, int idx = 0);
  bool CheckWord(const std::string& s, int idx = 0); 

 protected:
  bool word_end_;
  std::vector<TrieNode> children_;
};

#endif
