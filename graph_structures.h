#ifndef _GRAPH_DATA_STRUCTURES_H
#define _GRAPH_DATA_STRUCTURES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "internal_graph_structures.h"



int read_entries(FILE * input_file);

void print_team_node_info();

void print_graph_representation();

int make_shoot_subgraphs();

void cleanup_graph();
#endif
