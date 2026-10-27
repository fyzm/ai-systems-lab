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

// For small subarrays, insertion sort has less overhead than recursively
// splitting and merging all the way down to a single element.

/*
 * sort_c.c
 * Recursion Coarsening for Merge Sort
 */

#include "./util.h"

#define CUTOFF 16

static void merge_c(data_t* A, int p, int q, int r);
static inline void copy_c(data_t* source, data_t* dest, int n);
static inline void insertion_sort_c(data_t* A, int p, int r);

/* Insertion sort for small subarrays */
static inline void insertion_sort_c(data_t* A, int p, int r) {
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

/* Coarsened merge sort */
void sort_c(data_t* A, int p, int r) {
  assert(A);

  if (p >= r) {
    return;
  }

  int n = r - p + 1;

  /* Base case: insertion sort */
  if (n <= CUTOFF) {
    insertion_sort_c(A, p, r);
    return;
  }

  int q = p + (r - p) / 2;

  sort_c(A, p, q);
  sort_c(A, q + 1, r);

  merge_c(A, p, q, r);
}

/* Pointer-based copy */
static inline void copy_c(data_t* source, data_t* dest, int n) {
  assert(source);
  assert(dest);

  data_t* source_end = source + n;

  while (source < source_end) {
    *dest = *source;
    source++;
    dest++;
  }
}

/* Merge two sorted subarrays */
static void merge_c(data_t* A, int p, int q, int r) {
  assert(A);
  assert(p <= q);
  assert((q + 1) <= r);

  int n1 = q - p + 1;
  int n2 = r - q;

  data_t* left = 0;
  data_t* right = 0;

  mem_alloc(&left, n1 + 1);
  mem_alloc(&right, n2 + 1);

  if (left == NULL || right == NULL) {
    mem_free(&left);
    mem_free(&right);
    return;
  }

  copy_c(A + p, left, n1);
  copy_c(A + q + 1, right, n2);

  /* Sentinel values */
  left[n1] = UINT_MAX;
  right[n2] = UINT_MAX;

  data_t* left_cur = left;
  data_t* right_cur = right;
  data_t* dest = A + p;
  data_t* dest_end = A + r + 1;

  while (dest < dest_end) {
    if (*left_cur <= *right_cur) {
      *dest = *left_cur;
      left_cur++;
    } else {
      *dest = *right_cur;
      right_cur++;
    }
    dest++;
  }

  mem_free(&left);
  mem_free(&right);
}
