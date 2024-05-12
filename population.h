#ifndef POPULATION_H
#define POPULATION_H

#include <time.h>
#include <algorithm>
#include <QList>
#include <QDebug>
#include <QDateTime>
#include <chromosome.h>
#include <methodinfo.h>
#include <jsonparser.h>

/*!
 * \brief Набор тест-кейсов (популяция)
 */
class Population : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Конструктор класса
     */
    Population();

    /*!
     * \brief Конструктор класса
     * \param mut Вероятность мутации
     * \param cross Вероятность кроссовера
     */
    Population(float mut, float cross);

    /*!
     * \brief Конструктор класса
     * \param index Номер популяции
     * \param total Количество итераций
     * \param filePath Имя файла с JSON
     */
    Population(int index, int total, QString filePath);

    /*!
     * \brief Деструктор
     */
    ~Population();

    /*!
     * \brief Установить веротноять мутации
     * \param mut Вероятность мутации
     */
    void setProbMutation(const float mut) { this->mutationProbability = mut; }

    /*!
     * \brief Установить вероятность кроссовера
     * \param cross Вероятность кроссовера
     */
    void setProbCrossover(const float cross) { this->crossoverProbability = cross;}

    /*!
     * \brief Получить текущий список хромосом
     * \return Текущий список хромосом
     */
    QList<AbstractChromosome*> getCurrentList () { return currentListChromosome; }

    /*!
     * \brief Получить список хромосом от другой популяции
     * \param list Список новых хромосом
     */
    void receiveChromosomes(QList<AbstractChromosome*> list);

    /*!
     * \brief Отправить список хромосом другой популяции
     * \param count Количество отправляемых хромосом
     * \return Список отправляемых хромосом
     */
    QList<AbstractChromosome*> sendChromosomes(const int count);

    /*!
     * \brief Вывести сообщение на печать
     * \param message Сообщение на печать
     */
    void writeMessage(const QString& message);

public slots:
    /*!
     * \brief Слот начала алгоритма
     */
    void start();
signals:
    /*!
     * \brief Сигнал о готовности популяции к обмену особями
     */
    void signalReadySwap();

    /*!
     * \brief Сигнал о завершении алгоритма
     */
    void signalFinish();

    /*!
     * \brief Вывод сообщения на экран
     * \param index Индекс популяции
     * \param message Сообщение
     */
    void signalWriteMessage(int index, const QString& message);

    void signalTotalMethods(int total);

    void signalCoveragedMethods(int coveraged);

private:

    /*!
     * \brief Текущий набор хромосом в популяции
     */
    QList<AbstractChromosome*> currentListChromosome;

    /*!
     * \brief Временный список с хромосомами
     */
    QList<AbstractChromosome*> tempListChromosome;

    /*!
     * \brief Новый набор хромосом в популяции
     */
    QList<AbstractChromosome*> newListChromosome;

    /*!
     * \brief Вероятность мутации = 0.02
     */
    mutable float mutationProbability ;

    /*!
     * \brief Вероятность кроссовера = 0.8
     */
    mutable float crossoverProbability;

    /*!
     * \brief Размер турнира = 3
     */
    const ushort tournamentNum;

    /*!
     * \brief Размер популяции
     */
    int populationSize;

    /*!
     * \brief Список информации о классе и методах
     */
//    QList<GeneralInfo*> listGeneralInfo;

    /*!
     * \brief Среднее значение функции пригодности в популяции
     */
    int avgFitnessFunc;

    /*!
     * \brief Количество итераций, по завершению которых происходит обмен особями с другой популяцией
     */
    int numberIteration;

    /*!
     * \brief Общее количество итераций
     */
    int totalIterations;

    /*!
     * \brief Текущая итерация
     */
    int currentIteration;

    /*!
     * \brief Порядковый номер популяции
     */
    int index;

    /*!
     * \brief Имя файла, с которого будет считываться JSON
     */
    QString filePath;

    /*!
     * \brief Список общих функций приспособленностей для все популяции
     * \details Условие останова: если в течении нескольких поколений (наверное будет 5)
     *          функция приспособленности не будет сильно меняться, то будет остановка работы
     */
    QList<float> fitnessFunction;

    int totalMethods;

    /*!
     * \brief Сгенерировать случайное целочисленное число в диапозоне [min, max]
     * \param Минимальное значение
     * \param Максимальное значение
     * \return Целочисленное число
     */
    int random(const int min, const int max);

    /*!
     * \brief Сгенерировать случайное вещественное число
     * \return Вещественное число
     */
    float randomFloat();



    /*!
     * \brief Оператор скрещивания
     */
    void operatorCrossover();

    /*!
     * \brief Оператор мутации
     */
    void operatorMutation();

    /*!
     * \brief Оператор выбора родителей для оператора скрещивания
     */
    void operatorSelection();

    /*!
     * \brief Оценить степень приспособленности каждой особи
     */
    void fitnessCalculation();

    /*!
     * \brief Оценить среднее значение функции приспособленности в популяции
     */
    void avgFitnessCalculation();

    /*!
     * \brief Создать началую популяцю
     */
    void initPopulation();

    /*!
     * \brief Оператор отбора особей в новую популяцию (редукция)
     */
    void operatorReduction();

    /*!
     * \brief Оператор выбора родителей (выбран аутбридинг)
     * \return Пара родителей, к которым будет применен оператор скрещивания
     */
    QPair<AbstractChromosome*, AbstractChromosome*> outbreeding();

    /*!
     * \brief Проверить условие останова
     * \return Решение: продолжить исполнение алгоритма или выйти
     */
    bool checkStopCondition();

    /*!
     * \brief Установить размер популяции
     * \param populationSize Размер популяции
     */
    void setPopulationSize(int populationSize) { this->populationSize = populationSize; }

    /*!
     * \brief Получить размер популяции
     * \return Размер популяции
     */
    int getPopulationSize() { return populationSize; }

    /*!
     * \brief Установить текущий список хромосом
     * \param list Список хромосом
     */
    void setCurrentList(QList<AbstractChromosome*> list) { this->currentListChromosome = list; }

    /*!
     * \brief Получить номер итерации (используется для обмена особями с другой популяцией)
     * \return Номер итерации
     */
    int getNumberIterations() { return numberIteration; }

    /*!
     * \brief Установить номер итерации
     * \param num Номер итерации
     */
    void setNumberIterations(int num) { this->numberIteration = num; }

    /*!
     * \brief Напечатать хромосомы
     */
    void printChromosome();

//    void checkTotalMethods();

    void checkCoveragedMethods();

};

#endif // POPULATION_H
