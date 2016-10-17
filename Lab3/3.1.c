//
// Created by taylorsdugger on 9/30/16.
//

#include <pthread.h>
#include <stdio.h>

void* thread1();
void* thread2();

int main (int argc, char *argv[]){

    pthread_t i1;
    pthread_t i2;

    pthread_create(&i1, NULL, (void*)&thread1, NULL);
    pthread_create(&i2, NULL, (void*)&thread2, NULL);

    pthread_join(i1, NULL);
    pthread_join(i2, NULL);

    printf("Hello, I am main process\n");

    return 0;
}

void* thread1(){
    sleep(5);
    printf("Hello, I am thread 1\n");
}

void* thread2(){
    sleep(5);
    printf("Hello, I am thread 2\n");
}