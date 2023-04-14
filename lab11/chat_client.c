#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#ifndef PORT
  #define PORT 30000
#endif
#define BUF_SIZE 128

int main(void) {
    // Create the socket FD.
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("client: socket");
        exit(1);
    }

    // Set the IP and port of the server to connect to.
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) < 1) {
        perror("client: inet_pton");
        close(sock_fd);
        exit(1);
    }

    // Connect to the server.
    if (connect(sock_fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("client: connect");
        close(sock_fd);
        exit(1);
    }

    // Get the user to provide a name.
    char buf[2 * BUF_SIZE + 2]; // 2x to allow for usernames
    printf("Please enter a username: ");
    fflush(stdout);
    int num_read = read(STDIN_FILENO, buf, BUF_SIZE);
    if (num_read == 0) {
        close(sock_fd);
        exit(0);
    }
    buf[num_read] = '\0';
    if (write(sock_fd, buf, num_read) != num_read) {
        perror("client: write");
        close(sock_fd);
        exit(1);
    }

    // Set up the file descriptor sets for select()
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    FD_SET(sock_fd, &read_fds);

    int max_fd = (STDIN_FILENO > sock_fd) ? STDIN_FILENO : sock_fd;

    // Monitor stdin and the socket for input
    while (1) {
        fd_set tmp_fds = read_fds;
        if (select(max_fd + 1, &tmp_fds, NULL, NULL, NULL) == -1) {
            perror("client: select");
            close(sock_fd);
            exit(1);
        }

        // Check for input on stdin
        if (FD_ISSET(STDIN_FILENO, &tmp_fds)) {
            num_read = read(STDIN_FILENO, buf, BUF_SIZE);
            if (num_read == 0) {
                break;
            }
            buf[num_read] = '\0';

            if (write(sock_fd, buf, num_read) != num_read) {
                perror("client: write");
                close(sock_fd);
                exit(1);
            }
        }

        // Check for input on the socket
        if (FD_ISSET(sock_fd, &tmp_fds)) {
            num_read = read(sock_fd, buf, sizeof(buf) - 1);
            if (num_read == 0) {
                break;
            }
            buf[num_read] = '\0';
            printf("[Server] %s", buf);
        }
    }

    close(sock_fd);
    return 0;
}
