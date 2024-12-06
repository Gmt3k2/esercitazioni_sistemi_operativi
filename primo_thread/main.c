#include <pthread.h>
#include <stdio.h>

void* print(){
    printf("Hello world\n");
    pthread_exit(NULL);
}

int main(void){

    pthread_t th1;
    pthread_create(th1, NULL, print, NULL);
    pthread_exit(NULL);
    return 0;
}