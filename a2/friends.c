#include "friends.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Create a new user with the given name.  Insert it at the tail of the list
 * of users whose head is pointed to by *user_ptr_add.
 *
 * Return:
 *   - 0 if successful
 *   - 1 if a user by this name already exists in this list
 *   - 2 if the given name cannot fit in the 'name' array
 *       (don't forget about the null terminator)
 */
int create_user(const char *name, User **user_ptr_add) {
    struct user* curr = *user_ptr_add;
    while (curr != NULL) {
        if (strcmp(curr->name, name) == 0) {
            return 1;
        }
        curr = curr->next;
    }

    struct user* new_user = (struct user*)malloc(sizeof(struct user));
    strcpy(new_user->name, name);
    new_user->next = NULL;

    if (*user_ptr_add == NULL) {
        *user_ptr_add = new_user;
    } else {
        curr = *user_ptr_add;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = new_user;
    }

    return 0;
}


/*
 * Return a pointer to the user with this name in
 * the list starting with head. Return NULL if no such user exists.
 *
 * NOTE: You'll likely need to cast a (const User *) to a (User *)
 * to satisfy the prototype without warnings.
 */
User *find_user(const char *name, const User *head) {
    const User *curr = head;

    while (curr != NULL) {
        if (strcmp(curr->name, name) == 0) {
            return (User *) curr;
        }
        curr = curr->next;
    }

    return NULL;
}


/*
 * Print the usernames of all users in the list starting at curr.
 * Names should be printed to standard output, one per line.
 */
void list_users(const User *curr) {
    if (curr != NULL){
        while (curr != NULL){
            printf("%s\n", (*curr).name);
            curr = (*curr).next;
        }
    }
}


/*
 * Change the filename for the profile pic of the given user.
 *
 * Return:
 *   - 0 on success.
 *   - 1 if the file does not exist or cannot be opened.
 *   - 2 if the filename is too long.
 */
int update_pic(User *user, const char *filename) {
    if (strlen(filename) > MAX_NAME){return 2;}
    else if (fopen(filename, "r") == NULL){return 1;};
    strcpy(user->profile_pic, filename);
    return 0;
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
    if (name1 == name2){return 3;}
    else if(find_user(name1, head) == NULL || find_user(name2, head) == NULL){return 4;}
    else{
        User *user1 = find_user(name1, head);
        User *user2 = find_user(name2, head);
        int a = 0;
        int b = 0;
        for (int i = 0; i < MAX_FRIENDS; i++){
            if ((*user1).friends[i] == user2){return 1;}
            if ((*user2).friends[i] == user1){return 1;}
            if ((*user1).friends[i] == NULL && a == 0){
                (*user1).friends[i] = user2;
                a = 1;
                }
            if ((*user2).friends[i] == NULL && b == 0){(
                *user2).friends[i] = user1;
                b = 1;
                }
        }
        if (a == 0 && b == 0){
            return 2;
        }
        else {return 0;}
    }
}


/*
 * Print a user profile.
 * For an example of the required output format, see the example output
 * linked from the handout.
 * Return:
 *   - 0 on success.
 *   - 1 if the user is NULL.
 */
int print_user(const User *user) {
    if (user == NULL){return 1;}
    FILE *file = fopen(user->profile_pic, "r");
    int chr;
    if (file) {
        while ((chr = fgetc(file)) != EOF) {
            putchar(chr);
        }
        fclose(file);
        printf("\n");
    }
    printf("Name: %s\n", user->name);
    printf("------------------------------------------\n");
    printf("Friends:\n");
    for (int i = 0; i < MAX_FRIENDS; i++){
        if (user->friends[i] != NULL) {
            printf("%s\n", user->friends[i]->name);
        }
    }
    printf("------------------------------------------\n");
    printf("Posts:\n");
    Post *curr_post = user->first_post;
    int i = 0;
    while (curr_post != NULL) {
        if (i != 0){
            printf("\n\n===\n\n");
        }
        printf("From: %s\n", curr_post->author);
        if (curr_post->date != NULL) {
            printf("Date: %s\n", ctime(curr_post->date));
        }
        printf("%s\n", curr_post->contents);
        curr_post = curr_post->next;
        i++;
    }
    printf("------------------------------------------\n");
    return 0;
}


/*
 * Make a new post from 'author' to the 'target' user,
 * containing the given contents, IF the users are friends.
 *
 * Insert the new post at the *front* of the user's list of posts.
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
    if (author == NULL || target == NULL){return 2;}
    int friends = 0;
    for (int i = 0; i < MAX_FRIENDS; i++){
        if ((*target).friends[i] == author){friends = 1;}
    }
    if (friends == 0){ return 1;}
    else{
        Post *new_post = (Post *)malloc(sizeof(Post));
        strcpy(new_post->author, author->name);
        new_post->contents = contents;
        new_post->date = (time_t *)malloc(sizeof(time_t));
        if (new_post->date == NULL) {
            free(new_post);
            return 2;
        }
        time(new_post->date);
        new_post->next = target->first_post;
        target->first_post = new_post;

        return 0;
    }
}


/*
 * From the list pointed to by *user_ptr_del, delete the user
 * with the given name.
 * Remove the deleted user from any lists of friends.
 *
 * Return:
 *   - 0 on success.
 *   - 1 if a user with this name does not exist.
 */
int delete_user(const char *name, User **user_ptr_del) {
    User *prev = NULL;
    User *curr = *user_ptr_del;
    while (curr != NULL && strcmp(curr->name, name) != 0) {
        prev = curr;
        curr = curr->next;
    }
    if (curr == NULL) {
        return 1;
    }
    User *curr2 = *user_ptr_del;
    while (curr2 != NULL) {
        for (int i = 0; i < MAX_FRIENDS; i++) {
            if (curr2->friends[i] != NULL && strcmp(curr2->friends[i]->name, name) == 0) {
                curr2->friends[i] = NULL;
            }
        }
        curr2 = curr2->next;
    }
    if (prev == NULL) {
        *user_ptr_del = curr->next;
    } else {
        prev->next = curr->next;
    }
    return 0;
}
