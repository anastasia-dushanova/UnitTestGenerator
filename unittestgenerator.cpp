#include "unittestgenerator.h"
#include "ui_unittestgenerator.h"

UnitTestGenerator::UnitTestGenerator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UnitTestGenerator)
{
    ui->setupUi(this);


    TestCluster* cluster = new TestCluster("main.json");
    cluster->makeClusters();

    controller = new PopulationsController(12);

    connect(controller, SIGNAL(signalWrite(int,QString)), this, SLOT(slotWriteMessage(int,QString)));


    controller->checkFiles();
    controller->initPopulation();
}


UnitTestGenerator::~UnitTestGenerator()
{
    delete ui;
}


void UnitTestGenerator::slotWriteMessage(int index, QString message){
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
    default:{
        qDebug() << "Неизвестный индекс популяции";
        break;
    }
    }
}

