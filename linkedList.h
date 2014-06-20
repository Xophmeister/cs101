#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdlib.h>

typedef struct llNode {
  void*          payload;
  struct llNode* next;
} llNode;

/* Create raw node */
extern llNode* llCreateNode(void* payload);

/* Link arbitrary nodes */
extern void llLink(llNode* alpha, llNode* beta);

/* List length from root */
extern size_t llLength(llNode* root);

/* Node at index from root */
extern llNode* llTraverse(llNode* root, size_t index);

/* Memory allocation */
extern llNode* llCopy(llNode* root);
extern void llNuke(llNode* root);

/* Structural/data operations */
extern void llAppend(llNode* root, void* payload);
extern void llInsertAfter(llNode* root, size_t index, void* payload);
extern void llInsertBefore(llNode** root, size_t index, void* payload);
extern void llDelete(llNode** root, size_t index);
extern void llReverse(llNode** root);

/* Array conversion */
extern llNode* llFromArray(void** array, size_t length);
extern void** llToArray(llNode* root);

/* Check cyclicity...is that a word? */
extern int llIsCyclic(llNode* root);

#endif
