#include <stdio.h>
#include <string.h>

/*
    Write the main() function of a program that takes exactly two arguments,
    and prints one of the following:
        - "Same\n" if the arguments are the same.
        - "Different\n" if the arguments are different.
        - "Invalid\n" if the program is called with an incorrect number of
          arguments.

    NOTE: Cut and paste these strings into your printf statements to
    ensure that they will be exactly the strings above.

    Your main function should return 0, regardless of what's printed.
*/
int main(int argc, char** argv){
	if (argc != 3) {
		printf("Invalid\n");
		return 0;
	}
	for (int i = 0; i < strlen(argv[1]); i++){
		if (argv[1][i] != argv[2][i]){
			printf("Different\n");
			return 0;
		}
	}
	printf("Same\n");
//	printf("%s and %s\n", argv[1], argv[2]);
	return 0;
}
