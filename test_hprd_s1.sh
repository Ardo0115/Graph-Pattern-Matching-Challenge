#!/bin/bash

i=1

while [ $i -le 1 ]
do
    ./build/main/program data/lcc_hprd.igraph query/lcc_hprd_s1.igraph candidate_set/lcc_hprd_s1.cs ../final_wo_weight_hprd_s1.txt
    ((i++))
done
