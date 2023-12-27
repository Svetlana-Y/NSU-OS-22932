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

void checkSelect(int* max, fd_set* newSetFd) {
    if (select(*(max) + 1, newSetFd, NULL, NULL, NULL) == -1) {
        perror("Error occurred with the client selection ");
        exit(0);
    }
}

void checkAccept(int* clFd) {
    if (*(clFd) == -1) {
        perror("Ñonnection error has occurred ");
        exit(0);
    }
}


int main() {
    struct sockaddr_un server_un;
    struct sockaddr clAddr;

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

    listen(sFd, 5);
    fd_set setFd;
    FD_ZERO(&setFd);
    FD_SET(sFd, &setFd);

    int max = sFd;
    while (1) { 
        fd_set newSetFd = setFd;
        
        checkSelect(&max, &newSetFd);

        if (FD_ISSET(sFd, &newSetFd)) {
            socklen_t c_address_len = sizeof(clAddr);
            clFd = accept(sFd, &clAddr, &c_address_len);
           
            checkAccept(&clFd);
           
            FD_SET(clFd, &setFd);
            if (clFd > max) max = clFd;
        }

        for (int i = 0; i <= max; i++) {       
            if (FD_ISSET(i, &newSetFd)) {
                if (i != sFd) {
                    int n = read(i, buff, 100);
                    if (n != -1) {
                        for (int j = 0; j < n; j++)
                            printf("%c", toupper(buff[j]));
                    }
                    else {
                        exit(0);
                    }
                }
                else continue;
            }
        }
    }
    close(sFd);
    return 0;
}