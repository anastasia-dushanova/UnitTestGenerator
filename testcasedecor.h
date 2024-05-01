#ifndef TESTCASEDECOR_H
#define TESTCASEDECOR_H

#include <QFile>
#include <QList>
#include <QApplication>
#include <chromosome.h>

class TestCaseDecor
{
public:
    TestCaseDecor();
    TestCaseDecor(const QString fileName);
    ~TestCaseDecor();

    void setListTestCase(QList<AbstractChromosome*> list) { this->listTestCase = list; }
    QList<AbstractChromosome*> getListTestCase() { return listTestCase; }

    void setFileName(QString fileName) { this->fileName = fileName; }
    QString getFileName() { return fileName; }

    void appendToListTestCase(QList<AbstractChromosome*> appendedList);

    void decor();

private:
    QList<AbstractChromosome*> listTestCase;

    QString fileName;
};

#endif // TESTCASEDECOR_H
