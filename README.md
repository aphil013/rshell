Homework folder for open source course

#rshell
Program based on bash terminal

Similar usage and behavior

#Running
``$ git clone https://github.com/hannesphillips/rshell.git``

``$ cd rshell``

``$ git checkout hw0``

``$ make``

``$ bin/rshell``

#Usage
Uses getlogin() and gethostname() 

``<username>@<hostname>$``

Commands can then be input, eg.:

``$ ls``

``$ ls -a || exit``

``$ pwd && ls -l <directory> || mkdir <directory>``

The command to quit a session is exit

Connectors may follow a command as shown above. The &&, or AND, connector
will execute the second command if the first one succeeded. The ||, or OR,
connector executes the second command if the first one failed.

Option flags are also accepted and can be input without any order.
ls for example can take -a or -aaaaa -a -a, -al or -l -a, etc.

Terminal comments are given after a '#'. Anything written here is for
the user(s) use only. The terminal will overlook anything entered after

#Bugs
The sequence `$ || &&` causes the shell to output a non-fatal error

More than 3 connectors with no command causes a logical error

Change directory (cd) is not currently supported
