#include "initial_page_reader.h"


void skip_element_at_current_position(FILE *fp);
void read_element_at_current_position(FILE *fp, char *result);

void get_path_from_current_line(FILE *fp, char *result);
void get_host_from_current_line(FILE *fp, char *result);

int get_initial_fd();
FILE *get_initial_fp*();


// functions defined in header
int get_initial_page_count(){
  FILE *fp = get_initial_fp();
  int count = 0;
  char buff[1024];

  while(getline(&buff, sizeof(buff), fp) != -1){
    count++;
  }

  fclose(fp);
  return count;
}

char **get_initial_hosts(){
  int count = get_initial_page_count();
  char **hosts = calloc(count, sizeof(char *));
  FILE *fp = get_initial_fp();

  for(int i = 0; i < count; i++){
    get_host_from_current_line(hosts[i]);
  }

  fclose(fp);
  return hosts;
}

char **get_initial_paths(){
  int count = get_initial_page_count();
  char **paths = calloc(count, sizeof(char *));
  FILE *fp = get_initial_fp();

  for(int i = 0; i < count; i++){
    get_path_from_current_line(fp, paths[i]);
  }

  fclose(fp);
  return paths;
}

void free_initial_pages(char **hosts, char **paths){
  int count = get_initial_page_count();
  for(int i = 0; i < count; i++){
    free(hosts[i]);
    free(paths[i]);
  }
  free(hosts);
  free(paths);
}

// helper functions
void get_host_from_current_line(FILE *fp, char *result){
  read_element_at_current_position(fp, result);
  skip_element_at_current_position(fp);
}

void get_path_from_current_line(FILE *fp, char *result){
  skip_element_at_current_position(fp);
  read_element_at_current_position(fp, result);
}

void skip_element_at_current_position(FILE *fp){
  char buff[1024];
  scanf(fp, "%s", buff);
}

void read_element_at_current_position(FILE *fp, char *result){
  char buff[1024];
  scanf(fp, "%s", buff);
  malloc(strlen(buff) + 1);
  strcpy(result, buff);
}

FILE *get_initial_fp*(){
  return fdopen(get_initial_fd(), "rw");
}

int get_initial_fd(){
  int fd = open(INIT_PAGE_FILE, O_CREAT | O_WRONLY, 0600);

  if(fd < 0){
    perror("File Open Error");
    exit(1);
  } 
  return fd;
}
