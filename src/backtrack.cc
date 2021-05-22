/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}


std::map<Vertex, std::vector<Vertex>> Backtrack::findCandidate(const Graph &data, const Graph &query , const CandidateSet &cs, MapAndSet partialEmbedding) {
    // TODO!!!!
    // partialEmbedding을 바탕으로 Candidate(query Vertex와 data Vertex 리스트의 페어) 리턴하기
    std::map<Vertex, Vertex> currentEmbedding =  partialEmbedding.PartialEmbedding;
    std::set<Vertex> currentExtendable = partialEmbedding.extendable;
    std::map<Vertex, std::vector<Vertex>> result;

    bool isCurrentDataVertexConnectedToParent;

    for (Vertex currentExtendableVertex : currentExtendable){
        std::vector<Vertex> allCandidate = getAllCandidate(cs, currentExtendableVertex);
        for (Vertex v : allCandidate){
            isCurrentDataVertexConnectedToParent = true;
            std::vector<Vertex> parentQueryVertices = getParentList(query, currentExtendableVertex);

            for (Vertex parentQueryVertex : parentQueryVertices){
                if (data.IsNeighbor(partialEmbedding.PartialEmbedding[parentQueryVertex], v) == false){
                    isCurrentDataVertexConnectedToParent = false;
                    break;
                }
            }
            if (isCurrentDataVertexConnectedToParent){
                result[currentExtendableVertex].insert(result[currentExtendableVertex].end(), v);
            }
        }
    }

    // we do not erase anything here
    return result;

}

std::vector<Vertex> Backtrack::getChildList(const Graph &graph, Vertex index) {
    int firstNeighborOffset = graph.GetNeighborStartOffset(index);
    int endNeighborOffset = graph.GetNeighborEndOffset(index);
    std::vector<Vertex> neighbors;
    for (int i = firstNeighborOffset; i < endNeighborOffset; ++i) {
        if (graph.GetNeighbor(i) > index){
            neighbors.push_back(graph.GetNeighbor(i));
        }
    }
    return  neighbors;
}

std::vector<Vertex> Backtrack::getParentList(const Graph &graph, Vertex index) {
    int firstNeighborOffset = graph.GetNeighborStartOffset(index);
    int endNeighborOffset = graph.GetNeighborEndOffset(index);
    std::vector<Vertex> neighbors;
    for (int i = firstNeighborOffset; i < endNeighborOffset; ++i) {
        if (graph.GetNeighbor(i) < index){
            neighbors.push_back(graph.GetNeighbor(i));
        }
    }
    return  neighbors;
}

std::vector<Vertex> Backtrack::getAllCandidate(const CandidateSet &cs, Vertex queryVertex) {
    int candidateSize = cs.GetCandidateSize(queryVertex);
    std::vector<Vertex> allCandidate;
    for (int i = 0; i < candidateSize; ++i) {
        allCandidate.insert(allCandidate.end(), cs.GetCandidate(queryVertex, i));
    }
    return allCandidate;
}

std::vector<Vertex> Backtrack::modifyExtendable(const Graph &graph, std::vector<Vertex> extendableQueryNodes, std::map<Vertex, Vertex> partialEmbedding) {
    // u is newly added query Vertex
    for (Vertex extend : extendableQueryNodes){
        if (partialEmbedding.find(extend) != partialEmbedding.end()){
            extendableQueryNodes.erase(std::remove(extendableQueryNodes.begin(), extendableQueryNodes.end(), extend), extendableQueryNodes.end());
            continue;
        }

        std::vector<Vertex> parentList = getParentList(graph, extend);
        for (Vertex parent : parentList){
            // if parent does not exist in partial embedding
            if (partialEmbedding.find(parent) == partialEmbedding.end()){
                // remove current extendable vertex(which do not have all its parent vertex in partial embedding) from the extendable list
                extendableQueryNodes.erase(std::remove(extendableQueryNodes.begin(), extendableQueryNodes.end(), extend), extendableQueryNodes.end());
                break;
            }
        }
    }
    return extendableQueryNodes;
}

void Backtrack::backTrack(const Graph &data, const Graph &query, const CandidateSet &cs, MapAndSet partialEmbeddingM) {
    // query graph is already in DAG format

    if (partialEmbeddingM.PartialEmbedding.size() == query.GetNumVertices()){
        // report M
        std::cout << "a ";
        for (std::pair<Vertex, Vertex> pair : partialEmbeddingM.PartialEmbedding){
            std::cout << pair.second << " ";
        }
        std::cout << std::endl;
    } else if (partialEmbeddingM.PartialEmbedding.empty()){

        /*
         * foreach v in C(r) do {
         *  M <- {(r,v)}};
         *  Mark v as visited;
         *  Backtrack(q, q_d, CS, M);
         *  Mark v as unvisited;
         * */


        int rootCandidateSize = cs.GetCandidateSize(0);

        std::vector<Vertex> extendableQueryNodes = getChildList(query, 0);
        partialEmbeddingM.extendable.insert(extendableQueryNodes.begin(), extendableQueryNodes.end());


        for (int i = 0; i < rootCandidateSize; ++i) {
            Vertex v = cs.GetCandidate(0,i);
            partialEmbeddingM.PartialEmbedding[0] = v;
            visitedSet.insert(v);
            Backtrack::backTrack(data, query, cs, partialEmbeddingM);
            visitedSet.erase(visitedSet.find(v));
        }
        return;


    } else {

        /*
         * u <- extendable vertex with min weight w_M(u) or |C(u)|;
         * foreach v in C_M(u) do
         *  if v is unvisited then
         *      M' <- M +  {(u,v)};
         *      Mark v as visited;
         *      Backtrack(q, q_d, CS, M);
         *      Mark v as unvisited;
         *      */


        // select and remove from extendable node
        std::map<Vertex, std::vector<Vertex>> candidate = findCandidate(data, query, cs, partialEmbeddingM);
        if (candidate.size() == 0) return; // no extendable vertex
//        if (partialEmbeddingM.extendable.empty()) return;

        int minWeight = INT_MAX;
        std::pair<Vertex, std::vector<Vertex>> selectedCandidate;

        // Candidate size ordering for decision_switch = 1
        // Path size ordering for decision_switch = 2
        int decision_switch = 2;

        if (decision_switch == 1){
            // find candidate with min |C_M(u)|
            for (auto tempCandidate : candidate){
                if (tempCandidate.second.size() < minWeight){
                    minWeight = tempCandidate.second.size();
                    selectedCandidate = tempCandidate;

                }
            }
        } else if (decision_switch == 2 ){
            // find candidate with min w_M(u)
            for (auto tempCandidate : candidate){
                int current_weight = 0;
                for (Vertex extendableDataVertex : tempCandidate.second){
                    current_weight += weight[tempCandidate.first][extendableDataVertex];
                }
                if (current_weight < minWeight){
                    minWeight = current_weight;
                    selectedCandidate = tempCandidate;
                }
            }
        }



        Vertex u = selectedCandidate.first;
        std::vector<Vertex> v_list = selectedCandidate.second;
        std::vector<std::pair<Vertex, unsigned int>> verticesAndWeight;
        for (Vertex v : v_list){
            verticesAndWeight.push_back(std::make_pair(v, weight[u][v]));
        }
        std::sort(verticesAndWeight.begin(), verticesAndWeight.end(), cmp);

        for (/*Vertex v : v_list*/ std::pair<Vertex, unsigned int> vertexAndWeight : verticesAndWeight) {
            Vertex v = vertexAndWeight.first;
            if (visitedSet.count(v) == 0) {
                // line for debugging
                std::cout << "u : " << u << ", v : " << v << std::endl;
//                if (u == 33 && v == 123){
//                    std::cout << std::endl;
//                }

                MapAndSet newPartialEmbedding(partialEmbeddingM);
                newPartialEmbedding.PartialEmbedding[u] = v;

                // TODO extendable revision
                std::vector<Vertex> extendableQueryNodes = getChildList(query, u);
                newPartialEmbedding.extendable.erase( u );
                extendableQueryNodes = modifyExtendable(query, extendableQueryNodes, newPartialEmbedding.PartialEmbedding);
                newPartialEmbedding.extendable.insert(extendableQueryNodes.begin(), extendableQueryNodes.end());

                visitedSet.insert(v);
                Backtrack::backTrack(data, query, cs, newPartialEmbedding);
                visitedSet.erase(visitedSet.find(v));
                }
            }
        }
    return;


    }



void Backtrack::PrintAllMatches(const Graph &data, const Graph &query, const CandidateSet &cs) {
    std::cout << "t " << query.GetNumVertices() << "\n";
    // implement your code here.
    MapAndSet emptyPartialEmbedding;
    weight = buildWeightCS(data, query, cs);
    Backtrack::backTrack(data, query, cs, emptyPartialEmbedding);
}


std::map<Vertex, std::map<Vertex, unsigned int>> Backtrack::buildWeightCS(const Graph &data, const Graph &query, const CandidateSet &cs) {

    // initialize weight
    std::map<Vertex, std::map<Vertex, unsigned int>> weight;
    for (int i = 0; i < query.GetNumVertices(); ++i) {
        std::vector<Vertex> allCandidate = getAllCandidate(cs, i);
        for (Vertex candidate : allCandidate){
            weight[i][candidate] = 0;
        }
    }

    // mark unchecked query vertices
    std::set<Vertex> uncheckedQueryVertices;
    for (int i = 0; i < query.GetNumVertices(); ++i) {
        uncheckedQueryVertices.insert(i);
    }

    // assign 1 to "end of path" query vertex
    bool haveSingleParentChild = false;
    for (int i = query.GetNumVertices() -1; i >= 0 ; --i) {
        std::vector<Vertex> childList = getChildList(query, i);
        for (Vertex child : childList){
            std::vector<Vertex> parentList = getParentList(query, child);
            if (parentList.size() == 1){
                haveSingleParentChild = true;
                break;
            }
        }

        if (haveSingleParentChild){
            haveSingleParentChild = false;
            continue;
        } else {
            // finally assign 1
            uncheckedQueryVertices.erase(i);
            for (int j = 0; j < cs.GetCandidateSize(i); ++j) {
                weight[i][cs.GetCandidate(i,j)] = 1;
            }
        }
    }

    // assign weight from the bottom up manner
    for (int u_current = query.GetNumVertices() - 1; u_current >= 0 ; --u_current) {
        // if already assigned, continue
        if (uncheckedQueryVertices.find(u_current) == uncheckedQueryVertices.end()){
            continue;
        }

        std::vector<Vertex> currentCandidate = getAllCandidate(cs, u_current);
        int minWeight;
        for (Vertex v_current : currentCandidate){
            minWeight = INT_MAX;
            for (Vertex u : getChildList(query, u_current)){
                int current_weight = 0;
                for (Vertex v : getAllCandidate(cs, u)){
                    if (data.IsNeighbor(v, v_current)){
                        current_weight += weight[u][v];
                    }
                }
                if (current_weight < minWeight){
                    minWeight = current_weight;
                }
            }
            weight[u_current][v_current] = minWeight;
        }
    }

    return weight;
}

bool Backtrack::cmp(std::pair<Vertex, unsigned int> &w1, std::pair<Vertex, unsigned int> &w2) {
    return w1.second < w2.second;
}






