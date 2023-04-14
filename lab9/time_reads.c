/* The purpose of this program is to practice writing signal handling
 * functions and observing the behaviour of signals.
 */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

/* Message to print in the signal handling function. */
#define MESSAGE "%ld reads were done in %ld seconds.\n"

/* Global variables to store number of read operations and seconds elapsed. 
 */
long num_reads, seconds;

void sigprof_handler(int signum);

/* The first command-line argument is the number of seconds to set a timer to run.
 * The second argument is the name of a binary file containing 100 ints.
 * Assume both of these arguments are correct.
 */

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: time_reads s filename\n");
        exit(1);
    }
    seconds = strtol(argv[1], NULL, 10);

    FILE *fp;
    if ((fp = fopen(argv[2], "r")) == NULL) {
      perror("fopen");
      exit(1);
    }
    struct sigaction sa;
    sa.sa_handler = sigprof_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGPROF, &sa, NULL) == -1) {
        printf("sigaction");
        return 1;
    }

    struct itimerval timer;
    timer.it_value.tv_sec = seconds;        // Initial expiration time (seconds)
    timer.it_value.tv_usec = 0;       // Initial expiration time (microseconds)
    timer.it_interval.tv_sec = seconds;     // Interval between expiration times (seconds)
    timer.it_interval.tv_usec = 0;    // Interval between expiration times (microseconds)

    if (setitimer(ITIMER_PROF, &timer, NULL) == -1) {
        perror("setitimer");
        exit(1);
    }

    for (;;) {
        int num_lines = 0;
        char line[9];
        while (fgets(line, 9, fp) != NULL) {
            num_lines++;
        }
        rewind(fp);
        int random_line = rand() % 100;

        // Read the random line
        for (int i = 0; i < random_line; i++) {
            fgets(line, 9, fp);
        }

        // Print the integer to stderr
        fprintf(stderr, "%s", line);
        num_reads++;
    }
    return 1; // something is wrong if we ever get here!
}

void sigprof_handler(int signum) {
    printf(MESSAGE, num_reads, seconds);
    exit(0);
}
