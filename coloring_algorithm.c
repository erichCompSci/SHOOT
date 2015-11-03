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
  if(!queue_to_handle)
  {
    fprintf(stderr, "Error: could not allocate extra team node for queue!\n");
    return 0;
  }

  memcpy(queue_to_handle, curr_team, sizeof(team_node));
  queue_to_handle->next = NULL;
  
  while(queue_to_handle)
  {
    if(curr_list->number_of_nodes == 0)
    {
      curr_list->number_of_nodes++;
      curr_list->teams = queue_to_handle
    

  }
    

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

    
    _make_subgraph_list(curr_subgraph, curr_team, nodes_assigned, size_of_all_graphs);
    
    
      
    
  }
  
  


}
