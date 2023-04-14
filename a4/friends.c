#include "friends.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef PORT
  #define PORT 59663
#endif

/*
 * Create a new user with the given name. Insert it at the tail of the list
 * of users whose head is pointed to by *user_ptr_add.
 *
 * Return:
 *   - 0 on success.
 *   - 1 if a user by this name already exists in this list.
 *   - 2 if the given name cannot fit in the 'name' array
 *       (don't forget about the null terminator).
 */
int create_user(const char *name, User **user_ptr_add) {
       if (strlen(name) >= MAX_NAME) {
        return 2;
    }

    User *new_user = malloc(sizeof(User));
    if (new_user == NULL) {
        perror("malloc");
        exit(1);
    }
    strncpy(new_user->name, name, MAX_NAME); // name has max length MAX_NAME - 1

    for (int i = 0; i < MAX_NAME; i++) {
        new_user->profile_pic[i] = '\0';
    }

    new_user->first_post = NULL;
    new_user->next = NULL;
    for (int i = 0; i < MAX_FRIENDS; i++) {
        new_user->friends[i] = NULL;
    }

    // Add user to list
    User *prev = NULL;
    User *curr = *user_ptr_add;
    while (curr != NULL && strcmp(curr->name, name) != 0) {
        prev = curr;
        curr = curr->next;
    }

    if (*user_ptr_add == NULL) {
        *user_ptr_add = new_user;
        return 0;
    } else if (curr != NULL) {
        free(new_user);
        return 1;
    } else {
        prev->next = new_user;
        return 0;
    }
}


/*
 * Return a pointer to the user with this name in
 * the list starting with head. Return NULL if no such user exists.
 *
 * NOTE: You'll likely need to cast a (const User *) to a (User *)
 * to satisfy the prototype without warnings.
 */
User *find_user(const char *name, const User *head) {
    while (head != NULL && strcmp(name, head->name) != 0) {
        head = head->next;
    }

    return (User *)head;
}


/*
 * Return an array of dynamically allocated strings containing the usernames of all
 * users in the list starting at curr. The returned array must be freed by the caller
 * when it is no longer needed.
 */
char **list_users(const User *curr) {  
    // printf("User List\n");  
    int num_users = 0;  
    const User *p = curr;  
    while (p != NULL) {  
        num_users++;  
        p = p->next;  
    }  
  
    char **usernames = malloc(num_users * sizeof(char *) + 1);  
    int i = 0;  
    while (curr != NULL) {  
        usernames[i] = strdup(curr->name);  
        // printf("\t%s\r\n", usernames[i]);  
        i++;  
        curr = curr->next;  
    }  

    usernames[num_users] = NULL;
  
    return usernames;  
}  

/*
 * Make two users friends with each other.  This is symmetric - a pointer to
 * each user must be stored in the 'friends' array of the other.
 *
 * New friends must be added in the first empty spot in the 'friends' array.
 *
 * Return:
 *   - 0 on success.
 *   - 1 if the two users are already friends.
 *   - 2 if the users are not already friends, but at least one already has
 *     MAX_FRIENDS friends.
 *   - 3 if the same user is passed in twice.
 *   - 4 if at least one user does not exist.
 *
 * Do not modify either user if the result is a failure.
 * NOTE: If multiple errors apply, return the *largest* error code that applies.
 */
int make_friends(const char *name1, const char *name2, User *head) {
    User *user1 = find_user(name1, head);
    User *user2 = find_user(name2, head);

    if (user1 == NULL || user2 == NULL) {
        return 4;
    } else if (user1 == user2) { // Same user
        return 3;
    }

    int i, j;
    for (i = 0; i < MAX_FRIENDS; i++) {
        if (user1->friends[i] == NULL) { // Empty spot
            break;
        } else if (user1->friends[i] == user2) { // Already friends.
            return 1;
        }
    }

    for (j = 0; j < MAX_FRIENDS; j++) {
        if (user2->friends[j] == NULL) { // Empty spot
            break;
        }
    }

    if (i == MAX_FRIENDS || j == MAX_FRIENDS) { // Too many friends.
        return 2;
    }

    user1->friends[i] = user2;
    user2->friends[j] = user1;
    return 0;
}


/*
 *  Print a post.
 *  Use localtime to print the time and date.
 *  Return:
 *    - A dynamically allocated string containing the formatted post.
 *    - NULL if the post is NULL.
 */
char* print_post(const Post *post) {
    if (post == NULL) {
        return NULL;
    }

    // Allocate memory for the output string
    size_t total_length = strlen(post->author) + strlen(post->contents) + 50; // 50 is for date string
    char *output = (char*) malloc(total_length * sizeof(char));
    if (output == NULL) {
        return NULL; // Unable to allocate memory
    }

    // Construct the output string
    sprintf(output, "From: %s\nDate: %s%s\n", post->author, asctime(localtime(post->date)), post->contents);

    return output;
}

/*
 * Return a dynamically allocated string containing the user profile.
 * The returned string must be freed by the caller when it is no longer needed.
 * If the user is NULL, return NULL.
 */
char *print_user(const User *user) {
    if (user == NULL) {
        return NULL;
    }

    // Allocate memory for the profile string
    char *profile = malloc(1024 * sizeof(char));
    int profile_len = 0;

    // Print name
    profile_len += sprintf(profile + profile_len, "Name: %s\n\n", user->name);
    profile_len += sprintf(profile + profile_len, "------------------------------------------\n");

    // Print friend list.
    profile_len += sprintf(profile + profile_len, "Friends:\n");
    for (int i = 0; i < MAX_FRIENDS && user->friends[i] != NULL; i++) {
        profile_len += sprintf(profile + profile_len, "%s\n", user->friends[i]->name);
    }
    profile_len += sprintf(profile + profile_len, "------------------------------------------\n");

    // Print post list.
    profile_len += sprintf(profile + profile_len, "Posts:\n");
    const Post *curr = user->first_post;
    while (curr != NULL) {
        char *post_str = print_post(curr);
        if (post_str != NULL) {
            profile_len += sprintf(profile + profile_len, "%s", post_str);
            free(post_str);
        }
        curr = curr->next;
        if (curr != NULL) {
            profile_len += sprintf(profile + profile_len, "\n===\n\n");
        }
    }
    profile_len += sprintf(profile + profile_len, "------------------------------------------\n");

    return profile;
}

/*
 * Make a new post from 'author' to the 'target' user,
 * containing the given contents, IF the users are friends.
 *
 * Insert the new post at the *front* of the user's list of posts.
 *
 * Use the 'time' function to store the current time.
 *
 * 'contents' is a pointer to heap-allocated memory - you do not need
 * to allocate more memory to store the contents of the post.
 *
 * Return:
 *   - 0 on success
 *   - 1 if users exist but are not friends
 *   - 2 if either User pointer is NULL
 */
int make_post(const User *author, User *target, char *contents) {
    if (target == NULL || author == NULL) {
        return 2;
    }

    int friends = 0;
    for (int i = 0; i < MAX_FRIENDS && target->friends[i] != NULL; i++) {
        if (strcmp(target->friends[i]->name, author->name) == 0) {
            friends = 1;
            break;
        }
    }

    if (friends == 0) {
        return 1;
    }

    // Create post
    Post *new_post = malloc(sizeof(Post));
    if (new_post == NULL) {
        perror("malloc");
        exit(1);
    }
    strncpy(new_post->author, author->name, MAX_NAME);
    new_post->contents = contents;
    new_post->date = malloc(sizeof(time_t));
    if (new_post->date == NULL) {
        perror("malloc");
        exit(1);
    }
    time(new_post->date);
    new_post->next = target->first_post;
    target->first_post = new_post;

    return 0;
}

