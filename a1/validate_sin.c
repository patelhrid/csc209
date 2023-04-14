#include <stdio.h>
#include <stdlib.h>
#include "sin_helpers.c"


// int populate_array(int, int *);  
// int check_sin(int *);  

int main(int argc, char **argv) {
    // TODO: Verify that command line arguments are valid.
    if (argc != 2) {
		fprintf(stderr, "USAGE: validate_sin sin\n");
		return 2;
	}
    // TODO: Parse arguments and then call the two helpers in sin_helpers.c
    // to verify the SIN given as a command line argument.
	int *sin_array = malloc(sizeof(int) * 10);
//	printf("%d\n", atoi(argv[1]));
	populate_array(strtol(argv[1], NULL, 10), sin_array);
//	printf("%d\n", sin_array);
//	printf("Expected return value: %d\n", check_sin(&sin_array));
	int result = check_sin(sin_array);
	printf("%d\n", result);
	free(sin_array);
	return result;
//	free(sin_array);
//	return 0;
}
