import requests
from requests.exceptions import ConnectionError
import random

# Valores que a v.a. Z_i pode assumir:
letters = [
	"a","b","c","d","e","f","g","h","i","j","k","l","m","n",
	"o","p","q","r","s","t","u","v","w","x","y","z"
]

n = 100000 # Numero de experimentos
hits = 0 # Numero de sites existentes

# Realizar n experimentos:
for trial in range(n):
	site = ""
	k = random.randrange(1,5)
	# Gera um dominio aleatorio:
	for i in range(k):
		uniform = random.randrange(0,26)
		site += letters[uniform]
	# Mostra dominio gerado:
	print(site + " " + str(trial))
	# Tenta validar dominio:
	try:
	    request = requests.get("http://www." + site + ".ufrj.br")
	except ConnectionError:
	    pass
	else:
	    hits += 1
# Printa resultado:
print("p = " + str(hits*1.0/n))
print(str(hits))
