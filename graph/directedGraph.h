/**
  @file       directedGraph.h
  @brief      Directed graph header file
  @author     Christopher Harrison (Xophmeister)
  @copyright  @ref license

  Implements a directed graph (i.e., nodes with an arbitrary number of
  links), where each node can additionally point to arbitrary data in
  memory.

  This is an abstraction used to build more specific structures.
*/

#ifndef DIRECTEDGRAPH_H
#define DIRECTEDGRAPH_H

#include "../indexed/dynamicArray.h"

/**
  @struct     dgNode
  @brief      Directed graph node
  @var        dgNode::payload
              Pointer to the current node's contents
  @var        dgNode::links
              Dynamic array of pointers to connected nodes

  @note       Links are indexed in a dynamic array and referenced as
              such. While the indexing is designed to be arbitrary,
              using them semantically is probably a good idea; in which
              case, an `enum` may be appropriate to disambiguate their
              meanings.
*/
typedef struct dgNode {
  void*     payload;
  dynArray* links;
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

/**
  @fn         dgNode* dgCreateNode(void* payload, size_t links)
  @brief      Create a new directed graph node
  @param      payload  Pointer to the node's contents
  @param      links    Number of links to initialise
  @return     Pointer to the newly created node; or `NULL` in the event
              of an allocation failure

  Create a directed graph node with the given contents, initialised (but
  not set) with the specified number of links.
*/
extern dgNode* dgCreateNode(void*, size_t);

/**
  @fn         dgNode** dgLink(dgNode* node, size_t index, size_t depth)
  @brief      The link to the node a given depth from a starting node down a fixed index
  @param      node   The starting node
  @param      index  The link index to traverse
  @param      depth  The distance to traverse from the starting node
  @return     Pointer to the pointer to the resolved node; or `NULL` in
              the event of a routing failure

  Return the pointer of the link to the node that is a given depth from
  the starting node down a specific link index, this can then be
  dereferenced to point to the specific node link for, for example, its
  definition:
  
  @code{.c}
  dgNode* rootNode = dgCreate(NULL, 1);
  dgNode* leafNode = dgCreate(NULL, 2);
 **dgLink(rootNode, 0, 1) = leafNode;
  @endcode

  ...will generate the following structure:

  @verbatim
  +----------+
  | rootNode |  +----------+
  | Link 0 -----> leafNode |
  +----------+  | Link 0 -----> NULL
                | Link 1 -----> NULL
                +----------+
  @endverbatim
  
  Presuming the link is defined, if dereferenced again, you can then set
  the linked node's attributes (i.e., payload and links). Alternatively,
  the dgTraverse() helper function makes this more straightforward.

  @note       Depth must be greater than or equal to one
*/
extern dgNode** dgLink(dgNode*, size_t, size_t);

/**
  @fn         dgNode* dgTraverse(dgNode* node, size_t index, size_t depth)
  @brief      Traverse the graph a given depth down a specified link index from the starting node
  @param      node   The starting node
  @param      index  The link index to traverse
  @param      depth  The distance to traverse from the starting node
  @return     Pointer to the resolved node; or `NULL` in the event of a
              routing failure

  Return the pointer to the node a specified depth down a particular
  link index from a given starting node.

  @note       The traversal is homogeneous over the index; for a more
              complex route, use dgRoute()
*/
extern dgNode* dgTraverse(dgNode*, size_t, size_t);

/**
  @fn         dgNode* dgRoute(dgNode* node, dynArray* route)
  @brief      Walk the graph from a given starting node following a specific route
  @param      node   The starting node
  @param      route  A dynamic array of link indices that describe the route
  @return     Pointer to the resolved node; or `NULL` in the event of a
              routing failure
  @todo       Not yet implemented

  Return the pointer to the node resolved by following the specified
  route from the given starting node. The routing array ought to contain
  valid link indices for each point in the graph.

  For example, to get from `node0` to `nodeX` in the following graph:

  @verbatim
      +--------+                     +--------+    
   ,--> node0  |   ,-----------------> [Node] |    
  /   | Link 0 ---'                  | Link 0 ---, 
  |   | Link 1 --,   +--------+   ,--- Link 1 |   \
  |   +--------+  '--> [Node] <--'   +--------+   |
  |                  | Link 0 --,                 |
  |                  +--------+  \                |
  |                              |                |
  |                            +-V------+         /
  \                            | nodeX  <--------' 
   '---------------------------- Link 0 |          
                               +--------+          
  @endverbatim

  ...you could use any of these routes: {0, 0}; {0, 1, 0}; or {1, 0}.
  Indeed, because the graph is cyclic, there are an infinite number of
  routes connecting the two nodes:

  @code{.c}
  size_t myRouteArray[7] = {0, 1, 0, 0, 0, 1, 0};
  dynArray* myRoute = dynProject(myRouteArray, 7, sizeof(size_t));
  dynArray* nodeX = dynRoute(node0, myRoute);
  @endcode
*/
extern dgNode* dgRoute(dgNode*, dynArray*);

/**
  @fn         cyclicity dgIsCyclic(dgNode* node)
  @brief      Check the directed graph for cycles
  @param      node  Node to traverse from
  @return     The cyclicity of the directed graph
  @todo       Not yet implemented

  Determines if the given directed graph contains any cycles when
  traversed from a given node.
*/
extern cyclicity dgIsCyclic(dgNode*);

/**
  @fn         dgNode* dgCopy(dgNode* node)
  @brief      Shallow copy a directed graph/subgraph
  @param      node  Node to traverse from
  @return     Pointer to the equivalent copied starting node; or `NULL`
              in the event of an allocation failure
  @warning    Running this against a graph that contains cycles will be
              non-terminating
  @todo       Not yet implemented

  Copy the directed graph's structure (i.e., nodes and links) to a free
  point in memory.

  @note       Copying will not copy the directed graph's nodes'
              contents, but references will be preserved
*/
extern dgNode* dgCopy(dgNode*);

/**
  @fn         void dgNuke(dgNode* node)
  @brief      Free the memory allocated by the directed graph's nodes
  @param      node  Node to traverse from
  @warning    Running this against a graph containing cycles will result
              in a double free memory corruption error

  Traverse the directed graph from a given node, down all links, and
  free the memory allocated for each.

  @note       The nodes' contents will not be freed
  @note       Cyclic graphs will need to be disconnected or freed
              manually
*/
extern void dgNuke(dgNode*);

#endif
