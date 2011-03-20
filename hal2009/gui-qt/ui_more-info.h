/********************************************************************************
** Form generated from reading UI file 'more-info.ui'
**
** Created: Sun Mar 20 00:23:28 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MORE_2D_INFO_H
#define UI_MORE_2D_INFO_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_MoreInfo
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QFrame *frame;
    QGridLayout *gridLayout_2;
    QLabel *label_3;
    QLineEdit *i_word;
    QLabel *label_4;
    QLineEdit *i_sentence;
    QLabel *label_5;
    QTextEdit *i_text;
    QLineEdit *i_source;
    QPushButton *w_n;
    QLabel *label_2;
    QPushButton *w_v;
    QLabel *label_6;
    QPushButton *w_a;
    QLabel *label_7;
    QPushButton *w_c;
    QLabel *label_8;
    QPushButton *w_i;
    QLabel *label_9;
    QPushButton *w_p;
    QLabel *label_10;
    QDialogButtonBox *buttonBox;
    QSpacerItem *verticalSpacer;
    QFrame *line;
    QGroupBox *box_genus;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *g_male;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *g_female;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *g_neuter;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *g_unknown;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *MoreInfo)
    {
        if (MoreInfo->objectName().isEmpty())
            MoreInfo->setObjectName(QString::fromUtf8("MoreInfo"));
        MoreInfo->resize(479, 459);
        QIcon icon;
        icon.addFile(QString::fromUtf8("../icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        MoreInfo->setWindowIcon(icon);
        MoreInfo->setModal(false);
        gridLayout = new QGridLayout(MoreInfo);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(MoreInfo);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label, 0, 0, 1, 2);

        frame = new QFrame(MoreInfo);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Plain);
        gridLayout_2 = new QGridLayout(frame);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_3 = new QLabel(frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 1, 0, 1, 1);

        i_word = new QLineEdit(frame);
        i_word->setObjectName(QString::fromUtf8("i_word"));
        i_word->setFrame(true);
        i_word->setReadOnly(true);

        gridLayout_2->addWidget(i_word, 1, 2, 1, 1);

        label_4 = new QLabel(frame);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 2, 0, 1, 1);

        i_sentence = new QLineEdit(frame);
        i_sentence->setObjectName(QString::fromUtf8("i_sentence"));
        i_sentence->setReadOnly(true);

        gridLayout_2->addWidget(i_sentence, 2, 2, 1, 1);

        label_5 = new QLabel(frame);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 3, 0, 1, 1);

        i_text = new QTextEdit(frame);
        i_text->setObjectName(QString::fromUtf8("i_text"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(100);
        sizePolicy1.setHeightForWidth(i_text->sizePolicy().hasHeightForWidth());
        i_text->setSizePolicy(sizePolicy1);
        i_text->setFrameShape(QFrame::StyledPanel);
        i_text->setFrameShadow(QFrame::Sunken);
        i_text->setReadOnly(true);

        gridLayout_2->addWidget(i_text, 0, 0, 1, 3);

        i_source = new QLineEdit(frame);
        i_source->setObjectName(QString::fromUtf8("i_source"));
        i_source->setReadOnly(true);

        gridLayout_2->addWidget(i_source, 3, 2, 1, 1);


        gridLayout->addWidget(frame, 2, 0, 1, 2);

        w_n = new QPushButton(MoreInfo);
        w_n->setObjectName(QString::fromUtf8("w_n"));

        gridLayout->addWidget(w_n, 6, 0, 1, 1);

        label_2 = new QLabel(MoreInfo);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 6, 1, 1, 1);

        w_v = new QPushButton(MoreInfo);
        w_v->setObjectName(QString::fromUtf8("w_v"));

        gridLayout->addWidget(w_v, 8, 0, 1, 1);

        label_6 = new QLabel(MoreInfo);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 8, 1, 1, 1);

        w_a = new QPushButton(MoreInfo);
        w_a->setObjectName(QString::fromUtf8("w_a"));

        gridLayout->addWidget(w_a, 9, 0, 1, 1);

        label_7 = new QLabel(MoreInfo);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 9, 1, 1, 1);

        w_c = new QPushButton(MoreInfo);
        w_c->setObjectName(QString::fromUtf8("w_c"));

        gridLayout->addWidget(w_c, 10, 0, 1, 1);

        label_8 = new QLabel(MoreInfo);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 10, 1, 1, 1);

        w_i = new QPushButton(MoreInfo);
        w_i->setObjectName(QString::fromUtf8("w_i"));

        gridLayout->addWidget(w_i, 11, 0, 1, 1);

        label_9 = new QLabel(MoreInfo);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout->addWidget(label_9, 11, 1, 1, 1);

        w_p = new QPushButton(MoreInfo);
        w_p->setObjectName(QString::fromUtf8("w_p"));

        gridLayout->addWidget(w_p, 12, 0, 1, 1);

        label_10 = new QLabel(MoreInfo);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout->addWidget(label_10, 12, 1, 1, 1);

        buttonBox = new QDialogButtonBox(MoreInfo);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel);

        gridLayout->addWidget(buttonBox, 13, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 4, 1, 1, 1);

        line = new QFrame(MoreInfo);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 5, 0, 1, 2);

        box_genus = new QGroupBox(MoreInfo);
        box_genus->setObjectName(QString::fromUtf8("box_genus"));
        horizontalLayout = new QHBoxLayout(box_genus);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        g_male = new QPushButton(box_genus);
        g_male->setObjectName(QString::fromUtf8("g_male"));

        horizontalLayout->addWidget(g_male);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        g_female = new QPushButton(box_genus);
        g_female->setObjectName(QString::fromUtf8("g_female"));

        horizontalLayout->addWidget(g_female);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        g_neuter = new QPushButton(box_genus);
        g_neuter->setObjectName(QString::fromUtf8("g_neuter"));

        horizontalLayout->addWidget(g_neuter);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_5);

        g_unknown = new QPushButton(box_genus);
        g_unknown->setObjectName(QString::fromUtf8("g_unknown"));

        horizontalLayout->addWidget(g_unknown);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        gridLayout->addWidget(box_genus, 7, 0, 1, 2);

        QWidget::setTabOrder(i_text, w_n);
        QWidget::setTabOrder(w_n, w_v);
        QWidget::setTabOrder(w_v, w_a);
        QWidget::setTabOrder(w_a, w_c);
        QWidget::setTabOrder(w_c, w_i);
        QWidget::setTabOrder(w_i, w_p);
        QWidget::setTabOrder(w_p, buttonBox);
        QWidget::setTabOrder(buttonBox, i_word);
        QWidget::setTabOrder(i_word, i_sentence);
        QWidget::setTabOrder(i_sentence, i_source);

        retranslateUi(MoreInfo);
        QObject::connect(buttonBox, SIGNAL(accepted()), MoreInfo, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), MoreInfo, SLOT(reject()));

        QMetaObject::connectSlotsByName(MoreInfo);
    } // setupUi

    void retranslateUi(QDialog *MoreInfo)
    {
        MoreInfo->setWindowTitle(QApplication::translate("MoreInfo", "Some information needed", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MoreInfo", "FreeHAL needs some information about a word or expression:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MoreInfo", "Word:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MoreInfo", "Sentence:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MoreInfo", "Source:", 0, QApplication::UnicodeUTF8));
        w_n->setText(QApplication::translate("MoreInfo", "Noun", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MoreInfo", "All nouns, names and personal pronouns", 0, QApplication::UnicodeUTF8));
        w_v->setText(QApplication::translate("MoreInfo", "Verb", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MoreInfo", "All verbs", 0, QApplication::UnicodeUTF8));
        w_a->setText(QApplication::translate("MoreInfo", "Adjective", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MoreInfo", "All adjectives and adverbs", 0, QApplication::UnicodeUTF8));
        w_c->setText(QApplication::translate("MoreInfo", "Conjunction", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MoreInfo", "Question words and conjunctions, like \"how\", \"while\", ...", 0, QApplication::UnicodeUTF8));
        w_i->setText(QApplication::translate("MoreInfo", "General Word", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("MoreInfo", "Interjections, e.g. greetings (\"hello\")", 0, QApplication::UnicodeUTF8));
        w_p->setText(QApplication::translate("MoreInfo", "Preposition", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("MoreInfo", "Prepositions like \"in\", \"of\", \"with\", \"at\"", 0, QApplication::UnicodeUTF8));
        box_genus->setTitle(QApplication::translate("MoreInfo", "grammatical gender (genus)", 0, QApplication::UnicodeUTF8));
        g_male->setText(QApplication::translate("MoreInfo", "male", 0, QApplication::UnicodeUTF8));
        g_female->setText(QApplication::translate("MoreInfo", "female", 0, QApplication::UnicodeUTF8));
        g_neuter->setText(QApplication::translate("MoreInfo", "neuter", 0, QApplication::UnicodeUTF8));
        g_unknown->setText(QApplication::translate("MoreInfo", "unknown", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MoreInfo: public Ui_MoreInfo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MORE_2D_INFO_H
