#ifndef _GRAPH_DATA_STRUCTURES_H
#define _GRAPH_DATA_STRUCTURES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "internal_graph_structures.h"
#include "coloring_algorithm.h"

enum time_slot { FIVE, SIX, SIX_THIRTY, SEVEN_THIRTY, EIGHT };

int read_entries(FILE * input_file);
int make_new_field_space(char * name, int number_of_teams, enum time_slot what_time);


void print_team_node_info();
void print_graph_representation();
void print_subgraph_info();


int make_shoot_graph();

void cleanup_graph();
#endif
