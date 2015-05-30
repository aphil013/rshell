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

#I/O Redirection and Piping
Rshell now supports redirection and piping

The current capabilities are taking input from a file, creating/overwriting output to a file, and appending output to a file. Piping links the output of a file/program to the the input of the following.

##Usage
`$ git clone https://github.com/hannesphillips/rshell.git`

`$ cd rshell`

`$ git checkout hw2`

`$ make`

`$ bin/rshell`

The new features are available under the tag, hw2.

To use input redirection, the `<` character must be directed after the command to send the input to the program, e.g.

`ls -a < (file name)`: This would be useful in examining the total contents of a directory stored in a file. The folder(s) in the file would be passed to ls -a, and only the directories there would be looked at, refining the information presented

Output redirection uses, `>` and `>>` to determine how the output is used. 
The single character overwrties an existing file or creates and new one and places the outpt there. The double character appends, or adds to the end, of an existing file or will create one if it isn't present

`ps > out.txt`: Here the output of the ps command would be stored in the out.txt file
	
`pwd >> directory_list` : The output of pwd would be appended to the direcotry list file where the areas of work can be stored for later reference

#CD and ^C
Rshell now supports the use of the cd command and handles cntrl c differently

##Usage
`$ git clone https://github.com/hannesphillips/rshell.git`

`$ cd rshell`

`$ git checkout hw3`

`$ make`

`$ bin/rshell`

##Features
`cd` changes the current working directory to the home directory

`cd <PATH NAME>` changes the current working directory to PATH NAME specified

`^C` input no longer kills the shell but the foreground process

#Known Bugs/Limitations
Connectors as of now, do not work with redirection

Redirection has the potential to create junk files

Change directory (cd) is not supported.

ls -R currently behaves as -R -a

The spacing of the program's output varies from BASH's ls

cd - is not supported


