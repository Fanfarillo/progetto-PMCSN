import matplotlib.pyplot as plt
import numpy as np

#----------------------------------------------------------------------------------
#Attenzione: questo programma funziona con un numero di variate random ben preciso.
#Se si cambia il file histogram.c potrebbe non funzionare correttamente
#----------------------------------------------------------------------------------

def sampleMeanDistribution():

	variates = []
	samples = []
	
	#Apro il file contenente le variate random
	f = open("histogramdata.dat", "r")
	
	#Leggo le variate random generate dalla simulazione
	for line in f:
		y = line.split()
		#print(float(y[0]))
		variates.append(float(y[0]))

	#La dimensione del campione è 36
	sampleSize = 36
	
	#Calcolo delle medie campionarie
	somma=0
	n=0
	p = 0	
	for i in range(1,10001):
		p=p+1
		if(i%sampleSize==0):
			somma = somma + variates[i-1]
			#print(float(somma/n))
			n=n+1
			samples.append(float(somma/n))
			n=0	
			somma=0
			continue
		somma = somma + variates[i-1]
		n = n + 1		
	
	#La media campionaria è uno stimatore puntuale non distorto della media
	somma = 0
	for i in range(0, len(samples)):
		somma = somma + samples[i]
	print("La media delle medie campionarie converge alla media teorica: ", float(somma/len(samples)))
	
	#Stampo le medie campionarie calcolate e il numero delle iterazioni eseguite
	print(samples)
	print("Numero iterazioni: ", p)
	
	#Costruzione dell'istogramma delle medie campionarie
	nparray = np.array(samples)
	plt.hist(nparray, density=True)
	plt.show()
	
def standardizedSampleMeanDistribution():

	variates = []
	samples = []
	
	f = open("histogramdata.dat", "r")
	
	for line in f:
		y = line.split()
		#print(float(y[0]))
		variates.append(float(y[0]))

	sampleSize = 36

	somma=0
	n=0
	p = 0	
	for i in range(1,10001):
		p=p+1
		if(i%sampleSize==0):
			somma = somma + variates[i-1]
			n=n+1
			samples.append(float((float(somma/n)-2)/(2/6)))
			n=0	
			somma=0
			continue
		somma = somma + variates[i-1]
		n = n + 1
		
	print(samples)
	print("Numero iterazioni: ", p)
	
	nparray = np.array(samples)
	plt.hist(nparray, density=True)
	plt.show()
	
sampleMeanDistribution()
standardizedSampleMeanDistribution()

