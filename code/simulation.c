#include "./headers/servicenode1.h"
#include "./headers/servicenode2.h"
#include "./headers/servicenode3.h"
#include "./headers/servicenode4.h"
#include "./headers/servicenode5.h"
#include "./headers/rngs.h"
#include "./headers/randomGeneratorFunctions.h"/*
#include "./dataStructure/eventList.h"
#include "./dataStructure/time.h"
#include "./dataStructure/area.h"
#include "./dataStructure/stateVariables.h"
#include "./dataStructure/numArrLoss.h"
#include "./dataStructure/arrivalTimes.h"
#include "./dataStructure/utilStructs.h"*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define START 0.0
#define STOP 1000000.0

#define QE 0.4
#define P_CAR 0.5

#define SERVICE_TIME_1 90
#define SERVICE_TIME_2 180
#define SERVICE_TIME_3 240
#define SERVICE_TIME_4 1800
#define SERVICE_TIME_5 1800
#define ABANDON_TIME_1 360
#define ABANDON_TIME_2 360

struct event_list events;
struct time t;
struct area a[5];
struct state_variables1 sv1[2];		//centers num 1, 3
struct state_variables2 sv2[3];		//centers num 2, 4, 5
struct arrival_loss al[5];
struct arrivals arr;



void initializeEventList(int *m) {

	events.carArr1.carArrivalTime = getCarArrival(START);
	events.carArr1.isCarArrivalActive = true;
	events.familyArr1.familyArrivalTime = getFamilyArrival1(START);
	events.familyArr1.isFamilyArrivalActive = true;

	events.familyArr2.familyArrivalTime = getFamilyArrival2(START);
	events.familyArr2.isFamilyArrivalActive = true;

	events.carArr3.carArrivalTime = (double) INFINITY;
	events.carArr3.isCarArrivalActive = true;
	events.familyArr3.familyArrivalTime = (double) INFINITY;
	events.familyArr3.isFamilyArrivalActive = true;

	events.familyArr4.familyArrivalTime = (double) INFINITY;
	events.familyArr4.isFamilyArrivalActive = true;

	events.familyArr5.familyArrivalTime = (double) INFINITY;
	events.familyArr5.isFamilyArrivalActive = true;

	events.head1 = NULL;
	events.tail1 = NULL;
	events.head2 = NULL;
	events.tail2 = NULL;

	events.completionTimes1 = (double *) malloc(sizeof(double)*m[0]);
	events.completionTimes2 = (double *) malloc(sizeof(double)*m[1]);
	events.completionTimes3 = (double *) malloc(sizeof(double)*m[2]);
	events.completionTimes4 = (double *) malloc(sizeof(double)*m[3]);
	events.completionTimes5 = (double *) malloc(sizeof(double)*m[4]);

	for(int i=0; i<m[0]; i++) {
		events.completionTimes1[i] = (double) INFINITY;
	}
	for(int i=0; i<m[1]; i++) {
		events.completionTimes2[i] = (double) INFINITY;
	}
	for(int i=0; i<m[2]; i++) {
		events.completionTimes3[i] = (double) INFINITY;
	}
	for(int i=0; i<m[3]; i++) {
		events.completionTimes4[i] = (double) INFINITY;
	}
	for(int i=0; i<m[4]; i++) {
		events.completionTimes5[i] = (double) INFINITY;
	}


}

void initializeTime() {

	t.current = 0.0;
	t.next = 0.0;
	for(int i=0; i<5; i++) {
		t.last[i] = 0.0;
	}

}

void initializeArea() {

	for(int i=0; i<5; i++) {
		a[i].node = 0.0;
		a[i].queue = 0.0;
		a[i].service = 0.0;
	}

}

void initializeStateVariables(int *m) {

	sv1[0].qA = 0;
	sv1[0].qF = 0;
	sv1[0].x = (int *) malloc(sizeof(int)*m[0]);
	for(int i=0; i<m[0]; i++) {
		sv1[0].x[i] = 0;	//0=IDLE, 1=BUSY_F, 2=BUSY_A
	}

	sv2[0].l = 0;
	sv2[0].x = (int *) malloc(sizeof(int)*m[1]);
	for(int i=0; i<m[1]; i++) {
		sv2[0].x[i] = 0;	//0=IDLE, 1=BUSY
	}

	sv1[1].qA = 0;
	sv1[1].qF = 0;
	sv1[1].x = (int *) malloc(sizeof(int)*m[2]);
	for(int i=0; i<m[2]; i++) {
		sv1[1].x[i] = 0;	//0=IDLE, 1=BUSY_F, 2=BUSY_A
	}

	sv2[1].l = 0;
	sv2[1].x = (int *) malloc(sizeof(int)*m[3]);
	for(int i=0; i<m[3]; i++) {
		sv2[1].x[i] = 0;	//0=IDLE, 1=BUSY
	}

	sv2[2].l = 0;  			//unuseful
	sv2[2].x = (int *) malloc(sizeof(int)*m[4]);
	for(int i=0; i<m[4]; i++) {
		sv2[2].x[i] = 0;	//0=IDLE, 1=BUSY
	}

}

void initializeArrivalLoss() {

	for(int i=0; i<5; i++) {
		al[i].index_a = 0;
		al[i].index_f = 0;
		al[i].numLoss_f = 0;
	}

}

void initializeArrivals() {

	arr.head3 = NULL;
	arr.tail3 = NULL;
	
	arr.head4 = NULL;
	arr.tail4 = NULL;
	
	arr.head5 = NULL;
	arr.tail5 = NULL;

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

struct next_completion *getMinCompletion(int numServers, int *x) {

	struct next_completion *min = (struct next_completion *) malloc(sizeof(struct next_completion));
	min->serverOffset = 0;
	min->isFamily = true;
	min->completionTime = (double) INFINITY;

	for(int i=0; i<numServers; i++) {
		if(events.completionTimes1[i] < min->completionTime) {
			min->serverOffset = i;
			min->isFamily = (bool) (x[i]==1);
			min->completionTime = events.completionTimes1[i];
		}
	}

	return min;

}

double getSmallest(double *values) {

	double smallest = (double) INFINITY;

	for(int i=0; i<len(values); i++) {
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
	
	if(events.head1 != NULL) {
		//almeno un job che deve abbandonare
		struct next_abandon *nextAb1 = getMinAbandon(events.head1);
		minAbandon1 = nextAb1->abandonTime;
	}
	if(events.head2 != NULL) {
		struct next_abandon *nextAb2 = getMinAbandon(events.head2);
		minAbandon2 = nextAb2->abandonTime;
	}

	struct next_completion *nextCom1 = getMinCompletion(m[0], sv1[0].x);
	struct next_completion *nextCom2 = getMinCompletion(m[1], sv2[0].x);
	struct next_completion *nextCom3 = getMinCompletion(m[2], sv1[1].x);
	struct next_completion *nextCom4 = getMinCompletion(m[3], sv2[1].x);
	struct next_completion *nextCom5 = getMinCompletion(m[4], sv2[2].x);

	double minService1 = nextCom1->completionTime;
	double minService2 = nextCom2->completionTime;
	double minService3 = nextCom3->completionTime;
	double minService4 = nextCom4->completionTime;
	double minService5 = nextCom5->completionTime;

	double timesToCompare[14];
	timesToCompare[0] = minAbandon1;
	timesToCompare[1] = minAbandon2;
	timesToCompare[2] = minService1;
	timesToCompare[3] = minService2;
	timesToCompare[4] = minService3;
	timesToCompare[5] = minService4;
	timesToCompare[6] = minService5;
	timesToCompare[7] = events.carArr1.carArrivalTime;
	timesToCompare[8] = events.familyArr1.familyArrivalTime;
	timesToCompare[9] = events.familyArr2.familyArrivalTime;
	timesToCompare[10] = events.carArr3.carArrivalTime;
	timesToCompare[11] = events.familyArr3.familyArrivalTime;
	timesToCompare[12] = events.familyArr4.familyArrivalTime;
	timesToCompare[13] = events.familyArr5.familyArrivalTime;

	free(nextAb1);
	free(nextAb2);
	free(nextCom1);
	free(nextCom2);
	free(nextCom3);
	free(nextCom4);
	free(nextCom5);

	return getSmallest(timesToCompare);

}

int countBusyServers(int numServers, int *serverList) {

	int count = 0;
	for(int i=0; i<numServers; i++) {
		if(serverList[i] > 0)
			count++;
	}

	return count;

}

int main(int argc, char **argv){

	if(argc < 7)
		exit(-1);

	int* m = NULL;
	m = (int *)malloc(sizeof(int)*(5));

	if(m==NULL)
		exit(-2);

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
			exit(-3);
	}

	initializeTime();
	initializeArea();
	initializeStateVariables(m);
	initializeArrivalLoss();
	initializeArrivals();

	PlantSeeds(7000);
	initializeEventList(m);

	while(events.carArr1.isCarArrivalActive || events.familyArr1.isFamilyArrivalActive || events.familyArr2.isFamilyArrivalActive || !isSystemEmpty(m)) {

		t.next = getMinimumTime(m);		//Next event time
		
		int xBusy1 = countBusyServers(m[0], sv1[0].x);
		a[0].service += (t.next - t.current)*xBusy1;
		a[0].queue += (t.next - t.current)*(sv1[0].qA + sv1[0].qF);
		a[0].node += (t.next - t.current)*(sv1[0].qA + sv1[0].qF + xBusy1);

		int xBusy2 = countBusyServers(m[1], sv2[0].x);
		a[1].service += (t.next - t.current)*xBusy2;
		a[1].queue += (t.next - t.current)*(sv2[0].l - xBusy2);
		a[1].node += (t.next - t.current)*(sv2[0].l);

		int xBusy3 = countBusyServers(m[2], sv1[1].x);
		a[2].service += (t.next - t.current)*xBusy3;
		a[2].queue += (t.next - t.current)*(sv1[1].qA + sv1[1].qF);
		a[2].node += (t.next - t.current)*(sv1[1].qA + sv1[1].qF + xBusy3);

		int xBusy4 = countBusyServers(m[3], sv2[1].x);
		a[3].service += (t.next - t.current)*xBusy4;
		a[3].queue += (t.next - t.current)*(sv2[1].l - xBusy4);
		a[3].node += (t.next - t.current)*(sv2[1].l);

		int xBusy5 = countBusyServers(m[4], sv2[2].x);
		a[4].service += (t.next - t.current)*xBusy5;
		a[4].node += (t.next - t.current)*xBusy5;

		t.current = t.next;		//Clock update

		struct next_abandon *nextAb1 = getMinAbandon(events.head1);
		struct next_abandon *nextAb2 = getMinAbandon(events.head2);
		struct next_completion *nextCom1 = getMinCompletion(m[1], sv1[0].x);
		struct next_completion *nextCom2 = getMinCompletion(m[2], sv2[0].x);
		struct next_completion *nextCom3 = getMinCompletion(m[3], sv1[1].x);
		struct next_completion *nextCom4 = getMinCompletion(m[4], sv2[1].x);
		struct next_completion *nextCom5 = getMinCompletion(m[5], sv2[2].x);

		if(t.current == events.carArr1.carArrivalTime) {
			carArrival1(&events, &t, &sv1[0], &al[0]);
		}
		else if(t.current == events.familyArr1.familyArrivalTime) {
			familyArrival1(&events, &t, &sv1[0], &al[0]);
		}
		else if(t.current == nextCom1->completionTime && !nextCom1->isFamily) {
			carDeparture1(&events, &t, &sv1[0], &arr, nextCom1->serverOffset);
		}
		else if(t.current == nextCom1->completionTime && nextCom1->isFamily) {
			familyDeparture1(&events, &t, &sv1[0], &arr, nextCom1->serverOffset);
		}
		else if(t.current == nextAb1->abandonTime) {
			abandon1(&events, &sv1[0], &al[0], nextAb1->jobId);
		}
		else if(t.current == events.familyArr2.familyArrivalTime) {
			arrival2(&events, &t, &sv2[0], &al[1]);
		}
		else if(t.current == nextCom2->completionTime) {
			departure2(&events, &t, &sv2[0], &arr, nextCom2->serverOffset);
		}
		else if(t.current == nextAb2->abandonTime) {
			abandon2(&events, &sv2[0], &al[1], nextAb2->jobId);
		}
		else if(t.current == events.carArr3.carArrivalTime) {
			carArrival3(&events, &t, &sv1[1], &al[2], &arr);
		}
		else if(t.current == events.familyArr3.familyArrivalTime) {
			familyArrival3(&events, &t, &sv1[1], &al[2], &arr);
		}
		else if(t.current == nextCom3->completionTime && !nextCom3->isFamily) {
			carDeparture3(&events, &t, &sv1[1], &arr, nextCom3->serverOffset);
		}
		else if(t.current == nextCom3->completionTime && nextCom3->isFamily) {
			familyDeparture3(&events, &t, &sv1[1], &arr, nextCom3->serverOffset);
		}
		else if(t.current == events.familyArr4.familyArrivalTime) {
			arrival4(&events, &t, &sv2[1], &al[3], &arr);
		}
		else if(t.current == nextCom4->completionTime) {
			departure4(&events, &t, &sv2[1], &arr, nextCom4->serverOffset);
		}
		else if(t.current == events.familyArr5.familyArrivalTime) {
			arrival5(&events, &t, &sv2[2], &al[4], &arr);
		}
		else if(t.current == nextCom5->completionTime) {
			departure5(&events, &sv2[2], nextCom5->serverOffset);
		}

		free(nextAb1);
		free(nextAb2);
		free(nextCom1);
		free(nextCom2);
		free(nextCom3);
		free(nextCom4);
		free(nextCom5);			

	}

	//TODO: recuperare le statistiche di output e invocare la free() per tutte le aree di memoria allocate dinamicamente

	return 0;

}
