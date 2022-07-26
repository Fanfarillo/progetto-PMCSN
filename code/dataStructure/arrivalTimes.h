struct arrival_time {
    double timeValue;
    struct arrival_time *prev;
    struct arrival_time *next;
};

struct arrivals {
    struct arrival_time *head3;
    struct arrival_time *tail3;
    struct arrival_time *head4;
    struct arrival_time *tail4;
    struct arrival_time *head5;
    struct arrival_time *tail5;
};