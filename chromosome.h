#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>
#include <QList>
#include <QString>
#include <QDebug>
#include <QPair>
#include <QStringList>



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
 * при тестировании такого вида функции, проверяется изменение состояний полей класса
 */

class Method{

public:
    Method() : name(""), args(""), linesCount(0), returnValue(false){ }
    Method(QString name, QStringList args, int linesCount, bool value)
        : name(name), args(args), linesCount(linesCount), returnValue(value){ }
    ~Method() { }

    QString getName() { return name; }
    void setName(QString name) { this->name = name; }

    QStringList getArgs() { return args; }
    void setArgs(QStringList args) {this->args = args; }

    int getLinesCount() { return linesCount; }
    void setLinesCount(int count) { this->linesCount = count; }

    bool getReturnValue() { return returnValue; }
    void setReturnValue(bool value) { this->returnValue = value; }

private:
    QString name;
    QStringList args;
    int linesCount;
    bool returnValue;
};

class ClassInfo{
public:
    ClassInfo() : name(""), fields(QStringList()) { }
    ClassInfo(QString name, QStringList fields) : name(name), fields(fields){ }
    ~ClassInfo() { }

    QString getName() { return name; }
    void setName(QString name) { this->name = name; }

    QStringList getFields() { return fields; }
    void setFields(QStringList fields) {this->fields = fields; }

private:
    QString name;
    QStringList fields;
};

/*!
 * \brief Представление тест-кейса в виде хромосомы
 */

class AbstractChromosome
{
public:

    //базовые типы
    enum class types : int {
        typesInt = 1,
        typesFloat,
        typesStr,
        typeList,
        typeDict
    };

    const int totalTypeNum = 4;

    AbstractChromosome();
    AbstractChromosome(ClassInfo className, Method method);

    AbstractChromosome(const AbstractChromosome&) = delete;
    AbstractChromosome(AbstractChromosome&&) = delete;

    AbstractChromosome& operator=(const AbstractChromosome&) = delete;
    AbstractChromosome& operator=(AbstractChromosome&&) = delete;

    virtual ~AbstractChromosome() {}

    virtual void setChromosome(ClassInfo classInfo, Method method);

    void setFitness(float fitness) { this->fitness = fitness; }
    float getFitness() { return fitness; }

    void setGens(QList<QString> list) {
        gens.clear();
        this->gens = list;
    }
    QList<QString> getGens() { return gens; }

    ClassInfo getClassInfo() { return classInfo; }
    void setClassInfo(ClassInfo className) { this->classInfo = className; }

    Method getMethod() { return method; }
    void setMethod(Method method) { this->method = method; }

    //TODO еще заполнить эти поля
    //они заполняются при обработке JSON
    QStringList getOtherClasses() { return otherClasses; }
    void setOtherClasses(QStringList otherClasses) { this->otherClasses = otherClasses; }

    QStringList getGlobalFields() { return globalFields; }
    void setGlobalFields(QStringList fields) { this->globalFields = fields; }

    virtual void mutationGen();
    virtual void fitnessCalculation();

protected:

    int randomInt();
    float randomFloat();
    QString randomString();

    QPair<QString, QString> generateRandom(const int type);

private:
    /*!
     * \brief Имя класса, если есть
     */
    ClassInfo classInfo;

    /*!
     * \brief Информация о методе
     */
    Method method;

    /*!
     * \brief Приспособленность особи
    */
    float fitness;

    /*!
     * \brief Содержимое хромосомы (гены)
     */
    QStringList gens;

    /*!
     * \brief Дополнительные типы (объекты классов)
     */
    QStringList otherClasses;

    /*!
     * \brief Дополнительные типы (глобальные переменные)
     */
    QStringList globalFields;

};

class ChromosomeReturningFunc : public AbstractChromosome{

public:
    ChromosomeReturningFunc();
    ~ChromosomeReturningFunc() {}

    void setChromosome(ClassInfo classInfo, Method method) override;

    //TODO доделать
    void fitnessCalculation() override;

    //TODO доделать
    void mutationGen() override;

private:

};

class ChromosomeNonReturningFunc : public AbstractChromosome{

public:
    ChromosomeNonReturningFunc();
    ~ChromosomeNonReturningFunc(){}

    void setChromosome(ClassInfo classInfo, Method method) override;

    //TODO доделать
    void fitnessCalculation() override;

    //TODO доделать
    void mutationGen() override;

private:

};

#endif // CHROMOSOME_H
