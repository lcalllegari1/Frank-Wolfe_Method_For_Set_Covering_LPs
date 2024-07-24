# Frank-Wolfe Method For Set-Covering LPs

# DATASET GENERATION

The file `datagen.py` can be used to generate datasets. The command 

```
python3 datagen.py <size> <rows> <cols> <prob>
```

can be used to generate a dataset with `<size>` instances, each one
referring to a binary matrix with `<rows>` rows and `<cols>` columns. The
`<prob>` value represents the probability of ones, and should be a value
between 0 and 1. For the Set-Covering problem is common to use sparse
matrices, i.e. matrices with a lot of zero entries.

# FRANK-WOLFE ALGORITHM

The file `solver.c` implements the Frank-Wolfe algorithm and can be used to
solve instances of a dataset. Use

```
gcc solver.c -o solver -Ofast
```

to compile the code and 

```
./solver <path/to/dataset/instance/csr.dat> <K>
```

to run the algorithm on the instance referred to by `csr.dat`. The value
`<K>` represents the number of iterations of the algorithm. 100-1000 is
reasonable, +10000 not so much in terms of what is gained.

# SIMPLEX SOLVER

In order to compare the result obtained with FW, you can solve the same
instance using pyscipopt with the following command.

```
python3 solver.py <path/to/dataset/instance/mat.dat>
```

In order for this to work, make sure pyscipopt is available on your system.

## EXAMPLES

Use

```
python3 datagen.py 1 100 10 0.1 
```

to generate a dataset of a single (1) istance, that is, a binary matrix of
100 rows and 10 columns, with a 0.1 probability of generating ones in the
process of costructing the matrix.

The default behavior is to save the instance in `datasets/100x10/0.1/1`.
The folder contains two files, `mat.dat` and `csr.dat`. The first one is
the binary matrix itself while the second is its CSR (Compressed Sparse
Row) representation. The matrix file `mat.dat` is used for the simplex
solver, while the other one serves the Frank-Wolfe algorithm.

Once the dataset has been generated, you can use 
```
gcc solver.c -o solver -Ofast
```

to compile the code and 

```
./solver datasets/100x10/0.1/1/csr.dat 1000
```

to run the Frank-Wolfe algorithm on the specified instance. Make sure you
refer to the `csr.dat` file.

You can then use 

```
python3 solver.py datasets/100x10/0.1/1/mat.dat 
```

to solve the same istance with the simplex algorithm to compare the results
and the running times. Make sure you refer to the `mat.dat` file.
