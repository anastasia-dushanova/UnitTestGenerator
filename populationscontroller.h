#ifndef POPULATIONSCONTROLLER_H
#define POPULATIONSCONTROLLER_H

#include <QObject>
#include <QThread>
#include <QTime>
#include <population.h>
#include <testcasedecor.h>

class PopulationsController : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Конструктор класса
     * \param Количество итераций в алгоритме
     * \param parent Родитель
     */
    explicit PopulationsController(int total, QObject *parent = nullptr);

    /*!
     * \brief Деструктор класса
     */
    ~PopulationsController();

    /*!
     * \brief Установить список популяций
     * \param list Список популяций
     */
    void setListPopulation(QList<Population*> list) { this->listPopulation = list; }

    /*!
     * \brief Получить список популяций
     * \return Список популяций
     */
    QList<Population*> getListPopulation() { return listPopulation; }

    /*!
     * \brief Получить общее количество итераций
     * \return Количество итераций
     */
    int getTotalIteration () { return totalIterations; }

    /*!
     * \brief Добавить популяцию
     * \param pop Популция
     */
    void appendPopulation(Population* pop){ listPopulation.append(pop); }

    /*!
     * \brief Проверить 4 JSON-файла
     */
    void checkFiles();

    /*!
     * \brief Инициализация популяций и запуск
     */
    void initPopulation();

signals:
    /*!
     * \brief Сигнал популциям о начале вычислений
     */
    void signalStart();

    /*!
     * \brief Сигнал популяциям о продолжении вычислений
     */
    void signalContinue();

public slots:
    /*!
     * \brief Слот для окончания алгоритма. Собираем хромосомы со всех популяций и оформляем их в тест-кейсы
     */
    void slotFinish();

    /*!
     * \brief Слот для проверки готовности всех популяций к обмену
     */
    void slotCheckCount();

private:
    /*!
     * \brief Обменяться хромосомами
     */
    void swapChromosoms();

    /*!
     * \brief Список с потоками
     */
    QList<QThread*> listThreads;

    /*!
     * \brief Список популяций
     */
    QList<Population*> listPopulation;

    /*!
     * \brief Количество популяций готовых к обмену
     */
    int populationsReady;

    /*!
     * \brief Количество популяций закончивших выполнение алгоритма
     */
    int populationsFinished;

    /*!
     * \brief Количество обменов
     */
    int countSwap;

    /*!
     * \brief Декоратор (оформление получившихся хромосом в тест-кейсы)
     */
    TestCaseDecor* decor;

    /*!
     * \brief Карта с файлами, по которой определяется количество хромосом
     */
    QMap<QString, bool> listFileNames;

    /*!
     * \brief Общее количество итераций алгоритма
     */
    const int totalIterations;

    /*!
     * \brief Таймер
     */
    QTime timer;

};

#endif // POPULATIONSCONTROLLER_H
