#include "../headers/servicenode1.h"
#include "../headers/servicenode2.h"
#include "../headers/servicenode3.h"
#include "../headers/servicenode4.h"
#include "../headers/servicenode5.h"
#include "../headers/rngs.h"
#include "../headers/rvms.h"
#include "../headers/sim.h"
#include "../headers/randomGeneratorFunctions.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

struct event_list *events;
struct time *t;
struct area *a, *aSampling;
struct state_variables1 *sv1;		//centers num 1, 3
struct state_variables2 *sv2;		//centers num 2, 4, 5
struct arrival_loss *al;
struct arrivals *arr;

void errorMalloc(int code) {

	printf("ERRORE: impossibile allocare nuova memoria nell'heap [sim.c].\n");
	fflush(stdout);
	exit(code);

}

void allocateDataStructures() {

	events = (struct event_list *) malloc(sizeof(struct event_list));
	if(events==NULL)
		errorMalloc(-1021);
		
	t = (struct time *) malloc(sizeof(struct time));
	if(t==NULL)
		errorMalloc(-1022);
		
	a = (struct area *) malloc(sizeof(struct area)*5);
	if(a==NULL)
		errorMalloc(-1023);	
		
	sv1 = (struct state_variables1 *) malloc(sizeof(struct state_variables1) * 2);
	if(sv1==NULL)
		errorMalloc(-1024);
		
	sv2 = (struct state_variables2 *) malloc(sizeof(struct state_variables2)*3);
	if(sv2==NULL)
		errorMalloc(-1025);
		
	al = (struct arrival_loss *) malloc(sizeof(struct arrival_loss)*5);
	if(al ==NULL)
		errorMalloc(-1026);
		
	arr = (struct arrivals *) malloc(sizeof(struct arrivals));
	if(arr == NULL)
		errorMalloc(-1027);
		
	aSampling = (struct area *) malloc(sizeof(struct area)*5);
	if(aSampling==NULL)
		errorMalloc(-1028);
	
}

void deallocateDataStructures() {

	free(events);
	free(t);
	free(a);
	free(sv1);
	free(sv2);
	free(al);
	free(arr);
	free(aSampling);

}

void initializeEventList(int *m, bool simType) {
	//m is maxArray

	if(P_CAR==0) {
		events->carArr1.carArrivalTime = (double) INFINITY;
		events->carArr1.isCarArrivalActive = false;
	}
	else {
		events->carArr1.carArrivalTime = getCarArrival(START);
		events->carArr1.isCarArrivalActive = true;
	}

	if(1-QE-P_CAR==0) {
		events->familyArr1.familyArrivalTime = (double) INFINITY;
		events->familyArr1.isFamilyArrivalActive = false;
	}
	else {
		events->familyArr1.familyArrivalTime = getFamilyArrival1(START);
		events->familyArr1.isFamilyArrivalActive = true;
	}
	
	if(QE==0) {
		events->familyArr2.familyArrivalTime = (double) INFINITY;
		events->familyArr2.isFamilyArrivalActive = false;
	}
	else {
		events->familyArr2.familyArrivalTime = getFamilyArrival2(START);
		events->familyArr2.isFamilyArrivalActive = true;
	}

	events->carArr3.carArrivalTime = (double) INFINITY;
	events->carArr3.isCarArrivalActive = true;
	events->familyArr3.familyArrivalTime = (double) INFINITY;
	events->familyArr3.isFamilyArrivalActive = true;

	events->familyArr4.familyArrivalTime = (double) INFINITY;
	events->familyArr4.isFamilyArrivalActive = true;

	events->familyArr5.familyArrivalTime = (double) INFINITY;
	events->familyArr5.isFamilyArrivalActive = true;

	events->head1 = NULL;
	events->tail1 = NULL;
	events->head2 = NULL;
	events->tail2 = NULL;

	events->completionTimes1 = (double *) malloc(sizeof(double)*m[0]);
	if(events->completionTimes1==NULL)
		errorMalloc(-1029);
	events->completionTimes2 = (double *) malloc(sizeof(double)*m[1]);
	if(events->completionTimes2==NULL)
		errorMalloc(-1030);
	events->completionTimes3 = (double *) malloc(sizeof(double)*m[2]);
	if(events->completionTimes3==NULL)
		errorMalloc(-1031);
	events->completionTimes4 = (double *) malloc(sizeof(double)*m[3]);
	if(events->completionTimes4==NULL)
		errorMalloc(-1032);
	events->completionTimes5 = (double *) malloc(sizeof(double)*m[4]);
	if(events->completionTimes5==NULL)
		errorMalloc(-1033);

	for(int i=0; i<m[0]; i++) {
		events->completionTimes1[i] = (double) INFINITY;
	}
	for(int i=0; i<m[1]; i++) {
		events->completionTimes2[i] = (double) INFINITY;
	}
	for(int i=0; i<m[2]; i++) {
		events->completionTimes3[i] = (double) INFINITY;
	}
	for(int i=0; i<m[3]; i++) {
		events->completionTimes4[i] = (double) INFINITY;
	}
	for(int i=0; i<m[4]; i++) {
		events->completionTimes5[i] = (double) INFINITY;
	}
	
	if(simType){
		events-> changeInterval = 0.0;
		events -> sampling = SAMPLINGINTERVAL;
	}
	else{
		events-> changeInterval = (double) INFINITY;
		events -> sampling = (double) INFINITY;
	}

}

void initializeTime() {

	t->current = 0.0;
	t->next = 0.0;
	for(int i=0; i<5; i++) {
		t->last[i] = 0.0;
	}

}

void initializeArea() {

	for(int i=0; i<5; i++) {
		a[i].node = 0.0;
		a[i].queue = 0.0;
		a[i].service = 0.0;
		aSampling[i].node = 0.0;
		aSampling[i].queue = 0.0;
		aSampling[i].service = 0.0;
		
	}

}

void initializeStateVariables(int *maxArray, int *m) {

	sv1[0].qA = 0;
	sv1[0].qF = 0;
	sv1[0].x = (int *) malloc(sizeof(int)*maxArray[0]);
	if(sv1[0].x==NULL)
		errorMalloc(-1034);
	
	for(int i=0; i<m[0]; i++) {
		sv1[0].x[i] = 0;	//0=IDLE, 1=BUSY_F, 2=BUSY_A
	}
	for(int i=m[0]; i<maxArray[0]; i++) {
		sv1[0].x[i] = -3;	//-3 = NOT ACTIVATED
	}

	sv2[0].l = 0;
	sv2[0].x = (int *) malloc(sizeof(int)*maxArray[1]);
	if(sv2[0].x==NULL)
		errorMalloc(-1035);

	for(int i=0; i<m[1]; i++) {
		sv2[0].x[i] = 0;	//0=IDLE, 1=BUSY
	}
	for(int i=m[1]; i<maxArray[1]; i++) {
		sv2[0].x[i] = -3;	//-3 = NOT ACTIVATED
	}

	sv1[1].qA = 0;
	sv1[1].qF = 0;
	sv1[1].x = (int *) malloc(sizeof(int)*maxArray[2]);
	if(sv1[1].x==NULL)
		errorMalloc(-1036);

	for(int i=0; i<m[2]; i++) {
		sv1[1].x[i] = 0;	//0=IDLE, 1=BUSY_F, 2=BUSY_A
	}
	for(int i=m[2]; i<maxArray[2]; i++) {
		sv1[1].x[i] = -3;	//-3 = NOT ACTIVATED
	}

	sv2[1].l = 0;
	sv2[1].x = (int *) malloc(sizeof(int)*maxArray[3]);
	if(sv2[1].x==NULL)
		errorMalloc(-1037);

	for(int i=0; i<m[3]; i++) {
		sv2[1].x[i] = 0;	//0=IDLE, 1=BUSY
	}
	for(int i=m[3]; i<maxArray[3]; i++) {
		sv2[1].x[i] = -3;	//-3 = NOT ACTIVATED
	}

	sv2[2].l = 0;  			//unuseful
	sv2[2].x = (int *) malloc(sizeof(int)*maxArray[4]);
	if(sv2[2].x==NULL)
		errorMalloc(-1038);

	for(int i=0; i<m[4]; i++) {
		sv2[2].x[i] = 0;	//0=IDLE, 1=BUSY
	}
	for(int i=m[4]; i<maxArray[4]; i++) {
		sv2[2].x[i] = -3;	//-3 = NOT ACTIVATED
	}

}

void initializeArrivalLoss() {

	for(int i=0; i<5; i++) {
		al[i].index_a = 0;
		al[i].index_f = 0;
		al[i].numLoss_f = 0;
		al[i].prev_index_a = 0;
		al[i].prev_index_f = 0;
		al[i].prev_numLoss_f = 0;
		al[i].compl_a = 0;
		al[i].compl_f = 0;
		al[i].prev_compl_a = 0;
		al[i].prev_compl_f = 0;
	}

}

void initializeArrivals() {

	arr->head3 = NULL;
	arr->tail3 = NULL;
	
	arr->head4 = NULL;
	arr->tail4 = NULL;
	
	arr->head5 = NULL;
	arr->tail5 = NULL;

}

int *getMaxArray(int **array_m) {

	int *maxArray = (int *)malloc(sizeof(int)*CENTERS);
	if(maxArray==NULL)
		errorMalloc(-1039);

	for(int j=0; j<CENTERS; j++) {
		maxArray[j] = array_m[0][j];
	}

	for(int i=0; i<6; i++) {
		for(int j=0; j<CENTERS; j++) {
			if(maxArray[j] < array_m[i][j])
				maxArray[j] = array_m[i][j];
		}

	}

	return maxArray;

}


void initialize(int **array_m)
{
	int *maxArray = getMaxArray(array_m);
	allocateDataStructures();
	initializeTime();	
	initializeArea();
	initializeStateVariables(maxArray, array_m[0]);
	initializeArrivalLoss();
	initializeArrivals();	
	initializeEventList(maxArray, true);	
}


void initializeSimInf(int *m)
{
	allocateDataStructures();
	initializeTime();	
	initializeArea();
	initializeStateVariables(m, m);
	initializeArrivalLoss();
	initializeArrivals();	
	initializeEventList(m, false);	
}


bool isSystemEmpty(int *m) {

	if(sv1[0].qA > 0 || sv1[0].qF > 0 || sv1[1].qA > 0 || sv1[1].qF > 0 || sv2[0].l > 0 || sv2[1].l > 0)
		return false;
		
	for(int i=0; i<m[0]; i++) {
		if(sv1[0].x[i] != 0 && sv1[0].x[i] != -3)
			return false;
	}
	for(int i=0; i<m[1]; i++) {
		if(sv2[0].x[i] != 0 && sv2[0].x[i] != -3)
			return false;
	}
	for(int i=0; i<m[2]; i++) {
		if(sv1[1].x[i] != 0 && sv1[1].x[i] != -3)
			return false;
	}
	for(int i=0; i<m[3]; i++) {
		if(sv2[1].x[i] != 0 && sv2[1].x[i] != -3)
			return false;
	}
	for(int i=0; i<m[4]; i++) {
		if(sv2[2].x[i] != 0 && sv2[2].x[i] != -3)
			return false;
	}
	
	//check sugli arrivi nei centri 3, 4, 5
	if(arr->head5 != NULL || arr->head4 != NULL || arr->head3 != NULL)
		return false;

	return true;

}

struct next_abandon *getMinAbandon(struct job *head) {

	struct next_abandon *min = (struct next_abandon *) malloc(sizeof(struct next_abandon));

	if(head != NULL) {
		min->jobId = head->id;
		min->abandonTime = head->abandonTime;

		struct job *current = head;

		while(current != NULL) {
			if(current->abandonTime < min->abandonTime){
				min->jobId = current->id;
				min->abandonTime = current->abandonTime;
			}
			current = current->next;
		}
	}

	else {
		min->jobId = -1;
		min->abandonTime = (double) INFINITY;
	}

	return min;

}

struct next_completion *getMinCompletion(int numServers, int *x, int index) {

	struct next_completion *min = (struct next_completion *) malloc(sizeof(struct next_completion));
	min->serverOffset = 0;
	min->isFamily = true;
	min->completionTime = (double) INFINITY;
	double * completionTimes = NULL;
	
	switch(index){
			case 1:
				completionTimes = events->completionTimes1;
				break;
			case 2:
				completionTimes = events->completionTimes2;
				break;
			case 3:
				completionTimes = events->completionTimes3;
				break;
			case 4:
				completionTimes = events->completionTimes4;
				break;
			case 5:
				completionTimes = events->completionTimes5;
				break;
			default:
				printf("ERRORE: il terzo parametro della funzione getMinCompletion() deve essere un valore intero compreso tra 1 e 5.\n");
				fflush(stdout);
				exit(-8);
	}

	for(int i=0; i<numServers; i++) {
		if(completionTimes[i] < min->completionTime) {
			min->serverOffset = i;
			min->isFamily = (bool) (x[i]==1 || x[i]==-1);
			min->completionTime = completionTimes[i];
		}
	}

	return min;

}

double getSmallest(double *values, int len) {

	double smallest = (double) INFINITY;

	for(int i=0; i<len; i++) {
		if(values[i] < smallest)
			smallest = values[i];
	}

	return smallest;

}

double getMinimumTime(int *m) {

	double minAbandon1 = (double) INFINITY;
	double minAbandon2 = (double) INFINITY;
	struct next_abandon *nextAb1 = NULL;
	struct next_abandon *nextAb2 = NULL;
	int len = 16;
	
	if(events->head1 != NULL) {
		//almeno un job che deve abbandonare
		struct next_abandon *nextAb1 = getMinAbandon(events->head1);
		minAbandon1 = nextAb1->abandonTime;
	}
	if(events->head2 != NULL) {
		struct next_abandon *nextAb2 = getMinAbandon(events->head2);
		minAbandon2 = nextAb2->abandonTime;
	}

	struct next_completion *nextCom1 = getMinCompletion(m[0], sv1[0].x, 1);
	struct next_completion *nextCom2 = getMinCompletion(m[1], sv2[0].x, 2);
	struct next_completion *nextCom3 = getMinCompletion(m[2], sv1[1].x, 3);
	struct next_completion *nextCom4 = getMinCompletion(m[3], sv2[1].x, 4);
	struct next_completion *nextCom5 = getMinCompletion(m[4], sv2[2].x, 5);

	double minService1 = nextCom1->completionTime;
	double minService2 = nextCom2->completionTime;
	double minService3 = nextCom3->completionTime;
	double minService4 = nextCom4->completionTime;
	double minService5 = nextCom5->completionTime;

	double timesToCompare[len];
	timesToCompare[0] = minAbandon1;
	timesToCompare[1] = minAbandon2;
	timesToCompare[2] = minService1;
	timesToCompare[3] = minService2;
	timesToCompare[4] = minService3;
	timesToCompare[5] = minService4;
	timesToCompare[6] = minService5;
	timesToCompare[7] = events->carArr1.carArrivalTime;
	timesToCompare[8] = events->familyArr1.familyArrivalTime;
	timesToCompare[9] = events->familyArr2.familyArrivalTime;
	timesToCompare[10] = events->carArr3.carArrivalTime;
	timesToCompare[11] = events->familyArr3.familyArrivalTime;
	timesToCompare[12] = events->familyArr4.familyArrivalTime;
	timesToCompare[13] = events->familyArr5.familyArrivalTime;
	timesToCompare[14] = events->changeInterval;
	timesToCompare[15] = events->sampling;

	free(nextAb1);
	free(nextAb2);
	free(nextCom1);
	free(nextCom2);
	free(nextCom3);
	free(nextCom4);
	free(nextCom5);

	return getSmallest(timesToCompare, len);

}

int countBusyServers(int numServers, int *serverList) {

	int count = 0;
	for(int i=0; i<numServers; i++) {
		if(serverList[i] != 0 && serverList[i] != -3)
			count++;
	}

	return count;

}

void computeAreaStrucuture(int *m){

	int xBusy1 = countBusyServers(m[0], sv1[0].x);
	a[0].service += (t->next - t->current)*xBusy1;
	a[0].queue += (t->next - t->current)*(sv1[0].qA + sv1[0].qF);
	a[0].node += (t->next - t->current)*(sv1[0].qA + sv1[0].qF + xBusy1);

	int xBusy2 = countBusyServers(m[1], sv2[0].x);
	a[1].service += (t->next - t->current)*xBusy2;
	a[1].queue += (t->next - t->current)*(sv2[0].l - xBusy2);
	a[1].node += (t->next - t->current)*(sv2[0].l);

	int xBusy3 = countBusyServers(m[2], sv1[1].x);
	a[2].service += (t->next - t->current)*xBusy3;
	a[2].queue += (t->next - t->current)*(sv1[1].qA + sv1[1].qF);
	a[2].node += (t->next - t->current)*(sv1[1].qA + sv1[1].qF + xBusy3);

	int xBusy4 = countBusyServers(m[3], sv2[1].x);
	a[3].service += (t->next - t->current)*xBusy4;
	a[3].queue += (t->next - t->current)*(sv2[1].l - xBusy4);
	a[3].node += (t->next - t->current)*(sv2[1].l);

	int xBusy5 = countBusyServers(m[4], sv2[2].x);
	a[4].service += (t->next - t->current)*xBusy5;
	a[4].node += (t->next - t->current)*xBusy5;
		
	aSampling[0].service += (t->next - t->current)*xBusy1;
	aSampling[0].queue += (t->next - t->current)*(sv1[0].qA + sv1[0].qF);
	aSampling[0].node += (t->next - t->current)*(sv1[0].qA + sv1[0].qF + xBusy1);
		
	aSampling[1].service += (t->next - t->current)*xBusy2;
	aSampling[1].queue += (t->next - t->current)*(sv2[0].l - xBusy2);
	aSampling[1].node += (t->next - t->current)*(sv2[0].l);
		
	aSampling[2].service += (t->next - t->current)*xBusy3;
	aSampling[2].queue += (t->next - t->current)*(sv1[1].qA + sv1[1].qF);
	aSampling[2].node += (t->next - t->current)*(sv1[1].qA + sv1[1].qF + xBusy3);
		
	aSampling[3].service += (t->next - t->current)*xBusy4;
	aSampling[3].queue += (t->next - t->current)*(sv2[1].l - xBusy4);
	aSampling[3].node += (t->next - t->current)*(sv2[1].l);
	
	aSampling[4].service += (t->next - t->current)*xBusy5;
	aSampling[4].node += (t->next - t->current)*xBusy5;

}

void sampling(int interval, int replica, double ****nsim, int *m)
{
	double diff;
	switch(interval){		
		case 1:
			diff=0.0;
			break;
		case 2:
			diff=7200.0;
			break;
		case 3:
			diff=10800.0;
			break;
		case 4:
			diff=21600.0;
			break;
		case 5:
			diff=32400.0;
			break;
		case 6:
			diff=46800.0;
			break;
	}
	
	for(int center=0;center<CENTERS;center++)
	{
		nsim[replica][center][interval-1][0]=a[center].service/((t->current-diff) * m[center]);		//UTILIZZAZIONE
		nsim[replica][center][interval-1][1]=a[center].queue/(t->current-diff);		//POPOLAZIONE MEDIA IN CODA
		nsim[replica][center][interval-1][2]=a[center].node/(t->current-diff);		//POPOLAZIONE MEDIA NEL CENTRO
		nsim[replica][center][interval-1][3]=a[center].service/(((al[center].compl_a-al[center].prev_compl_a) + (al[center].compl_f-al[center].prev_compl_f)));		//TEMPO MEDIO DI SERVIZIO
		nsim[replica][center][interval-1][4]=a[center].queue/((al[center].compl_a-al[center].prev_compl_a) + (al[center].compl_f-al[center].prev_compl_f));		//TEMPO MEDIO DI ATTESA
		nsim[replica][center][interval-1][5]=a[center].node/((al[center].compl_a-al[center].prev_compl_a) + (al[center].compl_f-al[center].prev_compl_f));		//TEMPO MEDIO DI RISPOSTA
		nsim[replica][center][interval-1][6]=(t->last[center]-diff)/((al[center].index_a-al[center].prev_index_a) + (al[center].index_f-al[center].prev_index_f));	//TEMPO MEDIO DI INTERARRIVO
		nsim[replica][center][interval-1][7]=(double)(al[center].index_f-al[center].prev_index_f);		//NUMERO DI ARRIVI DELLE FAMIGLIE
		nsim[replica][center][interval-1][8]=(double)(al[center].index_a-al[center].prev_index_a);		//NUMERO DI ARRIVI DELLE AUTOMOBILI
		nsim[replica][center][interval-1][9]=(double)(al[center].numLoss_f-al[center].prev_numLoss_f);	//NUMERO DI PERDITE
		nsim[replica][center][interval-1][10]=(double)(al[center].numLoss_f-al[center].prev_numLoss_f)/(double)(al[center].index_f-al[center].prev_index_f);	//PROBABILITA' DI AVERE UNA PERDITA	
	}
}

void samplingTimeFunction(int count, int replica, double ****samplingTime, int **array_m, int interval, double ***numMedioServentiAttivi)
{
	double denominatore = 0.0;
	double sizeInterval[6] = {7200.0, 3600.0, 10800.0, 10800.0, 14400.0, 3600.0};
	double diff;
	
	switch(interval){		
		case 1:
			diff=0.0;
			break;
		case 2:
			diff=7200.0;
			break;
		case 3:
			diff=10800.0;
			break;
		case 4:
			diff=21600.0;
			break;
		case 5:
			diff=32400.0;
			break;
		case 6:
			diff=46800.0;
			break;
	}
	for(int center=0;center<CENTERS;center++)
	{
		for(int i=0;i<interval-1;i++){
				denominatore += array_m[i][center]*sizeInterval[i];
		}
		denominatore += (t->current - diff)*array_m[interval-1][center];
		numMedioServentiAttivi[replica][center][count] = denominatore;

		if(denominatore==0)
			samplingTime[replica][center][0][count]=0;
		else{			
			samplingTime[replica][center][0][count]=aSampling[center].service/denominatore;		//UTILIZZAZIONE
			printf("UTILIZZAZIONE:\treplica = %d\tcount = %d\tvalore = %f\tcentro = %d\tfascia = %f\n",replica, count, samplingTime[replica][center][0][count], center, interTime);fflush(stdout);
		}
		
		if(t->current==0)
			samplingTime[replica][center][1][count]=0;
		else
			samplingTime[replica][center][1][count]=aSampling[center].queue/t->current;		//POPOLAZIONE MEDIA IN CODA
		
		if(t->current==0)
			samplingTime[replica][center][2][count]=0;
		else	
			samplingTime[replica][center][2][count]=aSampling[center].node/t->current;		//POPOLAZIONE MEDIA NEL CENTRO
		
		if((al[center].compl_a + al[center].compl_f)==0)
			samplingTime[replica][center][3][count]=0;
		else
			samplingTime[replica][center][3][count]=aSampling[center].service/(al[center].compl_a + al[center].compl_f);	//TEMPO MEDIO DI SERVIZIO
		
		if((al[center].compl_a + al[center].compl_f)==0)
			samplingTime[replica][center][4][count]=0;
		else
			samplingTime[replica][center][4][count]=aSampling[center].queue/(al[center].compl_a + al[center].compl_f);		//TEMPO MEDIO DI ATTESA
		
		if((al[center].compl_a + al[center].compl_f)==0)
			samplingTime[replica][center][5][count]=0;
		else
			samplingTime[replica][center][5][count]=aSampling[center].node/(al[center].compl_a + al[center].compl_f);		//TEMPO MEDIO DI RISPOSTA
		
		if((al[center].index_a + al[center].index_f)==0)
			samplingTime[replica][center][6][count]=0;
		else
			samplingTime[replica][center][6][count]=t->last[center]/(al[center].index_a + al[center].index_f);				//TEMPO MEDIO DI INTERARRIVO
		
		samplingTime[replica][center][7][count]=(double)(al[center].index_f);		//NUMERO DI ARRIVI DELLE FAMIGLIE
				
		samplingTime[replica][center][8][count]=(double)(al[center].index_a);		//NUMERO DI ARRIVI DELLE AUTOMOBILI
		
		samplingTime[replica][center][9][count]=(double)(al[center].numLoss_f);		//NUMERO DI PERDITE
		
		if((al[center].index_f)==0)
			samplingTime[replica][center][10][count]=0;
		else
			samplingTime[replica][center][10][count]=(double)(al[center].numLoss_f)/(double)(al[center].index_f);		//PROBABILITA' DI AVERE UNA PERDITA
		
	}
}

void applyServersVariation(int *old_m, int *new_m, struct event_list *events, struct time *t, struct state_variables1 *sv1, struct state_variables2 *sv2) {

	//CENTER 1
	if(new_m[0]-old_m[0] > 0) {
		for(int i=old_m[0]; i<new_m[0]; i++) {
			sv1[0].x[i] = 0;
		}
		fixState1(events, t, &sv1[0], old_m[0], new_m[0]-old_m[0]);
	}
	else if(new_m[0]-old_m[0] < 0) {
		for(int i=new_m[0]; i<old_m[0]; i++) {

			switch(sv1[0].x[i]) {
				case 0:
					sv1[0].x[i] = -3;	//-3 = NOT ACTIVATED
					break;
				case 1:
					sv1[0].x[i] = -1;	//-1 = BUSY_F but then it will become NOT ACTIVATED
					break;
				case 2:
					sv1[0].x[i] = -2;	//-2 = BUSY_A but then it will become NOT ACTIVATED
					break;
			}
		}
	}

	//CENTER 2
	if(new_m[1]-old_m[1] > 0) {
		for(int i=old_m[1]; i<new_m[1]; i++) {
			sv2[0].x[i] = 0;
		}
		fixState2(events, t, &sv2[0], old_m[1], new_m[1]-old_m[1]);
	}
	else if(new_m[1]-old_m[1] < 0) {
		for(int i=new_m[1]; i<old_m[1]; i++) {

			switch(sv2[0].x[i]) {
				case 0:
					sv2[0].x[i] = -3;	//-3 = NOT ACTIVATED
					break;
				case 1:
					sv2[0].x[i] = -1;	//-1 = BUSY_F but then it will become NOT ACTIVATED
					break;
			}
		}
	}

	//CENTER 3
	if(new_m[2]-old_m[2] > 0) {
		for(int i=old_m[2]; i<new_m[2]; i++) {
			sv1[1].x[i] = 0;
		}
		fixState3(events, t, &sv1[1], old_m[2], new_m[2]-old_m[2]);
	}
	else if(new_m[2]-old_m[2] < 0) {
		for(int i=new_m[2]; i<old_m[2]; i++) {

			switch(sv1[1].x[i]) {
				case 0:
					sv1[1].x[i] = -3;	//-3 = NOT ACTIVATED
					break;
				case 1:
					sv1[1].x[i] = -1;	//-1 = BUSY_F but then it will become NOT ACTIVATED
					break;
				case 2:
					sv1[1].x[i] = -2;	//-2 = BUSY_A but then it will become NOT ACTIVATED
					break;
			}
		}
	}	
}

void simulation(int **array_m, int replica, double**** nsim, double ****samplingTime, double ***numMedioServentiAttivi)
{
	int *maxArray = getMaxArray(array_m);
	int interval = 0;
	int count=0;
	int *m = array_m[0];

	while(events->carArr1.isCarArrivalActive || events->familyArr1.isFamilyArrivalActive || events->familyArr2.isFamilyArrivalActive || !isSystemEmpty(maxArray)) {

		t->next = getMinimumTime(maxArray);		//Next event time		
		computeAreaStrucuture(maxArray);
		t->current = t->next;					//Clock update
		
		struct next_abandon *nextAb1 = getMinAbandon(events->head1);
		struct next_abandon *nextAb2 = getMinAbandon(events->head2);
		struct next_completion *nextCom1 = getMinCompletion(maxArray[0], sv1[0].x, 1);
		struct next_completion *nextCom2 = getMinCompletion(maxArray[1], sv2[0].x, 2);
		struct next_completion *nextCom3 = getMinCompletion(maxArray[2], sv1[1].x, 3);
		struct next_completion *nextCom4 = getMinCompletion(maxArray[3], sv2[1].x, 4);
		struct next_completion *nextCom5 = getMinCompletion(maxArray[4], sv2[2].x, 5);

		if(t->current == events->changeInterval){
			printf("EVENTO: cambio intervallo.\n");
			if(interval!=0)
			{
				sampling(interval, replica, nsim, m);
				for(int i=0;i<CENTERS;i++)
				{
					a[i].service = 0.0;
					a[i].queue = 0.0;
					a[i].node = 0.0;
					al[i].prev_index_a = al[i].index_a; 
					al[i].prev_index_f = al[i].index_f; 
					al[i].prev_numLoss_f = al[i].numLoss_f;
					al[i].prev_compl_a = al[i].compl_a;
					al[i].prev_compl_f = al[i].compl_f;
				}
			}
			interval++;
			if(events->changeInterval==0.0){			
				interTime = 180.0;
				events->changeInterval = 7200.0;
				printf("AAAA\n");fflush(stdout);
			}
			else if(events->changeInterval==7200.0){			
				interTime = 480.0;
				events->changeInterval = 10800.0;
				m = array_m[1];
				applyServersVariation(array_m[0], m, events, t, sv1, sv2);
				printf("AAAA\t%d\n", m[2]);fflush(stdout);
			}
			else if(events->changeInterval==10800.0){
				interTime = 60.0;
				events->changeInterval = 21600.0;
				m = array_m[2];
				applyServersVariation(array_m[1], m, events, t, sv1, sv2);
				printf("AAAA\t%d\n", m[2]);fflush(stdout);
			}
			else if(events->changeInterval==21600.0){
				interTime = 300.0;
				events->changeInterval = 32400.0;
				m = array_m[3];
				applyServersVariation(array_m[2], m, events, t, sv1, sv2);
				printf("AAAA\t%d\n", m[2]);fflush(stdout);
			}
			else if(events->changeInterval==32400.0){
				interTime = 30.0;
				events->changeInterval = 46800.0;
				m = array_m[4];
				applyServersVariation(array_m[3], m, events, t, sv1, sv2);
				printf("AAAA\t%d\n", m[2]);fflush(stdout);
			}
			else if(events->changeInterval==46800.0){
				interTime = 180.0;
				events->changeInterval = (double) INFINITY;
				m = array_m[5];
				applyServersVariation(array_m[4], m, events, t, sv1, sv2);
				printf("AAAA\t%d\n", m[2]);fflush(stdout);
			}
		}

		else if(t->current == events->sampling){
			printf("EVENTO: sampling temporale num %d.\n", count);
			samplingTimeFunction(count, replica, samplingTime, array_m, interval, numMedioServentiAttivi);
			if(t->current + SAMPLINGINTERVAL <= STOP){
				events->sampling += SAMPLINGINTERVAL;
				count++;
			}
			else
				events->sampling = (double) INFINITY;
		}

		else if(t->current == events->carArr1.carArrivalTime) {
			printf("EVENTO: arrivo di un'automobile nel centro 1.\n");
			carArrival1(events, t, &sv1[0], &al[0], maxArray[0], true);
		}
		else if(t->current == events->familyArr1.familyArrivalTime) {
			printf("EVENTO: arrivo di una famiglia nel centro 1.\n");
			familyArrival1(events, t, &sv1[0], &al[0], maxArray[0], true);
		}
		else if(t->current == nextCom1->completionTime && !nextCom1->isFamily) {
			printf("EVENTO: partenza di un'automobile dal centro 1.\n");
			carDeparture1(events, t, &sv1[0], arr, nextCom1->serverOffset, m[0], &al[0]);	//Qui m[0] al posto di maxArray[0] è giusto
		}
		else if(t->current == nextCom1->completionTime && nextCom1->isFamily) {
			printf("EVENTO: partenza di una famiglia dal centro 1.\n");
			familyDeparture1(events, t, &sv1[0], arr, nextCom1->serverOffset, maxArray[0], &al[0]);
		}
		else if(t->current == nextAb1->abandonTime) {
			printf("EVENTO: abbandono di una famiglia dal centro 1.\n");
			abandon1(events, &sv1[0], &al[0], nextAb1->jobId);
		}
		else if(t->current == events->familyArr2.familyArrivalTime) {
			printf("EVENTO: arrivo di una famiglia nel centro 2.\n");
			arrival2(events, t, &sv2[0], &al[1], maxArray[1], true);
		}
		else if(t->current == nextCom2->completionTime) {
			printf("EVENTO: partenza di una famiglia dal centro 2.\n");
			departure2(events, t, &sv2[0], arr, nextCom2->serverOffset, m[1], &al[1]);	//Qui m[1] al posto di maxArray[1] è giusto
		}
		else if(t->current == nextAb2->abandonTime) {
			printf("EVENTO: abbandono di una famiglia dal centro 2.\n");
			abandon2(events, &sv2[0], &al[1], nextAb2->jobId);
		}
		else if(t->current == events->carArr3.carArrivalTime) {
			printf("EVENTO: arrivo di un'automobile nel centro 3.\n");
			carArrival3(events, t, &sv1[1], &al[2], arr, maxArray[2]);
		}
		else if(t->current == events->familyArr3.familyArrivalTime) {
			printf("EVENTO: arrivo di una famiglia nel centro 3.\n");
			familyArrival3(events, t, &sv1[1], &al[2], arr, maxArray[2]);
		}
		else if(t->current == nextCom3->completionTime && !nextCom3->isFamily) {
			printf("EVENTO: partenza di un'automobile dal centro 3.\n");
			carDeparture3(events, t, &sv1[1], arr, nextCom3->serverOffset, m[2], &al[2]);	//Qui m[2] al posto di maxArray[2] è giusto
		}
		else if(t->current == nextCom3->completionTime && nextCom3->isFamily) {
			printf("EVENTO: partenza di una famiglia dal centro 3.\n");
			familyDeparture3(events, t, &sv1[1], arr, nextCom3->serverOffset, maxArray[2], &al[2]);
		}
		else if(t->current == events->familyArr4.familyArrivalTime) {
			printf("EVENTO: arrivo di una famiglia nel centro 4.\n");
			arrival4(events, t, &sv2[1], &al[3], arr, maxArray[3]);
		}
		else if(t->current == nextCom4->completionTime) {
			printf("EVENTO: partenza di una famiglia dal centro 4.\n");
			departure4(events, t, &sv2[1], arr, nextCom4->serverOffset, m[3], &al[3]);	//Qui m[3] al posto di maxArray[3] è giusto
		}
		else if(t->current == events->familyArr5.familyArrivalTime) {
			printf("EVENTO: arrivo di una famiglia nel centro 5.\n");
			arrival5(events, t, &sv2[2], &al[4], arr, maxArray[4]);
		}
		else if(t->current == nextCom5->completionTime) {
			printf("EVENTO: partenza di una famiglia dal centro 5.\n");
			departure5(events, &sv2[2], nextCom5->serverOffset, &al[4]);
		}

		/*if(t->current < 50000.0) {
			printf("Clock: %f\n", t->current);

			printf("qA centro 1: %d\n", sv1[0].qA);
			printf("qF centro 1: %d\n", sv1[0].qF);
			printf("Serventi centro 1: ");
			for(int k=0; k<maxArray[0]; k++) {
				printf("%d, ", sv1[0].x[k]);
			}
			printf("\n");
			printf("l centro 2: %d\n", sv2[0].l);
			printf("Serventi centro 2: ");
			for(int k=0; k<maxArray[1]; k++) {
				printf("%d, ", sv2[0].x[k]);
			}
			printf("\n");
			printf("qA centro 3: %d\n", sv1[1].qA);
			printf("qF centro 3: %d\n", sv1[1].qF);
			printf("Serventi centro 3: ");
			for(int k=0; k<maxArray[2]; k++) {
				printf("%d, ", sv1[1].x[k]);
			}
			printf("\n");
			printf("l centro 4: %d\n", sv2[1].l);
			printf("Serventi centro 4: ");
			for(int k=0; k<maxArray[3]; k++) {
				printf("%d, ", sv2[1].x[k]);
			}
			printf("\n");
			printf("Serventi centro 5: ");
			for(int k=0; k<maxArray[4]; k++) {
				printf("%d, ", sv2[2].x[k]);
			}
			printf("\n");

			for(int k=0; k<CENTERS; k++) {
				printf("ALCUNE STATISTICHE PER IL CENTRO %d:\n", k+1);
				printf("aSampling[%d].service = %f\n", k, aSampling[k].service);
				printf("aSampling[%d].queue = %f\n", k, aSampling[k].queue);
				printf("aSampling[%d].node = %f\n", k, aSampling[k].node);
				printf("al[%d].compl_a = %d\n", k, al[k].compl_a);
				printf("al[%d].compl_f = %d\n", k, al[k].compl_f);
				printf("al[%d].index_a = %d\n", k, al[k].index_a);
				printf("al[%d].index_f = %d\n", k, al[k].index_f);
				printf("al[%d].numLoss_f = %d\n", k, al[k].numLoss_f);
			}
			printf("\n");
		}*/

		free(nextAb1);
		free(nextAb2);
		free(nextCom1);
		free(nextCom2);
		free(nextCom3);
		free(nextCom4);
		free(nextCom5);			

	}
	
	sampling(interval, replica, nsim, m);	
	
}

struct result_finite *finite_sim(int **array_m)
{
	PlantSeeds(7000);	
	
	double ****nsim = (double ****) malloc(sizeof(double***)*REPLICATIONS);
	double ****samplingTime = (double ****)malloc(sizeof(double ***)*REPLICATIONS);
	double ***numMedioServentiAttivi = (double ***)malloc(sizeof(double **)*REPLICATIONS);
	if(nsim==NULL || samplingTime==NULL || numMedioServentiAttivi==NULL)
		errorMalloc(-1007);
		
	
	for(int i=0; i<REPLICATIONS; i++)
	{
		nsim[i] = (double ***)malloc(sizeof(double **)*CENTERS);
		if(nsim[i]==NULL)
			errorMalloc(-1008);

		for(int j=0; j<CENTERS; j++)
		{
			nsim[i][j] = (double**)malloc(sizeof(double*)*INTERVALS);
			if(nsim[i][j]==NULL)
				errorMalloc(-1009);

			for(int k=0; k<INTERVALS; k++)
			{
				nsim[i][j][k] = (double*)malloc(sizeof(double)*STATISTICS);
				if(nsim[i][j][k]==NULL)
				errorMalloc(-1010);
			}
		}
	}	
	
	int samplingSize = STOP/SAMPLINGINTERVAL;
	for(int i=0; i<REPLICATIONS; i++)
	{
		samplingTime[i] = (double ***)malloc(sizeof(double **)*CENTERS);
		if(samplingTime[i]==NULL)
			errorMalloc(-1011);

		for(int j=0; j<CENTERS; j++)
		{
			samplingTime[i][j] = (double**)malloc(sizeof(double*)*STATISTICS);
			if(samplingTime[i][j]==NULL)
				errorMalloc(-1012);

			for(int k=0; k<STATISTICS; k++)
			{
				samplingTime[i][j][k] = (double*)malloc(sizeof(double)*samplingSize);
				if(samplingTime[i][j][k]==NULL)
				errorMalloc(-1013);
			}
		}
	}

	for(int i=0; i<REPLICATIONS; i++)
	{
		numMedioServentiAttivi[i] = (double **)malloc(sizeof(double *)*CENTERS);
		if(numMedioServentiAttivi[i]==NULL)
			errorMalloc(-1042);

		for(int j=0; j<CENTERS; j++)
		{
			numMedioServentiAttivi[i][j] = (double*)malloc(sizeof(double)*samplingSize);
			if(numMedioServentiAttivi[i][j]==NULL)
				errorMalloc(-1043);

		}
	}		
	
	//finite-horizon simulation
	for(int i=0; i<REPLICATIONS; i++)
	{
		initialize(array_m);
		simulation(array_m, i, nsim, samplingTime, numMedioServentiAttivi);
		deallocateDataStructures();

	}	
	//computeInterval();
	
	struct result_finite *ret = (struct result_finite *)malloc(sizeof(struct result_finite));
	if(ret==NULL)
		errorMalloc(-1044);

	ret->nsim = nsim;
	ret->samplingTime = samplingTime;
	ret->numMedioServentiAttivi = numMedioServentiAttivi;
	
	return ret;
}

void samplingBatch(int centerIndex, int *count, double ***siminf, double **sum, double **mean, double diff, int *m)
{
	double diffWelford;
	
	//UTILIZZAZIONE
	siminf[centerIndex][0][(count[centerIndex]/B)-1]=a[centerIndex].service/((t->current-diff) * m[centerIndex]);
	//POPOLAZIONE MEDIA IN CODA
	siminf[centerIndex][1][(count[centerIndex]/B)-1]=a[centerIndex].queue/(t->current-diff);
	//POPOLAZIONE MEDIA NEL CENTRO
	siminf[centerIndex][2][(count[centerIndex]/B)-1]=a[centerIndex].node/(t->current-diff);
	//TEMPO MEDIO DI SERVIZIO
	siminf[centerIndex][3][(count[centerIndex]/B)-1]=a[centerIndex].service/(((al[centerIndex].compl_a-al[centerIndex].prev_compl_a) + (al[centerIndex].compl_f-al[centerIndex].prev_compl_f)));
	//TEMPO MEDIO DI ATTESA
	siminf[centerIndex][4][(count[centerIndex]/B)-1]=a[centerIndex].queue/((al[centerIndex].compl_a-al[centerIndex].prev_compl_a) + (al[centerIndex].compl_f-al[centerIndex].prev_compl_f));
	//TEMPO MEDIO DI RISPOSTA
	siminf[centerIndex][5][(count[centerIndex]/B)-1]=a[centerIndex].node/((al[centerIndex].compl_a-al[centerIndex].prev_compl_a) + (al[centerIndex].compl_f-al[centerIndex].prev_compl_f));
	//TEMPO MEDIO DI INTERARRIVO
	siminf[centerIndex][6][(count[centerIndex]/B)-1]=(t->last[centerIndex]-diff)/((al[centerIndex].index_a-al[centerIndex].prev_index_a) + (al[centerIndex].index_f-al[centerIndex].prev_index_f));
	//NUMERO DI ARRIVI DELLE FAMIGLIE
	siminf[centerIndex][7][(count[centerIndex]/B)-1]=(double)(al[centerIndex].index_f-al[centerIndex].prev_index_f);
	//NUMERO DI ARRIVI DELLE AUTOMOBILI
	siminf[centerIndex][8][(count[centerIndex]/B)-1]=(double)(al[centerIndex].index_a-al[centerIndex].prev_index_a);
	//NUMERO DI PERDITE
	siminf[centerIndex][9][(count[centerIndex]/B)-1]=(double)(al[centerIndex].numLoss_f - al[centerIndex].prev_numLoss_f);
	//PROBABILITA' DI AVERE UNA PERDITA
	siminf[centerIndex][10][(count[centerIndex]/B)-1]=(double)(al[centerIndex].numLoss_f - al[centerIndex].prev_numLoss_f)/(double)(al[centerIndex].index_f-al[centerIndex].prev_index_f);	
	
	int n = count[centerIndex]/B; 

	for(int i=0; i<STATISTICS; i++)
	{
		diffWelford= siminf[centerIndex][i][(count[centerIndex]/B)-1] - mean[centerIndex][i];
		sum[centerIndex][i] += diffWelford * diffWelford * (n - 1.0) / n;
		mean[centerIndex][i] += diffWelford / n;

		//NEW
		siminf[centerIndex][i][(count[centerIndex]/B)-1] = mean[centerIndex][i];
	}
			
	a[centerIndex].service = 0.0;
	a[centerIndex].queue = 0.0;
	a[centerIndex].node = 0.0;
	al[centerIndex].prev_index_a = al[centerIndex].index_a; 
	al[centerIndex].prev_index_f = al[centerIndex].index_f; 
	al[centerIndex].prev_numLoss_f = al[centerIndex].numLoss_f;
	al[centerIndex].prev_compl_a = al[centerIndex].compl_a;
	al[centerIndex].prev_compl_f = al[centerIndex].compl_f;  
}


double*** infinite_sim(int *m)
{
	PlantSeeds(8000);
	
	double ***siminf = (double ***)malloc(sizeof(double**)*CENTERS);	
	if(siminf==NULL)
		errorMalloc(-1014);

	for(int i=0; i<CENTERS; i++)
	{
		siminf[i] = (double **)malloc(sizeof(double *)*STATISTICS);
		if(siminf[i]==NULL)
			errorMalloc(-1015);

		for(int j=0;j<STATISTICS;j++){
			siminf[i][j] = (double *)malloc(sizeof(double)*K);
			if(siminf[i][j]==NULL)
				errorMalloc(-1016);
		}
	}
	
	double **sum = (double **)malloc(sizeof(double *)*CENTERS);
	if(sum==NULL)
		errorMalloc(-1017);
		
	for(int i=0;i<CENTERS;i++)
	{
		sum[i] = (double *)malloc(sizeof(double)*STATISTICS);
		if(sum[i]==NULL)
			errorMalloc(-1018);
	}
	
	double **mean = (double **)malloc(sizeof(double *)*CENTERS);
	if(mean==NULL)
		errorMalloc(-1019);
		
	for(int i=0;i<CENTERS;i++)
	{
		mean[i] = (double *)malloc(sizeof(double)*STATISTICS);
		if(mean[i]==NULL)
			errorMalloc(-1020);
	}
	
	initializeSimInf(m);
	
	int count[5] = {0,0,0,0,0};
	int rimanenti;
	double startBatch[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
	
	while(true){
	
		rimanenti=0;
		for(int i=0;i<5;i++){
			if(count[i]<B*K)
				rimanenti++;
		}
		if(rimanenti==0)
			break;
			
		t->next = getMinimumTime(m);		//Next event time			
		computeAreaStrucuture(m);
		t->current = t->next;				//Clock update
		
		struct next_abandon *nextAb1 = getMinAbandon(events->head1);
		struct next_abandon *nextAb2 = getMinAbandon(events->head2);
		struct next_completion *nextCom1 = getMinCompletion(m[0], sv1[0].x, 1);
		struct next_completion *nextCom2 = getMinCompletion(m[1], sv2[0].x, 2);
		struct next_completion *nextCom3 = getMinCompletion(m[2], sv1[1].x, 3);
		struct next_completion *nextCom4 = getMinCompletion(m[3], sv2[1].x, 4);
		struct next_completion *nextCom5 = getMinCompletion(m[4], sv2[2].x, 5);

		
		if(t->current == events->carArr1.carArrivalTime) {
			printf("EVENTO: arrivo di un'automobile nel centro 1.\n");
			printf("CLOCK: %f\n", t->current);fflush(stdout);
			carArrival1(events, t, &sv1[0], &al[0], m[0], false);
			if(count[0] < B * K){
				count[0]+=1;
				if(count[0]%B==0)
				{
					//Mi calcolo la batch mean e poi uso Welford per calcolarmi la media e la varianza del campione delle medie batches
					samplingBatch(0,count, siminf, sum,mean, startBatch[0], m);
					startBatch[0] = t->current;
				}
			}
		}
		else if(t->current == events->familyArr1.familyArrivalTime) {
			printf("EVENTO: arrivo di una famiglia nel centro 1.\n");
			familyArrival1(events, t, &sv1[0], &al[0], m[0], false);
			if(count[0] < B * K){
				count[0]+=1;
				if(count[0]%B==0)
				{
					//Mi calcolo la batch mean e poi uso Welford per calcolarmi la media e la varianza del campione delle medie batches
					samplingBatch(0,count, siminf, sum,mean, startBatch[0], m);    
					startBatch[0] = t->current;
				}
			}
		}
		else if(t->current == nextCom1->completionTime && !nextCom1->isFamily) {
			printf("EVENTO: partenza di un'automobile dal centro 1.\n");
			carDeparture1(events, t, &sv1[0], arr, nextCom1->serverOffset, m[0], &al[0]);
		}
		else if(t->current == nextCom1->completionTime && nextCom1->isFamily) {
			printf("EVENTO: partenza di una famiglia dal centro 1.\n");
			familyDeparture1(events, t, &sv1[0], arr, nextCom1->serverOffset, m[0], &al[0]);
		}
		else if(t->current == nextAb1->abandonTime) {
			printf("EVENTO: abbandono di una famiglia dal centro 1.\n");
			abandon1(events, &sv1[0], &al[0], nextAb1->jobId);
		}
		else if(t->current == events->familyArr2.familyArrivalTime) {
			printf("EVENTO: arrivo di una famiglia nel centro 2.\n");
			arrival2(events, t, &sv2[0], &al[1], m[1], false);
			if(count[1] < B * K){
				count[1]+=1;			
				if(count[1]%B==0)
				{	
					//Mi calcolo la batch mean e poi uso Welford per calcolarmi la media e la varianza del campione delle medie batches
					samplingBatch(1,count, siminf, sum,mean, startBatch[1], m);    
					startBatch[1] = t->current;
				}
			}
		}
		else if(t->current == nextCom2->completionTime) {
			printf("EVENTO: partenza di una famiglia dal centro 2.\n");
			departure2(events, t, &sv2[0], arr, nextCom2->serverOffset, m[1], &al[1]);	//Qui m[1] al posto di maxArray[1] è giusto
		}
		else if(t->current == nextAb2->abandonTime) {
			printf("EVENTO: abbandono di una famiglia dal centro 2.\n");
			abandon2(events, &sv2[0], &al[1], nextAb2->jobId);
		}
		else if(t->current == events->carArr3.carArrivalTime) {
			printf("EVENTO: arrivo di un'automobile nel centro 3.\n");
			carArrival3(events, t, &sv1[1], &al[2], arr, m[2]);
			if(count[2] < B * K){
				count[2]+=1;
				if(count[2]%B==0)
				{
					//Mi calcolo la batch mean e poi uso Welford per calcolarmi la media e la varianza del campione delle medie batches
					samplingBatch(2,count, siminf, sum,mean, startBatch[2], m);   
					startBatch[2] = t->current;
				}
			}
		}
		else if(t->current == events->familyArr3.familyArrivalTime) {
			printf("EVENTO: arrivo di una famiglia nel centro 3.\n");
			familyArrival3(events, t, &sv1[1], &al[2], arr, m[2]);
			if(count[2] < B * K){
				count[2]+=1;
				if(count[2]%B==0)
				{
					//Mi calcolo la batch mean e poi uso Welford per calcolarmi la media e la varianza del campione delle medie batches
					samplingBatch(2,count, siminf, sum,mean, startBatch[2], m);    
					startBatch[2] = t->current;
				}
			}
		}
		else if(t->current == nextCom3->completionTime && !nextCom3->isFamily) {
			printf("EVENTO: partenza di un'automobile dal centro 3.\n");
			carDeparture3(events, t, &sv1[1], arr, nextCom3->serverOffset, m[2], &al[2]);
		}
		else if(t->current == nextCom3->completionTime && nextCom3->isFamily) {
			printf("EVENTO: partenza di una famiglia dal centro 3.\n");
			familyDeparture3(events, t, &sv1[1], arr, nextCom3->serverOffset, m[2], &al[2]);
		}
		else if(t->current == events->familyArr4.familyArrivalTime) {
			printf("EVENTO: arrivo di una famiglia nel centro 4.\n");
			arrival4(events, t, &sv2[1], &al[3], arr, m[3]);
			if(count[3] < B * K){
				count[3]+=1;
				if(count[3]%B==0)
				{
					//Mi calcolo la batch mean e poi uso Welford per calcolarmi la media e la varianza del campione delle medie batches
					samplingBatch(3,count, siminf, sum,mean,startBatch[3], m);   
					startBatch[3] = t->current;
				}
			}
		}
		else if(t->current == nextCom4->completionTime) {
			printf("EVENTO: partenza di una famiglia dal centro 4.\n");
			departure4(events, t, &sv2[1], arr, nextCom4->serverOffset, m[3], &al[3]);	//Qui m[3] al posto di maxArray[3] è giusto
		}
		else if(t->current == events->familyArr5.familyArrivalTime) {
			printf("EVENTO: arrivo di una famiglia nel centro 5.\n");
			arrival5(events, t, &sv2[2], &al[4], arr, m[4]);
			if(count[4] < B * K){
				count[4]+=1;
				if(count[4]%B==0)
				{
					//Mi calcolo la batch mean e poi uso Welford per calcolarmi la media e la varianza del campione delle medie batches
					samplingBatch(4,count, siminf, sum,mean, startBatch[4], m);    
					startBatch[4] = t->current;
				}
			}
		}
		else if(t->current == nextCom5->completionTime) {
			printf("EVENTO: partenza di una famiglia dal centro 5.\n");
			departure5(events, &sv2[2], nextCom5->serverOffset, &al[4]);
		}

		/*if(true) {
			printf("Clock: %f\n", t->current);

			printf("qA centro 1: %d\n", sv1[0].qA);
			printf("qF centro 1: %d\n", sv1[0].qF);
			printf("Serventi centro 1: ");
			for(int k=0; k<maxArray[0]; k++) {
				printf("%d, ", sv1[0].x[k]);
			}
			printf("\n");
			printf("l centro 2: %d\n", sv2[0].l);
			printf("Serventi centro 2: ");
			for(int k=0; k<maxArray[1]; k++) {
				printf("%d, ", sv2[0].x[k]);
			}
			printf("\n");
			printf("qA centro 3: %d\n", sv1[1].qA);
			printf("qF centro 3: %d\n", sv1[1].qF);
			printf("Serventi centro 3: ");
			for(int k=0; k<maxArray[2]; k++) {
				printf("%d, ", sv1[1].x[k]);
			}
			printf("\n");
			printf("l centro 4: %d\n", sv2[1].l);
			printf("Serventi centro 4: ");
			for(int k=0; k<maxArray[3]; k++) {
				printf("%d, ", sv2[1].x[k]);
			}
			printf("\n");
			printf("Serventi centro 5: ");
			for(int k=0; k<maxArray[4]; k++) {
				printf("%d, ", sv2[2].x[k]);
			}
			printf("\n");

			for(int k=0; k<CENTERS; k++) {
				printf("ALCUNE STATISTICHE PER IL CENTRO %d:\n", k+1);
				printf("aSampling[%d].service = %f\n", k, aSampling[k].service);
				printf("aSampling[%d].queue = %f\n", k, aSampling[k].queue);
				printf("aSampling[%d].node = %f\n", k, aSampling[k].node);
				printf("al[%d].compl_a = %d\n", k, al[k].compl_a);
				printf("al[%d].compl_f = %d\n", k, al[k].compl_f);
				printf("al[%d].index_a = %d\n", k, al[k].index_a);
				printf("al[%d].index_f = %d\n", k, al[k].index_f);
				printf("al[%d].numLoss_f = %d\n", k, al[k].numLoss_f);
			}
			printf("\n");
		}*/

		free(nextAb1);
		free(nextAb2);
		free(nextCom1);
		free(nextCom2);
		free(nextCom3);
		free(nextCom4);
		free(nextCom5);						
			
	}

	/*
	double **stdv = (double **)malloc(sizeof(double*)*CENTERS);
	if(stdv==NULL)
		errorMalloc(9004);
	for(int j=0;j<STATISTICS;j++){
		stdv[j]=(double *)malloc(sizeof(double))
		if(stdv[j]==NULL)
			errorMalloc(9004);
	}*/
	
	double **w = (double **)malloc(sizeof(double*)*CENTERS);
	if(w==NULL)
		errorMalloc(-1040);

	for(int j=0; j<STATISTICS; j++){
		w[j]=(double *)malloc(sizeof(double));
		if(w[j]==NULL)
			errorMalloc(-1041);
	}
	
	int n;
	double u, t, stdv;
	
	for(int j=0; j<STATISTICS; j++){
		for(int i=0; i<CENTERS; i++)
		{
			n = count[i]/B;
			stdv = sqrt(sum[i][j] / n);			
    			u = 1.0 - 0.5 * (1.0 - LOC);              /* interval parameter  */
    			t = idfStudent(n - 1, u);                 /* critical value of t */
    			w[i][j] = t * stdv / sqrt(n - 1);         /* interval half width */

    			printf("\nINTERVALLO statistica %d\tcentro %d\n",j,i);
    			printf("INTERVALLO based upon %d data points", n);
    			printf(" INTERVALLO and with %d%% confidence\n", (int) (100.0 * LOC + 0.5));
    			printf("INTERVALLO the expected value is in the interval");
    			printf(" INTERVALLO %10.6f +/- %6.6f\n", mean[i][j], w[i][j]);
		}
	}
	
	deallocateDataStructures();

	return siminf;
	
}
