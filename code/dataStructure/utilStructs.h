#include <stdbool.h>

struct next_abandon {
    int jobId;
    double abandonTime;
};

struct next_completion {
    int serverOffset;
    bool isFamily;
    double completionTime; 
};