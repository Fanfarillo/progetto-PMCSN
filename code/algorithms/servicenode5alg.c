#include "../headers/servicenode5.h"
#include "../headers/randomGeneratorFunctions.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

void arrival5(struct event_list *eventsPtr, struct time *tPtr, struct state_variables2 *svPtr, struct arrival_loss *alPtr, struct arrivals *arrPtr, int len){

	alPtr->index_f = alPtr->index_f + 1;

	//Rimozione del nodo testa dalla lista degli arrivi al centro 5 (playground)
	struct arrival_time *toRemove = arrPtr->head5;
	tPtr->last[4] = tPtr->current;
	if(toRemove->next == NULL) {
		arrPtr->head5 = NULL;
		arrPtr->tail5 = NULL;
		eventsPtr->familyArr5.familyArrivalTime = (double) INFINITY;	//Se la lista degli arrivi diventa vuota, vuol dire che per ora non ci sono nuovi arrivi per questo centro
		
	}
	else {
		arrPtr->head5 = toRemove->next;
		arrPtr->head5->prev = NULL;
		eventsPtr->familyArr5.familyArrivalTime = arrPtr->head5->timeValue;
	}
	free(toRemove);

	int idleOffset = -1;
	for(int i=0; i<len; i++) {
		if(svPtr->x[i] == 0) {		//0 == IDLE
			idleOffset = i;
			break;
		}
	}

	if(idleOffset >= 0) {
		svPtr->x[idleOffset] = 1;
		eventsPtr->completionTimes5[idleOffset] = getService5(tPtr->current);
	}
	else {
		alPtr->numLoss_f = alPtr->numLoss_f + 1;
	}

}

void departure5(struct event_list *eventsPtr, struct state_variables2 *svPtr, int serverOffset, struct arrival_loss *alPtr){
	alPtr -> compl_f += 1;
	eventsPtr->completionTimes5[serverOffset] = (double) INFINITY;
	svPtr->x[serverOffset] = 0;

}
