#include "procedure.h"

void* produttore(void*);
void* consumatore(void*);

int main(void){

    pthread_t th_produttori[4];
    pthread_t th_consumatori[2];

    GestioneIO* g = (GestioneIO*)malloc(sizeof(GestioneIO));
    Inizializza(g);

    for (int i = 0; i < 4; i++)
    {
        if(pthread_create(&th_produttori[i], NULL, produttore, (void*) g) == 0)
            printf("Thread produttore creato correttamente!\n");
    }
    
    for (int i = 0; i < 2; i++)
    {
        if(pthread_create(&th_consumatori[i], NULL, consumatore, (void*) g) == 0)
            printf("Thread produttore creato correttamente!\n");
    }



    for (int i = 0; i < 4; i++)
    {
        if(pthread_join(th_produttori[i], NULL) == 0)
            printf("Thread produttore terminato correttamente!\n");
    }
    
    for (int i = 0; i < 2; i++)
    {
        if(pthread_join(th_consumatori[i], NULL) == 0)
            printf("Thread consumatore terminato correttamente!\n");
    }


    pthread_mutex_destroy(&g->mutex);
    pthread_cond_destroy(&g->cv_produttore);
    free(g);
    return 0;
}

void* produttore(void* x){
    GestioneIO* g = (GestioneIO*)x;
    Buffer b;
    b.indirizzo = rand()%11;
    b.dato = rand()%11;
    srand(time(NULL));
    for (int i = 0; i < 3; i++){
        Produci(g, &b);
        b.indirizzo++;
        b.dato++;
        sleep(1);
    }
    return NULL;
}
void* consumatore(void* x){
    GestioneIO* g = (GestioneIO*)x;
    Buffer b;
    for (int i = 0; i < 4; i++){
        while(Consuma(x, &b) != 1){
            printf("[CONSUMATORE] la coppia di valori prelevati è: (%d, %d)\n", b.dato, b.indirizzo);
        }
        printf("[CONSUMATORE] Buffer vuoto\n");
        sleep(3);
    }
    return NULL;
}
