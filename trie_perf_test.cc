#include "word_finder.h"
#include <stdlib.h>
#include <unordered_set>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>

using Clock = std::chrono::high_resolution_clock;

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

  std::vector<std::string> words_dict(words.begin(), words.end());

  TrieWordFinder finder(words_dict);
  std::cout << "done reading" << std::endl;

  const auto time_1 = Clock::now();
  // Do 10000 random lookups.
  const int nLookups = 10000;
  bool x;
  for(int i = 0; i< nLookups; ++i) {
    x ^= finder.LookUp(words_dict[rand() % words_dict.size()]);
  }

  const auto time_2 = Clock::now();
  // Do 10000 random matches.
  int y = 0;
  const int nMatches = 10000;
  std::vector<std::string> matches;
  for(int i=0;i<nMatches; ++i) {
    matches.clear();
    std::string word = words_dict[rand() % words_dict.size()];
    int a = 0;
    for(char& c : word) {
      if((++a)%2) c = ' ';
    }
    finder.FillMatches(&matches, word);
  }
  
  const auto time_3 = Clock::now();
  std::chrono::duration<double> elapsed_seconds_1 = time_2 - time_1;
  std::cout << "done lookup time:" << elapsed_seconds_1.count() << std::endl;

  std::chrono::duration<double> elapsed_seconds_2 = time_3 - time_2;
  std::cout << "done match time:" << elapsed_seconds_2.count() << std::endl;
}
