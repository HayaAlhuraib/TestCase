#ifndef __INCLUDE_TYPES_H_
#define __INCLUDE_TYPES_H_

#include <stddef.h>  // For size_t

// Define the argument structure
typedef struct {
    void* input;       // Pointer to the input data (e.g., matrices A and B as floats)
    void* output;      // Pointer to the output data (e.g., result matrix R as floats)
    size_t size;       // Dimension size (used to define M, N, P)
    int cpu;           // CPU core to execute the benchmark (optional)
    int nthreads;      // Number of threads to use (optional for parallel implementation)
} args_t;
extern const int ROWS_A; // Number of rows in Matrix A
extern const int COLS_A; // Number of columns in Matrix A
extern const int COLS_B; // Number of columns in Matrix B
#endif // __INCLUDE_TYPES_H_
