#include <iostream>
#include <stdlib.h>
#include <unistd.h>				// Sys calls
#include <stdio.h>
#include <sys/wait.h>				// Wait
#include <errno.h>				// perror()
#include <sys/types.h>

#include <string.h>				// strtok
#include <cstring>				// strcpy()
#include <ctype.h>
#include <vector>

using namespace std;

void user_prompt(string& user)				// Gets user info for command prompt
{
	char login[1024]; 
	if(getlogin_r(login, sizeof(login)-1) != 0)
		perror("getlogin"); 
	string name = login;
	if(gethostname(login, sizeof(login)-1) == -1)
		perror("gethostname");
	string host = login;
	name += "@";
	user = name + host;
}

bool execute(char** commands)					// Executes and returns success value 
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
		string cond = commands[0];
		if(cond == "true") return true;
		if(cond == "false") return false;
		
			
	}
	return true;
}
void replace(string &str, const string& curr, const string& hip)
{
	size_t pos = 0;
	while((pos = str.find(curr, pos)) != string::npos)
	{
		str.replace(pos, curr.length(), hip);
		pos += hip.length();
	}
	return;
}
void redirect_spacer(string &str)
{
	replace(str, "<", " < ");
	replace(str, ">", " > ");
	replace(str, ">>", " >> ");
	replace(str, "|", " | ");
	
	return;
}

int main()				
{
	string user;
	user_prompt(user);		// Gets login/host info

	string cmd_line;
	int array_sz = 0x8000;

	while(cin.good())		// Continuous loop simulating terminal
	{
		bool prev = true;

		bool re_in = false;
		bool re_out = false;
		bool re_pipe = false;

		cout << user << "$ ";				// Prompt and input 
		getline(cin, cmd_line);
		int x = cmd_line.find("#", 0);
		if(x >= 0) cmd_line = cmd_line.substr(0, x);		// Takes care of comments

		cout << cmd_line << endl;
		redirect_spacer(cmd_line);
		cout << cmd_line << endl;

		x = 0;
		for(string::iterator it = cmd_line.begin(); it < cmd_line.end(); ++it)	// Spaces between ';' for parsing
		{
			int semi = cmd_line.find(";", x);
			if(semi >= 0)
			{
				cmd_line.insert(semi+1, 1, ' ');
				cmd_line.insert(semi, 1, ' ');

				it = cmd_line.begin();
				it += semi + 2;
				x = semi + 2;
			}
		}
	/*	
		char*  cmd_str = new char [cmd_line.size()+1];				// Create array of c-strings
		strcpy(cmd_str, cmd_line.c_str());
		char** commands = new char*[array_sz];

		

		string key; // Connector storage
		string logic; // Command storage
		string io;
		unsigned int i = 0;
		char* cmd = strtok(cmd_str, " \t");			// Begin parsing

		while(cmd != NULL)						// First command apart from connectors
		{
			logic = cmd;
			if(logic == "||" || logic == "&&" || logic == ";")
				break;	
			else{		
				commands[i] = cmd;
				++i;
				cmd = strtok(NULL, " \t");
			}
		}
		key = logic;
		if(commands[0] != NULL)						// Executes first command
		{

			for(unsigned int k = 0; k < i; ++k)
			{
				io = commands[k];
				if(io == "<") re_in = true;
				else if(io == ">") re_out = true;
				else if(io == "|") re_pipe = true;
			}
			logic = commands[0];
			if(logic == "exit")
				return 0;
			if(logic == "")
				prev = false;
			else
				if(!re_in && !re_out && !re_pipe)
					prev = execute(commands);
				else
				{
					if(re_in && !re_out && !re_pipe)
					{

					}
				}
					
		}
		for(unsigned int j = 0; j < sizeof(commands); ++j)		// For loops to "erase" executed command
		{
			commands[j] = '\0';
		}

		cmd = strtok(NULL, " \t");
		i = 0;		
		while(cmd != NULL)						// Everything else w/ connector calculation
		{
			logic = cmd;
			if(logic == "exit")						// Exit at anytime
				return 0;

			if(logic == "") prev = false;
			else if(logic != "||" && logic != "&&" && logic != ";")		// Continue parsing if no connectors reached
			{
				commands[i] = cmd;
				++i;
			}
			else if(logic == "||")						// Connectors determine each execution path
			{
				key = logic;
				logic = commands[0];
				if(logic == "exit")
					return 0;
				if(!prev) prev = execute(commands);
				else prev = 0;
				for(unsigned int j = 0; j < sizeof(commands); ++j)
				{
					commands[j] = '\0';
				}
				i = 0;
			}
			else if(logic == "&&")
			{
				key = logic;
				logic = commands[0];
				if(logic == "exit")
					return 0;
				if(prev) prev = execute(commands);
				for(unsigned int j = 0; j < sizeof(commands); ++j)
				{
					commands[j] = '\0';
				}
				i = 0;
			}
			else if(logic == ";")
			{
				prev = true; 
					
				for(unsigned int j = 0; j < sizeof(commands); ++j)
				{
					commands[j] = '\0';
				}
				i = 0;
			}
			cmd = strtok(NULL, " \t");
	
		}
		if(i != 0)									// Execute last command, if any
		{
			if(key == "||")
				if(!prev) execute(commands);
			if(key == "&&")
				if(prev) execute(commands);
			if(key == ";") execute(commands);
		}
			
	}*/
}
	return 0;

}
