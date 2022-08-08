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

#define LOC 0.99                       /* level of confidence,        */ 
                                       /* use 0.95 for 95% confidence */
#define SAMPLING 1                         

void errorMalloc2(int code) {

	printf("ERRORE: impossibile allocare nuova memoria nell'heap.\n");
	fflush(stdout);
	exit(code);

}

/*

void computeInterval(char *filename)
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
}


void verify(){

	double rho_0 = a[0].service/t->current;
	double q_0 = a[0].queue/t->current;
	double n_0 = a[0].node/t->current;
	if(n_0 - (q_0 + rho_0) > 0.000001)
		errorVerify(-2000);
		
	double rho_1 = a[1].service/t->current;
	double q_1 = a[1].queue/t->current;
	double n_1 = a[1].node/t->current;
	if(n_1 - (q_1 + rho_1) > 0.000001)
		errorVerify(-2001);
		
	double rho_2 = a[2].service/t->current;
	double q_2 = a[2].queue/t->current;
	double n_2 = a[2].node/t->current;
	if(n_2 - (q_2 + rho_2) > 0.000001)
		errorVerify(-2002);
		
	double rho_3 = a[3].service/t->current;
	double q_3 = a[3].queue/t->current;
	double n_3 = a[3].node/t->current;
	if(n_3 - (q_3 + rho_3) > 0.000001)
		errorVerify(-2003);
				
	double rho_4 = a[4].service/t->current;
	double q_4 = a[4].queue/t->current;
	double n_4 = a[4].node/t->current;
	if(n_4 - (q_4 + rho_4) > 0.000001)
		errorVerify(-2004);	
	
	double s_0 = a[0].service/(al[0].index_a + al[0].index_f);
	double tq_0 = a[0].queue/(al[0].index_a + al[0].index_f);
	double ts_0 = a[0].node/(al[0].index_a + al[0].index_f);
	if(ts_0 - (tq_0 + s_0) > 0.000001)
		errorVerify(-2005);
		
	double s_1 = a[1].service/(al[1].index_a + al[1].index_f);
	double tq_1 = a[1].queue/(al[1].index_a + al[1].index_f);
	double ts_1 = a[1].node/(al[1].index_a + al[1].index_f);
	if(ts_1 - (tq_1 + s_1) > 0.000001)
		errorVerify(-2006);
		
	double s_2 = a[2].service/(al[2].index_a + al[2].index_f);
	double tq_2 = a[2].queue/(al[2].index_a + al[2].index_f);
	double ts_2 = a[2].node/(al[2].index_a + al[2].index_f);
	if(ts_2 - (tq_2 + s_2) > 0.000001)
		errorVerify(-2007);
		
	double s_3 = a[3].service/(al[3].index_a + al[3].index_f);
	double tq_3 = a[3].queue/(al[3].index_a + al[3].index_f);
	double ts_3 = a[3].node/(al[3].index_a + al[3].index_f);
	if(ts_3 - (tq_3 + s_3) > 0.000001)
		errorVerify(-2008);	
		
	double s_4 = a[4].service/(al[4].index_a + al[4].index_f);
	double tq_4 = a[4].queue/(al[4].index_a + al[4].index_f);
	double ts_4 = a[4].node/(al[4].index_a + al[4].index_f);
	if(ts_4 - (tq_4 + s_4) > 0.000001)
		errorVerify(-2009);

}*/

FILE** createStatisticFiles(){


	struct stat st1 = {0}, st2 = {0}, st3 = {0};


	if (stat("./data", &st1) == -1) {
   		if(mkdir("./data", 0777)==-1){
   			printf("Errore creazione cartella data per i dati...\n");
   			fflush(stdout);
   			exit(-100);
   		}
   		
	}
	
	if (stat("./data/finite", &st2) == -1) {
   		if(mkdir("./data/finite", 0777)==-1){
   			printf("Errore creazione cartella finite per i dati...\n");
   			fflush(stdout);
   			exit(-101);
   		}
	}
	
	if (stat("./data/infinite", &st3) == -1) {
   		if(mkdir("./data/infinite", 0777)==-1){
   			printf("Errore creazione cartella infinite per i dati...\n");
   			fflush(stdout);
   			exit(-102);
   		}
	}

	int length1 = strlen("./data/finite/servicenode.dat")+2;
	int length2 = strlen("./data/infinite/servicenode.dat")+2;
	int length3 = strlen("./data/finite/servicenodesampling.dat")+2;
	
	char *filename;

	FILE **fps = (FILE**) malloc(sizeof(FILE *)*15);
	if(fps==NULL)
		errorMalloc2(-1018);
	
	for(int i = 0; i < 15; i++){
		FILE *fp;
		if(i<5)
			filename = (char *) malloc(length1);
		else if(i>=5 && i < 10)
			filename = (char *) malloc(length2);
		else
			filename = (char *)malloc(length3);
		if(filename==NULL)
			errorMalloc2(-1019);
		if(i<5)
   			sprintf(filename,"./data/finite/servicenode%d.dat", i+1);
   		else if(i>=5 && i < 10)
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

	if(argc < 7){
		printf("ERRORE: i parametri passati in input al programma sono errati.\n");
		printf("Formato richiesto: NUM_SERVENTI_CENTRO_1, NUM_SERVENTI_CENTRO_2, NUM_SERVENTI_CENTRO_3, NUM_SERVENTI_CENTRO_4, NUM_SERVENTI_CENTRO_5, FASCIA_ORARIA.\n");
		fflush(stdout);
		exit(-1);
	}

	FILE *fp_config = fopen("properties.conf", "r");
	if(fp_config==NULL)
		errorMalloc2(-2000);

	int **array_m = (int **)malloc(sizeof(int *)*6);
	if(array_m==NULL)
		errorMalloc2(-2001);
	for(int i=0; i<6; i++) {
		array_m[i] = (int *)malloc(sizeof(int)*CENTERS);
		if(array_m[i]==NULL)
			errorMalloc2(-2002);
	}

	int check;
	for(int i=0; i<6; i++) {
		check = fscanf(fp_config, "%d %d %d %d %d\n", &array_m[i][0], &array_m[i][1], &array_m[i][2], &array_m[i][3], &array_m[i][4]);
		if(check!=5) {
			printf("ERRORE: c'è stato un problema nella lettura del file properties.conf.\n");
			fflush(stdout);
			exit(-6);
		}
	}

/*	int* m = NULL;
	m = (int *)malloc(sizeof(int)*(5));

	if(m==NULL){
		errorMalloc2(-1000);
	}

	m[0] = atoi(argv[1]);
	m[1] = atoi(argv[2]);
	m[2] = atoi(argv[3]);
	m[3] = atoi(argv[4]);
	m[4] = atoi(argv[5]);

	for(int i=0; i<5; i++) {
		if(m[i]<=0) {
			printf("ERRORE: il numero di serventi specificato non è valido. Fornire un valore intero strettamente positivo.\n");
			fflush(stdout);
			exit(-2);
		}
	}*/
	
	FILE ** fps = createStatisticFiles();	
	
	double *****ret = finite_sim(array_m);
	/*
	for(int replica=0;replica<REPLICATIONS;replica++)
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
					printf("%d: %f\n", stat, ret[0][replica][center][interval][stat]);
					fflush(stdout);	
				}
			}
		}
	}*/

	int interval = atoi(argv[6]);

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
			exit(-3);
	}
	
	char *stat_value;
	FILE *fp = NULL;
	for(int center=0;center<CENTERS;center++){
		fp = fps[center];
		for(int replica=0; replica<REPLICATIONS;replica++)
		{
			for(int interval=0; interval<INTERVALS;interval++)
			{
				for(int stat=0;stat<STATISTICS;stat++)
				{
					stat_value=(char *)malloc(30);
					if(stat_value==NULL)
						errorMalloc2(-8000);
					sprintf(stat_value, "%f;", ret[0][replica][center][interval][stat]);
					fputs(stat_value, fps[center]);
					free(stat_value);
				}
				fputs("\n", fps[center]);
				
			}
			fputs("\n", fps[center]);
			
		}
	}
	
	int n = STOP/SAMPLINGINTERVAL;
	
	for(int center=0;center<CENTERS;center++){
		fp = fps[center+10];
		for(int replica=0; replica<REPLICATIONS;replica++)
		{
			
			for(int stat=0;stat<STATISTICS;stat++)
			{
				for(int count=0; count < n;count++)
				{
					stat_value=(char *)malloc(30);
					if(stat_value==NULL)
						errorMalloc2(-8000);
					if(replica==2)
						printf("stat %d count %d: %f\n", stat, count, ret[1][replica][center][stat][count]);
					sprintf(stat_value, "%f;", ret[1][replica][center][stat][count]);
					fputs(stat_value, fps[center+10]);
					free(stat_value);
				}
				fputs("\n", fps[center+10]);
				
			}				
			
			fputs("\n", fps[center+10]);
			
		}
	}
	
	for(int i = 0; i<15;i++){
		fclose(fps[i]);
	}
	
	printf("Simulazione completata con successo...\n");
	fflush(stdout);

	return 0;

}
