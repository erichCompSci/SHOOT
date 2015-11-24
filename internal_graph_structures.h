#ifndef _INTERNAL_GRAPH_STRUCTURES_H
#define _INTERNAL_GRAPH_STRUCTURES_H

typedef struct _shoot_edge * shoot_edge_ptr, shoot_edge;

typedef struct _team_node * team_node_ptr, team_node;

typedef struct _shoot_subgraph * shoot_subgraph_ptr, shoot_subgraph;

typedef struct _coach_id_map_llist coach_id_map_llist;

typedef struct _shoot_field_space * shoot_field_space_ptr, shoot_field_space;

typedef struct _shoot_field_time * shoot_field_time_ptr, shoot_field_time;

struct _coach_id_map_llist
{
  char * coach_name;
  int coach_id;
  coach_id_map_llist * next;
};

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

struct _shoot_field_space
{
  char * field_name;
  int number_of_spaces;
  int days_avail;
  shoot_field_space_ptr next;
};


#endif
