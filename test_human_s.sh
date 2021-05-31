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


# human n
echo human_s1
./build/main/program data/lcc_human.igraph query/lcc_human_s1.igraph candidate_set/lcc_human_s1.cs ../tmp_test.txt

echo human_s3
./build/main/program data/lcc_human.igraph query/lcc_human_s3.igraph candidate_set/lcc_human_s3.cs ../tmp_test.txt


echo human_s5
./build/main/program data/lcc_human.igraph query/lcc_human_s5.igraph candidate_set/lcc_human_s5.cs ../tmp_test.txt

echo human_s8
./build/main/program data/lcc_human.igraph query/lcc_human_s8.igraph candidate_set/lcc_human_s8.cs ../tmp_test.txt
