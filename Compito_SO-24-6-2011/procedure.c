#include "monitor_hoare.h"
#include "procedure.h"

void scheduler(int* disco, CodaRichieste* b){
    int pos = 0;
    int pos_prec = 0;
    for (int i = 0; i < 25; i++){
        enter_monitor(&(b->m));
        
        printf("[%d] Lo scheduler è entrato nel monitor\n", getpid());
        if(b->count == 0){
            printf("[%d] Consumatore sospeso\n", getpid());
            wait_condition(&(b->m), VARCOND_CONSUMATORE);
            printf("[%d] Consumazione dele rischieste ripresa\n", getpid());
        }
        
        pos_prec = pos;
        pos = b->buf[b->testa].posizione;
        sleep(abs(pos-pos_prec));
        int value = b->buf[b->testa].processo;
        b->count--;
        b->testa = (b->testa +1 )%DIM;


        disco[pos] = value;

        printf("[%d] consumato la richiesta: (%d): %d\n", getpid(), pos, value);
        
        signal_condition(&(b->m), VARCOND_PRODUTTORE);
        leave_monitor(&(b->m));
        printf("[%d] Scheduler ha lasciato il monitor\n", getpid());
    }
    
}


void user(CodaRichieste* b){
    for (int i = 0; i < 5; i++)
    {
        enter_monitor(&(b->m));
        printf("[%d] User ha acquisito il monitor\n", getpid());
        if(b->count == DIM){
            printf("[%d] User sospeso\n", getpid());
            wait_condition(&(b->m), VARCOND_PRODUTTORE);
            printf("[%d] User ripreso esecuzione\n", getpid());
        }

        Richiesta richiesta;
        richiesta.processo = getpid();
        richiesta.posizione = rand()%20;
        b->buf[b->coda] = richiesta;
        b->count++;
        b->coda = (b->coda + 1)%DIM;
        printf("[%d] User ha richiesto spazio %d\n", getpid(), richiesta.posizione);

        signal_condition(&(b->m), VARCOND_CONSUMATORE);
        leave_monitor(&(b->m));
        printf("[%d] User ha lasciato il monitor\n", getpid());
    }
    
}