/********************************************************************************
** Form generated from reading ui file 'db_edit.ui'
**
** Created: Fri Apr 24 17:08:07 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_DB_EDIT_H
#define UI_DB_EDIT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_db_edit
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout1;
    QTextEdit *editor;
    QPushButton *save;
    QPushButton *hide;
    QSpacerItem *spacerItem;

    void setupUi(QDialog *db_edit)
    {
        if (db_edit->objectName().isEmpty())
            db_edit->setObjectName(QString::fromUtf8("db_edit"));
        db_edit->resize(400, 300);
        gridLayout = new QGridLayout(db_edit);
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout->setMargin(9);
#endif
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox = new QGroupBox(db_edit);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout1 = new QGridLayout(groupBox);
#ifndef Q_OS_MAC
        gridLayout1->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout1->setMargin(9);
#endif
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        editor = new QTextEdit(groupBox);
        editor->setObjectName(QString::fromUtf8("editor"));

        gridLayout1->addWidget(editor, 0, 0, 1, 1);


        gridLayout->addWidget(groupBox, 0, 0, 1, 3);

        save = new QPushButton(db_edit);
        save->setObjectName(QString::fromUtf8("save"));

        gridLayout->addWidget(save, 1, 2, 1, 1);

        hide = new QPushButton(db_edit);
        hide->setObjectName(QString::fromUtf8("hide"));

        gridLayout->addWidget(hide, 1, 1, 1, 1);

        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacerItem, 1, 0, 1, 1);


        retranslateUi(db_edit);

        QMetaObject::connectSlotsByName(db_edit);
    } // setupUi

    void retranslateUi(QDialog *db_edit)
    {
        db_edit->setWindowTitle(QApplication::translate("db_edit", "Datenbankeditor", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("db_edit", "Datenbankeditor", 0, QApplication::UnicodeUTF8));
        save->setText(QApplication::translate("db_edit", "Speichern", 0, QApplication::UnicodeUTF8));
        hide->setText(QApplication::translate("db_edit", "Abbrechen", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(db_edit);
    } // retranslateUi

};

namespace Ui {
    class db_edit: public Ui_db_edit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DB_EDIT_H
