#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>

#include "header.h"

#define NUM_PRODUTTORI 2
#define NUM_CONSUMATORI 5

#define PRODUZIONI 10
#define CONSUMAZIONI 4

void produttore(VettoreProdCons * vettore);
void consumatore(VettoreProdCons * vettore, BufferMutuaEsclusione * buffer);

int main() {

    key_t key = IPC_PRIVATE;
    int shm_vettore = shmget(key, sizeof(VettoreProdCons), IPC_CREAT|0664);
    if (shm_vettore < 0){
        perror("Error:");
        exit(-1);
    }
    
    int shm_buffer = shmget(key, sizeof(BufferMutuaEsclusione), IPC_CREAT|0664);
    if(shm_buffer < 0){
        perror("Error:");
        exit(-1);
    }

    VettoreProdCons * vettore = (VettoreProdCons*) shmat(shm_vettore, NULL, 0);
    if(vettore == (void *) 0){
        perror("Error:");
        exit(-1);
    }

    BufferMutuaEsclusione * buffer = (BufferMutuaEsclusione*) shmat(shm_buffer, NULL, 0);
    if(buffer == (void *) 0){
        perror("Error:");
        exit(-1);
    }


    inizializza_vettore(vettore);
    inizializza_buffer(buffer);

    pid_t pid;
    for(int i=0; i<NUM_PRODUTTORI; i++) {
        /* TBD: Creare i processi produttori, e fargli chiamare la funzione "produttore()"*/
        pid = fork();
        if(pid <0){
            perror("Error:");
        }
        else if(pid == 0){
            produttore(vettore);
            exit(0);
        }
    }


    for(int i=0; i<NUM_CONSUMATORI; i++) {
        /* TBD: Creare i processi consumatori, e fargli chiamare la funzione "consumatore()"*/
        pid = fork();
        if(pid <0){
            perror("Error:");
        }
        else if(pid == 0){
            consumatore(vettore, buffer);
            exit(0);
        }
    }


    /* TBD: Attendere la terminazione dei processi figli */
    for (int i = 0; i < NUM_CONSUMATORI+NUM_PRODUTTORI; i++)
    {
        wait(NULL);
        printf("[MAIN] Processo terminato correttamente!\n");
    }
    

    /* TBD: De-allocare le strutture dati */
    rimuovi_buffer(buffer);
    // rimuovi_vettore(vettore);
    
    remove_monitor(&vettore->monitor);
    shmctl(shm_buffer, IPC_RMID, 0);
    shmctl(shm_vettore, IPC_RMID, 0);
    return 0;

}

void produttore(VettoreProdCons * vettore) {

    srand(getpid());
    
    for(int i=0; i<PRODUZIONI; i++) {

        int valore = rand() % 10;

        printf("[MAIN PRODUTTORE] Produzione: %d\n", valore);

        /* TBD: Chiamare il metodo "produci()" del monitor */
        produci(vettore, valore);
    }

}

void consumatore(VettoreProdCons * vettore, BufferMutuaEsclusione * buffer) {

    srand(getpid());

    for(int i=0; i<CONSUMAZIONI; i++) {

        /* TBD: Chiamare il metodo "consuma()" del monitor */
        int valore = consuma(vettore);
        printf("[MAIN CONSUMATORE] Consumazione: %d\n", valore);

        /* TBD: Chiamare il metodo "aggiorna()", passandogli il valore che è stato appena consumato*/
        aggiorna(buffer, valore);
    }
}
