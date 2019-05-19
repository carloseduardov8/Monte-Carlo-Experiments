import numpy as np
import math

n_ring = 1023
n_tree = 1023
n_grid = 1024


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
	return matrix, pi

def create_tree():
	matrix = np.zeros((n_tree,n_tree))
	pi = np.zeros((n_tree))
	for i in range(n_tree):
		# Faz o self-loop:
		matrix[i][i] = 0.5
		# Raiz:
		if i == 0:
			matrix[i][1] = 0.25
			matrix[i][2] = 0.25
			pi[i] = 0
		# Folhas:
		elif i > 510:
			father = math.floor((i+1)/2)
			matrix[i][father-1] = 0.5
			pi[i] = 0
		# Vertices intermediarios:
		else:
			father = math.floor((i+1)/2)
			firstChild = (i+1)*2
			secondChild = ((i+1)*2) + 1
			matrix[i][father-1] = 1.0/6
			matrix[i][firstChild-1] = 1.0/6
			matrix[i][secondChild-1] = 1.0/6
			pi[i] = 0
	return matrix, pi


def create_grid():
	matrix = np.zeros((n_grid,n_grid))
	pi = np.zeros((n_grid))
	for i in range(n_grid):
		# Faz o self-loop:
		matrix[i][i] = 0.5
		# Caso em que i é quina:
		if i == 0:
			matrix[i][1] = 0.25
			matrix[i][32] = 0.25
			pi[i] = 1.0/1984
		elif i == 31:
			matrix[i][30] = 0.25
			matrix[i][63] = 0.25
			pi[i] = 1.0/1984
		elif i == 992:
			matrix[i][993] = 0.25
			matrix[i][960] = 0.25
			pi[i] = 1.0/1984
		elif i == 1023:
			matrix[i][1022] = 0.25
			matrix[i][991] = 0.25
			pi[i] = 1.0/1984
		elif i > 0 and i < 31:
			matrix[i][i-1] = 1.0/6
			matrix[i][i+1] = 1.0/6
			matrix[i][i+32] = 1.0/6
			pi[i] = 3.0/3968
		elif i > 992 and i < 1023:
			matrix[i][i-1] = 1.0/6
			matrix[i][i+1] = 1.0/6
			matrix[i][i-32] = 1.0/6
			pi[i] = 3.0/3968
		elif i % 32 == 0:
			matrix[i][i-32] = 1.0/6
			matrix[i][i+32] = 1.0/6
			matrix[i][i+1] = 1.0/6
			pi[i] = 3.0/3968
		elif (i+1) % 32 == 0:
			matrix[i][i-32] = 1.0/6
			matrix[i][i+32] = 1.0/6
			matrix[i][i-1] = 1.0/6
			pi[i] = 3.0/3968
		else:
			matrix[i][i-1] = 0.125
			matrix[i][i+1] = 0.125
			matrix[i][i-32] = 0.125
			matrix[i][i+32] = 0.125
			pi[i] = 1.0/992
	return matrix, pi

def total_distance(v1,v2):
	acc = 0
	for i in range(len(v1)):
		acc += abs(v1[i] - v2[i])
	return acc*1.0/2


matrix, pi = create_tree()
pi_t = np.zeros((n_tree))
pi_t[455] = 1
for i in range(30000):
	mult = pi_t.dot(matrix)
	pi_t = mult
print(total_distance(pi_t, pi))
print(pi_t)
