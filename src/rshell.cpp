#include <iostream>
#include <stdlib.h>
#include <unistd.h>				// Sys calls
#include <stdio.h>
#include <sys/wait.h>				// Wait
#include <errno.h>				// perror()
#include <sys/types.h>
#include <fcntl.h>

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

void ctrl_c(int x)
{
	signal(SIGINT, SIG_IGN);
}

vector<string> tok(char x[], string key)		// Redone nicer tokens
{
	const char* str = key.c_str(); // Error without const
	char* cmd = strtok(x, str);
	vector<string> z;
	while(cmd != '\0')
	{
		z.push_back(cmd);
		cmd = strtok('\0', str);
	}
	return z;
}

void redirection(char** cmd)
{
	for(unsigned int i = 0; cmd[i] != '\0'; ++i)
	{
		int ps = 0;
		if(!strcmp(cmd[i], "<"))
		{
			cmd[i] = 0;
			if((ps = open(cmd[i+1], O_RDONLY)) == -1)
				perror("open");
			if((dup2(ps, 0)) == -1)
				perror("dup2");
		}
		else if(!strcmp(cmd[i], ">"))
		{
			if(!strcmp(cmd[i+1], ">"))
			{
				cmd[i] = 0;
				cmd[i+1] = 0;
				if((ps = open(cmd[i+2], O_CREAT | O_WRONLY | O_APPEND, 0666)) == -1)
					perror("open");
				if((dup2(ps, 1)) == -1)
					perror("dup2");
				++i;
			}
			else
			{
				cmd[i] = 0;
				if((ps = open(cmd[i+1], O_CREAT | O_WRONLY | O_TRUNC, 0666)) == -1)
					perror("open");
				if((dup2(ps, 1)) == -1)
					perror("dup2");
			}
		}
	}
	return;
}

void execute(vector<string> commands)
{
	char* path = getenv("PATH");		// Preperation for cd
	vector<string> path_var = tok(path, ":");

	unsigned int size = commands.size();
	char** arg = new char*[size+1];
	arg[size] = '\0';
	for(unsigned int i = 0; i < size; ++i)
	{
		arg[i] = new char[commands.at(i).size()];
		strcpy(arg[i], commands.at(i).c_str());
	}

	redirection(arg);
	
	for(unsigned int i = 0; i < path_var.size(); ++i)
	{
		char hold[BUFSIZ];
		strcpy(hold, const_cast<char*>(path_var.at(i).c_str()));
		strcat(hold, "/");
		strcat(hold, const_cast<char*>(commands.at(0).c_str()));

		char* pirate[BUFSIZ]; // pirate -> arrrrrg
		pirate[0] = hold;

		for(unsigned int j = 1; j < commands.size(); ++j)
		{
			pirate[j] = const_cast<char*>(commands.at(j).c_str());
		}

		if(execv(pirate[0], pirate) == -1);
		else
			return;
	}
	
	if(errno && commands.at(0) != "cd")
	{
		perror("execv");
		exit(1);
	}
	else if(commands.at(0) == "cd")
		exit(1);
}

void execution(vector<string> commands)
{
	vector<string> sec1;	// Sections of connected commands
	vector<string> sec2;

	unsigned int i;
	for(i = 0; i < commands.size() && commands.at(i) != "|"; ++i)
		sec1.push_back(commands.at(i));
	for(i += 1; i < commands.size(); ++i)
		sec2.push_back(commands.at(i));
	
	if(sec2.size() == 0)	// No pipe present
		execute(commands);
	else
	{
		int ps[2];
		if(pipe(ps) == -1)		// Pipe sys call
			perror("pipe");  
		int pid = fork();
		if(pid == -1)
		{
			perror("fork");
			exit(1);
		}
		else if(pid == 0)
		{
			if(dup2(ps[1], 1) == -1)
				perror("dup2");
			if(close(ps[0]) == -1)
				perror("close");
			execute(sec1);
		}
		else					// Tons of error checking in parent
		{
			int stat = 0;
			if(waitpid(-1, &stat, 0) == -1)
				perror("wait");
			int rest = dup(0);
			if(rest == -1)
				perror("dup");
			if(dup2(ps[0], 0) == -1)
				perror("dup2");
			if(close(ps[1]) == -1)
				perror("close");
			execution(sec2);	// Recursive call on second section
			if(dup2(rest, 0) == -1)
				perror("dup2");
		}
	}
	return;
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
void redirect_spacer(string &str)			// Calls replace to make parsing redirection simpler
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

	while(cin.good())		// Continuous loop simulating terminal
	{
		begin:								// Label to jump to

		signal(SIGINT, ctrl_c);

		char buffer[BUFSIZ];
		if(!getcwd(buffer, sizeof(buffer)))
			perror("getcwd");
		cout << buffer << endl;

		cout << user << "$ ";				// Prompt and input 
		getline(cin, cmd_line);
		int x = cmd_line.find("#", 0);
		if(x >= 0) cmd_line = cmd_line.substr(0, x);		// Takes care of comments

		redirect_spacer(cmd_line);

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
		
		char*  cmd_str = new char [cmd_line.size()+1];				// Create array of c-strings
		strcpy(cmd_str, cmd_line.c_str());
		vector<string> commands;
		string key = ""; // Connector storage
	
		string io;

		if(cmd_line.find(";") != string::npos)
		{
			commands = tok(cmd_str, ";");
			key = ";";
		}
		else if(cmd_line.find("&&") != string::npos)
		{
			commands = tok(cmd_str, "&&");
			key = "&&";
		}
		else if(cmd_line.find("||") != string::npos)
		{
			commands = tok(cmd_str, "||");
			key = "||";
		}
		
		if(key == "")
		{
			int stat = 0;
			commands = tok(cmd_str, " ");

			if(commands.size() == 1 && commands.at(0) == "exit")
			{
				delete[] cmd_str;
				return 0;
			}
			if(commands.at(0) == "cd")
			{
				if(commands.size() == 1)
				{
					if(chdir(getenv("HOME")) == -1)		// Error check
						perror("chdir");
				}
				else
				{
					if(chdir(const_cast<char*>(commands.at(1).c_str())) == -1)	
					// Have to change type for chdir's parameters
					// Error check at the same time
						perror("chdir");
				}
			}

			int pid = fork();
			if(pid == -1)
			{
				perror("fork");
				exit(1);
			}
			else if(pid == 0) // Child process
				execution(commands);
			else
			{
				if(waitpid(-1, &stat, 0) == -1)
				{
					perror("waitpid");
					exit(1);
				}
				else
				{
					delete[] cmd_str;
					goto begin;
				}
			}
		}

		vector<vector<string> > super;	// Vector to be populated for cmds
		for(unsigned int i = 0; i < commands.size(); ++i)
		{
			char* alpha = new char[commands.at(i).size()+1];
			strcpy(alpha, commands.at(i).c_str());
			super.push_back(tok(alpha, " "));
		}
		for(unsigned int i = 0; i < super.size(); ++i)
		{
			int stat = 0;
			if(super.at(i).size() == 1 && super.at(i).at(0) == "exit")
			{
				delete[] cmd_str;
				return 0;
			}

			if(super.at(i).at(0) == "cd")
			{
				if(super.at(i).size() == 1)
				{
					if(chdir(getenv("HOME")) == -1)
						perror("chdir");		// Same change dir error check
				}
				else
				{
					//Identical call
					if(chdir(const_cast<char*>(super.at(i).at(1).c_str())) == -1)
						perror("chdir");
				}
			}
			else
			{
				int pid = fork();
				if(pid == -1)
				{
					perror("fork");
					exit(1);
				}
				else if(pid == 0)
					execution(commands);
				else
				{
					if(waitpid(-1, &stat, 0) == -1)
					{
						perror("waitpid");
						exit(1);
					}
					else
					{
						if(key == "")
							break;
						else if(key == ";");
						else if(key == "||")
						{
							if(stat <= 0)
								break;
						}
						else if(key == "&&")
						{
							if(stat > 0)
								break;
						}
					}
				}
			}
		}
		delete[] cmd_str;
	}

}
