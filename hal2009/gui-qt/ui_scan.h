/********************************************************************************
** Form generated from reading ui file 'scan.ui'
**
** Created: Fri Apr 24 17:08:07 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SCAN_H
#define UI_SCAN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_Scan
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout1;
    QTextEdit *text;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Scan)
    {
        if (Scan->objectName().isEmpty())
            Scan->setObjectName(QString::fromUtf8("Scan"));
        Scan->resize(400, 300);
        gridLayout = new QGridLayout(Scan);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox = new QGroupBox(Scan);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout1 = new QGridLayout(groupBox);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        text = new QTextEdit(groupBox);
        text->setObjectName(QString::fromUtf8("text"));

        gridLayout1->addWidget(text, 0, 0, 1, 1);


        gridLayout->addWidget(groupBox, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(Scan);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 1, 0, 1, 1);


        retranslateUi(Scan);
        QObject::connect(buttonBox, SIGNAL(rejected()), Scan, SLOT(close()));

        QMetaObject::connectSlotsByName(Scan);
    } // setupUi

    void retranslateUi(QDialog *Scan)
    {
        Scan->setWindowTitle(QApplication::translate("Scan", "Scan a text", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("Scan", "Scan this text:", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Scan);
    } // retranslateUi

};

namespace Ui {
    class Scan: public Ui_Scan {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCAN_H
