#ifndef _INTERNAL_GRAPH_STRUCTURES_H
#define _INTERNAL_GRAPH_STRUCTURES_H

typedef struct _shoot_edge * shoot_edge_ptr, shoot_edge;

typedef struct _team_node * team_node_ptr, team_node;

typedef struct _shoot_subgraph * shoot_subgraph_ptr, shoot_subgraph;

struct _team_node
{
  char * team_age_group;
  char * team_level_name;
  int team_id;
  int team_level;          //The team level is 1 for Elite and 5 for Silver
  char * coach_name;
  int coach_id;
  shoot_edge_ptr edges;
  int number_of_edges;
  team_node_ptr next;

};

struct _shoot_edge
{
  int team_id;
  team_node_ptr other_node;
  shoot_edge_ptr next;
};

struct _shoot_subgraph
{
  int number_of_nodes;
  team_node_ptr teams;
  shoot_subgraph_ptr next;
};

#endif
