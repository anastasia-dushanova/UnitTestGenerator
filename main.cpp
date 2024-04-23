#include "unittestgenerator.h"

#include <QApplication>
#include <QDebug>
//#include <chromosome.h>
#include <population.h>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
//    UnitTestGenerator w;
//    w.show();

//    Chromosome ch;

    Population* pop = new Population();
    pop->initMethodInfo(19);
//    pop->printMethodInfo();
    pop->initPopulation();
//    pop->printChromosome();
    pop->avgFitnessCalculation();
    pop->operatorSelection();
    pop->operatorCrossover();
    pop->operatorMutation();
    pop->operatorReduction();
    pop->avgFitnessCalculation();

    return a.exec();
}
