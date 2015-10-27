#ifndef _GRAPH_DATA_STRUCTURES_H
#define _GRAPH_DATA_STRUCTURES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct _team_node
{
  char * team_name;
  int team_id;
  char * coach_name;
  int coach_id;
} * team_node_ptr, team_node;

int read_entries(FILE * input_file);

void print_team_node_info();


#endif
