#include <stdio.h>
#include <string.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <aio.h>
#include <signal.h>

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
    if (select(*(max)+1, newSetFd, NULL, NULL, NULL) == -1) {
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

void sig_event(int sigNumber, siginfo_t* info, void* context){
    struct aiocb* new = info->si_value.sival_ptr;
    
    if (aio_error(new) != 0) {
        perror("error sig_event");
        exit(0);
    }
    size_t n = aio_return(new);
    char* buff = (char*)new->aio_buf;
    if (n != 0) {
        for (int i = 0; i < n; ++i)
            printf("%c", toupper(buff[i]));
        aio_read(new);
    }
    else {
        close(new->aio_fildes);
        free(new);
        free(buff);
    }
    
}


int main() {
    struct sockaddr_un server_un;
    struct sockaddr clAddr;
    struct sigaction sig;
    char buff[100];
    int sFd, clFd;

    memset(&sig, 0, sizeof(sig));
    sig.sa_sigaction = sig_event;
    sig.sa_flags = SA_SIGINFO | SA_RESTART;
    sigaction(SIGIO, &sig, NULL);


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

    int max = sFd;
    while (1) {
        checkAccept(&clFd);

        struct aiocb* cb = calloc(1, sizeof(struct aiocb));
        cb->aio_fildes = clFd;
        cb->aio_buf = malloc(100);
        cb->aio_nbytes = 100;
        cb->aio_sigevent.sigev_signo = SIGIO;
        cb->aio_sigevent.sigev_value.sival_ptr = cb;

        if (aio_read(cb) == -1) {
            perror("Failed to queue the read request: ");
            close(sFd);
            close(clFd);
        }
    }
    close(sFd);
    return 0;
}