#include "chromosome.h"

AbstractChromosome::AbstractChromosome()
    : classInfo("", QStringList()),method(), fitness(-1), gens(), otherClasses(), globalFields() { }

AbstractChromosome::AbstractChromosome(ClassInfo classInfo, Method method)
    : classInfo(classInfo), method(method), fitness(-1), gens(), otherClasses(), globalFields() { }

void AbstractChromosome::setChromosome(ClassInfo classInfo, Method method)
{
    classInfo = ClassInfo("", QStringList());
    method = Method();
}

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

void AbstractChromosome::mutationGen()
{

//    //генерируем тип аргумента
//    QPair<QString, QString> pair = generateRandom(1+rand()%3);

//    //определяем позицию для замены
//    int index = 1 + rand()%gens.size()-1;
//    qDebug() <<"\nПозиция для замены: "<<index;
//    if(index == gens.size()-1){
//        qDebug() << "Не буду менять последний ген!";
//        return;
//    }

//    //TODO проверку сделать на out of range
//    gens[index] = "args_"+QString::number(randomInt())+" = " + pair.first;
}

int AbstractChromosome::randomInt(){
//    srand(time(0));
    const int min{0};
    const int max{100};
    return rand() % (max - min + 1) + min;
}

float AbstractChromosome::randomFloat(){
//    srand(time(0));
    const float min{0.0};
    const float max{100.0};
    return (float)(rand() / (RAND_MAX + 1.0)) * (max - min) + min;
}

QString AbstractChromosome::randomString(){

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

QPair<QString, QString> AbstractChromosome::generateRandom(const int type)
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
    case static_cast<int>(types::typeList) :{
        QString content{"[ "};

        //добавляем базовые типы
        for(int i{1}; i < 3; ++i){
            QPair<QString, QString> pair = generateRandom(i);
            content.append(pair.first + ", ");
        }

        //если нет дополнительных типов
//        if(otherClasses.size() == 0){
//            content.chop(2);
//            content.append(" ]");
//            return qMakePair(content, QString());
//        }

        QString classObject{""};

        //добавляем объекты классов
        for(int i{0}; i < otherClasses.size(); ++i){
             classObject = "obj_"+QString::number(i);
             content.append(classObject+", ");
        }

        //добавляем глобальные переменные
        for(int i{0}; i < globalFields.size(); ++i){
            content.append(globalFields.at(i)+", ");
        }

        content.chop(2);
        content.append(" ]");
        return qMakePair(content, QString());
    }
    //TODO сделать генерацию для словаря
    default:{
        value= "NULL";
        result = "NULL";
        break;
    }
    }
    return qMakePair(value, result);
}

void AbstractChromosome::fitnessCalculation(){


}

ChromosomeReturningFunc::ChromosomeReturningFunc()
    : AbstractChromosome(){ }

void ChromosomeReturningFunc::setChromosome(ClassInfo classInfo, Method method){

    setClassInfo(classInfo);
    setMethod(method);
    srand(time(0));
    QStringList gens{};
    QString listArgs{""};
    QString result{""};
    QString field{""};
    QPair<QString, QString> pair;

    //метод из глобальной области
    if(classInfo.getName().isEmpty()){

        //если метод не содержит аргументов
        if(method.getArgs().size() == 0){

            srand(time(0));
            //если есть глобальные переменные
            if(getGlobalFields().size() != 0){

                //если есть дополнительные классы
                if(getOtherClasses().size() != 0)
                    for(int i{0}; i < getOtherClasses().size(); ++i)
                        gens.append("obj_"+QString::number(i)+" = "+getOtherClasses().at(i)+"()");


                int type = 1+rand()%4;
                pair = generateRandom(type);
                result = (type >= 4) ? pair.first : pair.second;
                gens.append("assert "+method.getName()+"("+listArgs+") == "+result);
            }else{

                //если есть дополнительные классы
                if(getOtherClasses().size() != 0){
                    for(int i{0}; i < getOtherClasses().size(); ++i)
                        gens.append("obj_"+QString::number(i)+" = "+getOtherClasses().at(i)+"()");

                    int type = 1+rand()%4;
                    pair = generateRandom(type);
                    result = (type >= 4) ? pair.first : pair.second;
                    gens.append("assert "+method.getName()+"("+listArgs+") == "+result);
                }else{
                    int type = 1+rand()%4;
                    pair = generateRandom(type);
                    result = (type >= 4) ? pair.first : pair.second;
                    gens.append("assert "+method.getName()+"("+listArgs+") == "+result);
                }
            }
        }else{
            //метод содержит аргументы
            for(int i{0}; i < method.getArgs().size(); ++i){

                int type = 1+rand()%4;
                pair = generateRandom(type);
                result = (type >= 4) ? pair.first : pair.second;
                gens.append("args_"+QString::number(i)+" = " + result);
                listArgs += "args_"+QString::number(i)+",";
            }

            listArgs.chop(1);
            //если есть глобальные переменные
            if(getGlobalFields().size() != 0){

                //если есть дополнительные классы
                if(getOtherClasses().size() != 0)
                    for(int i{0}; i < getOtherClasses().size(); ++i)
                        gens.append("obj_"+QString::number(i)+" = "+getOtherClasses().at(i)+"()");

                int type = 1+rand()%4;
                pair = generateRandom(type);
                result = (type >= 4) ? pair.first : pair.second;
                gens.append("assert "+method.getName()+"("+listArgs+") == "+result);
            }else{

                //если есть дополнительные классы
                if(getOtherClasses().size() != 0){
                    for(int i{0}; i < getOtherClasses().size(); ++i)
                        gens.prepend("obj_"+QString::number(i)+" = "+getOtherClasses().at(i)+"()");

                    int type = 1+rand()%4;
                    pair = generateRandom(type);
                    result = (type >= 4) ? pair.first : pair.second;
                    gens.append("assert "+method.getName()+"("+listArgs+") == "+result);
                }else{
                    int type = 1+rand()%4;
                    pair = generateRandom(type);
                    result = (type >= 4) ? pair.first : pair.second;
                    gens.append("assert "+method.getName()+"("+listArgs+") == "+result);
                }
            }
        }
    }else{
        //Метод класса
        srand(time(0));
        QString obj{"object_"+classInfo.getName()};
        gens.append(obj+" = "+classInfo.getName()+"()");

        //если метод не содержит аргументов
        if(method.getArgs().size() == 0){

            //если класс не содержит полей
            if(classInfo.getFields().size() == 0){

                //если есть глобальные переменные
                if(getGlobalFields().size() != 0){

                    //если есть дополнительные классы
                    if(getOtherClasses().size() != 0)
                        for(int i{0}; i < getOtherClasses().size(); ++i)
                            gens.append("obj_"+QString::number(i)+" = "+getOtherClasses().at(i)+"()");


                    int type = 1+rand()%4;
                    pair = generateRandom(type);
                    result = (type >= 4) ? pair.first : pair.second;
                    gens.append("assert "+obj+"."+method.getName()+"("+listArgs+") == "+result);
                }else{

                    //если есть дополнительные классы
                    if(getOtherClasses().size() != 0){
                        for(int i{0}; i < getOtherClasses().size(); ++i)
                            gens.prepend("obj_"+QString::number(i)+" = "+getOtherClasses().at(i)+"()");

                        int type = 1+rand()%4;
                        pair = generateRandom(type);
                        result = (type >= 4) ? pair.first : pair.second;
                        gens.append("assert "+obj+"."+method.getName()+"("+listArgs+") == "+result);
                    }else{
                        int type = 1+rand()%4;
                        pair = generateRandom(type);
                        result = (type >= 4) ? pair.first : pair.second;
                        gens.append("assert "+obj+"."+method.getName()+"("+listArgs+") == "+result);
                    }
                }

//                gens.insert(gens.size()-1, obj+"."+method.getName()+"()");
            }else{
                //Класс содержит поля
                //если есть глобальные переменные
                if(getGlobalFields().size() != 0){

                    //если есть дополнительные классы
                    if(getOtherClasses().size() != 0)
                        for(int i{0}; i < getOtherClasses().size(); ++i)
                            gens.append("obj_"+QString::number(i)+" = "+getOtherClasses().at(i)+"()");

                    int type = 1+rand()%4;
                    pair = generateRandom(type);
                    result = (type >= 4) ? pair.first : pair.second;
                    gens.append("assert "+obj+"."+method.getName()+"("+listArgs+") == "+result);
                }else{
                    //нет глобальных переменных
                    //если есть дополнительные классы
                    if(getOtherClasses().size() != 0){
                        for(int i{0}; i < getOtherClasses().size(); ++i)
                            gens.prepend("obj_"+QString::number(i)+" = "+getOtherClasses().at(i)+"()");

                        int type = 1+rand()%4;
                        pair = generateRandom(type);
                        result = (type >= 4) ? pair.first : pair.second;
                        gens.append("assert "+obj+"."+method.getName()+"("+listArgs+") == "+result);
                    }else{
                        //нет дополнительных классов
                        int type = 1+rand()%4;
                        pair = generateRandom(type);
                        result = (type >= 4) ? pair.first : pair.second;
                        gens.append("assert "+obj+"."+method.getName()+"("+listArgs+") == "+result);
                    }
                }
//                gens.insert(gens.size()-1, obj+"."+method.getName()+"()");

            }

        }else{
            //метод имеет аргументы
            for(int i{0}; i < method.getArgs().size(); ++i){

                int type = 1+rand()%4;
                pair = generateRandom(type);
                result = (type >= 4) ? pair.first : pair.second;
                gens.append("args_"+QString::number(i)+" = " + result);
                listArgs += "args_"+QString::number(i)+",";
            }
            listArgs.chop(1);
            //если класс не содержит полей
            if(classInfo.getFields().size() == 0){

                //если есть глобальные переменные
                if(getGlobalFields().size() != 0){

                    //если есть дополнительные классы
                    if(getOtherClasses().size() != 0)
                        for(int i{0}; i < getOtherClasses().size(); ++i)
                            gens.append("obj_"+QString::number(i)+" = "+getOtherClasses().at(i)+"()");


                    int type = 1+rand()%4;
                    pair = generateRandom(type);
                    result = (type >= 4) ? pair.first : pair.second;
                    gens.append("assert "+obj+"."+method.getName()+"("+listArgs+") == "+result);
                }else{

                    //если есть дополнительные классы
                    if(getOtherClasses().size() != 0){
                        for(int i{0}; i < getOtherClasses().size(); ++i)
                            gens.prepend("obj_"+QString::number(i)+" = "+getOtherClasses().at(i)+"()");

                        int type = 1+rand()%4;
                        pair = generateRandom(type);
                        result = (type >= 4) ? pair.first : pair.second;
                        gens.append("assert "+obj+"."+method.getName()+"("+listArgs+") == "+result);
                    }else{
                        //нет дополнительных классов
                        int type = 1+rand()%4;
                        pair = generateRandom(type);
                        result = (type >= 4) ? pair.first : pair.second;
                        gens.append("assert "+obj+"."+method.getName()+"("+listArgs+") == "+result);
                    }
                }

            }else{
                //Класс содержит поля
                //если есть глобальные переменные
                if(getGlobalFields().size() != 0){

                    //если есть дополнительные классы
                    if(getOtherClasses().size() != 0)
                        for(int i{0}; i < getOtherClasses().size(); ++i)
                            gens.append("obj_"+QString::number(i)+" = "+getOtherClasses().at(i)+"()");

                    int type = 1+rand()%4;
                    pair = generateRandom(type);
                    result = (type >= 4) ? pair.first : pair.second;
                    gens.append("assert "+obj+"."+method.getName()+"("+listArgs+") == "+result);
                }else{
                    //если есть дополнительные классы
                    if(getOtherClasses().size() != 0){
                        for(int i{0}; i < getOtherClasses().size(); ++i)
                            gens.prepend("obj_"+QString::number(i)+" = "+getOtherClasses().at(i)+"()");

                        int type = 1+rand()%4;
                        pair = generateRandom(type);
                        result = (type >= 4) ? pair.first : pair.second;
                        gens.append("assert "+obj+"."+method.getName()+"("+listArgs+") == "+result);
                    }else{
                        //нет дополнительных классов
                        int type = 1+rand()%4;
                        pair = generateRandom(type);
                        result = (type >= 4) ? pair.first : pair.second;
                        gens.append("assert "+obj+"."+method.getName()+"("+listArgs+") == "+result);
                    }
                }

            }

        }

    }
    setGens(gens);

}

void ChromosomeReturningFunc::fitnessCalculation(){


    //текущее количестов генов
    QStringList currentGens = getGens();

    //мы должны посчитать количество выражений, и сравнить с количеством аргументов функции
    int currentArgs = currentGens.size() - 1;

    //если кол-во аргументов в текущем тест-кейсе меньше с кол-вом аргументов функции
    //фитнесс пригодность равно нулю
    qDebug() << "currentArgs = "<<currentArgs<<"\tmethod args = "<<getMethod().getArgs().size();
    if(currentArgs < getMethod().getArgs().size()){
        setFitness(0);
//        qDebug() << 0;
        return;
    }

    float fit{0.0};
    fit = static_cast<float>(1/1 + getMethod().getLinesCount()/getMethod().getLinesCount());
    setFitness(fit);

    //    qDebug() << fit;
}

void ChromosomeReturningFunc::mutationGen()
{
    //TODO оптимизировать
    QStringList newGens = getGens();

    srand(time(0));
    //генерируем тип аргумента
    QPair<QString, QString> pair = generateRandom(1+rand()%4);

    //определяем позицию для замены
    int index = 1 + rand()%newGens.size()-1;
    qDebug() <<"\nПозиция для замены: "<<index;
    if(index == newGens.size()-1){
        qDebug() << "Не буду менять последний ген!";
        return;
    }
    QString argName = newGens.at(index);
    QString newGen{""};
    if(argName.contains("args_"))
        newGen = argName.split(" = ").at(0) + " = " + pair.first;

    else if(argName.contains("expected"))
        newGen = argName.split(" = ").at(0) + " = " + pair.first;

    else if(argName.contains("field"))
        newGen = argName.split(" = ").at(0) + " = " + pair.first;

    else if(argName.contains("global"))
        newGen = argName.split(" = ").at(0) + " = " + pair.first;

    else
        newGen = "args_0 = " + pair.first;

    //TODO проверку сделать на out of range
    QStringList list = getGens();
    list[index] = newGen;
    setGens(list);
}

ChromosomeNonReturningFunc::ChromosomeNonReturningFunc()
    : AbstractChromosome() {}

void ChromosomeNonReturningFunc::fitnessCalculation(){
    //если функция из глобальной области и нет глобальных переменных, то
    //функция пригодности равна нулю
    if(getGens().at(getGens().size()-1).contains("assert") && getGens().size() < 2){
        setFitness(0);
        return;
    }

    //текущее количестов генов
    QStringList currentGens = getGens();

    //выкидываем assert
    if(currentGens.size() > 1)
        currentGens.removeLast();

    //выкидываем вызов функции
    if(currentGens.size() > 1)
        currentGens.removeLast();

    int currentArgs = currentGens.size()-1; //выкидываем exptected
    if(currentArgs < getMethod().getArgs().size()){
        setFitness(0);
        return;
    }

    float fit{0.0};
    fit = static_cast<float>(1/1 + getMethod().getLinesCount()/getMethod().getLinesCount());
    setFitness(fit);

}

void ChromosomeNonReturningFunc::mutationGen()
{
    //если функция из глобальной области и нет глобальных переменных
    if(getGens().at(getGens().size()-1).contains("assert") && getGens().size() < 2){
        qDebug() <<"Не буду делать мутацию функции из глобальной области и без глобальных переменных";
        return;
    }

    QStringList newGens = getGens();

    //выкидываем assert
    if(newGens.size() > 1)
        newGens.removeLast();

    //выкидываем вызов функции
    if(newGens.size() > 1)
        newGens.removeLast();

    srand(time(0));
    int index = 1 + rand()%newGens.size()-1;
    QPair<QString, QString> pair = generateRandom(1+rand()%4);

    QString argName = newGens.at(index);
    QString newGen{""};
    if(argName.contains("args_"))
        newGen = argName.split(" = ").at(0) + " = " + pair.first;

    else if(argName.contains("expected"))
        newGen = argName.split(" = ").at(0) + " = " + pair.first;

    else if(argName.contains("object_class"))
        newGen = argName.split(" = ").at(0) + " = " + pair.first;

    else if(argName.contains("global"))
        newGen = argName.split(" = ").at(0) + " = " + pair.first;

    else
        newGen = "args_0 = " + pair.first;

    //TODO проверку сделать на out of range
    QStringList list = getGens();
    list[index] = newGen;
    setGens(list);
}

void ChromosomeNonReturningFunc::setChromosome(ClassInfo classInfo, Method method){

    QStringList gens{};
    QPair<QString, QString> pair;
    QString field{""};
    QString result{""};
    QString listArgs{""};
    setClassInfo(classInfo);
    setMethod(method);
    //метод из глобальной области
    if(classInfo.getName().isEmpty()){

        srand(time(0));
        //если метод не содержит аргументов
        if(method.getArgs().size() == 0){

            //если есть глобальные переменные
            if(getGlobalFields().size() != 0){

                //если есть дополнительные классы
                if(getOtherClasses().size() != 0)
                    for(int i{0}; i < getOtherClasses().size(); ++i)
                        gens.append("obj_"+QString::number(i)+" = "+getOtherClasses().at(i)+"()");


                int type = 1+rand()%4;
                pair = generateRandom(type);
                field = getGlobalFields().at(1 + rand()%getGlobalFields().size()-1);
                gens.append("expected = "+pair.first);
                gens.append(field + " = "+pair.first);
                gens.append("assert "+field+" == expected");
            }else{

                //если есть дополнительные классы
                if(getOtherClasses().size() != 0){
                    for(int i{0}; i < getOtherClasses().size(); ++i)
                        gens.append("obj_"+QString::number(i)+" = "+getOtherClasses().at(i)+"()");

                    int type = 1+rand()%4;
                    pair = generateRandom(type);
                    field = gens.at(1 + rand()%getOtherClasses().size()-1).split(" = ").at(0);
                    gens.append("expected = "+pair.first);
                    gens.append(field + " = "+pair.first);
                    gens.append("assert "+field+" == expected");
                }else{
                    int type = 1+rand()%4;
                    pair = generateRandom(type);
                    gens.append("expected = "+pair.first);
                    gens.append(method.getName()+"()");
                    setGens(gens);
                    return;
                }
            }
            gens.insert(gens.size()-1, method.getName()+"()");

        }else{
            //метод содержит аргументы
            for(int i{0}; i < method.getArgs().size(); ++i){

                int type = 1+rand()%4;
                pair = generateRandom(type);
                result = (type >= 4) ? pair.first : pair.second;
                gens.append("args_"+QString::number(i)+" = " + result);
                listArgs += "args_"+QString::number(i)+",";
            }

            listArgs.chop(1);
            //если есть глобальные переменные
            if(getGlobalFields().size() != 0){

                //если есть дополнительные классы
                if(getOtherClasses().size() != 0)
                    for(int i{0}; i < getOtherClasses().size(); ++i)
                        gens.append("obj_"+QString::number(i)+" = "+getOtherClasses().at(i)+"()");


                int type = 1+rand()%4;
                pair = generateRandom(type);
                field = getGlobalFields().at(1 + rand()%getGlobalFields().size()-1);
                gens.append("expected = "+pair.first);
                gens.append(field + " = "+pair.first);
                gens.append("assert "+field+" == expected");
            }else{

                //если есть дополнительные классы
                if(getOtherClasses().size() != 0){
                    for(int i{0}; i < getOtherClasses().size(); ++i)
                        gens.prepend("obj_"+QString::number(i)+" = "+getOtherClasses().at(i)+"()");

                    int type = 1+rand()%4;
                    pair = generateRandom(type);
                    field = gens.at(1 + rand()%getOtherClasses().size()-1).split(" = ").at(0);
                    gens.append("expected = "+pair.first);
                    gens.append(field + " = "+pair.first);
                    gens.append("assert "+field+" == expected");
                }else{
                    int type = 1+rand()%4;
                    pair = generateRandom(type);
                    gens.append("expected = "+pair.first);
                    gens.append(method.getName()+"("+listArgs+")");
                    setGens(gens);
                    return;
                }
            }
            gens.insert(gens.size()-1, method.getName()+"("+listArgs+")");

        }
    }else{
        //Метод класса
        srand(time(0));
        QString obj{"object_"+classInfo.getName()};
        gens.append(obj+" = "+classInfo.getName()+"()");

        //если метод не содержит аргументов
        if(method.getArgs().size() == 0){

            //если класс не содержит полей
            if(classInfo.getFields().size() == 0){

                //если есть глобальные переменные
                if(getGlobalFields().size() != 0){

                    //если есть дополнительные классы
                    if(getOtherClasses().size() != 0)
                        for(int i{0}; i < getOtherClasses().size(); ++i)
                            gens.append("obj_"+QString::number(i)+" = "+getOtherClasses().at(i)+"()");


                    int type = 1+rand()%4;
                    pair = generateRandom(type);
                    field = getGlobalFields().at(1 + rand()%getGlobalFields().size()-1);
                    gens.append("expected = "+pair.first);
                    gens.append(field + " = "+pair.first);
                    gens.append("assert "+field+" == expected");
                }else{

                    //если есть дополнительные классы
                    if(getOtherClasses().size() != 0){
                        for(int i{0}; i < getOtherClasses().size(); ++i)
                            gens.prepend("obj_"+QString::number(i)+" = "+getOtherClasses().at(i)+"()");

                        int type = 1+rand()%4;
                        pair = generateRandom(type);
                        field = gens.at(1 + rand()%getOtherClasses().size()-1).split(" = ").at(0);
                        gens.append("expected = "+pair.first);
                        gens.append(field + " = "+pair.first);
                        gens.append("assert "+field+" == expected");
                    }else{
                        int type = 1+rand()%4;
                        pair = generateRandom(type);
                        gens.append("expected = "+pair.first);
                        gens.append(obj+"."+method.getName()+"("+listArgs+")");
                        setGens(gens);
                        return;
                    }
                }

                gens.insert(gens.size()-1, obj+"."+method.getName()+"()");
            }else{
                //Класс содержит поля

                //если есть глобальные переменные
                if(getGlobalFields().size() != 0){

                    //если есть дополнительные классы
                    if(getOtherClasses().size() != 0)
                        for(int i{0}; i < getOtherClasses().size(); ++i)
                            gens.append("obj_"+QString::number(i)+" = "+getOtherClasses().at(i)+"()");

                    //Ожидаемое значение
                    int type = 1+rand()%4;
                    QPair<QString, QString> random = generateRandom(type);

                    QStringList total = classInfo.getFields() + getGlobalFields();

                    //Выбираем случайно поле класса или глобальную переменную
                    QString field = total.at(1+rand()%total.size()-1);

                    //Вызываем метод
                    QString method = obj+classInfo.getName()+"()";

                    QString result = (type >= 4) ? random.first : random.second;

                    gens.append("expected = "+result);
                    gens.append(field + " = "+result);
                    gens.append("assert "+field+" == expected");
                }else{

                    //если есть дополнительные классы
                    if(getOtherClasses().size() != 0){
                        for(int i{0}; i < getOtherClasses().size(); ++i)
                            gens.prepend("obj_"+QString::number(i)+" = "+getOtherClasses().at(i)+"()");

                        int type = 1+rand()%4;
                        pair = generateRandom(type);
                        field = classInfo.getFields().at(1 + rand()%classInfo.getFields().size()-1);
                        gens.append("expected = "+pair.first);
                        gens.append(field + " = "+pair.first);
                        gens.append("assert "+field+" == expected");
                    }else{
                        int type = 1+rand()%4;
                        pair = generateRandom(type);
                        field = classInfo.getFields().at(1 + rand()%classInfo.getFields().size()-1);
                        gens.append("expected = "+pair.first);
                        gens.append(field + " = "+pair.first);
                        gens.append("assert "+field+" == expected");
                    }
                }
                gens.insert(gens.size()-1, obj+"."+method.getName()+"()");

            }

        }else{
            for(int i{0}; i < method.getArgs().size(); ++i){

                int type = 1+rand()%4;
                pair = generateRandom(type);
                result = (type >= 4) ? pair.first : pair.second;
                gens.append("args_"+QString::number(i)+" = " + result);
                listArgs += "args_"+QString::number(i)+",";
            }

            //если класс не содержит полей
            if(classInfo.getFields().size() == 0){

                //если есть глобальные переменные
                if(getGlobalFields().size() != 0){

                    //если есть дополнительные классы
                    if(getOtherClasses().size() != 0)
                        for(int i{0}; i < getOtherClasses().size(); ++i)
                            gens.append("obj_"+QString::number(i)+" = "+getOtherClasses().at(i)+"()");


                    int type = 1+rand()%4;
                    pair = generateRandom(type);
                    field = getGlobalFields().at(1 + rand()%getGlobalFields().size()-1);
                    gens.append("expected = "+pair.first);
                    gens.append(field + " = "+pair.first);
                    gens.append("assert "+field+" == expected");
                }else{

                    //если есть дополнительные классы
                    if(getOtherClasses().size() != 0){
                        for(int i{0}; i < getOtherClasses().size(); ++i)
                            gens.prepend("obj_"+QString::number(i)+" = "+getOtherClasses().at(i)+"()");

                        int type = 1+rand()%4;
                        pair = generateRandom(type);
                        field = gens.at(1 + rand()%getOtherClasses().size()-1).split(" = ").at(0);
                        gens.append("expected = "+pair.first);
                        gens.append(field + " = "+pair.first);
                        gens.append("assert "+field+" == expected");
                    }else{
                        int type = 1+rand()%4;
                        pair = generateRandom(type);
                        gens.append("expected = "+pair.first);
                        gens.append(obj+"."+method.getName()+"("+listArgs+")");
                        setGens(gens);
                        return;
                    }
                }

                gens.insert(gens.size()-1, obj+"."+method.getName()+"("+listArgs+")");
            }else{
                //Класс содержит поля

                //если есть глобальные переменные
                if(getGlobalFields().size() != 0){

                    //если есть дополнительные классы
                    if(getOtherClasses().size() != 0)
                        for(int i{0}; i < getOtherClasses().size(); ++i)
                            gens.append("obj_"+QString::number(i)+" = "+getOtherClasses().at(i)+"()");

                    //Ожидаемое значение
                    int type = 1+rand()%4;
                    QPair<QString, QString> random = generateRandom(type);

                    QStringList total = classInfo.getFields() + getGlobalFields();

                    //Выбираем случайно поле класса или глобальную переменную
                    QString field = total.at(1+rand()%total.size()-1);

                    //Вызываем метод
                    QString method = obj+classInfo.getName()+"("+listArgs+")";

                    QString result = (type >= 4) ? random.first : random.second;

                    gens.append("expected = "+result);
                    gens.append(field + " = "+result);
                    gens.append("assert "+field+" == expected");
                }else{

                    //если есть дополнительные классы
                    if(getOtherClasses().size() != 0){
                        for(int i{0}; i < getOtherClasses().size(); ++i)
                            gens.prepend("obj_"+QString::number(i)+" = "+getOtherClasses().at(i)+"()");

                        int type = 1+rand()%4;
                        pair = generateRandom(type);
                        field = classInfo.getFields().at(1 + rand()%classInfo.getFields().size()-1);
                        gens.append("expected = "+pair.first);
                        gens.append(field + " = "+pair.first);
                        gens.append("assert "+field+" == expected");
                    }else{
                        int type = 1+rand()%4;
                        pair = generateRandom(type);
                        field = classInfo.getFields().at(1 + rand()%classInfo.getFields().size()-1);
                        gens.append("expected = "+pair.first);
                        gens.append(field + " = "+pair.first);
                        gens.append("assert "+field+" == expected");
                    }
                }
                gens.insert(gens.size()-1, obj+"."+method.getName()+"("+listArgs+")");

            }

        }

    }
    setGens(gens);

}


