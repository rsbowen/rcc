#include "graph.h"
#include "puzzle.h"
#include <iostream>
#include <algorithm>
#include <string>

int main(int argc, char* argv[]) {
  // Graph with 3 connected components
  // a b-c d-e-f
  Word a({0,0}, Direction::ACROSS);
  Word b({1,0}, Direction::ACROSS);
  Word c({2,0}, Direction::ACROSS);
  Word d({3,0}, Direction::ACROSS);
  Word e({4,0}, Direction::ACROSS);
  Word f({5,0}, Direction::ACROSS);

  PuzzleGraph puzzle_graph;
  puzzle_graph[a] = {};
  puzzle_graph[b] = {c};
  puzzle_graph[c] = {b};
  puzzle_graph[d] = {e,f};
  puzzle_graph[e] = {d,f};
  puzzle_graph[f] = {d,e};
  
  auto cc = ConnectedComponents(puzzle_graph);
  // Canonicalize because order is not guaranteed.
  std::string components_as_string;
  // This could fail if the test graph had two components of the same size!
  std::sort(cc.begin(), cc.end(), [] (const std::vector<Word>& a, const std::vector<Word>& b) {return a.size() < b.size();});
  for (auto& component: cc) {
    // This could fail if the test graph had different vertices with the same row coords!
    std::sort(component.begin(), component.end(), [] (const Word& a, const Word& b) {return a.coords_.first < b.coords_.first;});
    for (const auto& word : component) {
      components_as_string.push_back('0'+word.coords_.first);
    }
  components_as_string.push_back(',');
  }
  if(components_as_string != "0,12,345,") std::cout << "bad components string " << components_as_string;

  return 0;
}
