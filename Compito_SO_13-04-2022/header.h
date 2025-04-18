#define NUM_UPDATES 10
#define NUM_READS 20
#define NUM_READERS 3

#define MSG_UPDATE_REQUEST 1
#define MSG_READ_REQUEST 2

typedef struct {
	//TODO: completare con la struttura del messaggio di richiesta
	long type;
	int pid;
	int num;
} req; 

typedef struct {
	//TODO: completare con la struttura del messaggio di risposta
	long pid;
	int num;
} res; 


