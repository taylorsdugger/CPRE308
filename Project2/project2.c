#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "Bank.c"
#include "Bank.h"

/*****************************************************************/
int appserver(char **args);
int check(char **args);
int trans(char **args);
int end(char **args);
void *worker();

/*****************************************************************/
int reqID = 0;
int balReq = 0;
int tranReq = 0;

pthread_mutex_t mut;
pthread_cond_t  check_cv;
pthread_cond_t  trans_cv;
pthread_cond_t  end_cv;

/*****************************************************************/
struct account{
    pthread_mutex_t lock;
    int ID;
};

struct balCheck{
    int AccID;
    int bal;
};

struct account accounts[];
struct balCheck bals[];

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

    printf("%c,  %d", args[0], args[1]);
    int numThreads = args[1];
    int numAccounts = args[2];
    char *outFile = args[3];

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

    pthread_mutex_lock(&mut);

    while(balReq == 0)
        pthread_cond_wait(&check_cv, &mut);

        do{
            pthread_mutex_lock(&(accounts[bals[reqID].AccID]).lock);
            int val = read_account(bals[reqID].AccID);

            balReq--;
        }while(balReq > 0);

        do{

        }while(tranReq > 0);


    //numReq--;
    pthread_mutex_unlock(&mut);
}

//Balance check
int check(char **args){
    int i = 0;

    if(args[1] < 0){
        exit(1);
    }else{
        i=args[1];
    }

    pthread_mutex_lock(&mut);
    //pthread_mutex_lock(&(accounts[i].lock));

    reqID++;
    balReq++;

    bals[reqID].AccID = i;

    pthread_cond_broadcast(&check_cv);

    printf("ID %d", reqID);

    pthread_mutex_unlock(&mut);

    return 1;
}

int trans(char **args){


    return 1;
}

int end(char **args){

    pthread_mutex_lock(&mut);

    while(balReq > 0 || tranReq>0)
        pthread_cond_wait(&end_cv, &mut);

    pthread_mutex_unlock(&mut);

    exit(EXIT_SUCCESS);

    return 1;
}

//ReadLine function
char *readLine(void){
    char *line = NULL;
    size_t size;
    getline(&line, &size, stdin);
    return line;
}

char **split(char *in){
    char *word = strtok(in, " ");
    char *result[12];
    int i =0;

    while(word != NULL){
        result[i++]=  word;
        word= strtok(NULL, " ");
    }

    return result;
    /*for(int i = 0; i < (nspace+1); i++){

    }*/

    //printf("%c, %c, %c\n", word[0], word[1], word[2]);
    //return word;

    /*printf("%c\n", in);
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
    //list[pos] = NULL;
    printf(list);
    return list;*/
}

//this is the starting function for each command
int start(char **args){
    pid_t pid, ppid;
    int status;

    pid = fork();//it creates a child process
    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("error");
        }
        exit(EXIT_FAILURE);

    } else if (pid < 0) {
        perror("error");

    } else {
        do {
            ppid = waitpid(pid, &status, WUNTRACED);//the parent will wait for child
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int execute(char **args){
    //if(args[0] == NULL){return 1;}

    for(int i = 0; i<4; i++){
        printf(strcmp(args[0], inputs[0]));

        if(strcmp(args[0], inputs[i]) == 0) return (*functions[i])(args);
    }

    return start(args);
}

void run(void){
    char *next[100];
    char **donut;
    int s;

    do{
        printf("project2: ");
        next = readLine();
        //donut = split(next);
        //donut = strsep(&next, " ");
        //s = execute(&donut);

        free(next);
        free(donut);

    }while(s);

}

int main(int argc, char **argv){

    run();

    return EXIT_SUCCESS;
}