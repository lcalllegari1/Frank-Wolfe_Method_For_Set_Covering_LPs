#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define MAX_SIZE 1000000

uint32_t M = 0, N = 0;

void read(char *filepath, uint8_t *_buf);
void print(uint8_t *mat, uint32_t rows, uint32_t cols);

void transpose(uint8_t *mat, uint8_t *_buf);
void compute_starting_point(double *_multiplier);
double compute_lagrangian(uint8_t *mat, double *multiplier, uint8_t *_buf);
void compute_subgradient(uint8_t *mat, uint8_t *vec, int32_t *_buf);
void compute_argmax(int32_t *subgradient, uint8_t *_buf);
void compute_next(double *_current, uint8_t *target, double gamma);
void update_lower_bound(double *_lower_bound, double candidate);
void update_upper_bound(double *_upper_bound, double current_objective, int32_t *subgradient, uint8_t *arg, double *multiplier);

int main(int argc, char **argv) {
    char *filepath = argv[1];
    uint32_t K = atoi(argv[2]);
    uint8_t A[MAX_SIZE];
    read(filepath, A);

    uint8_t T[N * M], x[N], arg[M]; int32_t subgradient[M];
    double multiplier[M], current_objective = 0, lower_bound = 0, upper_bound = 1e9;

    transpose(A, T);
    compute_starting_point(multiplier);

    clock_t start = clock(), end;
    for (int k = 0; k < K; k++) {
        current_objective = compute_lagrangian(T, multiplier, x);
        compute_subgradient(A, x, subgradient);
        compute_argmax(subgradient, arg);
        update_lower_bound(&lower_bound, current_objective);
        update_upper_bound(&upper_bound, current_objective, subgradient, arg, multiplier);
        compute_next(multiplier, arg, 2 / (((double) k) + 2));
    }
    end = clock();

    printf("Lower Bound Value (Frank-Wolfe): %f\n", lower_bound);
    printf("Upper Bound Value (Frank-Wolfe): %f\n", upper_bound);
    printf("Gap               (Frank-Wolfe); %f\n", (upper_bound - lower_bound) / upper_bound * 100);
    printf("Solving Time      (Frank-Wolfe): %f\n", ((double) (end - start)) / CLOCKS_PER_SEC);

    return 0;
}

void read(char *filepath, uint8_t *_buf) {
    FILE *file = fopen(filepath, "r");

    if (file == NULL) {
        printf("Could not open the specified file.");
        exit(-1);
    }

    uint32_t ch, i = 0;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == ' ') {
            continue;
        } else if (ch == '\n') {
            M++;
        } else {
            _buf[i++] = (ch % 2);
        }
    }
    N = i / M;
}

void print(uint8_t *mat, uint32_t rows, uint32_t cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", mat[i * cols + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void transpose(uint8_t *mat, uint8_t *_buf) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            _buf[j * M + i] = mat[i * N + j];
        }
    }
}

void compute_starting_point(double *_multiplier) {
    for (int i = 0; i < M; i++) {
        _multiplier[i] = 1;
    }
}

double compute_lagrangian(uint8_t *mat, double *multiplier, uint8_t *_buf) {
    double result[N], objective = 0;
    for (int i = 0; i < N; i++) {
        result[i] = 1;
        for (int j = 0; j < M; j++) {
            result[i] -= mat[i * M + j] * multiplier[j];
        }
    }

    for (int i = 0; i < N; i++) {
        if (_buf[i] = (result[i] < 0)) {
            objective += result[i];
        }
    }
    for (int i = 0; i < M; i++) {
        objective += multiplier[i];
    }
    return objective;
}

void compute_subgradient(uint8_t *mat, uint8_t *vec, int32_t *_buf) {
    for (int i = 0; i < M; i++) {
        _buf[i] = 1;
        for (int j = 0; j < N; j++) {
            _buf[i] -= mat[i * N + j] & vec[j];
        }
    }
}

void compute_argmax(int32_t *subgradient, uint8_t *_buf) {
    for (int i = 0; i < M; i++) {
        _buf[i] = (subgradient[i] > 0);
    }
}

void compute_next(double *_current, uint8_t *target, double gamma) {
    for (int i = 0; i < M; i++) {
        _current[i] += gamma * (target[i] - _current[i]);
    }
}

void update_lower_bound(double *_lower_bound, double candidate) {
    if (candidate > *_lower_bound) {
        *_lower_bound = candidate;
    }
}

void update_upper_bound(double *_upper_bound, double current_objective, int32_t *subgradient, uint8_t *arg, double *multiplier) {
    double candidate = current_objective;
    for (int i = 0; i < M; i++) {
        candidate += subgradient[i] * (arg[i] - multiplier[i]);
    }
    if (candidate < *_upper_bound) {
        *_upper_bound = candidate;
    }
}