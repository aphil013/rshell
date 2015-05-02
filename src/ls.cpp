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
	struct passwd *pwd;
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
	while((dirent_ptr = readdir(dir_ptr)))
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
			if((files.at(i)[0] == '.') && !(flag & FLAG_a));
			dir.push_back(dir_path);
		}
		dir_path = copy;
	}
	// Determine which type of output is to follow
	
	if((flag & FLAG_a) && !(flag & FLAG_l)) // -a and not -l
	{
		unsigned width = 80;	// Output 
		unsigned longest = 0;	// format

		for(unsigned int i = 0; i < files.size(); ++i)
		{
			if(files.at(i).size() > longest)
				longest = files.at(i).size();
		}
		longest += 1; // Space b/w largest entries

		unsigned columns = width/longest;
		unsigned curr = 0;

		for(unsigned int i = 0; i < files.size(); ++i)
		{
			dir_path.append(files.at(i));
			if((stat(dir_path.c_str(), &statbuf)) == -1)
			{
				perror("stat");
				exit(1);
			}

			if(files.at(i)[0] == '.') // If hidden file
			{
				cout << H_COLOR;
			}
			if(S_ISDIR(statbuf.st_mode)) // If directory
			{
				cout << D_COLOR;
				cout << files.at(i); // << "/";
				curr = files.at(i).size() + 1;
			}
			else
			{
				if(statbuf.st_mode & S_IEXEC) // If executable
				{
					cout << E_COLOR;
				}
				cout << files.at(i);
				curr = files.at(i).size();
			}

			cout << RESET_C; // Reset color output
			cout << string(longest - curr, ' ');
			columns--;
			if(columns == 0)
			{
				cout << endl;
				columns = width/longest;
			}
			dir_path = copy;
		}
		cout << endl;

	}

	else if(flag & FLAG_l) // -l
	{
		int block = 0;
		for(unsigned int i = 0; i < files.size(); ++i)
		{
			if(!(flag & FLAG_a) && files.at(i)[0] == '.')
				continue;
			else
				block += info.at(i).st_blocks;
		}
		cout << "total: " << block/2 << endl;

		for(unsigned int i = 0; i < files.size(); ++i)
		{
			if((files.at(i)[0] == '.') && !(flag & FLAG_a));
			else
			{
				if(S_ISDIR(info.at(i).st_mode))
					cout << 'd';
				else if(S_ISLNK(info.at(i).st_mode))
					cout << 'l';
				else
					cout << '-';

				//Permissions
				
				//user
				if(info.at(i).st_mode & S_IRUSR)
					cout << 'r';
				else cout << '-';
				if(info.at(i).st_mode & S_IWUSR)
					cout << 'w';
				else cout << '-';
				if(info.at(i).st_mode & S_IXUSR)
					cout << 'x';
				else cout << '-';

				//Group
				if(info.at(i).st_mode & S_IRGRP)
					cout << 'r';
				else cout << '-';
				if(info.at(i).st_mode & S_IWGRP)
					cout << 'w';
				else cout << '-';
				if(info.at(i).st_mode & S_IXGRP)
					cout << 'x';
				else cout << '-';

				//Other
				if(info.at(i).st_mode & S_IROTH)
					cout << 'r';
				else cout << '-';
				if(info.at(i).st_mode & S_IWOTH)
					cout << 'w';
				else cout << '-';
				if(info.at(i).st_mode & S_IXOTH)
					cout << 'x';
				else cout << '-';

				cout << " " << info.at(i).st_nlink << " ";

				pwd = getpwuid(info.at(i).st_uid);
				cout << pwd->pw_name << " ";

				grp = getgrgid(info.at(i).st_gid);
				cout << grp->gr_name << " ";

				cout << info.at(i).st_size << " ";

				time_t time = info.at(i).st_mtime;
				string time_s = ctime(&time);
				cout << time_s.substr(4, 12) << " ";

				if(files.at(i)[0] == '.')
					cout << H_COLOR;
				if(S_ISDIR(info.at(i).st_mode))
				{
					cout << D_COLOR;
					cout << files.at(i);
				}
				else
				{
					if(S_ISLNK(info.at(i).st_mode));
					else if(info.at(i).st_mode & S_IEXEC)
						cout << E_COLOR;

					cout << files.at(i);
				}

				cout << RESET_C;

				if(!(i + 1 == files.size()))
					cout << endl;
			}
		}
		cout << endl;
	}

	else
	{
		unsigned width = 80;
		unsigned longest = 0;

		for(unsigned int i = 0; i < files.size(); ++i)
		{
			if(files.at(i).size() > longest)
				longest = files.at(i).size();
		}

		longest += 2;
		unsigned columns = width/longest;
		unsigned curr = 0;

		for(unsigned int i = 0; i < files.size(); ++i)
		{
			if(files.at(i)[0] == '.')
				continue;
			else
			{
				dir_path.append(files.at(i));
				if((stat(dir_path.c_str(), &statbuf)) == -1)
				{
					perror("stat");
					exit(1);
				}

				if(S_ISDIR(statbuf.st_mode))
				{
					cout << D_COLOR;
					cout << files.at(i); // << "/";
					curr = files.at(i).size() + 1;
					cout << RESET_C;
				}

				else
				{
					if(statbuf.st_mode & S_IEXEC)
						cout << E_COLOR;

					cout << files.at(i);
					curr = files.at(i).size();
					cout << RESET_C;
				}

				cout << string(longest - curr, ' ');
				columns--;
				if(columns == 0)
				{
					columns = width/longest;
				}
				
				dir_path = copy;
			}

			if((columns == width/longest && columns != 0) || (i+1) == files.size())
				cout << endl;
		}
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

	for(int i = 1; i < argc; ++i)
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
