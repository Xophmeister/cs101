/**
  @file       dynamicArray.h
  @brief      Dynamic array header file
  @author     Christopher Harrison (Xophmeister)
  @copyright  @ref license

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
  @var        dynArray::allocated
              Actual number of elements currently allocated
  @var        dynArray::buffer
              Array's data buffer

  @warning    dynArray::length and dynArray::allocated are not write
              protected
*/
typedef struct {
  size_t length;
  size_t allocated;
  void** buffer;
} dynArray;

/**
  @typedef    dynForEachCallback
  @brief      Function signature for dynForEach() callbacks

  The callback function for dynForEach() must have the following
  signature:

 *@code{.c}
 *int callback(void** element, size_t index, dynArray* array)
 *@endcode

  That is, on each element, the callback is called with the following:

  @param      element  The pointer to the pointer to the current element
  @param      index    The current index
  @param      array    The dynamic array

  @note       The element argument is doubly indirected so that it can
              be reassigned, if required
  @warning    Be careful not to dereference a `NULL` pointer in the
              event of a bounds error or phantom element
*/
typedef int(*dynForEachCallback)(void**, size_t, dynArray*);

/**
  @typedef    dynMapCallback
  @brief      Function signature for dynMap() callbacks

  The callback function for dynMap() must have the following signature:

 *@code{.c}
 *void* callback(void* element, size_t index, dynArray* array)
 *@endcode

  That is, on each element, the callback is called with the following:

  @param      element  The pointer to the current element
  @param      index    The current index
  @param      array    The dynamic array

  The callback function must return the pointer to the transformed
  result. For example, if the dynamic array contained only integers, the
  following callback would have the effect of doubling each element:

 *@code{.c}
 *void* doubleInt(void* e, size_t i, dynArray* array) {
 *  int* newValue = malloc(sizeof(int));
 *  *newValue = *(int*)e * 2;
 *  return (void*)newValue;
 *}
 *@endcode

  @note       The callback's element argument is, contrary to that for
              dynForEach(), singly indirected to encourage immutability
              on the original array
  @note       The callback function must handle the memory allocation of
              the transformed element; likewise, final freeing must be
              done manually
*/
typedef void*(*dynMapCallback)(void*, size_t, dynArray*);

/**
  @typedef    dynFilterCallback
  @brief      Function signature for dynFilter() callbacks

  The callback function for dynFilter() must have the following
  signature:

 *@code{.c}
 *int callback(void* element, size_t index, dynArray* array)
 *@endcode

  That is, on each element, the callback is called with the following:

  @param      element  The pointer to the current element
  @param      index    The current index
  @param      array    The dynamic array

  The element is included in the filter if the callback function returns
  a non-zero value.

  @note       The callback's element argument is, contrary to that for
              dynForEach(), singly indirected to encourage immutability
              on the original array
*/
typedef int(*dynFilterCallback)(void*, size_t, dynArray*);

/**
  @typedef    dynFoldCallback
  @brief      Function signature for dynFold() callbacks
  
  The callback function for dynFold() must have the following signature:

 *@code{.c}
 *void callback(void* accumulator, void* element, size_t index, dynArray* array)
 *@endcode

  That is, on each element, the callback is called with the following:

  @param      accumulator  The pointer to the accumulator
  @param      element      The pointer to the current element
  @param      index        The current index
  @param      array        The dynamic array

  The final result of the fold will end up in the accumulator. For
  example, if the dynamic array contained only integers, the following
  callback would have the effect of summing the array:

 *@code{.c}
 *void sum(void* acc, void* e, size_t i, dynArray* array) {
 *  if (e) {
 *    *(int*)acc += *(int*)e;
 *  }
 *}
 *@endcode

  @note       The callback's element argument is, contrary to that for
              dynForEach(), singly indirected to encourage immutability
              on the original array
*/
typedef void(*dynFoldCallback)(void*, void*, size_t, dynArray*);

/**
  @typedef    dynZipWithCallback
  @brief      Function signature for dynZipWith() callbacks
  
  The callback function for dynZipWith() must have the following
  signature:

 *@code{.c}
 *void* callback(void* elementAlpha, void* elementBeta, size_t index, dynArray* arrayAlpha, dynArray* arrayBeta)
 *@endcode

  That is, on each element, the callback is called with the following:

  @param      elementAlpha  The pointer to the current element from the
                            first dynamic array
  @param      elementBeta   The pointer to the current element from the
                            second dynamic array
  @param      index         The current index
  @param      arrayAlpha    The first dynamic array
  @param      arrayBeta     The second dynamic array

  The callback function must return the pointer to the zipped result.
  For example, if the two dynamic arrays contained only integers, the
  following callback would have the effect of adding them together:

 *@code{.c}
 *void* sumPair(void* e1, void* e2, size_t i, dynArray* a1, dynArray* a2) {
 *  int* newValue = malloc(sizeof(int));
 *  *newValue = *(int*)e1 + *(int*)e2;
 *  return (void*)newValue;
 *}
 *@endcode

  @note       The callback's element arguments are, contrary to that for
              dynForEach(), singly indirected to encourage immutability
              on the original arrays
  @note       The callback function must handle the memory allocation of
              the transformed element; likewise, final freeing must be
              done manually
*/
typedef void*(*dynZipWithCallback)(void*, void*, size_t, dynArray*, dynArray*);

/**
  @fn         dynArray* dynCreate(size_t length)
  @brief      Create a dynamic array of a given size
  @param      length  Number of elements to initially allocate
  @return     Pointer to new dynamic array structure; or `NULL` in the
              event of an allocation failure

  Create a dynamic array of a given size, with all elements initialised
  with `NULL` pointers.
  
  @note       The allocation size will match the requested size
*/
extern dynArray* dynCreate(size_t);

/**
  @fn         void dynResize(dynArray* array, size_t length)
  @brief      Resize the array to the given number of elements
  @param      array   The dynamic array to resize
  @param      length  The new length

  Enlarge or reduce the size of a dynamic array to a given length.

  @note       If a dynamic array is reduced in length, any tail elements
              will be unrecoverable
  @warning    In the event of a reallocation failure, the original array
              will be lost and the length reset to zero
*/
extern void dynResize(dynArray*, size_t);

/**
  @fn         void dynAppend(dynArray* array, void* payload)
  @brief      Append the specified array with the given argument
  @param      array    The dynamic array to append to
  @param      payload  Pointer to the new element

  Append an element pointer to the end of the dynamic array, updating
  its structure appropriately.

  @note       Memory will be over-allocated if there is not enough free
              space in the buffer
  @warning    In the event of a reallocation failure, the original array
              will be lost and the length reset to zero
*/
extern void dynAppend(dynArray*, void*);

/**
  @fn         void** dynElement(dynArray* array, size_t index)
  @brief      Get the pointer to the array element at the given index
  @param      array  The dynamic array to query
  @param      index  The element offset
  @return     Pointer to the pointer to the specified element; or `NULL`
              in the event of a bounds error

  Return the pointer which points to the specified array's element at
  the given index. This can then be dereferenced to get/set the element.
*/
extern void** dynElement(dynArray*, size_t);

/**
  @fn         dynArray* dynSlice(dynArray* array, size_t from, size_t to)
  @brief      Shallow copy a dynamic array between two indices
  @param      array  The dynamic array to slice
  @param      from   The initial index
  @param      to     The final index
  @return     Pointer to the copied dynamic array slice; or `NULL` in
              the event of an allocation failure

  Copy a portion of the dynamic array to a free point in memory.

  @note       The terminal indices are both included in the slice
  @note       This is a shallow copy, so element pointer changes to the
              slice will not be reflected in the parent array and vice
              versa
*/
extern dynArray* dynSlice(dynArray*, size_t, size_t);

/**
  @fn         dynArray* dynCopy(dynArray* array)
  @brief      Shallow copy a dynamic array
  @param      array  The dynamic array to copy
  @return     Pointer to the copied dynamic array; or `NULL` in the
              event of an allocation failure

  Copy the dynamic array's structure to a free point in memory.

  @note       Any over-allocated memory will be reduced to match the
              array length
*/
extern dynArray* dynCopy(dynArray*);

/**
  @fn         dynArray* dynJoin(dynArray* arrayAlpha, dynArray* arrayBeta)
  @brief      Concatenate two dynamic arrays together
  @param      arrayAlpha  The first dynamic array
  @param      arrayBeta   The second dynamic array
  @return     Pointer to the joined dynamic array; or `NULL` in the
              event of an allocation failure

  Concatenate two dynamic arrays together.
*/
extern dynArray* dynJoin(dynArray*, dynArray*);

/**
  @fn         void dynForEach(dynArray* array, dynForEachCallback callback)
  @brief      Iterate over the array and apply a function to each element
  @param      array     The dynamic array to iterate over
  @param      callback  Pointer to callback function

  Iterate over all the elements of the dynamic array and apply the
  specified callback function against each.

  @note       The iteration will break if the callback function returns
              a non-zero value
*/
extern void dynForEach(dynArray*, dynForEachCallback);

/**
  @fn         dynArray* dynMap(dynArray* array, dynMapCallback callback)
  @brief      Map the elements of a dynamic array through a function
  @param      array     The dynamic array to iterate over
  @param      callback  Pointer to callback function
  @return     Pointer to the transformed dynamic array; or `NULL` in the
              event of an allocation failure

  Creates a new dynamic array by applying the specified callback
  function to each element from the original.
*/
extern dynArray* dynMap(dynArray*, dynMapCallback);

/**
  @fn         dynArray* dynFilter(dynArray* array, dynFilterCallback callback)
  @brief      Filter the dynamic array's elements to only those which pass the callback
  @param      array     The dynamic array to filter
  @param      callback  Pointer to callback function
  @return     Pointer to the transformed dynamic array; or `NULL` in the
              event of an allocation failure

  Creates a new dynamic array by testing each element against the
  specified callback function and appending those which pass.
*/
extern dynArray* dynFilter(dynArray*, dynFilterCallback);

/**
  @fn         void dynFold(dynArray* array, void* accumulator, dynFoldCallback callback)
  @brief      Fold the dynamic array elements down to a single value
  @param      array        The dynamic array to fold
  @param      accumulator  Pointer to the accumulator
  @param      callback     Pointer to callback function

  Perform a (right) fold on the dynamic array, reducing the elements to
  a single value by applying the callback function against a running
  total (accumulator) and the current element.

  @note       The accumulator's initial value should be set prior to
              performing the fold
*/
extern void dynFold(dynArray*, void*, dynFoldCallback);

/**
  @fn         dynArray* dynZipWith(dynArray* arrayAlpha, dynArray* arrayBeta, dynZipWithCallback callback)
  @brief      Apply the given callback function pairwise to the given arrays elements
  @param      arrayAlpha  The first dynamic array
  @param      arrayBeta   The second dynamic array
  @param      callback    Pointer to callback function
  @return     Pointer to the zipped dynamic array; or `NULL` in the
              event of an allocation failure

  Zips the two given arrays together, through the specified callback
  function applied to successive element pairs.

  @note       The length of the returned dynamic array will match that
              of the shorter input array; tail elements from the longer
              array will not be processed
*/
extern dynArray* dynZipWith(dynArray*, dynArray*, dynZipWithCallback);

/**
  @fn         void dynNuke(dynArray* array)
  @brief      Free the memory allocated by the dynamic array
  @param      array  The dynamic array to free

  Free the memory allocated by the dynamic array and its data buffer.

  @note       The array's elements will not be freed
*/
extern void dynNuke(dynArray*);

#endif
