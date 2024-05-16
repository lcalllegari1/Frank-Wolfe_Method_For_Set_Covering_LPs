# Frank-Wolfe Method For Set-Covering LPs

## NOTES

In order for the code presented below to run, pyscipopt is needed. Anyway, the Frank-Wolfe algorithm can be tested separately, allowing its execution without pyscipopt. In order to test the Frank-Wolfe Method alone, you can use 

```
$ gcc main.c -o main -Ofast
$ ./main <filepath> <K>
```
where `filepath` is the path of the file that contains the matrix associated with the problem and `K` represents the number of iterations of the algorithm.

## USAGES
The following commands can be used to test the implementation. They provide a simple way to run both the Simplex Method (SCIP with pyscipopt) and my implementation of the Frank-Wolfe Method. 
```
$ python3 main.py <filename>
```
where `filename` must be a valid name for a file located in the `datasets` directory. This command
runs the algorithm on the matrix read from `filename`.

```
$ python3 main.py <M> <N> <P> [<filename>]
```

where `M` represents the number of items of the problem (i.e. the number of rows of the matrix), `N` represents the number of sets of the problem (i.e. the number of columns of the matrix) and `P` represents the probability of ones in the matrix associated with the problem. The optional argument `filename` can be used to specify the name of the file that will be used to store the matrix, so that you can refer to it using the first command. If `filename` is not provided, the default value `random.txt` will be used. This file is overwritten every time the command is invoked without the optional argument (or when the optional argument is exactly `random.txt`, of course).

## Frank-Wolfe Implementation (C)

For this (first) implementation I did not use dynamic memory allocation. All operations are made on the stack for maximum efficiency (I hope). For this reason, all functions used to implement the algorithm are void. All the values that are passed (by pointer) to those functions and that are modified inside the function (e.g. used as return values), are marked with a leading underscore character in order to distinguish them from the other parameters. 


