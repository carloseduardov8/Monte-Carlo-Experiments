import numpy as np
from scipy.linalg import eig

p_0 = 0.7
p_1 = 0.5
p_2 = 0.3

A = np.array([[2*p_0*(1-p_0)+((1-p_0)**2), p_0**2, 0, 0, 0, 0],
	[0, (1-p_1)**2, p_1**2, 0, 2*p_1*(1-p_1), 0],
	[0, 0, (p_2**2)+((1-p_2)**2), 2*p_2*(1-p_2), 0, 0],
	[p_2*(1-p_0), 0, 0, p_0*(1-p_2)+(1-p_0)*(1-p_2), 0, p_0*p_2],
	[p_1*(1-p_0), 0, 0, 0, p_0*(1-p_1)+(1-p_0)*(1-p_1), p_0*p_1],
[0, 0, p_1*p_2, p_1*(1-p_2), p_2*(1-p_1), (1-p_1)*(1-p_2)]])

values, left, right = eig(A, left=True)

for i in range(len(values)):
    print("Left eigenvector for eigenvalue {}:".format(values[i]))
    print(left[:,i])
    print()
