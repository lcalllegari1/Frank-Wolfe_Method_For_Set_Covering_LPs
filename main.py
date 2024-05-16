import subprocess
from sys import argv as args
import dataset
from solver import solve

PATH = "datasets/"
K = 1000

def main(args):
    if len(args) != 2 and len(args) != 4 and len(args) != 5:
        print("Usages:")
        print(f"$ python3 {args[0]} <filename>")
        print(f"$ python3 {args[0]} <M> <N> <P> [<filename>]")
        exit(-1)
    elif len(args) == 2:
        filename = args[1]
        mat = dataset.read(PATH + filename)
    else:
        if len(args) == 5:
            filename = args[4]
        else:
            filename = "random.txt"
        mat = dataset.generate(int(args[1]), int(args[2]), float(args[3]))
        dataset.write(PATH + filename, mat)
    
    objective, time = solve(mat)
    print("======================================================")
    print(f"Dataset (M = {len(mat)}, N = {len(mat[0])}, K = {K}): {filename}")
    print("======================================================")
    print(f"Optimal Value (Simplex Method): {objective}")
    print(f"Solving Time  (Simplex Method): {time}")
    print(f"------------------------------------------------------")

    subprocess.call(["gcc", "main.c", "-o", "main", "-Ofast"])
    subprocess.call(["./main", PATH + filename, str(K)])

    print("======================================================")

if __name__ == "__main__":
    main(args)
