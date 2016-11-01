//
// Created by taylorsdugger on 10/31/16.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MSGSIZE 16

int main() {
    char *msg = "How are you?";
    char inbuff[MSGSIZE];

    int p[2];
    int ret;

    pipe(p);
    ret = fork();

    if (ret > 0) {
        write(p[1], msg, MSGSIZE);
    } else {
        sleep(1);
        read(p[0], inbuff, MSGSIZE);
        printf("%s\n", inbuff);
    }

    exit(0);
}