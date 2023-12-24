#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h> 

struct sockaddr_un client_un;
char* socket_path = "./socket2";

void checkMistakeCl(int fd) {
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket creation error ");
        exit(0);
    }
}

void checkMistakeConnect(int fd) {
    if (connect(fd, (struct sockaddr*)&client_un, sizeof(client_un)) == -1) {
        perror("connect error");
        exit(-1);
    }
}

void checkSend(int fd, char buff[100], int n) {
    if (n != 1) {
        send(fd, buff, n, 0);

    }
    else {
        perror("Connect mistake ");
        exit(0);
    }
}



int main() {   
    char buff[100];
    int fd;

    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket creation error ");
        exit(0);
    }

    memset(&client_un, 0, sizeof(client_un));
    client_un.sun_family = AF_UNIX;
    strncpy(client_un.sun_path, socket_path, sizeof(client_un.sun_path) - 1);

    if (connect(fd, (struct sockaddr*)&client_un, sizeof(client_un)) == -1) {
        perror("connect error");
        exit(-1);
    }

    while (1) {
        buff[0] = '\0';
        int n = read(STDIN_FILENO, buff, sizeof(buff));
        if (n != -1) {
            send(fd, buff, n, 0);

        }
        else {
            perror("Connect mistake ");
            exit(0);
        }
    }

    return 0;
}
