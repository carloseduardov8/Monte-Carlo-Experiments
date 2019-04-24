import numpy as np
import math

n = 10**3
N = 10**3

# Constant for h(i):
k2 = 5912.1281

# Rejection sampling so we can generate samples with distribution h(i):
def rejection_sampling():
	# Generates uniform:
	y = np.random.uniform()
	# Calculates envolope (using y=7*x*(x+1)/(2*k1)), i.e. use y=7x as envelope
	k1 = 3503500
	x = math.ceil(0.5*(-1 + math.sqrt(4004000*y+1)))
	# Accepts with given probability:
	prob = y - math.log(x)/k2
	print(prob,x)
	if (np.random.binomial(1,prob) == 1):
		return x
	else:
		return rejection_sampling()

accumulator = 0

# Monte Carlo:
for i in range(n):
	x = rejection_sampling()
	# Calculates g(x)/h(x):
	value = x*math.log(x)/(math.log(x)/k2)
	accumulator += value

answer = (accumulator/n)*N

print("Final result: " + str(answer))
