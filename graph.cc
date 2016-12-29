#include <algorithm>
#include <set>
#include <limits>
#include <string>

#include "graph.h"

// tmp
#include <iostream>

std::vector<std::vector<Word>> ConnectedComponents(const PuzzleGraph& graph) {
  std::vector<std::vector<Word>> connected_components;
  std::unordered_set<Word> visited_vertices;
  for(const auto& vertex_and_neighbors: graph) {
    const auto& vertex = vertex_and_neighbors.first;
    const auto& neighbors = vertex_and_neighbors.second;
    if(visited_vertices.count(vertex) != 0) continue;
    connected_components.emplace_back();
    connected_components.back().push_back(vertex);
    visited_vertices.insert(vertex);
    std::vector<Word> vertices_to_visit = neighbors;
    while(!vertices_to_visit.empty()) {
      const Word to_visit = vertices_to_visit.back();
      vertices_to_visit.pop_back();
      if(visited_vertices.count(to_visit) != 0) continue;
      visited_vertices.insert(to_visit);
      connected_components.back().push_back(to_visit);
      // TODO: probably a single standard library call can do this
      for(const auto& neighbor : graph.at(to_visit)) {
        vertices_to_visit.push_back(neighbor);
      }
    }
  }
  return connected_components;
}

// TODO: break this into 3 functions which can be tested individually
std::vector<FillOrderEntry> FillOrder(Puzzle puzzle) {
  std::vector<FillOrderEntry> fill_order;
  // Fill each component with a single letter to make it easier to read as a human. This variable controls which to use next.
  int alphabet_offset = 0;
  for(PuzzleGraph graph = puzzle.AsGraph(); !graph.empty(); graph=puzzle.AsGraph(), ++alphabet_offset) {
    // Temp. TODO: maybe control this somehow, only print when debug mode or not opt mode or something.
    std::unordered_set<Word> word_set;
    fill_order.emplace_back();
    const auto connected_components = ConnectedComponents(graph);
    int min_size = std::numeric_limits<int>::max(), index;
    for(int i = 0; i<connected_components.size(); ++i) {
      if(connected_components[i].size() < min_size) {
        min_size = connected_components[i].size();
	index = i;
      }
    }
    // If any connected component has size less than 6, just fill it.
    if(min_size <= 6) {
      fill_order.back().is_cc = true;
      //std::cout << "Small-components rule" << std::endl;
      // TODO: this can cause some extra work because it may fill in words that are already filled in (the last word in the cc, for example, is probably filled in by cross-words by the time we get to it); fill.cc will notice though
      for(const auto& word : connected_components[index]) {
        // TODO: make puzzle take Word arguments
        std::string word_already = puzzle.WordAt(word.coords_, word.direction_);
	for(char& c : word_already) c=(char)('a'+(alphabet_offset%26));
	puzzle.SetWord(word.coords_, word.direction_, word_already);
	fill_order.back().words.push_back(word);
      }
      continue;
    }

    // See if there's a word which, if filled, will break into more connected components.
    int max_num_connected_components = connected_components.size(), connected_components_score = std::numeric_limits<int>::max();
    Word location;
    
    for(PuzzleIterator iter(puzzle); iter.Valid(); iter.Increment()) {
      // TODO: this is probably wildly inefficient, can be smarter about how tracking connected components to not redo this calculation every time.
      Puzzle sub_puzzle = puzzle; 
      // TODO: change Word to something else or make it so you can use it like a string (yikes...)
      Word word = iter.Get();
      std::string word_already = sub_puzzle.WordAt(word.coords_, word.direction_);
      // If this word doesn't have any blanks, nothing to fill, move on.
      if(std::find(word_already.begin(), word_already.end(), ' ') == word_already.end()) continue;
      for(char& c : word_already) c= 'x';
      sub_puzzle.SetWord(word.coords_, word.direction_, word_already);
      auto new_connected_components = ConnectedComponents(sub_puzzle.AsGraph());
      if(new_connected_components.size() > max_num_connected_components) {
        int sub_cc_score = 0;
	for(const auto& cc : new_connected_components) { sub_cc_score += (cc.size())*(cc.size()); }
	if(sub_cc_score < connected_components_score) {
	  max_num_connected_components = new_connected_components.size();
	  connected_components_score = sub_cc_score;
	  location = word;
	}
      }
    }
    // Found a breaker! Fill it in.
    fill_order.back().is_cc = false;
    if(max_num_connected_components > connected_components.size()) {
      //std::cout << "cc rule: new max is " << max_num_connected_components;
      std::string word_already = puzzle.WordAt(location.coords_, location.direction_);
      for(char& c : word_already) c=('a'+(alphabet_offset%26));
      puzzle.SetWord(location.coords_, location.direction_, word_already);
      fill_order.back().words.push_back(location);
      continue;
    }

    // Nothing breaks, no small CCs, fill in the max num blanks.
    //std::cout << "max-num-blanks rule" << std::endl;
    int max_num_blanks = 0;
    for(PuzzleIterator iter(puzzle); iter.Valid(); iter.Increment()) {
      std::string word = puzzle.WordAt(iter.Get().coords_, iter.Get().direction_);
      int blanks = std::count(word.begin(), word.end(), ' ');
      if(blanks > max_num_blanks) { 
        max_num_blanks = blanks;
	location = iter.Get();
      }
    }
    std::string word_already = puzzle.WordAt(location.coords_, location.direction_);
    for(char& c : word_already) c='a'+(alphabet_offset % 26);
    puzzle.SetWord(location.coords_, location.direction_, word_already);
    fill_order.back().is_cc = false;
    fill_order.back().words.push_back(location);
  }
  return fill_order;
}

void FillComponent(Puzzle* puzzle, const std::vector<Word> component, const std::string& dumped_component) {
  auto dumped_ptr = dumped_component.begin();
  for(const auto& word : component) {
    std::string word_from_component = puzzle->WordAt(word.coords_, word.direction_);
    std::copy(dumped_ptr, dumped_ptr+word_from_component.size(), word_from_component.begin());
    dumped_ptr += word_from_component.size();
    puzzle->SetWord(word.coords_, word.direction_, word_from_component);
  }
}

std::string DumpComponent(const Puzzle& puzzle, const std::vector<Word> component) {
  std::string x;
  for(const auto& word : component) {
    x.append(puzzle.WordAt(word.coords_, word.direction_));
  }
  return x;
}

std::string DumpComponentWithCrosswords(const Puzzle& puzzle, const PuzzleGraph& puzzle_graph, const std::vector<Word> component) {
  // TODO: this might be slow in a tight loop; might be good to add up the size of /component/ and pre-allocate x.
  std::string x = DumpComponent(puzzle, component);
  std::set<Word> words_accounted_for(component.begin(), component.end());
  for(const auto& word : component) {
    for(const auto& crossword : puzzle_graph.at(word)) {
      if(words_accounted_for.find(crossword) != words_accounted_for.end()) continue;
      words_accounted_for.insert(crossword);
      x.append(puzzle.WordAt(crossword.coords_, crossword.direction_));
    }
  }
  return x;
}
