#ifndef EVENTLIST_H		//header guard
#define EVENTLIST_H

#include <stdbool.h>

struct job {
    int id;
    double abandonTime;
    struct job *prev;
    struct job *next; 
};

struct car_arrival {
    double carArrivalTime;
    bool isCarArrivalActive;
};

struct family_arrival {
    double familyArrivalTime;
    bool isFamilyArrivalActive;
};

struct event_list {
    struct car_arrival carArr1;
    struct family_arrival familyArr1;
    double *completionTimes1;
    struct job *head1;
    struct job *tail1;
    struct family_arrival familyArr2;
    double *completionTimes2;
    struct job *head2;
    struct job *tail2;
    struct car_arrival carArr3;
    struct family_arrival familyArr3;
    double *completionTimes3;
    struct family_arrival familyArr4;
    double *completionTimes4;
    struct family_arrival familyArr5;
    double *completionTimes5;
};

#endif
