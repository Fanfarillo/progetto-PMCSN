import matplotlib.pyplot as plt
import numpy as np

#----------------------------------------------------------------------------------
#Attenzione: questo programma funziona con un numero di variate random ben preciso.
#Se si cambia il file histogram.c potrebbe non funzionare correttamente
#----------------------------------------------------------------------------------


def plotWaitingTime(centro):

	if (centro < 1 | centro > 5):
		print("Centro non valido...\n")
		return 1
		
	utilizzazioni = []
	numeroCoda = []
	numeroCentro = []
	servizi = []
	attese = []
	risposte = []
	interarrivo = []
	arriviFam = []
	arriviAuto = []
	nmPerdite = []
	probDiPerdita = []
	statistiche = []
	
	statistiche.append(utilizzazioni)
	statistiche.append(numeroCoda)
	statistiche.append(numeroCentro)
	statistiche.append(servizi)
	statistiche.append(attese)
	statistiche.append(risposte)
	statistiche.append(interarrivo)
	statistiche.append(arriviFam)
	statistiche.append(arriviAuto)
	statistiche.append(nmPerdite)
	statistiche.append(probDiPerdita)
	
	#costruzione struttura dati per la manipolazione dei dati
	for stat in range(0,11):
		statistiche[stat] = []
		for replica in range(0,16):
			statistiche[stat].append([])
		
	#print(statistiche)
	
	filename = "../data/finite/servicenodesampling" + str(centro) +  ".dat"
	f = open("../data/finite/servicenodesampling1.dat", "r")	
	
	replica = []
	for i in range(0,16):
		replica.append(0)
	
	count = 0
	s = 0.0
	item1 = 0
	item2 = 0
	item3 = 0
	item4 = 0
	item5 = 0
	item6 = 0
	item7 = 0
	item8 = 0
	item9 = 0
	item10 = 0
	item11 = 0	
	
	
	for line in f:
		if line != "\n":
			if(count%11==0):			
				rigaSplittata = line.split(";")
				for rho in rigaSplittata:
					if rho!="\n":
						item1 = item1 + 1
						statistiche[0][replica[0]].append(float(rho))
				replica[0] = replica[0] + 1				
			elif(count%11==1):		
				rigaSplittata = line.split(";")
				for q in rigaSplittata:
					if q!="\n":
						item2 = item2 + 1
						statistiche[1][replica[1]].append(float(q))
				replica[1] = replica[1] + 1
			elif(count%11==2):
				rigaSplittata = line.split(";")
				for n in rigaSplittata:
					if n!="\n":
						item3 = item3 + 1
						statistiche[2][replica[2]].append(float(n))
				replica[2] = replica[2] + 1
			elif(count%11==3):
				rigaSplittata = line.split(";")
				for s in rigaSplittata:
					if s!="\n":
						item4 = item4 + 1
						statistiche[3][replica[3]].append(float(s))
				replica[3] = replica[3] + 1
			elif(count%11==4):
				rigaSplittata = line.split(";")
				for d in rigaSplittata:
					if d!="\n":
						item5 = item5 + 1
						statistiche[4][replica[4]].append(float(d))
				replica[4] = replica[4] + 1
			elif(count%11==5):
				rigaSplittata = line.split(";")
				for w in rigaSplittata:
					if w!="\n":
						item6 = item6 + 1
						statistiche[5][replica[5]].append(float(w))
				replica[5] = replica[5] + 1
			elif(count%11==6):
				rigaSplittata = line.split(";")
				for r in rigaSplittata:
					if r!="\n":
						item7 = item7 + 1
						statistiche[6][replica[6]].append(float(r))
				replica[6] = replica[6] + 1
			elif(count%11==7):
				rigaSplittata = line.split(";")
				for f in rigaSplittata:
					if f!="\n":
						item8 = item8 + 1
						statistiche[7][replica[7]].append(float(f))
				replica[7] = replica[7] + 1
			elif(count%11==8):
				rigaSplittata = line.split(";")
				for a in rigaSplittata:
					if a!="\n":
						item9 = item9 + 1
						statistiche[8][replica[8]].append(float(a))
				replica[8] = replica[8] + 1
			elif(count%11==9):
				rigaSplittata = line.split(";")
				for np in rigaSplittata:
					if np!="\n":
						item10 = item10 + 1
						statistiche[9][replica[9]].append(float(np))
				replica[9] = replica[9] + 1
			elif(count%11==10):
				rigaSplittata = line.split(";")
				for p in rigaSplittata:
					if p!="\n":
						item11 = item11 + 1
						statistiche[10][replica[10]].append(float(p))
				replica[10] = replica[10] + 1
			count = count + 1
			
	print("item1 = %d" %item1)
	print("item2 = %d" %item1)
	print("item3 = %d" %item1)
	print("item4 = %d" %item1)
	print("item5 = %d" %item1)
	print("item6 = %d" %item1)
	print("item7 = %d" %item1)
	print("item8 = %d" %item1)
	print("item9 = %d" %item1)
	print("item10 = %d" %item1)
	print("item11 = %d" %item1)
	
	#campioni
	rho = []
	q = []
	n = []
	s = []
	tq = []
	ts = []
	r = []
	f = []
	a = []
	np = []
	p = []
	stats = []
	stats.append(rho)
	stats.append(q)
	stats.append(n)
	stats.append(s)
	stats.append(tq)
	stats.append(ts)
	stats.append(r)
	stats.append(f)
	stats.append(a)
	stats.append(np)
	stats.append(p)
	
	count = 0
	
	#costruzione dei campioni
	for stat in range(0,11):
		for value in range(0,168):
			sommatoria = 0.0
			for rep in range(0,16):
				sommatoria = sommatoria + statistiche[stat][rep][value]
			media = float(sommatoria / 16)
			stats[stat].append(media)
	for i in range(0,11):
		count = 0
		for j in stats[i]:
			count = count + 1
			#print(j)
		print(count)
		
	#costruisco il grafico
	
	y = stats[0]
	print(y)
	plt.plot(y, color = 'r')
	plt.ylabel("rho")
	plt.xlabel("Tempo")
	plt.title("UTILIZZAZIONE")
	plt.show()
	
	
	y = stats[1]
	print(y)
	#nparray = np.array(y)
	plt.plot(y, color = 'r')
	plt.ylabel("Nq")
	plt.xlabel("Tempo")
	plt.title("NUMERO JOBS IN CODA")
	plt.show()
	
	y = stats[2]
	print(y)
	#nparray = np.array(y)
	plt.plot(y, color = 'r')
	plt.ylabel("Ns")
	plt.xlabel("Tempo")
	plt.title("NUMERO JOBS NEL CENTRO")
	plt.show()
	
	y = stats[3]
	print(y)
	#nparray = np.array(y)
	plt.plot(y, color = 'r')
	plt.ylabel("S")
	plt.xlabel("Tempo")
	plt.title("TEMPO DI SERVIZIO")
	plt.show()
	
	y = stats[4]
	print(y)
	#nparray = np.array(y)
	plt.plot(y, color = 'r')
	plt.ylabel("Tq")
	plt.xlabel("Tempo")
	plt.title("TEMPO DI ATTESA")
	plt.show()
	
	y = stats[5]
	print(y)
	#nparray = np.array(y)
	plt.plot(y, color = 'r')
	plt.ylabel("Ns")
	plt.xlabel("Tempo")
	plt.title("TEMPO DI RISPOSTA")
	plt.show()
	
	y = stats[6]
	print(y)
	#nparray = np.array(y)
	plt.plot(y, color = 'r')
	plt.ylabel("r")
	plt.xlabel("Tempo")
	plt.title("INTERARRIVO")
	plt.show()
	
	y = stats[7]
	print(y)
	#nparray = np.array(y)
	plt.plot(y, color = 'r')
	plt.ylabel("F")
	plt.xlabel("Tempo")
	plt.title("ARRIVI FAMIGLIE")
	plt.show()
	
	y = stats[8]
	print(y)
	#nparray = np.array(y)
	plt.plot(y, color = 'r')
	plt.ylabel("A")
	plt.xlabel("Tempo")
	plt.title("ARRIVI AUTOMOBILI")
	plt.show()
	
	y = stats[9]
	print(y)
	#nparray = np.array(y)
	plt.plot(y, color = 'r')
	plt.ylabel("perdite")
	plt.xlabel("Tempo")
	plt.title("NUMERO DI PERDITE NEL SISTEMA")
	plt.show()
	
	y = stats[10]
	print(y)
	#nparray = np.array(y)
	plt.plot(y, color = 'r')
	plt.ylabel("P(perdita)")
	plt.xlabel("Tempo")
	plt.title("PROBABILITA' DI AVERE UNA PERDITA NEL SISTEMA")
	plt.show()
	
	
	
	return 0
	
	
	
	

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
	

	
	
#sampleMeanDistribution()
#standardizedSampleMeanDistribution()
plotWaitingTime(1)

