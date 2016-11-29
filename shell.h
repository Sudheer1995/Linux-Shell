#include <sys/types.h>
                                                  // structure for process list
typedef struct node{
  pid_t pid;
  char *name;
  struct node *left,*right;
}node;
                                              //declarations of functions
node *create(pid_t pid,char *proc_name);
int isEmpty(node *head);
void insert(pid_t pid,char *proc_name);
void delete(node *p);
void signalHandler(int sig_num);
void getjobnumber(int jobnum);
void sendsignal(int jobnum,int signum);
void listjobs(int flag);
void runproc(char *command[],int bckgrnd);
void curworkdir(char buf[]);
void sethomdir();
char *gethomdir();
char *removespace(char *input);
char *redirect(char *input);
void parseinput(char *input);
void process(char *command[],char *argument);
void parseinput(char *input);
int child_dir(char *actpath,char *curpath);
int substr(char *actpath,char *curpath);
void getuserdetails(char username[],char pc_name[]);
