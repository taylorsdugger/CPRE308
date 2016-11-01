//
// Created by taylorsdugger on 10/31/16.
//

#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void my_routine( );

int ret;

int main() {
    ret = fork( );
    signal(SIGINT, my_routine);
    printf("Entering infinite loop\n");

    while(1) { sleep(10); }
    printf("Can’t get here\n");
}

void my_routine() {
    printf("Return value from fork = %d\n", ret);
}