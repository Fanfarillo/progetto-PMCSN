import csv

data=['Tempo di risposta', 'Numero di jobs']
x = [
['21.2', 9],
['32', 8],
['98', 78]
]

filename = './data.csv'
with open(filename, 'w') as file:
	z = csv.writer(file)
	z.writerow(data)
	z.writerows(x)
