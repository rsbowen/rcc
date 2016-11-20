#include <unordered_set>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>

#include "fill.h"
#include "word_finder.h"
  
using Clock = std::chrono::high_resolution_clock;

// From a partially-completed crossword
void RealWorldTest(const VectorWordFinder& finder) {
  Puzzle puzzle("FAB# C   #DAGAIR# A   #OBOALI# K   #YES##D  E  # O  PRESS##   U##  A #TWELFTHS  N#  E  #A    D   D # K    G   ##  E    R #     T##  O#HONOR#H    O#     #IDD  M#     #S  ", 13);
  std::cout << "Puzzle is " << puzzle.PrettyString();
  const std::string soln = Fill(puzzle, &finder);

  // Betcha this is real slow :)
  if(soln.empty()) {
    std::cout << "no solution " << std::endl;
  } else {
    std::cout << "solution " << Puzzle(soln, 13).PrettyString() << std::endl;
  }
}

void NXNTest(const VectorWordFinder& finder, const int side_length) {
  // Empty n-by-n puzzle
  const std::string puzzle_template(side_length*side_length, ' ');
  Puzzle puzzle(puzzle_template, side_length);
  std::cout << "solution '" << Fill(puzzle, &finder) << "'" << std::endl;
}

int main(int argc, char* argv[]) {
  std::ifstream dictionary_file;
  dictionary_file.open("/usr/share/dict/words");
  std::unordered_set<std::string> words;
  std::string dictionary_word;
  if(!dictionary_file.is_open()) return 1;
  while(getline(dictionary_file, dictionary_word)) {
    std::string word;
    for (const auto& c : dictionary_word) {
      if('a' <= c && c <= 'z') { word.push_back(c-'a'+'A');}
      else if ('A' <= c && c <= 'Z') { word.push_back(c);}
    }
    if(!word.empty()) words.insert(word);
  }

  VectorWordFinder finder({words.begin(), words.end()});
  std::cout << "done reading" << std::endl;
  const auto time_1 = Clock::now();
  NXNTest(finder, 4);
  const auto time_2 = Clock::now();
  std::chrono::duration<double> elapsed_seconds_1 = time_2 - time_1;
  std::cout << "done nxn time(s):" << elapsed_seconds_1.count() << std::endl;
  RealWorldTest(finder);
  const auto time_3 = Clock::now();
  std::chrono::duration<double> elapsed_seconds_2 = time_3 - time_2;
  std::cout << "done rw time(s):" << elapsed_seconds_2.count() << std::endl;

  std::cout << "nxn test " << elapsed_seconds_1.count() << " rw test " << elapsed_seconds_2.count()<<std::endl;
}
