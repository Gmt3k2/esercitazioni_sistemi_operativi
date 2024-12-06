#include "monitor.h"
#include "common.h"


int main(void){

    key_t chiave = IPC_PRIVATE;
    int id_buffer = shmget(chiave,sizeof(BufferSingolo), IPC_CREAT|0664);
    if(id_buffer < 0){
        perror("Errore nella creazione della shm");
    }
    BufferSingolo* b = (BufferSingolo*) shmat(id_buffer, NULL, 0);

    init_monitor(&(b->m), 1);
    b->buffer_vuoto = 1;
    b->buffer_pieno = 0;

    //zona creazione processi consumatori e produttori
    __pid_t pid;
    for (int i = 0; i < N_CONSUMATORI; i++){
        pid = fork();
        if (pid < 0)
        {
            perror("Errore nella creazione processo figlio");
            exit(1);
        }
        else if(pid == 0){
            printf("Inizio consumatore\n");

            int valore = consuma(b);

            printf("Consumazione: %d\n", valore);
            exit(0);
        }

    }

    for (int i = 0; i < N_PRODUTTORI; i++){
        pid = fork();
        if (pid < 0){
            perror("Errore nella creazione processo figlio");
            exit(1);
        }
        else if(pid == 0){
            sleep(5);
            srand(time(NULL));
            produci(b);
            exit(0);
        }

    }
    
    for (int i = 0; i < N_CONSUMATORI+N_PRODUTTORI; i++)
    {
        wait(NULL);
    }
    

    shmctl(id_buffer, IPC_RMID, 0);
    remove_monitor(&(b->m));
    
    return 0;
}