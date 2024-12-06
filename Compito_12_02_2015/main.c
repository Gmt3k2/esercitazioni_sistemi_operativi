#include "procedure.h"


int main(void){

    pthread_t threads[6];

    MonitorElaborazioni* m = (MonitorElaborazioni*) malloc(sizeof(MonitorElaborazioni));

    pthread_mutex_init(&(m->mutex), NULL);
    pthread_cond_init(&(m->cv_consumatore), NULL);
    pthread_cond_init(&(m->cv_produttore), NULL);

    for (int i = 0; i < 5; i++){
        m->stato[i] = LIBERO;
    }
    m->numero_liberi = 5;
    m->numero_occupati = 0;

    printf("Risorse inizializzate\n");
    //creazione thread
    for (int i = 0; i < 6; i++){
        if (i<2){
            if(pthread_create(&(threads[i]), NULL, elaboratore, (void *) m) != 0){
                perror("Errore nella creazione thread elaboratore!");
            }
        }
        else{
            if(pthread_create(&(threads[i]), NULL, richiedente, (void *) m) != 0){
                perror("Errore nella creazione thread richiedente!");
            }

        }
    }
    
    for (int i = 0; i < 6; i++){
        
        if(pthread_join(threads[i], NULL) != 0)
            perror("Errore nella fase di join!");
        
        if (i<4)
            printf("Terminato il richiedente\n");
        else
            printf("Terminato l'elaboratore\n");
    }

    pthread_mutex_destroy(&(m->mutex));
    pthread_cond_destroy(&(m->cv_consumatore));
    pthread_cond_destroy(&(m->cv_produttore));

    pthread_exit(NULL);
    free(m);
    return 0;
}