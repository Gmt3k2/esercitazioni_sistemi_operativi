#ifndef __COMMON
#define __COMMON

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include "monitor.h"

#define N_CONSUMATORI 4
#define N_PRODUTTORI 2


typedef struct
{
    int buffer;

    
    int buffer_pieno;
    int buffer_vuoto;

    Monitor m;
}BufferSingolo;


void produci(BufferSingolo*);
int consuma(BufferSingolo*);

#endif