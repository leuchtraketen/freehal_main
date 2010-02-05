/********************************************************************************
** Form generated from reading ui file 'help.ui'
**
** Created: Fri Feb 5 15:41:39 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_HELP_H
#define UI_HELP_H

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

class Ui_HelpDialog
{
public:
    QGridLayout *gridLayout;
    QPushButton *changelog;
    QSpacerItem *spacerItem;
    QPushButton *pushButton;
    QLabel *label_2;
    QLabel *label;

    void setupUi(QDialog *HelpDialog)
    {
        if (HelpDialog->objectName().isEmpty())
            HelpDialog->setObjectName(QString::fromUtf8("HelpDialog"));
        HelpDialog->resize(351, 368);
        QIcon icon;
        icon.addFile(QString::fromUtf8(""), QSize(), QIcon::Normal, QIcon::Off);
        HelpDialog->setWindowIcon(icon);
        gridLayout = new QGridLayout(HelpDialog);
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout->setMargin(9);
#endif
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        changelog = new QPushButton(HelpDialog);
        changelog->setObjectName(QString::fromUtf8("changelog"));

        gridLayout->addWidget(changelog, 2, 0, 1, 1);

        spacerItem = new QSpacerItem(51, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacerItem, 2, 1, 1, 1);

        pushButton = new QPushButton(HelpDialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout->addWidget(pushButton, 2, 2, 1, 1);

        label_2 = new QLabel(HelpDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_2, 1, 0, 1, 3);

        label = new QLabel(HelpDialog);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 0, 0, 1, 3);


        retranslateUi(HelpDialog);
        QObject::connect(pushButton, SIGNAL(clicked()), HelpDialog, SLOT(close()));

        QMetaObject::connectSlotsByName(HelpDialog);
    } // setupUi

    void retranslateUi(QDialog *HelpDialog)
    {
        HelpDialog->setWindowTitle(QApplication::translate("HelpDialog", "Help", 0, QApplication::UnicodeUTF8));
        changelog->setText(QApplication::translate("HelpDialog", "License", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("HelpDialog", "OK", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("HelpDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:600;\">Version:</span><span style=\" font-family:'Sans Serif'; font-size:9pt;\"><br />ABC<br /><br /></span><span style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:600;\">License:</span><span style=\" font-family:'Sans Serif'; font-size:9pt;\"><br />GNU General Public Licence<br /><br /></span><span style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:600;\">Developers:</span><span style=\" font-family:'Sans Serif'; font-size:9pt;\"><br />Tobias Schulz (Administr"
                        "ator)<br /></span><span style=\" font-family:'Sans Serif'; font-size:9pt; font-style:italic;\">weitere Entwickler finden Sie auf der Website...</span><span style=\" font-family:'Sans Serif'; font-size:9pt;\"><br /><br /></span><span style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:600;\">Website:</span><span style=\" font-family:'Sans Serif'; font-size:9pt;\"><br />http://www.freehal.org/</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("HelpDialog", "<font size=\"30\">ABC</font>", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(HelpDialog);
    } // retranslateUi

};

namespace Ui {
    class HelpDialog: public Ui_HelpDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELP_H
