// TODO: get a proper testing library

#include <iostream>
#include "trie.h"

int main(int argc, char* argv[]) {
  TrieNode root;
  root.AddWord("alpha");
  root.AddWord("alphabet");
  root.AddWord("betamax");
  root.AddWord("beta");

  if(!root.CheckWord("alpha")) std::cout << "alpha" << std::endl;
  if(!root.CheckWord("alphabet")) std::cout << "alphabet" << std::endl;
  if(!root.CheckWord("betamax")) std::cout << "betamax" << std::endl;
  if(!root.CheckWord("beta")) std::cout << "beta" << std::endl;

  if(root.CheckWord("gamma")) std::cout << "gamma" << std::endl;
  if(root.CheckWord("alp")) std::cout << "alp" << std::endl;
  if(root.CheckWord("alps")) std::cout << "alps" << std::endl;

  std::cout << "Done testing" << std::endl;
}
