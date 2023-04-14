#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define PORT 59664
#define SERVER_ADDRESS "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create client socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_ADDRESS, &serv_addr.sin_addr) <= 0) {
        perror("invalid address");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connection failed");
        exit(EXIT_FAILURE);
    }

    char input[BUFFER_SIZE];
    printf("What is your user name?\n");
    fgets(input, BUFFER_SIZE, stdin);
    int input_len = strlen(input);
    write(sock, input, input_len);
    printf("Welcome.\nGo ahead and enter commands>\n");

    // Send input to server
    while (fgets(input, BUFFER_SIZE, stdin) != NULL) {
        int input_len = strlen(input);
        write(sock, input, input_len);

        // Receive response from server
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = read(sock, buffer, BUFFER_SIZE);
        if (bytes_received < 0) {
            perror("read failed");
            exit(EXIT_FAILURE);
        }
        printf("%s", buffer);
    }

    // Close client socket
    close(sock);

    return 0;
}


// char username[32] = {0};
//         int username_len = 0;

//         // Read username byte by byte until newline character is detected
//         char c;
//         while (read(client_fd, &c, 1) == 1) {
//             if (c == '\n') {
//                 username[username_len++] = c;
//                 break;
//             }
//             if (username_len < 31) {
//                 username[username_len++] = c;
//             }
//         }

//         username[31] = '\0';

//         struct user* new_user = (struct user*)malloc(sizeof(struct user));
//         strcpy(new_user->name, username);
//         new_user->next = NULL;

//         if (head == NULL) {
//             head = new_user;
//         } else {
//             User *curr = head;
//             while (curr->next != NULL) {
//                 curr = curr->next;
//             }
//             curr->next = new_user;
//         }


// void process_command(char *command, char *output) {
//     char *saveptr;
//     char *token = strtok_r(command, " ", &saveptr);

//     if (strcmp(token, "list_users\n") == 0) {
//         sprintf(output, "Listing users: Alice, Bob, Charlie\n");
//     } else if (strcmp(token, "make_friends") == 0) {
//         char *username = strtok_r(NULL, " ", &saveptr);
//         if (username == NULL) {
//             sprintf(output, "Incorrect syntax\n");
//         } else {
//             sprintf(output, "Making friends with user %s\n", username);
//         }
//     } else if (strcmp(token, "post") == 0) {
//         char *target = strtok_r(NULL, " ", &saveptr);
//         char *msgpiece = strtok_r(NULL, "", &saveptr);
//         if (target == NULL || msgpiece == NULL) {
//             sprintf(output, "Incorrect syntax\n");
//         } else {
//             // Remove extra newline characters from msgpiece
//             int msgpiece_len = strlen(msgpiece);
//             if (msgpiece[msgpiece_len-1] == '\n') {
//                 msgpiece[msgpiece_len-1] = '\0';
//             }
//             sprintf(output, "Posting to %s: %s\n", target, msgpiece);
//         }
//     } else if (strcmp(token, "profile") == 0) {
//         char *username = strtok_r(NULL, " \n", &saveptr);
//         if (username == NULL) {
//             sprintf(output, "Incorrect syntax\n");
//         } else {
//             sprintf(output, "Displaying profile for user %s\n", username);
//         }
//     } else if (strcmp(token, "quit\n") == 0) {
//         sprintf(output, "Exiting server\n");
//     } else {
//         sprintf(output, "Incorrect syntax\n");
//     }
// }