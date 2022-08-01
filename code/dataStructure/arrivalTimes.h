#ifndef ARRIVALTIMES_H		//header guard
#define ARRIVALTIMES_H

#include <stdbool.h>

struct arrival_time {
    double timeValue;
    bool isFamily;
    struct arrival_time *prev;
    struct arrival_time *next;
};

/**
	liste doppiamente correlate in cui indico i tempi di arrivo dei job
	in base alle partenze dai centri precedenti
**/

struct arrivals {
    struct arrival_time *head3;
    struct arrival_time *tail3;
    struct arrival_time *head4;
    struct arrival_time *tail4;
    struct arrival_time *head5;
    struct arrival_time *tail5;
};

#endif
