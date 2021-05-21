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
#include "MapAndSet.h"


class Backtrack {
 public:
  Backtrack();
  ~Backtrack();
  void PrintAllMatches(const Graph &data, const Graph &query, const CandidateSet &cs);
  void backTrack(const Graph &data, const Graph &query, const CandidateSet &cs, MapAndSet partialEmbeddingM);

  std::map<Vertex, std::vector<Vertex>> findCandidate(const Graph &data, const Graph &query, const CandidateSet &cs, MapAndSet partialEmbedding);

  std::vector<Vertex> getChildList(const Graph &graph, Vertex index);
  std::vector<Vertex> getParentList(const Graph &graph, Vertex index);
  std::vector<Vertex> getAllCandidate(const CandidateSet &cs, Vertex queryVertex);
  std::vector<Vertex> modifyExtendable(const Graph &graph ,std::vector<Vertex> extendableQueryNodes, std::map<Vertex, Vertex> partialEmbedding);

  std::map<Vertex, std::map<Vertex, unsigned int>> buildWeightCS(const Graph &data, const Graph &query, const CandidateSet &cs);


  std::map<Vertex, std::map<Vertex, unsigned int>> weight;
  std::set<Vertex> visitedSet;
  std::map<Vertex, std::vector<Vertex>> extendableMap;

};

#endif  // BACKTRACK_H_
