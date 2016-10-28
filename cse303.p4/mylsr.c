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
void mylsr(char **roots) {
    /* TODO: Complete this function */
  struct dirent **namelist;  
  if(roots[0]==NULL){
    //struct dirent **namelist;
    int i,n;
    n = scandir(".",&namelist,0,alphasort);
    if(n<0) perror("scandir error");
    else {
      for(i=0;i<n;i++) // print everything first 
	printf("%s\n",namelist[i]->d_name);
      for(i=0;i<n;i++){ // find subdirectory and hard link
	char * dirname = namelist[i]->d_name;
	if(namelist[i]->d_type == DT_DIR){
	  printf("Directory\n");
	  struct stat fileStat;
	  if(stat(dirname,&fileStat)<0){
	    printf("error");
	    exit(0);
	  }
	  if(fileStat.st_nlink > 2){
	    // hard link
	    printf("hard link\n");
	  } else {
	    char ** root = malloc(2*sizeof(char *));
	    root[0]= malloc(strlen(dirname));
	    strcpy(root[0],dirname);
	    mylsr(root);
	    free(root[0]);
	    free(root);
	  }
	}
	free(namelist[i]);
      }      
    }
    
  }
  else{
    int i,n,j=0;
    //    struct dirent **namelist;
    while(roots[i]){      
      //struct dirent **namelist;
      n = scandir(roots[i],&namelist,0,alphasort);
      if(n<0) printf("%s\n",roots[i]);
      else if(n==0) printf("wrong path");
      else{
	for(j=0;j<n;j++){
	  printf("%s\n",namelist[j]->d_name);
	}
	for(j=0;j<n;j++){
	  char *dirname = namelist[j]->d_name;
	  if(namelist[j]->d_type == DT_DIR){
	    struct stat fileStat;
	    if(stat(dirname,&fileStat)<0){
	      printf("error\n");
	      exit(0);
	    }
	    if(fileStat.st_nlink>2){
	      printf("hardlink\n"); // ignore
	    } else {
	      char ** root = malloc(2*sizeof(char *));
	      root[0] = malloc(strlen(dirname));
	      strcpy(root[0],dirname);
	      mylsr(root);
	      free(root[0]);
	      free(root);
	    }
	  }	 
	  free(namelist[j]);
	}
      }
      i++;
      // if(n>0) free(namelist);
    }
  }
  free(namelist);
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

    /* run a student name check */
    check_student(argv[0]);

    /* parse the command-line options.  For this program, we only support  */
    /* the parameterless 'h' option, for getting help on program usage. */
    /* TODO: make sure to handle any other arguments specified by the assignment */
    while ((opt = getopt(argc, argv, "hR")) != -1) {
        switch(opt) {
          case 'h': help(argv[0]); break;
        }
    }

    /* TODO: fix this invocation */
    // mylsr(NULL);
    char ** path = malloc((argc-1)*sizeof(char *));
    int i = 2;
    if(argc<2){
      printf("wrong command\n");
      exit(0);
      }
    for(;i<argc;i++){
      path[i-2]=malloc(strlen(argv[i]));
      strcpy(path[i-2],argv[i]);
    }
    mylsr(path);
    for(i=0;i<argc-2;i++) free(path[i]);
    free(path);
    
    
    //    printf("%s\n",argv[1]);
}
