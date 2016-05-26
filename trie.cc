#include "trie.h"

TrieNode::TrieNode() : word_end_(false) {}
// TODO: bounds checking on x
TrieNode& TrieNode::Child(char x) {
   if(children_.empty()) {
     children_.resize(26); // TODO: this makes 26 empty vectors!
   }
   return children_[x-'a'];
 }

void TrieNode::AddWord(const std::string& s, int idx) {
   if(idx == s.size()) word_end_ = true;
   else Child(s[idx]).AddWord(s, idx+1);
 }

bool TrieNode::CheckWord(const std::string& s, int idx) {
   if(idx == s.size()) return word_end_;
   else if (children_.empty()) return false;
   return Child(s[idx]).CheckWord(s, idx+1);
 }
