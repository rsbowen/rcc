#include "trie.h"

Trie::Trie() : root_(new TrieNode) {
}

void Upper(char* c) {
  if(*c >= 'a' && *c <= 'z') {
    *c = 'A' + (*c - 'a');
  }
}

void Trie::AddWord(const std::string& s) {
  TrieNode* node = root_.get();
  for(char c : s) {
    Upper(&c);
    auto iter = node->children.find(c);
    if(iter != node->children.end()) {
      node = iter->second.get();
      continue;
    }
    std::unique_ptr<TrieNode> new_node(new TrieNode);
    node->children[c] = std::move(new_node);
    node = node->children[c].get();
  }
  node->is_word_end = true;
}

bool Trie::CheckWord(const std::string& s) const {
  TrieNode* node = root_.get();
  for(char c : s) {
    Upper(&c);
    auto iter = node->children.find(c);
    if(iter == node->children.end()) {
      return false;
    }
    node = iter->second.get();
  }
  return node->is_word_end;
 }

TrieNode* ChildOrNull(TrieNode* node, char c) {
  Upper(&c);
  auto iter = node->children.find(c);
  if(iter == node->children.end()) return nullptr;
  return iter->second.get();
}

 void MatchesHelper(const std::string& pattern, const std::string& prefix, std::vector<std::string>* matches, TrieNode* node) {
   if(pattern.empty()) {
     if(node->is_word_end) {
       matches->push_back(prefix);
     }
     return;
   }

   char c = pattern[0];
   const std::string rest = pattern.substr(1,pattern.size());

   if(c==' ') {
     for(char f = 'A'; f<='Z'; ++f) {
       TrieNode* child = ChildOrNull(node, f);
       if(child) MatchesHelper(rest, prefix+f, matches, child);
     }
     return;
   }

   TrieNode* child = ChildOrNull(node, c);
   if(child) MatchesHelper(rest, prefix+c, matches, child);
 }

 void Trie::Matches(const std::string& pattern, std::vector<std::string>* matches) const {
   return MatchesHelper(pattern, "", matches, root_.get());
 }

void LazyMatchesHelper(int k, const std::string& pattern, TrieNode* node, int* total)
{
  if(*total >= k) return;

  if(pattern.empty()) {
    if(node->is_word_end) {
      (*total)++;
    }
    return;
  }
  char c = pattern[0];
  const std::string rest = pattern.substr(1,pattern.size());

  if(c==' ') {
    for(char f = 'A'; f<='Z'; ++f) {
      TrieNode* child = ChildOrNull(node, f);
      if(child) LazyMatchesHelper(k, rest, child, total);
    }
    return;
  }

  TrieNode* child = ChildOrNull(node, c);
  if(child) LazyMatchesHelper(k, rest, child, total);
}

 
int Trie::LazyNumberOfMatches(int k, const std::string& pattern) const {
  int total = 0;
  LazyMatchesHelper(k, pattern, root_.get(), &total);
  return total;
}
