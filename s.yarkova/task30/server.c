#include <stdio.h>
#include <string.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

void checkMistake(int* sFd) {
    if ((*(sFd) = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket creation error ");
        exit(0);
    }
}

void checkMistakeAccept(int* sFd, int* clFd) {
    if ((*(clFd) = accept(*(sFd), NULL, NULL)) == -1) {
        perror("client connection error");
        exit(0);
    }
}

int main() {
    struct sockaddr_un server_un;
    char buff[100];
    int sFd, clFd;

    checkMistake(&sFd);
    
    memset(&server_un, 0, sizeof(server_un));

    server_un.sun_family = AF_UNIX;
    strncpy(server_un.sun_path, "socket2", sizeof(server_un.sun_path) - 1);
    unlink(server_un.sun_path);

    if (bind(sFd, (struct sockaddr*)&server_un, sizeof(server_un)) == -1) {
        perror("bind creation error");
        exit(0);
    }

    listen(sFd, 1);

    checkMistakeAccept(&sFd, &clFd);

    while (1) {
        int n = read(clFd, buff, 100);
        if (n != -1) {
            for (int i = 0; i < n; ++i)
                printf("%c", toupper(buff[i]));
        }
        else {
            exit(0);
        }
    }
    close(sFd);
    return 0;
}