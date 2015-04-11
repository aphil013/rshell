#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char** argv)
{
	std::string cmd_line;
	while(1)
	{
		std::cout << "$ ";
		std::cin >> cmd_line;
		if(cmd_line.find("exit") != -1)
		{
			return 0;
		}
		int pid = fork();
		if(pid == -1)
		{
			perror("fork");
			exit(1);
		}
		else if(pid == 0)
		{
			execvp("cmd_line", argv);
		}
	}	
}


