#include "stdlib.h"
#include "stdio.h"
#include <unistd.h>
#include "string.h"
#include "signal.h"
#include "wait.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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

void siginthandler() {
   write(2, "\ncaught sigint\nCS361 >", 22);
   return;
}

void sigstphandler() {
   write(2, "\ncaught sigstp\nCS361 >", 22);
   return;
}


void outputtofile(char **args1, int toOutput){
  int pid = fork();

  if(pid == 0){
    dup2(toOutput, 1);
    execv(args1[0], args1);
    exit(0);

  }else{
    int status;
    wait(&status);

    if(status == 256){
      status = 1;
    }
    printf("pid:%d status:%d\n", pid, status);
  }
}


void forking2(char **args1, char **args2){
    int pid = fork();

    if(pid == 0){
      execv(args1[0], args1);
      exit(0);

    }else{
      int pid2 = fork();
      if(pid2 == 0){
        execv(args2[0], args2);
      }
      else{
        int stat;
        wait(&stat);
        if(stat == 256){
          stat = 1;
        }

        int status;
        wait(&status);

        if(status == 256){
          status = 1;
        }
        printf("pid:%d status:%d\n", pid, status);
        printf("pid:%d status:%d\n", pid2, stat);
      }

    }
}


void forking(char **args1){
    int pid = fork();

    if(pid == 0){
      execv(args1[0], args1);
      exit(0);
    }else{
      int status;
      wait(&status);

      if(status == 256){
        status = 1;
      }
      printf("pid:%d status:%d\n", pid, status);
    }
}


int main()
{
  char *line;
  char **args1 = (char**)malloc(sizeof(char*)*20);
  char **args2 = (char**)malloc(sizeof(char*)*20);
  char prompt[] = "CS361 >";
  signal(SIGINT, siginthandler);//used https://www.tutorialspoint.com/c_standard_library/c_function_signal.htm
                             // as reference
  signal(SIGTSTP, sigstphandler);
  printf("%s", prompt);
  line = (char*)malloc(sizeof(char)*500);
  fgets(line, 500, stdin);
  char *commands = strtok(line, " \n");

  int i; int com = 0;

  while(1){
    i = 0;
    //signal(SIGINT, siginthandler);
    //signal(SIGTSTP, sigstphandler);
    while(commands){
      if(strcmp(commands, ";") == 0){
          com = 1;
          args1[i] = (char*)0;
          i = 0;
          commands = strtok(NULL, " \n");
        }

      if(strcmp(commands, ">") == 0){
        commands = strtok(NULL, " \n");
        int toOutput = open(commands, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXO);
        args1[i] = (char *)0;
        com = 3;
        //dup2(toOutput, 1);
        outputtofile(args1, toOutput);
        printf("%s", prompt);
        //dup2(1, 1);
        i = 0;
      }

      if(com == 0){
        args1[i] = (char*)malloc(sizeof(char)*100);
        strcpy(args1[i], commands);
        i++;
      }
      else{

      	args2[i] = (char*)malloc(sizeof(char)*100);
        strcpy(args2[i], commands);
        i++;
      }
      commands = strtok(NULL, " \n");
    }
      if(com == 0){
        if(strncmp(args1[0], "exit",4) == 0){
         printf("im done");
         exit(-1);
        }
        args1[i] = (char*)0;
        forking(args1);
        printf("%s", prompt);
      }
      else if(com == 1){
        if(strncmp(args2[0], "exit",4) == 0){
           printf("im done");
           forking(args1);
           exit(-1);
        }
    	  args2[i] = (char*)0;
        //printf(">>%d", i);
        forking2(args1, args2);
        printf("%s", prompt);
      }

    free(line);
    line = malloc(sizeof(char)*500);
    fgets(line, 500, stdin);
    commands = strtok(line, " \n");
    com = 0;
    }

}
