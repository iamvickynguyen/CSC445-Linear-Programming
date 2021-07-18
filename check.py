import sys

def dotproduct(coef, var):
    dotprod = 0
    for i, val in enumerate(coef):
        dotprod += val * var[i]
    return dotprod

line = input()
obj = [float(x) for x in line.split()]

constraints = []
for line in sys.stdin:
    line = line.strip()
    constraints.append([float(x) for x in line.split()])

print("optitmal:", dotproduct(obj, constraints[-1]))
for i in range(len(constraints) - 1):
    lhs = dotproduct(constraints[i][:-1], constraints[-1])
    if lhs > constraints[i][-1]:
        print("wrong constraint at %d, constraint = %f, but got %f" %(i+1, constraints[i][-1], lhs))
