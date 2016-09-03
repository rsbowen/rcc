#include <unordered_set>
#include <string>

#include "graph.h"

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
