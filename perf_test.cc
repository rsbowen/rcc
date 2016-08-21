#include <unordered_set>
#include <string>
#include <iostream>
#include <fstream>

#include "fill.h"
#include "word_finder.h"

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

  VectorWordFinder vwf({words.begin(), words.end()});
  std::cout << "done reading" << std::endl;
/*
  // Empty n-by-n puzzle
  const size_t n=13;
  const std::string puzzle_template(n*n, ' ');
  Puzzle puzzle(puzzle_template , n);
*/

  Puzzle puzzle("FAB# C   #DAGAIR# A   #OBOALI# K   #YES##D  E  # O  PRESS##   U##  A #TWELFTHS  N#  E  #A    D   D # K    G   ##  E    R #     T##  O#HONOR#H    O#     #IDD  M#     #S  ", 13);
  std::cout << "Puzzle is " << puzzle.PrettyString();

  // Betcha this is real slow :)
  std::cout << "solution '" << Fill(puzzle, &vwf) << "'" << std::endl;
}
