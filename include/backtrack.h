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
#include <unordered_set>
#include <queue>
#include <algorithm>

class Backtrack {
public:
    Backtrack();
    ~Backtrack();
    void PrintAllMatches(const Graph &data, const Graph &query, const CandidateSet &cs);
    void backTrack(const Graph &data, const Graph &query, const CandidateSet &cs, MapAndSet partialEmbeddingM);

    std::map<Vertex, std::vector<Vertex>> findCandidate(const Graph &data, const Graph &query, const CandidateSet &cs, const MapAndSet& partialEmbedding);
    std::vector<Vertex> getConnectedVertices(const std::set<Vertex> &toFindSet, const std::set<Vertex> &fromFindSet, const Graph &graph);
    Vertex getNextTopologicElem(std::vector<Vertex> &S, const Graph &query, const CandidateSet &cs);
    std::set<Vertex> getAllVertices(const Graph &query);

    Vertex getMinLabelVertex(const Graph &graph, const CandidateSet &cs);
    std::vector<Vertex> getTopologicVector(const Graph &graph, const CandidateSet &cs);
    void setChildrenAndParentList(const Graph &query);
    std::vector<Vertex> getNeighborList(const Graph &graph, Vertex index);
    std::vector<Vertex> getChildList(const Graph &query, Vertex index);
    std::vector<Vertex> getParentList(const Graph &query, Vertex index);
    std::vector<Vertex> getAllCandidate(const CandidateSet &cs, Vertex queryVertex);

    int countFurtherOccurrence (const Graph &query, const CandidateSet &cs, const std::vector<Vertex>& unvisitedQueryVertices, Vertex u, Vertex v);

    void modifyExtendable(const Graph &graph ,std::vector<Vertex>& extendableQueryNodes, const std::map<Vertex, Vertex>& newPartialEmbedding);

    std::map<Vertex, std::map<Vertex, unsigned int>> buildWeightCS(const Graph &data, const Graph &query, const CandidateSet &cs);
    static bool cmp(std::pair<Vertex, unsigned int>& w1, std::pair<Vertex, unsigned int>& w2);


    std::map<Vertex, std::map<Vertex, unsigned int>> weight;
    std::set<Vertex> visitedSet;
    std::vector<Vertex> topologicVector;
    std::map< Vertex, std::pair< std::vector<Vertex>, std::vector<Vertex> > > parentAndChildrenMap;


};

#endif  // BACKTRACK_H_
