#include "../dataStructure/eventList.h"
#include "../dataStructure/time.h"
#include "../dataStructure/area.h"
#include "../dataStructure/stateVariables.h"
#include "../dataStructure/numArrLoss.h"
#include "../dataStructure/arrivalTimes.h"
#include "../dataStructure/utilStructs.h"

void arrival4(struct event_list *eventsPtr, struct time *tPtr, struct state_variables2 *svPtr, struct arrival_loss *alPtr, struct arrivals *arrPtr, int len);
void departure4(struct event_list *eventsPtr, struct time *tPtr, struct state_variables2 *svPtr, struct arrivals *arrPtr, int serverOffset, int n, struct arrival_loss *alPtr);
