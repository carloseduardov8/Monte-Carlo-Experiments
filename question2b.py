import numpy as np

x_m = 1
a = 0.393

uniform = np.random.uniform()
pareto = x_m/(uniform**(1/a))

print(pareto)
