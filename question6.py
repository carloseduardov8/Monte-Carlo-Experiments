import numpy as np
import math
from scipy.integrate import quad

# Parameters:
alpha = 3
a = 0
b = 4

# Integrand:
def f(x):
	return x**alpha

# Samples from distribution h(x,y):
def sample_h():
	x = np.random.uniform(a,b)
	y = np.random.uniform(0,f(b))
	if (y <= f(x)):
		return 1
	else:
		return 0

# Iterates over requested values of n:
for k in range(7):

	n = 10**k

	accumulator = 0

	# Samples from h(x,y) a total of n times:
	for i in range(n):
		accumulator += sample_h()

	# Calculates sample mean:
	result = accumulator*1.0 / n

	# Multiplies by area of rectangle [a,b]x[f(a),f(b)]:
	rect_area = (b-a)*f(b)
	result = result * rect_area

	# Calculates relative error and prints it:
	real_value = quad(f, a, b)[0]
	rel_error = math.fabs(result-real_value)/real_value
	print("("+str(n)+","+str(rel_error)+")")
