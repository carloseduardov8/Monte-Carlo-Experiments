import numpy as np
import math

n = 10**6
N = 10**3

# Constant for h(i):
k2 = 5912.1281

accumulator = 0

# Monte Carlo:
for i in range(n):
	x = rejection_sampling()
	# Calculates g(x)/h(x):
	value = x*math.log(x)/(math.log(x)/k2)
	accumulator += value

answer = (accumulator/n)

print("Final result: " + str(answer))
print("Relative error: " + str((answer-3207332)/3207332))
