#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Фамилия") << tr("Должность") << tr("Зарплата"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Добавленіе новой строки
void MainWindow::on_pushButton_clicked()
{
    int i = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(i);
}

//Удаление последней строки
void MainWindow::on_pushButton_2_clicked()
{
    int i = ui->tableWidget->rowCount();
    ui->tableWidget->removeRow(i-1);
}

