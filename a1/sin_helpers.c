// TODO: Implement populate_array
/*
 * Convert a 9 digit int to a 9 element int array.
 */
int populate_array(int sin, int *sin_array) {
	for (int i = 8; i >= 0; i--){
//		printf("%d\n", sin_array[i]);
		sin_array[i] = sin % 10;
        	sin /= 10;
	}
    return 0;
}

// TODO: Implement check_sin
/*
 * Return 0 if the given sin_array is a valid SIN, and 1 otherwise.
 */
int check_sin(int *sin_array) {
    	int mult[9] = {1, 2, 1, 2, 1, 2, 1, 2, 1};
	int result = 0;
	for (int i = 0; i < 9; i++){
//		printf("%d and %d\n", sin_array[i], mult[i]);
		int r = sin_array[i] * mult[i];
		if (r >= 10){
			result = result + 1 + (r % 10);
//			printf("r: %d, i: %d, result: %d IN IF\n", r, i, result); 
		} else {
			result = result + r;
//			printf("r: %d, i: %d, result: %d IN ELSE\n", r, i, result);
		}
	}
//	printf("FOR LOOP DONE\n");
	if (result % 10 == 0){return 0;}
	return 1;
}

