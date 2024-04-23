#include "unittestgenerator.h"
#include "ui_unittestgenerator.h"

UnitTestGenerator::UnitTestGenerator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UnitTestGenerator)
{
    ui->setupUi(this);
}

UnitTestGenerator::~UnitTestGenerator()
{
    delete ui;
}

