#!/bin/bash

i=1

while [ $i -le 10 ]
do
    ./build/main/program data/lcc_hprd.igraph query/lcc_hprd_n1.igraph candidate_set/lcc_hprd_n1.cs ../final_wo_weight.txt
    ((i++))
done
