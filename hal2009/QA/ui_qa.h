/********************************************************************************
** Form generated from reading ui file 'qa.ui'
**
** Created: Thu May 28 20:49:18 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_QA_H
#define UI_QA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTextBrowser>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QAClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QTextBrowser *input;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QTextBrowser *object;
    QPushButton *check;
    QProgressBar *bar;
    QComboBox *language;
    QComboBox *programming_language;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QAClass)
    {
        if (QAClass->objectName().isEmpty())
            QAClass->setObjectName(QString::fromUtf8("QAClass"));
        QAClass->resize(415, 286);
        centralWidget = new QWidget(QAClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout_3 = new QGridLayout(centralWidget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setMargin(11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setMargin(11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        input = new QTextBrowser(groupBox);
        input->setObjectName(QString::fromUtf8("input"));
        QFont font;
        font.setFamily(QString::fromUtf8("Monospace"));
        input->setFont(font);
        input->setReadOnly(false);
        input->setAcceptRichText(false);

        gridLayout->addWidget(input, 0, 0, 1, 1);


        gridLayout_3->addWidget(groupBox, 1, 0, 1, 1);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setMargin(11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        object = new QTextBrowser(groupBox_2);
        object->setObjectName(QString::fromUtf8("object"));
        object->setFont(font);

        gridLayout_2->addWidget(object, 0, 0, 1, 1);


        gridLayout_3->addWidget(groupBox_2, 1, 1, 1, 1);

        check = new QPushButton(centralWidget);
        check->setObjectName(QString::fromUtf8("check"));

        gridLayout_3->addWidget(check, 3, 0, 1, 1);

        bar = new QProgressBar(centralWidget);
        bar->setObjectName(QString::fromUtf8("bar"));
        bar->setValue(0);

        gridLayout_3->addWidget(bar, 3, 1, 1, 1);

        language = new QComboBox(centralWidget);
        language->setObjectName(QString::fromUtf8("language"));

        gridLayout_3->addWidget(language, 2, 0, 1, 1);

        programming_language = new QComboBox(centralWidget);
        programming_language->setObjectName(QString::fromUtf8("programming_language"));

        gridLayout_3->addWidget(programming_language, 2, 1, 1, 1);

        QAClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(QAClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QAClass->setStatusBar(statusBar);

        retranslateUi(QAClass);

        QMetaObject::connectSlotsByName(QAClass);
    } // setupUi

    void retranslateUi(QMainWindow *QAClass)
    {
        QAClass->setWindowTitle(QApplication::translate("QAClass", "FreeHAL quality assurance", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("QAClass", "Input", 0, QApplication::UnicodeUTF8));
        input->setHtml(QApplication::translate("QAClass", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Monospace'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'DejaVu Sans';\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("QAClass", "Output", 0, QApplication::UnicodeUTF8));
        check->setText(QApplication::translate("QAClass", "Check", 0, QApplication::UnicodeUTF8));
        language->clear();
        language->insertItems(0, QStringList()
         << QApplication::translate("QAClass", "de", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QAClass", "en", 0, QApplication::UnicodeUTF8)
        );
        programming_language->clear();
        programming_language->insertItems(0, QStringList()
         << QApplication::translate("QAClass", "perl5", 0, QApplication::UnicodeUTF8)
        );
        Q_UNUSED(QAClass);
    } // retranslateUi

};

namespace Ui {
    class QAClass: public Ui_QAClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QA_H
