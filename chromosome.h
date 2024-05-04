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

/*!
 * \brief Класс, содержащий информацию о методе
 */
class Method{

public:
    /*!
     * \brief Конструктор класса
     */
    Method() : name(""), args(""), linesCount(0), returnValue(false){ }

    /*!
     * \brief Конструктор класса
     * \param name - Имя метода
     * \param args - Аргументы метода
     * \param linesCount - Количество строк, занимаемые методом
     * \param value - Возвращающий ли метод
     */
    Method(QString name, QStringList args, int linesCount, bool value)
        : name(name), args(args), linesCount(linesCount), returnValue(value){ }

    /*!
     * \brief Деструктор класса
     */
    ~Method();

    /*!
     * \brief Получить имя метода
     * \return Имя метода
     */
    QString getName() { return name; }

    /*!
     * \brief Установить имя метода
     * \param name - Имя метода
     */
    void setName(QString name) { this->name = name; }

    /*!
     * \brief Получить аргументы метода
     * \return Список аргументов метода
     */
    QStringList getArgs() { return args; }

    /*!
     * \brief Установить аргументы метода
     * \param args - Список аргументов метода
     */
    void setArgs(QStringList args) {this->args = args; }

    /*!
     * \brief Получить количество строк метода
     * \return Количество строк
     */
    int getLinesCount() { return linesCount; }

    /*!
     * \brief Установить количество строк метода
     * \param count - Количество строк метода
     */
    void setLinesCount(int count) { this->linesCount = count; }

    /*!
     * \brief Возвращающий ли метод
     * \return Да или нет
     */
    bool getReturnValue() { return returnValue; }

    /*!
     * \brief Установить вид метода
     * \param value - Возвращающий или нет
     */
    void setReturnValue(bool value) { this->returnValue = value; }

private:

    /*!
     * \brief Имя метода
     */
    QString name;

    /*!
     * \brief Список аргументов
     */
    QStringList args;

    /*!
     * \brief Количество строк
     */
    int linesCount;

    /*!
     * \brief Возвращающий ли метод
     */
    bool returnValue;
};


/*!
 * \brief Информация о методе
 */
class ClassInfo{
public:

    /*!
     * \brief Конструктор класса
     */
    ClassInfo() : name(""), fields(QStringList()) { }

    /*!
     * \brief Конструктор класса
     * \param Имя класса
     * \param Поля класса
     */
    ClassInfo(QString name, QStringList fields) : name(name), fields(fields){ }

    /*!
     * \brief Деструктор класса
     */
    ~ClassInfo();

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
