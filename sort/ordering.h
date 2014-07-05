/**
  @file       ordering.h
  @brief      Ordering prototypes header file
  @author     Christopher Harrison (Xophmeister)
  @copyright  @ref license

  Standard ordering interface.
*/

#ifndef ORDERING_H
#define ORDERING_H

/**
  @enum       order
  @brief      The order on comparison
  @var        order::equal
              The comparison is equal
  @var        order::lessThan
              The LHS is less than the RHS
  @var        order::greaterThan
              The LHS is greater than the RHS
  @var        order::incomparable
              The LHS and RHS can't be compared (e.g., type mismatch)
*/
typedef enum {
  equal,
  lessThan,
  greaterThan,
  incomparable
} order;

/**
  @typedef    ordering
  @brief      Function signature for sort callbacks

  The callback function for sorting functions must have the following
  signature:

  @code{.c}
  order callback(void* lhs, void* rhs)
  @endcode

  That is, on sorting, the callback is used to order value pairs:

  @param      lhs      The pointer to the left hand side of the comparison
  @param      rhs      The pointer to the right hand side of the comparison

  For example, the following callback function could be used to sort
  integers:

  @code{.c}
  order orderInt(void* lhs, void* rhs) {
    if (lhs && rhs) {
      int l = *(int*)lhs;
      int r = *(int*)rhs;

      if (l == r) {
        return equal;
      } else if (l < r) {
        return lessThan;
      } else {
        return greaterThan;
      }
    } else {
      return incomparable;
    }
  }
  @endcode

  @note       The sort callbacks expect `void*`, for some semblance of
              genericness; you will have to cast and dereference them
              appropriately and take care of `NULL` pointers
*/
typedef order(*ordering)(void*, void*);

#endif
