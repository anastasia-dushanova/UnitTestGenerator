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
    friend class UnitTestGenerator;

    /*!
     * \brief Конструктор класса
     * \param Количество итераций в алгоритме
     * \param parent Родитель
     */
    explicit PopulationsController(QObject *parent = nullptr);

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
     * \brief Установить вероятность мутации
     * \param mut Вероятность мутации
     */
    void setProbMutation(float mut) { this->probMutation = mut; }

    /*!
     * \brief Установить вероятность кроссовера
     * \param cross Вероятность кроссовера
     */
    void setProbCrossover(float cross) { this->probCross = cross; }

    void setTotalIter(int total) { this->totalIterations = total; }

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

    /*!
     * \brief Удалить все популяции
     */
    void deletePopulations();

signals:
    /*!
     * \brief Сигнал популциям о начале вычислений
     */
    void signalStart();

    /*!
     * \brief Сигнал популяциям о продолжении вычислений
     */
    void signalContinue();

    /*!
     * \brief Сигнал о выводе на экран сообщения
     * \param index Индекс популяции
     * \param message Текст сообщения
     */
    void signalWrite(int index, const QString& message);

    /*!
     * \brief Сигнал об общем времени выполнении
     * \param time Время
     */
    void signalTimeElapsed(int& time);

    /*!
     * \brief Сигнал об обмене хромосомами между популяциями
     */
    void signalIteration();

    /*!
     * \brief Сигнал о заверешнии выполнения алгоритма
     */
    void signalFinish(int coveraged, int total);

public slots:
    /*!
     * \brief Слот для окончания алгоритма. Собираем хромосомы со всех популяций и оформляем их в тест-кейсы
     */
    void slotFinish();

    /*!
     * \brief Слот для проверки готовности всех популяций к обмену
     */
    void slotCheckCount();

    /*!
     * \brief Вывести сообщение на экран
     * \param index Индекс популяции
     * \param message Текст сообщения
     */
    void slotWriteMessage(int index, const QString& message);

private slots:

    void slotTotalMethods(int total);

    void slotCoveragedMethods(int coveraged);
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
    mutable int totalIterations;

    /*!
     * \brief Таймер
     */
    QTime timer;

    /*!
     * \brief Вероятность мутации
     */
    float probMutation;

    /*!
     * \brief Вероятность кроссовера
     */
    float probCross;

    /*!
     * \brief Порядковый номер
     */
    int index;

    int totalsMethods;

    int coveragedMethods;

};

#endif // POPULATIONSCONTROLLER_H
