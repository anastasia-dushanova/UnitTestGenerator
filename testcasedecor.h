#ifndef TESTCASEDECOR_H
#define TESTCASEDECOR_H

#include <QFile>
#include <QList>
#include <QApplication>
#include <chromosome.h>
#include <QTextStream>

/*!
 * \brief Класс для оформления хромосом в тест-кейсы
 */
class TestCaseDecor
{
public:
    /*!
     * \brief Конструктор класса
     */
    TestCaseDecor();

    /*!
     * \brief Конструктор класса
     * \param fileName Имя файла, в котором будут тест-кейсы
     */
    TestCaseDecor(const QString fileName);

    /*!
     * \brief Декоратор
     */
    ~TestCaseDecor();

    /*!
     * \brief Установить список тест-кейсов
     * \param list Список тест-кейсов
     */
    void setListTestCase(QList<AbstractChromosome*> list) { this->listTestCase = list; }

    /*!
     * \brief Получить список тест-кейсов
     * \return Список тест-кейсов
     */
    QList<AbstractChromosome*> getListTestCase() { return listTestCase; }

    /*!
     * \brief Установить имя файла
     * \param fileName Имя файла
     */
    void setFileName(QString fileName) { this->fileName = fileName; }

    /*!
     * \brief Получить имя файла
     * \return Имя файла
     */
    QString getFileName() { return fileName; }

    /*!
     * \brief Добавить список хромосом
     * \param Список хромосом
     */
    void appendToListTestCase(QList<AbstractChromosome*> appendedList);

    /*!
     * \brief Оформить хромосомы в тест-кейсы и занести в файл
     */
    void decor();

private:
    /*!
     * \brief Список хромосом для тест-кейсов
     */
    QList<AbstractChromosome*> listTestCase;

    /*!
     * \brief Имя файла
     */
    QString fileName;
};

#endif // TESTCASEDECOR_H
