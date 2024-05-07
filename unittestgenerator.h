#ifndef UNITTESTGENERATOR_H
#define UNITTESTGENERATOR_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QStandardItemModel>
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
    void slotWriteMessage(int index, const QString& message);

    void on_toolButton_addFile_clicked();

    void on_toolButton_deleteFile_clicked();

    void on_pushButton_clearPop_clicked();

    void on_pushButton_clearCurPop_clicked();

    void on_pushButton_clearAll_clicked();

    void on_toolButton_addFol_clicked();

    void on_action_addFol_triggered();

    void on_action_addFile_triggered();

    void on_action_exit_triggered();

    void on_action_help_triggered();

    void on_pushButton_start_clicked();

private:

    Ui::UnitTestGenerator *ui;

    PopulationsController* controller;
};
#endif // UNITTESTGENERATOR_H
