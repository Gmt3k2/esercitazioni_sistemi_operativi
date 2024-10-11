clean:
	rm shell.o shell

all: shell

shell: shell.o
	@echo "producing exe"
	@gcc -o shell shell.o

shell.o: shell.c
	@echo "compiling"
	@gcc -c shell.c