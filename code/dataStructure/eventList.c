#include <stdbool.h>

struct job {
    long id;
    long abandonTime;
    job *prev;
    job *next; 
} job;

struct car_arrival {
    long carArrivalTime;
    bool isCarArrivalActive;
} carArrival;

struct family_arrival {
    long familyArrivalTime;
    bool isFamilyArrivalActive;
} familyArrival;

struct event_list {
    carArrival carArr1;
    familyArrival familyArr1;
    long *completionTimes1;
    job *head1;
    job *tail1;
    familyArrival familyArr2;
    long *completionTimes2;
    job *head2;
    job *tail2;
    carArrival carArr3;
    familyArrival familyArr3;
    long *completionTimes3;
    familyArrival familyArr4;
    long *completionTimes4;
    familyArrival familyArr5;
    long *completionTimes5;
} evenList;