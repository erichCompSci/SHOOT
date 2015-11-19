#ifndef _GRAPH_DATA_STRUCTURES_H
#define _GRAPH_DATA_STRUCTURES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "internal_graph_structures.h"
#include "coloring_algorithm.h"

//This data structure is for the users convienience, it is tied
//to the field space variable in the internal code.  We need to think
//of more dynamic ways to do this in the future so it's not all hardcoded.
enum time_slot { FIVE, SIX, SIX_THIRTY, SEVEN_THIRTY, EIGHT };

int read_entries(FILE * input_file);

/* This is a wrapper function for the high layer to use to access the internal
 * coloring data structures in a controlled way */
int make_new_field_space(char * name, int number_of_teams, enum time_slot what_time);


void print_team_node_info();
void print_graph_representation();
void print_subgraph_info();
void print_field_info();


int make_shoot_graph();

void cleanup_graph();
#endif
