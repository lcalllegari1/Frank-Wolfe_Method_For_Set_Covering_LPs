import random
from sys import argv as args
import numpy as np

def generate(M, N, P):
    mat = np.random.choice([0, 1], size=(M, N), p=[1 - P, P])

    for row in range(M):
        mat[row][random.randint(0, N - 1)] = 1
    for col in range(N):
        mat[random.randint(0, M - 1)][col] = 1
    
    return mat

def read(filepath):
    mat = []
    with open(filepath, "r") as file:
        for row in file:
            mat.append(row.split())
    return mat

def write(filepath, mat):
    with open(filepath, "w") as file:
        for row in mat:
            file.write(' '.join(map(str, row)) + '\n')

def main(M, N, P, filename="dataset.txt"):
    write(f"datasets/{filename}", generate(M, N, P))

if __name__ == "__main__":
    if len(args) != 4 and len(args) != 5:
        print(f"Usage: $ python3 {args[0]} <M> <N> <P> [<filename>]")
        exit(-1)
    
    M = int(args[1])
    N = int(args[2])
    P = float(args[3])
    if len(args) == 5:
        main(M, N, P, args[4])
    else:
        main(M, N, P)

