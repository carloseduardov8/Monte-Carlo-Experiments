import numpy
import math

# Numero de elementos:
n = 100
# Vetor com todos os n elementos distintos:
total = list(range(n))
# Tamanho de cada subconjunto amostrado:
k = 4

# Gera subconjuntos de tamanho k com probabiliade 1/|S_k,n|:
for i in range(k):
	# Gera uma uniforme discreta podendo valer {i, i+1, ..., n-1}:
	j = math.floor(numpy.random.uniform(i,n))
	# Permuta no array:
	temp = total[j]
	total[j] = total[i]
	# Fixa o elemento no inicio do array:
	total[i] = temp

# Printa o resultado:
print("Sample:", total[:k])
