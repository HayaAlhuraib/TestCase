/* Standard C includes */
#include <stdlib.h>

/* Include common headers */
#include "common/macros.h"
#include "common/types.h"

/* Include application-specific headers */
#include "include/types.h"


void* impl_scalar_naive(void* args) {
    args_t* a = (args_t*)args;
    const size_t ROWS_A = 25000; // Number of rows in Matrix A
    const size_t COLS_A = 30000; // Number of columns in Matrix A
    const size_t COLS_B = 20000; // Number of columns in Matrix B

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
