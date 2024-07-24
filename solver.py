from pyscipopt import Model, quicksum
from sys import argv
import numpy as np

def solve(mat):
    model = build_model(mat)
    model.hideOutput()
    model.optimize()
    return (model.getObjVal(), model.getSolvingTime())

def build_model(mat):
    model = Model()
    x = [model.addVar(name=f"x_{j + 1}") for j in range(len(mat[0]))]
    model.setObjective(quicksum(x[j] for j in range(len(mat[0]))), sense="minimize")
    for row in mat:
        model.addCons(quicksum(int(val) * x[j] for j, val in enumerate(row)) >= 1)
    return model

def main(argv):
    if len(argv) != 2:
        print("Usage: $ python3 solver.py <path/to/dataset/instance/mat.dat>");
        exit(0)
    else:
        mat = np.array([[int(ch) for ch in row] for row in np.loadtxt(argv[1], dtype=str)])
        obj, tm = solve(mat)
        print(f"Optimal Value: {obj}, Solving Time: {tm}") 

if __name__ == "__main__":
   main(argv)
