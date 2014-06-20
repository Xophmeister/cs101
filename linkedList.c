#include <stdlib.h>
#include "linkedList.h"

llNode* llCreateNode(void* payload) {
  llNode* newNode = malloc(sizeof(llNode));

  if (newNode) {
    newNode->next = NULL;
    newNode->payload = payload;
  }

  return newNode;
}

void llLink(llNode* alpha, llNode* beta) {
  alpha->next = beta;
}

/* WARNING!
   Running this against a cyclic list will be non-halting. */
size_t llLength(llNode* root) {
  if (root->next) {
    return 1 + llLength(root->next);
  } else {
    return 1;
  }
}

llNode* llTraverse(llNode* root, size_t index) {
  if (index == 0) {
    return root;
  } else {
    if (index > 0 && root->next) {
      return llTraverse(root->next, index - 1);
    } else {
      return NULL;
    }
  }
}

/* WARNING!
   Running this against a cyclic list will be non-halting. */
llNode* llCopy(llNode* root) {
  size_t n = llLength(root);

  llNode** copy = malloc(sizeof(llNode) * n);

  /* FIXME */
  while(n--) {
    llNode* this = (llNode*)(copy + n);
    this->payload = llTraverse(root, n)->payload; 
    if (n) {
      this->next = (llNode*)(copy + n - 1);
    }
  }
 
  return *copy;
}

/* WARNING!
   Running this against a cyclic list will cause a double free memory
   corruption error. Cyclic lists need to be freed manually. */
void llNuke(llNode* root) {
  /* Traverse list */
  if (root->next) {
    llNuke(root->next);
  }

  free(root);
}

/* WARNING!
   Running this against a cyclic list will be non-halting. */
void llAppend(llNode* root, void* payload) {
  if (root->next) {
    llAppend(root->next, payload);
  } else {
    llLink(root, llCreateNode(payload));
  }
}

void llInsertAfter(llNode* root, size_t index, void* payload) {
  llNode* splice = llTraverse(root, index);

  if (splice) {
    llNode* newNode = llCreateNode(payload);
    llLink(newNode, splice->next);
    llLink(splice, newNode);
  }
}

void llInsertBefore(llNode** root, size_t index, void* payload) {
  if (index == 0) {
    llNode* newRoot = llCreateNode(payload);
    llLink(newRoot, *root);
    *root = newRoot;
  } else {
    llInsertAfter(*root, index - 1, payload);
  }
}

void llDelete(llNode** root, size_t index) {
  if (index == 0) {
    llNode* newRoot = (*root)->next;
    free(*root);
    *root = newRoot;
  } else {
    llNode* splice = llTraverse(*root, index - 1);

    if (splice) {
      llNode* toDelete = splice->next;
      if (toDelete) {
        llNode* reLink = toDelete->next;
        free(toDelete);
        llLink(splice, reLink);
      }
    }
  }
}
