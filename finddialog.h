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
    void find(const QString &str, bool caseSensitive, qint16&);
    void find_full(const QString &str, bool caseSensitive, qint16&);
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
    qint16 prevInd = 0;
};

#endif // FINDDIALOG_H
