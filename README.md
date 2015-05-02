Homework folder for open source course

#Rshell
Program based on bash terminal

Similar usage and behavior

##Running
``$ git clone https://github.com/hannesphillips/rshell.git``

``$ cd rshell``

``$ git checkout hw0``

``$ make``

``$ bin/rshell``

##Usage
Uses getlogin() and gethostname() 
``<username>@<hostname>$``

Commands can then be input, eg.:

``$ ls``
``$ ls -a || exit``
``$ pwd && ls -l <directory> || mkdir <directory>``

The command to quit a session is exit

Connectors may follow a command as shown above. The ``&&``, or AND, connector
will execute the second command if the first one succeeded. The ``||``, or OR,
connector executes the second command if the first one failed.

Option flags are also accepted and can be input without any order.
ls for example can take -a or -aaaaa -a -a, -al or -l -a, etc.

Terminal comments are given after a '#'. Anything written here is for
the user(s) use only. The terminal will overlook anything entered after


#ls
A seperately wriiten C++ program that fully implements the ls command.
The program takes all types and forms of flags the BASH ls takes and can be ran within the existing rshell program in this repository

##Usage

`$ git clone https://github.com/hannesphillips/rshell.git`

`$ cd rshell`

`$ git checkout hw1`

`$ make`

`$ bin/ls`


##Features
`$ bin/ls` runs basic list and will print all files and directories in the current working directory

`$ bin/ls <flag>... <directory>...`

The ellipses, `...`, indicate any number of these parameters can be input

`$ bin/ls -a` runs ls and displays hidden files and folders

`$ bin/ls -l` runs in depth list that displays permissions of file/folder (read, write, and execute), the owner and group, and when the item was last edited

`$ bin/ls -R` recursively displays contents of files and folders of folders in the working directory

All of these flags can be combined or used simultaneously, i.e. -

`$ bin/ls -a -l` or `$ bin/ls -Rl -a` etc.

The program also handles multiple inputs of the same flag and will treat it as a single flag of that type. Furthermore, `bin/ls -a -l` is equivalent to `bin/ls -al` and `bin/ls -lll -a -l`

Input of an incorrect flag will result in an error

The command also takes directories as an input parameter to specify listing the contents of only the specified directory

#Known Bugs/Limitations
The combination of connectors without commands causes the program to
seg fault.

Echo is currently unpredictable under my tests
Change directory (cd) is not supported.

ls -R currently behaves as -R -a

The spacing of the program's output varies from BASH's ls
