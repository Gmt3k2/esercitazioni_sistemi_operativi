#include "procedure.h"

int main(void){

    key_t chiave = IPC_PRIVATE;
    int shm_id = shmget(chiave, sizeof(Buffer), IPC_CREAT|0664);
    if(shm_id < 0){
        perror("Errore");
        exit(1);
    }
    
    Buffer* b = (Buffer*) shmat(shm_id, 0,0);
    if(b == (void*) 0){
        perror("Errore");
        exit(1);
    }

    b->count_lettori = 0;
    b->count_scrittori = 0;

    init_monitor(&(b->m), 2);
    
    pid_t pid;
    for (int i = 0; i < N_SERVIZIO_METEO; i++){
        pid = fork();
        if(pid < 0 ){
            perror("Error");
            exit(1);
        }
        else if(pid == 0){
            srand(time(NULL) * getpid());
            servizio_meteo(b);
            exit(0);
        }
    }

    for (int i = 0; i < N_UTENTI; i++){
        pid = fork();
        if(pid < 0 ){
            perror("Error");
            exit(1);
        }
        else if(pid == 0){
            utente(b);
            exit(0);
        }
    }
    

    for (int i = 0; i < N_UTENTI + N_SERVIZIO_METEO; i++){
        wait(NULL);
    }
    
    remove_monitor(&(b->m));
    shmctl(shm_id, IPC_RMID, 0);
    return 0;
}