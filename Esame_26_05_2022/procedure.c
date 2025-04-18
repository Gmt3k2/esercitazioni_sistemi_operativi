#include "monitor_hoare.h"
#include "procedure.h"

void produttore(BufferCircolare * bc, BufferSingolo* bs){
    int num_val_produzione = rand()%10;
    
    enter_monitor(&bs->monitor);
    if (bs->buffer_pieno == 1){
        wait_condition(&bs->monitor, SPAZIO_DISPONIBILE);
    }
    
    bs->value = num_val_produzione;
    bs->buffer_pieno = 1;
    bs->buffer_vuoto = 0;
    signal_condition(&bs->monitor, MESSAGGIO_DISPONIBILE);
    leave_monitor(&bs->monitor);

    for (int i = 0; i < num_val_produzione; i++)
    {
        enter_monitor(&bc->monitor);
        int valore = rand()%10;
        if(bc->count == DIM_BUFFER){
            wait_condition(&bc->monitor, VARCOND_PRODUTTORE);
        }
        bc->buffer[bc->testa] = valore;
        printf("[PRODUTTORE] Ho prodotto il valore %d\n", valore);
        bc->testa = (bc->testa +1 )%DIM_BUFFER;
        bc->count++;
        
        signal_condition(&bc->monitor, VARCOND_CONSUMATORE);
        leave_monitor(&bc->monitor);
    }
    

}
void consumatore(BufferCircolare * bc, BufferSingolo* bs){

    enter_monitor(&bs->monitor);
    if(bs->buffer_vuoto == 1){
        wait_condition(&bs->monitor, MESSAGGIO_DISPONIBILE);
    }
    int num_val_produzione = bs->value;
    bs->buffer_pieno = 0;
    bs->buffer_vuoto = 1;
    signal_condition(&bs->monitor, SPAZIO_DISPONIBILE);
    leave_monitor(&bs->monitor);

    for (int i = 0; i < num_val_produzione; i++){
        enter_monitor(&bc->monitor);
        if(bc->count == 0){
            wait_condition(&bc->monitor, VARCOND_CONSUMATORE);
        }
        int value = bc->buffer[bc->coda];
        bc->coda = (bc->coda+1) % DIM_BUFFER;
        bc->count--;
        printf("[CONSUMAOTORE] ho consumato il valore %d\n", value);
        
        signal_condition(&bc->monitor, VARCOND_CONSUMATORE);
        leave_monitor(&bc->monitor);
    }
    

}


