#ifndef TESTCLUSTER_H
#define TESTCLUSTER_H

#include <jsonparser.h>

class TestCluster
{
public:
    TestCluster();
    TestCluster(const QString fileName);
    ~TestCluster();

    void setFileName(QString fileName){ this->fileName = fileName; }
    QString getFileName() { return fileName; }

//    void setListFilesPopulation(QStringList list) { this->listFilesPopulation = list; }
//    QStringList getListFilesPopulation() { return listFilesPopulation; }

    void makeClusters();

private:
    JSONparser* parser;

    QString fileName;

//    QStringList listFilesPopulation;
};

#endif // TESTCLUSTER_H
