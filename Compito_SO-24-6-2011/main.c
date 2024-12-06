#include "procedure.h"
#include "monitor_hoare.h"

void stampa_disco(int* disco, int dim){
    for (int i = 0; i < dim; i++)
    {
        printf("Disco[%d] - %d\n", i, disco[i]);
    }
}

int main(){
    key_t key = IPC_PRIVATE;
    int shm_id = shmget(key, sizeof(CodaRichieste), IPC_CREAT|0664);
    if(shm_id < 0){
        perror("Errore nella shm");
    }
    CodaRichieste* b = (CodaRichieste*) shmat(shm_id, 0, 0);
    if(b == (void*) 0){
        perror("Errore nella shm");
    }

    printf("Parte delle shm completata correttamente\n");

    init_monitor(&(b->m), 2);
    b->coda = 0;
    b->testa = 0;
    b->count = 0;
    printf("Monitor correttamente inizializzato\n");

    pid_t pid;
    for (int i = 0; i < N_USER; i++){
        pid = fork();
        if(pid < 0){
            perror("Errore nella creazione processo");
        }
        else if (pid == 0){
            srand(time(NULL) * getpid());
            user(b);
            exit(0);
        }
    }

    int id_disco = shmget(IPC_PRIVATE, DIM_DISCO*sizeof(int), IPC_CREAT|0664);
    int* disco = (int*) shmat(id_disco, 0, 0);
    for (int i = 0; i < N_SCHEDULER; i++){
        pid = fork();
        if (pid < 0){
            perror("Errore nella creazione processo");
        }
        else if(pid == 0){
            scheduler(disco, b);
            exit(0);
        }
    }

    for (int i = 0; i < N_SCHEDULER+N_USER; i++)
    {
        wait(NULL);
    }

    stampa_disco(disco, DIM_DISCO);
    shmctl(shm_id, IPC_RMID, 0);
    remove_monitor(&(b->m));
    return 0;
}