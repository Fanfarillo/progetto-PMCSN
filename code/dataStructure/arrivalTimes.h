struct arrivalTime {
    double timeValue;
    struct arrivalTime *prev;
    struct arrivalTime *next;
};

struct arrivals {
    struct arrivalTime *head3;
    struct arrivalTime *tail3;
    struct arrivalTime *head4;
    struct arrivalTime *tail4;
    struct arrivalTime *head5;
    struct arrivalTime *tail5;
};