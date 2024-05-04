#ifndef POPULATIONSCONTROLLER_H
#define POPULATIONSCONTROLLER_H

#include <QObject>
#include <QThread>
#include <QTimer>
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

    QMap<QString, bool> listFileNames;

    const int totalIterations;

    QTimer timer;

};

#endif // POPULATIONSCONTROLLER_H
