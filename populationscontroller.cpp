#include "populationscontroller.h"

PopulationsController::PopulationsController(int total, QObject *parent)
    : QObject(parent),
      populationsReady(0),
      populationsFinished(0),
      countSwap(0),
      totalIterations(total){

    for(int i{0}; i < 4; ++i)
        listFileNames.insert(QString::number(i+1)+"_text.json", false);
}

PopulationsController::~PopulationsController(){
    for(int i{0}; i < listThreads.size(); ++i){
        listThreads.at(i)->quit();
        listThreads.at(i)->wait();
    }

    for(int i{0}; i < listPopulation.size(); ++i){
        delete listPopulation.at(i);
    }

    delete decor;
}

void PopulationsController::initPopulation(){

    qDebug() << "КОЛИЧЕСТВО ПОПУЛЯЦИЙ = "<<listPopulation.size();

    if(listPopulation.size() == 0){
        qDebug() << "КОЛИЧЕСТВО ПОПУЛЯЦИЙ = 0";
        return;
    }

    for(int i{0}; i < listPopulation.size(); ++i){
        QThread* thread = new QThread(this);
        listPopulation.at(i)->moveToThread(thread);

        listThreads.append(thread);

        connect(listPopulation.at(i), SIGNAL(signalReadySwap()), this, SLOT(slotCheckCount()));
        connect(listPopulation.at(i), SIGNAL(signalFinish()), this, SLOT(slotFinish()));
        connect(this, SIGNAL(signalStart()), listPopulation.at(i), SLOT(start()));
        connect(this, SIGNAL(signalContinue()), listPopulation.at(i), SLOT(start()));

        thread->start();
    }

    emit signalStart();

}

void PopulationsController::checkFiles(){
    int amount{0};

    for(auto itr = listFileNames.begin(); itr != listFileNames.end(); ++itr){
        QFile file(QApplication::applicationDirPath() + "/" + itr.key());
        if(file.size() != 0){
            ++amount;
            itr.value() = true;
        }
    }

    int i{0};
    for(auto itr = listFileNames.begin(); itr != listFileNames.end(); ++itr){
        if(itr.value() == true){
            Population* pop = new Population(++i, totalIterations, itr.key());
            listPopulation.append(pop);
        }
    }

}

void PopulationsController::slotCheckCount(){
    populationsReady++;
    if(populationsReady == listPopulation.size()){
        ++countSwap;
        qDebug() << "ВСЕ ПОПУЛЯЦИИ ГОТОВЫ К ОБМЕНУ ХРОМОСОМАМИ. НОМЕР ОБМЕНА "<<QString::number(countSwap);

        swapChromosoms();

        populationsReady = 0;
    }
}

void PopulationsController::swapChromosoms(){

    if(listPopulation.size() == 1){
        qDebug() << "КОЛИЧЕСТВО ПОПУЛЯЦИЙ - 1. ОБМЕНА НЕ ПРОИСХОДИТ";
        qDebug() << "ПРОДОЛЖАЕМ АЛГОРИТМ";
        emit signalContinue();
        return;
    }

    for(int i{0}; i < listPopulation.size(); ++i){

        if(i == listPopulation.size()-1){
            listPopulation.at(i)->receiveChromosomes(listPopulation.at(0)->sendChromosomes(1));
            break;
        }

        listPopulation.at(i)->receiveChromosomes(listPopulation.at(i+1)->sendChromosomes(1));
    }

    qDebug() << "Популяции обменялись особями\nПродолжаем...";
    emit signalContinue();
}

void PopulationsController::slotFinish(){

    //TODO установить таймеры

    populationsFinished++;
    if(populationsFinished == listPopulation.size()){
        qDebug() << "\nГОТОВО. ВСЕ ПОПУЛЯЦИИ ЗАВЕРШИЛИ СВОИ ОПЕРАЦИИ\nБыло выполнено обменов: "<<QString::number(countSwap);

        populationsFinished = 0;
    }

    decor = new TestCaseDecor("decor.txt");

    for(auto p : listPopulation)
        decor->appendToListTestCase(p->getCurrentList());

    decor->decor();

}
