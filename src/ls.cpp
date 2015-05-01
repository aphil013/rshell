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

bool compare(const string& s1, const string& s2)	// Comparison for sort function
{
	return strcasecmp(s1.c_str(), s2.c_str()) <= 0;
}

vector<string> listDir(int flag, string init)
{
	vector<string> dir;
	vector<string> files;
	vector<struct stat> info;

	struct stat statbuf;
	struct stat lstatbuf;
	struct pswd *pwd;
	struct group *grp;

	string dir_path(init);
	dir_path.append("/"); // Format for directory output
	string copy(dir_path);

	const char* directory_name = init.c_str();
	DIR *dir_ptr = opendir(directory_name);
	if(dir_ptr == NULL)
	{
		perror("opendir"); // Error checking
		return dir;
	}
	
	dirent *dirent_ptr;
	while(dirent_ptr = readdir(dir_ptr))
		files.push_back(dirent_ptr->d_name);
	
	if(errno != 0)
	{
		perror("readdir"); // Error while reading directory
		exit(1); // Exit after error
	}

	sort(files.begin(), files.end(), compare);

	for(unsigned int i = 0; i < files.size(); ++i)
	{
		dir_path.append(files.at(i));
		if((lstat(dir_path.c_str(), &lstatbuf) == -1))
		{
			perror("lstat");
			exit(1);
		}
		
		if(S_ISLNK(lstatbuf.st_mode))
		{
			info.push_back(lstatbuf);
			dir_path = copy;
			continue;
		}

		if((stat(dir_path.c_str(), &statbuf)) == -1)
		{
			perror("stat");
			exit(1);
		}
		
		info.push_back(statbuf);

		if((files.at(i) != ".") && (files.at(i) != "..") && (S_ISDIR(statbuf.st_mode)))
		{
			dir.push_back(dir_path);
		}
		dir_path = copy;
	}

	return dir;
}

void allDir(int flag, string init)
{
	cout << init << ":" << endl;

	vector<string> dir = listDir(flag, init);

	if(dir.empty())
		return;
	cout << endl;

	for(unsigned int i = 0; i < dir.size(); ++i) // Recursion happens here
	{
		allDir(flag, dir.at(i));
	}
	return;
}

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