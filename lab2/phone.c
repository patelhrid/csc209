#include <stdio.h>
#include <stdlib.h>

int main() {
	char phone_number[11];
	scanf("%s", phone_number);
	int digit;
	scanf("%d", &digit);
	if (digit <  -1 || digit > 9) {
		printf("ERROR\n");
	} else if (digit  == -1) {
		printf("%s\n", phone_number);
	} else {
		printf("%c\n", phone_number[digit]);
	}
	return 0;
}
