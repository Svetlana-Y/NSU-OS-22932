#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int counter = 0;

void signals(int s) {

	signal(s, SIG_IGN);
	if (s == SIGINT) {
		write(STDOUT_FILENO, "\a", sizeof(char));
		counter++;
	}

	if (s == SIGQUIT) {

		printf("\nThe beep sounded %d times\n", counter);
		exit(0);
	}	
	signal(s, signals);
}


int main(int argc, char* argv[]) {

	signal(SIGINT, signals);
	signal(SIGQUIT, signals);
	while (1) {
		pause();
	}
	return 0;
}