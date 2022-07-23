#include "./headers/servicenode1.h"
#include "./headers/servicenode2.h"
#include "./headers/servicenode3.h"
#include "./headers/servicenode4.h"
#include "./headers/servicenode5.h"
#include "./dataStructure/eventList.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define START 0.0
#define STOP 10000.0

struct event_list events;

long getCarArrival() {
	return 1;
}

long getFamilyArrival1() {
	return 1;
}

long getFamilyArrival2() {
	return 1;
}

void initializeEventList(long *n, int numCenters) {
	events.carArr1.carArrivalTime = getCarArrival();
	events.carArr1.isCarArrivalActive = true;
	events.familyArr1.familyArrivalTime = getFamilyArrival1();
	events.familyArr1.isFamilyArrivalActive = true;

	events.familyArr2.familyArrivalTime = getFamilyArrival2();
	events.familyArr2.isFamilyArrivalActive = true;

	events.carArr3.carArrivalTime = (long) INFINITY;
	events.carArr3.isCarArrivalActive = true;
	events.familyArr3.familyArrivalTime = (long) INFINITY;
	events.familyArr3.isFamilyArrivalActive = true;

	events.familyArr4.familyArrivalTime = (long) INFINITY;
	events.familyArr4.isFamilyArrivalActive = true;

	events.familyArr5.familyArrivalTime = (long) INFINITY;
	events.familyArr5.isFamilyArrivalActive = true;

	events.head1 = NULL;
	events.tail1 = NULL;
	events.head2 = NULL;
	events.tail2 = NULL;

	events.completionTimes1 = (long *) malloc(sizeof(long)*n[0]);
	events.completionTimes2 = (long *) malloc(sizeof(long)*n[1]);
	events.completionTimes3 = (long *) malloc(sizeof(long)*n[2]);
	events.completionTimes4 = (long *) malloc(sizeof(long)*n[3]);
	events.completionTimes5 = (long *) malloc(sizeof(long)*n[4]);

	for(int i=0; i<numCenters; i++) {
		printf("%ld\n", n[i]);
	}
}

int main(int argc, char **argv){
	//TODO: statistiche di output
	
	if(argc < 6)
		exit(-1);
	
	long* n = NULL;
	n = (long *)malloc(sizeof(long)*(argc - 1));
	if(n==NULL)
		exit(-2);

	n[0] = atoi(argv[1]);
	n[1] = atoi(argv[2]);
	n[2] = atoi(argv[3]);
	n[3] = atoi(argv[4]);
	n[4] = atoi(argv[5]);

	initializeEventList(n, argc-1);

	carArrivals1();
	familyArrivals1();
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
