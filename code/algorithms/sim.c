#include<stdio.h>
#include "../headers/servicenode1.h"
#include "../headers/servicenode2.h"
#include "../headers/servicenode3.h"
#include "../headers/servicenode4.h"
#include "../headers/servicenode5.h"
#include "../headers/rngs.h"
#include "../headers/rvms.h"
#include "../headers/sim.h"
#include "../headers/randomGeneratorFunctions.h"

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

	printf("ERRORE: impossibile allocare nuova memoria nell'heap.\n");
	fflush(stdout);
	exit(code);

}

void errorVerify(int code) {

	printf("ERRORE: le statistiche di output della simulazione non sono consistenti.\n");
	fflush(stdout);
	exit(code);

}

void allocateDataStructures() {

	events = (struct event_list *) malloc(sizeof(struct event_list));
	if(events==NULL)
		errorMalloc(-1001);
		
	t = (struct time *) malloc(sizeof(struct time));
	if(t==NULL)
		errorMalloc(-1002);
		
	a = (struct area *) malloc(sizeof(struct area)*5);
	if(a==NULL)
		errorMalloc(-1003);	
		
	sv1 = (struct state_variables1 *) malloc(sizeof(struct state_variables1) * 2);
	if(sv1==NULL)
		errorMalloc(-1004);
		
	sv2 = (struct state_variables2 *) malloc(sizeof(struct state_variables2)*3);
	if(sv2==NULL)
		errorMalloc(-1005);
		
	al = (struct arrival_loss *) malloc(sizeof(struct arrival_loss)*5);
	if(al ==NULL)
		errorMalloc(-1006);
		
	arr = (struct arrivals *) malloc(sizeof(struct arrivals));
	if(arr == NULL)
		errorMalloc(-1007);
		
	aSampling = (struct area *) malloc(sizeof(struct area)*5);
	if(aSampling==NULL)
		errorMalloc(-1008);
	
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

void initializeEventList(int *m) {
	//m is maxArray

	events->carArr1.carArrivalTime = getCarArrival(START);
	events->carArr1.isCarArrivalActive = true;
	events->familyArr1.familyArrivalTime = getFamilyArrival1(START);
	events->familyArr1.isFamilyArrivalActive = true;

	events->familyArr2.familyArrivalTime = getFamilyArrival2(START);
	events->familyArr2.isFamilyArrivalActive = true;

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
		errorMalloc(-1008);
	events->completionTimes2 = (double *) malloc(sizeof(double)*m[1]);
	if(events->completionTimes2==NULL)
		errorMalloc(-1009);
	events->completionTimes3 = (double *) malloc(sizeof(double)*m[2]);
	if(events->completionTimes3==NULL)
		errorMalloc(-1010);
	events->completionTimes4 = (double *) malloc(sizeof(double)*m[3]);
	if(events->completionTimes4==NULL)
		errorMalloc(-1011);
	events->completionTimes5 = (double *) malloc(sizeof(double)*m[4]);
	if(events->completionTimes5==NULL)
		errorMalloc(-1012);

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
	
	events-> changeInterval = 0.0;
	events -> sampling = SAMPLINGINTERVAL;

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
		errorMalloc(-1013);
	
	for(int i=0; i<m[0]; i++) {
		sv1[0].x[i] = 0;	//0=IDLE, 1=BUSY_F, 2=BUSY_A
	}
	for(int i=m[0]; i<maxArray[0]; i++) {
		sv1[0].x[i] = -1;	//-1 = NOT ACTIVATED
	}

	sv2[0].l = 0;
	sv2[0].x = (int *) malloc(sizeof(int)*maxArray[1]);
	if(sv2[0].x==NULL)
		errorMalloc(-1014);

	for(int i=0; i<m[1]; i++) {
		sv2[0].x[i] = 0;	//0=IDLE, 1=BUSY
	}
	for(int i=m[1]; i<maxArray[1]; i++) {
		sv2[0].x[i] = -1;	//-1 = NOT ACTIVATED
	}

	sv1[1].qA = 0;
	sv1[1].qF = 0;
	sv1[1].x = (int *) malloc(sizeof(int)*maxArray[2]);
	if(sv1[1].x==NULL)
		errorMalloc(-1015);

	for(int i=0; i<m[2]; i++) {
		sv1[1].x[i] = 0;	//0=IDLE, 1=BUSY_F, 2=BUSY_A
	}
	for(int i=m[2]; i<maxArray[2]; i++) {
		sv1[1].x[i] = -1;	//-1 = NOT ACTIVATED
	}

	sv2[1].l = 0;
	sv2[1].x = (int *) malloc(sizeof(int)*maxArray[3]);
	if(sv2[1].x==NULL)
		errorMalloc(-1016);

	for(int i=0; i<m[3]; i++) {
		sv2[1].x[i] = 0;	//0=IDLE, 1=BUSY
	}
	for(int i=m[3]; i<maxArray[3]; i++) {
		sv2[1].x[i] = -1;	//-1 = NOT ACTIVATED
	}

	sv2[2].l = 0;  			//unuseful
	sv2[2].x = (int *) malloc(sizeof(int)*maxArray[4]);
	if(sv2[2].x==NULL)
		errorMalloc(-1017);

	for(int i=0; i<m[4]; i++) {
		sv2[2].x[i] = 0;	//0=IDLE, 1=BUSY
	}
	for(int i=m[4]; i<maxArray[4]; i++) {
		sv2[2].x[i] = -1;	//-1 = NOT ACTIVATED
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
		errorMalloc(-2003);

	for(int j=0; j<CENTERS; j++) {
		maxArray[j] = array_m[0][j];
	}

	for(int i=0; i<6; i++) {
		for(int j=0; j<CENTERS; j++) {
			if(maxArray[j] < array_m[i][j])
				maxArray[j] = array_m[i][j];
		}

	}

	for(int j=0; j<CENTERS; j++) {
		printf("%d ", maxArray[j]);
	}
	printf("\n");
	fflush(stdout);

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
	initializeEventList(maxArray);	
}


bool isSystemEmpty(int *m) {

	if(sv1[0].qA > 0 || sv1[0].qF > 0 || sv1[1].qA > 0 || sv1[1].qF > 0 || sv2[0].l > 0 || sv2[1].l > 0)
		return false;
		
	for(int i=0; i<m[0]; i++) {
		if(sv1[0].x[i] != 0)
			return false;
	}
	for(int i=0; i<m[1]; i++) {
		if(sv2[0].x[i] != 0)
			return false;
	}
	for(int i=0; i<m[2]; i++) {
		if(sv1[1].x[i] != 0)
			return false;
	}
	for(int i=0; i<m[3]; i++) {
		if(sv2[1].x[i] != 0)
			return false;
	}
	for(int i=0; i<m[4]; i++) {
		if(sv2[2].x[i] != 0)
			return false;
	}
	
	//check sulle code
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
				exit(-4);
	}

	for(int i=0; i<numServers; i++) {
		if(completionTimes[i] < min->completionTime) {
			min->serverOffset = i;
			min->isFamily = (bool) (x[i]==1);
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
	//printf("%f\n", timesToCompare[0]);
	timesToCompare[1] = minAbandon2;
	//printf("%f\n", timesToCompare[1]);
	timesToCompare[2] = minService1;
	//printf("%f\n", timesToCompare[2]);
	timesToCompare[3] = minService2;
	//printf("%f\n", timesToCompare[3]);
	timesToCompare[4] = minService3;
	//printf("%f\n", timesToCompare[4]);
	timesToCompare[5] = minService4;
	//printf("%f\n", timesToCompare[5]);
	timesToCompare[6] = minService5;
	//printf("%f\n", timesToCompare[6]);
	timesToCompare[7] = events->carArr1.carArrivalTime;
	//printf("%f\n", timesToCompare[7]);
	timesToCompare[8] = events->familyArr1.familyArrivalTime;
	//printf("%f\n", timesToCompare[8]);
	timesToCompare[9] = events->familyArr2.familyArrivalTime;
	//printf("%f\n", timesToCompare[9]);
	timesToCompare[10] = events->carArr3.carArrivalTime;
	//printf("%f\n", timesToCompare[10]);
	timesToCompare[11] = events->familyArr3.familyArrivalTime;
	//printf("%f\n", timesToCompare[11]);
	timesToCompare[12] = events->familyArr4.familyArrivalTime;
	//printf("%f\n", timesToCompare[12]);
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
		if(serverList[i] > 0)
			count++;
	}

	return count;

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
		nsim[replica][center][interval-1][0]=a[center].service/((t->current-diff) * m[center]);
		nsim[replica][center][interval-1][1]=a[center].queue/(t->current-diff);
		nsim[replica][center][interval-1][2]=a[center].node/(t->current-diff);
		nsim[replica][center][interval-1][3]=a[center].service/(((al[center].compl_a-al[center].prev_compl_a) + (al[center].compl_f-al[center].prev_compl_f)));
		nsim[replica][center][interval-1][4]=a[center].queue/((al[center].compl_a-al[center].prev_compl_a) + (al[center].compl_f-al[center].prev_compl_f));
		nsim[replica][center][interval-1][5]=a[center].node/((al[center].compl_a-al[center].prev_compl_a) + (al[center].compl_f-al[center].prev_compl_f));
		nsim[replica][center][interval-1][6]=(t->last[center]-diff)/((al[center].compl_a-al[center].prev_compl_a) + (al[center].compl_f-al[center].prev_compl_f));
		nsim[replica][center][interval-1][7]=(double)(al[center].index_f-al[center].prev_index_f);
		nsim[replica][center][interval-1][8]=(double)(al[center].index_a-al[center].prev_index_a);
		nsim[replica][center][interval-1][9]=(double)(al[center].numLoss_f - al[center].prev_numLoss_f);
		nsim[replica][center][interval-1][10]=(double)(al[center].numLoss_f - al[center].prev_numLoss_f)/(double)(al[center].index_f-al[center].prev_index_f);		
	}
}

void samplingTimeFunction(int count, int replica, double ****samplingTime, int *m)
{
		for(int center=0;center<CENTERS;center++)
	{
		if((t->current * m[center])==0)
			samplingTime[replica][center][0][count]=0;
		else
			samplingTime[replica][center][0][count]=aSampling[center].service/(t->current * m[center]);
		
		if(t->current==0)
			samplingTime[replica][center][1][count]=0;
		else
			samplingTime[replica][center][1][count]=aSampling[center].queue/t->current;
		
		if(t->current==0)
			samplingTime[replica][center][2][count]=0;
		else	
			samplingTime[replica][center][2][count]=aSampling[center].node/t->current;
		
		if((al[center].compl_a + al[center].compl_f)==0)
			samplingTime[replica][center][3][count]=0;
		else
			samplingTime[replica][center][3][count]=aSampling[center].service/(al[center].compl_a + al[center].compl_f);
		
		if((al[center].compl_a + al[center].compl_f)==0)
			samplingTime[replica][center][4][count]=0;
		else
			samplingTime[replica][center][4][count]=aSampling[center].queue/(al[center].compl_a + al[center].compl_f);
		
		if((al[center].compl_a + al[center].compl_f)==0)
			samplingTime[replica][center][5][count]=0;
		else
			samplingTime[replica][center][5][count]=aSampling[center].node/(al[center].compl_a + al[center].compl_f);
		
		if((al[center].compl_a + al[center].compl_f)==0)
			samplingTime[replica][center][6][count]=0;
		else
			samplingTime[replica][center][6][count]=t->last[center]/(al[center].compl_a + al[center].compl_f);
		
		samplingTime[replica][center][7][count]=(double)(al[center].index_f);
				
		samplingTime[replica][center][8][count]=(double)(al[center].index_a);
		
		samplingTime[replica][center][9][count]=(double)(al[center].numLoss_f);
		
		if((al[center].index_f)==0)
			samplingTime[replica][center][10][count]=0;
		else
			samplingTime[replica][center][10][count]=(double)(al[center].numLoss_f)/(double)(al[center].index_f);		
		
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
			sv1[0].x[i] = -1;
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
			sv2[0].x[i] = -1;
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
			sv1[1].x[i] = -1;
		}
	}	


}

void simulation(int **array_m, int replica, double**** nsim, double ****samplingTime)

{

	int rate;
	int interval = 0;
	int count=0;
	int *m = array_m[0];

	while(events->carArr1.isCarArrivalActive || events->familyArr1.isFamilyArrivalActive || events->familyArr2.isFamilyArrivalActive || !isSystemEmpty(m)) {

		t->next = getMinimumTime(m);		//Next event time

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

		t->current = t->next;		//Clock update
		printf("Debugging: %f\n", t->current);fflush(stdout);
		printf("Debugging: %f\n", al[0].index_a);fflush(stdout);
		
		struct next_abandon *nextAb1 = getMinAbandon(events->head1);
		struct next_abandon *nextAb2 = getMinAbandon(events->head2);
		struct next_completion *nextCom1 = getMinCompletion(m[0], sv1[0].x, 1);
		struct next_completion *nextCom2 = getMinCompletion(m[1], sv2[0].x, 2);
		struct next_completion *nextCom3 = getMinCompletion(m[2], sv1[1].x, 3);
		struct next_completion *nextCom4 = getMinCompletion(m[3], sv2[1].x, 4);
		struct next_completion *nextCom5 = getMinCompletion(m[4], sv2[2].x, 5);

		if(t->current == events-> changeInterval){
			printf("EVENTO: cambio intervallo.\n");
			fflush(stdout);
			if(interval!=0)
			{
				printf("EVENTO: cambio intervallo sampling.\n");
				fflush(stdout);
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
				rate = 180.0;
				events->changeInterval = 7200.0;
			}
			else if(events->changeInterval==7200.0)	{			
				rate = 480.0;
				events->changeInterval = 10800.0;
				m = array_m[1];
				applyServersVariation(array_m[0], m, events, t, sv1, sv2);
			}
			else if(events->changeInterval==10800.0)	{
				rate = 60.0;
				events->changeInterval = 21600.0;
				m = array_m[2];
				applyServersVariation(array_m[1], m, events, t, sv1, sv2);
			}
			else if(events->changeInterval==21600.0)	{
				rate = 300.0;
				events->changeInterval = 32400.0;
				m = array_m[3];
				applyServersVariation(array_m[2], m, events, t, sv1, sv2);
			}
			else if(events->changeInterval==32400.0)	{
				rate = 30.0;
				events->changeInterval = 46800.0;
				m = array_m[4];
				applyServersVariation(array_m[3], m, events, t, sv1, sv2);
			}
			else if(events->changeInterval==46800.0)	{
				rate = 180.0;
				events->changeInterval = (double) INFINITY;
				m = array_m[5];
				applyServersVariation(array_m[4], m, events, t, sv1, sv2);
			}
		}
		else if(t->current == events-> sampling){
			printf("EVENTO: sampling temporale.\n");
			fflush(stdout);
			samplingTimeFunction(count, replica, samplingTime, m);
			if(t->current + SAMPLINGINTERVAL <= STOP){
				events->sampling += SAMPLINGINTERVAL;
				count++;
				printf("%d\n", count);
				fflush(stdout);}
			else
				events->sampling = (double) INFINITY;
		}
		else if(t->current == events->carArr1.carArrivalTime) {
			printf("EVENTO: arrivo di un'automobile nel centro 1.\n");
			carArrival1(events, t, &sv1[0], &al[0], m[0]);
		}
		else if(t->current == events->familyArr1.familyArrivalTime) {
			printf("EVENTO: arrivo di una famiglia nel centro 1.\n");
			familyArrival1(events, t, &sv1[0], &al[0], m[0]);
		}
		else if(t->current == nextCom1->completionTime && !nextCom1->isFamily) {
			printf("EVENTO: partenza di un'automobile dal centro 1.\n");
			carDeparture1(events, t, &sv1[0], arr, nextCom1->serverOffset, &al[0]);
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
			arrival2(events, t, &sv2[0], &al[1], m[1]);
		}
		else if(t->current == nextCom2->completionTime) {
			printf("EVENTO: partenza di una famiglia dal centro 2.\n");
			departure2(events, t, &sv2[0], arr, nextCom2->serverOffset, m[1], &al[1]);
		}
		else if(t->current == nextAb2->abandonTime) {
			printf("EVENTO: abbandono di una famiglia dal centro 2.\n");
			abandon2(events, &sv2[0], &al[1], nextAb2->jobId);
		}
		else if(t->current == events->carArr3.carArrivalTime) {
			printf("EVENTO: arrivo di un'automobile nel centro 3.\n");
			carArrival3(events, t, &sv1[1], &al[2], arr, m[2]);
		}
		else if(t->current == events->familyArr3.familyArrivalTime) {
			printf("EVENTO: arrivo di una famiglia nel centro 3.\n");
			familyArrival3(events, t, &sv1[1], &al[2], arr, m[2]);
		}
		else if(t->current == nextCom3->completionTime && !nextCom3->isFamily) {
			printf("EVENTO: partenza di un'automobile dal centro 3.\n");
			carDeparture3(events, t, &sv1[1], arr, nextCom3->serverOffset, &al[2]);
		}
		else if(t->current == nextCom3->completionTime && nextCom3->isFamily) {
			printf("EVENTO: partenza di una famiglia dal centro 3.\n");
			familyDeparture3(events, t, &sv1[1], arr, nextCom3->serverOffset, m[2], &al[2]);
		}
		else if(t->current == events->familyArr4.familyArrivalTime) {
			printf("EVENTO: arrivo di una famiglia nel centro 4.\n");
			arrival4(events, t, &sv2[1], &al[3], arr, m[3]);
		}
		else if(t->current == nextCom4->completionTime) {
			printf("EVENTO: partenza di una famiglia dal centro 4.\n");
			departure4(events, t, &sv2[1], arr, nextCom4->serverOffset, m[3], &al[3]);
		}
		else if(t->current == events->familyArr5.familyArrivalTime) {
			printf("EVENTO: arrivo di una famiglia nel centro 5.\n");
			arrival5(events, t, &sv2[2], &al[4], arr, m[4]);
		}
		else if(t->current == nextCom5->completionTime) {
			printf("EVENTO: partenza di una famiglia dal centro 5.\n");
			departure5(events, &sv2[2], nextCom5->serverOffset, &al[4]);
		}

		printf("qA centro 1: %d\n", sv1[0].qA);
		printf("qF centro 1: %d\n", sv1[0].qF);
		printf("Serventi centro 1: ");
		for(int k=0; k<m[0]; k++) {
			printf("%d, ", sv1[0].x[k]);
		}
		printf("\n");
		printf("l centro 2: %d\n", sv2[0].l);
		printf("Serventi centro 2: ");
		for(int k=0; k<m[1]; k++) {
			printf("%d, ", sv2[0].x[k]);
		}
		printf("\n");
		printf("qA centro 3: %d\n", sv1[1].qA);
		printf("qF centro 3: %d\n", sv1[1].qF);
		printf("Serventi centro 3: ");
		for(int k=0; k<m[2]; k++) {
			printf("%d, ", sv1[1].x[k]);
		}
		printf("\n");
		printf("l centro 4: %d\n", sv2[1].l);
		printf("Serventi centro 4: ");
		for(int k=0; k<m[3]; k++) {
			printf("%d, ", sv2[1].x[k]);
		}
		printf("\n");
		printf("Serventi centro 5: ");
		for(int k=0; k<m[4]; k++) {
			printf("%d, ", sv2[2].x[k]);
		}
		printf("\n");
		printf("Clock: %f\n\n\n", t->current);
		fflush(stdout);

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


double***** finite_sim(int **array_m)
{

	PlantSeeds(7000);
	
	
	double ****nsim = (double ****) malloc(sizeof(double***)*REPLICATIONS);
	double ****samplingTime = (double ****)malloc(sizeof(double ***)*REPLICATIONS);
	
	if(nsim==NULL || samplingTime==NULL)
		errorMalloc(-2000);
		
	
	for(int i=0; i<REPLICATIONS;i++)
	{
		nsim[i] = (double ***)malloc(sizeof(double **)*CENTERS);
		if(nsim[i]==NULL)
			errorMalloc(-2001);
		for(int j=0;j<CENTERS;j++)
		{
			nsim[i][j] = (double**)malloc(sizeof(double*)*INTERVALS);
			if(nsim[i][j]==NULL)
				errorMalloc(-2002);
			for(int k=0; k<INTERVALS;k++)
			{
				nsim[i][j][k] = (double*)malloc(sizeof(double)*STATISTICS);
				if(nsim[i][j][k]==NULL)
				errorMalloc(-2003);
			}
		}
	}	
	
	int samplingSize = STOP/SAMPLINGINTERVAL;
	for(int i=0; i<REPLICATIONS;i++)
	{
		samplingTime[i] = (double ***)malloc(sizeof(double **)*CENTERS);
		if(samplingTime[i]==NULL)
			errorMalloc(-2001);
		for(int j=0;j<CENTERS;j++)
		{
			samplingTime[i][j] = (double**)malloc(sizeof(double*)*STATISTICS);
			if(samplingTime[i][j]==NULL)
				errorMalloc(-2002);
			for(int k=0; k<STATISTICS;k++)
			{
				samplingTime[i][j][k] = (double*)malloc(sizeof(double)*samplingSize);
				if(samplingTime[i][j][k]==NULL)
				errorMalloc(-2003);
			}
		}
	}	
	
	
	//finite-horizon simulation
	for(int i=0; i<REPLICATIONS; i++)
	{
		initialize(array_m);
		simulation(array_m, i, nsim, samplingTime);
		//clear();
	}
	
	//computeInterval();
	//deallocateDataStructures();
	
	double *****ret = (double *****)malloc(sizeof(double *****)*2);
	ret[0] = nsim;
	ret[1] = samplingTime;
	
	
	return ret;
}


void infinite()
{

}

