#include "unittestgenerator.h"
#include "ui_unittestgenerator.h"

UnitTestGenerator::UnitTestGenerator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UnitTestGenerator)
{
    ui->setupUi(this);

    ui->treeView_files->setHeaderHidden(true);
}


UnitTestGenerator::~UnitTestGenerator()
{
    delete ui;
}


void UnitTestGenerator::slotWriteMessage(int index, const QString& message){
    switch (index) {
    case 1:{
        ui->plainTextEdit_1->setPlainText(ui->plainTextEdit_1->toPlainText()+"\n"+message);
        break;
    }
    case 2:{
        ui->plainTextEdit_2->setPlainText(ui->plainTextEdit_2->toPlainText()+"\n"+message);
        break;
    }
    case 3:{
        ui->plainTextEdit_3->setPlainText(ui->plainTextEdit_3->toPlainText()+"\n"+message);
        break;
    }
    case 4:{
        ui->plainTextEdit_4->setPlainText(ui->plainTextEdit_4->toPlainText()+"\n"+message);
        break;
    }
    case 5:{
        ui->plainTextEdit_main->setPlainText(ui->plainTextEdit_main->toPlainText()+"\n"+message);
        break;
    }
    default:{
        qDebug() << "Неизвестный индекс популяции";
        break;
    }
    }
}


void UnitTestGenerator::on_toolButton_addFile_clicked()
{
    QStringList listFiles = QFileDialog::getOpenFileNames(this, "Выбирите файл", "", "");

    if(listFiles.isEmpty())
        return;

    QStandardItemModel* model;
    if(ui->treeView_files->model() == nullptr)
        model = new QStandardItemModel();
    else
        model = static_cast<QStandardItemModel*>(ui->treeView_files->model());

    QStandardItem* parentItem = model->invisibleRootItem();

    for(auto l : listFiles){
        QStandardItem* item = new QStandardItem(QFileInfo(l).fileName());
        item->setData(l);
        item->setIcon(QIcon(":/src/file.png"));
        parentItem->appendRow(item);
    }

    ui->treeView_files->setModel(model);


    for(int row{0}; row < model->rowCount(); ++row){
        qDebug() << model->item(row)->data().toString();
    }
}


void UnitTestGenerator::on_toolButton_addFol_clicked()
{
    QString d = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
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

    QStringList list = dir.entryList(QDir::Files);
    for(auto name : list){
        QStandardItem* item = new QStandardItem(name);
        item->setData(d+"/"+name);
        item->setIcon(QIcon(":/src/file.png"));
        parentItem->appendRow(item);
    }
    ui->treeView_files->setModel(model);

    for(int row{0}; row < model->rowCount(); ++row){
        qDebug() << model->item(row)->data().toString();
    }

}

void UnitTestGenerator::on_toolButton_deleteFile_clicked()
{
    qDebug() << ui->treeView_files->selectionModel()->currentIndex().data().toString();

//    QStandardItemModel* model = static_cast<QStandardItemModel*>(ui->treeView_files->model());
//    for(int row{0}; row < model->rowCount(); ++row){
//        qDebug() << model->item(row)->data().toString();
//    }
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

}


void UnitTestGenerator::on_pushButton_start_clicked()
{
    float probMut = ui->comboBox_probMut->currentText().toFloat();
    float probCross = ui->spinBox_probCross->text().toFloat();
    int iter = ui->lineEdit_iterations->text().toInt();

    //здесь позже будет только Python-анализатор
    TestCluster* cluster = new TestCluster("main.json");
    cluster->makeClusters();

    controller = new PopulationsController(iter);
    controller->setProbMutation(probMut*0.01);
    controller->setProbCrossover(probCross*0.01);

    connect(controller, SIGNAL(signalWrite(int,const QString&)), this, SLOT(slotWriteMessage(int,const QString&)));


    controller->checkFiles();
    controller->initPopulation();
}

