#include "../headers/servicenode3.h"
#include "../headers/randomGeneratorFunctions.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

void carArrival3(struct event_list *eventsPtr, struct time *tPtr, struct state_variables1 *svPtr, struct arrival_loss *alPtr, struct arrivals *arrPtr, int len){
	//incremento il numero delle automobili che arrivano al centro
	alPtr->index_a = alPtr->index_a + 1;
		
	struct arrival_time *toRemove = arrPtr->head3;
	tPtr->last[2] = tPtr->current;
	if(toRemove->next == NULL) {
		arrPtr->head3 = NULL;
		arrPtr->tail3 = NULL;
		eventsPtr->carArr3.carArrivalTime = (double) INFINITY;	//Se la lista degli arrivi diventa vuota, vuol dire che per ora non ci sono nuovi arrivi per questo centro
		
	}
	else {
		arrPtr->head3 = toRemove->next;
		arrPtr->head3->prev = NULL;
		eventsPtr->carArr3.carArrivalTime = arrPtr->head3->timeValue;
	}
	free(toRemove);

	int idleOffset = -1;
	for(int i=0; i<len; i++) {
		if(svPtr->x[i] == 0) {		//0 == IDLE
			idleOffset = i;
			break;
		}
	}

	bool existsCar = false;
	for(int i=0; i<len; i++) {
		if(svPtr->x[i] == 2 || svPtr->x[i] == -2) {		//2 == CAR; -2 == CAR ma in un servente che è stato appena disattivato
			existsCar = true;
			break;
		}
	}
	
	if(idleOffset >= 0 && !existsCar && svPtr->qA == 0){
		svPtr->x[idleOffset] = 2;
		eventsPtr->completionTimes3[idleOffset] = getService3(tPtr->current);
	}
	else{
		svPtr->qA++;
	}	
}



void familyArrival3(struct event_list *eventsPtr, struct time *tPtr, struct state_variables1 *svPtr, struct arrival_loss *alPtr, struct arrivals *arrPtr, int len){
	//incremento il numero delle famiglie che arrivano al centro
	alPtr->index_f = alPtr->index_f + 1;
	
	
	struct arrival_time *toRemove = arrPtr->head3;
	tPtr->last[2] = tPtr->current;
	if(toRemove->next == NULL) {
		arrPtr->head3 = NULL;
		arrPtr->tail3 = NULL;
		eventsPtr->familyArr3.familyArrivalTime = (double) INFINITY;	//Se la lista degli arrivi diventa vuota, vuol dire che per ora non ci sono nuovi arrivi per questo centro
		
	}
	else {
		arrPtr->head3 = toRemove->next;
		arrPtr->head3->prev = NULL;
		eventsPtr->familyArr3.familyArrivalTime = arrPtr->head3->timeValue;
	}
	free(toRemove);
	

	int idleOffset = -1;
	for(int i=0; i<len; i++) {
		if(svPtr->x[i] == 0) {		//0 == IDLE
			idleOffset = i;
			break;
		}
	}
	
	bool existsCar = false;
	for(int i=0; i<len; i++) {
		if(svPtr->x[i] == 2 || svPtr->x[i] == -2) {		//2 == CAR; -2 == CAR ma in un servente che è stato appena disattivato
			existsCar = true;
			break;
		}
	}

	if(idleOffset >= 0 && svPtr->qF == 0 && (existsCar || svPtr->qA == 0)) {
		svPtr->x[idleOffset] = 1;
		eventsPtr->completionTimes3[idleOffset] = getService3(tPtr->current);
	}
	else {
		svPtr->qF++;
	}
}
void carDeparture3(struct event_list *eventsPtr, struct time *tPtr, struct state_variables1 *svPtr, struct arrivals *arrPtr, int serverOffset, struct arrival_loss *alPtr){
	alPtr -> compl_a += 1;

	if(svPtr->x[serverOffset] < 0) {
		eventsPtr->completionTimes3[serverOffset] = (double) INFINITY;
		svPtr->x[serverOffset] = -3;
	}
	else if(svPtr->qA != 0) {
		svPtr->qA--;
		eventsPtr->completionTimes3[serverOffset] = getService3(tPtr->current);
	}
	else if(svPtr->qF!=0){
		svPtr->qF--;
		eventsPtr->completionTimes3[serverOffset] = getService3(tPtr->current);
		svPtr->x[serverOffset] = 1;
	}
	else{
		eventsPtr->completionTimes3[serverOffset] = (double) INFINITY;
		svPtr->x[serverOffset] = 0;
	}
}


void familyDeparture3(struct event_list *eventsPtr, struct time *tPtr, struct state_variables1 *svPtr, struct arrivals *arrPtr, int serverOffset, int len, struct arrival_loss *alPtr){
	alPtr -> compl_f += 1;
	bool existsCar = false;
	for(int i=0; i<len; i++) {
		if(svPtr->x[i] == 2 || svPtr->x[i] == -2) {		//2 == CAR; -2 == CAR ma in un servente che è stato appena disattivato
			existsCar = true;
			break;
		}
	}

	if(svPtr->x[serverOffset] < 0) {
		eventsPtr->completionTimes3[serverOffset] = (double) INFINITY;
		svPtr->x[serverOffset] = -3;
	}
	else if(!existsCar && svPtr->qA!=0) {
		//almeno un job in coda e genero il suo tempo di completamento
		eventsPtr->completionTimes3[serverOffset] = getService3(tPtr->current);
		svPtr->qA--;
		svPtr->x[serverOffset] = 2;
	}
	else if((svPtr->qA==0 && svPtr->qF!=0) || (svPtr->qF!=0 && existsCar)){
		eventsPtr->completionTimes3[serverOffset] = getService3(tPtr->current);
		svPtr->qF--;
	}
	else {
		eventsPtr->completionTimes3[serverOffset] = (double) INFINITY;
		svPtr->x[serverOffset] = 0;
	}
	
	if(getTableProb() < QT){
		//Inserimento in coda di un nuovo nodo all'interno della lista degli arrivi al centro 4
		struct arrival_time *tailArrival = (struct arrival_time *) malloc(sizeof(struct arrival_time));
		 
		tailArrival->isFamily = true;
		tailArrival->timeValue = tPtr->current;
		tailArrival->next = NULL;
		tailArrival->prev = arrPtr->tail4;

		if(arrPtr->tail4 != NULL) {
			arrPtr->tail4->next = tailArrival;
		}
		else {
			arrPtr->head4 = tailArrival;
			eventsPtr->familyArr4.familyArrivalTime = tPtr->current;
		}
		arrPtr->tail4 = tailArrival;		
	}

}

void fixState3(struct event_list *eventsPtr, struct time *tPtr, struct state_variables1 *svPtr, int firstServerOffset, int variation) {

	bool existsCar = false;
	for(int i=0; i<firstServerOffset; i++) {
		if(svPtr->x[i] == 2 || svPtr->x[i] == -2) {		//2 == CAR; -2 == CAR ma in un servente che è stato appena disattivato
			existsCar = true;
			break;
		}
	}

	if(!existsCar && svPtr->qA > 0) {
		eventsPtr->completionTimes3[firstServerOffset] = getService3(tPtr->current);
		svPtr->qA -= 1;
		svPtr->x[firstServerOffset] = 2;
		firstServerOffset++;
		variation--;
	}
	if(variation > 0 && svPtr->qF > 0) {
		int numInService;
		if(variation < svPtr->qF)
			numInService = variation;
		else
			numInService = svPtr->qF;

		for(int i=0; i<numInService; i++) {
			svPtr->qF--;
			eventsPtr->completionTimes3[firstServerOffset+i] = getService3(tPtr->current);
			svPtr->x[firstServerOffset+i] = 1;			

		}

	}

}
