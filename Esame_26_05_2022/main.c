#include "procedure.h"
#include "monitor_hoare.h"


int main(void){

    key_t chiave = IPC_PRIVATE;

    int shm_buffer_singolo = shmget(chiave, sizeof(BufferSingolo), IPC_CREAT|0664);
    if(shm_buffer_singolo < 0)
        perror("Error:");
    
    int shm_buffer_circolare = shmget(chiave, sizeof(BufferCircolare), IPC_CREAT|0664);
    if(shm_buffer_circolare < 0)
        perror("Error:");

    BufferSingolo* bs = (BufferSingolo*) shmat(shm_buffer_singolo, NULL, 0);
    if(bs == (void*) 0)
        perror("Error:");
    
    BufferCircolare* bc = (BufferCircolare*) shmat(shm_buffer_circolare, NULL, 0);
    if(bc == (void*) 0)
        perror("Error:");
    
    //inizializzazione buffer singolo
    bs->buffer_pieno = 0;
    bs->buffer_vuoto = 1;
    bs->value = 0;
    init_monitor(&bs->monitor, 2);
    
    //inizializzazione buffer circolare
    bc->coda = 0;
    bc->testa = 0;
    bc->count = 0;
    init_monitor(&bc->monitor, 2);


    pid_t pid;
    pid = fork();
    if(pid < 0){
        printf("Errore nella creazione del processo\n");
        exit(-1);
    }
    else if(pid == 0){
        srand(time(NULL) * getpid());
        printf("[MAIN] Processo produttore creato correttamente\n");
        produttore(bc, bs);
        exit(0);
    }


    pid = fork();
    if(pid < 0){
        printf("[MAIN] Processo consumatore creato correttamente\n");
        printf("Errore nella creazione del processo\n");
        exit(-1);
    }
    else if(pid == 0){
        consumatore(bc, bs);
        exit(0);
    }

    //join
    for (int i = 0; i < 2; i++){
        wait(NULL);
        printf("[MAIN] Processo terminato correttamente!\n");
    }
    
    remove_monitor(&bc->monitor);
    remove_monitor(&bs->monitor);
    return 0;
}