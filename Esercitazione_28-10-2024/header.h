			/*----HEADER FILE----*/
#ifndef __HEADER
#define __HEADER

#define N 5

#define MUTEXC 0
#define MUTEXP 1
#define SPAZIO_DISP 2
#define BUFFER_PIENO 3


typedef struct{
	int elementi[N];
	int testa;
	int coda;
	int count;
} BufferCircolare;

//INSERIRE VARIABILI CHE SERVONO


void Wait_Sem(int, int);
void Signal_Sem (int, int);

void produci_elemento(int semid,BufferCircolare* buf);
void consuma_elementi(int semid, BufferCircolare* buf);

#endif
