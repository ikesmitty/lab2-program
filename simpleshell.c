/************************************************** 
 * CIS 452
 * Lab 2 - Programming Assignment - A simple shell
 * Authors: Isaac Smith, Bob Heine
**************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

#define BUFFER_LEN 1024

int main(){
  
    struct rusage usage;
  
    char line[BUFFER_LEN];  //get command line
    char* argv[100];        //user command
    char* path= "/bin/";    //set path at bin
    char progpath[20];      //full file path
    int argc;               //arg count
    int counter = 1;
    int status = 1;
    int prev_switches = 0;
    long prev_sec = 0.0;
    long prev_usec = 0.0;
    
while(1){

    //printf("My shell>> ");                    //print shell prompt
    printf("[%d]-> ", counter); 
    
    if(!fgets(line, BUFFER_LEN, stdin))
	break;
    size_t length = strlen(line);
    if (line[length - 1] == '\n')
	line[length - 1] = '\0';
    
    if(strcmp(line, "exit")==0){            //check if command is exit
        break;
    }

    char *token;                  //split command into separate strings
    token = strtok(line," ");
    int i=0;
    while(token!=NULL){
        argv[i]=token;      
        token = strtok(NULL," ");
        i++;
    }
    argv[i]=NULL;                     //set last value to NULL for execvp

    argc=i;                           //get arg count
    for(i=0; i<argc; i++){
        //printf("%s\n", argv[i]);      //print command/args
    }
    
    strcpy(progpath, path);           //copy /bin/ to file path
    strcat(progpath, argv[0]);            //add program to path

    for(i=0; i<strlen(progpath); i++){    //delete newline
        if(progpath[i]=='\n'){      
            progpath[i]='\0';
        }
    }
    
    int pid= fork();              //fork child
    
    if(pid==0){               //Child
        execvp(progpath,argv);
        fprintf(stderr, "Child process could not do execvp\n");
	_exit(status);

    }else{                    //Parent
        wait(NULL);
	if(getrusage(RUSAGE_CHILDREN, &usage)){
	  perror("getrusage");
	}	
	printf ("\nUser CPU time: %ld.%06ld\n",
          (usage.ru_utime.tv_sec - prev_sec), (usage.ru_utime.tv_usec - prev_usec));
	printf ("Involuntary Context Switches: %ld\n\n", (usage.ru_nivcsw - prev_switches));
	
	// RUSAGE_CHILDREN has cumulative data from all children of the parent loop
	// so every loop, take away the previous values.
	prev_switches = usage.ru_nivcsw;
	prev_sec = usage.ru_utime.tv_sec;
	prev_usec = usage.ru_utime.tv_usec;
    }
    counter++;		//increment counter to display
    
}

  return 0;
} 

