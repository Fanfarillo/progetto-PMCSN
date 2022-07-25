#include "./headers/servicenode1.h"
#include "./headers/servicenode2.h"
#include "./headers/servicenode3.h"
#include "./headers/servicenode4.h"
#include "./headers/servicenode5.h"
#include "./headers/rngs.h"
#include "./dataStructure/eventList.h"
#include "./dataStructure/time.h"
#include "./dataStructure/area.h"
#include "./dataStructure/stateVariables.h"
#include "./dataStructure/numArrLoss.h"
#include "./dataStructure/arrivalTimes.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define START 0.0
#define STOP 1000000.0
#define QE 0.4
#define P 0.5

struct event_list events;
struct time t[5];
struct area a[5];
struct state_variables1 sv1[2];		//centers num 1, 3
struct state_variables2 sv2[3];		//centers num 2, 4, 5
struct arrival_loss al[5];
struct arrivals arr;

double rate;

double exponential(double mu)
{
  return (-mu * log(1.0 - Random()));
}

double getCarArrival(double arrival) {
	arrival += exponential(rate*(1-QE)*P);
	return (arrival);
}

double getFamilyArrival1(double arrival){
	arrival += exponential(rate*(1-QE)*(1-P));
  	return (arrival);
}

double getFamilyArrival2(double arrival) {
	arrival += exponential(rate*QE);
  	return (arrival);
}

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

	for(int i=0; i<5; i++) {
		t[i].current = 0.0;
		t[i].next = 0.0;
		t[i].last = 0.0;
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
		sv1[0].x[i] = 0;	//0=IDLE, 1=BUSY_A, 2=BUSY_F
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
		sv1[1].x[i] = 0;	//0=IDLE, 1=BUSY_A, 2=BUSY_F
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
			rate = 180.0;
			break;
		case 2:
			rate = 480.0;
			break;
		case 3:
			rate = 60.0;
			break;
		case 4:
			rate = 300.0;
			break;
		case 5:
			rate = 30.0;
			break;
		case 6:
			rate = 180.0;
			break;
		default:
			exit(-3);
	}

	initializeEventList(m);
	initializeTime();
	initializeArea();
	initializeStateVariables(m);
	initializeArrivalLoss();
	initializeArrivals();

	carArrival1();
	familyArrival1();
	carDeparture1();
	familyDeparture1();
	abandon1();
	arrival2();
	departure2();
	abandon2();
	carArrival3();
	familyArrival3();
	carDeparture3();
	familyDeparture3();
	arrival4();
	departure4();
	arrival5();
	departure5();
	return 0;
}
