/**
  @file     stack.h
  @brief    Stack header file
  @author   Christopher Harrison (Xophmeister)

  Implements a stack (LIFO container), pointing to arbitrary data in
  memory.
*/

#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include "linkedList.h"

/**
  @struct   stack
  @brief    Stack
  @var      stack::count
            Number of items on the stack
  @var      stack::buffer
            Data buffer for the stack
*/
typedef struct stack {
  size_t  count;
  llNode* buffer;
} stack;

/**
  @fn       stack* stkCreate()
  @brief    Create a new stack
  @return   Address of the newly created stack; or `NULL` in the event
            of an allocation error

  Allocate memory for a new, empty stack
*/
extern stack* stkCreate();

/**
  @fn       void stkNuke(stack* stk)
  @brief    Free the memory allocated by the stack
  @param    stk  The stack

  Free the memory allocated by the stack and its buffer
*/
extern void stkNuke(stack*);

/**
  @fn       void stkPush(stack* stk, void* payload)
  @brief    Push a new element on to the top of the stack
  @param    stk      The stack
  @param    payload  Pointer to the element's contents

  Push a new element on to the top of the stack and update the structure
  appropriately.
*/
extern void stkPush(stack*, void*);

/**
  @fn       void* stkPop(stack* stk)
  @brief    Pop the latest element from the stack
  @param    stk  The stack
  @return   Pointer to the latest element; or `NULL` if the stack is
            empty

  Pop the latest element from the stack and update the structure
  appropriately.
*/
extern void* stkPop(stack*);

#endif
