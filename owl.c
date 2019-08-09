/*
Name: Jatin Gupta
zID / email: z5240221@ad.unsw.edu.au
Date last modified: 09/08/2019
Name of course and session: COMP9024 19T2
Task: OWL
Link to spec: https://webcms3.cse.unsw.edu.au/COMP9024/19T2/resources/28573 
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Graph.h"
#include "Quack.h"


#define INITIAL_LENGTH 8
#define WORD_LENGTH 21
#define MAXIMUM_PATHS 1000

/*
Input: array of characters
Return Value: int
Usage: Takes a array of word, returns the length of the array 
*/
int len(char *word) {
	return strlen(word);
}


/*
Input: character pointer array and integer count of words
Return Value: 2d character pointer array which has been freed and now is NULL
Usage: Free up the malloc-ed space
*/
char **freeDict(char **dict, int wordCount) {
	for (int i = 0; i < wordCount; i++) {
		free(dict[i]);
		dict[i] = NULL;
	}
	free(dict);
	dict = NULL;
	return dict;
}


/*
Input: integer, to give the malloc size
Return Value: character pointer array
Usage: To allocate memory to the dictionary
*/
char **performMalloc(int length) {
	char **dict = malloc(length * sizeof(char *));
	if (dict == NULL) {
		fprintf(stderr, "Ran out of memory, Quiting");
		exit(EXIT_FAILURE);
	}
	return dict;
}


/*
Input: integer, to give the malloc size
Return Value: array of character of the given size
Usage: to give memory to a word as array of characters
*/
char *mallocWord(int length) {
	char *block = malloc(length * sizeof(char));
	if (block == NULL) {
		fprintf(stderr, "Ran out of memory, Quiting");
		exit(EXIT_FAILURE);
	}
	return block;
}


/*
Input: array of char pointers and integer, to give the realloc size
Return Value: array of character pointers after giving them new memory of the given size
Usage: to give memory to each word and increase by twice when they fill up
*/
char **performRealloc(char **dict, int length) {
	dict = realloc(dict, length * 2 * sizeof(char *));
	if (dict == NULL) {
		fprintf(stderr, "Ran out of memory, Quiting");
		exit(EXIT_FAILURE);
	}
	return dict;
}


/*
Input: integer, to give the malloc size
Return Value: array of integers after giving them memory of the given size and assigning each value to -1
Usage: to make an array and assign them a constant value to all the elements of the array
*/
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


/*
Input: two array of characters
Return Value: true if they differ by one, else false. Differ by one is explained in the assignment specs
Usage: to check if two strings (character arrays) are differing by one or not
*/
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

/*
Input: pointer to the array of character pointers and integer, initial malloced size 
Return Value: integer, the number of words which has been read by the function 
Usage: to read input from stdin and store them into an array of character pointers and returning the caller the number of words read
*/
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


/*
Input: array of pointer to the character array and integer, count of the number of words
Return Value: Graph which has the vertices and edges
Usage: to create the graph for the read words
*/
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


/*
Input: array of pointer to the character array and integer, count of the number of words and the Graph 
Return Value: void
Usage: to print the value of the of all the words in the dict and call the showGraph from the Graph ADT
*/
void printGraph(char **dict, int wordCount, Graph graph) {
	printf("Dictionary\n");
	for (int i = 0; i < wordCount; i++) {
		printf("%d: %s\n", i, dict[i]);
	}
	printf("Ordered Word Ladder Graph\n");
	showGraph(graph);
}


/*
Input: array of pointer to the character array and integer, count of the number of words and the Graph 
Return Value: void
Usage: to print the value of the of all the words in the dict and call the showGraph from the Graph ADT
*/
bool childExist(int v, Graph g, int numV) {
	for (int w = v+1; w < numV; w++) {
		if (isEdge(newEdge(v, w), g)) {
			return true;
		}
	}
	return false;
}


/*
Input: Graph, vertix to start with (integer), number of vertixes (integer), a counter which counts the depth (integer), previous maximum seen value, length of path (pointer to the integer), 
		visited array to store the temp path (array of int), cursor to backtrack (pointer to the integer), array of quacks to store the paths (array of quacks), 
		number of paths (pointer to the integer)   
Return Value: void
Usage: to recurse till the node when we do not have any paths to go further and check of you have seen a bigger path or same length path, if yes save that path into a quack
*/
void dfsR(Graph g, Vertex v, int numV, int counter, int *maxSeen, int *visited, int *cursor, Quack *quackArray, int *path) {
	visited[*cursor] = v;
    for (Vertex w = v+1; w < numV; w++) {
       	if (isEdge(newEdge(v, w), g)) {
       		*cursor = *cursor + 1;
          	dfsR(g, w, numV, counter+1, maxSeen, visited, cursor, quackArray, path);
          	*cursor = *cursor - 1;
          	if (!childExist(w, g, numV)) {
		   		if (*maxSeen < counter+1) {
					*maxSeen = counter+1;
					
					*path = 0;
					makeEmptyQuack(quackArray[*path]);
					quackArray[*path] = createQuack();
					for (int i = 0; i < numV; i++) {
						qush(visited[i], quackArray[*path]);
					}
				} else if (counter+1 == *maxSeen) {
					
					*path = *path + 1;
					makeEmptyQuack(quackArray[*path]);
					quackArray[*path] = createQuack();
					for (int i = 0; i < numV; i++) {
						qush(visited[i], quackArray[*path]);
					}
				}
       		}
       	} 
    }
    return;
}

/*
Input: Graph, vertix to start with (integer), number of vertixes (integer), array of quacks to store the paths (array of quacks), number of paths (pointer to the integer), previous 			maximum seen value of length of path (pointer to the integer)   
Return Value: maximum seen value of length of path
Usage: to call the recursive function and get the values set/destroy before/after calling it
*/
int dfs(Graph g, Vertex rootv, int numV, Quack *quackArray, int *path, int *maxSeen) {
    int counter = 0;
    int *visited = mallocArray(numV); 
    int cursor = 0;
    Vertex startv = rootv;                    
    dfsR(g, startv, numV, counter+1, maxSeen, visited, &cursor, quackArray, path);
    
    free(visited);
   return *maxSeen;
}


/*
Input: Pointer to the quack array   
Return Value: void
Usage: to give memory to all the quacks at once
*/
void createQuackArray(Quack *quackArray) {
	for (int i = 0; i < MAXIMUM_PATHS; i++) {
		quackArray[i] = createQuack();
	}
}


/*
Input: Pointer to the quack array   
Return Value: void
Usage: to free memory to all the quacks at once
*/
void destroyQuackArray(Quack *quackArray) {
	for (int i = 0; i < MAXIMUM_PATHS; i++) {
		quackArray[i] = destroyQuack(quackArray[i]);
	}
	quackArray = NULL;
}

/*
Input: maximum seen value of length of path (integer), dictionary of words stored in array of character pointers, array of quacks, and number of paths (integer)
Return Value: void
Usage: print the longest paths in the graph with dictionary 
*/
void printArray(int maxSeen, char **dict, Quack *quackArray, int path) {
	printf("Longest ladder length: %d\nLongest ladders:\n", maxSeen);
	for (int i = 0; i <= path && i<99; i++) {
		printf("%2d: ", i+1);
		int j;
		for (j = 0; j < maxSeen-1; j++) {
			printf("%s -> ", dict[pop(quackArray[i])]);
		}
		printf("%s\n", dict[pop(quackArray[i])]);
	}
}


int main(void) {
    char **dict = NULL;
    dict = performMalloc(INITIAL_LENGTH);
    
	int wordCount = takeInput(&dict, INITIAL_LENGTH);
	
    Graph graph = createGraph(dict, wordCount);
    Quack quackArray[MAXIMUM_PATHS];
    createQuackArray(quackArray);
    
    printGraph(dict, wordCount, graph);

    int path = 0;
    int maxSeen = -1;
    if (wordCount > 0) {
    	for (int i = 0; i < wordCount; i++) {
    		int newMaxSeen = dfs(graph, i, wordCount, quackArray, &path, &maxSeen);
    		if (newMaxSeen > maxSeen) {
    			maxSeen = newMaxSeen;
    		}
    	}
    	if (maxSeen == -1) {
    		printf("Longest ladder length: 1\nLongest ladders:\n");
    		for (int i = 0; i < wordCount && i < 99; i++) {
    			printf("%2d: %s\n",i+1, dict[i]);
    		}
    	} else {
    		printArray( maxSeen, dict, quackArray, path);
    	}
    }
    dict = freeDict(dict, wordCount);
    destroyQuackArray(quackArray);
	return EXIT_SUCCESS;
}
