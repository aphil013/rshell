#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>						// DIR, DIRENT, OPENDIR,...
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <algorithm>					// Extra utilities
#include <ctime>
#include <time.h>
#include <iomanip>
#include <vector>

using namespace std;


int main(int argc, char** argv)
{
	//	cout << "\x1b[94m\x1b[100m";
	vector <string> arguments;
	
	arguments.push_back(".");

	bool files = false;

	unsigned int flag = 1;
	//flag represented by values
	//none, a, l, R, al, aR, lR, all
	//1,    2. 3. 5. 6.  10, 15, 30
	

	for (int i = 1; i < argc; i++)		// Checks all possible combinations of flags
	{
		if (!strcmp(argv[i] , "-a"))
		{
			flag *= 2;
		}

		else if (!strcmp(argv[i] , "-l"))
		{
			flag *= 3;
		}

		else if (!strcmp(argv[i], "-R"))
		{
			flag *= 5;
		}

		else if (!strcmp(argv[i], "-al"))
		{
			flag *= 6;
		}

		else if (!strcmp(argv[i], "-la"))
		{
			flag *= 6;
		}

		else if (!strcmp(argv[i], "-aR"))
		{
			flag *= 10;
		}

		else if (!strcmp(argv[i], "-Ra"))
		{
			flag *= 10;
		}

		else if (!strcmp(argv[i], "-lR"))
		{
			flag *= 15;
		}

		else if (!strcmp(argv[i], "-Rl"))
		{
			flag *= 15;
		}

		else if (!strcmp(argv[i], "-alR"))
		{
			flag *= 30;
		}
		
		else if (!strcmp(argv[i], "-aRl"))
		{
			flag *= 30;
		}

		else if (!strcmp(argv[i], "-Ral"))
		{
			flag *= 30;
		}
		
		else if (!strcmp(argv[i], "-laR"))
		{
			flag *= 30;
		}
		
		else if (!strcmp(argv[i], "-Rla"))
		{
			flag *= 30;
		}
		
		else if (!strcmp(argv[i], "-lRa"))			
		{
			flag *= 30;
		}

		else 
		{
			if (files == false)
			{
				files = true;
				string temp = argv[i]; //temp to store commands
				arguments.at(0) = temp;
			}
			else 
			{
				arguments.push_back(argv[i]);
			}
		}
	}

	return 0;
}
