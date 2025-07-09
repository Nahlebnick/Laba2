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

    modified = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Добавление новой строки в конец таблицы и списка
void MainWindow::on_NewPersonButton_clicked()
{
    modified = true;
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
    modified = true;
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
    modified = true;
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
    m_db.s
}

void MainWindow::on_FindButton_clicked()
{
    FindDialog* find_dialog = new FindDialog;
    find_dialog->exec();
}

void MainWindow::on_actionNew_triggered()
{
    if (maybeSave())
    {
        ui->tableWidget->clear();
        ui->tableWidget->setRowCount(0);
        ui->tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Фамилия") << tr("Должность") << tr("Зарплата"));
        ui->listWidget->clear();
        m_db.clear();
    }
}

void MainWindow::on_actionOpen_triggered()
{
    if (maybeSave())
    {
        ui->tableWidget->clear();
        ui->tableWidget->setRowCount(0);
        ui->tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Фамилия") << tr("Должность") << tr("Зарплата"));
        ui->listWidget->clear();
        m_db.clear();
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty()) loadFile(fileName);
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (currFile.isEmpty())
    {
        on_actionSave_2_triggered();
        return;
    }
    modified = false;
    saveFile(currFile);
}

void MainWindow::on_actionSave_2_triggered()
{
    QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."), QString(), tr("db files (*.db)"));
    if (fn.isEmpty()) return;
    if (! (fn.endsWith(".db", Qt::CaseInsensitive))) fn += ".db";
    setCurrentFileName(fn);
    on_actionSave_triggered();
}

bool MainWindow::maybeSave()
{
    if (modified)
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                                   tr("The document has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard
                                       | QMessageBox::Cancel);
        if (ret == QMessageBox::Yes)
        {
            on_actionSave_triggered();
            return true;
        }
        else if (ret == QMessageBox::Cancel)
            return false;
    } return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    if (!QFile::exists(fileName))
    {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot find file %1:\n%2.")
                                 .arg(fileName));
        return;
    }
    m_db.readFromFile(fileName);
    setCurrentFileName(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
    fillTable();
}

bool MainWindow::saveFile(const QString &fileName)
{
    if (m_db.writeIntoFile(fileName)) return true;
    else return false;
}

void MainWindow::setCurrentFileName(const QString &filename)
{
    currFile = filename;
    modified = false;
    QString shownName;
    if (currFile.isEmpty())
    {
        shownName = "untitled.db";
    }
    else
    {
        shownName = QFileInfo(currFile).fileName();
    }
    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Data Base")));
    setWindowModified(false);
}

void MainWindow::fillTable()
{
    int size = m_db.get_size();
    ui->tableWidget->setRowCount(size);
    for (int i = 0; i < size; i++)
    {
        qDebug() << m_db[i];
        //ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString("Person")));
        ui->listWidget->addItem(m_db[i].name);
    }
    current_person = 0;
}

