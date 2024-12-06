#include "common.h"


int main(int argc, char* argv[]){

    key_t key_sender;
    key_t key_receiver;
    if(argc != 3){
        printf("Misuse of the program.\n");
        printf("Usage: ./chat [chiave_mittente] [chiave_ricevente]\n");
        return 1;
    }
    else{
        key_sender = ftok("./", argv[1][0]);
        key_receiver = ftok("./", argv[2][0]);
    }

    int key_queue_1 = msgget(key_sender, IPC_CREAT|0664);
    int key_queue_2 = msgget(key_receiver, IPC_CREAT|0664);

    if(key_queue_1 < 0 || key_queue_2 < 0){
        perror("Errore nella creazione code");
    }

    pid_t pid_receiver = fork();
    pid_t pid_sender;
    if (pid_receiver < 0){
        perror("Errore nella fork");
    }
    else if(pid_receiver == 0){
        receiver(key_queue_2);
        kill(pid_sender, 15);
        exit(0);
    }

    pid_sender = fork();
    if (pid_sender < 0){
        perror("Errore nella fork");
    }
    else if(pid_sender == 0){
        sender(key_queue_1, key_queue_2); 
        exit(0);
    }

    for (int i = 0; i < 2; i++)
    {
        wait(NULL);
    }
    
    msgctl(key_queue_1, IPC_RMID, 0);
    msgctl(key_queue_2, IPC_RMID, 0);
    return 0;
}