#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

typedef struct{
    char id;
    int* a;
}dati;


int a=0;

void * fun(void * par){
    dati* d = (dati*)par;
    printf("Ciao sono il thread %c\n", (char)par);
    for (int i = 0; i < 10; i++)
    {
        printf("%c\n", d->id);
        usleep(200000);
    }
    pthread_exit(NULL);
}

int main(){

    int * a = malloc(sizeof(int));
    *a = 0;
    pthread_t th1, th2;
    dati id;
    id.id = 'a';
    id.a = a;
    pthread_create(&th1, NULL, fun,(void *)&id);
    dati d2;
    d2.id = 'a';
    d2.a = a;
    pthread_create(&th2, NULL, fun, (void *)'b');

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    printf("La variabile a: %d\n", a);
    pthread_exit(NULL);    
    return 0;
}