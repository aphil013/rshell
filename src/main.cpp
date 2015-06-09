#include <iostream>
#include <string.h>
#include <cstring>
#include <vector>
#include <ctype.h>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

using namespace std;

void get_user(string& s)
{
	char login[1024];
	if(getlogin_r(login, sizeof(login)-1) != 0)
		perror("getlogin");
	string name = login;
	if(gethostname(login, sizeof(login)-1) == -1)
		perror("gethostname");
	string host = login;
	name += "@";
	s = name + host;

	return;
}

vector<string> tok(char x[], string key)
{
	const char* str = key.c_str();
	char* cmd = strtok(x, str);

	vector<string> z;
	while(cmd != '\0')
	{
		z.push_back(cmd);
		cmd = strtok('\0', str);
	}
	return z;
}

bool execute(vector<string> v)
{
	unsigned int size = v.size();
	char** arg = new char*[size + 1];
	arg[size] = '\0';
	for(unsigned int i = 0; i < size; ++i)
	{
		arg[i] = new char[v.at(i).size()];
		strcpy(arg[i], v.at(i).c_str());
	}
	if(*arg == NULL)
	{
		delete[] arg;
		return false;
	}

	string cmd = arg[0];
	if(cmd == "true")
	{
		delete[] arg;
		return true;
	}
	else if(cmd == "false")
	{
		delete[] arg;
		return false;
	}

	int stat = 0;
	int pid = fork();
	if(pid == -1)
	{
		perror("fork");
		exit(1);
	}
	else if(pid == 0)
	{
		stat = execvp(arg[0], arg);
		if(stat == -1)
		{
			perror("execvp");
			exit(1);
			return false;
		}
	}
	else
	{
		if(waitpid(-1, &stat, 0) == -1)
		{
			perror("waitpid");
			exit(1);
		}

		if(stat > 0)
		{
			delete[] arg;
			return false;
		}
	}
	delete[] arg;
	return true;
}

int main()
{
	string prompt, cmd_line;
	get_user(prompt);
	while(cin.good())
	{
		begin:  // Label

		bool bridge = false;

		cout << prompt << "$ ";
		getline(cin, cmd_line);
		int x = cmd_line.find("#", 0);
		if(x >= 0)
			cmd_line = cmd_line.substr(0, x);

		if(cmd_line.length() == 0)
			goto begin;

		x = 0;
		string::iterator it;
		for(it = cmd_line.begin(); it < cmd_line.end(); ++it)
		{
			int semi = cmd_line.find(";", x);
			if(semi >= 0)
			{
				cmd_line.insert(semi + 1, 1, ' ');
				cmd_line.insert(semi, 1, ' ');

				it = cmd_line.begin();
				it += semi + 2;
				x = semi + 2;
			}
		}

		char* cmd_str = new char[cmd_line.size() + 1];
		strcpy(cmd_str, cmd_line.c_str());
		vector<string> commands;
		string key = "";

		if(cmd_line.find(";") != string::npos)
			key = ";";
		else if(cmd_line.find("&&") != string::npos)
			key = "&&";
		else if(cmd_line.find("||") != string::npos)
			key = "||";

		if(key == "")
		{
			commands = tok(cmd_str, " ");

			if(commands.at(0) == "exit")
			{
				delete[] cmd_str;
				return 0;
			}

			execute(commands);
			delete[] cmd_str;
			goto begin;
		}
		else
		{
			commands = tok(cmd_str, " ");
			if(commands.size() == 1 && (commands.at(0) == "||" || commands.at(0) == "&&" || commands.at(0) == ";"))
			{
				cout << "rshell: unexpected syntax near token '" << commands.at(0) << "'\n"; 
				delete[] cmd_str;
				goto begin;
			}
			vector<vector<string> > super;
			vector<string> empty;

			int sz = 0;
			unsigned int k;
			for(unsigned int i = 0; i < commands.size(); ++i)
			{
				k = i + 1;
				char* alpha = new char[commands.at(i).size() + 1];
				strcpy(alpha, commands.at(i).c_str());
				super.push_back(empty);
				super.at(sz).push_back(alpha);
				if(k != commands.size())
				{
					while(commands.at(k).find('-') != string::npos)
					{
						alpha = new char[commands.at(k).size() + 1];
						strcpy(alpha, commands.at(k).c_str());
						super.at(sz).push_back(alpha);
						++k;
					}
				}
				i = k - 1;
				++sz;
			}
			if(super.at(0).at(0) == "&&" || super.at(0).at(0) == "||" || super.at(0).at(0) == ";")
			{
				vector<vector<string> > v;
				v.push_back(empty);
				v.insert(v.end(), super.begin(), super.end());
				super = v;
			}
			bridge = execute(super.at(0));
			for(unsigned int i = 1; i < super.size(); i = i+2)
			{
				if(super.at(i).at(0) == ";")
				{
					if(super.at(i+1).at(0) == "exit")
					{
						delete[] cmd_str;
						return 0;
					}
					bridge = execute(super.at(i+1));
				}
				else if(super.at(i).at(0) == "||")
				{
					if(bridge == false)
					{
						if(super.at(i+1).at(0) == "exit")
						{
							delete[] cmd_str;
							return 0;
						}
						bridge = execute(super.at(i+1));
					}
				}
				else if(super.at(i).at(0) == "&&")
				{
					if(bridge)
					{
						if(super.at(i+1).at(0) == "exit")
						{
							delete[] cmd_str;
							return 0;
						}
						bridge = execute(super.at(i+1));
					}
				}

			}
			delete[] cmd_str;
			goto begin;
		}
		delete[] cmd_str;
	}
}

