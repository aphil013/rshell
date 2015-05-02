COMPILER = g++
CFLAGS = -Wall -Werror -pedantic -ansi --std=c++11
RESULT1 = ./bin/rshell
RESULT2 = ./bin/ls
DIRECTORY1 = ./src/rshell.cpp
DIRECTORY2 = ./src/ls.cpp
all:
	mkdir -p ./bin
	$(COMPILER) $(CFLAGS) $(DIRECTORY1) -o $(RESULT1)
	$(COMPILER) $(CFLAGS) $(DIRECTORY2) -o $(RESULT2)
rshell:
	mkdir -p ./bin
	$(COMPILER) $(CFLAGS) $(DIRECTORY1) -o $(RESULT1)
ls:
	mkdir -p ./bin
	$(COMPILER) $(CFLAGS) $(DIRECTORY2) -o $(RESULT2)
clean:
	rm -rf ./bin

