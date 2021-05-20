/**
 * @file backtrack.h
 *
 */

#ifndef BACKTRACK_H_
#define BACKTRACK_H_

#include "candidate_set.h"
#include "common.h"
#include "graph.h"
#include "map"


class Backtrack {
 public:
  Backtrack();
  ~Backtrack();
  void PrintAllMatches(const Graph &data, const Graph &query, const CandidateSet &cs);
  void backTrack(const Graph &data, const Graph &query, const CandidateSet &cs);

  Vertex findExtendable(const Graph &query,
                        const CandidateSet &cs);

  std::vector<Vertex> getChildList(const Graph &graph, Vertex index);
  std::vector<Vertex> getParentList(const Graph &graph, Vertex index);
  std::vector<Vertex> modifyExtendable(const Graph &graph ,std::vector<Vertex> extendableQueryNodes);

//  std::vector<std::pair<Vertex, Vertex>> partialEmbedding;
  std::map<Vertex, Vertex> partialEmbedding;
  std::vector<Vertex> extendable;
  std::set<Vertex> visitedSet;
  std::map<Vertex, std::vector<Vertex>> extendableMap;

};

#endif  // BACKTRACK_H_
