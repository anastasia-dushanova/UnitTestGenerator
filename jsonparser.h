#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QString>
#include <methodinfo.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QApplication>

/*!
 * \brief JSON-парсер
 */
class JSONparser
{
public:
    /*!
     * \brief Конструктор класса
     */
    JSONparser();

    /*!
     * \brief Конструктор класса
     * \param fileName Имя файла
     */
    JSONparser(const QString fileName);

    /*!
     * \brief Деструктор класса
     */
    ~JSONparser() {}

    /*!
     * \brief Получить имя файла
     * \return Имя файла
     */
    QString getFileName() { return fileName; }

    /*!
     * \brief Установить имя файла
     * \param fileName Имя файла
     */
    void setFileName(const QString fileName) { this->fileName = fileName; }

    /*!
     * \brief Парсить файл внутри для популяций
     * \return Список данных из файла
     */
    QList<GeneralInfo*> parseJSONPopulation();

    /*!
     * \brief Парсить файл для тестового кластера
     */
    void parseJSONCluster();

private:
    /*!
     * \brief Имя файла, которое будет читать парсер
     */
    QString fileName;
};

#endif // JSONPARSER_H
