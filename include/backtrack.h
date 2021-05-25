/**
 * @file backtrack.h
 *
 */

#ifndef BACKTRACK_H_
#define BACKTRACK_H_

#include "candidate_set.h"
#include "common.h"
#include "graph.h"
#include <map>
#include <unordered_set>
#include <queue>
#include "MapAndSet.h"
#include <unordered_set>
#include <queue>
#include <algorithm>


class Backtrack {
 public:
  Backtrack();
  ~Backtrack();
  void PrintAllMatches(const Graph &data, const Graph &query, const CandidateSet &cs);
  void backTrack(const Graph &data, const Graph &query, const CandidateSet &cs, MapAndSet &partialEmbeddingM);

  std::map<Vertex, std::vector<Vertex>> findCandidate(const Graph &data, const Graph &query, const CandidateSet &cs, MapAndSet partialEmbedding);
  std::map<Vertex, std::vector<Vertex>> findMinCandidate(const Graph &data, const Graph &query, const CandidateSet &cs, MapAndSet partialEmbedding);

  std::vector<Vertex> Backtrack::getConnectedVertices(const std::set<Vertex> &toFindSet, const std::set<Vertex> &fromFindSet, const Graph &graph);
  Vertex Backtrack::getNextTopologicElem(std::vector<Vertex> &S, const Graph &query, const CandidateSet &cs);
  std::set<Vertex> Backtrack::getAllVertices(const Graph &query){


  Vertex getMinCandidateVertex(const Graph &graph, const CandidateSet &cs);
  std::vector<Vertex> getTopologicVector(const Graph &graph, const CandidateSet &cs);
  std::vector<Vertex> getNeighborList(const Graph &graph, Vertex index);
  std::vector<Vertex> getChildList(const Graph &graph, Vertex index);
  std::vector<Vertex> getParentList(const Graph &graph, Vertex index);
  std::vector<Vertex> getAllCandidate(const CandidateSet &cs, Vertex queryVertex);
  std::vector<Vertex> modifyExtendable(const Graph &graph ,std::vector<Vertex> extendableQueryNodes, std::map<Vertex, Vertex> partialEmbedding);




  std::map<Vertex, std::map<Vertex, unsigned int>> buildWeightCS(const Graph &data, const Graph &query, const CandidateSet &cs);
  static bool cmp(std::pair<Vertex, unsigned int>& w1, std::pair<Vertex, unsigned int>& w2);


  std::map<Vertex, std::map<Vertex, unsigned int>> weight;
  std::unordered_set<Vertex> visitedSet;
  std::map<Vertex, std::vector<Vertex>> extendableMap;

};

#endif  // BACKTRACK_H_
