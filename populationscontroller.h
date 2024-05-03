#ifndef POPULATIONSCONTROLLER_H
#define POPULATIONSCONTROLLER_H

#include <QObject>
#include <QThread>
#include <population.h>
#include <testcasedecor.h>

class PopulationsController : public QObject
{
    Q_OBJECT
public:
    explicit PopulationsController(int total, QObject *parent = nullptr);
    ~PopulationsController();

    void setListPopulation(QList<Population*> count) { this->listPopulation = count; }
    QList<Population*> getListPopulation() { return listPopulation; }

//    void setTotalIteration(int total) { this->totalIterations = total; }
    int getTotalIteration () { return totalIterations; }

    void appendPopulation(Population* pop){ listPopulation.append(pop); }

    void checkFiles();
    void initPopulation();

signals:
    void signalStart();
    void signalContinue();

public slots:

    void slotFinish();
    void slotCheckCount();

private:
    void swapChromosoms();
    QList<QThread*> listThreads;
    QList<Population*> listPopulation;

    int populationsReady;

    int populationsFinished;

    int countSwap;

    TestCaseDecor* decor;

    QStringList listFileNames;

    const int totalIterations;

};

#endif // POPULATIONSCONTROLLER_H
