#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


int main() {
	char buffer[128];
	pid_t pid;
	int n;
	char* args[10];
	char* token;

	while(1){

		printf("Shell> ");
		//scanf("%127s\n", buffer); // scanf non legge l'intera stringa se contiene spazi
		fgets(buffer, 127, stdin);
		buffer[strlen(buffer) - 1] = '\0';

		printf("buffer='%s'\n", buffer);


		token = strtok(buffer, " ");

		args[0] = token;
		printf("ARG 0: '%s'\n", args[0]);

		n = 1;
		while(token != NULL && n<10) {

			token = strtok(NULL, " ");

			args[n] = token;
			printf("ARG %d: '%s'\n", n, args[n]);

			n++;
		}

		args[9] = (char *)0;

		if(!strcmp(args[0], "exit")){
			return 0;//exit(0);
		}


		pid = fork();

		if(pid == -1){
			//printf("Errore: fork non riuscita!\n");
			perror("Errore: fork non riuscita!");
			return 0;//exit(0);
		}
		else if(pid == 0){
			execvp(args[0], args);
			//printf("Errore: exec non riuscita!\n");
			perror("Errore: exec non riuscita!");
		}
		else{
			printf("Aspetto il figlio (%d)\n", pid);
			pid = wait(NULL);
			printf("Figlio (%d) terminato\n", pid);
		}
	}

	return 0;
}
