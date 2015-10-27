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
  new_coach->coach_id = current_coach->coach_id++;
  new_coach->next = NULL;

  return new_coach->coach_id;

}

int read_entries(FILE * input_file)
{
  char temp_team_name[50];
  char transfer_coach[50];
  char transfer_coach_last[25];
  int counter = 0;

  // First count the number of entries
  while((fscanf(input_file, "%s", temp_team_name) != EOF))
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

  while((fscanf(input_file, "%s", temp_team_name) != EOF))
  {
    if(fscanf(input_file, "%s%s", transfer_coach, transfer_coach_last) == EOF)
    {
      printf("Error...something weird happened!\n");
      exit(1);
    }

    strcat(transfer_coach, " ");
    strcat(transfer_coach, transfer_coach_last);

    teams[counter].team_name = strdup(temp_team_name);
    teams[counter].coach_name = strdup(transfer_coach);
    teams[counter].team_id = counter;

    int ret_coach_id = _check_coach_name(first_coach, teams[counter].coach_name);
    printf("%s\tCoach ID: %d\n", teams[counter].coach_name, ret_coach_id);
    teams[counter].coach_id = ret_coach_id;

    ++counter;
    //printf("%s\t%s\n", temp_team_name, transfer_coach);
  }


  return 1;
}


void print_team_node_info()
{
  return;
}
