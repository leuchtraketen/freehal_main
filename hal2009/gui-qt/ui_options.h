/********************************************************************************
** Form generated from reading ui file 'options.ui'
**
** Created: Sun Jan 3 16:37:12 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_OPTIONS_H
#define UI_OPTIONS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QGridLayout *gridLayout;
    QDialogButtonBox *buttonBox;
    QTabWidget *tabWidget;
    QWidget *General;
    QGridLayout *gridLayout1;
    QGroupBox *groupBox;
    QGridLayout *gridLayout2;
    QPushButton *clear_screen;
    QSpacerItem *verticalSpacer_2;
    QGroupBox *groupBox_7;
    QGridLayout *gridLayout3;
    QCheckBox *speech;
    QSpacerItem *verticalSpacer_3;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout;
    QRadioButton *ask_frame;
    QRadioButton *ask_dialog;
    QSpacerItem *verticalSpacer_4;
    QSpacerItem *verticalSpacer;
    QWidget *tab;
    QGridLayout *gridLayout4;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout5;
    QCheckBox *mode_offline;
    QSpacerItem *spacerItem;
    QLabel *label_3;
    QLineEdit *proxy;
    QSpacerItem *spacerItem1;
    QLabel *label_4;
    QLineEdit *proxy_port;
    QPushButton *proxy_ok;
    QSpacerItem *spacerItem2;
    QSpacerItem *spacerItem3;
    QCheckBox *mode_www;
    QSpacerItem *spacerItem4;
    QWidget *tab_2;
    QGridLayout *gridLayout6;
    QGroupBox *groupBox_9;
    QGridLayout *gridLayout7;
    QCheckBox *access_online_db;
    QSpacerItem *spacerItem5;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(715, 423);
        gridLayout = new QGridLayout(Dialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        buttonBox = new QDialogButtonBox(Dialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 2, 0, 1, 1);

        tabWidget = new QTabWidget(Dialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        General = new QWidget();
        General->setObjectName(QString::fromUtf8("General"));
        gridLayout1 = new QGridLayout(General);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        groupBox = new QGroupBox(General);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout2 = new QGridLayout(groupBox);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        clear_screen = new QPushButton(groupBox);
        clear_screen->setObjectName(QString::fromUtf8("clear_screen"));

        gridLayout2->addWidget(clear_screen, 0, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout2->addItem(verticalSpacer_2, 1, 0, 1, 1);


        gridLayout1->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_7 = new QGroupBox(General);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        gridLayout3 = new QGridLayout(groupBox_7);
        gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
        speech = new QCheckBox(groupBox_7);
        speech->setObjectName(QString::fromUtf8("speech"));

        gridLayout3->addWidget(speech, 0, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout3->addItem(verticalSpacer_3, 1, 0, 1, 1);


        gridLayout1->addWidget(groupBox_7, 0, 1, 1, 1);

        groupBox_2 = new QGroupBox(General);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(groupBox_2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        ask_frame = new QRadioButton(groupBox_2);
        ask_frame->setObjectName(QString::fromUtf8("ask_frame"));

        verticalLayout->addWidget(ask_frame);

        ask_dialog = new QRadioButton(groupBox_2);
        ask_dialog->setObjectName(QString::fromUtf8("ask_dialog"));
        ask_dialog->setChecked(true);

        verticalLayout->addWidget(ask_dialog);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_4);


        gridLayout1->addWidget(groupBox_2, 1, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout1->addItem(verticalSpacer, 2, 0, 1, 1);

        tabWidget->addTab(General, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout4 = new QGridLayout(tab);
        gridLayout4->setObjectName(QString::fromUtf8("gridLayout4"));
        groupBox_3 = new QGroupBox(tab);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout5 = new QGridLayout(groupBox_3);
        gridLayout5->setObjectName(QString::fromUtf8("gridLayout5"));
        mode_offline = new QCheckBox(groupBox_3);
        mode_offline->setObjectName(QString::fromUtf8("mode_offline"));

        gridLayout5->addWidget(mode_offline, 1, 0, 1, 2);

        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout5->addItem(spacerItem, 1, 2, 1, 1);

        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout5->addWidget(label_3, 2, 0, 1, 1);

        proxy = new QLineEdit(groupBox_3);
        proxy->setObjectName(QString::fromUtf8("proxy"));

        gridLayout5->addWidget(proxy, 2, 1, 1, 1);

        spacerItem1 = new QSpacerItem(81, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout5->addItem(spacerItem1, 2, 2, 1, 1);

        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout5->addWidget(label_4, 3, 0, 1, 1);

        proxy_port = new QLineEdit(groupBox_3);
        proxy_port->setObjectName(QString::fromUtf8("proxy_port"));

        gridLayout5->addWidget(proxy_port, 3, 1, 1, 1);

        proxy_ok = new QPushButton(groupBox_3);
        proxy_ok->setObjectName(QString::fromUtf8("proxy_ok"));

        gridLayout5->addWidget(proxy_ok, 3, 2, 1, 1);

        spacerItem2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout5->addItem(spacerItem2, 3, 3, 1, 1);

        spacerItem3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout5->addItem(spacerItem3, 0, 2, 1, 1);

        mode_www = new QCheckBox(groupBox_3);
        mode_www->setObjectName(QString::fromUtf8("mode_www"));
        mode_www->setChecked(true);

        gridLayout5->addWidget(mode_www, 0, 0, 1, 2);


        gridLayout4->addWidget(groupBox_3, 0, 0, 1, 1);

        spacerItem4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout4->addItem(spacerItem4, 2, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout6 = new QGridLayout(tab_2);
        gridLayout6->setObjectName(QString::fromUtf8("gridLayout6"));
        groupBox_9 = new QGroupBox(tab_2);
        groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
        gridLayout7 = new QGridLayout(groupBox_9);
        gridLayout7->setObjectName(QString::fromUtf8("gridLayout7"));
        access_online_db = new QCheckBox(groupBox_9);
        access_online_db->setObjectName(QString::fromUtf8("access_online_db"));
        access_online_db->setChecked(true);

        gridLayout7->addWidget(access_online_db, 0, 0, 1, 2);


        gridLayout6->addWidget(groupBox_9, 0, 1, 1, 1);

        spacerItem5 = new QSpacerItem(323, 81, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout6->addItem(spacerItem5, 1, 1, 2, 1);

        tabWidget->addTab(tab_2, QString());

        gridLayout->addWidget(tabWidget, 1, 0, 1, 1);


        retranslateUi(Dialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), Dialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Dialog, SLOT(reject()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "FreeHAL - Preferences", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("Dialog", "Talk Window", 0, QApplication::UnicodeUTF8));
        clear_screen->setText(QApplication::translate("Dialog", "Clear", 0, QApplication::UnicodeUTF8));
        groupBox_7->setTitle(QApplication::translate("Dialog", "Speech Synthesis", 0, QApplication::UnicodeUTF8));
        speech->setText(QApplication::translate("Dialog", "Use Speech Synthesis", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("Dialog", "When more Information is needed", 0, QApplication::UnicodeUTF8));
        ask_frame->setText(QApplication::translate("Dialog", "Ask in a side frame", 0, QApplication::UnicodeUTF8));
        ask_dialog->setText(QApplication::translate("Dialog", "Ask in a Dialog window", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(General), QApplication::translate("Dialog", "General", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("Dialog", "Internet", 0, QApplication::UnicodeUTF8));
        mode_offline->setText(QApplication::translate("Dialog", "Offline Mode", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Dialog", "HTTP Proxy:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Dialog", "Port:", 0, QApplication::UnicodeUTF8));
        proxy_port->setText(QApplication::translate("Dialog", "80", 0, QApplication::UnicodeUTF8));
        proxy_ok->setText(QApplication::translate("Dialog", "Activate", 0, QApplication::UnicodeUTF8));
        mode_www->setText(QApplication::translate("Dialog", "FreeHAL can use the Internet while I'm inactive", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Dialog", "Internet", 0, QApplication::UnicodeUTF8));
        groupBox_9->setTitle(QApplication::translate("Dialog", "Kommunkation", 0, QApplication::UnicodeUTF8));
        access_online_db->setText(QApplication::translate("Dialog", "FreeHAL may connect to other FreeHAL's in the current LAN", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("Dialog", "Communication with Peers", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Dialog);
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPTIONS_H
