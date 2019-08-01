// readGraph.c  read a graph from stdin and print it
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Graph.h"

#define WHITESPACE 100

int readNumV(void) { // returns the number of vertices numV or -1
   int numV;
   char w[WHITESPACE];
   scanf("%[ \t\n]s", w);  // skip leading whitespace
   if ((getchar() != '#') ||
       (scanf("%d", &numV) != 1)) {
       fprintf(stderr, "missing number (of vertices)\n");
       return -1;
   }
   return numV;
}

int readGraph(int numV, Graph g) { // reads number-number pairs until EOF
   int success = true;             // returns true if no error
   int v1, v2;
   while (scanf("%d %d", &v1, &v2) != EOF && success) {
       if (v1 < 0 || v1 >= numV || v2 < 0 || v2 >= numV) {
          fprintf(stderr, "unable to read edge\n");
          success = false;
       }
       else {
          insertEdge(newEdge(v1, v2), g);
       }
   }
   return success;
}

int main (void) { 
    int numV;
    if ((numV = readNumV()) >= 0) {
        Graph g = newGraph(numV);
        if (readGraph(numV, g)) {
            showGraph(g);
        }
        g = freeGraph(g);
    }
    else {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
