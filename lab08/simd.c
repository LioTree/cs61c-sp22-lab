#include <time.h>
#include <stdio.h>
#include <x86intrin.h>
#include "simd.h"

long long int sum(int vals[NUM_ELEMS]) {
    clock_t start = clock();

    long long int sum = 0;
    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        for(unsigned int i = 0; i < NUM_ELEMS; i++) {
            if(vals[i] >= 128) {
                sum += vals[i];
            }
        }
    }
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return sum;
}

long long int sum_unrolled(int vals[NUM_ELEMS]) {
    clock_t start = clock();
    long long int sum = 0;

    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        for(unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
            if(vals[i] >= 128) sum += vals[i];
            if(vals[i + 1] >= 128) sum += vals[i + 1];
            if(vals[i + 2] >= 128) sum += vals[i + 2];
            if(vals[i + 3] >= 128) sum += vals[i + 3];
        }

        // TAIL CASE, for when NUM_ELEMS isn't a multiple of 4
        // NUM_ELEMS / 4 * 4 is the largest multiple of 4 less than NUM_ELEMS
        // Order is important, since (NUM_ELEMS / 4) effectively rounds down first
        for(unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
            if (vals[i] >= 128) {
                sum += vals[i];
            }
        }
    }
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return sum;
}

long long int sum_simd(int vals[NUM_ELEMS]) {
    clock_t start = clock();
    __m128i _127 = _mm_set1_epi32(127); // This is a vector with 127s in it... Why might you need this?
    long long int result = 0; // This is where you should put your final result!
    /* DO NOT MODIFY ANYTHING ABOVE THIS LINE (in this function) */

    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        /* YOUR CODE GOES HERE */
        __m128i sum_vec = _mm_setzero_si128();
        for(unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
            __m128i tmp = _mm_loadu_si128((__m128i *) (vals + i));
            __m128i mask = _mm_cmpgt_epi32(tmp,_127);
            tmp = _mm_and_si128(tmp,mask);
            sum_vec = _mm_add_epi32(sum_vec, tmp);
        }
        int tmp_arr[4];
        _mm_storeu_si128((__m128i *) tmp_arr, sum_vec);
        result += tmp_arr[0] + tmp_arr[1] + tmp_arr[2] + tmp_arr[3];
        /* Hint: you'll need a tail case. */
        for(unsigned int i = NUM_ELEMS / 4 * 4;i < NUM_ELEMS;i++) {
           if (vals[i] >= 128) {
                result += vals[i];
            } 
        }
    }

    /* DO NOT MODIFY ANYTHING BELOW THIS LINE (in this function) */
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return result;
}

long long int sum_simd_unrolled(int vals[NUM_ELEMS]) {
    clock_t start = clock();
    __m128i _127 = _mm_set1_epi32(127);
    long long int result = 0;
    /* DO NOT MODIFY ANYTHING ABOVE THIS LINE (in this function) */

    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        /* YOUR CODE GOES HERE */
        /* Copy your sum_simd() implementation here, and unroll it */
        #define UNROLL 4
        __m128i sum_vec[UNROLL];
        for(unsigned int i = 0;i < UNROLL;i++)
            sum_vec[i] = _mm_setzero_si128();
        for(unsigned int i = 0; i < NUM_ELEMS / (4 * UNROLL) * (4 * UNROLL); i += (4 * UNROLL)) {
            // compiler cannot make loop unroll automatically since optimization is closed
            // for(unsigned int j = 0; j < UNROLL; j++) {
                __m128i tmp = _mm_loadu_si128(((__m128i *) (vals + i)) + 0);
                __m128i tmp2 = _mm_loadu_si128(((__m128i *) (vals + i)) + 1);
                __m128i tmp3 = _mm_loadu_si128(((__m128i *) (vals + i)) + 2);
                __m128i tmp4 = _mm_loadu_si128(((__m128i *) (vals + i)) + 3);
                __m128i mask = _mm_cmpgt_epi32(tmp,_127);
                __m128i mask2 = _mm_cmpgt_epi32(tmp2,_127);
                __m128i mask3 = _mm_cmpgt_epi32(tmp3,_127);
                __m128i mask4 = _mm_cmpgt_epi32(tmp4,_127);
                tmp = _mm_and_si128(tmp,mask);
                tmp2 = _mm_and_si128(tmp2,mask2);
                tmp3 = _mm_and_si128(tmp3,mask3);
                tmp4 = _mm_and_si128(tmp4,mask4);
                // sum_vec[j] = _mm_add_epi32(sum_vec[j], tmp);
                sum_vec[0] = _mm_add_epi32(sum_vec[0], tmp);
                sum_vec[1] = _mm_add_epi32(sum_vec[1], tmp2);
                sum_vec[2] = _mm_add_epi32(sum_vec[2], tmp3);
                sum_vec[3] = _mm_add_epi32(sum_vec[3], tmp4);
            // }
        }
        // for(unsigned int i = 0;i < UNROLL;i++) {
            int tmp_arr[4][4];
            _mm_storeu_si128((__m128i *) tmp_arr[0], sum_vec[0]);
            _mm_storeu_si128((__m128i *) tmp_arr[1], sum_vec[1]);
            _mm_storeu_si128((__m128i *) tmp_arr[2], sum_vec[2]);
            _mm_storeu_si128((__m128i *) tmp_arr[3], sum_vec[3]);
            result += tmp_arr[0][0] + tmp_arr[0][1] + tmp_arr[0][2] + tmp_arr[0][3];
            result += tmp_arr[1][0] + tmp_arr[1][1] + tmp_arr[1][2] + tmp_arr[1][3];
            result += tmp_arr[2][0] + tmp_arr[2][1] + tmp_arr[2][2] + tmp_arr[2][3];
            result += tmp_arr[3][0] + tmp_arr[3][1] + tmp_arr[3][2] + tmp_arr[3][3];
        // }
        /* Hint: you'll need 1 or maybe 2 tail cases here. */
        for(unsigned int i = NUM_ELEMS / (4 * UNROLL) * (4 * UNROLL);i < NUM_ELEMS;i++) {
           if (vals[i] >= 128) {
                result += vals[i];
            } 
        }
    }

    /* DO NOT MODIFY ANYTHING BELOW THIS LINE (in this function) */
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return result;
}
