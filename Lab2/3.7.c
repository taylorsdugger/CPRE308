#include <unistd.h>
#include <stdio.h>

int main() {
	execl("/binasf/ls", "lds", NULL);
	printf("What happened?\n");
}

