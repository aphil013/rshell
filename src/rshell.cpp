#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char** argv)
{
	char login[64]; 
	int chk = getlogin_r(login, sizeof(login)-1); 
	std::string name = login;
	chk = gethostname(login, sizeof(login)-1);
	std::string host = login;
	name += "@";
	std::string user = name + host;
	std::string cmd_line;
	while(1)
	{
		std::cout << user  << "$ ";
		std::cin >> cmd_line;
		char* cmd = const_cast<char*>(cmd_line.c_str());
		if(cmd_line == "exit")
		{
			return 0;
		}
		int pid = fork();
		if(pid == 0)
		{
			execvp(cmd, argv);
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
}


