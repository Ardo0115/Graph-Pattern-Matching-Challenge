#!/bin/bash

i=1

while [ $i -le 1 ]
do
    ./build/main/program data/lcc_hprd.igraph query/lcc_hprd_n8.igraph candidate_set/lcc_hprd_n8.cs ../final_wo_weight_hprd_n8.txt
    ((i++))
done
