#include "monitor_signal_continue.h"
#include "procedure.h"

int main(){

    key_t key = IPC_PRIVATE;
    int id_shm = shmget(key, sizeof(PriorityProdCons), IPC_CREAT|0664);
    PriorityProdCons* b = (PriorityProdCons*) shmat(id_shm, NULL, 0);

    inizializza_prod_cons(b);

    //creazione processi figli
    pid_t pid;

    int i;
    for (i = 0; i < N_PRODUTTORI_ALTI; i++)
    {
        pid = fork();
        if(pid < 0){
            perror("Errore nella fork");
        }
        if(pid == 0){
            produci_alta_prio(b);
            exit(0);
        }
    }
    

    for (i = 0; i < N_PRODUTTORI_BASSI; i++)
    {
        pid = fork();
        if(pid < 0){
            perror("Errore nella fork");
        }
        if(pid == 0){
            produci_bassa_prio(b);
            exit(0);
        }
    }

    for (i = 0; i < N_CONSUMATORI; i++)
    {
        pid = fork();
        if(pid < 0){
            perror("Errore nella fork");
        }
        if(pid == 0){
            consuma(b);
            exit(0);
        }
    }
    

    for (i = 0; i < N_CONSUMATORI+N_PRODUTTORI_ALTI+N_PRODUTTORI_BASSI; i++)
    {
        wait(NULL);
    }
    
    rimuovi_prod_cons(b);
    shmctl(id_shm, IPC_RMID, NULL);


    return 0;
}