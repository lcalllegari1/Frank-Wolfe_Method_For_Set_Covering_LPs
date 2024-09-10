# Frank-Wolfe Method For Set-Covering LPs

# DATASET GENERATION

The file `datagen.py` can be used to generate datasets. The command 

```
python3 datagen.py <size> <rows> <cols> <sparsity>
```

can be used to generate a dataset with `<size>` instances, each one
referred to by a binary matrix with `<rows>` rows and `<cols>` columns. The
`<sparsity>` value represents the sparsity of the generated matrices.

# FRANK-WOLFE ALGORITHM

The file `solver.c` implements the Frank-Wolfe algorithm and can be used to
solve instances of a dataset. Use

```
gcc solver.c -o solver -Ofast
```

to compile the code and 

```
./solver <path/to/dataset/instance/csr_matrix.dat> <K>
```

to run the algorithm on the instance referred to by `csr.dat`. The value
`<K>` represents the number of iterations of the algorithm.

# SIMPLEX SOLVER

In order to compare the result obtained with FW, you can solve the same
instance using pyscipopt with the following command.

```
python3 solver.py <path/to/dataset/instance/csr_matrix.dat>
```

In order for this to work, make sure pyscipopt is available on your system.

## EXAMPLES

Use

```
python3 datagen.py 1 100 10 0.8
```

to generate a dataset of a single istance; that is, a binary matrix of
100 rows and 10 columns, with 0.8 sparsity.

The default behavior is to save the instance in `datasets/100x10/0.1/1`
using the CSR representation of the matrix.

Once the dataset has been generated, you can use 
```
gcc solver.c -o solver -Ofast
```

to compile the code and 

```
./solver datasets/100x10/0.1/1/csr_matrix.dat 1000
```

to run the Frank-Wolfe algorithm on the specified instance. Make sure you
refer to the `csr_matrix.dat` file.

You can then use 

```
python3 solver.py datasets/100x10/0.1/1/crs_matrix.dat 
```

to solve the same istance with the simplex algorithm to compare the results
and the running times. Make sure you refer to the `csr_matrix.dat` file.
