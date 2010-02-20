/********************************************************************************
** Form generated from reading ui file 'changelog.ui'
**
** Created: Sat Feb 20 10:23:50 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CHANGELOG_H
#define UI_CHANGELOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_ChangelogDialog
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox;
    QTextEdit *te_changelog;

    void setupUi(QDialog *ChangelogDialog)
    {
        if (ChangelogDialog->objectName().isEmpty())
            ChangelogDialog->setObjectName(QString::fromUtf8("ChangelogDialog"));
        ChangelogDialog->resize(400, 300);
        buttonBox = new QDialogButtonBox(ChangelogDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(50, 260, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        groupBox = new QGroupBox(ChangelogDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 381, 241));
        te_changelog = new QTextEdit(groupBox);
        te_changelog->setObjectName(QString::fromUtf8("te_changelog"));
        te_changelog->setGeometry(QRect(10, 20, 361, 211));

        retranslateUi(ChangelogDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ChangelogDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ChangelogDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ChangelogDialog);
    } // setupUi

    void retranslateUi(QDialog *ChangelogDialog)
    {
        ChangelogDialog->setWindowTitle(QApplication::translate("ChangelogDialog", "License", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("ChangelogDialog", "Lizenz", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ChangelogDialog);
    } // retranslateUi

};

namespace Ui {
    class ChangelogDialog: public Ui_ChangelogDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHANGELOG_H
