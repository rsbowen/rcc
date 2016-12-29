#include "word_finder.h"
#include <iostream>

using std::string;
using std::vector;

void TestWordFinder(WordFinder* finder) {
  for(int i = 0; i<5; ++i) {
    int expected_outcome = i<2?i:2;
    int actual_outcome = finder->LazyNumberOfMatches(i, "    a");
    if(expected_outcome != actual_outcome) {
      std::cout << expected_outcome << "," << actual_outcome;
    }
  }

  vector<string> things;
  finder->FillMatches(&things, "b   ");
  if(things.size() != 2) std::cout << "things problem" << std::endl;

  if(!finder->LookUp("alpha")) std::cout << "doesn't lookup alpha" << std::endl;
  if(finder->LookUp("delta")) std::cout << "lookups delta" << std::endl;
}
int main(int argc, char* argv[]) {
  const vector<string> words({"alpha", "beta", "gamma", "bouy"});
  VectorWordFinder vwf(words);
  TrieWordFinder twf(words);

  std::cout << "vector..." << std::endl;
  TestWordFinder(&vwf);
  std::cout << "trie..." << std::endl;
  TestWordFinder(&twf);

  std::cout << "Done testing.";

  return 0;
}
