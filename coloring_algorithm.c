#include "coloring_algorithm.h"

static int _get_size_of_graph(team_node_ptr teams)
{
  int total_size = 0;
  team_node_ptr curr_node = teams;
  if(!curr_node)
    return total_size;

  while(curr_node)
  {
    total_size++;
    curr_node = curr_node->next;
  }

  return total_size;
}

int _make_subgraph_list(shoot_subgraph_ptr curr_list, team_node_ptr curr_team, int * nodes_assigned, int total_nodes)
{
  team_node_ptr queue_to_handle = malloc(sizeof(team_node));
  team_node_ptr curr_unassigned = queue_to_handle;
  team_node_ptr last_in_queue;
  if(!queue_to_handle)
  {
    fprintf(stderr, "Error: could not allocate extra team node for queue!\n");
    return 0;
  }

  memcpy(queue_to_handle, curr_team, sizeof(team_node));
  queue_to_handle->next = NULL;
  
  while(curr_unassigned)
  {
    if(curr_list->number_of_nodes == 0)
    {
      curr_list->teams = curr_unassigned; 
      last_in_queue = curr_unassigned;
    }

    curr_list->number_of_nodes++;

    shoot_edge_ptr curr_edge = queue_to_handle->edges;
    while(curr_edge)
    {
      //Check to make sure the other node isn't in place already
      team_node_ptr temp_check = queue_to_handle;
      while(temp_check)
      {
        if(temp_check->team_id == curr_edge->team_id)
          break;

        temp_check = temp_check->next;
      }
      
      if(temp_check)
      {
        curr_edge = curr_edge->next;
        continue;
      }

      //If we reach this point, we must add the node to the queue
      team_node_ptr copied_object = malloc(sizeof(team_node));
      if(!copied_object)
      {
        fprintf(stderr, "Error: could not allocate team node data struct!\n");
        return 0;
      }
      memcpy(copied_object, curr_edge->other_node, sizeof(team_node));

      last_in_queue->next = copied_object;
      copied_object->next = NULL;

      last_in_queue = copied_object;
      curr_list->number_of_nodes++;

      nodes_assigned[curr_edge->team_id] = 1;

      curr_edge = curr_edge->next;
      // A little bookeeping for speed...

    }

    curr_unassigned = curr_unassigned->next;
  }
  return 1;

}


shoot_subgraph_ptr make_subgraphs(team_node_ptr teams)
{
  int size_of_all_graphs = _get_size_of_graph(teams);
  
  if(size_of_all_graphs == 0)
    return NULL;

  int * nodes_assigned = calloc(size_of_all_graphs, sizeof(int));
  if(!nodes_assigned)
  {
    fprintf(stderr, "Error: could not allocate data table for assigned nodes!\n");
    return NULL;
  }


  shoot_subgraph_ptr first_subgraph = NULL;
  for(int i = 0; i < size_of_all_graphs; ++i)
  {
    if(nodes_assigned[i])
      continue;

    if(!first_subgraph)
    {
      first_subgraph = malloc(sizeof(shoot_subgraph));
      if(!first_subgraph)
      {
        fprintf(stderr, "Error: could not allocate a subgraph data struct!\n");
        return NULL;
      }
      first_subgraph->number_of_nodes = 0;
      first_subgraph->teams = NULL;
      first_subgraph->next = NULL;
    }
    shoot_subgraph_ptr last_subgraph = first_subgraph;

    while(last_subgraph->next)
      last_subgraph = last_subgraph->next;

    shoot_subgraph_ptr curr_subgraph;
    if(last_subgraph == first_subgraph)
      curr_subgraph = last_subgraph;
    else
    {
      curr_subgraph = malloc(sizeof(shoot_subgraph));
      if(!curr_subgraph)
      {
        fprintf(stderr, "Error: could not allocate a subgraph data struct!\n");
        return NULL;
      }
      last_subgraph->next = curr_subgraph;
    }

    team_node_ptr curr_team = teams;
    int k = 0;
    while(k < i)
    {
      curr_team = curr_team->next;
      ++k;
    }

    
    if(!_make_subgraph_list(curr_subgraph, curr_team, nodes_assigned, size_of_all_graphs))
    {
      fprintf(stderr, "Error: could not make subgraph on team: %d\n", curr_team->team_id);
      return NULL;
    }

    
  }

  return first_subgraph;
  
}
