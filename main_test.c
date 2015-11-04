#include <stdio.h>
#include <stdlib.h>
#include "graph_structures.h"

int main(int argc, char** argv)
{
  FILE * input_file = NULL;

  if(argc != 2)
  {
    printf("Usage ./program file_with_teams_and_coaches\n");
    exit(0);
  }

  input_file = fopen(argv[1], "r");
  if(!input_file)
  {
    fprintf(stderr, "Error: %s could not be read!\n", argv[1]);
    exit(1);
  }

  read_entries(input_file);

  fclose(input_file);


  make_shoot_graph();
  print_subgraph_info();
  //print_team_node_info();
  //print_graph_representation();

  cleanup_graph();

  return 0;
}
