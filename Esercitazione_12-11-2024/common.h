#ifndef _COMMON_
#define _COMMON_

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

#define BUFFER_DIM 10

/* Definizione Tipi di Messaggi*/
#define MSG_CLIENT_SERVER 1
#define MSG_SERVER_PRINTER 2 
#define MSG_EXIT 3

/* Definizione Messaggio Richiesta del Client */
typedef struct{
    long message_type;
    pid_t msgProcesso;
}msgRichiestaClient;

/* Definizione Messaggio Richiesta del Server, contenente il buffer */
typedef struct{
    long message_type;
    pid_t pidBuffer[BUFFER_DIM];
}msgRichiestaServer;


/* Prototipi delle funzioni dei processi */
void printer(int msgq_print);
void server(int msgq_guest, int msgq_print);
void client(int msgq_guest);

#endif
