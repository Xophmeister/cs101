#include <stdlib.h>
#include <string.h>

#include "directedGraph.h"
#include "../indexed/dynamicArray.h"

dgNode* dgCreateNode(void* payload, size_t links) {
  dgNode* newNode = malloc(sizeof(dgNode));

  if (newNode) {
    newNode->payload = payload;
    newNode->links   = dynCreate(links);

    if (!newNode->links) {
      /* Memory allocation failure :P */
      free(newNode);
      newNode = NULL;
    }
  }

  return newNode;
}

dgNode** dgLink(dgNode* node, size_t index, size_t depth) {
  if (depth) {
    dgNode** next = (dgNode**)dynElement(node->links, index);

    if (depth == 1) {
      return next;
    } else {
      if (*next) {
        return dgLink(*next, index, depth - 1);
      } else {
        return NULL;
      }
    }
  } else {
    return NULL;
  }
}

dgNode* dgTraverse(dgNode* node, size_t index, size_t depth) {
  if (depth) {
    dgNode** end = dgLink(node, index, depth);
    return *end;
  } else {
    return node;
  }
}

/* TODO */
dgNode* dgRoute(dgNode* node, dynArray* route) {

}

cyclicity dgIsCyclic(dgNode* node) {
  /* TODO */
}

dgNode* dgCopy(dgNode* node) {
  /* TODO */
}

static int freeNode(void** node, size_t i, dynArray* links) {
  if (*node) {
    dgNuke((dgNode*)*node);
  }
  return 0;
}

void dgNuke(dgNode* node) {
  if (node) {
    if (node->links) {
      dynForEach(node->links, &freeNode);
      dynNuke(node->links);
    }
    free(node);
  }
}
