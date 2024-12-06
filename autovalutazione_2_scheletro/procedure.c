#include "procedure.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <time.h>


float* leggi_risultati(struct MonitorRisultati * ls) {

	//completare lettore con monitor sig and wait

	// var di appoggio in cui inserire i risultati letti dal monitor
	float* risultati = (float*) malloc(3*sizeof(float));

	enter_monitor(&(ls->m));
	printf("[%ld] Lettura - ingresso monitor\n", getpid());

	if(ls->numero_scrittori > 0){
		wait_condition(&(ls->m), VARCOND_LETTORI);
	}
	
	ls->numero_lettori++;
	signal_condition(&(ls->m), VARCOND_LETTORI);

	leave_monitor(&(ls->m));

	for (int i = 0; i <3; i++)
	{
		risultati[i] = ls->risultati[i];
	}
	

	enter_monitor(&(ls->m));
	ls->numero_lettori--;

	//se sono l'ultimo lettore posso riattivare uno scrittore
	if (ls->numero_lettori == 0)
	{
		signal_condition(&(ls->m), VARCOND_SCRITTORI);
	}
	leave_monitor(&(ls->m));


	printf("[%ld] Lettura - uscita monitor\n", getpid());
	
	
	return risultati;
}


void inserisci_risultato(struct MonitorRisultati * ls, float valore, int operazione) {

	// completare scrittore con monitor sig and wait
	enter_monitor(&(ls->m));

	if(ls->numero_lettori > 0 || ls->numero_scrittori > 0){
		printf("Scrittura sospesa");
		wait_condition(&(ls->m), VARCOND_SCRITTORI);
	}
	ls->numero_scrittori++;
	printf("[%ld] Scrittura - ingresso monitor\n", getpid());
	
	leave_monitor(&(ls->m)); //il monitor può essere rilasciato

	/*
	Effettuo la scrittura
	la scrittura deve avvenire in ls->risultati in posizione 0 per la somma, 1 per il prodotto, 2 per la divisione
	*/
	ls->risultati[operazione-1] = valore;
	printf("[%ld] Scrittura - valore [%f] operazione %d\n", getpid(),valore, operazione);

	enter_monitor(&(ls->m));
	ls->numero_scrittori--;

	if (queue_condition(&(ls->m), VARCOND_SCRITTORI))
	{
		signal_condition(&(ls->m), VARCOND_SCRITTORI);
	}
	else{
		signal_condition(&(ls->m), VARCOND_LETTORI);	
	}

	leave_monitor(&(ls->m));
	printf("[%ld] Scrittura - uscita monitor\n", getpid());
}

void calcolo(struct MonitorRisultati * ls, int operazione, int mailbox){
	// TODO completa come segue:
	// riceve 2 operandi dalla coda per l'operazione indicata dal parametro "operazione" (ad es. 1=somma, 2=prodotto, 3=divisione)
	// effettua l'operazione indicata
	// inserisce il risultato nel monitor con la funzione inserisci_risultato
	// effettua tutte le operazioni di cui sopra 2 volte
	Messaggio m1;
	Messaggio m2;
	float risulato;
	for (int i = 0; i < 2; i++)
	{
		msgrcv(mailbox, &m1, sizeof(Messaggio) - sizeof(long),operazione,0);  //receive bloccante
		msgrcv(mailbox, &m2, sizeof(Messaggio) - sizeof(long),operazione,0);  

		if(operazione == 1){
			/* somma */
			risulato = m1.value + m2.value;
		}
		else if (operazione == 2){
			/* prodotto */
			risulato = m1.value * m2.value;

		}
		else if (operazione == 3){
			/* divisione */
			risulato = m1.value / m2.value;
		}
		sleep(1);
		/* inserimento risultato nel monitor */
		inserisci_risultato(ls, risulato, operazione);	
	}
	return;
}
void generazione(int operazione, int mailbox) {
	srand(time(NULL));
	// TODO completa come segue:
	// crea un messaggio di ponendo il tipo uguale a "operazione" e il valore dell'operando casuale
	// invia il messaggio sulla coda
	// effettua tutte le operazioni di cui sopra 4 volte
	Messaggio m;
	for (int i = 0; i < 4; i++)
	{
		m.messageType = operazione;
		m.value = (float)rand()/(float)(RAND_MAX/19.0)+1;
		msgsnd(mailbox, &m, sizeof(Messaggio) - sizeof(long), 0);   //send bloccante
		printf("[%ld] Il messaggio[%ld] è stato mandato: [%f]\n",getpid(), m.messageType, m.value);
	}
	return;
}
void printer(struct MonitorRisultati * ls) {
	// già implementata, nulla da fare 
	float* risultati; 
	//Professore non mi trovo con la condizione del for perchè la traccia dice di chiamare leggi_risultati 6 volte in totale
	//quindi i non dovrebbe essere minore di 2?
	for (int i = 0 ; i < 4; i++) {
		sleep(2);
		risultati = leggi_risultati(ls);
		printf("[%ld] Risultati 1: %f, 2: %f, 3: %f\n",getpid(), risultati[0], risultati[1], risultati[2]);
		free(risultati);
	}
}
