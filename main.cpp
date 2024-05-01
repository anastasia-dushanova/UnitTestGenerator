#include "unittestgenerator.h"

#include <QApplication>
#include <QDebug>
//#include <chromosome.h>
#include <population.h>
#include <testcasedecor.h>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
//    UnitTestGenerator w;
//    w.show();

//    Chromosome ch;

//    Population* pop = new Population();
//    pop->initMethodInfo(5);
//    pop->printMethodInfo();
//    pop->initPopulation();
//    pop->printChromosome();
//    int i{0};
//    while(pop->checkStopCondition() == false){
//        qDebug() << "\nИТЕРАЦИЯ № "<<i++;
//        pop->fitnessCalculation();
//        pop->operatorSelection();
//        pop->operatorCrossover();
//        pop->operatorMutation();
//        pop->operatorReduction();
//        pop->fitnessCalculation();
//        pop->avgFitnessCalculation();
//        pop->printChromosome();
//    }

//    pop->printChromosome();

    TestCaseDecor* decor = new TestCaseDecor("decor.txt");
    decor->decor();

    delete  decor;
    return a.exec();
}
