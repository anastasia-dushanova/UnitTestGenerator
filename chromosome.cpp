#include "chromosome.h"

Chromosome::Chromosome() : fitness(-1) { }

//Chromosome::Chromosome(QString method, QList<QString> types)
//    : fitness(0), types(types), method(method) {


//}

/* тест-кейс, при которой тестируется функция, возвращающая что-то, имеет вид:
 * types1 = 5
 * types2 = 7
 * assert method(types1,types2) == 12
 */

/*
 * тест-кейс, при которой тестируется функция, ничего невозвращающая, имеет вид:
 *  # Инициализация необходимых переменных
    expected_state = 'initial state'
    actual_state = expected_state

    # Вызов тестируемой функции
    func_with_side_effect()

    # Проверка изменения состояния
    assert actual_state == 'expected state'
 *
 * при тестировании такого вида функции, проверяется изменение состояний переменных
 */


void Chromosome::setChromosome(QString method, size_t args, int type, bool returnValue){

    srand(time(NULL));


    if(returnValue){
        QString listArgs{""};
        QPair<QString, QString> pair;
        if(args == 0){
            pair = generateRandom(type);
            gens.append("assert "+method+"("+listArgs+") == "+pair.second);
            return;
        }

        for(size_t i{0}; i < args; ++i){

            pair = generateRandom(type);
            gens.append("args_"+QString::number(i)+" = " + pair.first);
            listArgs += "args_"+QString::number(i)+",";
        }
        listArgs.chop(1);
        gens.append("assert "+method+"("+listArgs+") == "+pair.second);
    }
}

void Chromosome::mutationGen()
{

    //генерируем тип аргумента
    QPair<QString, QString> pair = generateRandom(1+rand()%3);

    //определяем позицию для замены
    int index = 1 + rand()%gens.size()-1;
    qDebug() <<"\nПозиция для замены: "<<index;
    if(index == gens.size()-1){
        qDebug() << "Не буду менять последний ген!";
        return;
    }

    //TODO проверку сделать на out of range
    gens[index] = "args_"+QString::number(randomInt())+" = " + pair.first;
}

int Chromosome::randomInt(){
//    srand(time(0));
    const int min{0};
    const int max{100};
    return rand() % (max - min + 1) + min;
}

float Chromosome::randomFloat(){
//    srand(time(0));
    const float min{0.0};
    const float max{100.0};
    return (float)(rand() / (RAND_MAX + 1.0)) * (max - min) + min;
}

QString Chromosome::randomString(){

    const int length{10};
    static const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, chars.size() - 1);
    std::vector<char> string(length);
    for (int i = 0; i < length; ++i) {
        string[i] = chars[dis(gen)];
    }
    return QString::fromStdString(std::string(string.begin(), string.end()));
}

QPair<QString, QString> Chromosome::generateRandom(const int type)
{
    QString value{""}, result{""};
    switch(type){
    case static_cast<int>(types::typesInt) :{
        value = QString::number(randomInt());
        result = QString::number(randomInt());
        break;
    }
    case static_cast<int>(types::typesFloat) :{
        value = QString::number(randomFloat());
        result = QString::number(randomFloat());
        break;
    }
    case static_cast<int>(types::typesStr) :{
        value = randomString();
        result = randomString();
        break;
    }
    default:{
        value= "NULL";
        result = "NULL";
        break;
    }
    }
    return qMakePair(value, result);
}

