#include <stdlib.h>
#include "stack.h"

stack* stkCreate() {
  stack* newStack = malloc(sizeof(stack));

  if (newStack) {
    newStack->count  = 0;
    newStack->buffer = NULL;
  }

  return newStack;
}

void stkNuke(stack* stk) {
  if (stk->count) {
    llNuke(stk->buffer);
  }
  free(stk);
}

void stkPush(stack* stk, void* payload) {
  if (stk->count) {
    llInsertBefore(&(stk->buffer), 0, payload);
  } else {
    stk->buffer = llCreateNode(payload);
  }

  ++stk->count;
}

void* stkPop(stack* stk) {
  if (stk->count--) {
    void* payload = stk->buffer->payload;
    llDelete(&(stk->buffer), 0);    
    return payload;
  } else {
    return NULL;
  }
}
