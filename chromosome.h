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

/*!
 * \brief Представление тест-кейса в виде хромосомы
 */

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

class Chromosome
{
public:

    enum class types : int {
        typesInt = 1,
        typesFloat,
        typesStr
    };

    Chromosome();
//    Chromosome(QString method, int args);

    Chromosome(const Chromosome&) = delete;
    Chromosome(Chromosome&&) = delete;

    Chromosome& operator=(const Chromosome&) = delete;
    Chromosome& operator=(Chromosome&&) = delete;
    ~Chromosome() {}

    /*!
     * \brief Установить хромосому
     * \param Имя метода
     * \param Количество аргументов метода
     */
    void setChromosome(QString method, size_t args, int type ,bool returnValue);
//    Chromosome& getChromosome() { return *this;}

    void setFitness(float fitness) { this->fitness = fitness; }
    float getFitness() { return fitness; }

    void setGens(QList<QString> list) {
        gens.clear();
        this->gens = list;
    }
    QList<QString> getGens() { return gens; }

    void mutationGen();

private:

    int randomInt();
    float randomFloat();
    QString randomString();

    QPair<QString, QString> generateRandom(const int type);

    /*!
     * \brief Приспособленность особи
    */
    float fitness;

    /*!
     * \brief Содержимое хромосомы (гены)
     */
    QList<QString> gens;

    /*!
     * \brief Доступные типы
     */
//    enum class types

    /*!
     * \brief Метод на проверку
     */
//    QString method;

    /*!
     * \brief Возвращаемый тип
     */
//    QString returnType;
};

#endif // CHROMOSOME_H
