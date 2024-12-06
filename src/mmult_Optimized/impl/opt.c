/* Standard C includes */
#include <stdlib.h>

/* Include common headers */
#include "common/macros.h"
#include "common/types.h"

/* Include application-specific headers */
#include "include/types.h"

/* Alternative Implementation */
#pragma GCC push_options
#pragma GCC optimize ("O1")
void* impl_scalar_opt(void* args) {
    /* Extract arguments */
    args_t* arguments = (args_t*)args;

    // Define the dimensions of the matrices
    const size_t ROWS_A = 25000; // Number of rows in Matrix A
    const size_t COLS_A = 30000; // Number of columns in Matrix A
    const size_t COLS_B = 20000; // Number of columns in Matrix B

    float* A = (float*)arguments->input; // Matrix A
    float* B = (float*)(arguments->input + ROWS_A * COLS_A * sizeof(float)); // Matrix B
    float* R = (float*)arguments->output; // Result matrix R

    /* Set block size (tunable parameter) */
    size_t block_size = 64; // A typical value for cache optimization (adjust if necessary)

    /* Initialize Result Matrix */
    for (size_t i = 0; i < ROWS_A; i++) {
        for (size_t j = 0; j < COLS_B; j++) {
            R[i * COLS_B + j] = 0.0f; // Initialize all elements of R to zero
        }
    }

    /* Perform Blocked Matrix Multiplication */
    for (size_t ii = 0; ii < ROWS_A; ii += block_size) {
        for (size_t jj = 0; jj < COLS_B; jj += block_size) {
            for (size_t kk = 0; kk < COLS_A; kk += block_size) {
                /* Multiply blocks */
                for (size_t i = ii; i < ii + block_size && i < ROWS_A; i++) {
                    for (size_t j = jj; j < jj + block_size && j < COLS_B; j++) {
                        float sum = R[i * COLS_B + j];
                        for (size_t k = kk; k < kk + block_size && k < COLS_A; k++) {
                            sum += A[i * COLS_A + k] * B[k * COLS_B + j];
                        }
                        R[i * COLS_B + j] = sum;
                    }
                }
            }
        }
    }

    return NULL;
}
#pragma GCC pop_options