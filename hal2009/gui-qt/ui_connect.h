/********************************************************************************
** Form generated from reading ui file 'connect.ui'
**
** Created: Fri Feb 5 15:41:39 2010
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
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_ConnectDialog
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *spacerItem;
    QSpacerItem *spacerItem1;
    QGroupBox *groupBox;
    QGridLayout *gridLayout1;
    QSpacerItem *spacerItem2;
    QRadioButton *radioButton;
    QSpacerItem *spacerItem3;
    QSpacerItem *spacerItem4;
    QRadioButton *radioButton_2;
    QSpacerItem *spacerItem5;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout2;
    QCheckBox *start_kernel;
    QComboBox *database_engine;
    QLabel *label;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout3;
    QLabel *label_4;
    QLineEdit *ip;
    QSpacerItem *spacerItem6;
    QSpacerItem *spacerItem7;
    QDialogButtonBox *buttonBox;
    QSpacerItem *spacerItem8;
    QLabel *label_2;

    void setupUi(QDialog *ConnectDialog)
    {
        if (ConnectDialog->objectName().isEmpty())
            ConnectDialog->setObjectName(QString::fromUtf8("ConnectDialog"));
        ConnectDialog->resize(673, 348);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ConnectDialog->sizePolicy().hasHeightForWidth());
        ConnectDialog->setSizePolicy(sizePolicy);
        ConnectDialog->setMaximumSize(QSize(748546, 16777215));
        gridLayout = new QGridLayout(ConnectDialog);
        gridLayout->setMargin(0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        spacerItem = new QSpacerItem(20, 6, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(spacerItem, 1, 2, 1, 1);

        spacerItem1 = new QSpacerItem(16, 16, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(spacerItem1, 2, 0, 1, 1);

        groupBox = new QGroupBox(ConnectDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMinimumSize(QSize(560, 0));
        gridLayout1 = new QGridLayout(groupBox);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        spacerItem2 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout1->addItem(spacerItem2, 0, 0, 1, 1);

        radioButton = new QRadioButton(groupBox);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));
        radioButton->setChecked(true);

        gridLayout1->addWidget(radioButton, 0, 1, 1, 1);

        spacerItem3 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout1->addItem(spacerItem3, 0, 2, 1, 1);

        spacerItem4 = new QSpacerItem(71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout1->addItem(spacerItem4, 0, 3, 1, 1);

        radioButton_2 = new QRadioButton(groupBox);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));

        gridLayout1->addWidget(radioButton_2, 0, 4, 1, 1);

        spacerItem5 = new QSpacerItem(51, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout1->addItem(spacerItem5, 0, 5, 1, 1);

        groupBox_3 = new QGroupBox(groupBox);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setAlignment(Qt::AlignHCenter);
        gridLayout2 = new QGridLayout(groupBox_3);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        start_kernel = new QCheckBox(groupBox_3);
        start_kernel->setObjectName(QString::fromUtf8("start_kernel"));
        start_kernel->setChecked(true);

        gridLayout2->addWidget(start_kernel, 0, 0, 1, 2);

        database_engine = new QComboBox(groupBox_3);
        database_engine->setObjectName(QString::fromUtf8("database_engine"));

        gridLayout2->addWidget(database_engine, 1, 1, 1, 1);

        label = new QLabel(groupBox_3);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout2->addWidget(label, 1, 0, 1, 1);


        gridLayout1->addWidget(groupBox_3, 1, 0, 1, 3);

        groupBox_2 = new QGroupBox(groupBox);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setEnabled(false);
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy1);
        groupBox_2->setAlignment(Qt::AlignHCenter);
        gridLayout3 = new QGridLayout(groupBox_2);
        gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout3->addWidget(label_4, 0, 0, 1, 1);

        ip = new QLineEdit(groupBox_2);
        ip->setObjectName(QString::fromUtf8("ip"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(ip->sizePolicy().hasHeightForWidth());
        ip->setSizePolicy(sizePolicy2);

        gridLayout3->addWidget(ip, 0, 2, 1, 1);


        gridLayout1->addWidget(groupBox_2, 1, 3, 1, 3);


        gridLayout->addWidget(groupBox, 2, 1, 1, 2);

        spacerItem6 = new QSpacerItem(6, 6, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(spacerItem6, 2, 3, 1, 1);

        spacerItem7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacerItem7, 3, 1, 1, 1);

        buttonBox = new QDialogButtonBox(ConnectDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(buttonBox->sizePolicy().hasHeightForWidth());
        buttonBox->setSizePolicy(sizePolicy3);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 3, 2, 1, 1);

        spacerItem8 = new QSpacerItem(20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(spacerItem8, 4, 2, 1, 1);

        label_2 = new QLabel(ConnectDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setMinimumSize(QSize(0, 0));
        label_2->setMaximumSize(QSize(750, 142));
        label_2->setAutoFillBackground(false);
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/ui/logo.png")));
        label_2->setMargin(0);

        gridLayout->addWidget(label_2, 0, 0, 1, 4);


        retranslateUi(ConnectDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ConnectDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ConnectDialog, SLOT(reject()));
        QObject::connect(radioButton_2, SIGNAL(toggled(bool)), groupBox_2, SLOT(setEnabled(bool)));
        QObject::connect(radioButton_2, SIGNAL(toggled(bool)), groupBox_3, SLOT(setDisabled(bool)));

        QMetaObject::connectSlotsByName(ConnectDialog);
    } // setupUi

    void retranslateUi(QDialog *ConnectDialog)
    {
        ConnectDialog->setWindowTitle(QApplication::translate("ConnectDialog", "Connect to an artificial intelligence", 0, QApplication::UnicodeUTF8));
        ConnectDialog->setStyleSheet(QString());
        groupBox->setStyleSheet(QString());
        groupBox->setTitle(QApplication::translate("ConnectDialog", "Connect to...", 0, QApplication::UnicodeUTF8));
        radioButton->setText(QApplication::translate("ConnectDialog", "... this computer", 0, QApplication::UnicodeUTF8));
        radioButton_2->setText(QApplication::translate("ConnectDialog", "... a remote computer", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("ConnectDialog", "This Computer", 0, QApplication::UnicodeUTF8));
        start_kernel->setText(QApplication::translate("ConnectDialog", "If the FreeHAL service isn't running yet, start it", 0, QApplication::UnicodeUTF8));
        database_engine->clear();
        database_engine->insertItems(0, QStringList()
         << QApplication::translate("ConnectDialog", "disk", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConnectDialog", "ram", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConnectDialog", "disk (traditional)", 0, QApplication::UnicodeUTF8)
        );
        label->setText(QApplication::translate("ConnectDialog", "Use Database engine...", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("ConnectDialog", "A Remote Computer", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("ConnectDialog", "IP Address:", 0, QApplication::UnicodeUTF8));
        ip->setText(QApplication::translate("ConnectDialog", "127.0.0.1", 0, QApplication::UnicodeUTF8));
        buttonBox->setStyleSheet(QString());
        label_2->setStyleSheet(QApplication::translate("ConnectDialog", "background-color: white; border-bottom: 2px solid black;", 0, QApplication::UnicodeUTF8));
        label_2->setText(QString());
        Q_UNUSED(ConnectDialog);
    } // retranslateUi

};

namespace Ui {
    class ConnectDialog: public Ui_ConnectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECT_H
