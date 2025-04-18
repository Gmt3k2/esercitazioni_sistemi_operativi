#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "monitor_hoare.h"
#include "header.h"

// le code
int mq_offerte;
int mq_risposte;

// suggerimento: definire delle costanti per enumerare le condition
#define VARCOND_LETTORE 0
#define VARCOND_SCRITTORE 1

#define VARCOND_AGGIUDICATORE 0

//l'elemento di offerte in posizione '0' non è usato in quanto i clienti
//sono numerati da 1 a MAX_CLIENTI+1
typedef struct tipo_asta {
    int offerte[MAX_CLIENTI+1]; //elemento in posizione '0' non usato
    // TODO: aggiungere monitor e variabili per la sincronizzazione
    int num_lettori;
    int num_scrittori;

    Monitor monitor;
} Asta;

typedef struct tipo_max_offerta {
    int valore_offerto;
    int id_client_offerente;
    int offerta_aggiudicata;
    // TODO: aggiungere monitor
    Monitor monitor;
} Max_Offerta;

// TODO: lettori-scrittori con starvation di entrambi usando monitor Hoare
void InizioLettura(Asta * asta){
	// TODO
    enter_monitor(&asta->monitor);
    if(asta->num_scrittori > 0){
        wait_condition(&asta->monitor, VARCOND_LETTORE);
    }
    asta->num_lettori++;
    signal_condition(&asta->monitor, VARCOND_LETTORE);
    leave_monitor(&asta->monitor);
}
void FineLettura(Asta * asta){
    // TODO
    enter_monitor(&asta->monitor);
    asta->num_lettori--;
    if(asta->num_lettori == 0){
        signal_condition(&asta->monitor, VARCOND_SCRITTORE);
    }
    leave_monitor(&asta->monitor);
}
void InizioScrittura(Asta * asta){
    //TODO
    enter_monitor(&asta->monitor);
    if(asta->num_lettori > 0 || asta->num_scrittori > 0){
        wait_condition(&asta->monitor, VARCOND_SCRITTORE);
    }
    asta->num_scrittori++;
    leave_monitor(&asta->monitor);
}
void FineScrittura (Asta * asta){
    //TODO
    enter_monitor(&asta->monitor);
    asta->num_scrittori--;
    if (queue_condition(&asta->monitor, VARCOND_SCRITTORE)){
        signal_condition(&asta->monitor, VARCOND_SCRITTORE);//se ho degli scrittori in attesa segnali quelli in modo da bilanciare la starvation
    }
    else{
        //se non ho altri scrittori che vogliono scrivere segnalo il primo lettore che poi a sua volta segnalerà gli altri
        signal_condition(&asta->monitor, VARCOND_LETTORE);
    }
    leave_monitor(&asta->monitor);
    
}

void banditore(Asta* asta, Max_Offerta* max_offerta, int client_id){
    
    int aggiudicata = 0;
    while(!aggiudicata) {
        offerta_msg off;
        // TODO ricevere il messaggio di offerta dal proprio client_id
        msgrcv(mq_offerte, &off, sizeof(offerta_msg)-sizeof(long), client_id, 0);
        printf("BANDITORE: Ricevuto un messaggio di offerta dal client %ld\n",off.id_cliente);
        
        // TODO recuperare in mutua esclusione lo stato dell'offerta
        enter_monitor(&max_offerta->monitor);
        aggiudicata = max_offerta->offerta_aggiudicata;
        leave_monitor(&max_offerta->monitor);

        if (!aggiudicata) {
            // TODO rispondere al cliente con lo stato dell'asta, accedendo
            // a 'max_offerta' in mutua esclusione
            risposta_offerta_msg risp;
            enter_monitor(&max_offerta->monitor);
            risp.asta_aggiudicata = max_offerta->offerta_aggiudicata;
            risp.id_cliente = client_id;
            risp.id_vincitore = max_offerta->id_client_offerente;
            risp.max_valore_offerto = max_offerta->valore_offerto;
            leave_monitor(&max_offerta->monitor);
            msgsnd(mq_risposte, &risp, sizeof(risposta_offerta_msg)-sizeof(long), 0);

            // TODO aggiorna l'array offerte come SCRITTORE usando inizio-fine scrittura
            // inserendo l'offerta ricevuta nella posizone corrispondente al client_id
            InizioScrittura(asta);
            asta->offerte[client_id] = off.valore_offerto;
            FineScrittura(asta);

            printf("BANDITORE: Valore offerto da %ld pari a: %d\n",off.id_cliente,off.valore_offerto);
        }
    }
}

void giudice(Asta* asta, Max_Offerta* max_offerta){

    

    for (int i = 0; i < TURNI; i++) {
        sleep(1);
        // TODO Accedendo in LETTURA su 'asta' con inizio-fine lettura, determina l'offerta massima 'max_off'
        // e l'indice 'max_off_client_id' del client dell'offerta massima corrente
        int max_off = 0;
        int max_off_client_id = -1;

        InizioLettura(asta);
        for (int j = 1; j <= MAX_CLIENTI; j++) {
            if(asta->offerte[i] > max_off){
                max_off = asta->offerte[i];
                max_off_client_id = j;
            }
        }
        FineLettura(asta);

        //TODO: Aggiornare i dati di 'max_offerta' in mutua esclusione
        enter_monitor(&max_offerta->monitor);
        max_offerta->valore_offerto = max_off;
        max_offerta->id_client_offerente = max_off_client_id;
        leave_monitor(&max_offerta->monitor);
        printf("GIUDICE turno %d: massimo valore correntemente offerto %d dal client %d\n",i,max_off,max_off_client_id);
    }

    // TODO: finiti i turni determina l'asta aggiudicata, modificando 'max_offerta'
    // in mutua esclusione e segnalando l'aggiudicatore
    //...
    enter_monitor(&max_offerta->monitor);
    max_offerta->offerta_aggiudicata = 1;
    signal_condition(&max_offerta->monitor, VARCOND_AGGIUDICATORE);
    leave_monitor(&max_offerta->monitor);

    printf("GIUDICE: asta chiusa\n");
}

void aggiudicatore(Max_Offerta* max_offerta) {
    risposta_offerta_msg risp;
    
    // TODO attesa che l'offerta sia aggiudicata
    enter_monitor(&max_offerta->monitor);
    while (!max_offerta->offerta_aggiudicata){
        wait_condition(&max_offerta->monitor, VARCOND_AGGIUDICATORE);
    }
    
    // TODO preparaziomne risposta
    risp.asta_aggiudicata = max_offerta->offerta_aggiudicata;
    risp.id_vincitore = max_offerta->id_client_offerente;
    risp.max_valore_offerto = max_offerta->valore_offerto;
    for (int i = 1; i < MAX_CLIENTI+1; i++) {
        risp.id_cliente = i;
        // TODO e invio ad ogni client
        msgsnd(mq_risposte, &risp, sizeof(risposta_offerta_msg)-sizeof(long), 0);
    }
    printf("AGGIUDICATORE: asta aggiudicata a %d!\n",risp.id_vincitore);  
    leave_monitor(&max_offerta->monitor);
    
}

int main() {
    //TODO: completare l'inizializzazione code
    int key_queue_off = ftok("./", 'o');
    int key_queue_risp = ftok("./", 'r');
    mq_offerte = msgget(key_queue_off, 0);
    mq_risposte = msgget(key_queue_risp, 0);

    //TODO: completare inizializzazione shm 
    int key_shm_asta = IPC_PRIVATE;
    int key_shm_max = IPC_PRIVATE;
    int shm_asta = shmget(key_shm_asta, sizeof(Asta), IPC_CREAT|0664);
    int shm_max = shmget(key_shm_max, sizeof(Max_Offerta), IPC_CREAT|0664);

    // TODO inizializzazione parametri
    Asta* asta = (Asta*) shmat(shm_asta, NULL, 0);
    if(asta == (void*) 0)
        perror("Error:");
    init_monitor(&asta->monitor, 2);
    asta->num_lettori = 0;
    asta->num_scrittori = 0;
    
    Max_Offerta* max_off = (Max_Offerta*) shmat(shm_max, NULL, 0);
    if(max_off == (void *) 0)
        perror("Error:");
    init_monitor(&max_off->monitor, 1);
    max_off->id_client_offerente = 0;
    max_off->offerta_aggiudicata = 0;
    max_off->valore_offerto = 0;

    //TODO creazione processo giudice
    pid_t pid = fork();
    if (pid < 0){
        printf("Errore nella creazione processo giudice\n");
    }
    else if (pid == 0){
        giudice(asta, max_off);
        exit(0);
    }

    //TODO creazione processo aggiudicatore
    pid = fork();
    if (pid < 0){
        printf("Errore nella creazione processo aggiudicatore\n");
    }
    else if (pid == 0){
        aggiudicatore(max_off);
        exit(0);
    }

    //TODO creazione di un banditore per ogni cliente
    for (int client_id = 1; client_id < MAX_CLIENTI+1;client_id++) {
        pid = fork();
        if (pid < 0){
            printf("Errore nella creazione processo banditore\n");
        }
        else if (pid == 0){
            banditore(asta, max_off, client_id);
            exit(0);
        }
    }

    //TODO attesa processi figli
    for (int client_id = 1; client_id < MAX_CLIENTI+3;client_id++) {
        wait(NULL);
        printf("Processo terminato correttamente\n");
    }
    // e rimozione shm ecc.
    remove_monitor(&asta->monitor);
    remove_monitor(&max_off->monitor);
    shmctl(shm_asta, IPC_RMID, 0);
    shmctl(shm_max, IPC_RMID, 0);
    return 0;
}