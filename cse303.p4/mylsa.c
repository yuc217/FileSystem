#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "support.h"
#include <dirent.h>
/*
 * mylsa() - produce the appropriate directory listing(s)
 */

// for print line -1
static int line = 0;
// for number of path
static int num=0;
/*
 * myls() - produce the appropriate directory listing(s)
 */
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
    // don't check for hidden file
    if(line) printf("%s\n",list[i]); // new line 
    else printf("%s\t ",list[i]);     
  }
  if(!line) printf("\n"); // new line at the end
  printf("\n");
}

/** process the given path */
void read_path(char * path){
  //printf("in reading ::::::;%s\n",path);
  struct dirent ** namelist;
  char ** list_file; 
  int i,n;
  // scan the path and save to namelist
  n = scandir(path,&namelist,0,alphasort);
  if(n<0) perror("scandir error");
  else if (n==0){
    // it's a file name
    printf("%s\t",path);
  } else {
    // allocate memory for file list
    list_file = malloc(n*sizeof(char*));
    for(i=0;i<n;i++){
      // save to list-file
      list_file[i]=malloc(strlen(namelist[i]->d_name+1));
      strcpy(list_file[i],namelist[i]->d_name);
      //printf("%s\n",list_file[i]);
      free(namelist[i]);
    }
    // sort as /bin/ls order 
    qsort(list_file,n,sizeof(char*),myCompare);
    // printf("after sort\n");
    if(num>1) printf("%s:\n",path);
    print_ls(list_file,n);
  }
  for(i=0;i<n;i++) free(list_file[i]);
  free(list_file);
  free(namelist);
}

/** myls */
void mylsa(char **roots) {
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
    printf("Do not ignore entries starting with .\n");
    printf("Behavior should mirror /bin/ls -1 -a\n");
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
    while ((opt = getopt(argc, argv, "h1")) != -1) {
        switch(opt) {
          case 'h': help(argv[0]); break;
          case '1':line =1; break;
        }
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
    mylsa(path);
    // free
    for(i=0;i<argc-optind;i++)
      free(path[i]);
    free(path);
}
