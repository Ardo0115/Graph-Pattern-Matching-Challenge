/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}


std::map<Vertex, std::vector<Vertex>> Backtrack::findCandidate(const Graph &data, const Graph &query , const CandidateSet &cs, const MapAndSet& partialEmbedding) {

    std::map<Vertex, Vertex> currentEmbedding =  partialEmbedding.PartialEmbedding;
    std::set<Vertex> currentExtendable = partialEmbedding.extendable;
    std::map<Vertex, std::vector<Vertex>> result;

    bool isCurrentDataVertexConnectedToParent;

    for (Vertex currentExtendableVertex : currentExtendable){
        std::vector<Vertex> parentQueryVertices = getParentList(query, currentExtendableVertex);

        std::vector<Vertex> allCandidate = getAllCandidate(cs, currentExtendableVertex);
        for (Vertex v : allCandidate){
            isCurrentDataVertexConnectedToParent = true;
            for (Vertex parentQueryVertex : parentQueryVertices){
                if (data.IsNeighbor(currentEmbedding[parentQueryVertex], v) == false){
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
        if (partialEmbedding.find(*extend) != partialEmbedding.end()) {
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
}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query, const CandidateSet &cs) {
    std::cout << "t " << query.GetNumVertices() << "\n";
    // implement your code here.
    MapAndSet emptyPartialEmbedding;
    Backtrack::topologicVector = Backtrack::getTopologicVector(query, cs);
    setChildrenAndParentList(query);
    Backtrack::backTrack(data, query, cs, emptyPartialEmbedding);
}


void Backtrack::backTrack(const Graph &data, const Graph &query, const CandidateSet &cs, MapAndSet partialEmbeddingM) {

    /* If partialEmbedding's size is equal to query's size, Print it*/
    if (partialEmbeddingM.PartialEmbedding.size() == query.GetNumVertices()){
        std::cout << "a ";
        for (std::pair<Vertex, Vertex> pair : partialEmbeddingM.PartialEmbedding){
            std::cout << pair.second << " ";
        }
        std::cout << "\n";
    } 
    /* If partialEmbedding is empty, push root */
    else if (partialEmbeddingM.PartialEmbedding.empty()){
        /* Perform the following pseudo code */
        /*
         * foreach v in C(r) do {
         *  M <- {(r,v)}};
         *  Mark v as visited;
         *  Backtrack(q, q_d, CS, M);
         *  Mark v as unvisited;
         * */
        Vertex root = topologicVector.at(0);

        int rootCandidateSize = cs.GetCandidateSize(root);
        partialEmbeddingM.PartialEmbedding[root] = -1;

        std::vector<Vertex> extendableQueryNodes = getChildList(query, root);
        modifyExtendable(query, extendableQueryNodes, partialEmbeddingM.PartialEmbedding);
        partialEmbeddingM.extendable.insert(extendableQueryNodes.begin(), extendableQueryNodes.end());
        modifyExtendable(query, extendableQueryNodes, partialEmbeddingM.PartialEmbedding);
        std::vector<Vertex> unvisitedQueryVertices(topologicVector.begin(), topologicVector.end());
        unvisitedQueryVertices.erase(std::remove(unvisitedQueryVertices.begin(), unvisitedQueryVertices.end(), root), unvisitedQueryVertices.end());

        std::vector<Vertex> v_list = getAllCandidate(cs, root);


        for (int i = rootCandidateSize -1; i >= 0; --i) {
            Vertex v = v_list.at(i);
            partialEmbeddingM.PartialEmbedding[root] = v;
            visitedSet.insert(v);
            Backtrack::backTrack(data, query, cs, partialEmbeddingM);
            visitedSet.erase(visitedSet.find(v));
        }


    } 
    /* If partialEmbedding is in backtracking process */
    else {
        /* Perform the following pseudo code */
        /*
         * u <- extendable vertex with min weight w_M(u) or |C(u)|;
         * foreach v in C_M(u) do
         *  if v is unvisited then
         *      M' <- M +  {(u,v)};
         *      Mark v as visited;
         *      Backtrack(q, q_d, CS, M);
         *      Mark v as unvisited;
         *      */

        /* Select and remove from extendable node */
        std::map<Vertex, std::vector<Vertex>> candidate = findCandidate(data, query, cs, partialEmbeddingM);
        if (candidate.size() == 0) return; // no extendable vertex
        if (partialEmbeddingM.extendable.size() != candidate.size()) return;

        size_t minWeight = UINT_MAX;
        /* Find unvisited query node */
        std::vector<Vertex> unvisitedQueryVertices(topologicVector.begin(), topologicVector.end());
        for (Vertex query_u : unvisitedQueryVertices){
            if (partialEmbeddingM.PartialEmbedding.find(query_u) != partialEmbeddingM.PartialEmbedding.end()){
                unvisitedQueryVertices.erase(std::remove(unvisitedQueryVertices.begin(), unvisitedQueryVertices.end(), query_u), unvisitedQueryVertices.end());
            }
        }
        std::pair<Vertex, std::vector<Vertex>> selectedCandidate;

        /* Find candidate with min |C_M(u)| */
        for (auto tempCandidate : candidate){
            if (tempCandidate.second.size() < minWeight){
                minWeight = tempCandidate.second.size();
                selectedCandidate = tempCandidate;

            }
        }

        Vertex u = selectedCandidate.first;
        MapAndSet newPartialEmbedding(partialEmbeddingM);

        newPartialEmbedding.PartialEmbedding[u] = -1; // just mark u is in partialEmbedding with meaningless value
        std::vector<Vertex> extendableQueryNodes = getChildList(query, u);
        modifyExtendable(query, extendableQueryNodes, newPartialEmbedding.PartialEmbedding);
        newPartialEmbedding.extendable.erase( u );
        newPartialEmbedding.extendable.insert(extendableQueryNodes.begin(), extendableQueryNodes.end());

        std::vector<Vertex> v_list = selectedCandidate.second;
        std::sort(v_list.begin(), v_list.end(), cmp);

        for (Vertex v : v_list) {
            if (visitedSet.count(v) == 0) {
                newPartialEmbedding.PartialEmbedding[u] = v;
                visitedSet.insert(v);
                Backtrack::backTrack(data, query, cs, newPartialEmbedding);
                visitedSet.erase(visitedSet.find(v));
            }
        }
    }
    return;


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

/* This function defines the order of DAG */
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

/* Build DAG referring to Query, and Candidate Set */
std::vector<Vertex> Backtrack::getTopologicVector(const Graph &query, const CandidateSet &cs){
    std::set<Vertex> visited;
    std::set<Vertex> unvisited;
    std::vector<Vertex> S;
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
        /* Get vertices connected to visited in unvisited */
        S = Backtrack::getConnectedVertices(unvisited, visited, query);
        u = Backtrack::getNextTopologicElem(S, query, cs);
        visited.insert(u);
        unvisited.erase(u);
        topologicVector.push_back(u);
    }
    return topologicVector;
}




Vertex Backtrack::getMinlabelVertex(const Graph &graph, const CandidateSet &cs){
    Vertex r = -1;
    size_t minCuSize = UINT_MAX;
    size_t numVertices = graph.GetNumVertices();
    for (size_t u = 0; u < numVertices; ++u){
        size_t currentLabelSize = graph.GetLabelFrequency(graph.GetLabel(u)) ;
        if (currentLabelSize < minCuSize){
            r = u;
            minCuSize = currentLabelSize;
        }
    }

    return r;
}


bool Backtrack::cmp(Vertex w1, Vertex w2) {
    // < is minimum first (increasing order)
    // > is maximum first (decreasing order)
    return w1 > w2;
}

int Backtrack::countFurtherOccurrence (const Graph &query, const CandidateSet &cs, const std::vector<Vertex>& unvisitedQueryVertices, Vertex u, Vertex v) {
    int further_occurrence = 0;
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







