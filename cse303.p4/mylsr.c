#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "support.h"

/*
 * mylsr() - produce the appropriate directory listing(s)
 */
void mylsr(char **roots) {
    /* TODO: Complete this function */
    struct dirent **namelist;
    if(roots[0]==NULL){
        int i,n;
        n=scandir(".",&namelist,0,alphasort);
        if(n<0){
            perror("scandir");
        } 
        else{
           for(i=0;i<n;i++){
                printf("%s\n",namelist[i]->d_name); 
                free(namelist[i]);
            }
        }
        free(namelist);
    } else {
        int i=0;
        while (roots[i]){
        int c,n;
        n = scandir(roots[i],&namelist,0,alphasort);
        if(n<0){
            printf("%s\n",roots[i]);
        } else if(n==0){
            printf("wrong");
        } else {
            for(int j=0;j<n;j++){
                printf("%s\n",namelist[j]->d_name);
                free(namelist[j]);
            }
        }
        if(n>0)free(namelist);
        i++;
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
    mylsr(NULL);
}
