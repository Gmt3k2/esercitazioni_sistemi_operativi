#include <sys/random.h>
#include "monitor.h"
#include "common.h"

void produci(BufferSingolo* b){
    for (int i = 0; i < 3; i++)
    {
        enter_monitor(&(b->m));
        if(b->buffer_pieno == 1){
            wait_condition(&(b->m), PRODUTTORE);
        }

        b->buffer = rand()%10+1;
        b->buffer_pieno = 1;
        b->buffer_vuoto = 0;
        printf("Il produttore[%d] ha prodotto %d\n", getpid(), b->buffer);

        signal_condition(&(b->m), CONSUMATORE);
        leave_monitor(&(b->m));
    }
}

int consuma(BufferSingolo* b){
    int valore;
    enter_monitor(&(b->m));
    if(b->buffer_vuoto == 1){
        wait_condition(&(b->m), CONSUMATORE);
    }

    valore = b->buffer;
    b->buffer_pieno = 0;
    b->buffer_vuoto = 1;
    printf("Il consumatore[%d] ha consumato %d\n", getpid(), valore);
    
    signal_condition(&(b->m), CONSUMATORE);
    leave_monitor(&(b->m));
    return valore;

}