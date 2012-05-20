/********************************************************************************
** Form generated from reading ui file 'freehal_db.ui'
**
** Created: Sat Aug 22 14:34:01 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_FREEHAL_DB_H
#define UI_FREEHAL_DB_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableView>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FreeHAL_DBClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QLineEdit *subject;
    QLabel *label_4;
    QLineEdit *adverbs;
    QLabel *label_3;
    QLineEdit *verb;
    QLabel *label_5;
    QLineEdit *questionword;
    QLabel *label_2;
    QLabel *label_6;
    QLineEdit *extra;
    QPushButton *matchingfacts;
    QPushButton *allfacts;
    QTableView *tableView;
    QLineEdit *object;
    QWidget *tab_2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FreeHAL_DBClass)
    {
        if (FreeHAL_DBClass->objectName().isEmpty())
            FreeHAL_DBClass->setObjectName(QString::fromUtf8("FreeHAL_DBClass"));
        FreeHAL_DBClass->resize(715, 400);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(FreeHAL_DBClass->sizePolicy().hasHeightForWidth());
        FreeHAL_DBClass->setSizePolicy(sizePolicy);
        centralWidget = new QWidget(FreeHAL_DBClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setMargin(11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_2 = new QGridLayout(tab);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setMargin(11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        subject = new QLineEdit(tab);
        subject->setObjectName(QString::fromUtf8("subject"));

        gridLayout_2->addWidget(subject, 0, 1, 1, 1);

        label_4 = new QLabel(tab);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 0, 2, 1, 1);

        adverbs = new QLineEdit(tab);
        adverbs->setObjectName(QString::fromUtf8("adverbs"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(adverbs->sizePolicy().hasHeightForWidth());
        adverbs->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(adverbs, 0, 3, 1, 1);

        label_3 = new QLabel(tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 1, 0, 1, 1);

        verb = new QLineEdit(tab);
        verb->setObjectName(QString::fromUtf8("verb"));

        gridLayout_2->addWidget(verb, 1, 1, 1, 1);

        label_5 = new QLabel(tab);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 1, 2, 1, 1);

        questionword = new QLineEdit(tab);
        questionword->setObjectName(QString::fromUtf8("questionword"));
        sizePolicy1.setHeightForWidth(questionword->sizePolicy().hasHeightForWidth());
        questionword->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(questionword, 1, 3, 1, 1);

        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 2, 0, 1, 1);

        label_6 = new QLabel(tab);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 2, 2, 1, 1);

        extra = new QLineEdit(tab);
        extra->setObjectName(QString::fromUtf8("extra"));
        sizePolicy1.setHeightForWidth(extra->sizePolicy().hasHeightForWidth());
        extra->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(extra, 2, 3, 1, 1);

        matchingfacts = new QPushButton(tab);
        matchingfacts->setObjectName(QString::fromUtf8("matchingfacts"));

        gridLayout_2->addWidget(matchingfacts, 3, 2, 1, 1);

        allfacts = new QPushButton(tab);
        allfacts->setObjectName(QString::fromUtf8("allfacts"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(allfacts->sizePolicy().hasHeightForWidth());
        allfacts->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(allfacts, 3, 3, 1, 1);

        tableView = new QTableView(tab);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        gridLayout_2->addWidget(tableView, 4, 0, 1, 4);

        object = new QLineEdit(tab);
        object->setObjectName(QString::fromUtf8("object"));

        gridLayout_2->addWidget(object, 2, 1, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget->addTab(tab_2, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

        FreeHAL_DBClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(FreeHAL_DBClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 715, 22));
        FreeHAL_DBClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FreeHAL_DBClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        FreeHAL_DBClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(FreeHAL_DBClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        FreeHAL_DBClass->setStatusBar(statusBar);
        QWidget::setTabOrder(subject, verb);
        QWidget::setTabOrder(verb, object);
        QWidget::setTabOrder(object, adverbs);
        QWidget::setTabOrder(adverbs, questionword);
        QWidget::setTabOrder(questionword, extra);
        QWidget::setTabOrder(extra, matchingfacts);
        QWidget::setTabOrder(matchingfacts, allfacts);
        QWidget::setTabOrder(allfacts, tableView);
        QWidget::setTabOrder(tableView, tabWidget);

        retranslateUi(FreeHAL_DBClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(FreeHAL_DBClass);
    } // setupUi

    void retranslateUi(QMainWindow *FreeHAL_DBClass)
    {
        FreeHAL_DBClass->setWindowTitle(QApplication::translate("FreeHAL_DBClass", "FreeHAL_DB", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("FreeHAL_DBClass", "Subjekt:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("FreeHAL_DBClass", "Adverbiale Bestimmungen:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("FreeHAL_DBClass", "Verb:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("FreeHAL_DBClass", "Fragewort:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("FreeHAL_DBClass", "Objekt:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("FreeHAL_DBClass", "Zusatz:", 0, QApplication::UnicodeUTF8));
        matchingfacts->setText(QApplication::translate("FreeHAL_DBClass", "passende Fakten", 0, QApplication::UnicodeUTF8));
        allfacts->setText(QApplication::translate("FreeHAL_DBClass", "alle Fakten", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("FreeHAL_DBClass", "Tab 1", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("FreeHAL_DBClass", "Tab 2", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(FreeHAL_DBClass);
    } // retranslateUi

};

namespace Ui {
    class FreeHAL_DBClass: public Ui_FreeHAL_DBClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FREEHAL_DB_H
