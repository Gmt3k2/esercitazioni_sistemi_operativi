#include "procedure.h"

void* stampa(void * v){
    pthread_exit(NULL);
}

void* cliente(void * v){
    


    pthread_exit(NULL);
}

int main(void){
    pthread_t threads[5];

    struct Monitor* catalogo = (struct Monitor*) malloc(sizeof(struct Monitor));
    //pthread_cond_init(catalogo->) inizializzazione condizioni

    for (int i = 0; i < 5; i++)
    {
        if(i < 4){
            if(pthread_create(threads[i], NULL, cliente, (void *) 0) == 0)
                printf("Thread[%d] creato correttamente\n", i);
        }
        else{
            if(pthread_create(threads[i], NULL, stampa, (void*) 0) == 0)
                printf("Thread di stampa creato correttamente\n");
        }
    }
    

    for (int i = 0; i < 5; i++)
    {
        if(pthread_join(threads[i], NULL) == 0){
            if(i<4)
                pritnf("Terminato thread cliente\n");
            else
                pirntf("Terminato thread di stampa\n");
        }
    }
    

    free(catalogo);
    return 0;
}