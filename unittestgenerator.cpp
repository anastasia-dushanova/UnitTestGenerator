#include "unittestgenerator.h"
#include "ui_unittestgenerator.h"

UnitTestGenerator::UnitTestGenerator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UnitTestGenerator)
{
    ui->setupUi(this);

    ui->treeView_files->setHeaderHidden(true);

//    totalIterations = 0;

    listFiles = new QStringList();

    controller = new PopulationsController();

    connect(controller, SIGNAL(signalWrite(int,const QString&)), this, SLOT(slotWriteMessage(int,const QString&)));
    connect(controller, SIGNAL(signalTimeElapsed(int&)), this, SLOT(slotTimeElapsed(int&)));
    connect(controller, SIGNAL(signalIteration()), this, SLOT(slotIterations()));
    connect(controller, SIGNAL(signalFinish(int, int)), this, SLOT(slotFinish(int, int)));

    QFont font;
    font.setStyleHint(QFont::Times);
    ui->plainTextEdit_show->setFont(font);

    process = new QProcess(this);

//    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(slotReadPythonAnalys()));
    connect(process, SIGNAL(readyReadStandardError()), this, SLOT(slotReadPythonAnalys()));

    setAllButtonsEnabled(false);


}

void UnitTestGenerator::setAllButtonsEnabled(bool b){
    QString enable{""};
    ui->pushButton_show->setEnabled(b);
    ui->pushButton_edit->setEnabled(b);
    ui->pushButton_edit->setEnabled(b);
    ui->pushButton_start->setEnabled(b);
    ui->pushButton_save->setEnabled(b);

    if(b)
        enable = "background-color: #0094F5; border 1px solid #0d6efd";
    else
        enable = "background-color: #c1e6fe; border 1px solid #00d0f0";


    ui->pushButton_show->setStyleSheet(enable);
    ui->pushButton_edit->setStyleSheet(enable);
    ui->pushButton_edit->setStyleSheet(enable);
    ui->pushButton_start->setStyleSheet(enable);
    ui->pushButton_save->setStyleSheet(enable);
}


UnitTestGenerator::~UnitTestGenerator()
{
    delete ui;
}


void UnitTestGenerator::slotWriteMessage(int index, const QString& message){
    switch (index) {
    case 1:{
        ui->plainTextEdit_1->textCursor().movePosition(QTextCursor::End);
        ui->plainTextEdit_1->insertPlainText("\n"+message);
        ui->plainTextEdit_1->verticalScrollBar()->setValue(ui->plainTextEdit_1->verticalScrollBar()->maximum());
        break;
    }
    case 2:{
        ui->plainTextEdit_2->textCursor().movePosition(QTextCursor::End);
        ui->plainTextEdit_2->insertPlainText("\n"+message);
        ui->plainTextEdit_2->verticalScrollBar()->setValue(ui->plainTextEdit_2->verticalScrollBar()->maximum());
        break;
    }
    case 3:{
        ui->plainTextEdit_3->textCursor().movePosition(QTextCursor::End);
        ui->plainTextEdit_3->insertPlainText("\n"+message);
        ui->plainTextEdit_3->verticalScrollBar()->setValue(ui->plainTextEdit_3->verticalScrollBar()->maximum());
        break;
    }
    case 4:{
        ui->plainTextEdit_4->textCursor().movePosition(QTextCursor::End);
        ui->plainTextEdit_4->insertPlainText("\n"+message);
        ui->plainTextEdit_4->verticalScrollBar()->setValue(ui->plainTextEdit_4->verticalScrollBar()->maximum());
        break;
    }
    case 5:{
        ui->plainTextEdit_main->textCursor().movePosition(QTextCursor::End);
        ui->plainTextEdit_main->insertPlainText("\n"+message);
        ui->plainTextEdit_main->verticalScrollBar()->setValue(ui->plainTextEdit_main->verticalScrollBar()->maximum());
        break;
    }
    default:{
        qDebug() << "Неизвестный индекс популяции";
        break;
    }
    }
}

void UnitTestGenerator::slotTimeElapsed(int &time)
{
    ui->label_totalTimeValue->clear();
    ui->label_totalTimeValue->setText(QString::number(time) + " мс");
}

void UnitTestGenerator::slotIterations()
{
    ui->progressBar->setValue(ui->progressBar->value()+4);
}

void UnitTestGenerator::slotFinish(int coveraged, int total)
{
//    ui->progressBar->setValue(ui->progressBar->maximum());

    ui->label_statusValue->setStyleSheet("color: #008131");
    ui->label_statusValue->setText("Готово");


    float cover = ((float)coveraged/(float)total);
    qDebug() << "покрытие "<<cover;

    ui->label_coverageValue->clear();
    ui->label_coverageValue->setText(QString::number(cover, 'f', 2));

    ui->pushButton_show->setEnabled(true);

    //TODO удалить все популяции
    controller->deletePopulations();
    qDebug() << "pass1";
//    setAllButtonsEnabled(true);
    ui->pushButton_show->setEnabled(true);
    ui->pushButton_show->setStyleSheet("background-color: #0094F5; border 1px solid #0d6efd");
}

void UnitTestGenerator::on_toolButton_addFile_clicked()
{
    QStringList listFile = QFileDialog::getOpenFileNames(this, "Выбирите файл", "", "");

    if(listFile.isEmpty())
        return;

    QStandardItemModel* model;
    if(ui->treeView_files->model() == nullptr)
        model = new QStandardItemModel();
    else
        model = static_cast<QStandardItemModel*>(ui->treeView_files->model());

    QStandardItem* parentItem = model->invisibleRootItem();

    for(auto l : listFile){
        QStandardItem* item = new QStandardItem(QFileInfo(l).fileName());
        item->setData(l);
        item->setIcon(QIcon(":/src/file.png"));
        parentItem->appendRow(item);
    }

    ui->treeView_files->setModel(model);

    ui->pushButton_start->setEnabled(true);
    ui->pushButton_start->setStyleSheet("background-color: #0094F5; border 1px solid #0d6efd");
}


void UnitTestGenerator::on_toolButton_addFol_clicked()
{
    QString d = QFileDialog::getExistingDirectory(this, tr("Выбирите папку"),
                                                "",
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);

    if(d.isEmpty())
        return;

    QDir dir(d);
    if(dir.isEmpty())
        return;

    QStandardItemModel* model;
    if(ui->treeView_files->model() == nullptr)
        model = new QStandardItemModel();
    else
        model = static_cast<QStandardItemModel*>(ui->treeView_files->model());

    QStandardItem* parent = model->invisibleRootItem();
    QStandardItem* parentItem = new QStandardItem(dir.dirName());
    parentItem->setData(d);
    parentItem->setIcon(QIcon(":/src/folder.png"));
    parent->appendRow(parentItem);

    QStringList list = dir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    for(auto name : list){

        addFolderContent(d+"/"+name, parentItem);
    }
    ui->treeView_files->setModel(model);

    ui->pushButton_start->setEnabled(true);
    ui->pushButton_start->setStyleSheet("background-color: #0094F5; border 1px solid #0d6efd");

}

void UnitTestGenerator::addFolderContent(QString path, QStandardItem* parent){

    QFileInfo info(path);
    if(!info.isDir()){
//        qDebug() <<"file "<<path;
        QStandardItem* item = new QStandardItem(info.fileName());
        item->setData(info.filePath());
        item->setIcon(QIcon(":/src/file.png"));
        parent->appendRow(item);
    }else{
//        qDebug() << "dir "<<path;
        QDir dir(path);
        QStandardItem* parentItem = new QStandardItem(dir.dirName());
        parentItem->setData(path);
        parentItem->setIcon(QIcon(":/src/folder.png"));
        parent->appendRow(parentItem);
        QStringList list = dir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
        for(auto name : list){
            addFolderContent(path + "/" + name, parentItem);
        }
    }

}

void UnitTestGenerator::on_toolButton_deleteFile_clicked()
{
    if(ui->treeView_files->model() == nullptr)
        return;

    QItemSelectionModel* model = static_cast<QItemSelectionModel*>(ui->treeView_files->selectionModel());
    int row = model->currentIndex().row();

    if(row == -1)
        return;

    QModelIndex parent = model->currentIndex().parent();
    QStandardItemModel* standard = static_cast<QStandardItemModel*>(ui->treeView_files->model());
    standard->removeRow(row, parent);

}

void UnitTestGenerator::on_pushButton_clearPop_clicked()
{
    ui->plainTextEdit_main->clear();
}

void UnitTestGenerator::on_pushButton_clearCurPop_clicked()
{
    int index = ui->tabWidget_populations->currentIndex()+1;
    switch (index) {
    case 1:{
        ui->plainTextEdit_1->clear();
        break;
    }
    case 2:{
        ui->plainTextEdit_2->clear();
        break;
    }
    case 3:{
        ui->plainTextEdit_3->clear();
        break;
    }
    case 4:{
        ui->plainTextEdit_4->clear();
        break;
    }
    default:{
        qDebug() << "Неизвестный индекс популяции";
        break;
    }
    }
}

void UnitTestGenerator::on_pushButton_clearAll_clicked()
{
    ui->plainTextEdit_1->clear();
    ui->plainTextEdit_2->clear();
    ui->plainTextEdit_3->clear();
    ui->plainTextEdit_4->clear();
    ui->plainTextEdit_main->clear();
    ui->plainTextEdit_show->clear();

    if(ui->treeView_files->model() == nullptr)
        return;

    QStandardItemModel* model = static_cast<QStandardItemModel*>(ui->treeView_files->model());
    model->clear();

    ui->label_coverageValue->setText("");
    ui->label_statusValue->setText("");
    ui->label_totalTimeValue->setText("");
    ui->progressBar->setValue(0);

    setAllButtonsEnabled(false);
}


void UnitTestGenerator::on_action_addFol_triggered()
{
    on_toolButton_addFol_clicked();
}


void UnitTestGenerator::on_action_addFile_triggered()
{
    on_toolButton_addFile_clicked();
}


void UnitTestGenerator::on_action_exit_triggered()
{
    close();
}


void UnitTestGenerator::on_action_help_triggered()
{
//    QMessageBox::information();
}

void UnitTestGenerator::slotReadPythonAnalys(){

    QString output = process->readAllStandardError();
    qDebug() << "python-анализатор: "<<output;
    if(output != "all parsed"){
        QMessageBox::warning(this,
                             "Сообщение от Python-анализатора",
                             output);
        return;
    }
    float probMut = ui->comboBox_probMut->currentText().toFloat();
    float probCross = ui->spinBox_probCross->text().toFloat();
    int iter = ui->lineEdit_iterations->text().toInt();

    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(iter);

    ui->label_statusValue->setStyleSheet("color: rgb(0, 0, 0)");
    ui->label_statusValue->setText("Выполнение");

    TestCluster* cluster = new TestCluster("python/sw_templates.json");
//    TestCluster* cluster = new TestCluster("main.json");
    cluster->makeClusters();

    controller->setTotalIter(iter);
    controller->setProbMutation(probMut*0.01);
    controller->setProbCrossover(probCross*0.01);

    controller->checkFiles();
    controller->initPopulation();

    delete cluster;
}

void UnitTestGenerator::checkFiles(QStandardItem *parent){

//    qDebug() << parent->hasChildren();
//    if(!parent->hasChildren()){
//        qDebug() << "нет детей" << parent->data();
//        listFiles->append(parent->data().toString());
//    }
//    qDebug() << "есть дети "<<parent->data();
//    checkFiles(parent->child(0));

    if(parent->hasChildren()){
        for(auto row{0}; row < parent->rowCount(); ++row){
            checkFiles(parent->child(row));

        }
    }else{
//        qDebug() << parent->data();
        listFiles->append(parent->data().toString());
    }

}

void UnitTestGenerator::on_pushButton_start_clicked()
{
    if(ui->treeView_files->model() == nullptr)
        return;

    ui->plainTextEdit_main->textCursor().movePosition(QTextCursor::End);
    ui->plainTextEdit_main->insertPlainText(QDateTime::currentDateTime().time().toString("hh:mm:ss")+"\tЗапускается Python-анализатор...\n");

    QFile file(QApplication::applicationDirPath()+"/python/list_python_code.txt");
    file.open(QIODevice::WriteOnly|QIODevice::Truncate);
    file.close();

    if(!file.open(QIODevice::WriteOnly | QIODevice::Append)){
        qDebug() << "Невозможно открыть файл /python/list_python_code.txt";
        QMessageBox::warning(this,
                             "Внимание",
                             "Невозможно открыть файл /python/list_python_code.txt\nОн используется для Python-анализатора");
        return;
    }

    //получаем список файлов с кодом питона
    QStandardItemModel* model = static_cast<QStandardItemModel*>(ui->treeView_files->model());


    for(int row{0}; row < model->rowCount(); ++row){
        QStandardItem* parent = model->item(row);
        checkFiles(parent);
    }


    QTextStream stream(&file);
    //нужно оставить файлы только с питоном (.py)
    for(int i{0}; i < listFiles->size(); ++i){
        QFileInfo info(listFiles->at(i));
        if(info.completeSuffix() == "py"){
            //добавляем пути до файлов с кодом в list_python_code.txt
            stream << info.filePath().toUtf8() << "\n";
        }
    }


    file.close();

//    float probMut = ui->comboBox_probMut->currentText().toFloat();
//    float probCross = ui->spinBox_probCross->text().toFloat();
//    int iter = ui->lineEdit_iterations->text().toInt();
    //здесь позже будет только Python-анализатор
    QString scriptFile = QCoreApplication::applicationDirPath()+"/python/main.py";

    QStringList pythonCommandArguments = QStringList() << scriptFile;

    qDebug() << "запускаем питон";
    process->start("python", pythonCommandArguments);
    process->waitForFinished();


//    listFiles->clear();
//    TestCluster* cluster = new TestCluster("main.json");
//    cluster->makeClusters();

//    controller->setTotalIter(iter);
//    controller->setProbMutation(probMut*0.01);
//    controller->setProbCrossover(probCross*0.01);

//    controller->checkFiles();
//    controller->initPopulation();

//    delete cluster;
}


void UnitTestGenerator::on_pushButton_show_clicked()
{
    ui->plainTextEdit_show->clear();
    QFile file(QApplication::applicationDirPath()+"/decor.txt");
    if(file.open(QIODevice::ReadOnly)){
        QTextStream stream(&file);
        QString text = stream.readAll();
        ui->plainTextEdit_show->setPlainText(text);
    }
    file.close();

    ui->pushButton_edit->setEnabled(true);
    ui->pushButton_save->setEnabled(true);
    ui->pushButton_edit->setStyleSheet("background-color: #0094F5; border 1px solid #0d6efd");
    ui->pushButton_save->setStyleSheet("background-color: #0094F5; border 1px solid #0d6efd");
}

void UnitTestGenerator::on_pushButton_edit_clicked()
{
    ui->plainTextEdit_show->setReadOnly(false);
    ui->plainTextEdit_show->setStyleSheet("background-color: #fff");
}


void UnitTestGenerator::on_pushButton_save_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this,
                                 "Выбирите файл для сохранения",
                                 "");

    if(filePath.isEmpty())
        return;

    QFile file(filePath);
    file.open(QIODevice::WriteOnly|QIODevice::Truncate);
    file.close();
    if(!file.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this, "Внимание", "Невозможно открыть файл для сохранения");
        return;
    }
    QTextStream stream(&file);
    stream << ui->plainTextEdit_show->toPlainText();
    file.close();
    QMessageBox::information(this, "Файл сохранен", "Готово. Файл успешно сохранен");
}

