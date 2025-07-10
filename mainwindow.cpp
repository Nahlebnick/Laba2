#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setFixedSize(800, 600);
    ui->setupUi(this);

    setupTable();
    m_current_person = -1;

    ui->SortTypeComboBox->addItem(QString("By name"));
    ui->SortTypeComboBox->addItem(QString("By salary"));

    ui->FileToolBar->addAction(ui->actionNew);
    ui->FileToolBar->addAction(ui->actionOpen);
    ui->FileToolBar->addAction(ui->actionSave);
    ui->FileToolBar->addAction(ui->actionSave_2);

    ui->EditToolBar->addAction(ui->actionAdd_Person);
    ui->EditToolBar->addAction(ui->actionRemove_Person);
    ui->EditToolBar->addAction(ui->actionSort);
    ui->EditToolBar->addAction(ui->actionFind);

    ui->pushButton_3->setVisible(false);
    ui->label->setText("Sort by");
    ui->label->setAlignment(Qt::AlignCenter);

    m_modified = false;
    m_sort_flag = true;
    findDialog = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupTable()
{
    QStringList headers;
    headers << tr("Фамилия") << tr("Должность") << tr("Зарплата");
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);
}

void MainWindow::updateDeleteButtonState()
{
    bool hasData = m_db.get_size() > 0;
    ui->DeletePersonButton->setEnabled(hasData);
}

void MainWindow::showErrorMessage(const QString &message)
{
    QMessageBox::warning(this, tr("Application"), message);
}

void MainWindow::SortBySalaryDown()
{
    Person tmp;
    long i, j;
    for ( i=0; i < m_db.get_size(); i++)
    {  // цикл проходов, i - номер прохода
        tmp = m_db[i];
        // поиск места элемента в готовой 		последовательности
        for ( j=i-1; j>=0 && m_db[j].salary < tmp.salary; j--)
            m_db[j+1] = m_db[j];
                // сдвигаем элемент направо
        m_db[j+1] = tmp; // вставка элемента
    }
}

void MainWindow::SortBySalaryUp()
{
    Person tmp;
    long i, j;
    for ( i=0; i < m_db.get_size(); i++)
    {  // цикл проходов, i - номер прохода
        tmp = m_db[i];
        // поиск места элемента в готовой 		последовательности
        for ( j=i-1; j>=0 && m_db[j].salary > tmp.salary; j--)
            m_db[j+1] = m_db[j];
                // сдвигаем элемент направо
        m_db[j+1] = tmp; // вставка элемента
    }
}

//Find person by Full name
void MainWindow::FindFullPerson(const QString & name, bool caseSensitive, qint16& startInd)
{
    bool found = false;
    int row;
    for (int i = 0; i < m_db.get_size(); i++)
    {
        int x;
        if (caseSensitive) x = QString::compare(name, m_db[i].name, Qt::CaseSensitive);
        else x = QString::compare(name, m_db[i].name, Qt::CaseInsensitive);
        if (x == 0)
        {
            qDebug() << "Jepa";
            row = i;
            found = true;
            break;
        }
    }

    if (found)
    {
        ui->tableWidget->setCurrentCell(row, 0);
        ui->listWidget->setCurrentRow(row);
        m_current_person = row;
        QMessageBox::information(this, tr("Application"), tr("Person found at string %1").arg(QString::number(row+1)));
    }
    else
    {
        QMessageBox::information(this, tr("Application"), tr("Person not found"));
    }
    findDialog->raise();
}

//Find person
void MainWindow::FindPerson(const QString &name, bool caseSensitive, qint16& startInd)
{
    bool found = false;
    int row;
    for (int i = 0; i < m_db.get_size(); i++)
    {
        if (caseSensitive) found = m_db[i].name.contains(name);
        else found = m_db[i].name.contains(name, Qt::CaseInsensitive);
        if (found)
        {
            row = i;
            break;
        }
    }

    if (found)
    {
        ui->tableWidget->setCurrentCell(row, 0);
        ui->listWidget->setCurrentRow(row);
        m_current_person = row;
        QMessageBox::information(this, tr("Application"), tr("Person found at string %1").arg(QString::number(row+1)));
    }
    else
    {
        QMessageBox::information(this, tr("Application"), tr("Person not found"));
    }
    findDialog->raise();
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

    QListWidgetItem* pItem = ui->listWidget->takeItem(row);
    if (pItem)
    {
        delete pItem;
    }

    m_current_person = qMin(row, m_db.get_size() - 1);
    updateDeleteButtonState();
}

void MainWindow::on_actionExit_triggered()
{
    if (maybeSave()) QApplication::quit();
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
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    setupTable();

    ui->listWidget->clear();
    if (m_sort_flag)
    {
        m_sort_flag = false;
        if (ui->SortTypeComboBox->currentIndex() == 0) m_db.sort_db_up();
        else SortBySalaryUp();
    }
    else
    {
        m_sort_flag = true;
        if (ui->SortTypeComboBox->currentIndex() == 0) m_db.sort_db_down();
        else SortBySalaryDown();
    }
    fillTable();
}

void MainWindow::on_FindButton_clicked()
{
    if (!findDialog)
    {
        findDialog = new FindDialog;
        connect(findDialog, SIGNAL(find(QString,bool,qint16&)), this, SLOT(FindPerson(QString,bool,qint16&)));
        connect(findDialog, SIGNAL(find_full(QString,bool,qint16&)), this, SLOT(FindFullPerson(QString,bool,qint16&)));
    }
    findDialog->show();
    findDialog->raise();
    //delete findDialog;
}

void MainWindow::on_actionNew_triggered()
{
    if (maybeSave())
    {
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
        setupTable();

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
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), QString(), tr("db files (*.db)"));
        if (!fileName.isEmpty())
        {
            ui->tableWidget->clear();
            ui->tableWidget->setRowCount(0);
            setupTable();
            ui->listWidget->clear();
            m_db.clear();
            qDebug() << 11;
            loadFile(fileName);
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (m_current_file.isEmpty())
    {
        on_actionSave_2_triggered();
        return;
    }
    if (saveFile(m_current_file)) {
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
        statusBar()->showMessage(tr("File loaded"), 2000);
        updateDeleteButtonState();
        fillTable();
        m_modified = false;
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

    const QString name = person.name;
    const QString Job = person.Job;
    const qint16 salary = person.salary;

    // Добавляем элементы в таблицу
    /*QTableWidgetItem* nameItem = new QTableWidgetItem;
    QTableWidgetItem* jobItem = new QTableWidgetItem;
    QTableWidgetItem* salaryItem = new QTableWidgetItem;

    nameItem->setText(name);
    jobItem->setText(Job);
    salaryItem->setText(QString::number(salary));
    ui->tableWidget->setItem(row, 0, nameItem);
    ui->tableWidget->setItem(row, 1, jobItem);
    ui->tableWidget->setItem(row, 2, salaryItem);*/

    // Добавляем в список
    ui->listWidget->addItem(person.name);

    // Обновляем текущую выбранную персону
    m_current_person = row;
}
