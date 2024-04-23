#ifndef UNITTESTGENERATOR_H
#define UNITTESTGENERATOR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class UnitTestGenerator; }
QT_END_NAMESPACE

class UnitTestGenerator : public QMainWindow
{
    Q_OBJECT

public:
    UnitTestGenerator(QWidget *parent = nullptr);
    ~UnitTestGenerator();

private:
    Ui::UnitTestGenerator *ui;
};
#endif // UNITTESTGENERATOR_H
