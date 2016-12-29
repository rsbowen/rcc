// TODO: get a proper testing library

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include "trie.h"

int main(int argc, char* argv[]) {
  Trie trie;

  std::cout << "adding" << std::endl;
  trie.AddWord("alpha");
  trie.AddWord("alphabet");
  trie.AddWord("betamax");
  trie.AddWord("beta");
  trie.AddWord("data");

  std::cout << "checking" << std::endl;
  if(!trie.CheckWord("alpha")) std::cout << "alpha" << std::endl;
  if(!trie.CheckWord("alphabet")) std::cout << "alphabet" << std::endl;
  if(!trie.CheckWord("betamax")) std::cout << "betamax" << std::endl;
  if(!trie.CheckWord("beta")) std::cout << "beta" << std::endl;

  if(trie.CheckWord("gamma")) std::cout << "gamma" << std::endl;
  if(trie.CheckWord("alp")) std::cout << "alp" << std::endl;
  if(trie.CheckWord("alps")) std::cout << "alps" << std::endl;

  std::vector<std::string> matches;
  trie.Matches("  ta", &matches);
  if(matches.size() != 2) {
    std::cout << "bad matches size: " << matches.size() << std::endl;
  }
  std::sort(matches.begin(), matches.end());
  if(matches[0] != "beta") std::cout << "matches[0] neq beta " << matches[0] << std::endl;
  if(matches[1] != "data") std::cout << "matches[1] neq data " << matches[1] << std::endl;

  std::cout << "Done testing" << std::endl;
}
