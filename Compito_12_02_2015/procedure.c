#include "procedure.h"

int ricerca_posizione_libera(MonitorElaborazioni* m){
    int j = 0;
    while (m->stato[j] != LIBERO){
        j++;
    }
    return j;
}

int ricerca_posizione_occupata(MonitorElaborazioni* m){
    int min = 5;
    int pos;
    for (int i = 0; i < 5; i++){
        if(m->stato[i] == OCCUPATO){
            if(m->elaborazioni[i].totale_operandi < min){
                min = m->elaborazioni[i].totale_operandi;
                pos = i;
            }
        }
    }
    return pos;
}

void* richiedente(void* arg){
    Buffer b;
    MonitorElaborazioni* m = (MonitorElaborazioni*) arg;
    for (int i = 0; i < 3; i++){
        srand(time(NULL) * SYS_gettid);
        b.totale_operandi = (rand()%3) +2;
        // printf("TOTALE OPERANDI: %d\n", b.totale_operandi);

        for (int j = 0; j < b.totale_operandi; j++){
            b.operandi[j] = rand()%11;
        }

        pthread_mutex_lock(&(m->mutex));
        printf("Il thread produttore ha acquisito il mutex\n");
        while(m->numero_liberi == 0){
            printf("Produzione sospesa\n");
            pthread_cond_wait(&(m->cv_produttore), &(m->mutex));
            printf("Riprendo produzione\n");
        }        
        int pos = ricerca_posizione_libera(m);

        m->stato[pos] = IN_USO;
        m->numero_liberi--;
        m->elaborazioni[pos] = b;
        m->stato[pos] = OCCUPATO;
        m->numero_occupati++;

        printf("Messaggio inserito nel vettore - fine produzione\n");
        pthread_mutex_unlock(&(m->mutex));
        pthread_cond_signal(&(m->cv_consumatore));
    }   
    pthread_exit(NULL);
    
}


void* elaboratore(void* arg){
    MonitorElaborazioni* m = (MonitorElaborazioni*) arg;
    for (int i = 0; i < 6; i++)
    {
        pthread_mutex_lock(&(m->mutex));

        while(m->numero_occupati == 0){
            printf("Consumazione sospesa\n");
            pthread_cond_wait(&(m->cv_consumatore), &(m->mutex));
            printf("Riprendo consumazione\n");
        }
        int pos = ricerca_posizione_occupata(m);

        m->stato[pos] = IN_USO;
        m->numero_occupati--;
        pthread_mutex_unlock(&(m->mutex));

        int somma = 0;
        sleep(m->elaborazioni[pos].totale_operandi);
        for(int i = 0; i < m->elaborazioni[pos].totale_operandi; i++){
            printf("Operando (%d): %d\n", i+1, m->elaborazioni[pos].operandi[i]);
            somma += m->elaborazioni[pos].operandi[i];
        }
        printf("La somma del vettore è: %d\n", somma);
        

        pthread_mutex_lock(&(m->mutex));
        m->stato[pos] = LIBERO;
        m->numero_liberi++;
        
        pthread_mutex_unlock(&(m->mutex));
        pthread_cond_signal(&(m->cv_produttore));
    }
    pthread_exit(NULL);
        
}