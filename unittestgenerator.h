#ifndef UNITTESTGENERATOR_H
#define UNITTESTGENERATOR_H

#include <QMainWindow>
#include <populationscontroller.h>
#include <testcluster.h>

QT_BEGIN_NAMESPACE
namespace Ui { class UnitTestGenerator; }
QT_END_NAMESPACE

class UnitTestGenerator : public QMainWindow
{
    Q_OBJECT

public:
    UnitTestGenerator(QWidget *parent = nullptr);
    ~UnitTestGenerator();

private slots:
    void slotWriteMessage(int index, QString message);

private:

    Ui::UnitTestGenerator *ui;

    PopulationsController* controller;
};
#endif // UNITTESTGENERATOR_H
