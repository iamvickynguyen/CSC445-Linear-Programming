# CSC445 Linear Programming

This is a linear optimization solver, which uses Simplex method.

## How to run
`make` : compile

`./program` : run program

`./runtests.sh` : run all tests in tests folder, outputs will be in tests/output folder

`make clean` : remove `.o` files and output test files

## Solver architecture
This Simplex uses a 2D vector `LP`, which represents a dictionary. `vector<string> basic` contains labels of basic variables and `vector<string> nonbasic` contains labels of non basic variables.

The logic is:

- If the inital dictionary is infeasible:
    - If its dual is feasible:
        - Convert the primal dictionary into the dual dictionary and solve it
    - Otherwise:
        - Use Primal-Dual method to find a feasible point
        - If no feasible point, the LP is infeasible

- Once the dictionary is feasible, use Largest Coefficient rule to perform some pivots. In order to avoid cycling, the solver uses Lexicographic method.
- While pivoting, if the dictionary is unbounded and we are using the primal, then the LP is unbounded. If the dictionary is unbounded and we are using the dual, then the LP is infeasible.
- If we reach optimal solution, output result.

## Extra features
1. Primal-Dual Methods \
Use a 2 phase primal-dual method to make an LP feasible, if both of the initial primal and dual are infeasible. \
Function `feasiblize()` set all of the coefficients in the objective row be 0. Then convert the LP into the dual. Use Largest Coefficient rule to do pivoting until the dictionary is optimal. If the dictionary is unbounded while pivoting, output "infeasible". If the dictionary is optimal, convert back to the primal, then reconstruct the objective row by doing some substutions.

2. Alternate Cycling Avoidance \
Function `largest_coefficient_rule()` finds the largest coefficient in the non basic row. The corresponding variable will be the entering variable. Then it finds all the basic rows that have minimum bound for leaving variable. If there is no such bound, output "unbounded" if we are solving the primal, or "infeasible" if we are solving the dual. If there is 1 candidate variable, it is the leaving variable. If there are more than 1 variable, then sort all the vectors corresponding to candidate leaving variables, and take the smallest one. All the vectors are normalized by dividing the coefficients by the negation of the candidate leaving variable's coefficient on the same row. \
For example, suppose we have

|      |   |   |x1 |x2 |x3 |
|------|---|---|---|---|---|
|      |   |0  |1  |3  |-2 |
|row 1 |w1 |2  |0  |-1 |6  |
|row 2 |w2 |4  |5  |6  |7  |
|row 3 |w3 |4  |0  |-2 |-2 |

row 1 : (2/1, 0/1, -1/1, 6/1) = (2, 0, -1, 6) \
row 3 : (4/2, 0/2, -2/2, -2/2) = (2, 0, -1, -1)

row 3 < row 1, so w3 leaves, x2 enters.

## Notes

The test cases are provided by Dr. William H. Bird (aka Bill Bird)
