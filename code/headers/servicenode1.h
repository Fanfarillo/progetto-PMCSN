#include "../dataStructure/eventList.h"
#include "../dataStructure/time.h"
#include "../dataStructure/area.h"
#include "../dataStructure/stateVariables.h"
#include "../dataStructure/numArrLoss.h"
#include "../dataStructure/arrivalTimes.h"
#include "../dataStructure/utilStructs.h"

#include<stdbool.h>

void carArrival1(struct event_list *eventsPtr, struct time *tPtr, struct state_variables1 *svPtr, struct arrival_loss *alPtr, int len, bool simType);
void familyArrival1(struct event_list *eventsPtr, struct time *tPtr, struct state_variables1 *svPtr, struct arrival_loss *alPtr, int len, bool simType);
void carDeparture1(struct event_list *eventsPtr, struct time *tPtr, struct state_variables1 *svPtr, struct arrivals *arrPtr, int serverOffset, int len, struct arrival_loss *alPtr);
void familyDeparture1(struct event_list *eventsPtr, struct time *tPtr, struct state_variables1 *svPtr, struct arrivals *arrPtr, int serverOffset, int len, struct arrival_loss *alPtr);
void abandon1(struct event_list *eventsPtr, struct state_variables1 *svPtr, struct arrival_loss *alPtr, int jobId);
void fixState1(struct event_list *eventsPtr, struct time *tPtr, struct state_variables1 *svPtr, int firstServerOffset, int variation);
