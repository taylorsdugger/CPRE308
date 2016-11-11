/*Created by Taylor Dugger CPRE 308 OCT 28 2016

OK....so this project doesn't work. I tried for hours to get the input working correctly
I keep getting segmentation faults everytime i run, so i technically dont know if the rest
even works. I tried for hours and with no success. But i coded the rest in and hopefully
they are close to right or right. I am hoping I can get some partial credit for those functions
even though the project wont run. I spent many hours on this, and struggled alot. Probably
over 18. i know i didn't use full advantage of tas and office hours, but I tried to get it
 done myself. So while this doesn't work completely, I am hoping for partial credit for all
 the work that I have put in.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "Bank.c"
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
int balReq = 0;
int tranReq = 0;

pthread_mutex_t mut;
pthread_cond_t  check_cv;
pthread_cond_t  trans_cv;
pthread_cond_t  end_cv;

/*****************************************************************/
//Stucts for storing along the way
struct account{
    pthread_mutex_t lock;
    int ID;
};

struct balCheck{
    int AccID;
    int bal;
};

struct tranReq{
    int AccID;
    int amnt;
};

struct account accounts[];
struct balCheck bals[];
struct tranReq ts[];

/*****************************************************************/
//what the user can enter
char *inputs[] = {
    "appserver",
    "CHECK",
    "TRANS",
    "END"
};

//the functions to run
int (*functions[])(char **)={
        &appserver,
        &check,
        &trans,
        &end
};

//how many functions avaible
int numOfFunctions(){
    return sizeof(inputs) / sizeof(char *);
}

//my manin appserver function
int appserver(char **args){

    //get some basic info from the user
    int numThreads = args[1];
    int numAccounts = args[2];
    char *outFile = args[3];

    int thread_index[numThreads];

    //my conditions
    pthread_cond_init(&check_cv, NULL);
    pthread_cond_init(&end_cv, NULL);
    pthread_cond_init(&trans_cv, NULL);

    //create the threads
    pthread_t threads[numThreads];

    //initialze the accounts
    struct account accounts[numAccounts];
    initialize_accounts(numAccounts);
    for(int i = 0; i < numAccounts; i++){
        accounts[i].ID = i;
    }

    //create workers
    for(int j = 0; j < numThreads; j++){
        thread_index[j] = j;
        pthread_create(&threads[j], NULL, worker, (void *)&thread_index[j]);
    }

    //and then join the threads
    for(int k = 0; k < numThreads; k++){
        pthread_join(threads[k], NULL);
    }

    return 1;
}

//Worker threads
void *worker(){

    //lock
    pthread_mutex_lock(&mut);

    //looks to see if any balReq are pending
    while(balReq == 0){
        pthread_cond_wait(&check_cv, &mut);}

    //this is the work for bal requests
    //first lock the specific account and read
    do{
        pthread_mutex_lock(&(accounts[bals[reqID].AccID]).lock);
        int val = read_account(bals[reqID].AccID);

        printf("%d BAL %d", reqID, val);

        balReq--;
    }while(balReq > 0);


    //this is for handling transaction requests
    while(tranReq == 0){
        pthread_cond_wait(&trans_cv, &mut);//check to see if its needs to wait
    }

    //do some work on the transaction
    do{
        pthread_mutex_lock(&(accounts[ts[reqID].AccID].lock));
        write_account(ts[reqID].AccID, ts[reqID].amnt);
    }while(tranReq > 0);

    pthread_mutex_unlock(&mut);
}

//Balance check
int check(char **args){
    int i = 0;

    //see if the account is positive
    if(args[1] < 0){
        exit(1);
    }else{
        i=args[1];
    }

    //lock
    pthread_mutex_lock(&mut);
    pthread_mutex_lock(&(accounts[i].lock));

    //add new reqs
    reqID++;
    balReq++;

    //set account id
    bals[reqID].AccID = i;

    //call the thread
    pthread_cond_broadcast(&check_cv);

    //print the reqID
    printf("ID %d", reqID);

    //unlock
    pthread_mutex_unlock(&mut);

    return 1;
}

//this is my transaction function
int trans(char **args){

    int numTrans = 0;

    //figure out how many there are up to 10
    for(int i =1; i < 22; i+=2){
        if(args[i] != NULL){
            numTrans++; //Figure out how many transactions we need
        }
    }

    //lock
    pthread_mutex_lock(&mut);

    //go through each transaction
    for(int j = 1; j < numTrans *2; j++){
        pthread_mutex_lock(&(accounts[j].lock));
        reqID++;//add new reqs
        tranReq++;

        //update the struct with the info
        ts[reqID].AccID = args[j];
        ts[reqID].amnt = args[j++];

        //call thread
        pthread_cond_broadcast(&trans_cv);

        //print reqID
        printf("ID %d", reqID);
    }

    //unlock
    pthread_mutex_unlock(&mut);

    return 1;
}

//function for end
int end(char **args){

    pthread_mutex_lock(&mut);

    //wait to finish
    while(balReq > 0 || tranReq>0)
        pthread_cond_wait(&end_cv, &mut);

    pthread_mutex_unlock(&mut);

    //quite
    exit(EXIT_SUCCESS);

    return 1;
}

//ReadLine function
//the next few functions i used in project 1, so not much documentation
char *readLine(void){
    char *line = NULL;
    size_t size;
    getline(&line, &size, stdin);
    return line;
}

//function for splitting lines
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
    printf(list);
    return list;
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

//the execute which calls right function
int execute(char **args){
    if(args[0] == NULL){return 1;}

    for(int i = 0; i<4; i++){
        printf(strcmp(args[0], inputs[0]));

        if(strcmp(args[0], inputs[i]) == 0) return (*functions[i])(args);
    }

    return start(args);
}

//run the proj
void run(void){
    char *next[100];
    char **donut;
    int s;

    do{
        printf("project2: ");
        strcpy(readLine(), *next);
        donut = split(next);
        s = execute(&donut);

        free(donut);

    }while(s);

}

//main
int main(int argc, char **argv){

    run();

    return EXIT_SUCCESS;
}