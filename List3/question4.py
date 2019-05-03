import numpy as np
import math

def prob(x):
	return math.exp(((-x**2)/2) + x - 0.5)

while (True):
	exp = np.random.exponential()
	if (np.random.binomial(n=1, p=prob(exp)) == 1):
		if (np.random.binomial(n=1, p=0.5) == 1):
			print(exp)
		else:
			print(-exp)
		break
