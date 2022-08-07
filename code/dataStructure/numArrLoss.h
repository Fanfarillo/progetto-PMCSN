#ifndef NUMARRLOSS_H		//header guard
#define NUMARRLOSS_H

struct arrival_loss {
    //numero di automobili che arrivano al centro
    int index_a;
    //numero delle famiglie che arrivano al centro
    int index_f;
    //numero delle famiglie che abbandonano
    int numLoss_f;
    //numero delle famiglie che sono arrivate al centro fino alla fascia oraria precendente
    int prev_index_a;
    //numero delle famiglie che sono arrivate al centro fino alla fascia oraria precendente
    int prev_index_f;
    //numero delle famiglie che hanno abbandonato fino alla fascia oraria precedente
    int prev_numLoss_f;
    //numero completamenti delle macchine
    int compl_a;
    //numero completamenti delle famiglie
    int compl_f;
    //numero completamenti delle macchine fino alla fascia oraria precedente
    int prev_compl_a;
    //numero completamenti delle famiglie fino alla fascia oraria precedente
    int prev_compl_f; 
};

#endif
