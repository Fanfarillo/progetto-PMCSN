#define START 0.0
#define STOP 100.0
#define N 3

#define P_CAR 0.6
#define QE 0.4
#define QT 0.5
#define QP 0.4

double Exponential(double mu);
double getCarArrival(double arrival);
double getFamilyArrival1(double arrival);
double getFamilyArrival2(double arrival);
double getService1(double start);
double getService2(double start);
double getService3(double start);
double getService4(double start);
double getService5(double start);
double getAbandon1(double arrival);
double getAbandon2(double arrival);
double getPlayProb();
double getTableProb();

extern double interTime;
