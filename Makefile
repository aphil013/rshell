COMPILER = g++
CFLAGS = -Wall -Werror -pedantic -ansi --std=c++11
RESULT = ./bin/rshell
DIRECTORY = ./src/main.cpp
all:
	mkdir -p ./bin
	$(COMPILER) $(CFLAGS) $(DIRECTORY) -o $(RESULT)

rshell:
	mkdir -p ./bin
	$(COMPILER) $(CFLAGS) $(DIRECTORY) -o $(RESULT)
clean:
	rm -rf ./bin

