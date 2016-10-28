#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "support.h"
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

/**
    test this one by 
    

    ./obj64/myrgrep test path
    

*/

/*
 * myls() - produce the appropriate directory listing(s)
 */
void hardlink(char ** list, char * par_path, int n);

int myCompare(const void * s1, const void *s2){
  // cast parameter to strings 
  char * str1 = *(char**)s1;
  char * str2 = *(char**)s2;
  return strcmp(str1,str2);
}

/** find key in given file */
int grep_search(char * file,char * key){
  if(file==NULL||key==NULL){
    printf("need search key and path\n");
    exit(0);
  }
  FILE * f;
  f = fopen(file,"r");
  if(!f){ // file path 
    printf("error open file\n");
    exit(0);
  }
  char buffer[1024];
  while(fgets(buffer,1024,f)){
    if(strstr(buffer,key)){  // search for string
        printf("%s\n",file);
        fclose(f);
        return 1;
    }
  }
  fclose(f);
  return 0;
}

/** process the given path */
void read_path(char * path,char * par_path,char * key){
  if(path[0]=='.'){ 
    return;
  }
  char * newPath = malloc(strlen(par_path)+strlen(path)+2);
  strcpy(newPath,par_path);
  strcat(newPath,"/");
  strcat(newPath,path);
  //printf("in reading ::::::;%s\n",newPath);
  struct dirent ** namelist;

  int i,n;
  int num_sub =0;
  int isFile = 0;
  int grep_result;
  // scan the path and save to namelist
  n = scandir(newPath,&namelist,0,alphasort);
  if(n<0) {
    DIR* directory = opendir(newPath);
    if(directory!=NULL) {
      closedir(directory);
      return;
    } 
    if(errno==ENOTDIR) isFile = 1;
    if(isFile) {
      //printf("%s\n",path);
      grep_result = grep_search(newPath,key);
      if(grep_result) {
        //printf("%s\n",path);
      }
    } else{
      printf("not a file or directory.\n");
    }
    return;
  }
  else if (n==0){
    printf("%s\t",newPath);return;
  } else {

    for(i=0;i<n;i++){
        // if directory
      if(namelist[i]->d_type==DT_DIR){
        // fo through directory

        read_path(namelist[i]->d_name,newPath,key);
        num_sub++;
      } else {
        //printf("namelist:%s\n",namelist[i]->d_name);
        char * filepath = malloc(strlen(newPath)+strlen(namelist[i]->d_name)+2);
        strcpy(filepath,newPath);
        strcat(filepath,"/");
        strcat(filepath,namelist[i]->d_name);
        //printf("path:%s\n",filepath);
        grep_search(filepath,key);
      }
      free(namelist[i]);
    }
    //qsort(list_file,n,sizeof(char*),myCompare);

  }
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
      //read_path(newPath);
    }
  }
}

/*
 * myrgrep() - find files (recursively) with matching pattern
 */
void myrgrep(char *pattern, char *roots) {
    /* TODO: Complete this function */
    
        read_path(roots,".",pattern);
    

}


/*
 * help() - Print a help message.
 */
void help(char *progname) {
    printf("Usage: %s [OPTIONS] [PATTERN] [FILE]...\n", progname);
    printf("List the names of the FILEs (the current directory by default)\n");
    printf("whose contents contain PATTERN.\n");
    printf("The search should be recursive into subdirectories of any FILE\n");
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
    /* TODO: make sure to handle any other arguments specified by the */
    /*       assignment */
    while ((opt = getopt(argc, argv, "h")) != -1) {
        switch(opt) {
          case 'h': help(argv[0]); break;
        }
    }

    if(argc != 3) printf("Wrong command.\n");  
    else myrgrep(argv[1],argv[2]);

}
