#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setFixedSize(800, 600);
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Фамилия") << tr("Должность") << tr("Зарплата"));
    current_person = -1;

    ui->FileToolBar->addAction(ui->actionNew);
    ui->FileToolBar->addAction(ui->actionOpen);
    ui->FileToolBar->addAction(ui->actionSave);
    ui->FileToolBar->addAction(ui->actionSave_2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Добавление новой строки в конец таблицы и списка
void MainWindow::on_NewPersonButton_clicked()
{
    Person tmp;
    tmp.name = QString("New Person");
    m_db.push(tmp);



    int i = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(i);
    //QTableWidgetItem *item = new QTableWidgetItem(tmp.name);
    //ui->tableWidget->setItem(i, 0, item);
    //ui->listWidget->addItem(m_db[current_person].name);

    ui->listWidget->addItem(tmp.name);

    if (!ui->DeletePersonButton->isEnabled())
    {
        ui->DeletePersonButton->setEnabled(true);
    }
}

//Удаление последней строки из конца строки и списка
void MainWindow::on_DeletePersonButton_clicked()
{
    current_person = m_db.get_size()-1;

    m_db.pop();
    ui->tableWidget->removeRow(current_person);
    auto *pItem = ui->listWidget->takeItem(current_person+1);
    if (pItem)
    {
        qDebug() << pItem->text();
        delete pItem;
    }

    //ui->listWidget->setCurrentRow(current_person);
    //ui->tableWidget->setCurrentCell(current_person, 0);
    if (m_db.get_size() < 1) ui->DeletePersonButton->setEnabled(false);
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_pushButton_3_clicked()
{
    m_db.show();
    qDebug() << m_db.get_size();
}

void MainWindow::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    current_person = currentRow;
    ui->listWidget->setCurrentRow(current_person);
}

void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    current_person = currentRow;
    ui->tableWidget->setCurrentCell(currentRow, 0);
}

void MainWindow::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    if (item->column() == 0)
    {
        m_db[item->row()].name = item->text();
        QListWidgetItem* list_item = ui->listWidget->item(current_person);
        list_item->setText(item->text());
    }
    else if (item->column() == 1)
    {
        m_db[item->row()].Job = item->text();
    }
    else if (item->column() == 2)
    {
        m_db[item->row()].salary = item->text().toInt();
    }
}

void MainWindow::on_SortButton_clicked()
{

}

void MainWindow::on_FindButton_clicked()
{
    FindDialog* find_dialog = new FindDialog;
    find_dialog->exec();
}

