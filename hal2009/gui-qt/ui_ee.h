/********************************************************************************
** Form generated from reading ui file 'ee.ui'
**
** Created: Fri Jan 22 20:48:27 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_EE_H
#define UI_EE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_EasterEgg
{
public:
    QGridLayout *gridLayout;
    QTextBrowser *textBrowser;

    void setupUi(QDialog *EasterEgg)
    {
        if (EasterEgg->objectName().isEmpty())
            EasterEgg->setObjectName(QString::fromUtf8("EasterEgg"));
        EasterEgg->resize(558, 466);
        gridLayout = new QGridLayout(EasterEgg);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        gridLayout->setVerticalSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        textBrowser = new QTextBrowser(EasterEgg);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        QFont font;
        font.setFamily(QString::fromUtf8("Courier 10 Pitch"));
        textBrowser->setFont(font);

        gridLayout->addWidget(textBrowser, 0, 0, 1, 1);


        retranslateUi(EasterEgg);

        QMetaObject::connectSlotsByName(EasterEgg);
    } // setupUi

    void retranslateUi(QDialog *EasterEgg)
    {
        EasterEgg->setWindowTitle(QApplication::translate("EasterEgg", "Dialog", 0, QApplication::UnicodeUTF8));
        textBrowser->setHtml(QApplication::translate("EasterEgg", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Courier 10 Pitch'; font-size:10pt; font-weight:400; font-style:normal;\" bgcolor=\"#000082\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:9pt;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'monospace'; font-size:9pt;\"><span style=\" color:#fff8dc;\">A problem has been detected and FreeHAL has been shut down to prevent damage to your computer.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'monospace'; font-size:9pt; color:#fff8dc;\"></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-le"
                        "ft:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'monospace'; font-size:9pt; color:#fff8dc;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'monospace'; font-size:9pt; color:#fff8dc;\">The end-user manually generated the crashdump.</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'monospace'; font-size:9pt; color:#fff8dc;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'monospace'; font-size:9pt; color:#fff8dc;\">If this is the first time you've seen this error screen, restart your computer.</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'monospace'; font-size:9pt; color:#fff8dc;\">If this screen "
                        "appears again, follow these steps:</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'monospace'; font-size:9pt; color:#fff8dc;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'monospace'; font-size:9pt; color:#fff8dc;\"> - If you don't live in America: swim over the Atlantic...</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'monospace'; font-size:9pt; color:#fff8dc;\"> - If you are still alive: make enough money to buy Micro$oft and dismiss William Henry Gates and Steve Ballmer</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'monospace'; font-size:9pt; color:#fff8dc;\"> - license Windows as open source under the GNU General P"
                        "ublic License Version 3</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'monospace'; font-size:9pt; color:#fff8dc;\"> - Try to run FreeHAL again</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'monospace'; font-size:9pt; color:#fff8dc;\"></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'monospace'; font-size:9pt; color:#fff8dc;\"></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'monospace'; font-size:9pt; color:#fff8dc;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'monospace'; font-si"
                        "ze:9pt; color:#fff8dc;\">Technical Information:</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'monospace'; font-size:9pt; color:#fff8dc;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'monospace'; font-size:9pt; color:#fff8dc;\">FreeHAL detected an Micro$oft Windows Installation on this computer, a computer which is connected to this LAN with at least one windows system .dll, or any Hardware  which is Micro$oft Vista compatible.</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'monospace'; font-size:9pt; color:#fff8dc;\"></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'mono"
                        "space'; font-size:9pt; color:#fff8dc;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'monospace'; font-size:9pt; color:#fff8dc;\">*** STOP: 0x00567386 (0x39608576,0x8937578,0x78432777,0x00000000)</p></body></html>", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(EasterEgg);
    } // retranslateUi

};

namespace Ui {
    class EasterEgg: public Ui_EasterEgg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EE_H
