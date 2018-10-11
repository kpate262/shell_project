#include "stdlib.h"
#include "stdio.h"
#include <unistd.h>
#include "string.h"

/*void forktwo(char **args1, char **args2){
  int pid = fork();
  if(pid == 0){
    pid = fork();
    if(pid == 0){
      execv(args1[0], args1);
    }
    else{
      //execv(args1[0], args1);
      int status2;
      wait(&status2);
      execv(args2[0], args2);

    }
  }else{
    int status;
    wait(&status);
  }
}
*/

void forking(char **args1){
    int pid = fork();

    if(pid == 0){
      execv(args1[0], args1);

    }else{
      int status;
      wait(&status);
    }
}


int main()
{
  char *line;
  char **args1 = (char**)malloc(sizeof(char*)*20);
  char **args2 = (char**)malloc(sizeof(char*)*20);
  char prompt[] = "CS361 >";

  printf("%s", prompt);
  line = (char*)malloc(sizeof(char)*500);
  fgets(line, 500, stdin);
  char *commands = strtok(line, " \n");

  int i; int com = 0;

 /* while(1){
    printf("%s", prompt);*/
    i = 0;
    while(commands){
      if(strcmp(commands, ";") == 0){
          com = 1;
          args1[i] = (char*)0;
          forking(args1);
        //  printf("2 here");
          i = 0;
          commands = strtok(NULL, " \n");
        }
      if(com == 0){
        args1[i] = (char*)malloc(sizeof(char)*100);
        strcpy(args1[i], commands);
        i++;

        if(strncmp(args1[0], "exit",4) == 0){
     	   printf("im done");
     	   exit(0);
      	}
      }
      else{

      	args2[i] = (char*)malloc(sizeof(char)*100);
        strcpy(args2[i], commands);
        i++;

        if(strncmp(args2[0], "exit",4) == 0){
           printf("im done");
           exit(0);
        }
      }
      commands = strtok(NULL, " \n");
    }
      if(com == 0){
          args1[i] = (char*)0;
          //printf("not\n" );
	        forking(args1);
      }
      else{
      	  args2[i] = (char*)0;
          //printf(">>%d", i);
      	  forking(args2);
      }
    /*line = malloc(sizeof(char)*500);
    fgets(line, 500, stdin);
    commands = strtok(line, " \n");
    }*/

}
