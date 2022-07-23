#include "./headers/servicenode1.h"
#include "./headers/servicenode2.h"
#include "./headers/servicenode3.h"
#include "./headers/servicenode4.h"
#include "./headers/servicenode5.h"
#include "./headers/rngs.h"
#include "./dataStructure/eventList.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define START 0.0
#define STOP 10000.0
#define QE 0.4
#define P 0.567

struct event_list events;
double rate;

double Exponential(double m)
{
  return (-m * log(1.0 - Random()));
}

double getCarArrival(double arrival) {
	arrival += Exponential(rate*(1-QE)*P);
	return (arrival);
}

double getFamilyArrival1(double arrival){
	arrival += Exponential(rate*(1-QE)*(1-P));
  	return (arrival);
}

double getFamilyArrival2(double arrival) {
	arrival += Exponential(rate*QE);
  	return (arrival);
}

void initializeEventList(long *n, int numCenters) {
	events.carArr1.carArrivalTime = getCarArrival(START);
	printf("%f\n", events.carArr1.carArrivalTime);
	events.carArr1.isCarArrivalActive = true;
	events.familyArr1.familyArrivalTime = getFamilyArrival1(START);
	printf("%f\n", events.familyArr1.familyArrivalTime);
	events.familyArr1.isFamilyArrivalActive = true;

	events.familyArr2.familyArrivalTime = getFamilyArrival2(START);
	printf("%f\n", events.familyArr2.familyArrivalTime);
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

	events.completionTimes1 = (double *) malloc(sizeof(double)*n[0]);
	events.completionTimes2 = (double *) malloc(sizeof(double)*n[1]);
	events.completionTimes3 = (double *) malloc(sizeof(double)*n[2]);
	events.completionTimes4 = (double *) malloc(sizeof(double)*n[3]);
	events.completionTimes5 = (double *) malloc(sizeof(double)*n[4]);

	for(int i=0; i<numCenters; i++) {
		printf("%ld\n", n[i]);
	}
}

int main(int argc, char **argv){
	//TODO: statistiche di output

	if(argc < 7)
		exit(-1);

	long* n = NULL;
	n = (long *)malloc(sizeof(long)*(argc - 1));
	if(n==NULL)
		exit(-2);

	int interval = atoi(argv[6]);

	n[0] = atoi(argv[1]);
	n[1] = atoi(argv[2]);
	n[2] = atoi(argv[3]);
	n[3] = atoi(argv[4]);
	n[4] = atoi(argv[5]);

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

	initializeEventList(n, argc-2);

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
