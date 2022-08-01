/**
	state_variables1 è relativa ai centri 1 e 3
	state_variables2 è realtiva ai centri 2, 4 e 5
**/

#ifndef STATEVARIABLES_H		//header guard
#define STATEVARIABLES_H

struct state_variables1 {
    int qA;
    int qF;
    int *x;
};

struct state_variables2 {
    int l;
    int *x;
};

#endif
