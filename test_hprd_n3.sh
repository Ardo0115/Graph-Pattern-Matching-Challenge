#!/bin/bash

i=1

while [ $i -le 1 ]
do
    ./build/main/program data/lcc_hprd.igraph query/lcc_hprd_n3.igraph candidate_set/lcc_hprd_n3.cs ../final_wo_weight_hprd_n3.txt
    ((i++))
done
