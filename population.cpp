#include "population.h"


Population::Population()
    : mutationProbability (0.02),
      crossoverProbability (0.80),
      tournamentNum(3),
      numberIteration(0),
      totalIterations(0),
      currentIteration(0),
      index(0),
      filePath(""){ }

Population::Population(float mutation, float crossover)
    :mutationProbability (mutation),
      crossoverProbability (crossover),
      tournamentNum(3),
        numberIteration(0),
        totalIterations(0),
        currentIteration(0),
        index(0),
        filePath("")
{

}

Population::Population(int index, int totalIterations, const QString filePath)
    :mutationProbability (0.02),
    crossoverProbability (0.80),
    tournamentNum(3),
    numberIteration(0),
    totalIterations(totalIterations),
    currentIteration(0),
    index(index),
    filePath(filePath)
{

}

Population::~Population(){
//    currentListChromosome.clear();  //??
//        listMethodInfo.clear();
    for(int i{0}; i < currentListChromosome.size(); ++i){
        AbstractChromosome* abs = currentListChromosome.at(i);
        delete abs;
    }


    for(int i{0}; i < tempListChromosome.size(); ++i){
        AbstractChromosome* abs = tempListChromosome.at(i);
        delete abs;
    }
    currentListChromosome.clear();
    tempListChromosome.clear();
}

QPair<AbstractChromosome *, AbstractChromosome *> Population::outbreeding()
{
    qDebug() <<"ПОПУЛЯЦИЯ "<<QString::number(this->index)<< "РАБОТАЕТ АУТБРИДИНГ";
    writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tРаботает аутбридинг");
    /* Выбор родителей.
     * Аутбридинг - 1ый родитель выбирается случайно, а 2ым выбирается такой,
     * который наименее похож на 1го.
     * Аутбридинг в фенотипной форме: похожесть измеряется в зависимости от значения
     * функции приспособленности (чем ближе значение целевой функции, тем особи более похожи)
     */

    /*
     *  UDP. Аутбридинг в генотипной форме: похожесть измеряется в зависимости от представления
     *  генотипа
     */

//    srand(time(0));
    QMap<int, int> map;

    int index1{1+rand()%tempListChromosome.size()-1};
    qDebug() <<"ПОПУЛЯЦИЯ "<<QString::number(this->index)<<": index1 = "<<index1<<"\ttemp.size = "<<tempListChromosome.size();
    writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tНомер второго родителя = "+QString::number(index1));

    AbstractChromosome* parent1 = tempListChromosome.at(index1);
    int index2{-1};

    QStringList parent1Gens = parent1->getGens();
    qDebug() << tempListChromosome;
    for(int i{0}; i < tempListChromosome.size(); ++i){
        if(tempListChromosome.at(i) != parent1){

            AbstractChromosome* abs = tempListChromosome.at(i);
//            qDebug() << abs->getClassInfo().getName();
            QStringList absGens = abs->getGens();
            int differences{0}; //различия
            if(parent1Gens.size() >= absGens.size()){
                for(int k{0}; k < absGens.size(); ++k){
                    if(parent1Gens.at(k) != absGens.at(k))
                        ++differences;
                }
            }else if(parent1Gens.size() < absGens.size()){
                for(int k{0}; k < parent1Gens.size(); ++k){
                    if(parent1Gens.at(k) != absGens.at(k))
                        ++differences;
                }
            }
//            else{
//                for(int k{0}; k < parent1Gens.size(); ++k){
//                    if(parent1Gens.at(k) != absGens.at(k))
//                        ++differences;
//                }
//            }
            map.insert(i, differences);
        }
    }

    if(map.isEmpty()){
        qDebug() << "Во временной популяции все особи одинаковы.";
        writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tВо временной популяции все особи одинаковы. Родителей нет");
        return qMakePair(new AbstractChromosome(), new AbstractChromosome());
    }

    int max{std::numeric_limits<int>::min()};
    for(auto itr = map.begin(); itr != map.end(); ++itr){
        int diff = itr.value();
        if(diff >= max){
            max = diff;
            index2 = itr.key();
        }
    }

    if(index2 == -1){
        qDebug() <<"ПОПУЛЯЦИЯ "<<QString::number(this->index)<< " index2 = -1. Родителей нет.";
        writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tНомер 2го родителя = -1. Родителей нет");
        return qMakePair(new AbstractChromosome(), new AbstractChromosome());
    }

    qDebug() <<"ПОПУЛЯЦИЯ "<<QString::number(this->index)<< " 2index2 = "<<index2;
    writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tНомер 2го родителя = "+QString::number(index2));
    AbstractChromosome* parent2 = tempListChromosome.at(index2);

    qDebug() <<"ПОПУЛЯЦИЯ "<<QString::number(this->index)<< "КОНЕЦ РАБОТЫ АУТБРИДИНГА";
    writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tКОНЕЦ РАБОТЫ АУТБРИДИНГА");
    return qMakePair(parent1, parent2);
}

void Population::operatorCrossover()
{
    srand(time(0));
    qDebug() << "\nРАБОТАЕТ ОПЕРАТОР КРОССОВЕР";
    writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tРаботает оператор кроссовер");
    QPair<AbstractChromosome*, AbstractChromosome*> parents = outbreeding();

    qDebug() << "operatorCrossover: parent1 = "<<parents.first->getFitness() << "parent2 = "<<parents.second->getFitness();
    if(parents.first->getFitness() == -1 && parents.second->getFitness() == -1){
        qDebug() << "Ошибка при выборе родителей";
        writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tОшибка при выборе родителей");
        return;
    }

    //одноточечный кроссовер
    QList<QString> list1 = parents.first->getGens();
    QList<QString> list2 = parents.second->getGens();

    qDebug() << "list1.size = "<<list1.size();
    qDebug() << "list2.size = "<<list2.size()<<"\n\nДо:";
    qDebug()<<"list1: "<<list1;
    qDebug()<<"list2: "<<list2;

    //если один из родителей содержит один ген
    if(list1.size() == 1 || list2.size() == 1){
        qDebug() << "Нет кроссовера";
        writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tНет кроссовера. Один из родителей содержит только один ген");
        return;
    }

    //определяем, будем проводить скрещивание или нет
    float curProbCrossover= randomFloat();

    qDebug() << "\ncurProbCrossover = "<<curProbCrossover<<"\tcrossoverProbability = "<<crossoverProbability;
    if(curProbCrossover <= 1 && curProbCrossover >= 0 && curProbCrossover > crossoverProbability  ){
        qDebug() << "Нет кроссовера. curProbCrossover выше требуемой crossoverProbability";
        writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tНет кроссовера. Сгенерированное число выше вероятности кроссовера");
        return;
    }

    //точка разрыва - середина хромосомы. Обмен происходит вторыми частями
    if(list1.size() < list2.size()){
        qDebug() <<"\nlist1.size < list2.size";

        // list1 имеет меньший размер, поэтому мы его дополняем до list2
        int size1 = list1.size();
        int size2 = list2.size();
        int diff = std::abs(size1-size2);
        for(int i{0}; i < diff; ++i)
            list1.push_back("");

        std::swap_ranges(list2.begin() + list2.size()/2, list2.end(),  list1.begin() + list1.size()/2);

        //теперь уменьшаем размер list2 до старого list1
        for(int i{0}; i < diff; ++i)
            list2.pop_back();
    }else{
        qDebug() <<"\nlist1.size > list2.size";

        // list2 имеет меньший размер, поэтому мы его дополняем до list1
        int size1 = list1.size();
        int size2 = list2.size();
        int diff = std::abs(size1-size2);
        for(int i{0}; i < diff; ++i)
            list2.push_back("");

        std::swap_ranges(list1.begin() + list1.size()/2, list1.end(),  list2.begin() + list2.size()/2);

        //теперь уменьшаем размер list1 до старого list2
        for(int i{0}; i < diff; ++i)
            list1.pop_back();
    }

    qDebug() << "\nlist1.size = "<<list1.size();
    qDebug() << "list2.size = "<<list2.size();
    qDebug()<<"После\nlist1: "<<list1;
    qDebug()<<"list2: "<<list2;

    //добавляем потомков в промежуточную популяцию
    AbstractChromosome* child1 = new AbstractChromosome();
    child1->setGens(list1);
    child1->setMethod(parents.first->getMethod());
    child1->setClassInfo(parents.first->getClassInfo());
    child1->fitnessCalculation();

    AbstractChromosome* child2 = new AbstractChromosome();
    child2->setGens(list2);
    child2->setMethod(parents.second->getMethod());
    child2->setClassInfo(parents.second->getClassInfo());
    child2->fitnessCalculation();

    newListChromosome.append(QList<AbstractChromosome*>() << child1 << child2);

    //очищаем промежуточный список
//    tempListChromosome.clear();

    qDebug()<<"Потомки добавлены в промежуточную популяцию";
    writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tПотомки добавлены в промежуточную популяцию");
    writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tКонец работы оператора кроссовера");
}

float Population::randomFloat(){
    srand(time(NULL));
    const float min{0.0};
    const float max{1.0};
    return (float)(rand() / (RAND_MAX + 1.0)) * (max - min) + min;
}

void Population::writeMessage(const QString& message)
{
    emit signalWriteMessage(index, message);
}

void Population::operatorMutation()
{
    srand(time(0));
    qDebug() << "\nРАБОТАЕТ ОПЕРАТОР МУТАЦИИ";
    writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tРаботает оператор мутации");
    float curProbMutation = randomFloat();
//    float curProbMutation = 0.01;
    qDebug() << "curProbMutation = "<<curProbMutation<< "\tmutationProbability = "<<mutationProbability;
    if(curProbMutation <=1 && curProbMutation >= 0 && curProbMutation > mutationProbability){
        qDebug() << "Нет мутации. curProbMutation выше требуемой mutationProbability";
        writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tНет мутации. Сгенерированое число выше вероятности мутации");
        return;
    }

    int index{1+rand()%tempListChromosome.size()-1};
    if(index < 0){
        qDebug() << "Нет мутации. Индекс = -1";
        writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tНет мутации. Индекс гена равен -1");
        return;
    }
    AbstractChromosome* chromosome = currentListChromosome.at(index);

    if(chromosome->getGens().size() == 1){
        qDebug() << "Нет мутации. Длина хромосомы не позовляет использовать замену гена";
        writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tНет мутации. Длина хромосомы не позволяет использовать замену гена");
        return;
    }

    qDebug() <<"\nДо:\n"<<chromosome->getGens();
    chromosome->mutationGen();
    qDebug()<<"\nПосле\n"<<chromosome->getGens();

    writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tКонец работы оператора мутации");
}

void Population::operatorSelection()
{
    /* Турнирный отбор.
     * Из популяции, содержищей N особей, выбираются случайным образом t особей, и
     * лучшая из них особь записывается в промежуточный массив.
     * Эта операция повторяется N раз.
     * Особи в полученном промежуточном массиве затем используются для скрещивания.
     */
    qDebug() << "РАБОТАЕТ ОТБОР РОДИТЕЛЕЙ ДЛЯ СЕЛЕКЦИИ";
    writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tРаботает отбор родителей для селекции");
    tempListChromosome.clear();
    srand(time(0));
    for(int i{0}; i<currentListChromosome.size(); ++i){

        QList<AbstractChromosome*> temp;
        for(ushort i{0}; i < tournamentNum; ++i){
            int ind = 1+rand() % currentListChromosome.size()-1;
            AbstractChromosome* chro = currentListChromosome.at(ind);
            temp.append(chro);
        }

        if(temp.at(0) == temp.at(1) && temp.at(1) == temp.at(2) && temp.at(2) == temp.at(0))
            qDebug() << "все особи равны";

        //выбираем наилучшую особь
        auto itr = std::max_element(temp.begin(), temp.end(), [](AbstractChromosome* ch1, AbstractChromosome* ch2){ return ch1->getFitness() < ch2->getFitness();});

        qDebug() << temp;
        int ind{0};
        if(temp.at(0)->getFitness() == temp.at(1)->getFitness() &&
                temp.at(1)->getFitness() == temp.at(2)->getFitness() &&
                temp.at(2)->getFitness() == temp.at(0)->getFitness()){


            ind = 0+rand()%3;
//            qDebug() << "3 отобранных особи в турнирном методе имеют одинаковое значение финтнесс-функции. Выбираем наугад "<<ind;
            itr = temp.begin()+ind;
        }


        //добавляем лучшую особь в промежуточный список
        tempListChromosome.append(*itr);
    }
    writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tКонец работы отбора родителей для селекции");
}

void Population::operatorReduction()
{
    qDebug() << "\nРАБОТАЕТ ОПЕРАТОР ОТБОРА РОДИТЕЛЕЙ В НОВУЮ ПОПУЛЯЦИЮ";
    writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tРаботает оператор отбора родителей в новую популяцию");
    //элитарный отбор. конкурентный вид: выбираем лучшие
    for(auto ch : currentListChromosome)
        newListChromosome.append(ch);

    currentListChromosome.clear();
    std::sort(newListChromosome.begin(), newListChromosome.end(), [](AbstractChromosome* ch1, AbstractChromosome* ch2) { return ch1->getFitness() > ch2->getFitness(); });

    for(int i{0}; i < populationSize; ++i)
        currentListChromosome.append(newListChromosome.at(i));

    int diff = std::abs(newListChromosome.size() - currentListChromosome.size());

    for(int i{newListChromosome.size()-1}; i < diff; ++i ){
        newListChromosome.removeLast();
    }

    newListChromosome.clear();
    writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tКонец работы оператора отбора родителей в новую популяцию");
}

void Population::fitnessCalculation()
{
    qDebug() << "\nРАСЧЕТ ФУНКЦИИ ПРИГОДНОСТИ ДЛЯ КАЖДОЙ ОСОБИ В ПОПУЛЯЦИИ";
    writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tРасчет функции пригодности для каждой особи в популяции");
    for(int i{0}; i < populationSize; ++i)
        currentListChromosome.at(i)->fitnessCalculation();

    writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tФункция пригодности для каждой особи в популяции расчитана");
}

void Population::avgFitnessCalculation()
{
    qDebug() <<"\nРАСЧЕТ СРЕДНЕЙ ФУНКЦИИ ПРИГОДНОСТИ ДЛЯ ПОПУЛЯЦИИ";
    writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tРасчет средней функции пригодности для популяции");
    float avg{0.0};
    for(auto ch : currentListChromosome)
        avg += ch->getFitness();

    avgFitnessFunc = avg/currentListChromosome.size();
    qDebug() << "среднее значение функции пригодности = "<<avgFitnessFunc;
    writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tСреднее значение функции пригодности = "+QString::number(avgFitnessFunc));

}

void Population::initPopulation()
{

    JSONparser* parser = new JSONparser();
    parser->setFileName(filePath);
    QList<GeneralInfo*> listGeneralInfo = parser->parseJSONPopulation();
//    qDebug() << "listGeneralInfo = "<<listGeneralInfo.size();
    if(listGeneralInfo.size() == 0)
        writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tНе удалось прочитать файл или файл пуст");

    writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tИнициализация популяции...");
    for(int k{0}; k < listGeneralInfo.size(); ++k){


        GeneralInfo* info = listGeneralInfo[k];
        AbstractChromosome* chrom;

        if(info->getMethod().getReturnValue() == true)
            chrom = new ChromosomeReturningFunc();
        else
            chrom = new ChromosomeNonReturningFunc();

        chrom->setFitness(0);
        chrom->setGlobalFields(info->getGbobalFields());
        chrom->setOtherClasses(info->getOtherClasses());
        chrom->setChromosome(info->getClassInfo(),
                             info->getMethod());

//        chrom->setFitness(rand()%100);

        currentListChromosome.append(chrom);

    }

    populationSize = listGeneralInfo.size();

    totalMethods = listGeneralInfo.size();
    delete parser;

    writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tПопуляция инициализирована...");
    for(auto ch : currentListChromosome)
        qDebug() << ch->getGens();
}

int Population::random(const int min, const int max){
    return rand() % (max - min + 1) + min;
}

void Population::start(){

    if(currentIteration == 0)
        initPopulation();

    //каждый 3 итерации популяция должна обменяться хромосомами с соседней популяцией
    while(numberIteration <= 3 && currentIteration <= totalIterations){
        writeMessage("\n"+QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tИТЕРАЦИЯ №"+QString::number(currentIteration));
        fitnessCalculation();
        operatorSelection();
        operatorCrossover();
        operatorMutation();
        operatorReduction();
//        fitnessCalculation();
        ++numberIteration;
        ++currentIteration;
    }
    qDebug() << "Популяция "<<QString::number(index)<<" готова к обмену хромосомами";
    writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tПопуляция "+QString::number(index)+" готова к обмену хромосомами");
    numberIteration = 0;

    if(currentIteration > totalIterations){
        checkCoveragedMethods();
        emit signalFinish();
    }else
        emit signalReadySwap();
}

void Population::checkCoveragedMethods(){
    QMap<QString, int> map;
    for(auto ch : currentListChromosome){
        QString clName = ch->getClassInfo().getName();
        QString mdName = ch->getMethod().getName();
        int mdArgs = ch->getClassInfo().getFields().size();

        QString method{""};
        QString obj{""};

        if(!clName.isEmpty())
            obj = clName+".";

        method = obj+mdName+"-"+QString::number(mdArgs);

        if(!map.contains(method))
            map.insert(method, 1);

        else
            map[method]++;
    }
    qDebug() << "map.size = " << map.size();
    qDebug() << map;

    qDebug() << totalMethods;
    emit signalCoveragedMethods(map.size());
    emit signalTotalMethods(totalMethods);
    writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tПокрытых методов: "+ QString::number(map.size()));
    writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tВсего методов: "+ QString::number(map.size()));
}

void Population::printChromosome(){
    qDebug() << "\nНАЧИНАЮ ПЕЧАТАТЬ ХРОМОСОМЫ";
//    writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"НАЧИНАЮ ПЕЧАТАТЬ ХРОМОСОМЫ");
    qDebug() <<"\nКоличество хромосом = "<<currentListChromosome.size();
    for(int i{0}; i < currentListChromosome.size(); ++i)
        qDebug() << currentListChromosome[i]->getGens();
}

void Population::receiveChromosomes(QList<AbstractChromosome *> list)
{
    writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tПолучение особей от другой популяции");
    for(auto ch : list)
        currentListChromosome.append(ch);
}

QList<AbstractChromosome *> Population::sendChromosomes(const int count)
{
    writeMessage(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tОтправление особей другой популяции");
    srand(time(0));
    QList<AbstractChromosome *> list;
    for(int i{0}; i < count; ++i){
        //выбираем любую хромосому
        int num = 1+rand()%currentListChromosome.size()-1;

        //забираем ее из этой популяции
        list.append(currentListChromosome.takeAt(num));
    }
    return list;
}

bool Population::checkStopCondition(){

    //вычисляем разницу между соседними элементами

    qDebug() << "\nПРОВЕРЯЕМ УСЛОВИЕ ОСТАНОВА";
    bool result;
    QList<float> diff;
    for(int i{0}; i < fitnessFunction.size()-1; ++i)
        diff.append(fitnessFunction.at(i) - fitnessFunction.at(i+1));

    float current{0.0};
    float next{0.0};
    for(int i{0}; i < diff.size()-1; ++i){
        current = diff.at(i);
        next = diff.at(i+1);
        result = (current < next) ? true : false;
    }

    return result;
}


