#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <getopt.h>
#define MAX_PROCESSES 100

void printUsage(const char *programName) {
    fprintf(stderr, "Usage: %s -n <total-process> -s <simultaneous-process> -t <iteration>\n", programName);
}

void singleuserprocess(int iters) {
    char itersarg[10];
    snprintf(itersarg, sizeof(itersarg), "%d", iters);

    execl("./user", "./user", itersarg, NULL);
    perror("execl");
    exit(1);
}

void launchuserprocess(int totalprocess, int simultaneousprocess, int iters) {
    for (int i = 0; i < totalprocess; i++) {
        if (i % simultaneousprocess == 0 && i != 0) {
            int status;
            wait(&status);
        }

        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(1);
        }

        if (pid == 0) {
            singleuserprocess(iters);
        }
    }

    int status;
    while (wait(&status) > 0);
}

int main(int argc, char *argv[]) {
    int totalprocess = 0;
    int simultaneousprocess = 0;
    int iters = 0;

    int opt;
    while ((opt = getopt(argc, argv, "n:s:t:h")) != -1) {
        switch (opt) {
            case 'n':
                totalprocess = atoi(optarg);
                break;
            case 's':
                simultaneousprocess = atoi(optarg);
                break;
            case 't':
                iters = atoi(optarg);
                break;
            case 'h':
                printUsage(argv[0]);
                return 0;
            default:
                fprintf(stderr, "Unknown option: %c\n", optopt);
                return 1;
        }
    }

    if (totalprocess <= 0 || simultaneousprocess <= 0 || iters <= 0) {
        fprintf(stderr, "Invalid input. Enter values for n, s, and t.\n");
        return 1;
    }

    launchuserprocess(totalprocess, simultaneousprocess, iters);

    return 0;
}
