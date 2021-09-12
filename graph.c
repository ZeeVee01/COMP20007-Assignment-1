/*
graph.c

Set of vertices and edges implementation.

Implementations for helper functions for graph construction and manipulation.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2021
*/
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "graph.h"
#include "utils.h"
#include "pq.h"

#define INITIALEDGES 32
#define LOCKED 1
#define UNLOCKED 0

struct edge;

/* Definition of a graph. */
struct graph {
  int numVertices;
  int numEdges;
  int allocedEdges;
  struct edge **edgeList;
};

/* Definition of an edge. */
struct edge {
  int start;
  int end;
  int cost;
  int locked;
};

struct graph *newGraph(int numVertices){
  struct graph *g = (struct graph *) malloc(sizeof(struct graph));
  assert(g);
  /* Initialise edges. */
  g->numVertices = numVertices;
  g->numEdges = 0;
  g->allocedEdges = 0;
  g->edgeList = NULL;
  return g;
}

/* Adds an edge to the given graph. */
void addEdge(struct graph *g, int start, int end, int cost){
  assert(g);
  struct edge *newEdge = NULL;
  /* Check we have enough space for the new edge. */
  if((g->numEdges + 1) > g->allocedEdges){
    if(g->allocedEdges == 0){
      g->allocedEdges = INITIALEDGES;
    } else {
      (g->allocedEdges) *= 2;
    }
    g->edgeList = (struct edge **) realloc(g->edgeList,
      sizeof(struct edge *) * g->allocedEdges);
    assert(g->edgeList);
  }

  /* Create the edge */
  newEdge = (struct edge *) malloc(sizeof(struct edge));
  assert(newEdge);
  newEdge->start = start;
  newEdge->end = end;
  newEdge->cost = cost;
  newEdge->locked = UNLOCKED;

  /* Add the edge to the list of edges. */
  g->edgeList[g->numEdges] = newEdge;
  (g->numEdges)++;
}

/* Frees all memory used by graph. */
void freeGraph(struct graph *g){
  int i;
  for(i = 0; i < g->numEdges; i++){
    free((g->edgeList)[i]);
  }
  if(g->edgeList){
    free(g->edgeList);
  }
  free(g);
}

struct solution *graphSolve(struct graph *g, enum problemPart part,
  int antennaCost, int numHouses){
  struct solution *solution = (struct solution *)
    malloc(sizeof(struct solution));
  assert(solution);
  if(part == PART_A){
    
    solution->antennaTotal = numHouses * antennaCost; // For full radio installation = antenna cost * numHouses;

    primMST(g); // Run primMST to get min tree and lock needed connections

    solution->cableTotal = treeCost(g); // Gets MST cost
    
  
  } else {
    
    primMST(g); // Run primMST to get min tree and lock needed connections

    solution->mixedTotal = mixedCost(g, antennaCost);
  }
  return solution;
}

void primMST(struct graph *g) {

  int i;
  int j;
  int connections; // Amount of connections with tree thus far
  struct pq *pq;
  int nodeNBR; // node Neighbour
  int nodeCUR; // node Current
  struct edge *curEdge;

  // Reassigns values for easier use
  int numVertices = g->numVertices;
  int numEdges = g->numEdges;

  int *lockedNodes = malloc(numVertices * sizeof(int));
  for (i=0; i<numVertices; i++) {
    lockedNodes[i] = UNLOCKED;
  }
  lockedNodes[0] = LOCKED;
  
  // Initalise values for primNode arr
  for (i=0; i<numEdges; i++){
    g->edgeList[i]->locked = UNLOCKED;
  }

  for (i=1; i<numVertices; i++) {
    
    // Queue all non locked edges in pq
    pq = newPQ();

    for (j=0; j<numEdges; j++) {
      if (g->edgeList[j]->locked == UNLOCKED) {
        enqueue(pq, g->edgeList[j], g->edgeList[j]->cost);
      }
    }


    
    // Deletes cheapest connection from pq and assigns it to curEdge
    curEdge = deletemin(pq);
    while (curEdge != NULL) {
      nodeCUR = curEdge->start; // Current Node
      nodeNBR = curEdge->end;   // Current Neighbour


      connections = 0; // Connections to tree thus far

      // Iterate through vertices and check for connections to lockedNodes
      for (j=0; j<numVertices; j++) {
        if ((lockedNodes[j] == LOCKED) && ((nodeCUR == j) || (nodeNBR == j))) {
          connections += 1;
        }
      }

      // If not connected, or connected on both sides to current tree, assign next cheapest and continue
      if (connections != 1) {
        curEdge = deletemin(pq);
        continue;
      }

      // Lock Nodes that are now connected, and lock edge
      curEdge->locked = LOCKED;
      lockedNodes[nodeCUR] = LOCKED;
      lockedNodes[nodeNBR] = LOCKED;
      curEdge = NULL; // Exit statement for loop

    }
    
    freePQ(pq); // Free pq so we can remake without locked nodes

  }

}

/* Sums locked edges */
int treeCost(struct graph *g) {
  int i;
  int cost = 0;
  

  for(i=0; i<g->numEdges; i++) {
    if (g->edgeList[i]->locked == LOCKED) {
      cost += g->edgeList[i]->cost;
    }
  }

  return cost;
}

/* Sums locked edges and antenna if cheaper */
int mixedCost(struct graph *g, int antennaCost) {
  int i;
  int cost = 0;
  

  for(i=0; i<g->numEdges; i++) {
    if (g->edgeList[i]->locked == LOCKED) {

      // If connection cost more then antenna, build antenna instead
      if (g->edgeList[i]->cost > antennaCost) {
        cost += antennaCost;
      }
      else {
        cost += g->edgeList[i]->cost;
      }
    }
  }

  return cost;
}
