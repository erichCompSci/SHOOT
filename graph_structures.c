#include "graph_structures.h"

static int number_of_teams;
static team_node_ptr teams;

typedef struct _coach_id_map_llist coach_id_map_llist;

struct _coach_id_map_llist
{
  char * coach_name;
  int coach_id;
  coach_id_map_llist * next;
};

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
  teams = malloc(number_of_teams * sizeof(team_node));

  if(!teams)
  {
    fprintf(stderr, "Error: malloc failed to initialize team nodes!\n");
    exit(1);
  }

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
    //strcat(temp_team_name, temp_team_level);

    teams[counter].team_age_group = strdup(temp_team_name);
    teams[counter].team_level_name = strdup(temp_team_level);
    teams[counter].coach_name = strdup(transfer_coach);
    teams[counter].team_id = counter;
    teams[counter].number_of_edges = 0;
    teams[counter].edges = NULL;
    teams[counter].team_level = _get_team_level(temp_team_level);

    int ret_coach_id = _check_coach_name(first_coach, teams[counter].coach_name);
    teams[counter].coach_id = ret_coach_id;

    ++counter;
  }

  //Cleaning up here...
  _free_coach_linked_list(first_coach);


  return 1;
}


void print_team_node_info()
{

  for(int i = 0; i < number_of_teams; ++i)
  {
    printf("Team Name: %s %s\n", teams[i].team_age_group, teams[i].team_level_name);
    printf("Coach's Name: %s\n", teams[i].coach_name);
    printf("Team ID: %d\n", teams[i].team_id);
    printf("Team Level: %d\n", teams[i].team_level);
    printf("Coach ID: %d\n", teams[i].coach_id);
    printf("*****************************\n");
    printf("Number of edges: %d\n", teams[i].number_of_edges);
    for(int j = 0; j < teams[i].number_of_edges; ++j)
    {
      //We have to follow the pointers to get the right edge
      shoot_edge_ptr curr_edge = teams[i].edges;
      for(int k = 0; k < j; ++k)
      {
        curr_edge = curr_edge->next;
      }
      printf("Edge to: %d\t", curr_edge->team_id);
      printf("Confirmed: ");
      if((teams + curr_edge->team_id) == curr_edge->other_node)
      {
        printf("Yes\n");
      }
      else
        printf("No\n");
    }
    printf("*******************************\n\n");
  }
}


// Return 1 on success, 0 on failure.  We are placeing on the source node.
static int _set_edge_on_node(int dest_offset, int source_offset)
{
  shoot_edge_ptr new_edge = malloc(sizeof(shoot_edge));
  if(!new_edge)
  {
    fprintf(stderr, "Error: ran out of memory while allocating edge!\n");
    return 0;
  }

  // We are setting the new_edge to the correct place in the if statements here
  if(teams[source_offset].number_of_edges == 0)
  {
    teams[source_offset].edges = new_edge;
  }
  else
  {
    shoot_edge_ptr temp_edge = teams[source_offset].edges;
    for(int j = 0; j < (teams[source_offset].number_of_edges - 1); ++j)
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

  teams[source_offset].number_of_edges++;

  new_edge->team_id = dest_offset;
  new_edge->other_node = teams + dest_offset;
  new_edge->next = NULL;
  return 1;
}

static int _find_earliest_coach(int coach_id)
{
  int current_id_test = teams[coach_id].coach_id;
  int current_i_value = coach_id;

  if(current_id_test == coach_id)
    return coach_id;
  while(current_id_test != coach_id)
  {
    current_i_value++;
    current_id_test = teams[current_i_value].coach_id;
  }
  return current_i_value;

}
    
// 0 on failure, 1 on success
static int _make_edges_by_coach()
{
  int next_coach_id = 0;
  for(int i = 0; i < number_of_teams; ++i)
  {
    if(teams[i].coach_id < next_coach_id)
    {
      int earliest_coach = _find_earliest_coach(teams[i].coach_id);
      
      if(teams[earliest_coach].number_of_edges > 0)
      {
        for(int j = 0; j < teams[earliest_coach].number_of_edges; ++j)
        {

          shoot_edge_ptr current_edge = teams[earliest_coach].edges + j;
          int which_node = current_edge->team_id;
          if(!_set_edge_on_node(i, which_node))
            return 0;
          if(!_set_edge_on_node(which_node, i))
            return 0;

        }
      }

      if(!_set_edge_on_node(i, earliest_coach))
        return 0;
      if(!_set_edge_on_node(earliest_coach, i))
        return 0;

    } 
    else
      next_coach_id++;
  }
  return 1;
}


// Right now we're going to take advantage of the fact that 
// we know what the list format is going to look like.  In the 
// future we will probably have to add some hash map to make the
// library more flexible
static int _make_edges_by_level()
{
  for(int i = 0; i < number_of_teams; ++i)
  {
    team_node_ptr curr_team = teams + i;
    int next_index = i + 1;
    team_node_ptr next_team = teams + next_index;

    while(next_index < number_of_teams && (!(strncmp(next_team->team_age_group, curr_team->team_age_group, 3))))
    {
      if(!next_team)
      {
        fprintf(stderr, "Error: no next team in makeing edges by level!\n");
        return 0;
      }

      if(( next_team->team_level - curr_team->team_level) > 1)
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
            should_we_add = 0;
        }
        // End check

        if(should_we_add)
        {
          if(!_set_edge_on_node(curr_team->team_id, next_team->team_id))
             return 0;
          if(!_set_edge_on_node(next_team->team_id, curr_team->team_id))
             return 0;
        }
      }
      next_index++;
      next_team = teams + next_index;
    }

  }

  return 1;
}



int make_shoot_subgraphs()
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

  return 1;
}


// 1 on success, 0 on failure
void cleanup_graph()
{
  for (int i = 0; i < number_of_teams; ++i)
  {
    // Free the edges
    team_node_ptr curr_team = teams + i;
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
    free(teams[i].team_age_group);
    free(teams[i].team_level_name);
    free(teams[i].coach_name);
    
  }
  // Free the nodes
  free(teams);

}
