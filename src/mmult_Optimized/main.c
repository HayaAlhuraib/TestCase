/* Standard C includes */
#define _GNU_SOURCE
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sched.h>
#include <stdbool.h>
#include <inttypes.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

/* Include all implementations declarations */
#include "impl/naive.h"
#include "impl/opt.h"  // Include optimized implementation

/* Include common headers */
#include "common/types.h"
#include "common/macros.h"

/* Include application-specific headers */
#include "include/types.h"
const int ROWS_A = 25000; // Number of rows in Matrix A
const int COLS_A = 30000; // Number of columns in Matrix A
const int COLS_B = 20000; // Number of columns in Matrix B
/* Helper function to print a matrix */
void print_matrix(const char* name, float* matrix, size_t rows, size_t cols) {
    printf("%s:\n", name);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            printf("%.2f ", matrix[i * cols + j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char** argv) {
    /* Set the buffer for printf to NULL */
    setbuf(stdout, NULL);

    /* Arguments */
    int nthreads = 1;
    int cpu      = 0;

    int nruns    = 10000;
    int nstdevs  = 3;

    /* Data */
    int data_size = SIZE_DATA; // Holds the size passed to the program

    /* Parse arguments */
    void* (*impl_scalar_naive_ptr)(void* args) = impl_scalar_naive;
    void* (*impl_scalar_opt_ptr)(void* args) = impl_scalar_opt;

    /* Chosen */
    void* (*impl)(void* args) = NULL;
    const char* impl_str      = NULL;

    bool help = false;
    for (int i = 1; i < argc; i++) {
        /* Implementations */
        if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--impl") == 0) {
            assert(++i < argc);
            if (strcmp(argv[i], "naive") == 0) {
                impl = impl_scalar_naive_ptr; impl_str = "scalar_naive";
            } else if (strcmp(argv[i], "opt") == 0) {
                impl = impl_scalar_opt_ptr; impl_str = "scalar_opt";
            } else {
                impl = NULL; impl_str = "unknown";
            }
            continue;
        }

        /* Input/output data size */
        if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--size") == 0) {
            assert(++i < argc); // Ensure an argument follows the flag
            data_size = atoi(argv[i]); // Parse size as an integer
            continue;
        }
    }

    if (help || impl == NULL) {
        printf("Usage: %s {-i | --impl} naive|opt [Options]\n", argv[0]);
        printf("  Options:\n");
        printf("    -h | --help      Print this message\n");
        printf("    -s | --size      Size of input matrices (default = %d)\n", SIZE_DATA);
        exit(help ? 0 : 1);
    }

    /* Initialize matrices */
    size_t size = data_size;  // Square matrix dimensions
    srand((unsigned int)time(NULL)); // Seed the random number generator

   float* A = malloc(ROWS_A * COLS_A * sizeof(float)); // Matrix A
    float* B = malloc(COLS_A * COLS_B * sizeof(float)); // Matrix B
    float* R = malloc(ROWS_A * COLS_B * sizeof(float)); // Result matrix R

       if (!A || !B || !R) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    /* Set A and B inside input */
    float* A = input;                           // A starts at the beginning
    float* B = input + size * size;             // B starts right after A

    /* Initialize input matrices A and B */
    for (size_t i = 0; i < ROWS_A * COLS_A; i++) {
        A[i] = (float)(rand() % 10); // Random numbers between 0 and 9
    }
    for (size_t i = 0; i < COLS_A * COLS_B; i++) {
        B[i] = (float)(rand() % 10); // Random numbers between 0 and 9
    }


    /* Print input matrices */
    print_matrix("Matrix A", A, ROWS_A, COLS_A);
    print_matrix("Matrix B", B, COLS_A, COLS_B);

    
    /* Compare Naive and Optimized Implementations */
    float* R_naive = malloc(ROWS_A * COLS_B * sizeof(float));
    args_t args_naive = { .input = A, .output = R_naive, .size = ROWS_A * COLS_A };

    clock_t start = clock();
    impl_scalar_naive(&args_naive);
    clock_t end = clock();
    double naive_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Naive Runtime: %.6f seconds\n", naive_time);
    print_matrix("Result Matrix R (Naive)", R_naive, ROWS_A, COLS_B);

    args_t args_opt = { .input = A, .output = R, .size = ROWS_A * COLS_A };

    start = clock();
    impl_scalar_opt(&args_opt);
    end = clock();
    double opt_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Optimized Runtime: %.6f seconds\n", opt_time);
    print_matrix("Result Matrix R (Optimized)", R, ROWS_A, COLS_B);

    printf("Speedup: %.2fx\n", naive_time / opt_time);

    /* Free memory */
    free(A);
    free(B);
    free(R);
    free(R_naive);

    return 0;
}