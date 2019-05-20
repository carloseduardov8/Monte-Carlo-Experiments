import numpy as np
import math

n_ring = 1023	# Total nodes will be n_ring
n_tree = 10		# Total nodes will be 2**n_tree - 1
n_grid = 32		# Total nodes will be n_grid * n_grid


def create_ring():
	matrix = np.zeros((n_ring,n_ring))
	pi = np.zeros((n_ring))
	for i in range(n_ring):
		pi[i] = 1.0/n_ring
		for j in range(n_ring):
			if abs(i-j) == 1:
				matrix[i][j] = 0.25
			elif abs(i-j) == n_ring-1:
				matrix[i][j] = 0.25
			elif abs(i-j) == 0:
				matrix[i][j] = 0.5
			else:
				matrix[i][j] = 0
	return matrix, pi, n_ring

def create_tree():
	# Solves system to calculate stationary distribution:
	A = np.array([ [2**(n_tree-1), 1, 2**n_tree-2-(2**(n_tree-1))], [3,0,-1], [0,3,-2] ])
	b = np.array([1,0,0])
	pi_values = np.linalg.solve(A,b)
	# Calculates probabilities and assigns stationary distribution values:
	nodes = (2**n_tree) - 1
	matrix = np.zeros((nodes,nodes))
	pi = np.zeros((nodes))
	for i in range(nodes):
		# Faz o self-loop:
		matrix[i][i] = 0.5
		# Raiz:
		if i == 0:
			matrix[i][1] = 0.25
			matrix[i][2] = 0.25
			#pi[i] = 1.0/1022
			pi[i] = pi_values[1]
		# Folhas:
		elif i > (2**(n_tree-1)) - 2:
			father = math.floor((i+1)/2)
			matrix[i][father-1] = 0.5
			#pi[i] = 1.0/2044
			pi[i] = pi_values[0]
		# Vertices intermediarios:
		else:
			father = math.floor((i+1)/2)
			firstChild = (i+1)*2
			secondChild = ((i+1)*2) + 1
			matrix[i][father-1] = 1.0/6
			matrix[i][firstChild-1] = 1.0/6
			matrix[i][secondChild-1] = 1.0/6
			#pi[i] = 3.0/2044
			pi[i] = pi_values[2]
	print(pi)
	return matrix, pi, nodes


def create_grid():
	# Solves system to calculate stationary distribution:
	A = np.array([ [4, 4*(n_grid-2), n_grid*n_grid - 4 - 4*(n_grid-2)], [3,-2,0], [0,4,-3] ])
	b = np.array([1,0,0])
	pi_values = np.linalg.solve(A,b)
	# Calculates probabilities and assigns stationary distribution values:
	nodes = n_grid*n_grid
	matrix = np.zeros((nodes,nodes))
	pi = np.zeros((nodes))
	for i in range(nodes):
		# Faz o self-loop:
		matrix[i][i] = 0.5
		# Caso em que i é quina:
		if i == 0:
			matrix[i][1] = 0.25
			matrix[i][n_grid] = 0.25
			#pi[i] = 1.0/1984
			pi[i] = pi_values[0]
		elif i == n_grid-1:
			matrix[i][n_grid-2] = 0.25
			matrix[i][(2*n_grid)-1] = 0.25
			#pi[i] = 1.0/1984
			pi[i] = pi_values[0]
		elif i == ((n_grid*n_grid) - n_grid):
			matrix[i][((n_grid*n_grid) - n_grid)+1] = 0.25
			matrix[i][((n_grid*n_grid) - 2*n_grid)] = 0.25
			#pi[i] = 1.0/1984
			pi[i] = pi_values[0]
		elif i == ((n_grid*n_grid) - 1):
			matrix[i][(n_grid*n_grid) - 2] = 0.25
			matrix[i][(n_grid*n_grid) - 1 - n_grid] = 0.25
			#pi[i] = 1.0/1984
			pi[i] = pi_values[0]
		# Casos em que i está nas fileiras das extremidades:
		elif i > 0 and i < n_grid-1:
			matrix[i][i-1] = 1.0/6
			matrix[i][i+1] = 1.0/6
			matrix[i][i+n_grid] = 1.0/6
			#pi[i] = 3.0/3968
			pi[i] = pi_values[1]
		elif i > ((n_grid*n_grid) - n_grid) and i < ((n_grid*n_grid) - 1):
			matrix[i][i-1] = 1.0/6
			matrix[i][i+1] = 1.0/6
			matrix[i][i-n_grid] = 1.0/6
			#pi[i] = 3.0/3968
			pi[i] = pi_values[1]
		elif i % n_grid == 0:
			matrix[i][i-n_grid] = 1.0/6
			matrix[i][i+n_grid] = 1.0/6
			matrix[i][i+1] = 1.0/6
			#pi[i] = 3.0/3968
			pi[i] = pi_values[1]
		elif (i+1) % n_grid == 0:
			matrix[i][i-n_grid] = 1.0/6
			matrix[i][i+n_grid] = 1.0/6
			matrix[i][i-1] = 1.0/6
			#pi[i] = 3.0/3968
			pi[i] = pi_values[1]
		# Caso em que i é nó intermediário:
		else:
			matrix[i][i-1] = 0.125
			matrix[i][i+1] = 0.125
			matrix[i][i-n_grid] = 0.125
			matrix[i][i+n_grid] = 0.125
			#pi[i] = 1.0/992
			pi[i] = pi_values[2]
	return matrix, pi, nodes

def total_distance(v1,v2):
	acc = 0
	for i in range(len(v1)):
		acc += abs(v1[i] - v2[i])
	return acc*1.0/2


###################
### QUESTION 3C ###
###################

def question3():
	matrix, pi, nodes = create_grid()
	pi_t = np.zeros((nodes))
	pi_t[0] = 1
	for i in range(100001):
		# Prints current total distance:
		if i in [0,10,100,1000,10000,30000,50000,70000,100000]:
			print("(", i, ",", total_distance(pi_t, pi), ")")
		# Matrix multiplication:
		mult = pi_t.dot(matrix)
		pi_t = mult
	print("Pi(t): ", pi_t)
	print("Pi: ", pi)

question3()
