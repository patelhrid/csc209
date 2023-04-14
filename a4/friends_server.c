#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "friends.h"

#define PORT 59664
#define BUFFER_SIZE 1024

struct user_socket {
    int client_fd;
    char *username;
    struct user_socket *next;
};

int connect_to_client(int client, struct user_socket *users, User **head) {
    int i = 0;
    while (users[i].client_fd != -1) {
        i++;
    }

    int client_fd = accept(client, NULL, NULL);
    if (client_fd < 0) {
        perror("server: accept");
        close(client);
        exit(1);
    }

    users[i].client_fd = client_fd;
	
	char username_input[BUFFER_SIZE];
	int num_read = read(client_fd, &username_input, BUFFER_SIZE);
	if (num_read == -1){
		exit(0);
	}
	username_input[num_read - 1] = '\0';
	char *username = malloc((sizeof(char) * num_read) + 1);
	strncpy(username, username_input, num_read + 1);
    users[i].username = username;
    create_user(username, head);
    
	return client_fd;
}

int read_client(int client_index, struct user_socket *users, User **head) {
    int fd = users[client_index].client_fd;
    char *username = users[client_index].username;
    char command[BUFFER_SIZE + 1];
    int command_len = 0;
    char c;

    while (read(fd, &c, 1) == 1) {  
            if (c == '\n') {  
                command[command_len++] = c;  
                break;  
            }  
            if (command_len < BUFFER_SIZE - 1) {  
                command[command_len++] = c;  
            }  
        }  
  
    // Null-terminate the command string  
    command[command_len] = '\0';  

    char *saveptr; 
    char *token = strtok_r(command, " ", &saveptr);  
    char output[BUFFER_SIZE] = {0};  
  
    if (strcmp(token, "list_users\n") == 0) {  
        // printf("BEFORE\n");
        char **usernames = list_users(*head);   
        // printf("AFTER\n");
        int i = 0;  
        // strcat(output, "User List\n");  
        while (usernames[i] != NULL) {  
            // strcat(output, "\t");
            strcat(output, usernames[i]);  
            strcat(output, "\n");  
            i++;  
        }  
        // strcat(output, "\n");  
        free(usernames);
    } else if (strcmp(token, "make_friends") == 0) {  
        char *friend = strtok_r(NULL, " ", &saveptr); 
        int len = strlen(friend);
        if (len > 0 && friend[len-1] == '\n') {
            friend[len-1] = '\0';
        } 
        // printf("%s\n", friend);
        if (friend == NULL) {  
            sprintf(output, "Incorrect syntax\n");  
        } else {  
            int result = make_friends(username, friend, *head);  
            if (result == 0){
                sprintf(output, "You are now friends with %s.\n", friend);
            } else if (result == 1){
                sprintf(output, "You are already friends.\n");  
            } else if (result == 2){
                sprintf(output, "A user has reached the maximum number of friends\n");  
            } else if (result == 3){
                sprintf(output, "You can't friend yourself\n");  
            } else if (result == 4){
                sprintf(output, "The user you entered does not exist\n");  
            }
        }  
    } else if (strcmp(token, "post") == 0) {  
        char *target = strtok_r(NULL, " ", &saveptr);
        char *msgpiece = strtok_r(saveptr, "\r\n", &saveptr);
        if (target == NULL || msgpiece == NULL) {
            sprintf(output, "Incorrect syntax\n");
        } else {
            // Remove extra newline characters from msgpiece
            int msgpiece_len = strlen(msgpiece);
            if (msgpiece_len > 0 && msgpiece[msgpiece_len-1] == '\n') {
                msgpiece[msgpiece_len-1] = '\0';
            }
            
            // Find the user who authored the post
            User *author = find_user(username, *head);
            
            // Find the user who is the target of the post
            User *target_user = find_user(target, *head);
            if (target_user == NULL) {
                sprintf(output, "The user '%s' does not exist.\n", target);
            }
            
            // Create the post and print a message if there is an error
            // printf("BEFORE\n");
            int result = make_post(author, target_user, msgpiece);
            // printf("AFTER\n");
            if (result == 1) {
                sprintf(output, "You can only post to your friends.\n");
            } else if (result == 2) {
                sprintf(output, "The user you want to post to does not exist");
            } 
        }
    } else if (strcmp(token, "profile") == 0) {  
        char *profile = strtok_r(NULL, " \n", &saveptr);  
        if (profile == NULL) {  
            sprintf(output, "Incorrect syntax\n");  
        }  
        User *profile_user = find_user(profile, *head);  
        if (profile_user == NULL){  
            sprintf(output, "User not found\n");  
        } else {  
            char *profile_str = print_user(profile_user);  
            strcat(output, profile_str);  
            free(profile_str);
        }  
    } else if (strcmp(token, "quit\n") == 0) {  
        close(users[client_index].client_fd);
        // users->client_fd = -1;
        return 0;  
    } else {  
        sprintf(output, "Incorrect syntax\n");  
    }  

    // printf("BEFORE\n");
    write(users[client_index].client_fd, output, strlen(output));
    // printf("AFTER\n");

    return 0;
}

int main(void) {
    User *head = NULL;
    struct user_socket users[1024];
    for (int index = 0; index < 1024; index++) {
        users[index].client_fd = -1;
        users[index].username = NULL;
    }

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Server Socket Error");
        exit(1);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int on = 1;
    int status = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char *) &on, sizeof(on));
    if (status == -1) {
        perror("REUSEADDR error (setsockopt)");
    }

    memset(&server_addr.sin_zero, 0, 8);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Server Bind Error");
        close(server_fd);
        exit(1);
    }

    if (listen(server_fd, 1024) < 0) {
        perror("Server Listen Error");
        close(server_fd);
        exit(1);
    }

    int max = server_fd;
    fd_set all_fds;
    FD_ZERO(&all_fds);
    FD_SET(server_fd, &all_fds);

    while (1) {
        fd_set listen_fds = all_fds;
        if (select(max + 1, &listen_fds, NULL, NULL, NULL) == -1) {
            perror("Server Select Error");
            exit(1);
        }

        if (FD_ISSET(server_fd, &listen_fds)) {
            int client_fd = connect_to_client(server_fd, users, &head);
            if (client_fd > max) {
                max = client_fd;
            }
            FD_SET(client_fd, &all_fds);
        }

        for (int index = 0; index < 1024; index++) {
            if (users[index].client_fd > -1 && FD_ISSET(users[index].client_fd, &listen_fds)) {
                int client_closed = read_client(index, users, &head);
                if (client_closed > 0) {
                    FD_CLR(client_closed, &all_fds);
                }
            }
        }
    }
}