#include "unittestgenerator.h"

#include <QApplication>
#include <QDebug>
#include <population.h>
#include <populationscontroller.h>
#include <testcluster.h>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    UnitTestGenerator w;
    w.show();


    return a.exec();
}
