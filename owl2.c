#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Graph.h"
#include "Quack.h"

#define INITIAL_LENGTH 8
#define WORD_LENGTH 21

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
		if (changeCount == 1 || f == s) {
			return true;
		}
	}
	return false;
	
}


char **performMalloc(int length) {
	char **dict = malloc(length * sizeof(char *));
	if (dict == NULL) {
		fprintf(stderr, "Ran out of memory, Quiting");
		exit(EXIT_FAILURE);
	}
	return dict;
}

char *mallocWord(int length) {
	char *block = malloc(length * sizeof(char));
	if (block == NULL) {
		fprintf(stderr, "Ran out of memory, Quiting");
		exit(EXIT_FAILURE);
	}
	return block;
}

char **performRealloc(char **dict, int length) {
	dict = realloc(dict, length * 2 * sizeof(char *));
	if (dict == NULL) {
		fprintf(stderr, "Ran out of memory, Quiting");
		exit(EXIT_FAILURE);
	}
	return dict;
}

int takeInput(char ***dict, int initialLength) {
	int wordCounter = 0;
	char word[WORD_LENGTH];
	while (fscanf(stdin, "%s", word) == 1) {
		int inputLength = strlen(word);
		(*dict)[wordCounter] = mallocWord(inputLength+1);
		strcpy((*dict)[wordCounter++], word);

		if (wordCounter == initialLength) {
			*dict = performRealloc(*dict, initialLength);	
			initialLength = initialLength * 2;
		}
	}
	return wordCounter;
}

Graph createGraph(char **dict, int wordCount){
    Graph g = newGraph(wordCount);
    for (int i = 0; i < wordCount; i++) {
        for (int j = 0; j < wordCount; j++) {
            if (differByOne(dict[i], dict[j])) {
                insertEdge(newEdge(i, j), g);
            }
        }
    }
    return g;
}


int main(void) {
    char **dict = NULL; // TODO dynamic allocation
    dict = performMalloc(INITIAL_LENGTH);
	int wordCount = takeInput(&dict, INITIAL_LENGTH);
    Graph graph = createGraph(dict, wordCount);
    showGraph(graph);
	return EXIT_SUCCESS;
}
