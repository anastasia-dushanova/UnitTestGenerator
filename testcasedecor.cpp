#include "testcasedecor.h"

TestCaseDecor::TestCaseDecor() : listTestCase(QList<AbstractChromosome*>()), fileName(""){ }

TestCaseDecor::TestCaseDecor(const QString fileName) : listTestCase(QList<AbstractChromosome*>()), fileName(fileName) { }

TestCaseDecor::~TestCaseDecor() { }

void TestCaseDecor::appendToListTestCase(QList<AbstractChromosome*> appendedList){

    for(auto ch : appendedList)
        listTestCase.append(ch);
}

void TestCaseDecor::decor(){

    QFile file(QApplication::applicationDirPath() + "/" + fileName);
    if(!file.exists()){
        qDebug() << "Файл не существует. Создаю "+fileName;
//        file.open(QIODevice::WriteOnly | QIODevice::Text);
//        file.close();
    }

    if(!file.open(QIODevice::WriteOnly | QIODevice::Append)){
        qDebug() << "Не удалось открыть файл";
        return;
    }else{
        file.resize(0);
        QByteArray array;
        QTextStream stream(&file);
        QString test{"\n\ndef test_case_"};

        for(int i{0}; i < listTestCase.size(); ++i){

            if(listTestCase.at(i)->getMethod().getReturnValue() == true){

                ChromosomeReturningFunc* chrom = static_cast<ChromosomeReturningFunc*>(listTestCase.at(i));

                stream << test+QString::number(i)+"():";

                for(auto ch : chrom->getGens())
                    stream << "\n\t"+ch;

            }else{

                ChromosomeNonReturningFunc* chrom = static_cast<ChromosomeNonReturningFunc*>(listTestCase.at(i));

                stream << test+QString::number(i)+"():";

                for(auto ch : chrom->getGens())
                    stream << "\n\t"+ch;
            }
        }
        file.write(array);
        file.close();
    }
}
