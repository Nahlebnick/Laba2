#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRandomGenerator>
#include <QTableWidgetItem>
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

private:
    bool maybeSave();
    void loadFile(const QString &fileName);
    void saveFile(const QString &fileName);

    Ui::MainWindow *ui;

    DataBase<Person> m_db;

    Person person;
    int current_person;
};
#endif // MAINWINDOW_H
