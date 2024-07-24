#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ROWS 10000
#define MAX_COLS 10000
#define MAX_SIZE MAX_ROWS * MAX_COLS

typedef struct {
    uint32_t *row_indexes;
    uint32_t *col_indexes;
} csr_matrix;

uint32_t M = MAX_ROWS; /* number of rows */
uint32_t N = MAX_COLS; /* number of cols */

uint32_t NNZ = MAX_SIZE; /* number of non-zero elements */

uint32_t a_row_indexes[MAX_ROWS], t_row_indexes[MAX_COLS];
uint32_t a_col_indexes[MAX_SIZE], t_col_indexes[MAX_SIZE];

csr_matrix a, t;

csr_matrix * const A = &a;
csr_matrix * const T = &t;

void initialize() {
    A->row_indexes = a_row_indexes;
    A->col_indexes = a_col_indexes;
    T->row_indexes = t_row_indexes;
    T->col_indexes = t_col_indexes;
}

void read(const char * const path) {
    FILE *file = fopen(path, "r");

    if (file == NULL) {
        exit(-1);
    }

    int t;

    initialize();
    t = fscanf(file, "%d %d %d", &M, &N, &NNZ); 

    for (int i = 0; i <= M; i++) {
        t = fscanf(file, "%d", A->row_indexes + i);
    }
    for (int i = 0; i < NNZ; i++) {
        t = fscanf(file, "%d", A->col_indexes + i);
    }

    for (int i = 0; i <= N; i++) {
        t = fscanf(file, "%d", T->row_indexes + i);
    }
    for (int i = 0; i < NNZ; i++) {
        t = fscanf(file, "%d", T->col_indexes + i);
    }

    fclose(file);
}

void start_u(double * const u) {
    for (int i = 0; i < M; i++) {
        u[i] = 1;
    }
}

double L(const double * const u, uint8_t * const x) {
    double objective = 0;
    
    for (int j = 0; j < M; j++) {
        objective += u[j];
    }

    for (int i = 0; i < N; i++) {
        const int start = T->row_indexes[i];
        const int end = T->row_indexes[i + 1];
        double result = 1;
        for (int j = start; j < end; j++) {
            result -= u[T->col_indexes[j]];
        }
        if (x[i] = (result < 0)) {
            objective += result;
        }
    }

    return objective;
}

void subgradient(const uint8_t * const x, int32_t * const s) {
    for (int i = 0; i < M; i++) {
        const int start = A->row_indexes[i];
        const int end = A->row_indexes[i + 1];
        s[i] = 1;
        for (int j = start; j < end; j++) {
            s[i] -= x[A->col_indexes[j]];
        }
    }
}

void argmax(const int32_t * const s, uint8_t * const d) {
    for (int i = 0; i < M; i++) {
        d[i] = (s[i] > 0);
    }
}

void next_u(double * const u, const uint8_t * const d, double gamma) {
    for (int i = 0; i < M; i++) {
        u[i] += gamma * (d[i] - u[i]);
    }
}

void update_lower_bound(double * const lower_bound, double candidate) {
    if (candidate > *lower_bound) {
        *lower_bound = candidate;
    }
}

void update_upper_bound(
    double * const upper_bound,
    double objective, 
    const int32_t * const s,
    const uint8_t * const d,
    const double * const u
) {
    double candidate = objective;

    for (int i = 0; i < M; i++) {
        candidate += s[i] * (d[i] - u[i]);
    }

    if (candidate < *upper_bound) {
        *upper_bound = candidate;
    }
}

int main(int argc, char **argv) {
    char *path = argv[1];
    int K = atoi(argv[2]);
    read(path);

    double objective, lower_bound = 0, upper_bound = 1e9;
    double u[M];
    uint8_t x[N], d[M];
    int32_t s[M];

    start_u(u);
    clock_t start = clock(), end;
    for (int k = 0; k < K; k++) {
        objective = L(u, x);
        subgradient(x, s);
        argmax(s, d);
        update_lower_bound(&lower_bound, objective);
        update_upper_bound(&upper_bound, objective, s, d, u);
        next_u(u, d, 2 / ((double) k + 2));
    }
    end = clock();

    printf("Lower Bound Value (Frank-Wolfe): %f\n", lower_bound);
    printf("Upper Bound Value (Frank-Wolfe): %f\n", upper_bound);
    printf("Gap               (Frank-Wolfe); %f\n", (upper_bound - lower_bound) / upper_bound * 100);
    printf("Solving Time      (Frank-Wolfe): %f\n", ((double) (end - start)) / CLOCKS_PER_SEC);
}
