#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/*****************************************************************/
int appserver(char **args);

/*****************************************************************/
int reqID = 0;
int numReq = 0;
pthread_mutux_t mut;
pthread_cond_t  worker_cv;

/*****************************************************************/
struct account{
    int ID = 0;
    int bal = 0;
};

struct account accounts[numAccounts];

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

int appserver(char **args){

    int numThreads = args[0];
    int numAccounts = args[1];
    char *outFile = args[2];

    pthread_mutux_init(&mut, NULL);
    pthread_cond_init(&worker_cv, NULL);
    pthread_t threads[numThreads];

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
void *worker(void *arg){

    pthread_mutux_lock(&mut);
    while(numReq == 0)
        pthread_cond_wait(&threads, &mut);

    //go
}

//Balance check
int check(char **args){
    reqID++;
    if(args[1] < 0){
        //throw
    }else{
        int i = args[1];
    }

    struct account x = accounts[i];

    //printf(reqID + " BAL " + x.bal);
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

    for(int i = 0; i<4; i++){
        if(strcmp(args[0], inputs[i]) == 0) return *functions[i](args);
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