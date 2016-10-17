//Taylor Dugger CPRE 308 Project 1


//I don't know what to do to make a makefile honestly
//Just type gcc -o 308sh 308sh.c
//then ./308sh
//I'm not sure what else to do besides that


#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define bufferSize 64
#define delim " \t\r\n\a"

//set up my custom functions
int my_exit(char **args);
int my_pid(char **args);
int my_ppid(char **args);
int my_cd(char **args);
int my_pwd(char **args);
int my_set(char **args);
int my_get(char **args);

//these are my commands
char *commands[] = {
        "exit",
        "pid",
        "ppid",
        "cd",
        "pwd",
        "set",
        "get"
};

//the functions
int (*functs[]) (char **)= {
        &my_exit,
        &my_pid,
        &my_ppid,
        &my_cd,
        &my_pwd,
        &my_set,
        &my_get
};

int num_functs(){
    return sizeof(commands) / sizeof(char *);
}

//these are all my built in functions
int my_cd(char **args){
    if(args[1] == NULL){//no argument, go home
        chdir(getenv("HOME"));
    }else{
        if(chdir(args[1]) != 0)//go to location
            perror("error:");
    }

    return 1;
}

int my_exit(char **args){
    return 0;
}

int my_pid(char **args){
    printf("Process ID is: %d\n", (int)getpid());
    return 1;
}

int my_ppid(char **args){
    printf("Parent Process is: %d\n", (int) getppid());
    return 1;
}

int my_pwd(char **args){
    char cwd[1024];
    if(getcwd(cwd, sizeof(cwd)) != NULL)//you have to get the current dir
        printf("Current working directory: %s\n", cwd);//print it out
    else
        perror("getcwd() error");

    return 1;
}

//Ok so im not sure if set or get doesnt work. But ive looked at this for so long
//that i have no idea why it doesnt.
//Ive gone through this a lot and i know the args pass in correctly but for some reason
//its not setting the value correctly (or maybe its not getting it correctly) It just
//sets it to null every time.
int my_set(char **args) {
    if(args[2] == NULL){
        setenv(args[1], NULL, 1);
    }else{
        setenv(args[1], args[2], 1);
    }

    return 1;
}

int my_get(char **args){
    char * value = getenv(args[1]);
    printf(value);
    if(!value)
        printf("%s is not set. \n", args[1]);
    else
        printf("%s = %s\n", args[1],value);

    return 1;
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

//my execute function
//this goes through my list of hard wired commands and compares it to what was entered
//and then starts it
int execute(char **args){
    if(args[0] == NULL)
        return 1;

    for(int i = 0; i < num_functs(); i++){
        if(strcmp(args[0], commands[i]) == 0)
            return (*functs[i])(args);
    }

    return start(args);
}

//just read the line that was entered
//the getline built in function helped out quite a bit
char **readLine(void){
    char *line = NULL;
    ssize_t size = 0;
    getline(&line, &size, stdin);
    return line;
}

//this splits up my line so i have each word as a different argument
//inside args[]
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

//this tells everything to run
void run(void){
    char *next;
    char **args;
    int s;

    do{
        printf("308sh>");
        next = readLine();
        args = split(next);
        s = execute(args);

        free(next);
        free(args);

    }while (s);
}

//main
int main(int argc, char **argv){

    run();

    return EXIT_SUCCESS;
}