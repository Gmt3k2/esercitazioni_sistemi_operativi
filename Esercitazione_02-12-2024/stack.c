#include "stack.h"

void StackInit(Stack * s, int dim) {
	s->dati = (Elem*) malloc(dim * sizeof(Elem));	

	for (int i = 0; i < dim; i++){
		s->dati[i] = 0;
	}
	
	s->testa = 0;
	s->dim = dim;
	pthread_mutex_init(&(s->mutex), NULL);
	pthread_cond_init(&(s->cv_consumatore), NULL);
	pthread_cond_init(&(s->cv_produttore), NULL);
}


void StackRemove(Stack * s) {
	pthread_mutex_destroy(&(s->mutex));
	pthread_cond_destroy(&(s->cv_consumatore));
	pthread_cond_destroy(&(s->cv_produttore));
	free(s->dati);
}

void StackPush(Stack * s, Elem e) {
	pthread_mutex_lock(&(s->mutex));
	while(s->testa == s->dim){
		printf("Push sospeso!\n");
		pthread_cond_wait(&(s->cv_produttore), &(s->mutex));
	}

	s->dati[s->testa] = e;
	s->testa++;

	pthread_mutex_unlock(&(s->mutex));
	pthread_cond_signal(&(s->cv_consumatore));
}


Elem StackPop(Stack * s) {

	int elemento;
	pthread_mutex_lock(&(s->mutex));
	while (s->testa == 0){
		printf("Pop sospesa!\n");
		pthread_cond_wait(&(s->cv_consumatore), &(s->mutex));
	}
	
	s->testa--;
	elemento = s->dati[s->testa];
	
	pthread_mutex_unlock(&(s->mutex));
	pthread_cond_signal(&(s->cv_produttore));
	
	return elemento;
}

int StackSize(Stack * s) {
	int size;
	size = s->testa;
	return size;
}
