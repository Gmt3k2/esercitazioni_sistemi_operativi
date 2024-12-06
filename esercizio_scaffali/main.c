#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include "procedure.h"

int main(){

    key_t chiave = IPC_PRIVATE;
    int id_mag = shmget(chiave, 100*sizeof(scaffale), IPC_CREAT|0664);
    if(id_mag<0){
        perror("Errore nella crezione della shm");
    }

    int id_liv_scorte = shmget(chiave, sizeof(int), IPC_CREAT|0664);
    if(id_liv_scorte<0){
        perror("Errore nella creazione shm");
    }

    scaffale* mag = shmat(id_mag, NULL, 0);
    if(mag == (void*)0){
        perror("Errore SHM");
    }

    int* p = (int*) shmat(id_liv_scorte, NULL, 0);
    if(p == (void*)0){
        perror("Errore SHM");
    }

    for (int i = 0; i < NUM_FORNITORI; i++)
    {
        pid_t pid = fork();
        if(pid<0){
            perror("ERROR");
        }
        else if (pid == 0)
        {
            /* eseguo la funzione fornitore che sta in procedure.c */
            exit(0);
        }
        
    }

    for (int i = 0; i < NUM_CLIENTI; i++)
    {
        pid_t pid = fork();
        if (pid <0)
        {
            perror("ERROR");
        }
        else if (pid == 0)
        {
            /* eseguo la funzione cliente che deve stare in procedure.c */
            exit(0);
        }
    }
    
    for (int i = 0; i < NUM_CLIENTI+NUM_FORNITORI; i++)
    {
        wait(NULL);
    }
    


    shmctl(id_liv_scorte, IPC_RMID, 0);
    shmctl(id_mag, IPC_RMID, 0);

    return 0;
}