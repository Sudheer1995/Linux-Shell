#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include <fcntl.h>
#include <errno.h>
#include <pwd.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int main()
{
  char  *ctrld,*cwd, input[100000], username[100],  pc_name[100];
  cwd = (char *)malloc(sizeof(char)*100);
  int IN,OUT;
  getuserdetails(username,pc_name);           //gets user details
  sethomdir();                          //sets the current working directory as home directory
  IN = dup(STDIN_FILENO);
  OUT = dup(STDOUT_FILENO);
  while(1){
    dup2(STDIN_FILENO,IN);
    dup2(STDOUT_FILENO,OUT);
    curworkdir(cwd);
    listjobs(1);
    if(!strcmp(gethomdir(),cwd))                    // if current working directory is home
      printf("%s@%s:~$ ",username,pc_name);
    else if(substr(gethomdir(),cwd))                // if current working directory is child of home
      {
      cwd += child_dir(gethomdir(),cwd);
      printf("%s@%s:~%s$ ",username,pc_name,cwd);
      }
    else                                                // if current working directory is not in home branch
      printf("%s@%s:~%s$ ",username,pc_name,cwd);
    signal(SIGINT,signalHandler);
    signal(SIGTSTP,signalHandler);
    ctrld = fgets(input,1000,stdin);
    if(ctrld == NULL)                                 //gets the input
      strcpy(input,"quit");
    else
      input[strlen(input)-1] = '\0';
    if(!strcmp(input,"quit"))
      break;
    parseinput(input);                            //parses the input for processing
  }
  return 0;
}
