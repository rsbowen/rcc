#include "trie.h"

Trie::Trie() : root_(new TrieNode) {
}

// TODO: maybe can reduce branching etc. by making a canonical representation and sanitizing input at hte Finder level.
void Upper(char* c) {
  if(*c >= 'a' && *c <= 'z') {
    *c = 'A' + (*c - 'a');
  }
}


TrieNode* TrieNode::ChildOrNull(char c) {
  Upper(&c);
  return children[c-'A'].get();
}

TrieNode* TrieNode::Child(char c) {
  Upper(&c);
  TrieNode* ptr = children[c-'A'].get();
  if(ptr==nullptr) {
    std::unique_ptr<TrieNode> new_node(new TrieNode);
    children[c-'A'] = std::move(new_node);
  }
  return children[c-'A'].get();
}

void Trie::AddWord(const std::string& s) {
  TrieNode* node = root_.get();
  for(char c : s) {
    node = node->Child(c);
  }
  node->is_word_end = true;
}

bool Trie::CheckWord(const std::string& s) const {
  TrieNode* node = root_.get();
  for(char c : s) {
    node = node->ChildOrNull(c);
    if(!node) return false;
  }
  return node->is_word_end;
 }

 void MatchesHelper(const std::string& pattern, int pattern_idx, std::string* prefix, std::vector<std::string>* matches, TrieNode* node) {
   if(pattern_idx == pattern.size()) {
     if(node->is_word_end) {
       matches->push_back(*prefix);
     }
     return;
   }

   char c = pattern[pattern_idx];
   Upper(&c);

   if(c==' ') {
     for(char f = 'A'; f<='Z'; ++f) {
       TrieNode* child = node->ChildOrNull(f);
       (*prefix)[pattern_idx] = f;
       if(child) MatchesHelper(pattern, pattern_idx+1, prefix, matches, child);
     }
     return;
   }

   TrieNode* child = node->ChildOrNull(c);
   (*prefix)[pattern_idx] = c;
   if(child) MatchesHelper(pattern, pattern_idx+1, prefix, matches, child);
 }

 void Trie::Matches(const std::string& pattern, std::vector<std::string>* matches) const {
   std::string prefix(pattern.size(), ' ');
   return MatchesHelper(pattern, 0, &prefix, matches, root_.get());
 }

void LazyMatchesHelper(int k, const std::string& pattern, int pattern_idx, TrieNode* node, int* total)
{
  if(*total >= k) return;

  if(pattern.size() == pattern_idx) {
    if(node->is_word_end) {
      (*total)++;
    }
    return;
  }
  char c = pattern[pattern_idx];

  if(c==' ') {
    for(char f = 'A'; f<='Z'; ++f) {
      TrieNode* child = node->ChildOrNull(f);
      if(child) LazyMatchesHelper(k, pattern, pattern_idx+1, child, total);
    }
    return;
  }

  TrieNode* child = node->ChildOrNull(c);
  if(child) LazyMatchesHelper(k, pattern, pattern_idx+1, child, total);
}

 
int Trie::LazyNumberOfMatches(int k, const std::string& pattern) const {
  int total = 0;
  LazyMatchesHelper(k, pattern, 0, root_.get(), &total);
  return total;
}
