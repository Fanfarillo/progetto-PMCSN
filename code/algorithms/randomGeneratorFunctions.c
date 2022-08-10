#include "../headers/randomGeneratorFunctions.h"
#include "../headers/rngs.h"
#include <math.h>

#define SERVICE_TIME_1 90
#define SERVICE_TIME_2 180
#define SERVICE_TIME_3 240
#define SERVICE_TIME_4 1800
#define SERVICE_TIME_5 1800
#define ABANDON_TIME_1 360
#define ABANDON_TIME_2 360

double interTime;

double Exponential(double mu)
{
	return (-mu * log(1.0 - Random()));
}

double getCarArrival(double arrival) {
	SelectStream(0);
	if(P_CAR==0)
		arrival = (double) INFINITY;
	else
		arrival += Exponential(interTime/(P_CAR));
	return arrival;
}

double getFamilyArrival1(double arrival){
	SelectStream(1);
	if(1-QE-P_CAR==0)
		arrival = (double) INFINITY;
	else
		arrival += Exponential(interTime/(1-QE-P_CAR));
  	return arrival;
}

double getFamilyArrival2(double arrival) {
	SelectStream(2);
	if(QE==0)
		arrival = (double) INFINITY;
	else
		arrival += Exponential(interTime/QE);
  	return arrival;
}

double getService1(double start) {
	SelectStream(3);
	double departure = start + Exponential(SERVICE_TIME_1);
  	return departure;
}

double getService2(double start) {
	SelectStream(4);
	double departure = start + Exponential(SERVICE_TIME_2);
  	return departure;
}

double getService3(double start) {
	SelectStream(5);
	double departure = start + Exponential(SERVICE_TIME_3);
  	return departure;
}

double getService4(double start) {
	SelectStream(6);
	double departure = start + Exponential(SERVICE_TIME_4);
  	return departure;
}

double getService5(double start) {
	SelectStream(7);
	double departure = start + Exponential(SERVICE_TIME_5);
  	return departure;
}

double getAbandon1(double arrival) {
	SelectStream(8);
	double abandon = arrival + Exponential(ABANDON_TIME_1);
  	return abandon;
}

double getAbandon2(double arrival) {
	SelectStream(9);
	double abandon = arrival + Exponential(ABANDON_TIME_2);
  	return abandon;
}

double getPlayProb(){
	SelectStream(10);
	return Random();
}

double getTableProb(){
	SelectStream(11);
	return Random();
}


