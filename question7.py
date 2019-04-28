import numpy
import math

# Numero de elementos:
n = 10
# Vetor com todos os n elementos distintos:
total = list(range(20))
# Tamanho de cada subconjunto amostrado:
k = 4

# Gera subconjuntos de tamanho k com probabiliade 1/|S_k,n|:
for i in range(k):
	# Gera uma uniforme discreta podendo valer {0,1,...,n-1}:
	j = math.floor(numpy.random.uniform(0,n))
	# Guarda o elemento gerado:
	temp = total[j]
	# Permuta no array:
	total[j] = total[n-j-1]
	total[n-j-1] = temp

print(total[:k])
