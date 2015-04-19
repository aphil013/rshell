#include <iostream>
#include <stdlib.h>
#include <unistd.h>				// Sys calls
#include <stdio.h>
#include <sys/wait.h>			// Wait
#include <errno.h>				// perror()
#include <string.h>				// strtok
#include <cstring>				// strcpy()
#include <sstream>
#include <vector>


int main()
{
	char login[64]; 
	if(getlogin_r(login, sizeof(login)-1) != 0)
		perror("getlogin"); 
	std::string name = login;
	if(gethostname(login, sizeof(login)-1) == -1)
		perror("gethostname");
	std::string host = login;
	name += "@";
	std::string user = name + host;

	std::string cmd_line;
	int array_sz = 4096;
	while(1)
	{
		char** commands = new char*[array_sz];
		std::cout << user << "$ ";			// Prompt and input 
		getline(std::cin, cmd_line);
		
		char*  cmd_str = new char [cmd_line.length()+1];	// Tokenizing
		std::strcpy(cmd_str, cmd_line.c_str());
		
		int i = 0;
		char* cmd = std::strtok(cmd_str, " ");
		while(cmd != NULL)					// Populate array of char
		{							// pointers to cmds and flags
			commands[i] = cmd;
			cmd = std::strtok(NULL, " ");
			++i;
		}
		char key[] = "exit";
		if(strcmp(commands[0], key) == 0)
		{
			return 0;
		}
		else
		{
			int pid = fork();
			if(pid == 0)
			{
				execvp(commands[0], commands);
			}
			else
			{
				if(pid == -1)
				{
					perror("fork");
					exit(1);
				}
				wait(0);
			}
		}
		for(int d = 0; d < sizeof(commands); ++d)
		{
			commands[i] = '\0';
		}
	}	
}

// klu006
