/**
  @file     linkedList.h
  @brief    Linked list header file
  @author   Christopher Harrison (Xophmeister)

  Implements a singly linked list, where each node can point to
  arbitrary data in memory.
*/

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdlib.h>

/**
  @struct   llNode
  @brief    Linked list node
  @var      llNode::payload
            Pointer to the current node's contents
  @var      llNode::next
            Pointer to the following node, if any
*/
typedef struct llNode {
  void*          payload;
  struct llNode* next;
} llNode;

/**
  @enum     cyclicity
  @brief    The cyclicity of a structure
  @var      cyclicity::acyclic
            Acyclic
  @var      cyclicity::cyclic
            Cyclic (i.e., contains cycles)
*/
typedef enum {
  acyclic,
  cyclic
} cyclicity;

/**
  @fn       llNode* llCreateNode(void* payload)
  @brief    Create a new linked list node
  @param    payload  Pointer to the node's contents
  @return   Address of the newly created node; or `NULL` in the event
            of an allocation error

  Allocate memory for a new unlinked node, with the given contents.
*/
extern llNode* llCreateNode(void*);

/**
  @fn       void llLink(llNode* alpha, llNode* beta)
  @brief    Link two arbitrary linked list nodes
  @param    alpha  Linker node
  @param    beta   Linkee node

  Create (and overwrite) the link from one node to another.

  @note     A node *can* link to itself
*/
extern void llLink(llNode*, llNode*);

/**
  @fn       size_t llLength(llNode* root)
  @brief    Number of elements in a linked list
  @param    root  Node to traverse from
  @return   Length of list/sublist.
  @warning  Running this against a cyclic list will be non-terminating

  Traverse the linked list from a given node to determine its length.
*/
extern size_t llLength(llNode*);

/**
  @fn       llNode* llTraverse(llNode* root, size_t index)
  @brief    Get the node at an offset down the linked list
  @param    root   Node to traverse from
  @param    index  Offset from the root node
  @return   Address of the offset node; or `NULL` in the event of a
            bounds error

  Get the node which is a given offset down the linked list from a given
  starting node.
  
  @note     The offset must be non-negative
  @note     The traversal is cycle-invariant, so the offset *may* exceed
            the number of physical nodes, providing they link cyclically
*/
extern llNode* llTraverse(llNode*, size_t);

/**
  @fn       llNode* llCopy(llNode* root)
  @brief    Shallow copy a linked list/sublist
  @param    root  Node to traverse from
  @return   Address of the new starting node
  @warning  Running this against a cyclic list will be non-terminating
  @bug      Currently segfaults

  Copy the linked list's structure (i.e., nodes and links) to a free
  point in memory.

  @note     Copying will not copy the linked list's node's contents, but
            references will be preserved
  @note     Copying will have the effect of defragmenting a linked list
            that has had any structural modifications
*/
extern llNode* llCopy(llNode*);

/**
  @fn       void llNuke(llNode* root)
  @brief    Free the memory allocated by the linked list's nodes
  @param    root  Node to traverse from
  @warning  Running this against a cyclic list will result in a double
            free memory corruption error

  Traverse the linked list from a given node and free the memory
  allocated for each.

  @note     The nodes' contents will not be freed
  @note     Cyclic lists need to be freed manually
*/
extern void llNuke(llNode*);

/**
  @fn       void llAppend(llNode* root, void* payload)
  @brief    Append data to a linked list
  @param    root     Node to traverse from
  @param    payload  Pointer to the appended contents
  @warning  Running this against a cyclic list will be non-terminating

  Create a new node, with the given contents, and link the current end
  node in the linked list to it.

  @note     Each append will traverse the linked list from the given
            starting node so, if you are appending a lot of data, it
            would be more efficient to keep track of the last node, or
            better yet, simply prepend with llInsertBefore()
*/
extern void llAppend(llNode*, void*);

/**
  @fn       void llInsertAfter(llNode* root, size_t index, void* payload)
  @brief    Insert a node into the linked list after a given offset
  @param    root     Node to traverse from
  @param    index    Offset from the root node
  @param    payload  Pointer to the inserted contents

  Create a new node, with the given contents, and insert it immediately
  after the node that lies a given offset from the starting node.
*/
extern void llInsertAfter(llNode*, size_t, void*);

/**
  @fn       void llInsertBefore(llNode** root, size_t index, void* payload)
  @brief    Insert a node into the linked list before a given offset
  @param    root     Node to traverse from
  @param    index    Offset from the root node
  @param    payload  Pointer to the inserted contents

  Create a new node, with the given contents, and insert it immediately
  before the node that lies a given offset from the starting node.

  @note     If you insert before the starting node, the pointer to the
            starting node will be updated appropriately
*/
extern void llInsertBefore(llNode**, size_t, void*);

/**
  @fn       void llDelete(llNode** root, size_t index)
  @brief    Delete the node of a linked list at a given offset
  @param    root   Node to traverse from 
  @param    index  Offset from the root node

  Free the memory allocated for the node at a given offset from the
  starting node, relinking neighbouring nodes where appropriate.

  @note     If you delete the starting node, the pointer to the starting
            node will be updated appropriately
  @note     The nodes' contents will not be freed
*/
extern void llDelete(llNode**, size_t);

/**
  @fn       void llReverse(llNode** root)
  @brief    Reverse the order of a linked list
  @param    root  Node to traverse from 
  @warning  Running this against a cyclic list will be non-terminating
  @todo     Not yet implemented

  Traverse the linked list and swap the links between nodes to reverse
  the list.

  @note     The pointer to the starting node will be updated
            appropriately
*/
extern void llReverse(llNode**);

/**
  @fn       llNode* llFromArray(void** array, size_t length)
  @brief    Convert an array to a linked list
  @param    array   Array  of `void*` to convert
  @param    length  Array length
  @return   Address of the starting node; or `NULL` in the event of an
            allocation error
  @todo     Not yet implemented

  Convert an array's elements into nodes and link them together
  sequentially.
*/
extern llNode* llFromArray(void**, size_t);

/**
  @fn       void** llToArray(llNode* root)
  @brief    Covert a linked list to an array
  @param    root  Node to traverse from
  @return   Address of the array's initial element; or `NULL` in the
            event of an allocation error
  @warning  Running this against a cyclic list will be non-terminating
  @todo     Not yet implemented

  Allocate a contiguous chunk of memory and sequentially copy the nodes
  of the linked list into it appropriately.
*/
extern void** llToArray(llNode*);

/**
  @fn       cyclicity llIsCyclic(llNode* root)
  @brief    Determine if the linked list contains a cycle
  @param    root  Node to traverse from
  @return   The cyclicity of the linked list
  @todo     Not yet implemented

  Determines if the linked list is cyclic from a given node.
*/
extern cyclicity llIsCyclic(llNode*);

#endif
