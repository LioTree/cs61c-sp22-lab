#include "transpose.h"

/* The naive transpose function as a reference. */
void transpose_naive(int n, int blocksize, int *dst, int *src) {
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            dst[y + x * n] = src[x + y * n];
        }
    }
}

/* Implement cache blocking below. You should NOT assume that n is a
 * multiple of the block size. */
void transpose_blocking(int n, int blocksize, int *dst, int *src) {
    // YOUR CODE HERE
    int m = (n % blocksize == 0)?n:(n + n % blocksize);
    for(int i = 0; i < m; i += blocksize) { // column
        for(int j = 0;j < m; j += blocksize) { // row
            for (int x = 0; x < blocksize; x++) {
                for (int y = 0; y < blocksize; y++) {
                    if((j + x) < n && (i + y) < n) {
                        dst[(i + y) + (j + x) * n] = src[(j + x) + (i + y) * n];
                    }
                }
            }
        }
    }
}
