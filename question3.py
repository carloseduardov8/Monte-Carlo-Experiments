import requests
from requests.exceptions import ConnectionError
import random

letters = [
	"a","b","c","d","e","f","g","h","i","j","k","l","m","n",
	"o","p","q","r","s","t","u","v","w","x","y","z"
]

n = 100000
hits = 0 # Numero de sites existentes

# Realizar n experimentos:
for trial in range(n):
	site = ""
	k = random.randrange(1,5)

	for i in range(k):
		uniform = random.randrange(0,26)
		site += letters[uniform]

	print(site + " " + str(trial))

	try:
	    request = requests.get("http://www." + site + ".ufrj.br")
	except ConnectionError:
	    pass
	else:
	    hits += 1

print("p = " + str(hits*1.0/n))
print(str(hits))


#100: 0.0 (0 hits)
#1000: 0.007 (7 hits)
#10000: 0.0038 (38 hits)
