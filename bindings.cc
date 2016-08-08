#include <emscripten/bind.h>
#include <string>
#include <vector>
#include "puzzle.h"
#include "fill.h"
#include "word_finder.h"

using namespace emscripten;
using std::string;

// Use a hard-coded dictionary so we can fill this
/*
ALPHA
MELON
ITALY
AGILE
SONAR
*/

string fill_puzzle(string p)
{
  VectorWordFinder finder({"ALPHA", "MELON", "ITALY", "AGILE", "SONAR", "AMIAS", "LETGO", "PLAIN", "HOLLA", "ANYER"});
  Puzzle puzzle(p, 5);
  return Fill(puzzle, &finder);
}

EMSCRIPTEN_BINDINGS(rcc_module) {
  function("fill_puzzle", &fill_puzzle);
}
