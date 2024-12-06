#ifndef HEADER_H
#define HEADER_H

#include "monitor_hoare.h"

#define N_GENERATORI 3
#define N_CALCOLATORI 3
#define N_PRINTERS 3

#define N_OPS 3

#define TYPE_SOMMA 1
#define TYPE_PROD 2
#define TYPE_DIV 3

#define VARCOND_SCRITTORI 0 
#define VARCOND_LETTORI 1

struct MonitorRisultati {
	float risultati[N_OPS];

	int numero_lettori;
	int numero_scrittori;

	Monitor m;
};


void inserisci_risultato(struct MonitorRisultati * ls, float valore, int operazione);
float* leggi_risultati(struct MonitorRisultati * ls);

typedef struct {
	long messageType;
	float value;
} Messaggio;

void calcolo(struct MonitorRisultati * ls, int operazione, int mailbox);
void generazione(int operazione, int mailbox);
void printer(struct MonitorRisultati * ls);

#endif
