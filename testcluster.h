#ifndef TESTCLUSTER_H
#define TESTCLUSTER_H

#include <jsonparser.h>

/*!
 * \brief Класс тестовый кластер
 */
class TestCluster
{
public:
    /*!
     * \brief Конструктор
     */
    TestCluster();

    /*!
     * \brief Конструктор
     * \param fileName Имя файла
     */
    TestCluster(const QString fileName);

    /*!
     * \brief Деструктор
     */
    ~TestCluster();

    /*!
     * \brief Установить имя файла
     * \param fileName Имя файла
     */
    void setFileName(QString fileName){ this->fileName = fileName; }

    /*!
     * \brief Получить имя файла
     * \return Имя файла
     */
    QString getFileName() { return fileName; }

//    void setListFilesPopulation(QStringList list) { this->listFilesPopulation = list; }
//    QStringList getListFilesPopulation() { return listFilesPopulation; }

    /*!
     *
     * \brief Начать разбиение
     */
    void makeClusters();

private:
    /*!
     * \brief JSON-парсер
     */
    JSONparser* parser;

    /*!
     * \brief Имя файла
     */
    QString fileName;

};

#endif // TESTCLUSTER_H
