#include "./headers/sim.h"
#include "./headers/servicenode1.h"
#include "./headers/servicenode2.h"
#include "./headers/servicenode3.h"
#include "./headers/servicenode4.h"
#include "./headers/servicenode5.h"
#include "./headers/rngs.h"
#include "./headers/rvms.h"
#include "./headers/randomGeneratorFunctions.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define SAMPLING 1                         

void errorMallocMain(int code) {

	printf("ERRORE: impossibile allocare nuova memoria nell'heap [simulation.c].\n");
	fflush(stdout);
	exit(code);

}

void errorVerify(int code) {

	printf("ERRORE: le statistiche di output della simulazione non sono consistenti.\n");
	fflush(stdout);
	exit(code);

}

/*void computeInterval(char *filename)
{

  	long   n    = 0;                     
  	double sum  = 0.0;
  	double mean = 0.0;
  	double data;
  	double stdev;
  	double u, t, w;
  	double diff;

	FILE *fp = fopen(filename, "r");
	if(fp==NULL)
		exit(-1);
	
	double variate;
	int ret;
	
	while((ret = fscanf(fp, "%lf", &variate))!=EOF)
	{
		if(ret != 1)
			exit(-3);
		n++;                                 
    		diff  = variate - mean;
    		printf("diff = %lf\n", diff);fflush(stdout);
    		sum  += diff * diff * (n - 1.0) / n;
    		printf("sum = %lf\n", sum);fflush(stdout);
    		mean += diff / n;
    		printf("mean = %lf\n", mean);fflush(stdout);
		//printf("%lf\n", variate);
	}
	
  	stdev  = sqrt(sum / n);
  	printf("stdev = %lf\n", stdev);fflush(stdout);

  	if (n > 1) {
    		u = 1.0 - 0.5 * (1.0 - LOC);              
    		t = idfStudent(n - 1, u);                 
    		printf("Valore critico: %lf\n", t);
    		w = t * stdev / sqrt(n - 1);              
    		printf("\nbased upon %ld data points", n);
    		printf(" and with %d%% confidence\n", (int) (100.0 * LOC + 0.5));
    		printf("the expected value is in the interval");
    		printf("%10.2f +/- %6.2f\n", mean, w);
  	}
  	else{
    		printf("ERROR - insufficient data\n");
  		exit(-9000);
	}
}*/

void finiteVerify(struct result_finite *ret) {

	for(int replica=0; replica<REPLICATIONS; replica++) {
		for(int center=0; center<CENTERS; center++) {
			for(int samp=0; samp<STOP/SAMPLINGINTERVAL; samp++) {
				if(ret->samplingTime[replica][center][2][samp] - (ret->samplingTime[replica][center][1][samp] + ret->numMedioServentiAttivi[replica][center][samp]*ret->samplingTime[replica][center][0][samp]) > 0.000001)
					errorVerify(-2003);

				if(ret->samplingTime[replica][center][5][samp] - (ret->samplingTime[replica][center][4][samp] + ret->samplingTime[replica][center][3][samp]) > 0.000001)
					errorVerify(-2004);

				if(ret->samplingTime[replica][center][0][samp] > 1 || ret->samplingTime[replica][center][0][samp] < 0 || ret->samplingTime[replica][center][10][samp] > 1 || ret->samplingTime[replica][center][10][samp] < 0) {
					//printf("ERRORE - rho = %f\t replica = %d\t centro = %d\t samp = %d\n", ret->samplingTime[replica][center][0][samp], replica, center, samp);
					errorVerify(-2005);
				}

			}
		}
	}

}

void infiniteVerify(double ***siminf, int* m){

	for(int center=0; center<CENTERS; center++) {
		for(int batch=0; batch<K; batch++) {
			if(siminf[center][2][batch] - (siminf[center][1][batch] + m[center]*siminf[center][0][batch]) > 0.000001)
				errorVerify(-2000);

			if(siminf[center][5][batch] - (siminf[center][4][batch] + siminf[center][3][batch]) > 0.000001)
				errorVerify(-2001);

			if(siminf[center][0][batch] > 1 || siminf[center][0][batch] < 0 || siminf[center][10][batch] > 1 || siminf[center][10][batch] < 0) {
				//printf("ERRORE - rho = %f\n", siminf[center][0][batch]);
				errorVerify(-2002);
			}

		}

	}

}

FILE** createStatisticFiles(){

	struct stat st1 = {0}, st2 = {0}, st3 = {0};

	if (stat("./data", &st1) == -1) {
   		if(mkdir("./data", 0777)==-1){
   			printf("Errore creazione cartella data per i dati...\n");
   			fflush(stdout);
   			exit(-4);
   		}
   		
	}
	
	if (stat("./data/finite", &st2) == -1) {
   		if(mkdir("./data/finite", 0777)==-1){
   			printf("Errore creazione cartella finite per i dati...\n");
   			fflush(stdout);
   			exit(-5);
   		}
	}
	
	if (stat("./data/infinite", &st3) == -1) {
   		if(mkdir("./data/infinite", 0777)==-1){
   			printf("Errore creazione cartella infinite per i dati...\n");
   			fflush(stdout);
   			exit(-6);
   		}
	}

	int length1 = strlen("./data/finite/servicenode.dat")+2;
	int length2 = strlen("./data/infinite/servicenode.dat")+2;
	int length3 = strlen("./data/finite/servicenodesampling.dat")+2;
	
	char *filename;

	FILE **fps = (FILE**) malloc(sizeof(FILE *)*15);
	if(fps==NULL)
		errorMallocMain(-1002);
	
	for(int i = 0; i < 15; i++){
		FILE *fp;

		if(i<5)
			filename = (char *) malloc(length1);
		else if(i>=5 && i < 10)
			filename = (char *) malloc(length2);
		else
			filename = (char *)malloc(length3);
		if(filename==NULL)
			errorMallocMain(-1003);

		if(i<5)
   			sprintf(filename,"./data/finite/servicenode%d.dat", i+1);
   		else if(i>=5 && i<10)
   			sprintf(filename,"./data/infinite/servicenode%d.dat", i-4);
   		else
   			sprintf(filename,"./data/finite/servicenodesampling%d.dat", i-9);

   		fp = fopen(filename, "w");
   		if(fp==NULL){
   			printf("ERRORE: impossibile creare il file %s.\n", filename);
			fflush(stdout);
   			exit(-5);
   		}
   		free(filename);
   		fps[i] = fp;   		
	}
	
	return fps;
}

int main(int argc, char **argv){

	if(argc < 2){
		printf("ERRORE: i parametri passati in input al programma sono errati.\n");
		printf("Formato richiesto: NOME_PROGRAMMA, FASCIA_ORARIA.\n");
		fflush(stdout);
		exit(-1);
	}

	FILE *fp_config = fopen("properties.conf", "r");
	if(fp_config==NULL) {
		printf("ERRORE: impossibile creare il file properties.conf.\n");
		fflush(stdout);
		exit(-2);
	}

	int **array_m = (int **)malloc(sizeof(int *)*6);
	if(array_m==NULL)
		errorMallocMain(-1000);
	for(int i=0; i<6; i++) {
		array_m[i] = (int *)malloc(sizeof(int)*CENTERS);
		if(array_m[i]==NULL)
			errorMallocMain(-1001);
	}

	int check;
	for(int i=0; i<6; i++) {
		check = fscanf(fp_config, "%d %d %d %d %d\n", &array_m[i][0], &array_m[i][1], &array_m[i][2], &array_m[i][3], &array_m[i][4]);
		if(check!=5) {
			printf("ERRORE: c'è stato un problema nella lettura del file properties.conf.\n");
			fflush(stdout);
			exit(-3);
		}
	}
	
	FILE **fps = createStatisticFiles();	
	
	//SIMULAZIONE A ORIZZONTE FINITO
	struct result_finite *ret = finite_sim(array_m);
	
	/*for(int replica=0;replica<REPLICATIONS;replica++)
	{
		printf("-------------------------------------- REPLICA %d -----------------------------\n", replica);
		for(int center=0;center<CENTERS;center++)
		{
			printf("-------------------------------------- CENTRO %d -----------------------------\n", center);
			for(int interval=0;interval<INTERVALS;interval++)
			{
				printf("-------------------------------------- INTERVALLO %d -----------------------------\n", interval);
				for(int stat=0;stat<STATISTICS;stat++)
				{
					printf("%d: %f\n", stat, ret->nsim[replica][center][interval][stat]);
					fflush(stdout);	
				}
			}
		}
	}*/
	
	char *stat_value;
	FILE *fp = NULL;

	for(int center=0; center<CENTERS; center++){
		fp = fps[center];
		for(int replica=0; replica<REPLICATIONS;replica++)
		{
			for(int interval=0; interval<INTERVALS;interval++)
			{
				for(int stat=0; stat<STATISTICS; stat++)
				{
					stat_value=(char *)malloc(30);
					if(stat_value==NULL)
						errorMallocMain(-1004);

					sprintf(stat_value, "%f;", ret->nsim[replica][center][interval][stat]);
					fputs(stat_value, fps[center]);
					free(stat_value);
				}
				fputs("\n", fps[center]);
				
			}
			fputs("\n", fps[center]);
			
		}
	}
	
	int n = STOP/SAMPLINGINTERVAL;
	
	for(int center=0; center<CENTERS; center++){
		fp = fps[center+10];

		for(int replica=0; replica<REPLICATIONS;replica++)
		{			
			for(int stat=0; stat<STATISTICS; stat++)
			{
				for(int count=0; count < n;count++)
				{
					stat_value=(char *)malloc(30);
					if(stat_value==NULL)
						errorMallocMain(-1005);

					sprintf(stat_value, "%f;", ret->samplingTime[replica][center][stat][count]);
					fputs(stat_value, fps[center+10]);
					free(stat_value);
				}
				fputs("\n", fps[center+10]);
				
			}							
			fputs("\n", fps[center+10]);
			
		}
	}

	finiteVerify(ret);
	
	int interval = atoi(argv[1]);

	switch(interval){
		case 1:
			interTime = 180.0;
			break;
		case 2:
			interTime = 480.0;
			break;
		case 3:
			interTime = 60.0;
			break;
		case 4:
			interTime = 300.0;
			break;
		case 5:
			interTime = 30.0;
			break;
		case 6:
			interTime = 180.0;
			break;
		default:
			printf("ERRORE: la fascia oraria specificata non è valida. Fornire un valore intero compreso tra 1 e 6.\n");
			fflush(stdout);
			exit(-7);
	}
	
	//SIMULAZIONE A ORIZZONTE INFINITO
	double ***siminf = infinite_sim(array_m[interval-1]);
	
	for(int center=0; center<CENTERS; center++){
		fp = fps[center+5];

		for(int stat=0; stat<STATISTICS; stat++)
		{
			for(int batch=0; batch<K; batch++){

				stat_value=(char *)malloc(30);
				if(stat_value==NULL)
					errorMallocMain(-1006);

				sprintf(stat_value, "%f;", siminf[center][stat][batch]);
				fputs(stat_value, fps[center+5]);
				free(stat_value);
			
				fputs("\n", fps[center+5]);
				
			}
			fputs("\n", fps[center+5]);
			
		}
	}
	
	for(int i=0; i<15; i++){
		fclose(fps[i]);
	}

	infiniteVerify(siminf, array_m[interval-1]);

	free(array_m);
	free(fps);
	free(ret);
	free(siminf);

	printf("FINISH\n");
	fflush(stdout);
	return 0;

}
