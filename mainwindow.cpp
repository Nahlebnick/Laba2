#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setFixedSize(800, 600);
    ui->setupUi(this);

    QStringList headers;
    headers << tr("Фамилия") << tr("Должность") << tr("Зарплата");
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    m_current_person = -1;

    ui->FileToolBar->addAction(ui->actionNew);
    ui->FileToolBar->addAction(ui->actionOpen);
    ui->FileToolBar->addAction(ui->actionSave);
    ui->FileToolBar->addAction(ui->actionSave_2);

    m_modified = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Добавление новой строки в конец таблицы и списка
void MainWindow::on_NewPersonButton_clicked()
{
    m_modified = true;
    Person newPerson;
    newPerson.name = QString("New Person");
    m_db.push(newPerson);

    addPersonToUi(newPerson);
    updateDeleteButtonState();
}

//Удаление последней строки из конца строки и списка
void MainWindow::on_DeletePersonButton_clicked()
{
    if (m_db.get_size() == 0)
        return;

    int row = m_db.get_size() - 1;
    m_modified = true;

    m_db.pop();


    ui->tableWidget->removeRow(row);
    //ui->listWidget->setCurrentRow(row);
    QListWidgetItem *pItem = ui->listWidget->takeItem(row);
    if (pItem)
    {
        delete pItem;
    }
    m_current_person = qMin(row, m_db.get_size());

    updateDeleteButtonState();
}

void MainWindow::on_actionExit_triggered()
{
    if (maybeSave())
    {
        QApplication::quit();
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    m_db.show();
    qDebug() << m_db.get_size();
}

void MainWindow::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    m_current_person = currentRow;
    ui->listWidget->setCurrentRow(m_current_person);
}

void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    m_current_person = currentRow;
    ui->tableWidget->setCurrentCell(currentRow, 0);
}

void MainWindow::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    m_modified = true;
    Person& person = m_db[m_current_person];

    switch (item->column()) {
    case 0: // Name
        person.name = item->text();
        ui->listWidget->item(m_current_person)->setText(item->text());
        break;
    case 1: // Job
        person.Job = item->text();
        break;
    case 2: // Salary
        person.salary = item->text().toInt();
        break;
    }
}

void MainWindow::on_SortButton_clicked()
{
    //m_db.sort_db();
}

void MainWindow::on_FindButton_clicked()
{
    FindDialog* find_dialog = new FindDialog;
    find_dialog->exec();
    delete find_dialog;
}

void MainWindow::on_actionNew_triggered()
{
    if (maybeSave())
    {
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
        QStringList headers;
        headers << tr("Фамилия") << tr("Должность") << tr("Зарплата");
        ui->tableWidget->setColumnCount(headers.size());
        ui->tableWidget->setHorizontalHeaderLabels(headers);

        ui->listWidget->clear();
        m_db.clear();

        m_current_person = -1;
        m_modified = false;
        m_current_file.clear();
        setWindowTitle(tr("untitled.db[*] - Data Base"));
        updateDeleteButtonState();
    }
}

void MainWindow::on_actionOpen_triggered()
{
    if (maybeSave())
    {
        //ui->tableWidget->clear();
        //ui->tableWidget->setColumnCount(3);
        //ui->tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Фамилия") << tr("Должность") << tr("Зарплата"));
        //ui->listWidget->clear()
        m_db.clear();
        qDebug() << 11;
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), QString(), tr("db files (*.db)"));
        if (!fileName.isEmpty()) loadFile(fileName);
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (m_current_file.isEmpty())
    {
        on_actionSave_2_triggered();
        return;
    }
    if (saveFile(m_current_file))
    {
        m_modified = false;
        statusBar()->showMessage(tr("File saved"), 2000);
    }
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
    if (m_modified)
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                                   tr("The document has been m_modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard
                                       | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
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
        showErrorMessage(tr("Cannot find file %1").arg(fileName));
        return;
    }

    if (m_db.readFromFile(fileName))
    {
        setCurrentFileName(fileName);
        fillTable();
        statusBar()->showMessage(tr("File loaded"), 2000);
        m_modified = false;
        updateDeleteButtonState();
    }
    else
    {
        showErrorMessage(tr("Failed to load file %1").arg(fileName));
    }
}

bool MainWindow::saveFile(const QString &fileName)
{
    if (m_db.writeIntoFile(fileName)) return true;
    else
    {
        showErrorMessage(tr("Failed to save file %1").arg(fileName));
        return false;
    }
}

void MainWindow::setCurrentFileName(const QString &filename)
{
    m_current_file = filename;
    m_modified = false;
    QString shownName;
    if (m_current_file.isEmpty())
    {
        shownName = "untitled.db";
    }
    else
    {
        shownName = QFileInfo(m_current_file).fileName();
    }
    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Data Base")));
    setWindowModified(false);
}

void MainWindow::fillTable()
{
    ui->tableWidget->setRowCount(0);
    ui->listWidget->clear();

    int size = m_db.get_size();
    for (int i = 0; i < size; i++)
    {
        addPersonToUi(m_db[i]);
    }
    if (m_db.get_size() > 0) m_current_person = 0;
}

void MainWindow::addPersonToUi(const Person & person)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    // Добавляем элементы в таблицу
    QTableWidgetItem* nameItem = new QTableWidgetItem(person.name);
    QTableWidgetItem* jobItem = new QTableWidgetItem(person.Job);
    QTableWidgetItem* salaryItem = new QTableWidgetItem(QString::number(person.salary));

    //qDebug() << "Do etogo momenta normalno";
    //ui->tableWidget->setItem(row, 0, nameItem);
    //ui->tableWidget->setItem(row, 1, jobItem);
    ui->tableWidget->setItem(row, 2, salaryItem);
    //qDebug() << "I posle toje vse normalno";

    ui->listWidget->addItem(person.name);

    m_current_person = row;
}

void MainWindow::updateDeleteButtonState()
{
    if (m_db.get_size() > 0) ui->DeletePersonButton->setEnabled(true);
    else ui->DeletePersonButton->setEnabled(false);
}

void MainWindow::showErrorMessage(const QString & message)
{
    QMessageBox::warning(this, tr("Application"), message);
}
