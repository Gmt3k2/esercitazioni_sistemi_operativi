#include "common.h"

//succede che quando chiamo la exit le code muoiono da entrambe le parti ma il processo rimane
void sender(int msg_queue1, int msg_queue2){
    Messaggio msg;

    while(1){
        printf("coda 1: %d\n", msg_queue1);
        printf("coda 2: %d\n",msg_queue2);
    
        printf("Inserisci il messaggio:");
        scanf("%s", msg.buf);
        if(strcmp(msg.buf, "exit") == 0){
            msg.messageType = MSG_EXIT;
            msgsnd(msg_queue2, &msg, DIM_MESS, 0);
            msgsnd(msg_queue1, &msg, DIM_MESS, 0);
            return;
        }
        else{
            msg.messageType = MSG_BUF;
            int res = msgsnd(msg_queue1, &msg, DIM_MESS, 0);
        }
    }
    return;
}

void receiver(int msg_queue){
    Messaggio msg;

    while(1){
        int res = msgrcv(msg_queue, &msg, DIM_MESS, 0, 0);
        if(res < 0){   
            return;
        }
        if(msg.messageType == MSG_EXIT){
            printf("Processo terminato\n");
            return;
        }
        else{
            printf("[+]  %s\n", msg.buf);
        }
        
    }
    printf("il ricevente è terminato\n");
    return;
}