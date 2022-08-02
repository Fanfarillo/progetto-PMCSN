#include "../dataStructure/eventList.h"
#include "../dataStructure/time.h"
#include "../dataStructure/area.h"
#include "../dataStructure/stateVariables.h"
#include "../dataStructure/numArrLoss.h"
#include "../dataStructure/arrivalTimes.h"
#include "../dataStructure/utilStructs.h"

void arrival2(struct event_list *eventsPtr, struct time *tPtr, struct state_variables2 *svPtr, struct arrival_loss *alPtr, int len);
void departure2(struct event_list *eventsPtr, struct time *tPtr, struct state_variables2 *svPtr, struct arrivals *arrPtr, int serverOffset, int n);
void abandon2(struct event_list *eventsPtr, struct state_variables2 *svPtr, struct arrival_loss *alPtr, int jobId);
