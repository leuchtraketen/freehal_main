/********************************************************************************
** Form generated from reading ui file 'connect.ui'
**
** Created: Sun Feb 28 14:41:53 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CONNECT_H
#define UI_CONNECT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConnectDialog
{
public:
    QGridLayout *gridLayout_3;
    QLabel *label_2;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_2;
    QCheckBox *start_kernel;
    QLabel *label;
    QComboBox *database_engine;
    QSpacerItem *verticalSpacer_2;
    QLabel *label_3;
    QComboBox *lang;
    QWidget *tab_2;
    QGridLayout *gridLayout;
    QLabel *label_4;
    QLineEdit *ip;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;
    QDialogButtonBox *buttonBox;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QDialog *ConnectDialog)
    {
        if (ConnectDialog->objectName().isEmpty())
            ConnectDialog->setObjectName(QString::fromUtf8("ConnectDialog"));
        ConnectDialog->resize(317, 317);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ConnectDialog->sizePolicy().hasHeightForWidth());
        ConnectDialog->setSizePolicy(sizePolicy);
        ConnectDialog->setMaximumSize(QSize(748546, 16777215));
        gridLayout_3 = new QGridLayout(ConnectDialog);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_2 = new QLabel(ConnectDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setMinimumSize(QSize(0, 0));
        label_2->setMaximumSize(QSize(750, 142));
        label_2->setAutoFillBackground(false);
        label_2->setFrameShadow(QFrame::Plain);
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/ui/logo.png")));
        label_2->setMargin(0);

        gridLayout_3->addWidget(label_2, 0, 0, 1, 2);

        tabWidget = new QTabWidget(ConnectDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_2 = new QGridLayout(tab);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        start_kernel = new QCheckBox(tab);
        start_kernel->setObjectName(QString::fromUtf8("start_kernel"));
        start_kernel->setChecked(true);

        gridLayout_2->addWidget(start_kernel, 0, 0, 1, 2);

        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 1, 0, 1, 1);

        database_engine = new QComboBox(tab);
        database_engine->setObjectName(QString::fromUtf8("database_engine"));

        gridLayout_2->addWidget(database_engine, 1, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 66, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_2, 3, 0, 1, 1);

        label_3 = new QLabel(tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 2, 0, 1, 1);

        lang = new QComboBox(tab);
        lang->setObjectName(QString::fromUtf8("lang"));

        gridLayout_2->addWidget(lang, 2, 1, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout = new QGridLayout(tab_2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_4 = new QLabel(tab_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 0, 0, 1, 1);

        ip = new QLineEdit(tab_2);
        ip->setObjectName(QString::fromUtf8("ip"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(ip->sizePolicy().hasHeightForWidth());
        ip->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(ip, 0, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 94, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 1, 1, 1, 1);

        tabWidget->addTab(tab_2, QString());

        gridLayout_3->addWidget(tabWidget, 2, 0, 1, 2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 3, 0, 1, 1);

        buttonBox = new QDialogButtonBox(ConnectDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(buttonBox->sizePolicy().hasHeightForWidth());
        buttonBox->setSizePolicy(sizePolicy2);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout_3->addWidget(buttonBox, 3, 1, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_3, 1, 0, 1, 1);


        retranslateUi(ConnectDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ConnectDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ConnectDialog, SLOT(reject()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ConnectDialog);
    } // setupUi

    void retranslateUi(QDialog *ConnectDialog)
    {
        ConnectDialog->setWindowTitle(QApplication::translate("ConnectDialog", "Connect to an artificial intelligence", 0, QApplication::UnicodeUTF8));
        ConnectDialog->setStyleSheet(QString());
        label_2->setStyleSheet(QString());
        label_2->setText(QString());
        start_kernel->setText(QApplication::translate("ConnectDialog", "Start the FreeHAL service", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ConnectDialog", "Database engine:", 0, QApplication::UnicodeUTF8));
        database_engine->clear();
        database_engine->insertItems(0, QStringList()
         << QApplication::translate("ConnectDialog", "disk", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConnectDialog", "ram", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConnectDialog", "disk (traditional)", 0, QApplication::UnicodeUTF8)
        );
        label_3->setText(QApplication::translate("ConnectDialog", "Language:", 0, QApplication::UnicodeUTF8));
        lang->clear();
        lang->insertItems(0, QStringList()
         << QApplication::translate("ConnectDialog", "de", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConnectDialog", "en", 0, QApplication::UnicodeUTF8)
        );
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("ConnectDialog", "Local Connection", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("ConnectDialog", "IP Address:", 0, QApplication::UnicodeUTF8));
        ip->setText(QApplication::translate("ConnectDialog", "127.0.0.1", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("ConnectDialog", "Remote Connection", 0, QApplication::UnicodeUTF8));
        buttonBox->setStyleSheet(QString());
        Q_UNUSED(ConnectDialog);
    } // retranslateUi

};

namespace Ui {
    class ConnectDialog: public Ui_ConnectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECT_H
