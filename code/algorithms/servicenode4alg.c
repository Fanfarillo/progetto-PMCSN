#include<stdio.h>
#include <math.h>
#include <stdlib.h>
#include "../headers/servicenode4.h"/*
#include "../headers/rngs.h"
#include "../dataStructure/eventList.h"
#include "../dataStructure/time.h"
#include "../dataStructure/area.h"
#include "../dataStructure/stateVariables.h"
#include "../dataStructure/numArrLoss.h"
#include "../dataStructure/arrivalTimes.h"
#include "../dataStructure/utilStructs.h"*/
#include "../headers/randomGeneratorFunctions.h"

#define qP 0.32

void arrival4(struct event_list *eventsPtr, struct time *tPtr, struct state_variables2 *svPtr, struct arrival_loss *alPtr, struct arrivals *arrPtr, int len){
	//incremento il numero delle famiglie che arrivano al centro
	alPtr->index_f = alPtr->index_f + 1;
	//incremento la popolazione di 1
	svPtr->l = svPtr->l + 1;
	
	struct arrival_time *toRemove = arrPtr->head4;

	if(toRemove->next == NULL) {
		arrPtr->head4 = NULL;
		arrPtr->tail4 = NULL;
		eventsPtr->familyArr4.familyArrivalTime = (double) INFINITY;	//Se la lista degli arrivi diventa vuota, vuol dire che per ora non ci sono nuovi arrivi per questo centro
		tPtr->last[3] = tPtr->current;
	}
	else {
		arrPtr->head4 = toRemove->next;
		arrPtr->head4->prev = NULL;
		eventsPtr->familyArr4.familyArrivalTime = arrPtr->head4->timeValue;
	}
	free(toRemove);

	int idleOffset = -1;
	//for(int i=0; i<len(svPtr->x); i++) {
	for(int i=0; i<len; i++) {
		if(svPtr->x[i] == 0) {		//0 == IDLE
			idleOffset = i;
			break;
		}
	}

	if(idleOffset >= 0) {
		svPtr->x[idleOffset] = 1;
		eventsPtr->completionTimes4[idleOffset] = getService4(tPtr->current);
	}
}
void departure4(struct event_list *eventsPtr, struct time *tPtr, struct state_variables2 *svPtr, struct arrivals *arrPtr, int serverOffset, int n){
	svPtr->l = svPtr->l - 1;
	
	

	if(svPtr->l >= n) {
		//almeno un job in coda e genero il suo tempo di completamento
		eventsPtr->completionTimes4[serverOffset] = getService4(tPtr->current);
	}
	else {
		eventsPtr->completionTimes4[serverOffset] = (double) INFINITY;
		svPtr->x[serverOffset] = 0;
	}

	//TODO: sistemare il generatore random in modo da utilizzare un solo generatore ma con il suo stream
	if(getPlayProb() < qP){
		
		struct arrival_time *tailArrival = (struct arrival_time *) malloc(sizeof(struct arrival_time));
		tailArrival->isFamily = true;
		tailArrival->timeValue = tPtr->current;
		tailArrival->next = NULL;
		tailArrival->prev = arrPtr->tail5;

		if(arrPtr->tail5 != NULL) {
			arrPtr->tail5->next = tailArrival;
		}
		else {
			arrPtr->head5 = tailArrival;
			eventsPtr->familyArr5.familyArrivalTime = tPtr->current;
		}
		arrPtr->tail5 = tailArrival;
	}
	
}
