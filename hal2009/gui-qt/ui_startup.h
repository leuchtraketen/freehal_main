/********************************************************************************
** Form generated from reading ui file 'startup.ui'
**
** Created: Sun Feb 28 14:41:53 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_STARTUP_H
#define UI_STARTUP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_Startup
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *spacerItem;
    QLabel *procent;
    QLabel *label;

    void setupUi(QDialog *Startup)
    {
        if (Startup->objectName().isEmpty())
            Startup->setObjectName(QString::fromUtf8("Startup"));
        Startup->setWindowModality(Qt::ApplicationModal);
        Startup->resize(248, 39);
        QIcon icon;
        icon.addFile(QString::fromUtf8(""), QSize(), QIcon::Normal, QIcon::Off);
        Startup->setWindowIcon(icon);
        gridLayout = new QGridLayout(Startup);
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout->setMargin(9);
#endif
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacerItem, 0, 1, 1, 1);

        procent = new QLabel(Startup);
        procent->setObjectName(QString::fromUtf8("procent"));

        gridLayout->addWidget(procent, 0, 2, 1, 1);

        label = new QLabel(Startup);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);


        retranslateUi(Startup);

        QMetaObject::connectSlotsByName(Startup);
    } // setupUi

    void retranslateUi(QDialog *Startup)
    {
        Startup->setWindowTitle(QApplication::translate("Startup", "FreeHAL is loading...", 0, QApplication::UnicodeUTF8));
        procent->setText(QApplication::translate("Startup", "0%", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Startup", "FreeHAL is loading...", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Startup);
    } // retranslateUi

};

namespace Ui {
    class Startup: public Ui_Startup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARTUP_H
