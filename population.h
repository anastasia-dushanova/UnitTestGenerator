#ifndef POPULATION_H
#define POPULATION_H

#include <time.h>
#include <algorithm>
#include <QList>
#include <QDebug>
#include <chromosome.h>
#include <methodinfo.h>
#include <jsonparser.h>

/*!
 * \brief Набор тест-кейсов (популяция)
 */
class Population
{
public:
    Population();
    Population(float mut, float cross);
    ~Population();

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

    void setPopulationSize(int populationSize) { this->populationSize = populationSize; }
    int getPopulationSize() { return populationSize; }

    void printMethodInfo();
    void printChromosome();
    void initMethodInfo(const int size);

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
    const float mutationProbability ;

    /*!
     * \brief Вероятность кроссовера = 0.8
     */
    const float crossoverProbability;

    /*!
     * \brief Размер турнира = 3
     */
    const ushort tournamentNum;

    /*!
     * \brief Размер популяции
     */
    int populationSize;

    QList<GeneralInfo*> listGeneralInfo;


    /*!
     * \brief Среднее значение функции пригодности в популяции
     */
    int avgFitnessFunc;

    /*!
     * \brief Список общих функций приспособленностей для все популяции
     * \details Условие останова: если в течении нескольких поколений (наверное будет 5)
     *          функция приспособленности не будет сильно меняться, то будет остановка работы
     */
    QList<float> fitnessFunction;

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


};

#endif // POPULATION_H
