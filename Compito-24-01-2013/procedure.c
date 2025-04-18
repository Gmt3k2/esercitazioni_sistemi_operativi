#include "procedure.h"

int affitta(struct Monitor * m, int id_film){
    
    pthread_mutex_lock(&(m->mutex));
    while (m->contatori_copie[id_film] == 0)
    {
        pthread_cond_wait(&(m->attesa_su_film[id_film]), &(m->mutex));
    }
    int indice_film = id_film + m->contatori_copie[id_film] - 1;
    m->dvd[indice_film].stato = AFFITTATO;
    m->contatori_copie[id_film]--;

    pthread_cond_signal(&(m->attesa_su_film[id_film]));
    pthread_mutex_unlock(&(m->mutex));
    return m->dvd[indice_film].identificativo_copia;
}
void restituisci(struct Monitor * m, int id_film, int id_copia){
    
    pthread_mutex_lock(&(m->mutex));
    while (m->contatori_copie[id_film] == 2){
        pthread_cond_wait(&(m->attesa_su_film[id_film]), &(m->mutex));
    }
    
    int indice_film = id_film + m->contatori_copie[id_film] - 1;
    m->dvd[indice_film].stato = AFFITTATO;
    m->contatori_copie[id_film]--;

    pthread_cond_signal(&(m->attesa_su_film[id_film]));
    pthread_mutex_unlock(&(m->mutex));


}
void stampa(struct Monitor * m){

}