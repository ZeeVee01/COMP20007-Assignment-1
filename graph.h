/*
graph.h

Visible structs and functions for graph construction and manipulation.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2021
*/

/* Definition of a graph. */
struct graph;

enum problemPart;

struct solution;

struct primNode;

/* A particular solution to a graph problem. */
#ifndef SOLUTION_STRUCT
#define SOLUTION_STRUCT
struct solution {
  int antennaTotal;
  int cableTotal;
  int mixedTotal;
};
#endif

/* Which part the program should find a solution for. */
#ifndef PART_ENUM
#define PART_ENUM
enum problemPart {
  PART_A=0,
  PART_C=1
};
#endif

/* Creates an undirected graph with the given numVertices and no edges and
returns a pointer to it. NumEdges is the number of expected edges. */
struct graph *newGraph(int numVertices);

/* Adds an edge to the given graph. */
void addEdge(struct graph *g, int start, int end, int cost);

/* Find the total radio-based cost, total cabled cost if the part is PART_A, and
  the mixed total cost if the part is PART_C. */
struct solution *graphSolve(struct graph *g, enum problemPart part,
  int antennaCost, int numHouses);

/* Frees all memory used by graph. */
void freeGraph(struct graph *g);

/* Frees all data used by solution. */
void freeSolution(struct solution *solution);

/* Prim minimum search tree algorithm */
void primMST(struct graph *g);

/* Locks edges that are present in MST */
void edgeLock(struct graph *g, struct primNode *nodeArr);

/* Sums locked edges */
int treeCost(struct graph *g);

int mixedCost(struct graph *g, int antennaCost);