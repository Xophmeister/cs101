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
    return *dgLink(node, index, depth);
  } else {
    return node;
  }
}

static dgNode* dgStep(dgNode* node, dynArray* route, size_t cursor) {
  if (node) {
    if (cursor == route->length) {
      return node;
    } else {
      size_t* nextTurn = *dynElement(route, cursor);

      if (nextTurn) {
        return dgStep(*dynElement(node->links, *nextTurn), route, cursor + 1);
      } else {
        return NULL;
      }
    }
  } else {
    return NULL;
  }
}

dgNode* dgRoute(dgNode* node, dynArray* route) {
  if (route) {
    return dgStep(node, route, 0);
  } else {
    return node;
  }
}

cyclicity dgIsCyclic(dgNode* node) {
  /* TODO */
}

dgNode* dgCopy(dgNode* node) {
  /* TODO */
}

static int chainNuke(void** node, size_t i, dynArray* links) {
  if (*node) {
    dgNuke((dgNode*)*node);
  }
  return 0;
}

void dgNuke(dgNode* node) {
  if (node) {
    if (node->links) {
      dynForEach(node->links, &chainNuke);
      dynNuke(node->links);
    }
    free(node);
  }
}
