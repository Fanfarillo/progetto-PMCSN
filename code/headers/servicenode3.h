#include "../dataStructure/eventList.h"
#include "../dataStructure/time.h"
#include "../dataStructure/area.h"
#include "../dataStructure/stateVariables.h"
#include "../dataStructure/numArrLoss.h"
#include "../dataStructure/arrivalTimes.h"
#include "../dataStructure/utilStructs.h"

void carArrival3(struct event_list *eventsPtr, struct time *tPtr, struct state_variables1 *svPtr, struct arrival_loss *alPtr, struct arrivals *arrPtr);
void familyArrival3(struct event_list *eventsPtr, struct time *tPtr, struct state_variables1 *svPtr, struct arrival_loss *alPtr, struct arrivals *arrPtr);
void carDeparture3(struct event_list *eventsPtr, struct time *tPtr, struct state_variables1 *svPtr, struct arrivals *arrPtr, int serverOffset);
void familyDeparture3(struct event_list *eventsPtr, struct time *tPtr, struct state_variables1 *svPtr, struct arrivals *arrPtr, int serverOffset);
