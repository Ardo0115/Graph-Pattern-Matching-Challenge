/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}


std::map<Vertex, std::vector<Vertex>> Backtrack::findCandidate(const Graph &data, const Graph &query , const CandidateSet &cs, const MapAndSet& partialEmbedding) {
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
                if (data.IsNeighbor(currentEmbedding[parentQueryVertex], v) == false){
                    isCurrentDataVertexConnectedToParent = false;
                    break;
                }
            }
//            if (visitedSet.find(v) != visitedSet.end()){continue;}
            if (isCurrentDataVertexConnectedToParent){
                result[currentExtendableVertex].insert(result[currentExtendableVertex].end(), v);
            }
        }
    }

    // we do not erase anything here
    return result;

}

std::vector<Vertex> Backtrack::getNeighborList(const Graph &graph, Vertex index){
    std::vector<Vertex> neighbors;
    size_t firstNeighborOffset = graph.GetNeighborStartOffset(index);
    size_t endNeighborOffset = graph.GetNeighborEndOffset(index);
    for (size_t i = firstNeighborOffset; i < endNeighborOffset; ++i){
        neighbors.push_back(graph.GetNeighbor(i));
    }
    return neighbors;
}

void Backtrack::setChildrenAndParentList(const Graph &query) {
    for (Vertex index : topologicVector){
        std::vector<Vertex> neighbors = getNeighborList(query, index);
        std::vector<Vertex> children;
        std::vector<Vertex> parents;
        auto currentIndexIterator = std::find(topologicVector.begin(), topologicVector.end(), index);
        std::vector<Vertex> nextVertices(currentIndexIterator + 1, topologicVector.end());
//        std::vector<Vertex> previousVertices(topologicVector.begin(), currentIndexIterator);

        for (Vertex neighbor : neighbors){
            if (std::find(nextVertices.begin(), nextVertices.end(), neighbor ) != nextVertices.end()){
                children.push_back(neighbor);
            } else {
                parents.push_back(neighbor);
            }
        }

    parentAndChildrenMap[index] = std::make_pair(parents, children);
    }

}


std::vector<Vertex> Backtrack::getChildList(const Graph &query, Vertex index) {
    // get child in query DAG
    return  parentAndChildrenMap[index].second;

}

std::vector<Vertex> Backtrack::getParentList(const Graph &query, Vertex index) {
    // get parents in query DAG
    return  parentAndChildrenMap[index].first;
}

std::vector<Vertex> Backtrack::getAllCandidate(const CandidateSet &cs, Vertex queryVertex) {
    // return all the candidate of query vertex u
    int candidateSize = cs.GetCandidateSize(queryVertex);
    std::vector<Vertex> allCandidate;
    for (int i = 0; i < candidateSize; ++i) {
        allCandidate.insert(allCandidate.end(), cs.GetCandidate(queryVertex, i));
    }
    return allCandidate;
}

void Backtrack::modifyExtendable(const Graph &graph, std::vector<Vertex>& extendableQueryNodes, const std::map<Vertex, Vertex>& partialEmbedding) {

    for (auto extend = extendableQueryNodes.begin(); extend != extendableQueryNodes.end();) {
        // if query vertex u (extend) is already visited, remove this and continue
        // TODO 여기서 정상적으로 처리가 되지 않고 있음
        // iteration 도중에 extend를 삭제해서 정상적으로 작동 X
        if (partialEmbedding.find(*extend) != partialEmbedding.end()) {
//            extendableQueryNodes.erase(std::remove(extendableQueryNodes.begin(), extendableQueryNodes.end(), extend), extendableQueryNodes.end());
            extendableQueryNodes.erase(extend);
        } else {
            ++extend;
        }
    }

    bool isAllParentInPartialEmbedding = true;
    for (auto extend = extendableQueryNodes.begin(); extend != extendableQueryNodes.end();) {
        // if all the parent of query vertex u is not in partial embedding, remove this and continue to next extendable query vertex u
        std::vector<Vertex> parentList = getParentList(graph, *extend);
        for (Vertex parent : parentList){
            // if parent does not exist in partial embedding
            if (partialEmbedding.find(parent) == partialEmbedding.end()){
                // remove current extendable vertex(which do not have all its parent vertex in partial embedding) from the extendable list
                isAllParentInPartialEmbedding = false;
                break;
            }
        }
        if (isAllParentInPartialEmbedding == false) {
            extendableQueryNodes.erase(extend);
            isAllParentInPartialEmbedding = true;
        }else {
            ++extend;
        }

    }
//    return extendableQueryNodes;
}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query, const CandidateSet &cs) {
    std::cout << "t " << query.GetNumVertices() << "\n";
    // implement your code here.
    MapAndSet emptyPartialEmbedding;
    Backtrack::topologicVector = Backtrack::getTopologicVector(query, cs);
    setChildrenAndParentList(query);
    weight = buildWeightCS(data, query, cs);
    Backtrack::backTrack(data, query, cs, emptyPartialEmbedding);
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

        Vertex root = topologicVector.at(0);

        int rootCandidateSize = cs.GetCandidateSize(root);
        std::vector<Vertex> extendableQueryNodes = getChildList(query, root);
        partialEmbeddingM.extendable.insert(extendableQueryNodes.begin(), extendableQueryNodes.end());

        std::vector<Vertex> unvisitedQueryVertices(topologicVector.begin(), topologicVector.end());
        unvisitedQueryVertices.erase(std::remove(unvisitedQueryVertices.begin(), unvisitedQueryVertices.end(), root), unvisitedQueryVertices.end());


//        std::vector<std::pair<Vertex, unsigned int>> verticesAndWeight;
//
        std::vector<Vertex> v_list = getAllCandidate(cs, root);
//        for (Vertex v : v_list){
//            int further_count = countFurtherOccurrence(query, cs, unvisitedQueryVertices, root, v);
//            verticesAndWeight.push_back(std::make_pair(v, weight[root][v]));
//            verticesAndWeight.push_back(std::make_pair(v, further_count));
//        }
//        std::sort(verticesAndWeight.begin(), verticesAndWeight.end(), cmp);


        for (/*std::pair<Vertex,  unsigned int> vertexAndWeight : verticesAndWeight*/int i = rootCandidateSize -1; i >= 0; --i) {
//            Vertex v = vertexAndWeight.first;
//            Vertex v = verticesAndWeight.at(9).first;
            Vertex v = v_list.at(i);
//            std::cout << "u : " << root << ", v : " << v << std::endl;
            partialEmbeddingM.PartialEmbedding[root] = v;
            visitedSet.insert(v);
            Backtrack::backTrack(data, query, cs, partialEmbeddingM);
            visitedSet.erase(visitedSet.find(v));
        }


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
        // find unvisited query node
        std::vector<Vertex> unvisitedQueryVertices(topologicVector.begin(), topologicVector.end());
        for (Vertex query_u : unvisitedQueryVertices){
            if (partialEmbeddingM.PartialEmbedding.find(query_u) != partialEmbeddingM.PartialEmbedding.end()){
                unvisitedQueryVertices.erase(std::remove(unvisitedQueryVertices.begin(), unvisitedQueryVertices.end(), query_u), unvisitedQueryVertices.end());
            }
        }
        std::pair<Vertex, std::vector<Vertex>> selectedCandidate;

        // Candidate size ordering for decision_switch = 1
        // Path size ordering for decision_switch = 2
        int decision_switch = 1;
        if (decision_switch == 1){
            // find candidate with min |C_M(u)|
            for (auto tempCandidate : candidate){
                if (tempCandidate.second.size() < minWeight){
                    minWeight = tempCandidate.second.size();
                    selectedCandidate = tempCandidate;

                }
            }
        } else if (decision_switch == 2 ){
            // find candidate (u) with min w_M(u)
            for (auto tempCandidate : candidate){
                int current_weight = 0;
                for (Vertex extendableDataVertex : tempCandidate.second){
                    current_weight += weight[tempCandidate.first][extendableDataVertex];
//                    if (current_weight < countFurtherOccurrence(query, cs, unvisitedQueryVertices, tempCandidate.first, extendableDataVertex)){
//                        current_weight = countFurtherOccurrence(query, cs, unvisitedQueryVertices, tempCandidate.first, extendableDataVertex);
//                    }
//                    current_weight += countFurtherOccurrence(query, cs, unvisitedQueryVertices, tempCandidate.first, extendableDataVertex);
                }
                if (current_weight < minWeight){
                    minWeight = current_weight;
                    selectedCandidate = tempCandidate;
                }
            }
        }

        Vertex u = selectedCandidate.first;
        MapAndSet newPartialEmbedding(partialEmbeddingM);

        newPartialEmbedding.PartialEmbedding[u] = 0; // just mark u is in partialEmbedding with meaningless value
        std::vector<Vertex> extendableQueryNodes = getChildList(query, u);
        modifyExtendable(query, extendableQueryNodes, newPartialEmbedding.PartialEmbedding);
        newPartialEmbedding.extendable.erase( u );
        newPartialEmbedding.extendable.insert(extendableQueryNodes.begin(), extendableQueryNodes.end());






        std::vector<Vertex> v_list = selectedCandidate.second;
        std::vector<std::pair<Vertex, unsigned int>> verticesAndWeight;
//        std::random_shuffle(verticesAndWeight.begin(), verticesAndWeight.end());



        for (Vertex v : v_list){
            // TODO
            // weight 대신에 further_occurrence 를 넣자
            // u 보다 topologic sort vec 뒤에 있는 애들 중 라벨이 같은 에들 중에서
            // v 가 몇변 등장하는 지 세서 넣어주자.
//            int further_occurrence = countFurtherOccurrence(query, cs, unvisitedQueryVertices, u, v);
            verticesAndWeight.push_back(std::make_pair(v, weight[u][v]));
        }
        std::sort(verticesAndWeight.begin(), verticesAndWeight.end(), cmp);
        std::random_shuffle(verticesAndWeight.begin(), verticesAndWeight.end());

        for (/*Vertex v : v_list*/ std::pair<Vertex, unsigned int> vertexAndWeight : verticesAndWeight) {
            Vertex v = vertexAndWeight.first;
            if (visitedSet.count(v) == 0) {
                // line for debugging
//                std::cout << "u : " << u << ", v : " << v << ", partial Embedding Size : "<< newPartialEmbedding.PartialEmbedding.size() << std::endl;



                newPartialEmbedding.PartialEmbedding[u] = v;
                visitedSet.insert(v);
                Backtrack::backTrack(data, query, cs, newPartialEmbedding);
                visitedSet.erase(visitedSet.find(v));
            }
        }
    }
    return;


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
    for (/*int i = query.GetNumVertices() -1; i >= 0 ; --i*/ auto r = topologicVector.rbegin(); r != topologicVector.rend(); r++) {
        int i = *r;
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
    for (/*int u_current = query.GetNumVertices() - 1; u_current >= 0 ; --u_current*/ auto r = topologicVector.rbegin(); r != topologicVector.rend(); r++) {
        int u_current = *r;
        // if already assigned, continue
        if (uncheckedQueryVertices.find(u_current) == uncheckedQueryVertices.end()){
            continue;
        }

        std::vector<Vertex> currentCandidate = getAllCandidate(cs, u_current);
        int minWeight; int maxWeight; int weightSum;
        for (Vertex v_current : currentCandidate){
            minWeight = INT_MAX; maxWeight = INT_MIN; weightSum = 0;
            for (Vertex u : getChildList(query, u_current)){
                int current_weight = 0;
                for (Vertex v : getAllCandidate(cs, u)){
                    if (data.IsNeighbor(v, v_current)){
                        current_weight += weight[u][v];
                    }
                }
                // minWeight Version
                if (current_weight < minWeight){
                    minWeight = current_weight;
                }
                // maxWeight version
                if (current_weight > maxWeight) {
                    maxWeight = current_weight;
                }
                weightSum += current_weight;
            }
            // minWeight version VS maxWeight version VS weightSum Version
            weight[u_current][v_current] = minWeight;
        }
    }

    return weight;
}

std::vector<Vertex> Backtrack::getConnectedVertices(const std::set<Vertex> &toFindSet, const std::set<Vertex> &fromFindSet, const Graph &graph){
    std::vector<Vertex> selected;
    for(auto u : toFindSet){
        for(auto v : fromFindSet){
            if(graph.IsNeighbor(u,v)){
                selected.push_back(u);
                break;
            }
        }
    }
    return selected;
}

Vertex Backtrack::getNextTopologicElem(std::vector<Vertex> &S, const Graph &query, const CandidateSet &cs){
    std::pair<Graph, CandidateSet> query_cs_pair(query, cs);
    std::sort(S.begin(), S.end(), [query_cs_pair](Vertex u, Vertex v) {
        Label label_u = query_cs_pair.first.GetLabel(u);
        Label label_v = query_cs_pair.first.GetLabel(v);
        if (query_cs_pair.first.GetLabelFrequency(label_u) != query_cs_pair.first.GetLabelFrequency(label_v))
            return query_cs_pair.first.GetLabelFrequency(label_u) < query_cs_pair.first.GetLabelFrequency(label_v);
        else if (query_cs_pair.second.GetCandidateSize(u) != query_cs_pair.second.GetCandidateSize(v))
            return query_cs_pair.second.GetCandidateSize(u) < query_cs_pair.second.GetCandidateSize(v);
        else
            return u < v;
    });
    return *(S.begin());
}

std::set<Vertex> Backtrack::getAllVertices(const Graph &query){
    std::set<Vertex> allVertices;
    size_t numVertices = query.GetNumVertices();
    for(size_t i=0; i < numVertices; ++i){
        allVertices.insert(i);
    }
    return allVertices;
}

std::vector<Vertex> Backtrack::getTopologicVector(const Graph &query, const CandidateSet &cs){
    std::set<Vertex> visited;
    std::set<Vertex> unvisited;
    std::vector<Vertex> S;
    // std::vector<Vertex> setDiff;
    std::vector<Vertex> topologicVector;
    Vertex u;

    /* Get all Vertices */
    unvisited = Backtrack::getAllVertices(query);

    /* Get r */
    Vertex r = Backtrack::getMinlabelVertex(query, cs);

    /* Traverse */
    visited.insert(r);
    unvisited.erase(r);
    topologicVector.push_back(r);
    size_t numVertices = query.GetNumVertices();
    while (numVertices != visited.size()){
        /* Compute setDiff (V-visited) */
        // setDiff.clear();
        // std::set_difference(allVertices.begin(), allVertices.end(), visited.begin(), visited.end(), std::inserter(setDiff, setDiff.begin()));

        /* Get vertices connected to visited in unvisited */
        S = Backtrack::getConnectedVertices(unvisited, visited, query); // new visited Vertex만 고려하면 되니까 최적화 가능할 듯
        u = Backtrack::getNextTopologicElem(S, query, cs);
        visited.insert(u);
        unvisited.erase(u);
        topologicVector.push_back(u);
    }
    return topologicVector;
}

// DAG ordering by BFS

//std::vector<Vertex> Backtrack::getTopologicVector(const Graph &query, const CandidateSet &cs){
//    std::unordered_set<Vertex> visited;
//    std::vector<Vertex> topologicVector;
//    std::vector<Vertex> neighbors;
//    std::queue<Vertex> Q;
//    Vertex v; // v is in query graph
//
//    /* Get r */
//    Vertex r = Backtrack::getMinlabelVertex(query, cs);
////    Vertex r = 1;
//
//    /* Traverse */
//    visited.insert(r);
//    topologicVector.push_back(r);
//    Q.push(r);
//    while (!Q.empty()){
//        v = Q.front();
//        Q.pop();
//        /* Get neighbors */
//        neighbors = Backtrack::getNeighborList(query, v);
//
//        /* Sort neighbors */
//        std::sort(neighbors.begin(), neighbors.end(), [query, cs](Vertex u, Vertex v) {
//            Label label_u = query.GetLabel(u);
//            Label label_v = query.GetLabel(v);
//            if (query.GetLabelFrequency(label_u) != query.GetLabelFrequency(label_v))
//                return query.GetLabelFrequency(label_u) < query.GetLabelFrequency(label_v);
//            else if (cs.GetCandidateSize(u) != cs.GetCandidateSize(v))
//                return cs.GetCandidateSize(u) < cs.GetCandidateSize(v);
//            else if (query.GetDegree(u) != query.GetDegree(v))
//                return query.GetDegree(u) > query.GetDegree(v);
//            else
//                return u < v;
//        });
//
//        /* Enqeue adjacent to v*/
//        for (auto adj_v : neighbors){
//            if (visited.count(adj_v) == 0){
//                visited.insert(adj_v);
//                topologicVector.push_back(adj_v);
//                Q.push(adj_v);
//            }
//        }
//
//    }
//
//    return topologicVector;
//}


Vertex Backtrack::getMinlabelVertex(const Graph &graph, const CandidateSet &cs){
    Vertex r = -1;
    size_t minCuSize = UINT_MAX;
    size_t numVertices = graph.GetNumVertices();
    for (size_t u = 0; u < numVertices; ++u){
        int currentLabelSize = graph.GetLabelFrequency(graph.GetLabel(u)) ;
//        double currentCuSize = cs.GetCandidateSize(u);
        if (currentLabelSize < minCuSize){
            r = u;
            minCuSize = currentLabelSize;
        }
    }

    return r;
}


bool Backtrack::cmp(std::pair<Vertex, unsigned int> &w1, std::pair<Vertex, unsigned int> &w2) {
    // < is minimum first (increasing order)
    // > is maximum first (decreasing order)
    if (w1.second != w2.second){
        return w1.second < w2.second;
    } else {
        return w1.first > w2.first;
    }
}

int Backtrack::countFurtherOccurrence (const Graph &query, const CandidateSet &cs, const std::vector<Vertex>& unvisitedQueryVertices, Vertex u, Vertex v) {
    int further_occurrence = 0;
//            auto currentIndexIterator = std::find(topologicVector.begin(), topologicVector.end(), u);
    for (Vertex furtherQueryNode : unvisitedQueryVertices){
        if (query.GetLabel(u) != query.GetLabel(furtherQueryNode)){
            continue;
        }
        std::vector<Vertex> furtherCandidateList_u = getAllCandidate(cs, furtherQueryNode);
        if ( std::find(furtherCandidateList_u.begin(), furtherCandidateList_u.end(), v) != furtherCandidateList_u.end()){
            further_occurrence++;
        }
    }
    return further_occurrence;
}







