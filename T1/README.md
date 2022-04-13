# Mini-Shell

In this project we were taked with creating a program capable of emulating a linux shell, handle arguments passed through the command line and system calls for process handling, such as fork(), pipe(), dup2(), exec(), wait(), exit(), among others. It's implementation was made to be as robust as possible, i.e. if a call to a system function fails, the program should display an error message and terminate with an appropriate exit code, making use of the perror() or strerror() functions to generate appropriate error messages.

The shell is be able to:
<ul>
  <li> Handle simple commands like ls, gcc, date, etc --------> ex: ls -l /tmp</li>
  <li> Handle simple input redirection using a file as stdin --------> ex: more < file_in </li>
  <li> Handle simple output redirection using a file as stdout --------> ex: s > file_out </li>
  <li> Support custom user commands --------> ex: user_command infile.txt outfile.txt command_input </li>
  <li> Support the chaining of commands with the use of pipes --------> ex: ps -ef | grep bash | wc </li>
  <li> Handle process competition --------> ex: gcc -o pois pois.c & </li>
</ul>

