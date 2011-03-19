/********************************************************************************
** Form generated from reading UI file 'view.ui'
**
** Created: Fri Mar 18 19:39:36 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEW_H
#define UI_VIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_ViewDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QSpacerItem *spacerItem;
    QPushButton *weiter;
    QSpacerItem *spacerItem1;

    void setupUi(QDialog *ViewDialog)
    {
        if (ViewDialog->objectName().isEmpty())
            ViewDialog->setObjectName(QString::fromUtf8("ViewDialog"));
        ViewDialog->resize(700, 403);
        gridLayout = new QGridLayout(ViewDialog);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(ViewDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setPixmap(QPixmap(QString::fromUtf8(":/ui/view.png")));
        label->setMargin(0);

        gridLayout->addWidget(label, 0, 0, 1, 3);

        spacerItem = new QSpacerItem(581, 43, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacerItem, 1, 0, 1, 1);

        weiter = new QPushButton(ViewDialog);
        weiter->setObjectName(QString::fromUtf8("weiter"));
        weiter->setMinimumSize(QSize(100, 0));

        gridLayout->addWidget(weiter, 1, 1, 1, 1);

        spacerItem1 = new QSpacerItem(16, 43, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(spacerItem1, 1, 2, 1, 1);


        retranslateUi(ViewDialog);

        QMetaObject::connectSlotsByName(ViewDialog);
    } // setupUi

    void retranslateUi(QDialog *ViewDialog)
    {
        ViewDialog->setWindowTitle(QApplication::translate("ViewDialog", "FreeHAL", 0, QApplication::UnicodeUTF8));
        ViewDialog->setStyleSheet(QString());
        label->setStyleSheet(QString());
        label->setText(QString());
        weiter->setText(QApplication::translate("ViewDialog", "Next", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ViewDialog: public Ui_ViewDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEW_H
