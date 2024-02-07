#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void userdetails(int iters) {
	printf("USER PID:%d PPID:%d Iteration:%d ", getpid(), getppid(), iters);
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <iterations>\n", argv[0]);
		return 1;
	}

	int iters = atoi(argv[1]);

	for (int i = 1; i <= iters; i++) {
		userdetails(i);
		printf("before sleeping\n");

		sleep(1);

		userdetails(i);
		printf("after sleeping\n");
	}

	return 0;
}
