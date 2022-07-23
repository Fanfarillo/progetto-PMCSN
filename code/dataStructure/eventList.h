#include <stdbool.h>

struct job {
    long id;
    long abandonTime;
    struct job *prev;
    struct job *next; 
};

struct car_arrival {
    long carArrivalTime;
    bool isCarArrivalActive;
};

struct family_arrival {
    long familyArrivalTime;
    bool isFamilyArrivalActive;
};

struct event_list {
    struct car_arrival carArr1;
    struct family_arrival familyArr1;
    long *completionTimes1;
    struct job *head1;
    struct job *tail1;
    struct family_arrival familyArr2;
    long *completionTimes2;
    struct job *head2;
    struct job *tail2;
    struct car_arrival carArr3;
    struct family_arrival familyArr3;
    long *completionTimes3;
    struct family_arrival familyArr4;
    long *completionTimes4;
    struct family_arrival familyArr5;
    long *completionTimes5;
};
