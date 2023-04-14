#include <stdio.h>
#include <stdlib.h>


// TODO: Implement a helper named check_permissions that matches the prototype below.
int check_permissions(char *p, char *s){
	if (*p == 'd') {return 0;}
//	printf("%s\n", p);
//        printf("%s\n", s);
	if (1 == 0) {return 0;} else {
		p = &p[1];
		for (int i = 0; i < 9; i++){
//			printf("P: %c, S: %c\n", p[i], s[i]);
			if (p[i] != s[i] && p[i] == '-' && s[i] != 0){
//				printf("\n");
				return 0;
			}
			if (s[i] == 0){break;}
		}
//	printf("\n");
	return 1;
	}
}


int main(int argc, char** argv) {
    if (!(argc == 2 || argc == 3)) {
        fprintf(stderr, "USAGE: count_large size [permissions]\n");
        return 1;
    }

    // TODO: Process command line arguments.
    int amount = strtol(argv[1], NULL, 10);
    int count = 0;
    int s;
    char p[11];
    scanf("%*s %*d");
    while (scanf("%s %*d %*s %*s %d %*s %*d %*s %*s", p, &s) != EOF){
//	printf("LINE: %s %d\n", p, s);
	if (s >= amount && argc == 2) {
		count++;
	}

    // TODO: Call check_permissions as part of your solution to count the files to
    // compute and print the correct value.
	else if (s >= amount && argc == 3){
		count = count + check_permissions(p, argv[2]);
	}
    }
    printf("%d\n", count);
    return 0;
}
