#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QObject>
#include <QDialog>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>

class FindDialog : public QDialog
{
    Q_OBJECT
public:
    FindDialog(QWidget *parent = nullptr);

signals:
    void find(const QString &str, bool caseSensitive);
    void find_full(const QString &str, bool caseSensitive);
private slots:
    void FindButtonClickedSlot();
    void TextChangedSlot(const QString&);
private:
    QLabel *label;
    QLineEdit *lineEdit;
    QCheckBox *caseCheckBox;
    QCheckBox *FullWordCheckBox;
    QPushButton *findButton;
    QPushButton *closeButton;
};

#endif // FINDDIALOG_H
