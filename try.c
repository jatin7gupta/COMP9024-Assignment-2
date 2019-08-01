#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(void) {
	char word1[21];
	char word2[21];
	while (fscanf(stdin, "%s", word1) == 1) {
		printf("%s   ", word1);
	}
	return 0;
}
