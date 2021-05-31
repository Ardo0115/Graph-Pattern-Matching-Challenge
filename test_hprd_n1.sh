#!/bin/bash

#i=1
#
#while [ $i -le 100 ]
#do
#    ./build/main/program data/lcc_hprd.igraph query/lcc_hprd_n1.igraph candidate_set/lcc_hprd_n1.cs ../jkj_decision2.txt
#    ((i++))
#done

# hprd n
./build/main/program data/lcc_hprd.igraph query/lcc_hprd_n1.igraph candidate_set/lcc_hprd_n1.cs ../tmp_test.txt
sleep 5
ps -ef | grep program | grep -v grep | awk '{print $2}' | xargs kill
./build/main/program data/lcc_hprd.igraph query/lcc_hprd_n3.igraph candidate_set/lcc_hprd_n3.cs ../tmp_test.txt
sleep 5
ps -ef | grep program | grep -v grep | awk '{print $2}' | xargs kill
./build/main/program data/lcc_hprd.igraph query/lcc_hprd_n5.igraph candidate_set/lcc_hprd_n5.cs ../tmp_test.txt
sleep 5
ps -ef | grep program | grep -v grep | awk '{print $2}' | xargs kill
./build/main/program data/lcc_hprd.igraph query/lcc_hprd_n8.igraph candidate_set/lcc_hprd_n8.cs ../tmp_test.txt
sleep 5
ps -ef | grep program | grep -v grep | awk '{print $2}' | xargs kill
