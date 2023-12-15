#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void checkMistake(int* fd) {
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(0);
    }
}

void parentPr(int* fd) {
    close(fd[0]);

    char* txt = "Some KiNd of TExt To ChecK ";
    write(fd[1], txt, strlen(txt)+1);
    close(fd[1]);
    wait(NULL);
}


void childPr(int* fd) {
    close(fd[1]);

    char buff[100];
    int n = read(fd[0], buff, sizeof(buff));

    for (int i = 0; i < n; i++) {
        buff[i] = toupper(buff[i]);
    }
    printf("txt - upper case: %s\n", buff);
    close(fd[0]);
    exit(0);
}

int main() {
    int fd[2];
    pid_t pid;

    checkMistake(fd);
    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(0);
    }
    else if (pid > 0) {
        parentPr(fd);
    }
    else { 
        childPr(fd);
    }
    return 0;
}