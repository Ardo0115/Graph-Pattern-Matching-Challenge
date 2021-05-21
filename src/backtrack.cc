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
        for (int i = 0; i < cs.GetCandidateSize(currentExtendableVertex); ++i) {
            Vertex v = cs.GetCandidate(currentExtendableVertex, i);
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

std::vector<Vertex> Backtrack::modifyExtendable(const Graph &graph, std::vector<Vertex> extendableQueryNodes, std::map<Vertex, Vertex> partialEmbedding) {
    for (Vertex extend : extendableQueryNodes){
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

        // TODO extendable revision
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
        // TODO 단순히 candidate 전체를 보는 게 아니라 extendable 한 candidate를 보도록하는 로직 추가해야함
        std::map<Vertex, std::vector<Vertex>> candidate = findCandidate(data, query, cs, partialEmbeddingM);
        if (candidate.size() == 0) return; // no extendable vertex
        int candidateNumber = INT_MAX;
        std::pair<Vertex, std::vector<Vertex>> selectedCandidate;

        // find candidate with min |C_M(u)|
        for (auto tempCandidate : candidate){
            if (tempCandidate.second.size() < candidateNumber){
                candidateNumber = tempCandidate.second.size();
                selectedCandidate = tempCandidate;
            }
        }
        Vertex u = selectedCandidate.first;

        for (Vertex v : selectedCandidate.second) {

            if (visitedSet.count(v) == 0) {
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


    }



void Backtrack::PrintAllMatches(const Graph &data, const Graph &query, const CandidateSet &cs) {
    std::cout << "t " << query.GetNumVertices() << "\n";
    // implement your code here.
    MapAndSet emptyPartialEmbedding;
    backTrack(data, query, cs, emptyPartialEmbedding);
}




