#ifndef _COLORING_ALGORITHM_H
#define _COLORING_ALGORITHM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "internal_graph_structures.h"


shoot_subgraph_ptr make_subgraphs(team_node_ptr teams);

//Return 0 on failure, 1 on success
int create_field(char * name, int number_of_spaces, int time_slot);

void print_internal_field_info();



#endif
