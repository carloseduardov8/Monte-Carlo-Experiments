import numpy as np
import math

a = 0.393

uniform = np.random.uniform()
exp = -math.log(uniform)/a

print(exp)
