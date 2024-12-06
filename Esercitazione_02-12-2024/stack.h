#ifndef __HEADER__
#define __HEADER__

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREAD 6

typedef int Elem;

typedef struct Stack {
	Elem * dati;	// puntatore ad un vettore dinamico di elementi
	int dim;		// dimensione dello Stack
	
	int testa;
	pthread_cond_t cv_produttore;
	pthread_cond_t cv_consumatore;
	pthread_mutex_t mutex;
} Stack;

void StackInit(Stack * s, int dim);	// alloca dinamicamente "dim" elementi
void StackRemove(Stack * s);		// dealloca gli elementi
void StackPush(Stack * s, Elem e);	// inserimento nello Stack
Elem StackPop(Stack * s);		// estrazione dallo Stack
int StackSize(Stack * s);		// ritorna il numero attuale di elementi


#endif
