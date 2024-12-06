#ifndef __PROCEDURE
#define __PROCEDURE

#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include "monitor_hoare.h"

#define N_UTENTI 10
#define N_SERVIZIO_METEO 1

#define CV_LETTORI 0
#define CV_SCRITTORI 1

typedef struct {
    int temperatura;
    unsigned int umidita;
    unsigned short pioggia;
} meteo;

typedef struct{
    meteo meteo;

    int count_lettori;
    int count_scrittori;

    Monitor m;
}Buffer;

void servizio_meteo(Buffer* b);
void utente(Buffer* b);

#endif