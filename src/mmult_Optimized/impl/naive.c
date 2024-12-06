/* Standard C includes */
#include <stdlib.h>

/* Include common headers */
#include "common/macros.h"
#include "common/types.h"

/* Include application-specific headers */
#include "include/types.h"

/* Naive Implementation */
#pragma GCC push_options
#pragma GCC optimize ("O1")

void* impl_scalar_naive(void* args) {
    args_t* a = (args_t*)args;
    float* A = a->input; // Pointer to Matrix A
    float* B = a->input + (ROWS_A * COLS_A); // Pointer to Matrix B
    float* R = a->output; // Pointer to Result Matrix R

    for (size_t i = 0; i < ROWS_A; i++) {
        for (size_t j = 0; j < COLS_B; j++) {
            R[i * COLS_B + j] = 0; // Initialize result element
            for (size_t k = 0; k < COLS_A; k++) {
                R[i * COLS_B + j] += A[i * COLS_A + k] * B[k * COLS_B + j];
            }
        }
    }
    return NULL;
}
#pragma GCC pop_options
