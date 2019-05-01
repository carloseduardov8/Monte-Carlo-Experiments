import numpy as np
import math

# Numero de experimentos
n = 10**7

# Parabola:
def f(x):
	return -(x**2) + 2

def sample_g():
	# Gera v.a.'s uniformes de 0 a 2:
	x = np.random.uniform(0,2)
	y = np.random.uniform(0,2)
	# Checa se ponto encontra-se abaixo da curva:
	if (y <= f(x)):
		return 1
	else:
		return 0

# MONTE CARLO
accumulator = 0
for i in range(n):
	accumulator += sample_g()

result = 3.0*accumulator/n
print(result, math.sqrt(2))
