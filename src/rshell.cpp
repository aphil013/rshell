#include <iostream>
#include <stdlib.h>
#include <unistd.h>				// Sys calls
#include <stdio.h>
#include <sys/wait.h>				// Wait
#include <errno.h>				// perror()
#include <string.h>				// strtok
#include <cstring>				// strcpy()
#include <vector>
#include <array>


void user_prompt(std::string& user)
{
	char login[1024]; 
	if(getlogin_r(login, sizeof(login)-1) != 0)
		perror("getlogin"); 
	std::string name = login;
	if(gethostname(login, sizeof(login)-1) == -1)
		perror("gethostname");
	std::string host = login;
	name += "@";
	user = name + host;
}

bool execute(char** commands)
{
	if(*commands == NULL)
	{
		return false;
	}
	
	int status = 0;
	int pid = fork();
	if(pid == -1)
	{
		perror("fork");
		exit(1);
	}
	else if(pid == 0)
	{
		status = execvp(commands[0], commands);
		if(status == -1)
		{
			perror("execvp");
			exit(1);
		}
	}	
	else
	{
		int wait = waitpid(pid, &status, 0);
		if(wait == -1)
			perror("wait");
		std::string cond = commands[0];
		if(cond == "true") return true;
		if(cond == "false") return false;
		
			
	}
	return true;
}

int main()
{
	std::string user;
	user_prompt(user);		// Gets login/host info

	std::string cmd_line;
	int array_sz = 4096;
	while(std::cin.good())
	{
		std::cout << user << "$ ";				// Prompt and input 
		getline(std::cin, cmd_line);
		int x = cmd_line.find("#", 0);
		if(x >= 0) cmd_line = cmd_line.substr(0, x);
		
		char*  cmd_str = new char [cmd_line.length()+1];
		std::strcpy(cmd_str, cmd_line.c_str());
		char** commands = new char*[array_sz];

		int i = 0;
		char* cmd = std::strtok(cmd_str, " \t");
		while(cmd != NULL)					// Populate array of char
		{							// pointers to cmds and flags
			commands[i] = cmd;
			cmd = std::strtok(NULL, " \t");
			++i;
		}
		std::string exit_key = commands[0];
		if(exit_key == "exit")
		{
			return 0;
		}
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
		
		for(unsigned int d = 0; d < sizeof(commands); ++d)
		{
			commands[d] = '\0';
		}
	}	
}
