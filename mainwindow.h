#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRandomGenerator>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QFileDialog>
#include "database.h"
#include "finddialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

    void on_pushButton_3_clicked();

    void on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_listWidget_currentRowChanged(int currentRow);

    void on_NewPersonButton_clicked();

    void on_DeletePersonButton_clicked();

    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_SortButton_clicked();

    void on_FindButton_clicked();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_2_triggered();

    void FindPerson(const QString&, bool, qint16);

    void FindFullPerson(const QString&, bool, qint16);

    void on_actionAdd_Person_triggered();

    void on_SortTypeComboBox_currentIndexChanged(int index);

private:
    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFileName(const QString&);
    void fillTable();
    void addPersonToUi(const Person&);
    void setupTable();
    void updateDeleteButtonState();
    void showErrorMessage(const QString& message);

    Ui::MainWindow *ui;

    DataBase<Person> m_db;
    FindDialog* findDialog;
    int m_current_person;
    bool m_modified;
    bool m_sort_flag;
    QString m_current_file;

};
#endif // MAINWINDOW_H
