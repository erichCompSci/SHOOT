#ifndef _GRAPH_DATA_STRUCTURES_H
#define _GRAPH_DATA_STRUCTURES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct _shoot_edge * shoot_edge_ptr, shoot_edge;

typedef struct _team_node
{
  char * team_age_group;
  char * team_level_name;
  int team_id;
  int team_level;          //The team level is 1 for Elite and 5 for Silver
  char * coach_name;
  int coach_id;
  shoot_edge_ptr edges;
  int number_of_edges;

} * team_node_ptr, team_node;

struct _shoot_edge
{
  int team_id;
  team_node_ptr other_node;
  shoot_edge_ptr next;
};


int read_entries(FILE * input_file);

void print_team_node_info();

int make_shoot_subgraphs();

void cleanup_graph();
#endif
