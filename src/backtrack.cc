/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}


Vertex Backtrack::findExtendable(const Graph &query, const CandidateSet &cs) {
    // TODO
    // extendable candidate 숫자를 기준으로 선택하는 함수로 변경하기
    int count = INT_MAX;
    Vertex minCandidateSizeVertex = -1; // FIXED : segmentation fault handled
    int size;
    for (Vertex u : extendable){
        size = cs.GetCandidateSize(u);
        if (count > size){
            count = size;
            minCandidateSizeVertex = u;
        }
    }

    // erase selected node
    // TODO extendable revision
    extendable.erase(minCandidateSizeVertex);
    return minCandidateSizeVertex;

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

void Backtrack::backTrack(const Graph &data, const Graph &query, const CandidateSet &cs, std::map<Vertex, Vertex> partialEmbeddingM) {

    // query graph is already in DAG format
    if (partialEmbeddingM.size() == query.GetNumVertices()){
        // report M
        std::cout << "a ";
        for (std::pair<Vertex, Vertex> pair : partialEmbeddingM){
            std::cout << pair.second << " ";
        }
        std::cout << std::endl;
    } else if (partialEmbeddingM.empty()){

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
        extendable.insert(extendableQueryNodes.begin(), extendableQueryNodes.end());


        for (int i = 0; i < rootCandidateSize; ++i) {
            Vertex v = cs.GetCandidate(0,i);

            partialEmbeddingM[0] = v;
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
        Vertex u = findExtendable(query, cs);

        if (u == -1) return; // no extendable vertex


        int candidateSize = cs.GetCandidateSize(u);
        bool dataGraphParentNotConnected = false;

        for (int j = 0; j < candidateSize; ++j) {
            Vertex v = cs.GetCandidate(u, j);


            std::vector<Vertex> parentQueryVertices = getParentList(query, u);
                for (Vertex parentQuery : parentQueryVertices){
                    // TODO fix memory access issue
                    // partialEmbeddingM might not have element at parentQuery
                    /*partialEmbeddingM.find(parentQuery) == partialEmbeddingM.end() || */
                    if (data.IsNeighbor(partialEmbeddingM[parentQuery], v) == false){
                        dataGraphParentNotConnected = true;
                        break;
                    }
                }
                if (dataGraphParentNotConnected){
                    dataGraphParentNotConnected = false;
                    continue;
                }

            if (visitedSet.count(v) == 0) {
                std::map<Vertex, Vertex> newPartialEmbedding(partialEmbeddingM);
                newPartialEmbedding[u] = v;
                visitedSet.insert(v);

                // TODO 이 부분에 extendableQueryNodes 중에서 부모가 전부 partial Embedding에 들어가있는 애들만 추가하도록 하는 로직 추가해야함
                // TODO extendable revision
                std::vector<Vertex> extendableQueryNodes = getChildList(query, u);
                extendableQueryNodes = modifyExtendable(query, extendableQueryNodes, newPartialEmbedding);
                extendable.insert(extendableQueryNodes.begin(), extendableQueryNodes.end());

                Backtrack::backTrack(data, query, cs, newPartialEmbedding);
                visitedSet.erase(visitedSet.find(v));
                }
            }
        }


    }



void Backtrack::PrintAllMatches(const Graph &data, const Graph &query, const CandidateSet &cs) {
    std::cout << "t " << query.GetNumVertices() << "\n";
    // implement your code here.
    std::map<Vertex, Vertex> emptyPartialEmbedding;
    backTrack(data, query, cs, emptyPartialEmbedding);
}




