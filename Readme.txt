1.  make was written in makefile.
2.  shell.h has all the declarations of the methods and structures used in shell.c.
3.  shell.c has all the definitions of the methods of declarations in shell.h.
4.  main.c has the runnable code and main method in it.
5.  pinfo , cd , pwd , echo commands are implemented in process method.
6.  foreground , background running of process are implemented using waitpid(),fork() methods.
7.  commands seperated with ; are executed sequentially.
8.  when a process is terminated shell shows its exit messages and pid of that process.
9.  to run process in backround '&' should be appended at last.
10. strtok_r method is used to parse the input with ';',' ' tokens. 
