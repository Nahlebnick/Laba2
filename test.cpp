#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_currentPerson(-1)
    , m_modified(false)
{
    initializeUI();
    setupConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeUI()
{
    ui->setupUi(this);
    setFixedSize(800, 600);
    
    // Настройка таблицы
    setupTable();
    
    // Настройка тулбара
    setupToolbar();
    
    // Инициализация состояния кнопок
    updateUiState();
}

void MainWindow::setupTable()
{
    QStringList headers;
    headers << tr("Фамилия") << tr("Должность") << tr("Зарплата");
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);
}

void MainWindow::setupToolbar()
{
    ui->FileToolBar->addAction(ui->actionNew);
    ui->FileToolBar->addAction(ui->actionOpen);
    ui->FileToolBar->addAction(ui->actionSave);
    ui->FileToolBar->addAction(ui->actionSave_2);
}

void MainWindow::setupConnections()
{
    // Соединение сигналов и слотов
    connect(ui->tableWidget, &QTableWidget::currentCellChanged, 
            this, &MainWindow::handleTableSelectionChange);
    connect(ui->listWidget, &QListWidget::currentRowChanged, 
            this, &MainWindow::handleListSelectionChange);
    connect(ui->tableWidget, &QTableWidget::itemChanged, 
            this, &MainWindow::handleTableItemChange);
}

void MainWindow::updateUiState()
{
    bool hasData = m_db.get_size() > 0;
    ui->DeletePersonButton->setEnabled(hasData);
}

// Добавление новой персоны
void MainWindow::on_NewPersonButton_clicked()
{
    m_modified = true;
    
    Person newPerson;
    newPerson.name = tr("New Person");
    m_db.push(newPerson);

    addPersonToUi(newPerson);
    updateUiState();
}

void MainWindow::addPersonToUi(const Person& person)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);
    
    // Добавляем элементы в таблицу
    QTableWidgetItem* nameItem = new QTableWidgetItem(person.name);
    QTableWidgetItem* jobItem = new QTableWidgetItem(person.Job);
    QTableWidgetItem* salaryItem = new QTableWidgetItem(QString::number(person.salary));
    
    ui->tableWidget->setItem(row, 0, nameItem);
    ui->tableWidget->setItem(row, 1, jobItem);
    ui->tableWidget->setItem(row, 2, salaryItem);
    
    // Добавляем в список
    ui->listWidget->addItem(person.name);
    
    // Обновляем текущую выбранную персону
    m_currentPerson = row;
}

// Удаление персоны
void MainWindow::on_DeletePersonButton_clicked()
{
    if (m_db.get_size() == 0)
        return;

    m_modified = true;
    int lastRow = m_db.get_size() - 1;
    
    // Удаляем из модели
    m_db.pop();
    
    // Удаляем из UI
    removePersonFromUi(lastRow);
    updateUiState();
}

void MainWindow::removePersonFromUi(int row)
{
    ui->tableWidget->removeRow(row);
    
    QListWidgetItem* item = ui->listWidget->takeItem(row);
    if (item) {
        delete item;
    }
    
    // Обновляем текущую выбранную персону
    m_currentPerson = qMin(row, m_db.get_size() - 1);
}

// Обработчики изменения выбора
void MainWindow::handleTableSelectionChange(int currentRow, int, int, int)
{
    m_currentPerson = currentRow;
    ui->listWidget->setCurrentRow(m_currentPerson);
}

void MainWindow::handleListSelectionChange(int currentRow)
{
    m_currentPerson = currentRow;
    ui->tableWidget->setCurrentCell(m_currentPerson, 0);
}

// Обработчик изменения данных в таблице
void MainWindow::handleTableItemChange(QTableWidgetItem *item)
{
    if (!item || m_currentPerson < 0 || m_currentPerson >= m_db.get_size())
        return;

    m_modified = true;
    Person& person = m_db[m_currentPerson];

    switch (item->column()) {
    case 0: // Name
        person.name = item->text();
        ui->listWidget->item(m_currentPerson)->setText(item->text());
        break;
    case 1: // Job
        person.Job = item->text();
        break;
    case 2: // Salary
        person.salary = item->text().toInt();
        break;
    }
}

// Файловые операции
void MainWindow::on_actionNew_triggered()
{
    if (confirmSaveBeforeAction()) {
        resetApplicationState();
    }
}

void MainWindow::on_actionOpen_triggered()
{
    if (confirmSaveBeforeAction()) {
        QString fileName = QFileDialog::getOpenFileName(this, 
            tr("Open file"), QString(), tr("db files (*.db)"));
        
        if (!fileName.isEmpty()) {
            loadFile(fileName);
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (m_currentFile.isEmpty()) {
        on_actionSave_2_triggered();
        return;
    }
    
    if (saveFile(m_currentFile)) {
        m_modified = false;
        statusBar()->showMessage(tr("File saved"), 2000);
    }
}

void MainWindow::on_actionSave_2_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, 
        tr("Save as..."), QString(), tr("db files (*.db)"));
    
    if (fileName.isEmpty()) 
        return;
    
    if (!fileName.endsWith(".db", Qt::CaseInsensitive)) 
        fileName += ".db";
    
    setCurrentFileName(fileName);
    on_actionSave_triggered();
}

bool MainWindow::confirmSaveBeforeAction()
{
    if (!m_modified)
        return true;

    QMessageBox::StandardButton ret = QMessageBox::warning(this, 
        tr("Application"),
        tr("The document has been modified.\nDo you want to save your changes?"),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    
    if (ret == QMessageBox::Save) {
        on_actionSave_triggered();
        return !m_modified; // Возвращаем true только если сохранение прошло успешно
    }
    
    return ret != QMessageBox::Cancel;
}

void MainWindow::resetApplicationState()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    setupTable();
    
    ui->listWidget->clear();
    m_db.clear();
    
    m_currentPerson = -1;
    m_modified = false;
    m_currentFile.clear();
    
    setWindowTitle(tr("untitled.db[*] - Data Base"));
    updateUiState();
}

void MainWindow::loadFile(const QString &fileName)
{
    if (!QFile::exists(fileName)) {
        showErrorMessage(tr("Cannot find file %1").arg(fileName));
        return;
    }

    if (m_db.readFromFile(fileName)) {
        setCurrentFileName(fileName);
        fillTableFromModel();
        statusBar()->showMessage(tr("File loaded"), 2000);
        m_modified = false;
        updateUiState();
    } else {
        showErrorMessage(tr("Failed to load file %1").arg(fileName));
    }
}

void MainWindow::fillTableFromModel()
{
    ui->tableWidget->setRowCount(0);
    ui->listWidget->clear();
    
    for (int i = 0; i < m_db.get_size(); ++i) {
        addPersonToUi(m_db[i]);
    }
    
    if (m_db.get_size() > 0) {
        m_currentPerson = 0;
    }
}

bool MainWindow::saveFile(const QString &fileName)
{
    if (!m_db.writeIntoFile(fileName)) {
        showErrorMessage(tr("Failed to save file %1").arg(fileName));
        return false;
    }
    return true;
}

void MainWindow::setCurrentFileName(const QString &fileName)
{
    m_currentFile = fileName;
    m_modified = false;
    
    QString shownName = fileName.isEmpty() ? 
        tr("untitled.db") : QFileInfo(fileName).fileName();
    
    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Data Base")));
    setWindowModified(false);
}

void MainWindow::showErrorMessage(const QString &message)
{
    QMessageBox::warning(this, tr("Application"), message);
}

// Прочие методы
void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_pushButton_3_clicked()
{
    m_db.show();
    qDebug() << "Current database size:" << m_db.get_size();
}

void MainWindow::on_SortButton_clicked()
{
    // Реализация сортировки
    // m_db.sort_db();
}

void MainWindow::on_FindButton_clicked()
{
    FindDialog* findDialog = new FindDialog(this);
    findDialog->exec();
    delete findDialog;
}