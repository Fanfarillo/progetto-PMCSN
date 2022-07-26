#include "../headers/servicenode2.h"
#include "./dataStructure/eventList.h"
#include "./dataStructure/time.h"
#include "./dataStructure/area.h"
#include "./dataStructure/stateVariables.h"
#include "./dataStructure/numArrLoss.h"
#include "./dataStructure/arrivalTimes.h"
#include "./dataStructure/utilStructs.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define N 3

void arrival2(struct event_list *eventsPtr, struct time *tPtr, struct state_variables2 *svPtr, struct arrival_loss *alPtr){

	alPtr->index_f = alPtr->index_f + 1;
	svPtr->l = svPtr->l + 1;
	eventsPtr->familyArr2.familyArrivalTime = getFamilyArrival2(tPtr->current);

	if(eventsPtr->familyArr2.familyArrivalTime > STOP) {
		eventsPtr->familyArr2.familyArrivalTime = (double) INFINITY;
		eventsPtr->familyArr2.isFamilyArrivalActive = false;
		tPtr->last[1] = tPtr->current;
	}

	int idleOffset = -1;
	for(int i=0; i<len(svPtr->x); i++) {
		if(svPtr->x[i] == 0) {		//0 == IDLE
			idleOffset = i;
			break;
		}
	}

	if(idleOffset >= 0) {
		svPtr->x[idleOffset] = 1;
		eventsPtr->completionTimes2[idleOffset] = getService2(tPtr->current);
	}
	else if(svPtr->l > N + len(svPtr->x)) {
		//Inserimento in coda di un nuovo nodo all'interno della lista degli abbandoni

		struct job *tailJob = (struct job *) malloc(sizeof(struct job));
		tailJob->id = alPtr->index_f;
		tailJob->abandonTime = getAbandon2(tPtr->current);
		tailJob->next = NULL;
		tailJob->prev = eventsPtr->tail2;

		if(eventsPtr->tail2 != NULL) {
			eventsPtr->tail2->next = tailJob;
		}
		else {
			eventsPtr->head2 = tailJob;
		}
		eventsPtr->tail2 = tailJob;

	}

}

void departure2(struct event_list *eventsPtr, struct time *tPtr, struct state_variables2 *svPtr, struct arrivals *arrPtr, int serverOffset){

	svPtr->l = svPtr->l - 1;

	if(eventsPtr->head2 != NULL) {
		//Rimozione del nodo testa dalla lista degli abbandoni

		struct job *toRemove = eventsPtr->head2;
		if(toRemove->next == NULL) {
			eventsPtr->head2 = NULL;
			eventsPtr->tail2 = NULL;
		}
		else {
			eventsPtr->head2 = toRemove->next;
			eventsPtr->head2->prev = NULL;
		}
		free(toRemove);
	}

	if(svPtr->l > 0) {
		eventsPtr->completionTimes2[serverOffset] = getService2(tPtr->current);
	}
	else {
		eventsPtr->completionTimes2[serverOffset] = (double) INFINITY;
		svPtr->x[serverOffset] = 0;
	}

	//Inserimento in coda di un nuovo nodo all'interno della lista degli arrivi al centro 3 (withdraw food)
	struct arrival_time *tailArrival = (struct arrival_time *) malloc(sizeof(struct arrival_time));
	tailArrival->isFamily = true;
	tailArrival->timeValue = tPtr->current;
	tailArrival->next = NULL;
	tailArrival->prev = arrPtr->tail3;

	if(eventsPtr->tail2 != NULL) {
		arrPtr->tail3->next = tailArrival;
	}
	else {
		arrPtr->head3 = tailArrival;
		eventsPtr->familyArr3.familyArrivalTime = tPtr->current;
	}
	arrPtr->tail3 = tailArrival;

}

void abandon2(struct event_list *eventsPtr, struct state_variables2 *svPtr, struct arrival_loss *alPtr, int jobId){
	//Rimozione di un nodo qualsiasi dalla lista degli abbandoni

	struct job *current = eventsPtr->head2;
	while(current != NULL) {
		if(current->id == jobId)
			break;
		current = current->next;
	}

	struct job *previous = current->prev;
	struct job *successor = current->next;

	if(previous != NULL) {
		previous->next = current->next;
	}
	else {
		eventsPtr->head2 = successor;
	}
	
	if(successor != NULL) {
		successor->prev = current->prev;
	}
	else {
		eventsPtr->tail2 = previous;
	}

	free(current);

	alPtr->numLoss_f = alPtr->numLoss_f + 1;
	svPtr->l = svPtr->l - 1;

}
