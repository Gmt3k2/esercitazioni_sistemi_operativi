#include "procedure.h"


void servizio_meteo(Buffer* b){
    for (int i = 0; i < 20; i++){
        enter_monitor(&(b->m));
        if(b->count_lettori > 0){
            printf("Scrittura sospesa\n");
            wait_cond(&(b->m), CV_SCRITTORI);
        }
        b->count_scrittori++;

        leave_monitor(&(b->m));

        b->meteo.temperatura = (rand()%101) - 50;
        b->meteo.umidita = rand()%101;
        b->meteo.pioggia = rand()%2;
        printf("Meteo prodotto: %d°C, um %d%%, pio %d\n", b->meteo.temperatura, b->meteo.umidita, b->meteo.pioggia);
        sleep(2);
        
        enter_monitor(&(b->m));
        b->count_scrittori--;

        signal_cond(&(b->m), CV_LETTORI);
        leave_monitor(&(b->m));
    }
    
}
void utente(Buffer* b){
    meteo meteo;
    enter_monitor(&(b->m));
    if(b->count_scrittori > 0){
        printf("Lettura sospesa\n");
        wait_cond(&(b->m), CV_LETTORI);
    }
    b->count_lettori++;
    leave_monitor(&(b->m));

    meteo = b->meteo;
    printf("Temperatura rilevata: %d°C, um: %d%%, pio: %d\n", meteo.temperatura,meteo.umidita,meteo.pioggia);
    sleep(1);
    
    enter_monitor(&(b->m));
    b->count_lettori--;
    if(b->count_lettori == 0){
        signal_cond(&(b->m), CV_SCRITTORI);
    }
    leave_monitor(&(b->m));
}