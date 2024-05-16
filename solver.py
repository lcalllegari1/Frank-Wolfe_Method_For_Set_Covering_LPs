from pyscipopt import Model, quicksum

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