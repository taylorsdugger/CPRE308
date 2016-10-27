#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "Bank.h"

#define bufferSize 64
#define delim " \t\r\n\a"
/*****************************************************************/
int appserver(char **args);
int check(char **args);
int trans(char **args);
int end(char **args);
void *worker();

/*****************************************************************/
int reqID = 0;
int numReq = 0;
pthread_mutex_t mut;
pthread_cond_t  check_cv;
pthread_cond_t  trans_cv;
pthread_cond_t  end_cv;

/*****************************************************************/
struct account{
    pthread_mutex_t lock;
    int ID;
};

struct request{
    int value;

};

struct account accounts[0];

/*****************************************************************/
char *inputs[] = {
    "appserver",
    "CHECK",
    "TRANS",
    "END"
};

int (*functions[])(char **)={
        &appserver,
        &check,
        &trans,
        &end
};

//
int numOfFunctions(){
    return sizeof(inputs) / sizeof(char *);
}

//
int appserver(char **args){

    int numThreads = args[0];
    int numAccounts = args[1];
    char *outFile = args[2];

    int thread_index[numThreads];

    pthread_cond_init(&check_cv, NULL);
    pthread_cond_init(&end_cv, NULL);
    pthread_cond_init(&trans_cv, NULL);

    pthread_t threads[numThreads];

    struct account accounts[numAccounts];
    initialize_accounts(numAccounts);
    for(int i = 0; i < numAccounts; i++){
        accounts[i].ID = i;
    }

    for(int j = 0; j < numThreads; j++){
        thread_index[j] = j;
        pthread_create(&threads[j], NULL, worker, (void *)&thread_index[j]);
    }

    for(int k = 0; k < numThreads; k++){
        pthread_join(threads[k], NULL);
    }

    return 1;
}

//Worker threads
void *worker(){

    pthread_mutux_lock(&mut);
    while(numReq == 0)
        pthread_cond_wait(&check_cv, &mut);






    //numReq--;
    pthread_mutex_unlock(&mut);
}

//Balance check
int check(char **args){
    int i = 0;

    if(args[1] < 0){
        //throw
    }else{
        i = args[1];
    }

    pthread_mutex_lock(&mut);
    //pthread_mutex_lock(&(accounts[i].lock));

    reqID++;

    pthread_cond_broadcast(&check_cv);

    //struct account x = accounts[i];

    //printf(reqID + " BAL " + x.bal);

    return 1;
}

int trans(char **args){

    return 1;
}

int end(char **args){

    return 1;
}

//ReadLine function
char **readLine(void){
    char *line = NULL;
    ssize_t size = 0;
    getline(&line, &size, stdin);
    return line;
}

char **split(char *in){
    int size = bufferSize;
    int pos = 0;
    char **list = malloc(bufferSize * sizeof(char*));
    char *token;

    if(!list){
        fprintf(stderr, "size error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(in, delim);

    while(token != NULL){
        list[pos] = token;
        pos++;

        if(pos >= size){//if its bigger then reallocate the size to be bigger
            size+= bufferSize;
            list = realloc(list, size * sizeof(char*));

            if(!list){
                fprintf(stderr, "size error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, delim);
    }
    list[pos] = NULL;
    return list;
}

int execute(char **args){
    if(args[0] == NULL){return 1;}

    for(int i = 0; i<numOfFunctions(); i++){
        if(strcmp(args[0], inputs[i]) == 0) return (*functions[i])(args);
    }

    return start(args);
}

void run(void){
    char *next;
    char **args;
    int s;

    do{
        printf("project2");
        next = readLine();
        args = split(next);
        s = execute(args);

    }while(s);

}

int main(int argc, char **argv){

    run();

    return EXIT_SUCCESS;
}