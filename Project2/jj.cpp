//
// Created by taylorsdugger on 10/27/16.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv){

    char *line[123];
    size_t size = 32;
    getline(&line, &size, stdin);

    printf(line);
    char *word = strtok(line, " ");
    char *result[12];
    int i =0;

    while(word != NULL){
        result[i++]=  word;
        word= strtok(NULL, " ");
    }

    printf("%c,  %c", result[0], result[1]);
}