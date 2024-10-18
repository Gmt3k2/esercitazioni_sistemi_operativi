#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
	char buffer[128];
	pid_t pid;
	int n;
	char* args[10];
	char* token;
	
	while(1){

		int isBackgorund = 0;
		printf("Shell> ");
		//scanf("%127s\n", buffer); // scanf non legge l'intera stringa se contiene spazi
		fflush(stdin);
		fgets(buffer, 127, stdin);
		if(buffer[0] == '\n'){continue;}
		buffer[strlen(buffer) - 1] = '\0'; //terminal char at the end of the buffer

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

		args[9] = (char *)0; //terminating char at the end of commands

		if(!strcmp(args[0], "exit")){
			return 0;//exit(0);
		}

		int i = 0;
		while(args[i] != NULL){
			if(strcmp(args[i], "&") == 0){
				isBackgorund = 1;
				args[i] = NULL;
			}
			i++;
		}
		printf("la flag del background è: %d\n", isBackgorund);
		
		pid = fork();

		if(pid == -1){
			perror("Errore: fork non riuscita!\n");
			return 0;//exit(0);
		}
		else if(pid == 0){
			//if I don't have the necessity of backgorund I'm going to keep it simple and not create another process
			if(isBackgorund == 0){
				execvp(args[0], args);
			}

			else if(isBackgorund == 1){
				pid_t pid1 = fork();

				if(pid1 > 0){
					//the father gets killed instantly ad nephew adopted by init
					exit(1);
				}
				else if(pid1 == 0){
					execvp(args[0], args);
				}
				else{
					perror("Errore: fork nel caso di backgorund non riuscita\n");
				}
			}

		}
		else{
			printf("Aspetto il figlio (%d)\n", pid);
			pid = waitpid(pid, NULL, 0);
			printf("Figlio (%d) terminato\n", pid);
		}
	}

	return 0;
}