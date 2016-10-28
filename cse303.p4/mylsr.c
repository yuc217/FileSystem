
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "support.h"
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

/*
 * mylsr() - produce the appropriate directory listing(s)
 */

// for print line -1
static int line = 0;
// for number of path
static int num=0;

/*
 * myls() - produce the appropriate directory listing(s)
 */
void hardlink(char ** list, char * par_path, int n);

int myCompare(const void * s1, const void *s2){
  int comp = 0;
  // cast parameter to strings 
  char * str1 = *(char**)s1;
  char * str2 = *(char**)s2;

  comp = strcmp(str1,str2);

  return comp;
}

/** print the list of files */
void print_ls(char ** list,int n){
  int i;
  for(i=0;i<n;i++){
    if(list[i][0]!='.'){ // if not a hidden file
      if(line) printf("%s\n",list[i]); // new line 
      else printf("%s\t ",list[i]); 
    }
  }
  if(!line) printf("\n"); // new line at the end
  printf("\n");
}

/** process the given path */
void read_path(char * path){
  //printf("in reading ::::::;%s\n",path);
  struct dirent ** namelist;
  char ** list_file; 
  char ** list_sub;
  int i,n;
  int num_sub =0;
  // scan the path and save to namelist
  n = scandir(path,&namelist,0,alphasort);
  if(n<0) perror("scandir error");
  else if (n==0){
    // it's a file name
    printf("%s\t",path);
  } else {
    // allocate memory for file list
    list_file = malloc(n*sizeof(char*));
    list_sub = malloc(n*sizeof(char*));
    for(i=0;i<n;i++){
      if(namelist[i]->d_type==DT_DIR&&namelist[i]->d_name[0]!='.'){
        // is a directory 
        // save to a new list
        list_sub[num_sub]=malloc(strlen(namelist[i]->d_name)+1);
        strcpy(list_sub[num_sub],namelist[i]->d_name);
        //printf("sub:%s\n",list_sub[num_sub]);
        num_sub++;
      }
      // save to list-file
      list_file[i]=malloc(strlen(namelist[i]->d_name)+1);
      strcpy(list_file[i],namelist[i]->d_name);
      //printf("%s\n",list_file[i]);
      free(namelist[i]);
    }
    // sort as /bin/ls order 
    qsort(list_file,n,sizeof(char*),myCompare);
    // printf("after sort\n");
    printf("%s:\n",path);
    print_ls(list_file,n);
    // find subdirectory and print its content
    if(num_sub>0) hardlink(list_sub,path,num_sub);
  }
  for(i=0;i<n;i++) free(list_file[i]);
  for(i=0;i<num_sub;i++) free(list_sub[i]);
  free(list_file);
  free(list_sub);
  free(namelist);
}

/** process given dir and find subdirectory */
void hardlink(char ** list,char* par_path, int n){
  int i=0;
  for(;i<n;i++){
    // concatenate so that the path is readable 
    char * link = list[i];
    char * newPath = malloc(strlen(par_path)+strlen(link)+2);
    strcpy(newPath,par_path);
    strcat(newPath,"/");
    strcat(newPath,link);
    //xsprintf("%s:\n",newPath);
    struct stat fileStat;
    if(stat(newPath,&fileStat)<0){
      printf("error\n");
      exit(0); // test for valid path
    }
    // check for hard links 
    if(S_ISLNK(fileStat.st_mode)){
      //printf("hardlink\n"); // ignore
    } else { // recursive reading file
      //printf("%s\n",newPath);
      read_path(newPath);
    }
  }
}

/** myls */
void mylsr(char **roots) {
  /* TODO: Complete this function */
  struct dirent **namelist;
  // if current directory 
  if(roots[0]==NULL){
    read_path("."); 
  } else{
    int i =0;
    for(;i<num;i++){
      //printf("%i:%s\n",i,roots[i]);
      read_path(roots[i]);
    }
  } 
}

/*
 * help() - Print a help message.
 */
void help(char *progname) {
    printf("Usage: %s [FILE]...\n", progname);
    printf("List information about the FILEs (the current directory by default).\n");
    printf("List subdirectories recursively\n");
    printf("Behavior should mirror /bin/ls -1 -r\n");
}

/*
 * main() - The main routine parses arguments and dispatches to the
 *          task-specific code.
 */
int main(int argc, char **argv) {
    /* for getopt */
    long opt;
    int rec=0;
    /* run a student name check */
    check_student(argv[0]);

    /* parse the command-line options.  For this program, we only support  */
    /* the parameterless 'h' option, for getting help on program usage. */
    /* TODO: make sure to handle any other arguments specified by the assignment */
    while ((opt = getopt(argc, argv, "h1R")) != -1) {
        switch(opt) {
          case 'h': help(argv[0]); break;
          case '1': line =1; break;
          case 'R': rec=1; break;
        }
    }
    if(!rec){
      printf("should use -R command\n");
      exit(0);
    }
    /* TODO: fix this invocation */
    //myls(NULL);    
    char ** path = malloc((argc-optind)*sizeof(char*));
    int i =optind;
    //printf("%i\n",optind);
    // store argv to roots 
    for(;i<argc;i++){
      path[i-optind]=malloc(strlen(argv[i])+1);
      strcpy(path[i-optind],argv[i]);
    }
    num = argc - optind; // number of path
    // printf("%i\n",num);
    mylsr(path);
    // free
    for(i=0;i<argc-optind;i++)
      free(path[i]);
    free(path);
}
