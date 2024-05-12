#ifndef UNITTESTGENERATOR_H
#define UNITTESTGENERATOR_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QStandardItemModel>
#include <QDateTime>
#include <QMessageBox>
#include <QScrollBar>
#include <QProcess>
#include <populationscontroller.h>
#include <testcluster.h>

QT_BEGIN_NAMESPACE
namespace Ui { class UnitTestGenerator; }
QT_END_NAMESPACE

/*!
 * \brief Генератор модульных тестов
 */
class UnitTestGenerator : public QMainWindow
{
    Q_OBJECT

public:

    /*!
     * \brief Конструктор класса
     * \param parent Родитель
     */
    UnitTestGenerator(QWidget *parent = nullptr);

    /*!
     * \brief Деструктор класса
     */
    ~UnitTestGenerator();

private slots:

    /*!
     * \brief Слот вывода сообщения на поле определенной популяции
     * \param index Индекс популяции
     * \param message Сообщение для вывода
     */
    void slotWriteMessage(int index, const QString& message);

    /*!
     * \brief Слот вывода времени работы
     * \param time Время
     */
    void slotTimeElapsed(int& time);

    /*!
     * \brief Слот вывода количества итераций на progressBar
     */
    void slotIterations();

    /*!
     * \brief Слот по окончанию работы алгоритма
     * \param coveraged Покрытых функций
     * \param total Общее кол-во функций
     */
    void slotFinish(int coveraged, int total);

    /*!
     * \brief Слот прочитать вывод сообщения от Python-анализатора
     */
    void slotReadPythonAnalys();

    /*!
     * \brief Слот на добавление файлов
     */
    void on_toolButton_addFile_clicked();

    /*!
     * \brief Слот на удаление файла
     */
    void on_toolButton_deleteFile_clicked();

    /*!
     * \brief Слот на очистку основного поля
     */
    void on_pushButton_clearPop_clicked();

    /*!
     * \brief Слот на очистку поля для выбранной популяции
     */
    void on_pushButton_clearCurPop_clicked();

    /*!
     * \brief Слот на очистку основных полей
     */
    void on_pushButton_clearAll_clicked();

    /*!
     * \brief Слот на добавление папки
     */
    void on_toolButton_addFol_clicked();

    /*!
     * \brief Слот на добавление папки
     */
    void on_action_addFol_triggered();

    /*!
     * \brief Слот на добавление файла
     */
    void on_action_addFile_triggered();

    /*!
     * \brief Слот на выход из программы
     */
    void on_action_exit_triggered();

    /*!
     * \brief Слот на вывод справки
     */
    void on_action_help_triggered();

    /*!
     * \brief Слот на запуск
     */
    void on_pushButton_start_clicked();

    /*!
     * \brief Слот на показ файла с тест-кейсами
     */
    void on_pushButton_show_clicked();

private:

    /*!
     * \brief Собрать список файлов в treeView
     * \param parent Родитель (каталог)
     */
    void checkFiles(QStandardItem* parent);

    /*!
     * \brief Рекурсивно добавить каталоги и файлы в treeView
     * \param path Путь до файла/каталога
     * \param parent Родитель (каталог)
     */
    void addFolderContent(QString path, QStandardItem* parent);

    Ui::UnitTestGenerator *ui;

    /*!
     * \brief Контроллер популяций
     */
    PopulationsController* controller;

    /*!
     * \brief Процесс для запуска Python-анализатора
     */
    QProcess* process;

    /*!
     * \brief Список файлов в treeView
     */
    QStringList* listFiles;

};
#endif // UNITTESTGENERATOR_H
