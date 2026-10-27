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

static void sort_f_recursive(data_t* A, int p, int r, data_t* scratch);
static void merge_f(data_t* A, int p, int q, int r, data_t* scratch);
static inline void insertion_sort_f(data_t* A, int p, int r);

static inline void insertion_sort_f(data_t* A, int p, int r) {
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

void sort_f(data_t* A, int p, int r) {
  assert(A);

  if (p >= r) {
    return;
  }

  if (r - p + 1 <= CUTOFF) {
    insertion_sort_f(A, p, r);
    return;
  }

  /* The largest merge copies at most ceil((r - p + 1) / 2) elements. */
  int scratch_size = (r - p + 2) / 2;
  data_t* scratch = NULL;
  mem_alloc(&scratch, scratch_size);
  if (scratch == NULL) {
    return;
  }

  sort_f_recursive(A, p, r, scratch);
  mem_free(&scratch);
}

static void sort_f_recursive(data_t* A, int p, int r, data_t* scratch) {
  if (p >= r) {
    return;
  }

  if (r - p + 1 <= CUTOFF) {
    insertion_sort_f(A, p, r);
    return;
  }

  int q = p + (r - p) / 2;
  sort_f_recursive(A, p, q, scratch);
  sort_f_recursive(A, q + 1, r, scratch);
  merge_f(A, p, q, r, scratch);
}

static void merge_f(data_t* A, int p, int q, int r, data_t* scratch) {
  assert(A);
  assert(scratch);
  assert(p <= q);
  assert(q + 1 <= r);

  int left_size = q - p + 1;
  data_t* source = A + p;
  data_t* left_cur = scratch;
  data_t* left_end = scratch + left_size;

  while (left_cur < left_end) {
    *left_cur++ = *source++;
  }

  left_cur = scratch;
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

  /* A remaining right suffix is already in its final location. */
  while (left_cur < left_end) {
    *dest++ = *left_cur++;
  }
}
