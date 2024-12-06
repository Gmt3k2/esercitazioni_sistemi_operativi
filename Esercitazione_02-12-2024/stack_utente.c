#include "stack.h"

void *Inserisci(void * s)
{

	int i;
	Elem v;
    
    Stack * stack = (Stack *) s;

	for(i=0; i<4; i++) {
		v = rand() % 11;
		StackPush(stack, v);
		printf("Inserimento: %d\n", v);
		sleep(1);
	}

	pthread_exit(NULL);
}


void *Preleva(void * s)
{

	int i;
	Elem v1, v2;

    Stack * stack = (Stack *) s;
    
	for(i=0; i<10; i++) {
		v1=StackPop(stack);
		printf("Prelievo: %d\n", v1);

		v2=StackPop(stack);
		printf("Prelievo: %d\n", v2);

		printf("Somma: %d\n", v1+v2);

		sleep(3);
	}

	pthread_exit(NULL);
}


int main(int argc, char *argv[])
{
	pthread_t threads[NUM_THREAD];

	int i;

	srand(time(NULL));

	// Istanziamo lo stack
	Stack stack;
	StackInit(&stack,5);

	// Attributi thread
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for(i=0; i<5; i++) {
		// Creazione thread per l'inserimento
		pthread_create(&threads[i], &attr, Inserisci, (void *) &stack);
	}

	// Creazione thread per il prelievo
	pthread_create(&threads[5], &attr, Preleva, (void *) &stack);


	for(i=0; i<6; i++) {
		// Attesa terminazione thread
		pthread_join(threads[i], NULL);
		if(i < 5){
			printf("Thread produttore terminato\n");
		}
		else{
			printf("Thread consumatore terminato\n");
		}
	}


	// Rimozione stack
	StackRemove(&stack);
}

