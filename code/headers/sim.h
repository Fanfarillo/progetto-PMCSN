#define REPLICATIONS 128
#define CENTERS 5
#define STATISTICS 11
#define INTERVALS 6
#define SAMPLINGINTERVAL 300

#define B 1024
#define K 128
#define LOC 0.99

struct result_finite {
    double ****nsim;
    double ****samplingTime;
    double ***numMedioServentiAttivi;
};

struct result_finite *finite_sim(int **array_m);
double*** infinite_sim(int *m);
