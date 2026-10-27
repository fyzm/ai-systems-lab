/**
 * Copyright (c) 2012 MIT License by 6.172 Staff
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 **/


#include "./util.h"

#define CUTOFF 16

static void merge_m(data_t* A, int p, int q, int r);
static inline void insertion_sort_m(data_t* A, int p, int r);

static inline void insertion_sort_m(data_t* A, int p, int r) {
  for (int i = p + 1; i <= r; i++) {
    data_t key = A[i];
    int j = i - 1;

    while (j >= p && A[j] > key) {
      A[j + 1] = A[j];
      j--;
    }
    A[j + 1] = key;
  }
}

void sort_m(data_t* A, int p, int r) {
  assert(A);

  if (p >= r) {
    return;
  }

  if (r - p + 1 <= CUTOFF) {
    insertion_sort_m(A, p, r);
    return;
  }

  int q = p + (r - p) / 2;
  sort_m(A, p, q);
  sort_m(A, q + 1, r);
  merge_m(A, p, q, r);
}

/*
 * Copy only the left half.  The right half remains in A and acts as the
 * second merge input, reducing temporary memory usage by about one half.
 */
static void merge_m(data_t* A, int p, int q, int r) {
  assert(A);
  assert(p <= q);
  assert(q + 1 <= r);

  int left_size = q - p + 1;
  data_t* left = NULL;
  mem_alloc(&left, left_size);
  if (left == NULL) {
    return;
  }

  data_t* source = A + p;
  data_t* left_end = left + left_size;
  data_t* left_cur = left;
  while (left_cur < left_end) {
    *left_cur++ = *source++;
  }

  left_cur = left;
  data_t* right_cur = A + q + 1;
  data_t* right_end = A + r + 1;
  data_t* dest = A + p;

  while (left_cur < left_end && right_cur < right_end) {
    if (*left_cur <= *right_cur) {
      *dest++ = *left_cur++;
    } else {
      *dest++ = *right_cur++;
    }
  }

  /* If the right half remains, it is already in the correct positions. */
  while (left_cur < left_end) {
    *dest++ = *left_cur++;
  }

  mem_free(&left);
}
