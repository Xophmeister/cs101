#include <stdlib.h>
#include <string.h>

#include "dynamicArray.h"

/* NULLify elements between from and to in array->buffer */
static void nullElements(dynArray* array, size_t from, size_t to) {
  if (to >= from && to < array->length) {
    size_t n = (to + 1) - from;
    while (n--) {
      *(array->buffer + from + n) = NULL;
    }
  }
}

dynArray* dynCreate(size_t length) {
  dynArray* newArray = malloc(sizeof(dynArray));
  
  if (newArray) {
    newArray->length = length;

    /* Allocate data buffer */
    if (length) {
      /* n.b., calloc sets everything to 0, but that's not necessarily
               the same as the NULL pointer value. So we use malloc and
               NULLify manually. */
      newArray->buffer = malloc(sizeof(void*) * length);
      
      if (newArray->buffer) {
        newArray->allocated = length;
        nullElements(newArray, 0, length - 1);
      } else {
        newArray = NULL;
      }
    } else {
      newArray->allocated = 0;
      newArray->buffer    = NULL;
    }
  }

  return newArray;
}

void dynResize(dynArray* array, size_t length) {
  if (array) {
    if (length) {
      size_t oldLength = array->length;
      array->length    = length;

      if (length > array->allocated) {
        if (oldLength) {
          array->buffer = realloc(array->buffer, sizeof(void*) * length);
        } else {
          array->buffer = malloc(sizeof(void*) * length);
        }
        array->allocated = length;
      }
        
      if (array->buffer) {
        /* NULLify newly created space */
        if (length > oldLength) {
          nullElements(array, oldLength, length - 1);
        }
      } else {
        /* Memory (re)allocation failed :P */
        array->length    = 0;
        array->allocated = 0;
      }
    } else {
      array->length    = 0;
      array->allocated = 0;

      if (array->buffer) {
        free(array->buffer);
      }
      array->buffer = NULL;
    }
  }
}

void dynAppend(dynArray* array, void* payload) {
  if (array->allocated > array->length) {
    *(array->buffer + (array->length++)) = payload;
  } else {
    if (array->length) {
      /* Double buffer's allocation */
      size_t newAllocation = array->length * 2;
      array->buffer = realloc(array->buffer, sizeof(void*) * newAllocation);
      if (array->buffer) {
        *(array->buffer + (array->length++)) = payload;
        array->allocated = newAllocation;
      } else {
        /* Memory reallocation failed :P */
        array->length    = 0;
        array->allocated = 0;
      }
    } else {
      /* Create single element buffer */
      array->buffer = malloc(sizeof(void*));
      if (array->buffer) {
        array->length    = 1;
        array->allocated = 1;
        *(array->buffer) = payload;
      }
    }
  }
}

void** dynElement(dynArray* array, size_t index) {
  if (index < array->length) {
    return array->buffer + index;
  } else {
    return NULL;
  }
}

dynArray* dynProject(void* array, size_t length, size_t width) {
  dynArray* projected = dynCreate(length);
  
  /* Project array on to data buffer */
  if (length && projected) {
    size_t n = length;
    while (n--) {
      *dynElement(projected, n) = array + (n * width);
    }
  }

  return projected;
}

dynArray* dynSlice(dynArray* array, size_t from, size_t to) {
  if (to >= from && to < array->length) {
    size_t    newLength = (to + 1) - from;
    dynArray* newArray  = dynCreate(newLength);

    /* Copy data buffer */
    if (newArray) {
      memcpy(newArray->buffer, array->buffer + from, sizeof(void*) * newLength);
    }

    return newArray;
  } else {
    return NULL;
  }
}

dynArray* dynCopy(dynArray* array) {
  return dynSlice(array, 0, array->length - 1);
}

dynArray* dynJoin(dynArray* arrayAlpha, dynArray* arrayBeta) {
  if (arrayAlpha && arrayBeta) {
    dynArray* joined = dynCreate(arrayAlpha->length + arrayBeta->length);

    /* Copy data buffers */
    if (joined) {
      memcpy(joined->buffer, arrayAlpha->buffer, sizeof(void*) * arrayAlpha->length);
      memcpy(joined->buffer + arrayAlpha->length, arrayBeta->buffer, sizeof(void*) * arrayBeta->length);
    }

    return joined;
  } else {
    return NULL;
  }
}

void dynForEach(dynArray* array, dynForEachCallback callback) {
  if (array && array->length) {
    int interrupt;
    size_t n = array->length;
    while (n--) {
      interrupt = callback(dynElement(array, n), n, array);
      if (interrupt) { break; }
    }
  }
}

dynArray* dynMap(dynArray* array, dynMapCallback callback) {
  dynArray* transform = dynCreate(0);

  if (array && array->length) {
    size_t n = array->length;
    dynResize(transform, n);

    if (transform->length) {
      while (n--) {
        *dynElement(transform, n) = callback(*dynElement(array, n), n, array);
      }
    }
  }

  return transform;
}

dynArray* dynFilter(dynArray* array, dynFilterCallback callback) {
  dynArray* transform = dynCreate(0);

  if (array && array->length) {
    size_t i;
    size_t n = array->length;

    for (i = 0; i < n ; i++) {
      void* e = *dynElement(array, i);
      if (callback(e, i, array)) {
        dynAppend(transform, e);

        /* Memory reallocation failed :P */
        if (!transform->length) { break; }
      }
    }
  }

  return transform;
}

void dynFold(dynArray* array, void* accumulator, dynFoldCallback callback) {
  if (array && array->length) {
    size_t n = array->length;

    while (n--) {
      callback(accumulator, *dynElement(array, n), n, array);
    }
  }
}

dynArray* dynZipWith(dynArray* arrayAlpha, dynArray* arrayBeta, dynZipWithCallback callback) {
  dynArray* zipped = dynCreate(0);

  if (arrayAlpha && arrayBeta) {
    size_t n = arrayAlpha->length > arrayBeta->length ? arrayBeta->length : arrayAlpha->length;
    dynResize(zipped, n);

    if (zipped->length) {
      while (n--) {
        *dynElement(zipped, n) = callback(*dynElement(arrayAlpha, n), *dynElement(arrayBeta, n), n, arrayAlpha, arrayBeta);
      }
    }
  }

  return zipped;
}

void dynNuke(dynArray* array) {
  if (array) {
    if (array->buffer) {
      free(array->buffer);
    }
    free(array);
  }
}
