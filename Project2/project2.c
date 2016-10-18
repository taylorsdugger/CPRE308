#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){

    appserver();

    return EXIT_SUCCESS;
}

void appserver(void){
    char *input;
    char **args;

    input = readLine();
    args = split(input);

    int numThreads = args[0];
    int numAccounts = args[1];
    char *outFile = args[2];

    free(next);
    free(args);
}

void account(int ID){
    int ID = ID;
}