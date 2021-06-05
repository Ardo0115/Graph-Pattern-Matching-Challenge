/**
 * @file main.cc
 *
 */

#include "backtrack.h"
#include "candidate_set.h"
#include "common.h"
#include "graph.h"
#include <ctime>
#include <fstream>

int main(int argc, char* argv[]) {
  if (argc < 4) {
    std::cerr << "Usage: ./program <data graph file> <query graph file> "
                  "<candidate set file>\n";
    return EXIT_FAILURE;
  }
  
  std::string data_file_name = argv[1];
  std::string query_file_name = argv[2];
  std::string candidate_set_file_name = argv[3];
 
  Graph data(data_file_name);
  Graph query(query_file_name, true);
  CandidateSet candidate_set(candidate_set_file_name);
  std::ios_base::sync_with_stdio(false);
  Backtrack backtrack;

  clock_t start, end;
  double result;
  start = clock();
  backtrack.PrintAllMatches(data, query, candidate_set);
  end = clock();
  result = (double)(end -start)/CLOCKS_PER_SEC;

  std::string filePath = argv[4];
  std::ofstream writeFile;
  writeFile.open(filePath, std::ios_base::app);
  if (writeFile.is_open()){
    writeFile << "Execution Time : " << result << "\n";
    writeFile.close();
  }

  return EXIT_SUCCESS;
}
