#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "support.h"
#include <dirent.h>
/*
 * myls() - produce the appropriate directory listing(s)
 */
void myls(char **roots) {
  /* TODO: Complete this function */
  struct dirent **namelist;
  if(roots[0]==NULL){
    // current directory
    int i,n;
    n=scandir(".",&namelist,0,alphasort);
    if(n<0){
      perror("scandir");
    } else{
      for(i=0;i<n;i++){
	char * filename = namelist[i]->d_name;
	if(filename[0]!='.'){
	  printf("%s\n",filename);
	}
	free(namelist[i]);
      }
    }
    free(namelist[i]);
  } else{
    int i=0;
    // using argument
    while(roots[i]){
      int c,n;
      n = scandir(roots[i],&namelist,0,alphasort);
      if(n<0){
	printf("%s\n",roots[i]);
      } else{
	for(i=0;i<n;i++){
	  char * filename = namelist[i]->d_name;
	  if(filename[0]!='.'){
	    printf("%s\n",filename);
	  }
	  free(namelist[i]);
	}
	free(namelist);
      }
      i++;
    }
  }
  
}

static int file_num = 0;

/*
 * help() - Print a help message.
 */
void help(char *progname) {
    printf("Usage: %s [FILE]...\n", progname);
    printf("List information about the FILEs (the current directory by default).\n");
    printf("Behavior should mirror /bin/ls -1\n");
}

/*
 * main() - The main routine parses arguments and dispatches to the
 *          task-specific code.
 */
int main(int argc, char **argv) {
    /* for getopt */
    long opt;

    /* run a student name check */
    check_student(argv[0]);

    /* parse the command-line options.  For this program, we only support  */
    /* the parameterless 'h' option, for getting help on program usage. */
    /* TODO: make sure to handle any other arguments specified by the assignment */
    while ((opt = getopt(argc, argv, "h")) != -1) {
        switch(opt) {
          case 'h': help(argv[0]); break;
        }
    }
    
    /* TODO: fix this invocation */
    //myls(NULL);
    char ** path = malloc((argc-1)*sizeof(char*));
    int i =1;
    for(;i<argc;i++){
      path[i-1]=malloc(strlen(argv[i]));
      // printf("%d: %s\n",i,argv[i]);
      strcpy(path[i-1],argv[i]);
    }
    myls(path);
    file_num = argc-1;
    for(i=0;i<argc-1;i++)
      free(path[i]);
    free(path);
}
