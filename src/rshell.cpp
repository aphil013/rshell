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


int main(int argc, char* argv[])
{
	char login[64]; 
	getlogin_r(login, sizeof(login)-1); 
	std::string name = login;
	gethostname(login, sizeof(login)-1);
	std::string host = login;
	name += "@";
	std::string user = name + host;

	std::string cmd_line;
	std::vector<char*> commands;
	std::vector<int> flag;
	while(1)
	{
		std::cout << user << "$ ";			// Prompt and input 
		getline(std::cin, cmd_line);
		
		char*  cmd_str = new char [cmd_line.length()+1];	// Tokenizing
		std::strcpy(cmd_str, cmd_line.c_str());

		char* cmd = std::strtok(cmd_str, " ");
		while(cmd != NULL)
		{
			commands.push_back(cmd);
			cmd = std::strtok(NULL, " ");
		}
		
		int flagcnt = 0;							// Locating cmd flags
		for(int i = 0; i < commands.size(); ++i)
		{
			if(commands.at(i)[0] == '-')
			{
				flag.push_back(i);
				flagcnt++;
			}
		}
		//for(int j = 0; j < commands.size() - flagcnt; ++j)
		int pid = fork();
		if(pid == 0)
		{
			execvp(commands.at(0), argv);		// Works for std cmds only
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

		while(commands.size() != 0)
			commands.pop_back();
	}	
}


