#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<stdbool.h>
#include "../headers/servicenode1.h"/*
#include "../dataStructure/eventList.h"
#include "../dataStructure/time.h"
#include "../dataStructure/area.h"
#include "../dataStructure/stateVariables.h"
#include "../dataStructure/numArrLoss.h"
#include "../dataStructure/arrivalTimes.h"
#include "../dataStructure/utilStructs.h"*/
#include "../headers/randomGeneratorFunctions.h"

//TODO: troppi stop e N
#define STOP 1000000.0
#define N 3

void carArrival1(struct event_list *eventsPtr, struct time *tPtr, struct state_variables1 *svPtr, struct arrival_loss *alPtr){
	//incremento il numero delle famiglie che arrivano al centro
	alPtr->index_a = alPtr->index_a + 1;
	//genero l'istante di tempo del prossimo arrivo della famiglia
	eventsPtr-> carArr1.carArrivalTime = getCarArrival(tPtr->current);


	if(eventsPtr-> carArr1.carArrivalTime > STOP) {
		eventsPtr-> carArr1.carArrivalTime = (double) INFINITY;
		eventsPtr-> carArr1.isCarArrivalActive = false;
		//l'istante dell'arrivo che sto processando
		tPtr->last[0] = tPtr->current;
	}

	int idleOffset = -1;
	for(int i=0; i<len(svPtr->x); i++) {
		if(svPtr->x[i] == 0) {		//0 == IDLE
			idleOffset = i;
			break;
		}
	}

	bool existsCar = false;
	for(int i=0; i<len(svPtr->x); i++) {
		if(svPtr->x[i] == 2) {		//2 == CAR
			existsCar = true;
			break;
		}
	}
	
	if(idleOffset >= 0 && !existsCar && svPtr->qA == 0){
		svPtr->x[idleOffset] = 2;
		eventsPtr->completionTimes1[idleOffset] = getService1(tPtr->current);
	}
	else{
		svPtr->qA++;
	}	
}





void familyArrival1(struct event_list *eventsPtr, struct time *tPtr, struct state_variables1 *svPtr, struct arrival_loss *alPtr){
	//incremento il numero delle famiglie che arrivano al centro
	alPtr->index_f = alPtr->index_f + 1;
	//genero l'istante di tempo del prossimo arrivo della famiglia
	eventsPtr->familyArr1.familyArrivalTime = getFamilyArrival1(tPtr->current);

	if(eventsPtr->familyArr1.familyArrivalTime > STOP) {
		eventsPtr->familyArr1.familyArrivalTime = (double) INFINITY;
		eventsPtr->familyArr1.isFamilyArrivalActive = false;
		//l'istante dell'arrivo che sto processando
		tPtr->last[0] = tPtr->current;
	}

	int idleOffset = -1;
	for(int i=0; i<len(svPtr->x); i++) {
		if(svPtr->x[i] == 0) {		//0 == IDLE
			idleOffset = i;
			break;
		}
	}
	
	bool existsCar = false;
	for(int i=0; i<len(svPtr->x); i++) {
		if(svPtr->x[i] == 2) {		//2 == CAR
			existsCar = true;
			break;
		}
	}

	if(idleOffset >= 0 && svPtr->qF == 0 && (existsCar || svPtr->qA == 0)) {
		svPtr->x[idleOffset] = 1;
		eventsPtr->completionTimes1[idleOffset] = getService1(tPtr->current);
	}
	else if(svPtr->qF > N) {
		//Inserimento in coda di un nuovo nodo all'interno della lista degli abbandoni

		struct job *tailJob = (struct job *) malloc(sizeof(struct job));
		tailJob->id = alPtr->index_f;
		tailJob->abandonTime = getAbandon1(tPtr->current);
		//inserimento in coda
		tailJob->next = NULL;
		tailJob->prev = eventsPtr->tail1;

		if(eventsPtr->tail1 != NULL) {
			eventsPtr->tail1->next = tailJob;
		}
		else {
			eventsPtr->head1 = tailJob;
		}
		eventsPtr->tail1 = tailJob;

	}

}



void carDeparture1(struct event_list *eventsPtr, struct time *tPtr, struct state_variables1 *svPtr, struct arrivals *arrPtr, int serverOffset){

	if(svPtr->qA != 0) {
		svPtr->qA--;
		eventsPtr->completionTimes1[serverOffset] = getService1(tPtr->current);
	}
	else if(svPtr->qF!=0){
		svPtr->qF--;
		eventsPtr->completionTimes1[serverOffset] = getService1(tPtr->current);
		svPtr->x[serverOffset] = 1;
		if(eventsPtr->head1 != NULL) {
			//Rimozione del nodo testa dalla lista degli abbandoni

			struct job *toRemove = eventsPtr->head1;
			if(toRemove->next == NULL) {
				eventsPtr->head1 = NULL;
				eventsPtr->tail1 = NULL;
			}
			else {
				eventsPtr->head1 = toRemove->next;
				eventsPtr->head1->prev = NULL;
			}
			free(toRemove);
		}
	}
	else{
		eventsPtr->completionTimes1[serverOffset] = (double) INFINITY;
		svPtr->x[serverOffset] = 0;
	}

	//Inserimento in coda di un nuovo nodo all'interno della lista degli arrivi al centro 3 (withdraw food)
	struct arrival_time *tailArrival = (struct arrival_time *) malloc(sizeof(struct arrival_time));
	tailArrival->isFamily = false;
	tailArrival->timeValue = tPtr->current;
	tailArrival->next = NULL;
	tailArrival->prev = arrPtr->tail3;

	if(arrPtr->tail3 != NULL) {
		arrPtr->tail3->next = tailArrival;
	}
	else {
		arrPtr->head3 = tailArrival;
		eventsPtr->carArr3.carArrivalTime = tPtr->current;
	}
	arrPtr->tail3 = tailArrival;
}


void familyDeparture1(struct event_list *eventsPtr, struct time *tPtr, struct state_variables1 *svPtr, struct arrivals *arrPtr, int serverOffset){

	
	bool existsCar = false;
	for(int i=0; i<len(svPtr->x); i++) {
		if(svPtr->x[i] == 2) {		//2 == CAR
			existsCar = true;
			break;
		}
	}

	if(!existsCar && svPtr->qA!=0) {
		//almeno un job in coda e genero il suo tempo di completamento
		eventsPtr->completionTimes1[serverOffset] = getService1(tPtr->current);
		svPtr->qA--;
		svPtr->x[serverOffset] = 2;
	}
	else if((svPtr->qA==0 && svPtr->qF!=0) || (svPtr->qF!=0 && existsCar)){
		eventsPtr->completionTimes1[serverOffset] = getService1(tPtr->current);
		svPtr->qF--;
		if(eventsPtr->head1 != NULL) {
			//Rimozione del nodo testa dalla lista degli abbandoni

			struct job *toRemove = eventsPtr->head1;
			if(toRemove->next == NULL) {
				eventsPtr->head1 = NULL;
				eventsPtr->tail1 = NULL;
			}
			else {
				eventsPtr->head1 = toRemove->next;
				eventsPtr->head1->prev = NULL;
			}
			free(toRemove);
		}
	}
	else {
		eventsPtr->completionTimes1[serverOffset] = (double) INFINITY;
		svPtr->x[serverOffset] = 0;
	}

	//Inserimento in coda di un nuovo nodo all'interno della lista degli arrivi al centro 3 (withdraw food)
	struct arrival_time *tailArrival = (struct arrival_time *) malloc(sizeof(struct arrival_time));
	//viene dal centro delle casse automatiche
	tailArrival->isFamily = true;
	tailArrival->timeValue = tPtr->current;
	tailArrival->next = NULL;
	tailArrival->prev = arrPtr->tail3;

	if(arrPtr->tail3 != NULL) {
		arrPtr->tail3->next = tailArrival;
	}
	else {
		arrPtr->head3 = tailArrival;
		eventsPtr->familyArr3.familyArrivalTime = tPtr->current;
	}
	arrPtr->tail3 = tailArrival;
	
}


void abandon1(struct event_list *eventsPtr, struct state_variables1 *svPtr, struct arrival_loss *alPtr, int jobId){
	struct job *current = eventsPtr->head1;
	while(current != NULL) {
		if(current->id == jobId)
			break;
		current = current->next;
	}

	struct job *previous = current->prev;
	struct job *successor = current->next;

	if(previous != NULL) {
		previous->next = successor;
	}
	else {
		eventsPtr->head1 = successor;
	}
	
	if(successor != NULL) {
		successor->prev = previous;
	}
	else {
		eventsPtr->tail1 = previous;
	}

	free(current);

	alPtr->numLoss_f = alPtr->numLoss_f + 1;
	svPtr->qF = svPtr->qF - 1;	
}

