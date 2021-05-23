#!/bin/bash

i=1

while [ $i -le 100 ]
do
    ./build/main/program data/lcc_hprd.igraph query/lcc_hprd_n1.igraph candidate_set/lcc_hprd_n1.cs ../jkj_optimization_before_endl_decision1.txt
    ((i++))
done
