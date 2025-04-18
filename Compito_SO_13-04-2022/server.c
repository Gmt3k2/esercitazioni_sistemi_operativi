#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <stdlib.h>
#include "header.h"

typedef struct {
    int value;
    // TODO completare con variabili per la sincronizzazione
    pthread_mutex_t mutex;
} data_str;

typedef struct {
    int queue_req;
    int queue_res;
    data_str* data;
} param;

void * update_worker(void* par) {
    
    //TODO prelievo parametro 'par'
    param * data = (param *)par;
    int i;
    for (i = 0; i < NUM_UPDATES; i++) {
        req msg;
        // TODO: ricezione del 'msg' di richiesta update
        msgrcv(data->queue_req, &msg, sizeof(req)-sizeof(long), MSG_UPDATE_REQUEST, 0);
        
        int value = msg.num;
        
        printf("UPDATE WORKER: ricevuta richiesta, valore: %d\n",value);
        // TODO: aggiornamento del dato condiviso in mutua esclusione
        pthread_mutex_lock(&data->data->mutex);

        data->data->value += value;
        pthread_mutex_unlock(&data->data->mutex);
    }
    pthread_exit(NULL);
}

void * reader_worker(void* par) {
    param* p = (param*) par;

    int i;
    for (i = 0; i < NUM_READS*NUM_READERS; i++) {
        req msg;
        // TODO: ricezione del 'msg' di richiesta lettura
        if(msgrcv(p->queue_req, &msg, sizeof(req)-sizeof(long), MSG_READ_REQUEST, 0) <=0 ){
            printf("ERRORE NELLA RICEZIONE DELLE RICHIESTE DI LETTURA\n");
        }
        
        int pid = msg.pid;
        printf("READER WORKER: ricevuta richiesta di lettura da: %d\n",pid);
        
        res risp;
        //TODO: preparazione e invio del messaggio di risposta
        risp.pid = msg.pid;
        pthread_mutex_lock(&p->data->mutex);
        risp.num = p->data->value;
        pthread_mutex_unlock(&p->data->mutex);
        msgsnd(p->queue_res, &risp, sizeof(res)-sizeof(long), 0);
    }
    pthread_exit(NULL);
}

int main() {

    key_t msg_req_key = ftok("./", 'q');
	key_t msg_res_key = ftok("./", 'r');

	int queue_req = msgget(msg_req_key, 0);
	int queue_res = msgget(msg_res_key, 0);

    pthread_t upd, rd;

    // TODO: alloacre la struttura dati condivisa
    data_str* data = (data_str*)malloc(sizeof(data_str));
    data->value = 0;
    pthread_mutex_init(&data->mutex, NULL);
    

    param p;
    // TODO: preparare il parametro di scambio 'p'
    p.data = data;
    p.queue_req = queue_req;
    p.queue_res = queue_res;

    /* TODO: creare un tread update_worker e un thread
            reader_worker passando a entrambi 'p'*/
    if(pthread_create(&upd, NULL, update_worker, (void*) &p) == 0);
        printf("Thread worker updater creato correttamente!\n");

    if(pthread_create(&rd, NULL, reader_worker, (void*) &p) == 0);
        printf("Thread worker updater creato correttamente!\n");
    
    // TODO: completare con la join sui thread
    if(pthread_join(upd, NULL) == 0)
        printf("Thread updater terminato correttamente!\n");

    if(pthread_join(rd, NULL) == 0)
        printf("Thread reader terminato correttamente!\n");

    // TODO: completare con la rimozione della struttura dati e delle code
    free(data);
    pthread_mutex_destroy(&data->mutex);
    msgctl(msg_req_key, IPC_RMID, 0);
    msgctl(msg_res_key, IPC_RMID, 0);

    return 0;
}