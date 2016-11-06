#include "word_finder.h"
#include <iostream>

using std::string;
using std::vector;

int main(int argc, char* argv[]) {
  VectorWordFinder vwf({"alpha", "beta", "gamma", "bouy"});
  for(int i = 0; i<5; ++i) {
    int expected_outcome = i<2?i:2;
    int actual_outcome = vwf.LazyNumberOfMatches(i, "    a");
    if(expected_outcome != actual_outcome) {
      std::cout << expected_outcome << "," << actual_outcome;
    }
  }

  vector<string> things;
  vwf.FillMatches(&things, "b   ");
  if(things.size() != 2) std::cout << "things problem";

  if(!vwf.LookUp("alpha")) std::cout << "doesn't lookup alpha";
  if(!vwf.LookUp("delta")) std::cout << "lookups delta";

  std::cout << "Done testing.";

  return 0;
}
