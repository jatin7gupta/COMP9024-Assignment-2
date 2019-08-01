#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Graph.h"
#include "Quack.h"

int len(char *word) {
	return strlen(word);
}

bool differByOne(char *firstWord, char *secondWord) {
	int firstWordLength = len(firstWord);
	int secondWordLength = len(secondWord);
	int changeCount = 0;
	if (firstWordLength == secondWordLength) {
		for (int f = 0, s = 0; f < firstWordLength && s < secondWordLength; f++, s++) {
			if (firstWord[f] != secondWord[f]) {
				changeCount++;
			}
		}
		if (changeCount == 1) {
			return true;
		}
	} else if (abs(firstWordLength - secondWordLength) == 1) {
		int f = 0;
		int s = 0;
		while (f < firstWordLength && s < secondWordLength) {
			if (firstWord[f] != secondWord[s]) {
				if (firstWordLength > secondWordLength) {
					f++;
				} else {
					s++;
				}
				changeCount++;
			} else {
				f++;
				s++;
			}
		}
		if (changeCount == 1) {
			return true;
		}
	}
	return false;
	
}

int takeInput(char dict[1000][21]) {
	int wordCounter = 0;
	char word[21];
	while (fscanf(stdin, "%s", word) == 1) {
		printf("%s   ", word);
		strcpy(dict[wordCounter++], word);
	}
	return wordCounter;
}

int main(void) {
    char dict[1000][21]; // TODO dynamic allocation
	takeInput(dict);
	return EXIT_SUCCESS;
}
