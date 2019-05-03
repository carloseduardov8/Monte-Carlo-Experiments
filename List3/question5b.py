import numpy as np
import math

n = 10**6
N = 10**3

# Constant for h(i):
k2 = 5912.1281

# Rejection sampling so we can generate samples with distribution h(i):
def rejection_sampling():
	# Uses c.g(x) (where g(x) is uniform) as envelope function:
	c = 1.2
	x = math.ceil(np.random.uniform(0,N))
	# Accepts with given probability:
	prob = (math.log(x)/k2)/(c/N)
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

answer = (accumulator/n)

print("Final result: " + str(answer))
print("Relative error: " + str((answer-3207332)/3207332))
