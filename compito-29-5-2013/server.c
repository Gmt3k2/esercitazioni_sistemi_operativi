#include "procedure.h"

void * manager(void * );
void * worker(void * );
void produzione(BufferManagerWorker*, Messaggio);
int consumazione(BufferManagerWorker*, int*);

int id_request;
int id_response;
pthread_t thread_manager;
pthread_t threads_worker[NUM_WORKER];
int main(void){

    key_t key_query = ftok("./", 'q');
    key_t key_response = ftok("./", 'r');

    id_request = msgget(key_query, 0);
    id_response = msgget(key_response, 0);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    BufferManagerWorker* buffer = (BufferManagerWorker*) malloc(sizeof(BufferManagerWorker));

    pthread_mutex_init(&(buffer->mutex), NULL);
    pthread_cond_init(&(buffer->cv_consumatore), NULL);
    pthread_cond_init(&(buffer->cv_produttore), NULL);

    //inizializzazione vettore di stato
    for (int i = 0; i < NUM_CLIENT; i++){
        buffer->buffer_stati[i] = DISPONIBILE;
    }
    buffer->numero_liberi = NUM_CLIENT;
    buffer->numero_occupati = 0;
    


    if(pthread_create(&thread_manager, NULL, manager, (void *) buffer) == 0)
        printf("Thread manager creato correttamente!\n");    

    for (int i = 0; i < NUM_WORKER; i++){
        if(pthread_create(&threads_worker[i], &attr, worker, (void *) buffer) == 0)
            printf("Thread worker creato correttamente!\n");        
    }

    if(pthread_join(thread_manager, NULL) == 0){
        printf("Thread manager terminato correttamente!\n");
    }
    pthread_attr_destroy(&attr);
    
    pthread_mutex_destroy(&(buffer->mutex));
    pthread_cond_destroy(&(buffer->cv_consumatore));
    pthread_cond_destroy(&(buffer->cv_produttore));
    free(buffer);
}



void * manager(void * x){

    BufferManagerWorker* b = (BufferManagerWorker*) x;
    while(1){
        Messaggio msg;
        if(msgrcv(id_request, &msg, DIM_MESSAGGIO, 0, IPC_NOWAIT)>0){
            printf("Ho ricevuto %d,%d\n",msg.num1,msg.num2);
            if(msg.num1 == -1 && msg.num2 == -1){
                pthread_cancel(threads_worker[0]);
                pthread_cancel(threads_worker[1]);
                //pthread_exit(NULL);
                return NULL;
            }
            else{
                produzione(b, msg);
                printf("Ho effettivamente messo i messaggi nella coda dei worker\n");
            }
        }
        else{
            sleep(1);
            printf("Attesa...\n");
        }
    }

    
}

void * worker(void * x){
    BufferManagerWorker* b = (BufferManagerWorker*) x;
    while (1){
        Messaggio msg;
        int pid;
        int prod = consumazione(b, &pid);
        msg.num1 = prod;
        msg.num2 = prod; //questo valore non viene comunque usato
        msg.type = pid;
        printf("IL PID è: %d\n", pid);
        if(msgsnd(id_response, &msg, DIM_MESSAGGIO, 0) == 0)
            printf("Risultato della moltiplicazione inviato correttamente !\n");
    }
    

    return NULL;
}

void produzione(BufferManagerWorker* b, Messaggio msg){
    pthread_mutex_lock(&(b->mutex));
    
    while(b->numero_occupati == NUM_CLIENT){
        pthread_cond_wait(&(b->cv_produttore), &(b->mutex));
    }
    
    //una volta che sono sicuro ci sia uno spazio libero lo vado a trovare
    int pos = 0;
    for (int i = 0; i < NUM_CLIENT; i++){
        if(b->buffer_stati[i] == DISPONIBILE)
            pos = i;
            break;
    }
    b->buffer_stati[pos] = IN_USO;
    b->numero_liberi--;

    pthread_mutex_unlock(&(b->mutex));

    b->messaggi[pos] = msg;

    pthread_mutex_lock(&(b->mutex));
    b->buffer_stati[pos] = OCCUPATO;
    b->numero_occupati++;
    
    pthread_cond_signal(&(b->cv_consumatore));
    pthread_mutex_unlock(&(b->mutex));
}

int consumazione(BufferManagerWorker* b, int* pid){
    pthread_mutex_lock(&(b->mutex));
    while (b->numero_occupati == 0){
        pthread_cond_wait(&(b->cv_consumatore), &(b->mutex));
    }
    int pos = 0;
    for (int i = 0; i < NUM_CLIENT; i++){
        if(b->buffer_stati[i] == OCCUPATO)
            pos = i;
            break;
    }
    b->buffer_stati[pos] = IN_USO;
    b->numero_occupati--;
    pthread_mutex_unlock(&(b->mutex));

    int product = b->messaggi[pos].num1 * b->messaggi[pos].num2;
    *pid = b->messaggi[pos].type;
    pthread_mutex_lock(&(b->mutex));
    b->buffer_stati[pos] = DISPONIBILE;
    b->numero_liberi++;

    pthread_cond_signal(&(b->cv_produttore));
    pthread_mutex_unlock(&(b->mutex));    
    return product;
}