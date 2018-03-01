# Linux-Shell
  - ## Functionality
    - Prompt: ***`<username@system_name:curr_dir>`*** The directory from which the shell is invoked will be the home directory of the shell and is indicated by "~". If the user executes ***"cd"*** change directory then the corresponding change must be reflected in the shell as well.
     - Builtin commands ***(cd , pwd and echo)*** are contained within the shell itself. When the name of a builtin command is used as the first word of a simple command the shell executes the command directly, without invoking another program.
     - ***Foreground processes:*** For example, executing a ***"vi"*** command in the foreground implies that your shell will wait for this process ​to complete and regain control when this process exits.
     - ***Background processes:*** Any command invoked with "&" is treated as background command. This implies that your shell will spawn that process and doesn't wait for the process to exit. It will keep taking user commands.
      - ***E.g:***
        - ***```<Name@UBUNTU:~> ls &```*** This command when finished, will print its result.
        - ***```<Name@UBUNTU:~> emacs &```***
        - ***```<Name@UBUNTU:~> ls -l -a```*** 
        - ***```<Name@UBUNTU:~> echo hello```***
      - If the background process exits then the shell display the appropriate message to the user.
      - ***pinfo*** : prints the process related info of your shell program.
      - Output of running one (or more) commands redirected to a file. Similarly, a command might be prompted to read input data from a file and asked to write output to another file. Appropriate error handling is done (like if the input file does not exist etc.)
        - ***E.g:*** Output redirection
          - ***```<NAME@UBUNTU:~>diff file1.txt file2.txt > output.txt```***
        - ***E.g:*** Input redirection
          - ***```<NAME@UBUNTU:~>sort < lines.txt```***
        - ***E.g:*** Input-Output redirection
          - ***```<NAME@UBUNTU:~>sort < lines.txt > sorted-lines.txt```***
          - ***Note:*** There is another clause for output direction '>>', is implemented too.
      - A pipe is identified by "|". One or more commands can be piped as the following examples shown, supports any number of pipes.
        - ***E.g:*** Two Commands
          - ***```<NAME@UBUNTU:~> more file.txt | wc```***
        - ***E.g:*** Three commands
          - ***```<NAME@UBUNTU:~> grep "new" temp.txt | cat somefile.txt | wc```***

      - ***listjobs:*** prints a list of all currently running jobs along with their pid, particularly background jobs, in order of their creation.
        - ***E.g:***
          - ***```<NAME@UBUNTU:~>jobs
            [1] emacs Assign.txt [231]
            [2] firefox [234]
            [3] vim [5678]```***
            Here [3] i.e vim is most recent background job, and the oldest one is emacs.
       - ***sendsig*** <jobNumber> <signalNumber>: takes the job id of a running job and send given signal value to that process
          - ***E.g:*** ***```<NAME@UBUNTU:~> kjob 2 9```*** It sends sigkill to the process firefox, and as a result it is terminated. Here 9 represents the signal number, which is sigkill. For further info, lookup man 7 Signal.
        - ***fg*** <jobNumber>: brings background job with given job number to foreground.
          - ***E.g:*** ***```<NAME@UBUNTU:~> fg 3```***
          Either brings the 3rd job which is vim to foreground or returns error if no such background number exists.
        - ***killallbg:*** kills all background process at once
        - ***quit:*** exits the shell. Your shell should exit only if the user types this ***"quit"*** command. It should ignore any other signal from user like : ***CTRL-D, CTRL-C, SIGINT, SIGCHLD*** etc.
        - ***CTRL-Z*** : It should change the status of currently running job to stop, and push it in Background process.
        - ***CTRL+D*** : Exit the shell.
        - ***CTRL+C*** : Match the functionality of bash shell.
        
  - ## Code Structure
    - shell.h has all the declarations of the methods and structures used in shell.c.
    - shell.c has all the definitions of the methods of declarations in shell.h.
    - main.c has the runnable code and main method in it.
    - pinfo, cd, pwd, echo commands are implemented in process method.
    - foreground, background running of process are implemented using waitpid(), fork() methods.
    - commands seperated with **";"**(semicolon seperated) are executed sequentially.
    - when a process is terminated shell shows its exit messages and pid of that process.
    - run process in backround **"&"** should be appended at last.
    - strtok_r method is used to parse the input with **";"** (semicolon separated), **" "** (space seperated) tokens.
  - ## Execute
    - run ```make``` in cloned directory.
    - execute ```./main``` to start shell.
