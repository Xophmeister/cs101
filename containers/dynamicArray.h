/**
  @file       dynamicArray.h
  @brief      Dynamic array header file
  @author     Christopher Harrison (Xophmeister)
  @copyright  MIT License

  Implements a dynamic array, where each element can point to arbitrary
  data in memory.
*/

#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include <stdlib.h>

/**
  @struct     dynArray
  @brief      Dynamic array
  @var        dynArray::length
              Number of elements in the array
  @var        dynArray::buffer
              Array's data buffer

  @warning    dynArray::length is not write protected
*/
typedef struct {
  size_t length;
  void** buffer;
} dynArray;

/**
  @fn         dynArray* dynCreate(size_t elements)
  @brief      Create a dynamic array of a given size
  @param      elements  Number of elements to initially allocate
  @return     Pointer to new dynamic array structure; or `NULL` on
              allocation failure

  Create a dynamic array of a given size, with all elements initialised
  to point to `NULL`.
*/
dynArray* dynCreate(size_t);

/**
  @fn         dynArray* dynCreateFrom(...)
  @brief      Create a dynamic array from the given arguments
  @param      ...  Pointers to the sequence of elements
  @return     Pointer to new dynamic array structure; or `NULL` on
              allocation failure

  Create a dynamic array from a variadic sequence of pointers, which
  will correspond to their respective element.
*/
dynArray* dynCreateFrom(...);

/**
  @fn         void dynAppend(dynArray* array, ...)
  @brief      Append the specified array with the given arguments
  @param      array  The dynamic array to append to
  @param      ...    Pointers to the sequence of elements

  Append a variadic sequence of pointers to the end of the dynamic
  array, updating its structure appropriately.
*/
void dynAppend(dynArray*, ...);

/**
  @fn         void* dynElement(dynArray* array, size_t index)
  @brief      Get the pointer to the array element at the given index
  @param      array  The dynamic array to query
  @param      index  The element offset
  @return     Pointer to the specified element; or `NULL` in the event
              of a bounds error

  Get the pointer to the specified array element at the given index.
*/
void* dynElement(dynArray*, size_t);

void dynResize(dynArray*, size_t);

dynArray* dynCopy(dynArray*);

void dynNuke(dynArray*);

#endif
