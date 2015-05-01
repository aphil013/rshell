#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <ctime>

using namespace std;

#define FLAG_a 1		// Flag value definitons
#define FLAG_l 2
#define FLAG_R 4

#define D_COLOR "\x1b[38;5;27m" // Print color definitions
#define E_COLOR "\x1b[38;5;34m"
#define H_COLOR "\x1b[47m"
#define RESET_C "\x1b[0m"

int main(int argc, char** argv)
{
	int flag = 0;

	vector<string> files;

	for(unsigned int i = 1; i < argc; ++i)
	{
		if(argv[i][0] == '-') // Is a flag
		{
			for(int j =1; argv[i][j] != 0; ++j)
			{
				if(argv[i][j] == 'a')
					flag |= FLAG_a;
				else if(argv[i][j] == 'l')
					flag |= FLAG_l;
				else if(argv[i][j] == 'R')
					flag |= FLAG_R;
				else // Invalid flags were passed
				{
					cerr << "ls: invalid option -- \'" << argv[i][j] << "\'\n";
					exit(1);
				}
			}
		}
		else files.push_back(argv[i]);
	}

	sort(files.begin(), files.end(), compare);
	if(files.empty())
	{
		if(flag & FLAG_R)
			allDir(flag, ".");
		else
			listDir(flag, ".");
	}
	else
	{
		for(unsigned int i = 0; i < files.size(); ++i)
		{
			if(flag & FLAG_R)
			{
				allDir(flag, files.at(i));

				if(!(i+1 == files.size()))
					cout << endl;
			}
			else
			{
				if(files.size() > 1)
					cout << files.at(i) << ":" << endl;
				listDir(flag, files.at(i));

				if(!(i+1 == files.size()))
					cout << endl;
			}
		}
	}
	return 0;
}
