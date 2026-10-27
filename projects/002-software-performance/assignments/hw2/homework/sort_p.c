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


static void merge_p(data_t* A, int p, int q, int r);

static inline void copy_p(data_t* source, data_t* dest, int n);

void sort_p(data_t* A, int p, int r) {
  assert(A);

  if (p < r) { 
  
    int q = (p + r) / 2;
    sort_p(A, p, q);
    sort_p(A, q + 1, r);
    merge_p(A, p, q, r); 
  
  }
  
}

static inline void copy_p(data_t* source, data_t* dest, int n) {
  assert(source);
  assert(dest);


  data_t* source_end = source +n;

  while (source < source_end) {
    *dest = *source;
    source++;
    dest++;
  }

}

static void merge_p(data_t* A, int p, int q, int r) {
  
  assert(A);
  assert(p <= q);
  assert((q + 1) <= r);
  int n1 = q - p + 1;
  int n2 = r - q;

  data_t* left = 0, * right = 0;
  mem_alloc(&left, n1 + 1);
  mem_alloc(&right, n2 + 1);
  if (left == NULL || right == NULL) {
    mem_free(&left);
    mem_free(&right);
    return;
  }
  copy_p(A + p, left, n1);
  copy_p(A + q + 1, right, n2);

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

