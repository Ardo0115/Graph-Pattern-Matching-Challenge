//
// Created by jkj72 on 2021-05-21.
//

#ifndef SUBGRAPH_MATCHING_MAPANDSET_H
#define SUBGRAPH_MATCHING_MAPANDSET_H

#include "candidate_set.h"
#include "common.h"
#include "graph.h"
#include "map"

class MapAndSet {
    public:
    MapAndSet() {};
    MapAndSet(MapAndSet& another){
        this->PartialEmbedding = another.PartialEmbedding;
        this->extendable = another.extendable;
    }
    ~MapAndSet() {};

    std::map<Vertex, Vertex> PartialEmbedding;
    std::set<Vertex> extendable;


};
#endif //SUBGRAPH_MATCHING_MAPANDSET_H
