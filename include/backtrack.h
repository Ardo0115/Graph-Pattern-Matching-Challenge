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

  std::map<Vertex, std::vector<Vertex>> findCandidate(const Graph &data, const Graph &query, const CandidateSet &cs, const MapAndSet& partialEmbedding);

  std::vector<Vertex> getChildList(const Graph &graph, Vertex index);
  std::vector<Vertex> getParentList(const Graph &graph, Vertex index);
  std::vector<Vertex> getAllCandidate(const CandidateSet &cs, Vertex queryVertex);
  void modifyExtendable(const Graph &graph ,std::vector<Vertex>& extendableQueryNodes, const std::map<Vertex, Vertex>& newPartialEmbedding);

  std::map<Vertex, std::map<Vertex, unsigned int>> buildWeightCS(const Graph &data, const Graph &query, const CandidateSet &cs);
  static bool cmp(std::pair<Vertex, unsigned int>& w1, std::pair<Vertex, unsigned int>& w2);


  std::map<Vertex, std::map<Vertex, unsigned int>> weight;
  std::set<Vertex> visitedSet;


};

#endif  // BACKTRACK_H_
