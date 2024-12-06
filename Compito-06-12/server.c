#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "header.h"

typedef struct {
    int buffer[MAX_VALUES];
    // TODO aggiungere le variabili per la gestione circolare
    // e per la sincronizzazione dei thread
    
    int testa;
    int coda;
    int count;

    pthread_mutex_t mutex;
    pthread_cond_t cv_consumatore;
    pthread_cond_t cv_produttore;

} data_str;

typedef struct {
    int somma;
    int num_valori_sommati;
    // TODO aggiungere le variabili per la sincronizzazione dei thread

    pthread_mutex_t mutex;
    pthread_cond_t ok_to_print;
} somma_valori;

//struttura per passare i parametri somma_valori e data_str ai thread
typedef struct {
    somma_valori* sum;
    data_str* buf;
} parametri;


// le code
int queue_req;
int queue_res;

int consuma(data_str* buf) {
    int value;
    //TODO consumatore, con gestione coda circolare
    pthread_mutex_lock(&(buf->mutex));
    while(buf->count == 0){
        pthread_cond_wait(&(buf->cv_consumatore), &(buf->mutex));
    }

    value = buf->buffer[buf->coda];
    buf->coda = (buf->coda +1) % MAX_VALUES;
    buf->count--;

    pthread_mutex_unlock(&(buf->mutex));
    pthread_cond_signal(&(buf->cv_produttore));
    return value;
}

void produci(data_str* buf, int new_value) {
    //TODO produttore, con gestione coda circolare
    pthread_mutex_lock(&(buf->mutex));
    while (buf->count == MAX_VALUES){
        pthread_cond_wait(&(buf->cv_produttore), &(buf->mutex));
    }
    
    buf->buffer[buf->testa] = new_value;
    buf->testa = (buf->testa + 1)%MAX_VALUES;
    buf->count++;

    pthread_mutex_unlock(&(buf->mutex));
    pthread_cond_signal(&(buf->cv_consumatore));
}


void * stampa_somma(void* par) {
    
    somma_valori * s = (somma_valori*) par;
    
    //TODO attesa che siano stati sommati NUM_CONS valori utilizzando una condition
    // e stampa della somma una volta verificata la condizione
    pthread_mutex_lock(&(s->mutex));
    while (s->num_valori_sommati != NUM_CONS){
        pthread_cond_wait(&(s->ok_to_print), &(s->mutex));
    }
    
    printf("La somma totale è: %d\n", s->somma);
    pthread_mutex_unlock(&(s->mutex));
    pthread_exit(NULL);
}

void * produttore(void* par) {
    data_str * buf = (data_str*) par;
    int new_value;
    int i;
    for (i = 0; i < NUM_UPDATES*MAX_VALUES; i++) {
        new_value = rand()%10+1;
        printf("PRODUTTORE: inserimento nuovo dato: %d\n",new_value);
        produci(buf,new_value);
        sleep(rand()%3+1);
    }
    pthread_exit(NULL);
}


void * consumatore(void* par) {
    parametri * p = (parametri *) par;
    data_str* buf = p->buf;
    somma_valori* sum = p->sum;
    int i;
    for (i = 0; i < NUM_CONS; i++) {
        req msg;
        //TODO ricezione messaggio di richiesta
        msgrcv(queue_req, &msg, sizeof(long), MSG_TYPE_REQUEST, 0);
        printf("CONSUMATORE_SERV: ricevuta richiesta di consumo\n");

        res risp;
        //TODO preparazione messaggio di risposta
        //prelevando il valore dalla funzione "consuma"
        risp.type = MSG_TYPE_RESPONSE;
        risp.value = consuma(buf);
        printf("CONSUMATORE_SERV: invio valore al consumatore client %d\n",risp.value);
        //TODO invio risposta
        msgsnd(queue_res, &risp, sizeof(res)-sizeof(long), 0);
        
        //TODO aggiornamento in mutua esclusione della somma in 'sum' con il nuovo valore appena consumato
        //TODO e notifica della condizione quando sommati NUM_CONS elementi
        pthread_mutex_lock(&(sum->mutex));
        sum->num_valori_sommati++;
        sum->somma+= risp.value;
        if (sum->num_valori_sommati == NUM_CONS)
        {
            pthread_cond_signal(&(sum->ok_to_print));
        }
        pthread_mutex_unlock(&(sum->mutex));
    }

    pthread_exit(NULL);
}

int main() {

    //TODO: completare l'inizializzazione code
    key_t msg_req_key = ftok("./", 'q');
	key_t msg_res_key = ftok("./", 'r');

    queue_req = msgget(msg_req_key, 0|0664);
	queue_res = msgget(msg_res_key, 0|0664);


    pthread_t prod, cons, sum;

    srand(time(NULL));

    somma_valori* s = (somma_valori*) malloc(sizeof(somma_valori));
    // TODO inizializzazione campi di s
    pthread_mutex_init(&(s->mutex), NULL);
    pthread_cond_init(&(s->ok_to_print), NULL);
    s->num_valori_sommati = 0;
    s->somma = 0;


    data_str* shared_buf = (data_str*) malloc(sizeof(data_str));
    // TODO inizializzazione campi di shared_buf
    pthread_cond_init(&(shared_buf->cv_consumatore), NULL);
    pthread_cond_init(&(shared_buf->cv_produttore), NULL);
    pthread_mutex_init(&(shared_buf->mutex), NULL);
    shared_buf->coda = 0;
    shared_buf->testa = 0;
    shared_buf->count = 0;

    parametri * par = (parametri *) malloc(sizeof(parametri));
    // TODO inizializzazione campi di par
    par->buf = shared_buf;
    par->sum = s;

    //TODO creazione dei tre thread prod cons e sum
    /* nota: al thread produttore basta passare solo il buffer 'shared_buf'
             al thread stampa basta passare solo la somma 's'
             al thread consumatore bisogna passare entrambi tramite 'par'
    */
    if(pthread_create(&(prod), NULL, produttore, (void *) shared_buf) == 0){
        printf("Thread produttore correttamente creato!\n");
    }
    if(pthread_create(&(sum), NULL, stampa_somma, (void *) s) == 0){
        printf("Thread sommatore correttamente creato!\n");
    }
    if(pthread_create(&(cons), NULL, consumatore, (void *) par) == 0){
        printf("Thread consumatore correttamente creato!\n");
    }

    //TODO join 
    if(pthread_join(prod, NULL) == 0){
        printf("Thread produttore correttamente terminato\n");
    }
    if(pthread_join(cons, NULL) == 0){
        printf("Thread consumatore correttamente terminato\n");
    }
    if(pthread_join(sum, NULL) == 0){
        printf("Thread somma correttamente terminato\n");
    }
    
    //TODO rimozione code e strutture dati
    msgctl(queue_req, IPC_RMID, 0);
	msgctl(queue_res, IPC_RMID, 0);
    free(s);
    free(shared_buf);
    free(par);
    return 0;
}
