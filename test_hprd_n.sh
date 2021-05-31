#!/bin/bash
# this function is called when Ctrl-C is sent
function trap_ctrlc ()
{
    # perform cleanup here
    echo "Ctrl-C caught...performing clean up"
    echo "Doing cleanup"
  #  ps -eaf | grep program | grep -v grep | awk  '{print $2}' | xargs kill -9
 
    # exit shell script with error code 2
    # if omitted, shell script will continue execution
}
 
# initialise trap to call trap_ctrlc function
# when signal 2 (SIGINT) is received
trap "trap_ctrlc" 2


# hprd n
echo hprd_n1
./build/main/program data/lcc_hprd.igraph query/lcc_hprd_n1.igraph candidate_set/lcc_hprd_n1.cs ../tmp_test.txt

echo hprd_n3
./build/main/program data/lcc_hprd.igraph query/lcc_hprd_n3.igraph candidate_set/lcc_hprd_n3.cs ../tmp_test.txt


echo hprd_n5
./build/main/program data/lcc_hprd.igraph query/lcc_hprd_n5.igraph candidate_set/lcc_hprd_n5.cs ../tmp_test.txt

echo hprd_n8
./build/main/program data/lcc_hprd.igraph query/lcc_hprd_n8.igraph candidate_set/lcc_hprd_n8.cs ../tmp_test.txt
