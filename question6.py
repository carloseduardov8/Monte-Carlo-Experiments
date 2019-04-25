import numpy as np
import math

# Parameters:
alpha = 0.5
a = 1
b = 8
n = 10**5

def f(x):
	return x**alpha

def sample_h():
	x = np.random.uniform(a,b)
	y = np.random.uniform(0,f(b))
	if (y <= f(x)):
		return 1
	else:
		return 0

accumulator = 0

for i in range(n):
	accumulator += sample_h()

result = accumulator*1.0 / n
print("Result so far: " + str(result))

# Multiplies by area of rectangle [a,b]x[f(a),f(b)]:
rect_area = (b-a)*f(b)
print("Area of rect: " + str(rect_area))
result = result * rect_area

print("Area: " + str(result))
