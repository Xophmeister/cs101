/**
  @file       directedGraph.h
  @brief      Directed graph header file
  @author     Christopher Harrison (Xophmeister)
  @copyright  MIT License

  Implements a directed graph (i.e., nodes with an arbitrary number of
  links), where each node can additionally point to arbitrary data in
  memory.

  This is an abstraction used to build more specific structures.
*/

#ifndef DIRECTEDGRAPH_H
#define DIRECTEDGRAPH_H

#include <stdlib.h>
#include <stdarg.h>

#include "../containers/dynamicArray.h"

/**
  @struct     dgNode
  @brief      Directed graph node
  @var        dgNode::payload
              Pointer to the current node's contents
  @var        dgNode::connections
              Dynamic array of pointers to connected nodes
*/
typedef struct dgNode {
  void*     payload;
  dynArray* connections;
} dgNode;

/**
  @enum       cyclicity
  @brief      The cyclicity of a structure
  @var        cyclicity::acyclic
              Acyclic
  @var        cyclicity::cyclic
              Cyclic (i.e., contains cycles)
*/
typedef enum {
  acyclic,
  cyclic
} cyclicity;

#endif
