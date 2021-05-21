/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}


// Vertex Backtrack::findExtendable(const Graph &query, const CandidateSet &cs) {
//     // TODO
//     // extendable candidate 숫자를 기준으로 선택하는 함수로 변경하기
//     int count = INT_MAX;
//     Vertex minCandidateSizeVertex;
//     int size;
//     for (Vertex u : extendable){
//         size = cs.GetCandidateSize(u);
//         if (count > size){
//             count = size;
//             minCandidateSizeVertex = u;
//         }
//     }

//     // erase selected node
//     // TODO extendable revision
//     extendable.erase(std::remove(extendable.begin(), extendable.end(), minCandidateSizeVertex), extendable.end());
//     return minCandidateSizeVertex;

// }

Vertex Backtrack::findMinExtendable(const Graph &query, const CandidateSet &cs, const std::map<Vertex, Vertex> &M) {
    // TODO
    // extendable candidate 숫자를 기준으로 선택하는 함수로 변경하기
    int count = INT_MAX;
    Vertex minCandidateSizeVertex;
    int size;
    
    for (auto current_pair : M){
        std::vector<Vertex> current_childs = Backtrack::getChildList(query, current_pair.first);
        for(auto current_child : current_childs){
            size = cs.GetCandidateSize(current_child);
            if (count > size){
                count = size;
                minCandidateSizeVertex = current_child;
            }
        }
        
    }

    // erase selected node
    // TODO extendable revision
    // extendable.erase(std::remove(extendable.begin(), extendable.end(), minCandidateSizeVertex), extendable.end());
    return minCandidateSizeVertex;

}

std::vector<Vertex> Backtrack::getCandidatesM_u(const Graph &query, const CandidateSet &cs, const std::map<Vertex, Vertex> &M, Vertex &u){
    int candidateSize = cs.GetCandidateSize(u);
    std::vector<Vertex> possibleCandidates;
    std::vector<Vertex> parentsOf_u = getParentList(query, u);

    for (int j = 0; j < candidateSize; ++j) {
        Vertex v = cs.GetCandidate(u, j); 
        bool test_pass = true;


        for (auto it = M.begin(); it != M.end(); ++it){
            /* Check injective */
            if (it->second == v){
                test_pass = false;
                break;
            }

            /* Check edge */
            for (auto parent : parentsOf_u){
                Vertex parent_v = M.find(parent)->second;
                

            }
            
        }




        if (test_pass == true){
            possibleCandidates.push_back(v);
        }
    }
                
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

// std::vector<Vertex> Backtrack::modifyExtendable(const Graph &graph, std::vector<Vertex> extendableQueryNodes) {
//     for (Vertex extend : extendableQueryNodes){
//         std::vector<Vertex> parentList = getParentList(graph, extend);
//         for (Vertex parent : parentList){
//             if (partialEmbedding.find(parent) == partialEmbedding.end()){
//                 // remove current extendable vertex from the extendable list
//                 extendableQueryNodes.erase(std::remove(extendableQueryNodes.begin(), extendableQueryNodes.end(), extend), extendableQueryNodes.end());
//                 break;
//             }
//         }
//     }
//     return extendableQueryNodes;
// }

void Backtrack::backTrack(const Graph &data, const Graph &query,
                          const CandidateSet &cs, std::map<Vertex, Vertex> &M) {



    // query graph is already in DAG format
    if (M.size() == query.GetNumVertices()){
        // TODO 1
        // report M
        std::cout << "a ";
        for (std::pair<Vertex, Vertex> pair : M){
            std::cout << pair.second << " ";
        }
        std::cout << std::endl;
    } else if (M.empty()){

        /* TODO 2
         * foreach v in C(r) do {
         *  M <- {(r,v)}};
         *  Mark v as visited;
         *  Backtrack(q, q_d, CS, M);
         *  Mark v as unvisited;
         * */

        int rootCandidateSize = cs.GetCandidateSize(0);

        // TODO extendable revision
        // std::vector<Vertex> extendableQueryNodes = getChildList(query, 0);
        // extendable.insert(extendable.end(), extendableQueryNodes.begin(), extendableQueryNodes.end());



        for (int i = 0; i < rootCandidateSize; ++i) {
            // M.clear();
            Vertex v = cs.GetCandidate(0,i);

            M[0] = v;
            visitedSet.insert(v);
            Backtrack::backTrack(data, query, cs, M);
            visitedSet.erase(visitedSet.find(v));

        }
        return;


    } else {

        /* TODO 3
         * u <- extendable vertex with min weight w_M(u) or |C(u)|;
         * foreach v in C_M(u) do
         *  if v is unvisited then
         *      M' <- M +  {(u,v)};
         *      Mark v as visited;
         *      Backtrack(q, q_d, CS, M);
         *      Mark v as unvisited;
         *      */

        // select and remove from extendable node
        Vertex u = findMinExtendable(query, cs, M);

        // Suggestion : Candidate Vector로 받기
        int candidateSize = cs.GetCandidateSize(u);
        // TODO 단순히 candidate 전체를 보는 게 아니라 extendable 한 candidate를 보도록하는 로직 추가해야함
        std::vector<Vertex> candidates_M_u = getCandidatesM_u(query, cs, M, u);
        for (int j = 0; j < candidateSize; ++j) {
            Vertex v = cs.GetCandidate(u, j);
            // Suggestion : Vertex 에 visited 멤버로 넣기
            if (visitedSet.count(v) == 0){
                

                std::map<Vertex, Vertex> M_prime;
                for(auto tmp_pair : M){
                    M_prime[tmp_pair.first] = tmp_pair.second;
                }
                M_prime[u] = v;
                visitedSet.insert(v);
              

                // TODO 이 부분에 extendableQueryNodes 중에서 부모가 전부 partial Embedding에 들어가있는 애들만 추가하도록 하는 로직 추가해야함
                // TODO extendable revision
                // std::vector<Vertex> extendableQueryNodes = getChildList(query, u);
                // extendableQueryNodes = modifyExtendable(query, extendableQueryNodes);
                // extendable.insert(extendable.end(), extendableQueryNodes.begin(), extendableQueryNodes.end());

                Backtrack::backTrack(data, query, cs, M_prime);
                visitedSet.erase(visitedSet.find(v));
            }
        }


    }

}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query, const CandidateSet &cs) {
    std::cout << "t " << query.GetNumVertices() << "\n";
    // implement your code here.
    std::map<Vertex, Vertex> M;
    backTrack(data, query, cs, M);
}





