/**
 * @file main.cc
 *
 */

#include "backtrack.h"
#include "candidate_set.h"
#include "common.h"
#include "graph.h"

int main(int argc, char* argv[]) {
 if (argc < 4) {
   std::cerr << "Usage: ./program <data graph file> <query graph file> "
                "<candidate set file>\n";
   return EXIT_FAILURE;
 }

 std::string data_file_name = argv[1];
 std::string query_file_name = argv[2];
 std::string candidate_set_file_name = argv[3];
// lcc_human, lcc_hprd, lcc_yeast
// test_data.igraph, test_query.igraph, test_cs.cs
    // std::string data_file_name = "C:\\Users\\jkj72\\Desktop\\graphChallenge\\Graph-Pattern-Matching-Challenge\\data\\lcc_yeast.igraph";
    // std::string query_file_name = "C:\\Users\\jkj72\\Desktop\\graphChallenge\\Graph-Pattern-Matching-Challenge\\query\\lcc_yeast_s3.igraph";
    // std::string candidate_set_file_name = "C:\\Users\\jkj72\\Desktop\\graphChallenge\\Graph-Pattern-Matching-Challenge\\candidate_set\\lcc_yeast_s3.cs";

  Graph data(data_file_name);
  Graph query(query_file_name, true);
  CandidateSet candidate_set(candidate_set_file_name);

  Backtrack backtrack;

  backtrack.PrintAllMatches(data, query, candidate_set);

  return EXIT_SUCCESS;
}
