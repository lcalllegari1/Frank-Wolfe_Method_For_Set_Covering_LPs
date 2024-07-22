from os import mkdir, makedirs
from sys import argv
from scipy.sparse import csr_matrix

import numpy as np


DATASET_DIR = "datasets"


def generate_matrix(rows, cols, prob):
    matrix = np.random.choice(
        a=[0, 1],
        size=(rows, cols),
        p=[1 - prob, prob]
    )

    while True: 
        null_cols = np.where(matrix.sum(axis=0) == 0)[0]
        null_rows = np.where(matrix.sum(axis=1) == 0)[0]

        if null_cols.size == 0 and null_rows.size == 0:
            break;

        if null_rows.size > 0:
            matrix[null_rows, :] = np.random.choice(
                a=[0, 1],
                size=(null_rows.size, cols),
                p=[1 - prob, prob]
            )
        if null_cols.size > 0:
            matrix[:, null_cols] = np.random.choice(
                a=[0, 1],
                size=(rows, null_cols.size),
                p=[1 - prob, prob]
            )
    
    return matrix

# create folders

def generate_dataset(size, rows, cols, prob):
    path = f"{DATASET_DIR}/{rows}x{cols}/{prob}/"

    for i in range(size):
        dir_name = f"{i + 1}"
        makedirs(path + dir_name, exist_ok=True)
        matrix = generate_matrix(rows, cols, prob)
        np.savetxt(path + dir_name + "/mat.dat", matrix, "%d", '')
        save_csr(matrix, path + dir_name)

def save_csr(matrix, path):
    rows, cols = matrix.shape
    csr_m = csr_matrix(matrix)
    csr_t = csr_matrix(matrix.transpose())
    with open(path + "/csr.dat", "w") as file:
        file.write(f"{rows} {cols} {csr_m.indices.size}\n")
        file.write(" ".join(map(str, csr_m.indptr)) + "\n")
        file.write(" ".join(map(str, csr_m.indices)) + "\n")
        file.write(" ".join(map(str, csr_t.indptr)) + "\n")
        file.write(" ".join(map(str, csr_t.indices)) + "\n")


def main(args):
    if len(args) != 5:
        print(f"Usage: $ python3 {args[0]} <size> <rows> <cols> <prob>")
    else:
        generate_dataset(int(args[1]), int(args[2]), int(args[3]), float(args[4]))


if __name__ == "__main__":
    main(argv)
