#include "shoot.h"

static int number_of_teams;
static team_node_ptr teams;
static shoot_subgraph_ptr subgraphs;

// This ugly concept is necessary to achieve my ideal architecture and prevent 
// a large useless global data structure from hanging around
static int _check_coach_name(coach_id_map_llist * first_coach, char * coach_name)
{

  if (first_coach->coach_name == NULL)
  {
    first_coach->coach_name = coach_name;
    first_coach->coach_id = 0;
    first_coach->next = NULL;
    return 0;
  }

  coach_id_map_llist * current_coach = first_coach;

  do
  {
    if(!strcmp(current_coach->coach_name, coach_name))
    {
      return current_coach->coach_id;
    }
    
  } while (current_coach->next && (current_coach = current_coach->next));

  // Never got a correct comparison so add a new coach to the end of the list
  coach_id_map_llist * new_coach = malloc(sizeof(coach_id_map_llist));
  current_coach->next = new_coach;
  new_coach->coach_name = coach_name;
  new_coach->coach_id = current_coach->coach_id + 1;
  new_coach->next = NULL;

  return new_coach->coach_id;

}


static void _free_coach_linked_list(coach_id_map_llist * first_coach)
{
  coach_id_map_llist * next_coach = first_coach->next;
  while(next_coach)
  {
    free(first_coach);
    first_coach = next_coach;
    next_coach = first_coach->next;
  }

  free(first_coach);

}

static int _get_team_level(char * team_level_name)
{
  if(!strcmp("Elite", team_level_name))
    return 1;
  else if(!strcmp("Premier", team_level_name))
    return 2;
  else if(!strcmp("Excel", team_level_name))
    return 3;
  else if(!strcmp("Gold", team_level_name))
    return 4;
  else if(!strcmp("Silver", team_level_name))
    return 5;

  if(!strcmp("Red", team_level_name))
    return 1;
  else if(!strcmp("White", team_level_name))
    return 2;
  else if(!strcmp("Blue", team_level_name))
    return 3;

  fprintf(stderr, "No identifiable level for: %s\n", team_level_name);
  return -1;

}


int read_entries(FILE * input_file)
{
  char temp_team_name[50];
  char temp_team_level[25];
  char transfer_coach[50];
  char transfer_coach_last[25];
  int counter = 0;
  char trash[25];

  fscanf(input_file, "%s", trash);
  // First count the number of entries
  while((fscanf(input_file, "%s%s", temp_team_name, temp_team_level ) != EOF))
  {
    if(fscanf(input_file, "%s%s", transfer_coach, transfer_coach_last) == EOF)
    {
      printf("Error...something weird happened!\n");
      exit(1);
    }

    ++counter;
    //strcat(transfer_coach, " ");
    //strcat(transfer_coach, transfer_coach_last);
    //printf("%s\t%s\n", temp_team_name, transfer_coach);
  }

  if(counter <= 0)
  {
    fprintf(stderr, "Error: counter is 0 or lower, incompatable file!\n");
    exit(1);
  }

  // Rewind to the beginning of the file
  rewind(input_file);
  fscanf(input_file, "%s", trash);

  //Dynamically allocate space for the number of teams
  number_of_teams = counter;
  //teams = malloc(number_of_teams * sizeof(team_node));
  //teams = malloc(sizeof(team_node));

  counter = 0;
  coach_id_map_llist * first_coach = malloc(sizeof(coach_id_map_llist));
  first_coach->coach_name = NULL;

  while((fscanf(input_file, "%s%s", temp_team_name, temp_team_level) != EOF))
  {
    if(fscanf(input_file, "%s%s", transfer_coach, transfer_coach_last) == EOF)
    {
      printf("Error...something weird happened!\n");
      exit(1);
    }


    strcat(transfer_coach, " ");
    strcat(transfer_coach, transfer_coach_last);
    team_node_ptr curr_team = teams;
    while(curr_team && curr_team->next)
      curr_team = curr_team->next;


    if(!curr_team)
    {
      curr_team = malloc(sizeof(team_node));
      if(!curr_team)
      {
        fprintf(stderr, "Error: could not allocate team node!\n");
        exit(1);
      }
      teams = curr_team;
    } 
    else
    {
      curr_team->next = malloc(sizeof(team_node));
      if(!curr_team->next)
      {
        fprintf(stderr, "Error: could not allocate team node!\n");
        exit(1);
      }
      curr_team = curr_team->next;
    }

    curr_team->team_age_group = strdup(temp_team_name);
    curr_team->team_level_name = strdup(temp_team_level);
    curr_team->coach_name = strdup(transfer_coach);
    curr_team->team_id = counter;
    curr_team->number_of_edges = 0;
    curr_team->edges = NULL;
    curr_team->team_level = _get_team_level(temp_team_level);

    int ret_coach_id = _check_coach_name(first_coach, curr_team->coach_name);
    curr_team->coach_id = ret_coach_id;
    //Set up the next node
    curr_team->next = NULL;

    ++counter;
  }

  //Cleaning up here...
  _free_coach_linked_list(first_coach);


  return 1;
}


int make_new_field_space(char * name, int number_of_teams, enum time_slot what_time, unsigned int which_days)
{
  
  if(!create_field(name, number_of_teams, what_time, which_days))
  {
    fprintf(stderr, "Could not create field %s\n", name);
    return 0;
  }

  return 1;
}


void print_team_node_info()
{
  team_node_ptr curr_team = teams;
  while(curr_team)
  {
    printf("Team Name: %s %s\n", curr_team->team_age_group, curr_team->team_level_name);
    printf("Coach's Name: %s\n", curr_team->coach_name);
    printf("Team ID: %d\n", curr_team->team_id);
    printf("Team Level: %d\n", curr_team->team_level);
    printf("Coach ID: %d\n", curr_team->coach_id);
    printf("*****************************\n");
    printf("Number of edges: %d\n", curr_team->number_of_edges);
    for(int j = 0; j < curr_team->number_of_edges; ++j)
    {
      //We have to follow the pointers to get the right edge
      shoot_edge_ptr curr_edge = curr_team->edges;
      for(int k = 0; k < j; ++k)
      {
        curr_edge = curr_edge->next;
      }
      printf("Edge to: %d\t", curr_edge->team_id);
      printf("Confirmed: ");
      team_node_ptr conf_node = teams;
      for(int z = 0; z < curr_edge->team_id; ++z)
        conf_node = conf_node->next;

      if(conf_node == curr_edge->other_node)
      {
        printf("Yes\n");
      }
      else
        printf("No\n");
    }
    printf("*******************************\n\n");
    curr_team = curr_team->next;
  }

}


void print_graph_representation()
{
  FILE * output = NULL;
  output = fopen("the_graph.dot", "w");
  if(!output)
  {
    fprintf(stderr, "Error: could not open the_graph.dot!\n");
    return;
  }

  fprintf(output, "digraph graph_representation {\n");
  fprintf(output, "  edge [dir=none]\n");

  team_node_ptr curr_node = teams;
  while(curr_node)
  {
    if(curr_node->number_of_edges == 0)
    {
      fprintf(output, "  %d\n", curr_node->team_id);
    }
    else
    {
      for(int j = curr_node->number_of_edges; j > 0; --j)
      {
        shoot_edge_ptr curr_edge = curr_node->edges;
        int k = 1;
        while(k < j)
        {
          curr_edge = curr_edge->next;
          ++k;
        }
        
        if(curr_edge->team_id > curr_node->team_id)
        {
          fprintf(output, "  %d->%d", curr_node->team_id, curr_edge->team_id);
          if(curr_edge->hard_edge)
            fprintf(output, "  [color=red]");
          fprintf(output, "\n");
        }
      }
    }
    curr_node = curr_node->next;
  }

  fprintf(output, "}");
  fclose(output);
}


void print_subgraph_info()
{
  shoot_subgraph_ptr curr_subgraph = subgraphs;
  int counter = 0;
  while(curr_subgraph)
  {
    
    printf("Printing info for subgraph: %d\n", ++counter);
    printf("Number of nodes: %d\n", curr_subgraph->number_of_nodes);
    printf("Teams involved: ");
    team_node_ptr curr_team = curr_subgraph->teams;
    while(curr_team)
    {
      printf("%d ", curr_team->team_id);
      curr_team = curr_team->next;
    }
    printf("\n\n");
    curr_subgraph = curr_subgraph->next;
  }
}


void print_field_info()
{
  print_internal_field_info(); 
}

// Return 1 on success, 0 on failure.  We are placeing on the source node.
static int _set_edge_on_node(int dest_offset, int source_offset, unsigned int hard_edge)
{
  shoot_edge_ptr new_edge = malloc(sizeof(shoot_edge));
  if(!new_edge)
  {
    fprintf(stderr, "Error: ran out of memory while allocating edge!\n");
    return 0;
  }

  //Get the correct nodes
  team_node_ptr source_node = teams;
  for(int i = 0; i < source_offset; ++i)
    source_node = source_node->next;

  team_node_ptr dest_node = teams;
  for(int i = 0; i < dest_offset; ++i)
    dest_node = dest_node->next;

  // We are setting the new_edge to the correct place in the if statements here
  if(source_node->number_of_edges == 0)
  {
    source_node->edges = new_edge;
  }
  else
  {
    shoot_edge_ptr temp_edge = source_node->edges;
    for(int j = 0; j < (source_node->number_of_edges - 1); ++j)
    {
      temp_edge = temp_edge->next;
    }
    if(temp_edge->next)
    {
      fprintf(stderr, "Error: set edge on node algorithm failing to find last edge!\n");
      return 0;
    }
    temp_edge->next = new_edge;
  }

  source_node->number_of_edges++;

  new_edge->team_id = dest_offset;
  new_edge->other_node = dest_node;
  new_edge->hard_edge = hard_edge;
  new_edge->next = NULL;
  return 1;
}

static team_node_ptr _find_earliest_coach(int coach_id)
{
  team_node_ptr curr_team = teams;
  for(int i = 0; i < coach_id; ++i)
    curr_team = curr_team->next;

  if(curr_team->coach_id == coach_id)
    return curr_team;
  while(curr_team->coach_id != coach_id)
  {
    curr_team = curr_team->next;
  }
  return curr_team;

}
    
// 0 on failure, 1 on success
static int _make_edges_by_coach()
{
  int next_coach_id = 0;
  team_node_ptr curr_team = teams;
  while(curr_team)
  {
    if(curr_team->coach_id < next_coach_id)
    {
      team_node_ptr earliest_coach = _find_earliest_coach(curr_team->coach_id);
      
      if(earliest_coach->number_of_edges > 0)
      {
        shoot_edge_ptr current_edge = earliest_coach->edges;
        while(current_edge)
        {
          int which_node = current_edge->team_id;
          if(!_set_edge_on_node(curr_team->team_id, which_node, 1))
            return 0;
          if(!_set_edge_on_node(which_node, curr_team->team_id, 1))
            return 0;

          current_edge = current_edge->next;

        }
      }

      if(!_set_edge_on_node(curr_team->team_id, earliest_coach->team_id, 1))
        return 0;
      if(!_set_edge_on_node(earliest_coach->team_id, curr_team->team_id, 1))
        return 0;

    } 
    else
      next_coach_id++;

    curr_team = curr_team->next;
  }
  return 1;
}


// Right now we're going to take advantage of the fact that 
// we know what the list format is going to look like.  In the 
// future we will probably have to add some hash map to make the
// library more flexible
static int _make_edges_by_level()
{
  team_node_ptr curr_team = teams;
  while(curr_team)
  {
    team_node_ptr next_team = curr_team->next;

    while(next_team && (!(strncmp(next_team->team_age_group, curr_team->team_age_group, 3))))
    {
      if(( next_team->team_level - curr_team->team_level) > 2)
      {
        //We need to make sure their isn't already an edge between the two teams
        int should_we_add = 1;
        for(int j = curr_team->number_of_edges; j > 0; j--)
        {
          shoot_edge_ptr curr_edge = curr_team->edges;
          int k = 1;
          while(k != j)
          {
            curr_edge = curr_edge->next;
            ++k;
          }
          if(!curr_edge)
          {
            fprintf(stderr, "Error: current edge is invalid when trying to check for duplicates in level edging!\n");
            return 0;
          }
          if(curr_edge->team_id == next_team->team_id)
          {
            should_we_add = 0;
            break;
          }
        }
        // End check

        if(should_we_add)
        {
          if(!_set_edge_on_node(curr_team->team_id, next_team->team_id, 0))
             return 0;
          if(!_set_edge_on_node(next_team->team_id, curr_team->team_id, 0))
             return 0;
        }
      }
      next_team = next_team->next;
    }
    curr_team = curr_team->next;

  }

  return 1;
}



int make_shoot_graph()
{
  if(!_make_edges_by_coach())
  {
    fprintf(stderr, "Error: makeing edges for coaches failed!\n");
    exit(1);
  }

  if(!_make_edges_by_level())
  {
    fprintf(stderr, "Error: makeing edges by level failed!\n");
    exit(1);
  }

  subgraphs = make_subgraphs(teams);

  return 1;
}


// 1 on success, 0 on failure
void cleanup_graph()
{
  team_node_ptr curr_team = teams;
  while(curr_team)
  {
    // Free the edges
    for (int j = curr_team->number_of_edges; j > 0; j--)
    {
      shoot_edge_ptr curr_edge = curr_team->edges;
      for(int k = 1; k < j; ++k)
      {
        curr_edge = curr_edge->next;
      }
      free(curr_edge);
    }
    //Free the names
    free(curr_team->team_age_group);
    free(curr_team->team_level_name);
    free(curr_team->coach_name);
    
    team_node_ptr temp_ptr = curr_team;
    curr_team = curr_team->next;
    free(temp_ptr);
    
  }

  shoot_subgraph_ptr curr_graph = subgraphs;
  while(curr_graph)
  {
    //Free the inner nodes
    team_node_ptr temp_ptr = curr_graph->teams;
    while(temp_ptr)
    {
      team_node_ptr temp_ptr2 = temp_ptr->next;
      free(temp_ptr);
      temp_ptr = temp_ptr2;
    }

    //Free the outer subgraph
    shoot_subgraph_ptr temp = curr_graph->next;
    free(curr_graph);
    curr_graph = temp;

  }
    

}
