#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "support.h"
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <errno.h>

/**

    test this one by 


    ./obj64/mylsl -l

    ./obj64/mylsl -l path
    

    test in mac terminal 

*/


/*
 * mylsl() - produce the appropriate directory listing(s)
 */

// for print line -1
static int line = 0;
// for number of path
static int num=0;

// compare function
int myCompare(const void * s1, const void *s2){
  int comp = 0;
  // cast parameter to strings 
  char * str1 = *(char**)s1;
  char * str2 = *(char**)s2;
  comp = strcmp(str1,str2);
  return comp;
}

/** print file permission per file */
void print_permission(char * link){
    struct stat fileStat;
    //printf("total: %4lld\n",fileStat.st_blocks);
    if(stat(link,&fileStat)<0){
        printf("file error.\n");exit(0);
    }
    // rwx permission
    printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
    printf("  ");

    // print links 
    printf("%3d  ",fileStat.st_nlink);

    // owner name and group name
    struct passwd *pw = getpwuid(fileStat.st_uid);
    struct group  *gr = getgrgid(fileStat.st_gid);
    printf("%10s ",pw->pw_name);
    printf("%7s ",gr->gr_name);

    // file size
    printf("%6lld ",fileStat.st_size);

    // print time
    char date[20];
    strftime(date, 20, "%b %d %H:%M ", localtime(&(fileStat.st_ctime)));
    printf("%14s ",date);
}

/** print the list of files */
void print_ls(char ** list,char * par_path, int n){
  //printf("here in print_ls\n");
  int i;
  int block_size = 0;
  struct stat fileStat;
    //printf("total: %4lld\n",fileStat.st_blocks);
  for(i=0;i<n;i++){

    // concatenate file path
    char * link = list[i];
    char * newPath = malloc(strlen(par_path)+strlen(link)+2);
    strcpy(newPath,par_path);
    strcat(newPath,"/");
    strcat(newPath,link);
   
    // sum up block size
    if(list[i][0]!='.'){
        //printf("list pass to block size:%s\n",newPath);
        if(stat(newPath,&fileStat)<0){
            printf("file error\n");exit(0);
        }
        block_size+=fileStat.st_blocks;
    }
  }

  printf("total %-4d\n",block_size);

  // print permission 
  for(i=0;i<n;i++){
    char * link = list[i];
    char * newPath = malloc(strlen(par_path)+strlen(link)+2);
    strcpy(newPath,par_path);
    strcat(newPath,"/");
    strcat(newPath,link);

    if(list[i][0]!='.'){ // if not a hidden file
      //printf("list pass to permission:%s\n",newPath);
      print_permission(newPath);  
      printf("%s\n",newPath); // new line   
    }
  }
  printf("\n");
}

/** process the given path */
void read_path(char * path){
  //printf("in reading ::::::;%s\n",path);
  struct dirent ** namelist;
  char ** list_file; 
  int i,n;
  int isFile = 0;
  // scan the path and save to namelist
  n = scandir(path,&namelist,0,alphasort);
  if(n<0) {
    DIR* directory = opendir(path);
    if(directory!=NULL) {
      closedir(directory);
      return;
    } 
    if(errno==ENOTDIR) isFile = 1;
    if(isFile) {
      printf("%s\n",path);
      return;
    } else{
      printf("not a file.\n");
    }
    return;
  }
  else if (n==0){
    // it's a file name
    printf("%s\t",path);return;
  } else {
    // allocate memory for file list
    list_file = malloc(n*sizeof(char*));
    for(i=0;i<n;i++){
      // save to list-file
      list_file[i]=malloc(strlen(namelist[i]->d_name)+1);
      strcpy(list_file[i],namelist[i]->d_name);
      //printf("%s\n",list_file[i]);
      free(namelist[i]);
    }
    // sort as /bin/ls order 
    qsort(list_file,n,sizeof(char*),myCompare);
    // printf("after sort\n");
    if(num>1) printf("%s:\n",path);
    print_ls(list_file,path,n);
  }
  for(i=0;i<n;i++) free(list_file[i]);
  free(list_file);
  free(namelist);
}

/** myls */
void mylsl(char **roots) {
  /* TODO: Complete this function */
  struct dirent **namelist;
  // if current directory 
  //if(roots[0]==NULL) printf("NULL path\n");
  //printf("current path:%s\n",roots[0]);
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
    printf("Use a long listing format\n");
    printf("Behavior should mirror /bin/ls -1 -l\n");
}

/*
 * main() - The main routine parses arguments and dispatches to the
 *          task-specific code.
 */
int main(int argc, char **argv) {
    /* for getopt */
    long opt;
    int longlist=0;
    /* run a student name check */
    check_student(argv[0]);

    /* parse the command-line options.  For this program, we only support  */
    /* the parameterless 'h' option, for getting help on program usage. */
    /* TODO: make sure to handle any other arguments specified by the assignment */
    while ((opt = getopt(argc, argv, "h1l")) != -1) {
        switch(opt) {
          case 'h': help(argv[0]); break;
          case '1': line =1; break;
          case 'l': longlist=1; break;
        }
    }
    if(!longlist){
      printf("should use -l command\n");
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
      //printf("%s\n",path[i-optind]);
    }
    num = argc - optind; // number of path
    // printf("%i\n",num);
    mylsl(path);
    // free
    for(i=0;i<argc-optind;i++)
      free(path[i]);
    free(path);
}
