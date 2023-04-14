#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: forkloop <iterations>\n");
        exit(1);
    }

    int num = strtol(argv[1], NULL, 10);
    int i = 0;
    int stat;
    int pid = getpid();

    while (num != i){
        if (getppid() != pid && i != 0){
            exit(0);
        }
        else {
            printf("ppid = %d, pid = %d, i = %d\n", getppid(), getpid(), i);
            pid = getpid();
            int n = fork();
            wait(&stat);
            i += 1;
        }
    }

    // for (int i = 0; i < num; i++) {
    //     int n = fork();
    //     if (n < 0) {
    //         perror("fork");
    //         exit(1);
    //     }
    //     printf("ppid = %d, pid = %d, i = %d\n", getppid(), getpid(), i);
    // }

    return 0;
}
