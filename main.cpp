#include "unittestgenerator.h"

#include <QApplication>
#include <QDebug>
#include <population.h>
#include <populationscontroller.h>
#include <testcluster.h>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
//    UnitTestGenerator w;
//    w.show();

//    Population* pop1 = new Population(1, 12);
//    Population* pop2 = new Population(2, 12);

    TestCluster* cluster = new TestCluster("main.json");
    cluster->makeClusters();

    PopulationsController* cont = new PopulationsController(12);
//    cont->appendPopulation(pop1);
//    cont->appendPopulation(pop2);
//    cont->initPopulation();

    cont->checkFiles();
    cont->initPopulation();

    return a.exec();
}
