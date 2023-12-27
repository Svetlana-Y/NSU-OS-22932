#include <stdio.h>
#include <string.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

struct sockaddr_un client_un;
char* socket_path = "./socket2";

void checkMistakeCl(int* fd) {
    if ((*(fd) = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket creation error ");
        exit(0);
    }
}

void checkMistakeConnect(int* fd) {
    if (connect(*(fd), (struct sockaddr*)&client_un, sizeof(client_un)) == -1) {
        perror("connect error");
        exit(0);
    }
}

int main() {
    int fd;
    checkMistakeCl(&fd);

    memset(&client_un, 0, sizeof(client_un));
    client_un.sun_family = AF_UNIX;
    strncpy(client_un.sun_path, socket_path, sizeof(client_un.sun_path) - 1);

    checkMistakeConnect(&fd);

    while (1) {
        char buff[100];
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
