#ifndef __PROCEDURE
#define __PROCEDURE

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include "monitor_hoare.h"

#define DIM 10
#define N_SCHEDULER 1
#define N_USER 5
#define DIM_DISCO 20

#define VARCOND_CONSUMATORE 0
#define VARCOND_PRODUTTORE 1

typedef struct {
    unsigned int posizione;
    pid_t processo;
}Richiesta;

typedef struct {
    Richiesta buf[DIM];
    int testa;
    int coda;
    int count;

    Monitor m;
}CodaRichieste;

void scheduler(int*, CodaRichieste*);
void user(CodaRichieste*);


#endif