#include "population.h"


Population::Population() : mutationProbability (0.02), crossoverProbability (0.80), tournamentNum(3) { }

Population::Population(float mutation, float crossover)
    :mutationProbability (mutation), crossoverProbability (crossover), tournamentNum(3){

}

Population::~Population(){
//    currentListChromosome.clear();  //??
//        listMethodInfo.clear();
    for(auto ch : currentListChromosome)
        delete ch;

    for(auto m : listGeneralInfo)
        delete m;

    for(auto temp : tempListChromosome) //??
        delete temp;
}

QPair<AbstractChromosome *, AbstractChromosome *> Population::outbreeding()
{
    /* Выбор родителей.
     * Аутбридинг - 1ый родитель выбирается случайно, а 2ым выбирается такой,
     * который наименее похож на 1го.
     * Аутбридинг в фенотипной форме: похожесть измеряется в зависимости от значения
     * функции приспособленности (чем ближе значение целевой функции, тем особи более похожи)
     */

    int index1{rand()%(tempListChromosome.size()-1)};
    qDebug() << "index1 = "<<index1;
    AbstractChromosome* parent1 = tempListChromosome.at(index1);
    int maxDiff{std::numeric_limits<int>::min()};
    int index2{-1};

    for(int i{0}; i<tempListChromosome.size(); ++i){
        float diff{std::abs(tempListChromosome[i]->getFitness() - parent1->getFitness())};
        if(diff > maxDiff && diff != 0){
            maxDiff = diff;
            index2 = i;
        }
    }

    if(index2 == -1)
        return qMakePair(new AbstractChromosome(), new AbstractChromosome());

    qDebug() <<"index2 = "<<index2;
    AbstractChromosome* parent2 = tempListChromosome[index2];

    return qMakePair(parent1, parent2);
}

void Population::operatorCrossover()
{
    qDebug() << "\nРАБОТАЕТ ОПЕРАТОР КРОССОВЕР";
    QPair<AbstractChromosome*, AbstractChromosome*> parents = outbreeding();

    qDebug() << "fitness: parent1 = "<<parents.first->getFitness() << "parent2 = "<<parents.second->getFitness();
    if(parents.first->getFitness() == -1 && parents.second->getFitness() == -1){
        qDebug() << "Ошибка при выборе родителей";
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
        return;
    }

    //определяем, будем проводить скрещивание или нет
    float curProbCrossover= randomFloat();

    qDebug() << "\ncurProbCrossover = "<<curProbCrossover<<"\tcrossoverProbability = "<<crossoverProbability;
    if(curProbCrossover <= 1 && curProbCrossover >= 0 && curProbCrossover > crossoverProbability  ){
        qDebug() << "Нет кроссовера. curProbCrossover выше требуемой crossoverProbability";
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
    child1->setFitness(rand()%100);

    AbstractChromosome* child2 = new AbstractChromosome();
    child2->setGens(list2);
    child2->setFitness(rand()%100);

    newListChromosome.append(QList<AbstractChromosome*>() << child1 << child2);

    //очищаем промежуточный список
    tempListChromosome.clear();

    qDebug()<<"Потомки добавлены в промежуточную популяцию";
}

float Population::randomFloat(){
    srand(time(NULL));
    const float min{0.0};
    const float max{1.0};
    return (float)(rand() / (RAND_MAX + 1.0)) * (max - min) + min;
}

void Population::operatorMutation()
{
    qDebug() << "\nРАБОТАЕТ ОПЕРАТОР МУТАЦИИ";
    float curProbMutation = randomFloat();
//    float curProbMutation = 0.01;
    qDebug() << "curProbMutation = "<<curProbMutation<< "\tmutationProbability = "<<mutationProbability;
    if(curProbMutation <=1 && curProbMutation >= 0 && curProbMutation > mutationProbability){
        qDebug() << "Нет мутации. curProbMutation выше требуемой mutationProbability";
        return;
    }

    int index{rand()%(tempListChromosome.size()-1)};
    AbstractChromosome* chromosome = currentListChromosome.at(index);

    if(chromosome->getGens().size() == 1){
        qDebug() << "Нет мутации. Длина гена не позовляет использовать замену гена";
        return;
    }

    qDebug() <<"\nДо:\n"<<chromosome->getGens();
    chromosome->mutationGen();
    qDebug()<<"\nПосле\n"<<chromosome->getGens();
}

void Population::operatorSelection()
{
    /* Турнирный отбор.
     * Из популяции, содержищей N особей, выбираются случайным образом t особей, и
     * лучшая из них особь записывается в промежуточный массив.
     * Эта операция повторяется N раз.
     * Особи в полученном промежуточном массиве затем используются для скрещивания.
     */

    srand(time(NULL));
    for(int i{0}; i<currentListChromosome.size(); ++i){

        QList<AbstractChromosome*> temp;
        for(ushort i{0}; i < tournamentNum; ++i){
            AbstractChromosome* chro = currentListChromosome.at(rand() % (currentListChromosome.size() - 1));
            temp.append(chro);
        }

        auto itr = std::max_element(temp.begin(), temp.end(), [](AbstractChromosome* ch1, AbstractChromosome* ch2){ return ch1->getFitness() < ch2->getFitness();});

        //добавляем лучшую особь в промежуточный список
        tempListChromosome.append(*itr);
    }

//    for(auto ch : tempListChromosome)
//        qDebug() << ch->getFitness();

//    outbreeding();

}

void Population::operatorReduction()
{
    qDebug() << "\nРАБОТАЕТ ОПЕРАТОР ОТБОРА РОДИТЕЛЕЙ В НОВУЮ ПОПУЛЯЦИЮ";
    //элитарный отбор. конкурентный вид: выбираем лучшие
    for(auto ch : currentListChromosome)
        newListChromosome.append(ch);

    currentListChromosome.clear();
    std::sort(newListChromosome.begin(), newListChromosome.end(), [](AbstractChromosome* ch1, AbstractChromosome* ch2) { return ch1->getFitness() > ch2->getFitness(); });

//    qDebug() << "newListChromosome.size = "<<newListChromosome.size();
//    for(auto ch : newListChromosome)
//        qDebug() << ch->getFitness();

//    qDebug() << "currentListChromosome.size = "<<currentListChromosome.size();
    for(int i{0}; i < populationSize; ++i)
        currentListChromosome.append(newListChromosome.at(i));

//    qDebug() << "currentListChromosome.size = "<<currentListChromosome.size();
//    for(auto ch : currentListChromosome)
//        qDebug() << ch->getGens();
}

void Population::fitnessCalculation()
{
    qDebug() << "\nРАСЧЕТ ФУНКЦИИ ПРИГОДНОСТИ ДЛЯ КАЖДОЙ ОСОБИ В ПОПУЛЯЦИИ";
    for(int i{0}; i < populationSize; ++i)
        currentListChromosome.at(i)->fitnessCalculation();

//    for(auto ch : currentListChromosome)
//        qDebug() << ch->getFitness();
}

void Population::avgFitnessCalculation()
{
    qDebug() <<"\nРАСЧЕТ СРЕДНЕЙ ФУКЦНИИ ПРИГОДНОСТИ ДЛЯ ПОПУЛЯЦИИ";
    float avg{0.0};
    for(auto ch : currentListChromosome)
        avg += ch->getFitness();

    avgFitnessFunc = avg/currentListChromosome.size();
    qDebug() << "среднее значение функции пригодности = "<<avgFitnessFunc;

    if(fitnessFunction.size() < 6){
        fitnessFunction.append(avgFitnessFunc);
        return;
    }else{
        fitnessFunction.removeFirst();
        fitnessFunction.append(avgFitnessFunc);
    }

}

void Population::initPopulation()
{

//    if(populationSize)
//        return;


//    srand(time(0));
    JSONparser* parser = new JSONparser();
    parser->setFileName("text.json");
    listGeneralInfo = parser->parseJSON();

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
    //    qDebug() << "listChromosome.size = "<<currentListChromosome.size()<<"\tpopulationSize = "<<populationSize;

    delete parser;
}

int Population::random(const int min, const int max){
    return rand() % (max - min + 1) + min;
}

void Population::initMethodInfo(const int size)
{
    srand(time(NULL));
    for(int i{0}; i<size; ++i){
        int count = 0 + rand() % 5;
        int count1 = 0 + rand() %3;
        QStringList args, fields;
        for(int k{0}; k < count; ++k)
            args.append("args_"+QString::number(k));

        for(int j{0}; j < count1; ++j)
            fields.append("field_"+QString::number(j));

        listGeneralInfo.append(new GeneralInfo(ClassInfo("class_"+QString::number(i), fields),
                                             Method("method_"+QString::number(i), args, 1+rand()%50, true)));
    }

//    srand(time(NULL));
//    for(int i{0}; i<size; ++i){
//        int count1 = 0 + rand() %5;
//        QStringList args;
//        for(int k{0}; k < count1; ++k)
//            args.append("args_"+QString::number(k));

//        listMethodInfo.append(new MethodInfo(ClassInfo(),
//                                             Method("glob_method_"+QString::number(i), args, 1+rand()%50)));
//    }
}

void Population::printMethodInfo()
{
    qDebug() << "\nНАЧИНАЮ ПЕЧАТАТЬ ИНФУ О МЕТОДАХ";
    for(int i{0}; i < listGeneralInfo.size(); ++i)
        qDebug() << listGeneralInfo[i]->getClassInfo().getName() << " "
                 <<listGeneralInfo[i]->getClassInfo().getFields()<<" "
                <<listGeneralInfo[i]->getMethod().getName()<<" "
               <<listGeneralInfo[i]->getMethod().getArgs() << " "
              <<listGeneralInfo[i]->getMethod().getLinesCount();
}

void Population::printChromosome(){
    qDebug() << "\nНАЧИНАЮ ПЕЧАТАТЬ ХРОМОСОМЫ";
    qDebug() <<"\nКоличество хромосом = "<<currentListChromosome.size();
    for(int i{0}; i < currentListChromosome.size(); ++i)
        qDebug() << currentListChromosome[i]->getGens();
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


