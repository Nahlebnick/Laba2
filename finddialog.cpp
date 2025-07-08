#include "finddialog.h"
#include <qpushbutton.h>

FindDialog::FindDialog(QWidget *parent)
    : QDialog{parent}
{
    label = new QLabel(tr("Find what"), this);
    lineEdit = new QLineEdit(this);
    caseCheckBox = new QCheckBox(tr("Match &case"), this);
    FullWordCheckBox = new QCheckBox(tr("Search &full"), this);
    findButton = new QPushButton(tr("&Find"), this);
    findButton->setDefault(true);
    findButton->setEnabled(false);
    closeButton = new QPushButton(tr("Close"), this);

    QHBoxLayout *TextLayout = new QHBoxLayout;
    TextLayout->addWidget(label);
    TextLayout->addWidget(lineEdit);

    QVBoxLayout *LeftLayot = new QVBoxLayout;
    LeftLayot->addLayout(TextLayout);
    LeftLayot->addWidget(caseCheckBox);
    LeftLayot->addWidget(FullWordCheckBox);

    QVBoxLayout *RightLayout = new QVBoxLayout;
    RightLayout->addWidget(findButton);
    RightLayout->addWidget(closeButton);

    QHBoxLayout *MainLayout = new QHBoxLayout;
    MainLayout->setSpacing(6);
    MainLayout->addLayout(LeftLayot);
    MainLayout->addLayout(RightLayout);

    this->setLayout(MainLayout);

    connect(closeButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(TextChangedSlot(const QString&)));
    connect(findButton, SIGNAL(clicked(bool)), this, SLOT(FindButtonClickedSlot()));
}

void FindDialog::FindButtonClickedSlot()
{
    QString text = lineEdit->text();
    bool sens = caseCheckBox->isChecked();

    if (FullWordCheckBox->isChecked())
        emit find_full(text, sens);
    else
        emit find(text, sens);
}

void FindDialog::TextChangedSlot(const QString & text)
{
    findButton->setEnabled(!text.isEmpty());
}
