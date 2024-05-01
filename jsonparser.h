#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QString>
#include <methodinfo.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QApplication>

class JSONparser
{
public:
    JSONparser();
    JSONparser(const QString fileName);
    ~JSONparser() {}

    QString getFileName() { return fileName; }
    void setFileName(const QString fileName) { this->fileName = fileName; }

    QList<GeneralInfo*> parseJSON();

private:
    QString fileName;
};

#endif // JSONPARSER_H
