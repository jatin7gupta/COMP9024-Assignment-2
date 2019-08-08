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


char **freeDict(char **dict, int wordCount) {
	for (int i = 0; i < wordCount; i++) {
		free(dict[i]);
		dict[i] = NULL;
	}
	free(dict);
	dict = NULL;
	return dict;
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


int *mallocArray(int length) {
	int *block = malloc(length * sizeof(int));
	if (block == NULL) {
		fprintf(stderr, "Ran out of memory, Quiting");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < length; i++) {
		block[i] = -1;
	}
	return block;
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


int takeInput(char ***dict, int initialLength) {
	int wordCounter = 0;
	char word[WORD_LENGTH];
	while (fscanf(stdin, "%s", word) == 1) {
		int inputLength = strlen(word);
		if (wordCounter > 0) {
			if (strcmp((*dict)[wordCounter-1], word) != 0) {
				(*dict)[wordCounter] = mallocWord(inputLength+1);
				strcpy((*dict)[wordCounter++], word);
			}
		} else {
			(*dict)[wordCounter] = mallocWord(inputLength+1);
			strcpy((*dict)[wordCounter++], word);			
		}
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

void printGraph(char **dict, int wordCount, Graph graph) {
	printf("Dictionary\n");
	for (int i = 0; i < wordCount; i++) {
		printf("%d: %s\n", i, dict[i]);
	}
	printf("Ordered Word Ladder Graph\n");
	showGraph(graph);
}


void printArray(char *string, int *array, int n, char **dict) {
	printf("%s", string);
	for (int i = 0; i < n; i++) {
		if (array[i] != -1) {
			printf("%s -> ", dict[i]);
		}
	}
	printf("\n");
}


bool childExist(int v, Graph g, int numV) {
	for (int w = v+1; w < numV; w++) {
		if (isEdge(newEdge(v, w), g)) {
			return true;
		}
	}
	return false;
}

void dfsR(Graph g, Vertex v, int numV, int counter, int *maxSeen) {
    for (Vertex w = v+1; w < numV; w++) {
       	if (isEdge(newEdge(v, w), g)) {
          	dfsR(g, w, numV, counter+1, maxSeen);
          	if (!childExist(w, g, numV)) {
		   		if (*maxSeen < counter+1) {
					*maxSeen = counter+1;
				}
       		}
       	} 
    }
    return;
}


int dfs(Graph g, Vertex rootv, int numV) {
    int maxSeen = 0;
    int counter = 0;
    Vertex startv = rootv;                      
    dfsR(g, startv, numV, counter+1, &maxSeen);
    printf("maxSeen = %d\n", maxSeen);
    // TODO: support disconnected graphs
   return maxSeen;
}


int main(void) {
    char **dict = NULL;
    dict = performMalloc(INITIAL_LENGTH);
	int wordCount = takeInput(&dict, INITIAL_LENGTH);
    Graph graph = createGraph(dict, wordCount);
    //printGraph(dict, wordCount, graph);
    int maxSeen = -1;
    if (wordCount > 0) {
    	maxSeen = dfs(graph, 0, wordCount);
		
		//printArray("Longest ladder length: _\nLongest ladders:\n", visited, wordCount, dict);
    }
    dict = freeDict(dict, wordCount);
	return EXIT_SUCCESS;
}

//TODO: number of paths
//TODO: use %2d and trailing \n
//TODO: free mallocs DONE
//TODO: handle duplicates before dict DONE
//TODO: handle empty dict DONE
