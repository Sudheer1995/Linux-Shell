#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <pwd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "shell.h"

static char *home;              //home directory

static node *head = NULL;         //process list head pointer

int isEmpty(node *head)             //return true if process list is empty
{
  if(head == NULL)
    return 1;
  else
    return 0;
}

node *create(pid_t pid,char *proc_name)       //creates a node to store the process name & process id
{
  node *p = (node *)malloc(sizeof(node));
  p->name = (char *)malloc(sizeof(char)*100);
  p->pid = pid;
  strcpy(p->name,proc_name);
  p->left = NULL;
  p->right = NULL;
  return p;
}

void insert(pid_t pid,char *proc_name)        // inserts into the list if the process in opened and running
{
  node *tmp,*p;
  p = create(pid,proc_name);
  if(head == NULL)
    {  head = p;
      return; }
  tmp = head;
  while(tmp->right!=NULL)
    tmp = tmp->right;
  tmp->right = p;
  p->left = tmp;
  p->right = NULL;
  return;
}

void delete(node *p)            // deletes the process from the list once they are completed
{
  if(p == head){
    free(p);
    head = head->right;
    return;
  }
  if(p->right == NULL){
    p->left->right = NULL;
    free(p);
    return;
   }
  p->left->right = p->right;
  p->right->left = p->left;
  free(p);
  return;
}

void signalHandler(int sig_num)   //  handles the signals like Ctrl+C , Ctrl+D
{
  if(sig_num == SIGINT){}
  return;
}

void getjobnumber(int jobnum)     // gets the background process into foreground process
{
  node *p;char *X[10] = {NULL};
  int stat,flag = 0,i = 1;
  p = head;
  while(p!=NULL){
    if(jobnum == i){
      flag = 1;
      break;
    }
    p=p->right;
    i++;
  }
  if(flag){
    while(wait(&stat)!=p->pid);
    delete(p);
  }
  return;
}

void sendsignal(pid_t pid,int sig_num){      //sends signal to a process
  kill(pid,sig_num);
}

void listjobs(int flag)                //check the status of the process and kill it, delete it from list
{
    int status;
    node *p;
    p = head;
    while(p != NULL){
      if(flag == 1){                                    //updates the process list
        if(waitpid(p->pid,&status,WNOHANG)){
          printf("%d %s exited normally\n",p->pid,p->name);
          kill(p->pid,SIGKILL);
          delete(p);
          }
        p = p->right;
      }
      else if(flag == 2){
        if(waitpid(p->pid,&status,WNOHANG)){             //lists all the process running
          printf("%d %s exited normally\n",p->pid,p->name);
          kill(p->pid,SIGKILL);
          delete(p);
          }
        else
          printf("%d %s\n",p->pid,p->name);
        p = p->right;
      }
      else{
        if(waitpid(p->pid,&status,WNOHANG)){          //kills all the process
          printf("%d %s exited normally\n",p->pid,p->name);
          kill(p->pid,SIGKILL);
          delete(p);
          }
        else
          kill(p->pid,SIGKILL);
        p = p->right;
      }
    }
  return;
}

void runproc(char *command[],int bckgrnd)
{
  int stat;                   // creates the process and runs them in either fore ground or back ground
  pid_t pid;  int status ;                 //creates a child process
  pid = fork();
  if(pid < 0){
    perror("process not created : ");
    exit(1);
    }
  if(pid == 0){
    if(bckgrnd){
      fclose(stdin);                  // child process
      fopen("/dev/null", "r");
      if(execvp(command[0],command) == -1){
        fprintf(stderr,"error occured :\n");
        exit(1);
        }
      }
  else{
    if(execvp(command[0],command) == -1){
      fprintf(stderr,"error occured :\n");
      exit(1);
      }
    }
  }                                   //parent process
  else{
    if(bckgrnd){
      insert(pid,command[0]);
      printf("[%d]  %s\n",pid,command[0]);     //background
      }
    else{
      insert(pid,command[0]);
      waitpid(pid,&status,WUNTRACED);
      if(WIFSTOPPED(status))
        fprintf(stderr, "\n[%d]+ stop %s\n",pid,command[0] );
      else{
         //signal(SIGTSTP,signalHandler);
        //  while(wait(&stat) != pid);
          kill(pid,SIGKILL);                                      //foreground
       }
      }
    }
  }

void curworkdir(char buf[]){ getcwd(buf,100); }     //returns the current working directory

void sethomdir()                        // sets the current directory as a home directory
{
    home = (char *)malloc(sizeof(char)*100);
    getcwd(home,100);
}

char *gethomdir(){  return home;  }       //return ~ home directory

void process(char *command[],char *argument)          //process the input
{
  char txt[1000],cwd[100],newarg[100]={'\0'},*buf,*str,*token,*saveptr,*argv[3];
  int line,fd,flag = 0;

  if(argument[strlen(argument)-1] == '&'){      //checks for the either background or foreground process
    argument[strlen(argument)-1] = '\0';
    flag = 1;
    }
    if(!strcmp(command[0],"cd")){          //changes the directory if input is cd "some directory"
      if(argument[0] == '~'){         //gets to home when used cd ~
        strcat(newarg,home);
        strcat(newarg,argument+1);
        argument = newarg;
      }
    if(access(argument,F_OK) == 0)
      chdir(argument);
    else
      perror(argument);
    }
  else if (!strcmp(command[0],"pinfo")){
    curworkdir(cwd);                        // shows the process information of a process if it correct
    chdir("/proc");                         //pid is given
    if(access(argument,F_OK) == 0){
      chdir(argument);
      buf = (char *)malloc(sizeof(char)*100000);
      fd = open("status",O_RDONLY);
      read(fd,buf,10000);
      for(line = 1, str = buf; ;line++, str = NULL){
        token = strtok_r(str,"\n",&saveptr);
        if(token == NULL)
            break;
        if(line == 1 || line == 2 || line == 5 || line == 17)
            puts(token);
        }
      }
      else
        perror(argument);
      chdir(cwd);
    }
    else if(!strcmp(command[0],"pwd")){
      curworkdir(cwd);
      puts(cwd);
    }
    else if (!strcmp(command[0],"echo")){
      if(argument[strlen(argument)-2] == '\"')
        argument[strlen(argument)-2] = '\0';
      if(argument[0] == '\"')
        argument += 1;
      printf("%s\n",argument);
    }
    else if(!strcmp(command[0],"listjobs"))
      listjobs(2);
    else if(!strcmp(command[0],"killallbg"))
      listjobs(3);
    else if(!strcmp(command[0],"fg"))
      getjobnumber(atoi(argument));
    else
      runproc(command,flag);
}

char *removespace(char *input)
{
  char *output;
  int i = 0,j = 0;
  output = (char *)malloc(sizeof(char)*100);
  for(i = 0; i < strlen(input); i++)
    if(input[i] != ' ')
      break;
  int start = i;
  for(i = strlen(input)-1; i>=0; i--)
    if(input[i] != ' ')
      break;
  int end = i;
  for(i = start;i <= end; i++)
    output[j++] = input[i];
  return output;
}

char *redirect(char *input)
{
  int stat,j,i;
	char symbol[2]={'\0'},*X[10] = { NULL };
	for(i = 0;i < strlen(input); i++){
		if(input[i] == '<')
			strcat(symbol,"<");
		if(input[i] == '>')
			strcat(symbol,">");
	}
	i = 0;
	char *str1,*str2,*token,*subtoken,*newarg[100]={NULL};
	char *saveptr1,*saveptr2,*arg[100]={NULL};
  for (j = 1, str1 = input; ; j++, str1 = NULL) {
  	token = strtok_r(str1,">", &saveptr1);
    if (token == NULL)
    	break;

    for (str2 = token; ; str2 = NULL) {
    	subtoken = strtok_r(str2,"<", &saveptr2);
      if (subtoken == NULL)
      	break;
			arg[i++] = subtoken;
		}
	}
  for(j = 0; j < i; j++)
    arg[j] = removespace(arg[j]);

    if(strcmp(symbol,"<") == 0){           //    '<'
      int fd1 = open(arg[1],O_RDONLY);
      dup2(fd1, STDIN_FILENO);
      close(fd1);
      X[0] = arg[0];
      }
    else if(strcmp(symbol,"<>") == 0){     // '<' '>'
      int fd1 = open(arg[1],O_RDONLY);
      dup2(fd1, STDIN_FILENO);
      close(fd1);
      int fd0 = creat(arg[2], 0644);
      dup2(fd0, STDOUT_FILENO);
      close(fd0);
      X[0] = arg[0];
    }
    else if(strcmp(symbol,"><") == 0){
      int fd1 = open(arg[2],O_RDONLY);
      dup2(fd1, STDIN_FILENO);        // '>' '<'
      close(fd1);
      int fd0 = creat(arg[1], 0644);
      dup2(fd0, STDOUT_FILENO);
      close(fd0);
      X[0] = arg[0];
    }
    else	{
      int j,i = 0;									// '>'
      int fd0 = creat(arg[1], 0644);
      char *token,*saveptr1,*str1;
      dup2(fd0, STDOUT_FILENO);
      close(fd0);
        X[0] = arg[0];
        }
    return X[0];
}

void redirect_pipe(char input[])    // pipe functionality is implemented
{
  char *str1, *token;
  char *argv[100]={NULL},*saveptr1;
  int stat,i = 0,fd_in = 0,filedes[2],j;
  pid_t pid;
  for (j = 1, str1 = input; ; j++, str1 = NULL) {
    token = strtok_r(str1,"|", &saveptr1);
    if (token == NULL)
      break;
    argv[i++] = token;
  }
  i = 0;
  while (argv[i] != NULL)
  {
    int redir = 0;
    for(int k = 0; k < strlen(argv[i]); k++)
      if(argv[i][k] == '<'||argv[i][k] == '>')
        redir = 1;
      pipe(filedes);
      pid = fork();
      if(pid < 0)
        exit(EXIT_FAILURE);
      else if (pid == 0) {
        char *str1,*saveptr1,*token,*X[10] = {NULL};
        if(redir)
        argv[i] = redirect(argv[i]);
        dup2(fd_in,STDIN_FILENO);
        if (argv[i+1]!= NULL)
          dup2(filedes[1],STDOUT_FILENO);
        close(filedes[0]);
        int j,k = 0;
        for (j = 1, str1 = argv[i]; ; j++, str1 = NULL) {
        	token = strtok_r(str1," ", &saveptr1);
          if (token == NULL)
          	break;
      		X[k++] = token;
      	}
        execvp(X[0],X);
        exit(EXIT_FAILURE);
      }
      else
      {
        while(wait(&stat)!=pid);
        close(filedes[1]);
        fd_in = filedes[0];
      }
    i++;
  }
  return ;
}

void parseinput(char *input)
{
  char *str1, *str2, *token, *subtoken;
  char *saveptr1, *saveptr2, *command[100]={NULL};
  int redirect,flag,i,j,len;
  for (j = 1, str1 = input; ; j++, str1 = NULL) {
  token = strtok_r(str1, ";", &saveptr1);           //breaks the input over ';'
  if (token == NULL)
    break;
  redirect = 0;
  for(int k = 0;k < strlen(token); k++)
    if(token[k] == '>'|| token[k] == '<'|| token[k] == '|')
      redirect = 1;
  if(redirect)
    redirect_pipe(token);
  else{
    str2 = token; len = 0;
    subtoken = strtok_r(str2, " ", &saveptr2);        //gets the input command for first ' ' break
    command[len++] = subtoken;
    str2 = NULL;
    flag = 0;
    char arg[100]={'\0'},argument[100]={'\0'};
    for (i = 0; ;i++, str2 = NULL) {
      subtoken = strtok_r(str2, " ", &saveptr2);      //gets the arguments
      if (subtoken == NULL) break;
      if(!strcmp(command[0],"echo")){
        strcat(argument,subtoken);
        strcat(argument," ");
      }
      else {
        strcat(argument,subtoken);
        command[len++] = subtoken;
        }
      }
    process(command,argument);
    }
  }
}

int child_dir(char *actpath,char *curpath)        //finds the pattern of home directory in current working directory
{
  int i;
  for(i = 0;i < strlen(actpath);i++)
  if(actpath[i] != curpath[i])
    break;
  return i;
}

int substr(char *actpath,char *curpath)    //finds if home is substring of current working directory
{
  int i;
  if(strlen(actpath) > strlen(curpath))
    return 0;
  for(i = 0;i < strlen(actpath);i++)
    if(actpath[i] != curpath[i])
      return 0;
  return 1;
}

void getuserdetails(char username[],char pc_name[])   //access the details of the username and hostname
{
  struct passwd *details;
  uid_t user_id;
  char comp_name[100];
  user_id = geteuid();
  details = getpwuid(user_id);
  gethostname(comp_name,100);
  strcpy(username, details->pw_name);
  strcpy(pc_name, comp_name);
}
