#include "../dataStructure/eventList.h"
#include "../dataStructure/time.h"
#include "../dataStructure/area.h"
#include "../dataStructure/stateVariables.h"
#include "../dataStructure/numArrLoss.h"
#include "../dataStructure/arrivalTimes.h"
#include "../dataStructure/utilStructs.h"

void arrival5(struct event_list *eventsPtr, struct time *tPtr, struct state_variables2 *svPtr, struct arrival_loss *alPtr, struct arrivals *arrPtr, int len);
void departure5(struct event_list *eventsPtr, struct state_variables2 *svPtr, int serverOffset, struct arrival_loss *alPtr);
