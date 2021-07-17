coef = [float(x) for x in input().split()]
res = [float(x) for x in input().split()]
dotprod = 0
for i, val in enumerate(coef):
    dotprod += val * res[i]
print(dotprod)