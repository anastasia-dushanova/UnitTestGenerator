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

    /*!
     * \brief Получить имя класса
     * \return Имя класса
     */
    QString getName() { return name; }

    /*!
     * \brief Установить имя класса
     * \param name Имя класса
     */
    void setName(QString name) { this->name = name; }

    /*!
     * \brief Получить поля класса
     * \return Поля класса
     */
    QStringList getFields() { return fields; }

    /*!
     * \brief Установить поля класса
     * \param fields Поля класса
     */
    void setFields(QStringList fields) {this->fields = fields; }

private:

    /*!
     * \brief Имя класса
     */
    QString name;

    /*!
     * \brief Поля класса
     */
    QStringList fields;
};

/*!
 * \brief Представление тест-кейса в виде хромосомы
 */

class AbstractChromosome
{
public:

    /*!
     * \brief Перечисление базовых типов
     */
    enum class types : int {
        typesInt = 1,
        typesFloat,
        typesStr,
        typeList,
        typeDict
    };

    /*!
     * \brief Количество базовых типов
     */
    const int totalTypeNum = 4;

    /*!
     * \brief Абстрактный класс для хромосом
     */
    AbstractChromosome();

    /*!
     * \brief Абстрактный класс для хромосом
     * \param className Информация о классе
     * \param method Информация о методе
     */
    AbstractChromosome(ClassInfo className, Method method);

    /*!
     * \brief Удалить конструктор копирования
     */
    AbstractChromosome(const AbstractChromosome&) = delete;

    /*!
     * \brief Удалить конструктор перемещения
     */
    AbstractChromosome(AbstractChromosome&&) = delete;

    /*!
     * \brief operator = Удалить оператор присваивания копии
     * \return
     */
    AbstractChromosome& operator=(const AbstractChromosome&) = delete;

    /*!
     * \brief operator = Удалить оператор присваивания перемещения
     * \return
     */
    AbstractChromosome& operator=(AbstractChromosome&&) = delete;

    /*!
     * \brief Виртуальный деструктор
     */
    virtual ~AbstractChromosome() {}

    /*!
     * \brief Установить хромосому
     * \param classInfo Информация о классе
     * \param method Информация о методе
     */
    virtual void setChromosome(ClassInfo classInfo, Method method);

    /*!
     * \brief Установить значение фукцнии пригодности
     * \param fitness Значение функции пригодности
     */
    void setFitness(float fitness) { this->fitness = fitness; }

    /*!
     * \brief Получить значение функции пригодности
     * \return Значение функции пригодности
     */
    float getFitness() { return fitness; }

    /*!
     * \brief Установить гены
     * \param list Список генов
     */
    void setGens(QList<QString> list) {
        gens.clear();
        this->gens = list;
    }

    /*!
     * \brief Получить список генов
     * \return Список генов
     */
    QList<QString> getGens() { return gens; }

    /*!
     * \brief Получить информацию о классе
     * \return Информация о классе
     */
    ClassInfo getClassInfo() { return classInfo; }

    /*!
     * \brief Установить информацию о классе
     * \param Информация о классе
     */
    void setClassInfo(ClassInfo className) { this->classInfo = className; }

    /*!
     * \brief Получить информацию о методе
     * \return Информация о методе
     */
    Method getMethod() { return method; }

    /*!
     * \brief Установить информацию о методе
     * \param method Информация о методе
     */
    void setMethod(Method method) { this->method = method; }

    //TODO еще заполнить эти поля
    //они заполняются при обработке JSON

    /*!
     * \brief Получить информацию о других классах
     * \return Информация о других классах
     */
    QStringList getOtherClasses() { return otherClasses; }

    /*!
     * \brief Установить информацию о других классах
     * \param Информация о других классах
     */
    void setOtherClasses(QStringList otherClasses) { this->otherClasses = otherClasses; }

    /*!
     * \brief Получить поля класса
     * \return Поля класса
     */
    QStringList getGlobalFields() { return globalFields; }

    /*!
     * \brief Установить поля класса
     * \param fields Поля класса
     */
    void setGlobalFields(QStringList fields) { this->globalFields = fields; }

    /*!
     * \brief Оператор мутации
     */
    virtual void mutationGen();

    /*!
     * \brief Вычислить значение функции пригодности
     */
    virtual void fitnessCalculation();

protected:

    /*!
     * \brief Сгенерировать целое число
     * \return Целое число
     */
    int randomInt();

    /*!
     * \brief Сгенерировать вещественное число
     * \return Вещественное число
     */
    float randomFloat();

    /*!
     * \brief Сгенерировать строку
     * \return Строка
     */
    QString randomString();

    /*!
     * \brief Сгенерировать пару определенного типа
     * \param type Тип генерации
     * \return Пара
     */
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

/*!
 * \brief Класс с возвращающим методом
 */
class ChromosomeReturningFunc : public AbstractChromosome{

public:
    /*!
     * \brief Конструктор класса
     */
    ChromosomeReturningFunc();

    /*!
     * \brief Деструктор класса
     */
    ~ChromosomeReturningFunc() {}

    /*!
     * \brief Установить хромосому
     * \param classInfo Информацию о классе
     * \param method Информация о методе
     */
    void setChromosome(ClassInfo classInfo, Method method) override;

    //TODO доделать
    /*!
     * \brief Вычислить значение функции пригодности
     */
    void fitnessCalculation() override;

    //TODO доделать
    /*!
     * \brief Оператор мутации
     */
    void mutationGen() override;

private:

};

/*!
 * \brief Класс с void функцией
 */
class ChromosomeNonReturningFunc : public AbstractChromosome{

public:
    /*!
     * \brief Конструктор класса
     */
    ChromosomeNonReturningFunc();

    /*!
     * \brief Деструктор класса
     */
    ~ChromosomeNonReturningFunc(){}

    /*!
     * \brief Установить хромосому
     * \param classInfo Информация о классе
     * \param method Информация о методе
     */
    void setChromosome(ClassInfo classInfo, Method method) override;

    //TODO доделать
    /*!
     * \brief Вычислить значение функции пригодности
     */
    void fitnessCalculation() override;

    //TODO доделать
    /*!
     * \brief Оператор мутации
     */
    void mutationGen() override;

private:

};

#endif // CHROMOSOME_H
