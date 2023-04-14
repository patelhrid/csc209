#include <stdio.h>
#include <stdlib.h>

/* Return a pointer to an array of two dynamically allocated arrays of ints.
   The first array contains the elements of the input array s that are
   at even indices.  The second array contains the elements of the input
   array s that are at odd indices.

   Do not allocate any more memory than necessary. You are not permitted
   to include math.h.  You can do the math with modulo arithmetic and integer
   division.
*/
int **split_array(const int *s, int length) {
	int size1;
	int size2;
	if (length % 2 == 1) {
		size1 = length + 1 / 2;
		size2 = length - 1 / 2;
	}
	else {
		size1 = length / 2;
		size2 = length / 2;
	}
	int **arrays = malloc(sizeof(int*)*2);
	arrays[0] = malloc(sizeof(int) * size1);
	arrays[1] = malloc(sizeof(int) * size2);
	int a = 0;
	int b = 0;
	for (int i = 0; i < length; i++){
		if (i % 2 == 0) {
			arrays[0][a] = s[i];
			a++;
		} else {
			arrays[1][b] = s[i];
			b++;
		}

	}
	return arrays;

}

/* Return a pointer to an array of ints with size elements.
   - strs is an array of strings where each element is the string
     representation of an integer.
   - size is the size of the array
 */

int *build_array(char **strs, int size) {
	int *array = malloc(sizeof(int)*size);
	for (int i = 1; i < size; i++) {
		array[i-1] = strtol(strs[i], NULL, 10);
	}
	return array;
}


int main(int argc, char **argv) {
    /* Replace the comments in the next two lines with the appropriate
       arguments.  Do not add any additional lines of code to the main
       function or make other changes.
     */
    int *full_array = build_array(argv, argc);
    int **result = split_array(full_array, sizeof(full_array));

    printf("Original array:\n");
    for (int i = 0; i < argc - 1; i++) {
        printf("%d ", full_array[i]);
    }
    printf("\n");

    printf("result[0]:\n");
    for (int i = 0; i < argc / 2; i++) {
        printf("%d ", result[0][i]);
    }
    printf("\n");

    printf("result[1]:\n");
    for (int i = 0; i < (argc - 1) / 2; i++) {
        printf("%d ", result[1][i]);
    }
    printf("\n");
    free(full_array);
    free(result[0]);
    free(result[1]);
    free(result);
    return 0;
}
