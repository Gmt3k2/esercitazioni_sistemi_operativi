#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    int pid;
    //TODO: completare l'inizializzazione code
    int key_queue_off = ftok("./", 'o');
    int key_queue_risp = ftok("./", 'r');
    int mq_offerte = msgget(key_queue_off, IPC_CREAT|0664);
    int mq_risposte = msgget(key_queue_risp, IPC_CREAT|0664);

    pid = fork();
    if (pid==0) {
        //TODO: exec del server_asta
        execl("./server_asta", "server_asta", NULL);
        exit(0);
    }

    pid = fork();
    if (pid==0) {
        //TODO: exec del client_asta
        execl("./client_asta", "client_asta", NULL);
        exit(0);
    }

    //TODO: attesa processi figli
    for (int i = 0; i < 2; i++){
        wait(NULL);
    }
    
    //TODO: eliminare le code
    msgctl(key_queue_off, IPC_RMID, 0);
    msgctl(key_queue_risp, IPC_RMID, 0);

    return 0;
}
