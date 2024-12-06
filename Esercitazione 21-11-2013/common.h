#ifndef __COMMON
#define __COMMON

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_LEN 60
#define DIM_MESS sizeof(Messaggio)-sizeof(long)

#define MSG_BUF 1
#define MSG_EXIT 2


typedef struct
{
    long messageType;
    char buf[MAX_LEN];
}Messaggio;

void sender(int msg_queue1,int msg_queue2);
void receiver(int msg_queue1);





#endif