#include "procedure.h"

int main(){

    srand(time(NULL) * getpid());

    key_t key_query = ftok("./", 'q');
    key_t key_response = ftok("./", 'r');

    int id_request = msgget(key_query, 0);
    int id_response = msgget(key_response, 0);
    
    Messaggio msg;

    for (int i = 0; i < 5; i++)
    {
        msg.type = getpid();
        msg.num1 = rand()%11;
        msg.num2 = rand()%11;
        if(msgsnd(id_request, &msg, DIM_MESSAGGIO, 0) == 0)
            printf("[%d] Messaggio inviato correttamente (%d*%d)\n", getpid(), msg.num1, msg.num2);
        else
            printf("Errore nella send\n");
        if (msgrcv(id_response, &msg, DIM_MESSAGGIO, getpid(), 0) > 0)
            printf("Il prodotto dei numeri mandati è %d\n", msg.num1);
        else
            printf("Errore nella receive\n");
    }
    

}