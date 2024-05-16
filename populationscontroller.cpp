#include "populationscontroller.h"

PopulationsController::PopulationsController(QObject *parent)
    : QObject(parent),
      populationsReady(0),
      populationsFinished(0),
      countSwap(0),
      index(5),
      totalsMethods(0),
      coveragedMethods(0){

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

void PopulationsController::deletePopulations(){

    qDebug() << "pass1";
    for(int i{0}; i < listPopulation.size(); ++i){
        Population* pop = listPopulation.at(i);
        delete pop;
    }
    qDebug() << "pass2";
    listPopulation.clear();

    for(int i{0}; i < listThreads.size(); ++i){
        listThreads.at(i)->quit();
        listThreads.at(i)->wait();
    }

}

void PopulationsController::initPopulation(){

    qDebug() << "КОЛИЧЕСТВО ПОПУЛЯЦИЙ = "<<listPopulation.size();
    emit signalWrite(index, QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tКоличество популяций = "+QString::number(listPopulation.size()));

    if(listPopulation.size() == 0){
        qDebug() << "КОЛИЧЕСТВО ПОПУЛЯЦИЙ = 0";
        emit signalWrite(index, QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tКоличество популяций = 0");
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
        connect(listPopulation.at(i), SIGNAL(signalWriteMessage(int,const QString&)), this, SLOT(slotWriteMessage(int,const QString&)));
        connect(listPopulation.at(i), SIGNAL(signalTotalMethods(int)), this, SLOT(slotTotalMethods(int)));
        connect(listPopulation.at(i), SIGNAL(signalCoveragedMethods(int)), this, SLOT(slotCoveragedMethods(int)));

        thread->start();
    }

    emit signalStart();

//    if(timer == nullptr){
//        timer = new QTime();
//        timer->start();
//        qDebug() << "таймер "<<timer;
//    }else{
//        delete timer;
//        timer = new QTime();
//        timer->start();
//        qDebug() << "delete таймер "<<timer;
//    }
    timer.start();

}

void PopulationsController::slotWriteMessage(int index, const QString& message){
    emit signalWrite(index, message);
}

void PopulationsController::slotTotalMethods(int total){

    totalsMethods += total;
}

void PopulationsController::slotCoveragedMethods(int coveraged){

    coveragedMethods += coveraged;
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
            pop->setProbMutation(probMutation);
            pop->setProbCrossover(probCross);
            listPopulation.append(pop);
        }
    }

}

void PopulationsController::slotCheckCount(){
    populationsReady++;
    qDebug() << "популяций "<<populationsReady<<" готово к обмену";
    if(populationsReady == listPopulation.size()){
        ++countSwap;
        qDebug() << "ВСЕ ПОПУЛЯЦИИ ГОТОВЫ К ОБМЕНУ ХРОМОСОМАМИ. НОМЕР ОБМЕНА "<<QString::number(countSwap);
        emit signalWrite(index, "\n"+QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tВсе популяции готовы к обмену хромосомами. Номер обмена "+QString::number(countSwap));
        swapChromosoms();

        populationsReady = 0;

        emit signalIteration();
    }
}

void PopulationsController::swapChromosoms(){

    if(listPopulation.size() == 1){
        qDebug() << "КОЛИЧЕСТВО ПОПУЛЯЦИЙ - 1. ОБМЕНА НЕ ПРОИСХОДИТ";
        qDebug() << "ПРОДОЛЖАЕМ АЛГОРИТМ";
        emit signalWrite(index, QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tКоличество популяций: 1. Обмена не происходит. Алгоритм продолжается.");
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
    emit signalWrite(index, QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tПопуляции обменялись особями. Алгоритм продолжается.");
    emit signalContinue();
}

void PopulationsController::slotFinish(){
    populationsFinished++;
    if(populationsFinished == listPopulation.size()){
        qDebug() << "\nГОТОВО. ВСЕ ПОПУЛЯЦИИ ЗАВЕРШИЛИ СВОИ ОПЕРАЦИИ\nБыло выполнено обменов: "<<QString::number(countSwap);
        emit signalWrite(index, "\n"+QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tГотово. Все популяции завершили свои операции.Было выполнено обменов: "+QString::number(countSwap));

        populationsFinished = 0;


        int time{timer.elapsed()};
        qDebug() << "finish таймер "<<timer;
        qDebug("Прошло времени: %d мс", time);
        decor = new TestCaseDecor("decor.txt");

        for(auto p : listPopulation)
            decor->appendToListTestCase(p->getCurrentList());

        decor->decor();

        emit signalWrite(index,  QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tПрошло времени: " + QString::number(time) + " мс");
        emit signalTimeElapsed(time);
        emit signalFinish(coveragedMethods, totalsMethods);
        emit signalWrite(index, QDateTime::currentDateTime().time().toString("hh:mm:ss")
                         +"\tВсего покрытых методов: "
                         + QString::number(coveragedMethods)
                         +"\t\tВсего методов: "
                         + QString::number(totalsMethods));

        totalsMethods = 0;
        coveragedMethods = 0;
        countSwap = 0;

    }

}
