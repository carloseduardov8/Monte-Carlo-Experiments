import numpy
import math
import scipy.stats

tail = 5
n = 10**6

# Pdf of bernoulli random variable:
def g(x):
	if (x >= tail):
		return 1
	else:
		return 0

# Pdf of normal distribution:
def f(x):
	return scipy.stats.norm(0, 1).pdf(x)

# Pdf of exponential:
def h(x):
	return math.exp(-x)

# Sample from exponential:
def sample_h():
	return numpy.random.exponential(scale=1)

# Sample from normal distribution:
def sample_f():
	return numpy.random.normal(0, 1)

# MAIN PROGRAM

# Naive Sampling:
accumulator = 0
for i in range(n):
	x = sample_f()
	naive = g(x)
	accumulator += naive

result = accumulator*1.0/n
print("MC Naive Sampling: " + str(result))


# Importance Sampling:
accumulator = 0
for i in range(n):
	x = sample_h()
	imp = g(x)*f(x)/h(x)
	accumulator += imp

result = accumulator*1.0/n
print("MC Importance Sampling: " + str(result))
print("Correct value: " + str(1-scipy.stats.norm.cdf(tail)))
