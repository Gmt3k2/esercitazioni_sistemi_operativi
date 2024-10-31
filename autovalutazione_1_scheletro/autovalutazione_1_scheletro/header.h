			/*----HEADER FILE----*/
#ifndef __HEADER
#define __HEADER

#define SIZE 4

#define LIBERO 0
#define IN_USO 1
#define OCCUPATO 2

#define NPROD 3
#define NCONS 3

typedef struct{
	int chiave;
	int valore;
} ElemBuffer;

typedef struct{
	ElemBuffer elementi[SIZE];
	int stati[SIZE];
} BufferCircolare;

#define SPAZIO_DISPONIBILE 0
#define DISPONIBILE_1 1
#define DISPONIBILE_2 2
#define DISPONIBILE_3 3
#define MUTEX_C 4
#define MUTEX_P 5


#define VUOTO 0
#define IN_USO 1
#define PIENO 2

void Wait_Sem(int, int);
void Signal_Sem (int, int);

void produci(BufferCircolare* buf, int chiave, int semid);
void consuma(BufferCircolare* buf, int chiave, int semid);

#endif
