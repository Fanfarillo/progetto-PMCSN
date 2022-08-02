#include "./headers/servicenode1.h"
#include "./headers/servicenode2.h"
#include "./headers/servicenode3.h"
#include "./headers/servicenode4.h"
#include "./headers/servicenode5.h"
#include "./headers/rngs.h"
#include "./headers/randomGeneratorFunctions.h"
#include <stdio.h>
#include<string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define START 0.0
#define STOP 1000000.0

struct event_list *events;
struct time *t;
struct area *a;
struct state_variables1 *sv1;		//centers num 1, 3
struct state_variables2 *sv2;		//centers num 2, 4, 5
struct arrival_loss *al;
struct arrivals *arr;

void errorMalloc(int code){
	printf("Errore malloc...\n");
	exit(code);
}

void allocateDataStructure(){
	events = (struct event_list *) malloc(sizeof(struct event_list));
	if(events == NULL)
		errorMalloc(-9);
		
	t = (struct time *) malloc(sizeof(struct time));
	if(t==NULL)
		errorMalloc(-10);
		
	a = (struct area *) malloc(sizeof(struct area)*5);
	if(a==NULL)
		errorMalloc(-11);
		
	sv1 = (struct state_variables1 *)malloc(sizeof(struct state_variables1) * 2);
	if(sv1==NULL)
		errorMalloc(-12);
		
	sv2 = (struct state_variables2 *)malloc(sizeof(struct state_variables2)*3);
	if(sv2==NULL)
		errorMalloc(-13);
	
	
	al = (struct arrival_loss *) malloc(sizeof(struct arrival_loss)*5);
	if(al ==NULL)
		errorMalloc(-14);
		
	arr = (struct arrivals *) malloc(sizeof(struct arrivals));
	if(arr == NULL)
		errorMalloc(-15);
		
	printf("Allocate data structures...\n");
	fflush(stdout);
	
}

void initializeEventList(int *m) {

	events->carArr1.carArrivalTime = getCarArrival(START);
	printf("carArrival: %f\n", events->carArr1.carArrivalTime);
	fflush(stdout);
	events->carArr1.isCarArrivalActive = true;
	events->familyArr1.familyArrivalTime = getFamilyArrival1(START);
	printf("familyArrival: %f\n", events->familyArr1.familyArrivalTime);
	fflush(stdout);
	events->familyArr1.isFamilyArrivalActive = true;

	events->familyArr2.familyArrivalTime = getFamilyArrival2(START);
	printf("familyArrival2 (electr): %f\n", events->familyArr2.familyArrivalTime);
	fflush(stdout);
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
		errorMalloc(-16);
	events->completionTimes2 = (double *) malloc(sizeof(double)*m[1]);
	if(events->completionTimes2==NULL)
		errorMalloc(-17);
	events->completionTimes3 = (double *) malloc(sizeof(double)*m[2]);
	if(events->completionTimes3==NULL)
		errorMalloc(-18);
	events->completionTimes4 = (double *) malloc(sizeof(double)*m[3]);
	if(events->completionTimes4==NULL)
		errorMalloc(-19);
	events->completionTimes5 = (double *) malloc(sizeof(double)*m[4]);
	if(events->completionTimes5==NULL)
		errorMalloc(-20);

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
	
	printf("Event list...\n");
	fflush(stdout);


}

void initializeTime() {

	t->current = 0.0;
	t->next = 0.0;
	for(int i=0; i<5; i++) {
		t->last[i] = 0.0;
	}
	
	printf("Time...\n");
	fflush(stdout);

}

void initializeArea() {

	for(int i=0; i<5; i++) {
		a[i].node = 0.0;
		a[i].queue = 0.0;
		a[i].service = 0.0;
	}
	printf("Area...\n");
	fflush(stdout);

}

void initializeStateVariables(int *m) {

	sv1[0].qA = 0;
	sv1[0].qF = 0;
	sv1[0].x = (int *) malloc(sizeof(int)*m[0]);
	if(sv1[0].x==NULL){
		printf("Errore malloc...\n");
		fflush(stdout);
		exit(-4);
	}
	for(int i=0; i<m[0]; i++) {
		sv1[0].x[i] = 0;	//0=IDLE, 1=BUSY_F, 2=BUSY_A
	}

	sv2[0].l = 0;
	sv2[0].x = (int *) malloc(sizeof(int)*m[1]);
	if(sv2[0].x==NULL){
		printf("Errore malloc...\n");
		fflush(stdout);
		exit(-5);
	}
	for(int i=0; i<m[1]; i++) {
		sv2[0].x[i] = 0;	//0=IDLE, 1=BUSY
	}

	sv1[1].qA = 0;
	sv1[1].qF = 0;
	sv1[1].x = (int *) malloc(sizeof(int)*m[2]);
	if(sv1[1].x==NULL){
		printf("Errore malloc...\n");
		fflush(stdout);
		exit(-6);
	}
	for(int i=0; i<m[2]; i++) {
		sv1[1].x[i] = 0;	//0=IDLE, 1=BUSY_F, 2=BUSY_A
	}

	sv2[1].l = 0;
	sv2[1].x = (int *) malloc(sizeof(int)*m[3]);
	if(sv2[1].x==NULL){
		printf("Errore malloc...\n");
		fflush(stdout);
		exit(-7);
	}
	for(int i=0; i<m[3]; i++) {
		sv2[1].x[i] = 0;	//0=IDLE, 1=BUSY
	}

	sv2[2].l = 0;  			//unuseful
	sv2[2].x = (int *) malloc(sizeof(int)*m[4]);
	if(sv2[2].x==NULL){
		printf("Errore malloc...\n");
		fflush(stdout);
		exit(-8);
	}
	for(int i=0; i<m[4]; i++) {
		sv2[2].x[i] = 0;	//0=IDLE, 1=BUSY
	}
	printf("State Variables...\n");
	fflush(stdout);

}

void initializeArrivalLoss() {

	for(int i=0; i<5; i++) {
		al[i].index_a = 0;
		al[i].index_f = 0;
		al[i].numLoss_f = 0;
	}
	printf("Arrival loss...\n");
	fflush(stdout);

}

void initializeArrivals() {

	arr->head3 = NULL;
	arr->tail3 = NULL;
	
	arr->head4 = NULL;
	arr->tail4 = NULL;
	
	arr->head5 = NULL;
	arr->tail5 = NULL;
	
	printf("Arrivals...\n");
	fflush(stdout);

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
				printf("Errore indice...\n");
				exit(-21);
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
	int len = 14;
	
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

	//double timesToCompare[14];
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
	//printf("%f\n", timesToCompare[13]);

	free(nextAb1);
	free(nextAb2);
	free(nextCom1);
	free(nextCom2);
	free(nextCom3);
	free(nextCom4);
	free(nextCom5);

	return getSmallest(timesToCompare, len);

}

void verify(){
	double rho_0 = a[0].service/t->current;
	double q_0 = a[0].queue/t->current;
	double n_0 = a[0].node/t->current;
	if(n_0 - (q_0 + rho_0) > 0.00001)
		exit(-100);
		
	double rho_1 = a[1].service/t->current;
	double q_1 = a[1].queue/t->current;
	double n_1 = a[1].node/t->current;
	if(n_1 - (q_1 + rho_1) > 0.00001)
		exit(-101);
		
	double rho_2 = a[2].service/t->current;
	double q_2 = a[2].queue/t->current;
	double n_2 = a[2].node/t->current;
	if(n_2 - (q_2 + rho_2) > 0.00001)
		exit(-102);
		
	double rho_3 = a[3].service/t->current;
	double q_3 = a[3].queue/t->current;
	double n_3 = a[3].node/t->current;
	if(n_3 - (q_3 + rho_3) > 0.00001)
		exit(-103);
		
		
	double rho_4 = a[4].service/t->current;
	double q_4 = a[4].queue/t->current;
	double n_4 = a[4].node/t->current;
	if(n_4 - (q_4 + rho_4) > 0.00001)
		exit(-104);
		
	
	
	double rho_00 = a[0].service/(al[0].index_a + al[0].index_f);
	double q_00 = a[0].queue/(al[0].index_a + al[0].index_f);
	double n_00 = a[0].node/(al[0].index_a + al[0].index_f);
	if(n_00 - (q_00 + rho_00) > 0.00001)
		exit(-110);
		
	double rho_01 = a[1].service/(al[1].index_a + al[1].index_f);
	double q_01 = a[1].queue/(al[1].index_a + al[1].index_f);
	double n_01 = a[1].node/(al[1].index_a + al[1].index_f);
	if(n_01 - (q_01 + rho_01) > 0.00001)
		exit(-111);
		
	double rho_02 = a[2].service/(al[2].index_a + al[2].index_f);
	double q_02 = a[2].queue/(al[2].index_a + al[2].index_f);
	double n_02 = a[2].node/(al[2].index_a + al[2].index_f);
	if(n_02 - (q_02 + rho_02) > 0.00001)
		exit(-112);
		
	double rho_03 = a[3].service/(al[3].index_a + al[3].index_f);
	double q_03 = a[3].queue/(al[3].index_a + al[3].index_f);
	double n_03 = a[3].node/(al[3].index_a + al[3].index_f);
	if(n_03 - (q_03 + rho_03) > 0.00001)
		exit(-113);
		
		
	double rho_04 = a[4].service/(al[4].index_a + al[4].index_f);
	double q_04 = a[4].queue/(al[4].index_a + al[4].index_f);
	double n_04 = a[4].node/(al[4].index_a + al[4].index_f);
	if(n_04 - (q_04 + rho_04) > 0.00000000000000001)
		exit(-114);
}

int countBusyServers(int numServers, int *serverList) {

	int count = 0;
	for(int i=0; i<numServers; i++) {
		if(serverList[i] > 0)
			count++;
	}

	return count;

}

FILE** createStatisticFiles(){
	int length = strlen("/home/ubuntu/progettoPMSN/progetto-PMCSN/code/servicenode.dat")+10;
	char *filename;
	FILE **fps = (FILE**) malloc(sizeof(FILE *)*5);
	if(fps==NULL)
		errorMalloc(-200);
	
	for(int i = 0; i < 5; i++){
		filename = (char *) malloc(length);
		//printf("%d\n", i);
		FILE *fp;
   		if(sprintf(filename,"/home/ubuntu/progettoPMSN/progetto-PMCSN/code/servicenode%d.dat", i+1)<0)
   			exit(-800);
   		//printf("%s", filename);
   		fp = fopen(filename, "w");
   		//printf("%s", filename);
   		if(fp==NULL){
   			printf("errore\n");
   			exit(-89);
   		}
   		free(filename);
   		fps[i] = fp;   		
	}

	
	return fps;
}

int main(int argc, char **argv){

	if(argc < 7){
		printf("errore nei parametri...\n");
		fflush(stdout);
		exit(-1);
	}

	int* m = NULL;
	m = (int *)malloc(sizeof(int)*(5));

	if(m==NULL){
		printf("Errore malloc...\n");
		fflush(stdout);
		exit(-2);
	}

	m[0] = atoi(argv[1]);
	m[1] = atoi(argv[2]);
	m[2] = atoi(argv[3]);
	m[3] = atoi(argv[4]);
	m[4] = atoi(argv[5]);

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
			printf("Errore nello switch...\n");
			fflush(stdout);
			exit(-3);
	}
	
	//printf("%f\n", interTime);
	//printf("%d %d %d %d %d\n", m[0], m[1], m[2], m[3], m[4]);
	//fflush(stdout);
	

	allocateDataStructure();
	initializeTime();	
	initializeArea();
	initializeStateVariables(m);
	initializeArrivalLoss();
	initializeArrivals();
	

	PlantSeeds(7000);
	
	//printf("PlanSeeds...\n");
	//fflush(stdout);
	
	
	initializeEventList(m);

	while(events->carArr1.isCarArrivalActive || events->familyArr1.isFamilyArrivalActive || events->familyArr2.isFamilyArrivalActive || !isSystemEmpty(m)) {

		

		t->next = getMinimumTime(m);		//Next event time
		
		//printf("next occurence: %f\n", t->next);
		//fflush(stdout);
		
		
		int xBusy1 = countBusyServers(m[0], sv1[0].x);
		a[0].service += (t->next - t->current)*xBusy1;
		//printf("service: %f\n", a[0].service);
		a[0].queue += (t->next - t->current)*(sv1[0].qA + sv1[0].qF);
		//printf("queue: %f\n", a[0].queue);
		a[0].node += (t->next - t->current)*(sv1[0].qA + sv1[0].qF + xBusy1);
		//printf("node: %f\n", a[0].node);

		int xBusy2 = countBusyServers(m[1], sv2[0].x);
		a[1].service += (t->next - t->current)*xBusy2;
		//printf("service: %f\n", a[1].service);
		a[1].queue += (t->next - t->current)*(sv2[0].l - xBusy2);
		//printf("queue: %f\n", a[1].queue);
		a[1].node += (t->next - t->current)*(sv2[0].l);
		//printf("node: %f\n", a[1].node);

		int xBusy3 = countBusyServers(m[2], sv1[1].x);
		a[2].service += (t->next - t->current)*xBusy3;
		//printf("service: %f\n", a[2].service);
		a[2].queue += (t->next - t->current)*(sv1[1].qA + sv1[1].qF);
		//printf("queue: %f\n", a[2].queue);
		a[2].node += (t->next - t->current)*(sv1[1].qA + sv1[1].qF + xBusy3);
		//printf("node: %f\n", a[2].node);

		int xBusy4 = countBusyServers(m[3], sv2[1].x);
		a[3].service += (t->next - t->current)*xBusy4;
		//printf("service: %f\n", a[3].service);
		a[3].queue += (t->next - t->current)*(sv2[1].l - xBusy4);
		//printf("queue: %f\n", a[3].queue);
		a[3].node += (t->next - t->current)*(sv2[1].l);
		//printf("node: %f\n", a[3].node);

		int xBusy5 = countBusyServers(m[4], sv2[2].x);
		a[4].service += (t->next - t->current)*xBusy5;
		//printf("service: %f\n", a[4].service);
		a[4].node += (t->next - t->current)*xBusy5;
		//printf("node: %f\n", a[4].node);

		t->current = t->next;		//Clock update
		
		//printf("current: %f\n", t->current);

		struct next_abandon *nextAb1 = getMinAbandon(events->head1);
		struct next_abandon *nextAb2 = getMinAbandon(events->head2);
		struct next_completion *nextCom1 = getMinCompletion(m[0], sv1[0].x, 1);
		struct next_completion *nextCom2 = getMinCompletion(m[1], sv2[0].x, 2);
		struct next_completion *nextCom3 = getMinCompletion(m[2], sv1[1].x, 3);
		struct next_completion *nextCom4 = getMinCompletion(m[3], sv2[1].x, 4);
		struct next_completion *nextCom5 = getMinCompletion(m[4], sv2[2].x, 5);
		
		//printf("ok...\n");
		//fflush(stdout);
		

		if(t->current == events->carArr1.carArrivalTime) {
			carArrival1(events, t, &sv1[0], &al[0], m[0]);
		}
		else if(t->current == events->familyArr1.familyArrivalTime) {
			familyArrival1(events, t, &sv1[0], &al[0], m[0]);
		}
		else if(t->current == nextCom1->completionTime && !nextCom1->isFamily) {
			carDeparture1(events, t, &sv1[0], arr, nextCom1->serverOffset);
		}
		else if(t->current == nextCom1->completionTime && nextCom1->isFamily) {
			familyDeparture1(events, t, &sv1[0], arr, nextCom1->serverOffset, m[0]);
		}
		else if(t->current == nextAb1->abandonTime) {
			abandon1(events, &sv1[0], &al[0], nextAb1->jobId);
		}
		else if(t->current == events->familyArr2.familyArrivalTime) {
			arrival2(events, t, &sv2[0], &al[1], m[1]);
		}
		else if(t->current == nextCom2->completionTime) {
			departure2(events, t, &sv2[0], arr, nextCom2->serverOffset, m[1]);
		}
		else if(t->current == nextAb2->abandonTime) {
			abandon2(events, &sv2[0], &al[1], nextAb2->jobId);
		}
		else if(t->current == events->carArr3.carArrivalTime) {
			carArrival3(events, t, &sv1[1], &al[2], arr, m[2]);
		}
		else if(t->current == events->familyArr3.familyArrivalTime) {
			familyArrival3(events, t, &sv1[1], &al[2], arr, m[2]);
		}
		else if(t->current == nextCom3->completionTime && !nextCom3->isFamily) {
			carDeparture3(events, t, &sv1[1], arr, nextCom3->serverOffset);
		}
		else if(t->current == nextCom3->completionTime && nextCom3->isFamily) {
			familyDeparture3(events, t, &sv1[1], arr, nextCom3->serverOffset, m[2]);
		}
		else if(t->current == events->familyArr4.familyArrivalTime) {
			arrival4(events, t, &sv2[1], &al[3], arr, m[3]);
		}
		else if(t->current == nextCom4->completionTime) {
			departure4(events, t, &sv2[1], arr, nextCom4->serverOffset, m[3]);
		}
		else if(t->current == events->familyArr5.familyArrivalTime) {
			arrival5(events, t, &sv2[2], &al[4], arr, m[4]);
		}
		else if(t->current == nextCom5->completionTime) {
			departure5(events, &sv2[2], nextCom5->serverOffset);
		}

		free(nextAb1);
		free(nextAb2);
		free(nextCom1);
		free(nextCom2);
		free(nextCom3);
		free(nextCom4);
		free(nextCom5);			

	}
	
	
	verify();

	//TODO: invocare la free() per tutte le aree di memoria allocate dinamicamente
	
	FILE ** fps = createStatisticFiles();
	
	char *rho, *q, *serv, *n, *delay, *wait, *interArr, *fam;
	
	
	for(int i = 0; i < 5; i++){
		rho = (char *)malloc(30);
		q = (char *)malloc(30);
		serv = (char *)malloc(30);
		n = (char *)malloc(30);
		delay = (char *)malloc(30);
		wait = (char *)malloc(30);
		interArr = (char *)malloc(30);
		fam = (char *)malloc(30);
		
		
		sprintf(rho, "%f\n", a[i].service/(t->current * m[i]));
		fputs(rho, fps[i]);
		
		
		sprintf(q, "%f\n", a[i].queue/t->current);
		printf("a[%d].queue = %f", i, a[i].queue);
		fputs(q, fps[i]);
		
		sprintf(n, "%f\n", a[i].node/t->current);
		printf("a[%d].node = %f", i, a[i].node);
		fputs(n, fps[i]);
		
		sprintf(serv, "%f\n", a[i].service/((al[i].index_a + al[i].index_f))*m[i]);
		fputs(serv, fps[i]);	
		
		sprintf(delay, "%f\n", a[i].queue/(al[i].index_a + al[i].index_f));
		fputs(delay, fps[i]);
		
		sprintf(wait, "%f\n", a[i].node/(al[i].index_a + al[i].index_f));
		fputs(wait, fps[i]);
		
		sprintf(interArr, "%f\n", t->last[i]/(al[i].index_a + al[i].index_f));
		fputs(interArr, fps[i]);
		
		sprintf(fam, "%f\n", (double)al[i].index_f);
		fputs(fam, fps[i]);
		
		free(rho);
		free(q);
		free(n);
		free(serv);
		free(delay);
		free(wait);
		free(interArr);
		free(fam);
	}

	
	//utilizzazione
	/*
	double rho_1 = a[0].service/t->current;
	double rho_2 = a[1].service/t->current;
	double rho_3 = a[2].service/t->current;
	double rho_4 = a[3].service/t->current;
	double rho_5 = a[4].service/t->current;
	
	//popolazione media nelle code
	double q_1 = a[0].queue/t->current;
	double q_2 = a[1].queue/t->current;
	double q_3 = a[2].queue/t->current;
	double q_4 = a[3].queue/t->current;
	double q_5 = a[4].queue/t->current;
	
	//popolazione media nel centro
	double n_1 = a[0].node/t->current;
	double n_2 = a[1].node/t->current;
	double n_3 = a[2].node/t->current;
	double n_4 = a[3].node/t->current;
	double n_5 = a[4].node/t->current;
	
	//tempo di servizio medio
	double serv0 = a[0].service/(al[0].index_a + al[0].index_f);
	double serv1 = a[1].service/(al[1].index_a + al[1].index_f);
	double serv2 = a[2].service/(al[2].index_a + al[2].index_f);
	double serv3 = a[3].service/(al[3].index_a + al[3].index_f);
	double serv4 = a[4].service/(al[4].index_a + al[4].index_f);
	
	//tempo di attesa medio nella coda
	double delay1 = a[0].queue/(al[0].index_a + al[0].index_f);
	double delay2 = a[1].queue/(al[1].index_a + al[1].index_f);
	double delay3 = a[2].queue/(al[2].index_a + al[2].index_f);
	double delay4 = a[3].queue/(al[3].index_a + al[3].index_f);
	double delay5 = a[4].queue/(al[4].index_a + al[4].index_f);
	
	//tempo di risposta medio
	double wait1 = a[0].node/(al[0].index_a + al[0].index_f);
	double wait2 = a[1].node/(al[1].index_a + al[1].index_f);
	double wait3 = a[2].node/(al[2].index_a + al[2].index_f);
	double wait4 = a[3].node/(al[3].index_a + al[3].index_f);
	double wait5 = a[4].node/(al[4].index_a + al[4].index_f);
	
	//tempo di interarrivo
	double interArr1 = t->last[0]/(al[0].index_a + al[0].index_f);
	double interArr2 = t->last[1]/(al[1].index_a + al[1].index_f);
	double interArr3 = t->last[2]/(al[2].index_a + al[2].index_f);
	double interArr4 = t->last[3]/(al[3].index_a + al[3].index_f);
	double interArr5 = t->last[4]/(al[4].index_a + al[4].index_f);
	
	
	//numero arrivi famiglie
	double fam1 = al[0].index_f;
	double fam2 = al[1].index_f;
	double fam3 = al[2].index_f;
	double fam4 = al[3].index_f;
	double fam5 = al[4].index_f;*/

	
		//percentuale di perdita per i centri 1, 2 e 5
	
	double lossProb1 = al[0].numLoss_f/al[0].index_f;
	double lossProb2 = al[1].numLoss_f/al[1].index_f;
	double lossProb3 = al[4].numLoss_f/al[4].index_f;
	
	//numero perdite
	
	double loss1 = al[0].numLoss_f;
	double loss2 = al[1].numLoss_f;
	double loss5 = al[4].numLoss_f;
	printf("loss1: %f\n", loss1);
	printf("loss2: %f\n", loss2);
	printf("loss5: %f\n", loss5);
	

	char *lossProbStr;
	char *lossStr;
	
	for(int i = 0; i<3; i++){
		lossProbStr = (char *)malloc(30);
		lossStr = (char *)malloc(30);
		printf("b\n");
		if(i==2)
		{
			sprintf(lossProbStr, "%f\n", (double)al[i+2].numLoss_f/(double)al[i+2].index_f);
			fputs(lossProbStr, fps[i+2]);
			sprintf(lossStr, "%f\n", (double)al[i+2].numLoss_f);
			fputs(lossStr, fps[i+2]);			
		}	
		sprintf(lossProbStr, "%f\n", (double)al[i].numLoss_f/(double)al[i].index_f);
		fputs(lossProbStr, fps[i]);
		sprintf(lossStr, "%f\n", (double)al[i].numLoss_f);
		fputs(lossStr, fps[i]);	
	}
	
	//numero arrivi macchine
	/*
	double car1 = al[0].index_a;
	double car2 = al[2].index_a;*/
	char *carStr;
	for(int i = 0; i<3;i = i + 2){
		carStr = (char *)malloc(30);
		sprintf(carStr, "%f\n", (double)al[i].index_a);
		fputs(carStr, fps[i]);
		//printf("c\n");
	}
	
	for(int i = 0; i<5;i++){
		//printf("d\n");
		fclose(fps[i]);
	}	
	
	
	
	
	printf("fine...\n");
	fflush(stdout);

	return 0;

}
