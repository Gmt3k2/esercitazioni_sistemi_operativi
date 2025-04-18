#include "procedure.h"

int main(void){

    key_t key_query = ftok("./", 'q');
    key_t key_response = ftok("./", 'r');

    int id_request = msgget(key_query, IPC_CREAT|0664);
    int id_response = msgget(key_response, IPC_CREAT|0664);

    pid_t pid;
    for (int i = 0; i < NUM_CLIENT; i++)
    {
        pid = fork();
        if(pid < 0)
            perror("Error:");
        else if(pid == 0){
            execl("./client", "client", NULL);
            exit(0);
        }
    }
    
    pid = fork();
    if(pid < 0)
        perror("Error:");
    else if(pid == 0){
        execl("./server", "server", NULL);
        exit(0);
    }

    for (int i = 0; i < NUM_CLIENT; i++){
        wait(NULL);
        
    }
   // sleep(3);
    
    printf("I processi client sono tutti terminati, chiudo il manager\n");

    Messaggio msgFine;
    msgFine.num1 = -1;
    msgFine.num2 = -1;
    msgFine.type = MESSAGGIO_TERMINAZIONE;
    if(msgsnd(id_request, &msgFine, DIM_MESSAGGIO, 0) == 0)
        printf("Messaggio di terminazione inviato correttamente\n");

    wait(NULL);

    //distruzione delle code;
    msgctl(id_request, IPC_RMID, 0);
    msgctl(id_response, IPC_RMID, 0);
    return 0;
}