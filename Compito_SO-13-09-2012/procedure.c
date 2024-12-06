#include "procedure.h"
#include "monitor_signal_continue.h"


void inizializza_prod_cons(PriorityProdCons * p){
    
    for (int i = 0; i < DIM; i++)
    {
        p->buffer[i] = 0;
    }
    
    init_monitor(&(p->m), 3);

    p->count = 0;
    p->testa = 0;
    p->coda = 0;
}
void produci_alta_prio(PriorityProdCons * p){
    for (int i = 0; i < 3; i++){
        srand(time(NULL));
        enter_monitor(&(p->m));
        while (p->count == DIM){
            printf("[%d] Produzione sospesa per mancanza di spazio\n", getpid());
            wait_condition(&(p->m), PRODUTTORI_ALTI);
            printf("Riprendo produzione\n");
        }

        int number = rand()%13;
        p->buffer[p->coda] = number;
        p->coda = (p->coda+1) % DIM;
        p->count++;
        sleep(2);
        printf("[%d] Prodotto alto: %d\n", getpid(), number);

        signal_condition(&(p->m), SPAZIO_DISP);
        leave_monitor(&(p->m));
    }
}

void produci_bassa_prio(PriorityProdCons * p){
    
    for (int i = 0; i < 3; i++){
        srand(time(NULL));
        enter_monitor(&(p->m));
        while (p->count == DIM || (queue_condition(&(p->m), PRODUTTORI_ALTI) > 0)){
            printf("Produzione BASSA sospesa\n");
            wait_condition(&(p->m), PRODUTTORI_BASSI);
            printf("Riprendo produzione bassa\n");
        }
        int number = (rand()%13) + 13;
        p->buffer[p->coda] = number;
        p->coda = (p->coda+1) % DIM;
        p->count++;
        sleep(1);
        printf("[%d] Prodotto basso: %d\n", getpid(), number);
        
        signal_condition(&(p->m), SPAZIO_DISP);
        leave_monitor(&(p->m));
    }
}
void consuma(PriorityProdCons * p){
    for (int i = 0; i < 12; i++)
    {
        enter_monitor(&(p->m));
        while (p->count == 0)
        {
            printf("Consumazione sospesa per mancanza di spazio\n");
            wait_condition(&(p->m), SPAZIO_DISP);
            printf("Consumazione - ripresa\n");
        }
        int number = p->buffer[p->testa];
        p->testa = (p->testa + 1)%DIM;
        p->count--;
        sleep(1);

        printf("[%d] Consumazione: %d\n", getpid(), number);
        
        if(queue_condition(&(p->m), PRODUTTORI_ALTI) > 0){
            signal_condition(&(p->m), PRODUTTORI_ALTI);
        }
        else{
            signal_condition(&(p->m), PRODUTTORI_BASSI);
        }
        leave_monitor(&(p->m));
    }
    
}
void rimuovi_prod_cons(PriorityProdCons * p){
    remove_monitor(&(p->m));
}