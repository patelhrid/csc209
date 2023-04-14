#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 256

#define SUCCESS "Password verified\n"
#define INVALID "Invalid password\n"
#define NO_USER "No such user\n"

int main(void) {
  char user_id[MAXLINE];
  char password[MAXLINE];
  int status;
  int pipe_fd[2];

  if (fgets(user_id, MAXLINE, stdin) == NULL) {
      perror("fgets");
      exit(1);
  }
  if (fgets(password, MAXLINE, stdin) == NULL) {
      perror("fgets");
      exit(1);
  }

  if (pipe(pipe_fd) == -1) {
      perror("pipe");
      exit(1);
  }

  int frk = fork();
  if (frk == -1){
      perror("fork");
      exit(1);
  }
  else if (frk == 0){ //CHILD
    close(pipe_fd[1]);
    dup2(pipe_fd[0], fileno(stdin));
    close(pipe_fd[0]);
    execl("./validate", "validate", NULL);
    perror("exec");
    exit(1);
    return 1;
  }
  else { // PARENT
    close(pipe_fd[0]);
    if (write(pipe_fd[1], user_id, 10) == -1) {
		perror("writing");
		exit(1);
    }
    if (write(pipe_fd[1], password, 10) == -1) {
		perror("writing");
		exit(1);
    }
    close(pipe_fd[1]);
  }

    if (wait(&status) == -1){
	    perror("Wait");
	    exit(1);
    }

	int exit = WEXITSTATUS(status);
    if (exit == 0){
        printf(SUCCESS);
    } else if (exit == 2){
        printf(INVALID);
    } else if (exit == 3){
        printf(NO_USER);	
    }
    return 0;
}
