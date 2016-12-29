#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

#include "puzzle.h"
#include "fill.h"
#include "word_finder.h"
#include<string>
#include<iostream>
#include<unordered_set>

int main(int argc, char* argv[]) {
  std::string line;
  std::string puzzle;

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
  std::vector<std::string> dict_vec(words.begin(), words.end());
  TrieWordFinder finder(dict_vec);

  std::ifstream myfile("puzzle.rcc");
  if(myfile.is_open()) {
  while(std::getline(myfile, line)) { puzzle.append(line); }
  }

  int side_len = std::sqrt(puzzle.size());

  if(side_len*side_len != puzzle.size()) {
    std::cout << "weird size " << puzzle.size() << std::endl;
    return -1;
  }


  Puzzle p(puzzle, side_len);
  std::cout << p.PrettyString() << std::endl;

  std::string biggest;
  std::string filled = Fill(p, &finder, &biggest);
  if(filled.empty()) {
    std::cout << "No soln, biggest " << Puzzle(biggest, side_len).PrettyString() << std::endl;
  } else {
    Puzzle p2(filled, side_len);
    std::cout << p2.PrettyString() << std::endl;
    PuzzleIterator pi(p2);
    while(pi.Valid()) {
      auto word = pi.Get();
      std::cout << word << p2.WordAt(word.coords_, word.direction_) << std::endl;
      pi.Increment();
    }
  }
}
