#include "../dataStructure/eventList.h"
#include "../dataStructure/time.h"
#include "../dataStructure/area.h"
#include "../dataStructure/stateVariables.h"
#include "../dataStructure/numArrLoss.h"
#include "../dataStructure/arrivalTimes.h"
#include "../dataStructure/utilStructs.h"

#include<stdbool.h>

void arrival2(struct event_list *eventsPtr, struct time *tPtr, struct state_variables2 *svPtr, struct arrival_loss *alPtr, int len, int currLen, bool simType);
void departure2(struct event_list *eventsPtr, struct time *tPtr, struct state_variables2 *svPtr, struct arrivals *arrPtr, int serverOffset, int n, struct arrival_loss *alPtr);
void abandon2(struct event_list *eventsPtr, struct state_variables2 *svPtr, struct arrival_loss *alPtr, int jobId);
void fixState2(struct event_list *eventsPtr, struct time *tPtr, struct state_variables2 *svPtr, int firstServerOffset, int variation);
