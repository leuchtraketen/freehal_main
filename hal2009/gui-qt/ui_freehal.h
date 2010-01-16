/********************************************************************************
** Form generated from reading ui file 'freehal.ui'
**
** Created: Sat Jan 16 21:20:57 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_FREEHAL_H
#define UI_FREEHAL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QScrollBar>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableView>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_freehalWindow
{
public:
    QAction *actionGespr_ch;
    QAction *actionDatenbank;
    QAction *actionBeenden;
    QAction *actionVorf_hrmodus;
    QAction *actionBildschirm_l_schen;
    QAction *actionDatenbank_editieren;
    QAction *actionOnlinedatenbank_laden;
    QAction *actionIm_WWW_Surfen;
    QAction *actionOfflinemodus;
    QAction *actionWikipedia_Artikel_Informationen_laden;
    QAction *actionWikipedia_Artikel_Informationen_speichern;
    QAction *actionInfo;
    QAction *actionEinstellungen;
    QAction *fullscreen;
    QAction *actionSemantisches_Netz;
    QAction *actionNeue_Verbindung_herstellen;
    QAction *action_scan_a_text;
    QWidget *centralwidget;
    QGridLayout *gridLayout_8;
    QGroupBox *msg_box_2;
    QGridLayout *gridLayout;
    QTextEdit *msg_2;
    QPushButton *male;
    QPushButton *female;
    QPushButton *thing;
    QPushButton *no_genus;
    QPushButton *abbr_1;
    QGroupBox *msg_box;
    QGridLayout *gridLayout1;
    QTextEdit *msg;
    QPushButton *w_name;
    QPushButton *w_nomen;
    QPushButton *w_verb;
    QPushButton *w_adj;
    QPushButton *w_adv;
    QPushButton *w_pronomen;
    QPushButton *w_quesword;
    QPushButton *w_interj;
    QPushButton *w_prep;
    QPushButton *w_interj2;
    QPushButton *w_adv_best;
    QPushButton *abbr_2;
    QTabWidget *tabWidget;
    QWidget *Talk;
    QGridLayout *gridLayout2;
    QTextEdit *textedit_talk;
    QLineEdit *lineedit_talk;
    QPushButton *normalscreen;
    QPushButton *pushButton;
    QWidget *Learn;
    QGridLayout *gridLayout_2;
    QProgressBar *learnbar;
    QPushButton *pushButton_learn;
    QLineEdit *lineedit_learn;
    QTextEdit *textedit_learn;
    QWidget *tab;
    QGridLayout *gridLayout_6;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout_4;
    QScrollBar *verticalScrollBar;
    QFrame *line;
    QLabel *label_2;
    QLineEdit *flowchart_input;
    QPushButton *compute_output;
    QPushButton *refresh_chart;
    QFrame *line_2;
    QLabel *label_3;
    QLabel *flowchart_view;
    QLabel *label_4;
    QLineEdit *flowchart_edit;
    QLabel *label;
    QSpinBox *flowchart_fact_pk;
    QPushButton *plus;
    QPushButton *minus;
    QPushButton *flowchart_fact_edit;
    QPushButton *flowchart_fact_delete;
    QWidget *Database;
    QGridLayout *gridLayout_7;
    QLabel *label_14;
    QLineEdit *subject;
    QLabel *label_12;
    QLineEdit *adverbs;
    QLabel *label_15;
    QLineEdit *verb;
    QLabel *label_11;
    QLineEdit *questionword;
    QLabel *label_13;
    QLineEdit *object;
    QLabel *label_16;
    QLineEdit *extra;
    QFrame *frame;
    QGridLayout *gridLayout_5;
    QSpacerItem *horizontalSpacer;
    QPushButton *matchingfacts;
    QPushButton *allfacts;
    QTableView *tableView;
    QMenuBar *menubar;
    QMenu *menuDatei;
    QMenu *menuNeu;
    QMenu *menuExtras;
    QMenu *menuHilfe;
    QMenu *menuVerbindung;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *freehalWindow)
    {
        if (freehalWindow->objectName().isEmpty())
            freehalWindow->setObjectName(QString::fromUtf8("freehalWindow"));
        freehalWindow->resize(986, 539);
        freehalWindow->setMaximumSize(QSize(16777215, 16000000));
        QIcon icon;
        icon.addFile(QString::fromUtf8("../../../../../../../../home/tobias/.designer/backup/freehal16.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        freehalWindow->setWindowIcon(icon);
        freehalWindow->setAnimated(false);
        freehalWindow->setDockNestingEnabled(true);
        freehalWindow->setUnifiedTitleAndToolBarOnMac(false);
        actionGespr_ch = new QAction(freehalWindow);
        actionGespr_ch->setObjectName(QString::fromUtf8("actionGespr_ch"));
        actionDatenbank = new QAction(freehalWindow);
        actionDatenbank->setObjectName(QString::fromUtf8("actionDatenbank"));
        actionBeenden = new QAction(freehalWindow);
        actionBeenden->setObjectName(QString::fromUtf8("actionBeenden"));
        actionVorf_hrmodus = new QAction(freehalWindow);
        actionVorf_hrmodus->setObjectName(QString::fromUtf8("actionVorf_hrmodus"));
        actionBildschirm_l_schen = new QAction(freehalWindow);
        actionBildschirm_l_schen->setObjectName(QString::fromUtf8("actionBildschirm_l_schen"));
        actionDatenbank_editieren = new QAction(freehalWindow);
        actionDatenbank_editieren->setObjectName(QString::fromUtf8("actionDatenbank_editieren"));
        actionOnlinedatenbank_laden = new QAction(freehalWindow);
        actionOnlinedatenbank_laden->setObjectName(QString::fromUtf8("actionOnlinedatenbank_laden"));
        actionIm_WWW_Surfen = new QAction(freehalWindow);
        actionIm_WWW_Surfen->setObjectName(QString::fromUtf8("actionIm_WWW_Surfen"));
        actionIm_WWW_Surfen->setCheckable(true);
        actionIm_WWW_Surfen->setChecked(true);
        actionOfflinemodus = new QAction(freehalWindow);
        actionOfflinemodus->setObjectName(QString::fromUtf8("actionOfflinemodus"));
        actionOfflinemodus->setCheckable(true);
        actionWikipedia_Artikel_Informationen_laden = new QAction(freehalWindow);
        actionWikipedia_Artikel_Informationen_laden->setObjectName(QString::fromUtf8("actionWikipedia_Artikel_Informationen_laden"));
        actionWikipedia_Artikel_Informationen_speichern = new QAction(freehalWindow);
        actionWikipedia_Artikel_Informationen_speichern->setObjectName(QString::fromUtf8("actionWikipedia_Artikel_Informationen_speichern"));
        actionInfo = new QAction(freehalWindow);
        actionInfo->setObjectName(QString::fromUtf8("actionInfo"));
        actionEinstellungen = new QAction(freehalWindow);
        actionEinstellungen->setObjectName(QString::fromUtf8("actionEinstellungen"));
        fullscreen = new QAction(freehalWindow);
        fullscreen->setObjectName(QString::fromUtf8("fullscreen"));
        actionSemantisches_Netz = new QAction(freehalWindow);
        actionSemantisches_Netz->setObjectName(QString::fromUtf8("actionSemantisches_Netz"));
        actionNeue_Verbindung_herstellen = new QAction(freehalWindow);
        actionNeue_Verbindung_herstellen->setObjectName(QString::fromUtf8("actionNeue_Verbindung_herstellen"));
        action_scan_a_text = new QAction(freehalWindow);
        action_scan_a_text->setObjectName(QString::fromUtf8("action_scan_a_text"));
        centralwidget = new QWidget(freehalWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_8 = new QGridLayout(centralwidget);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        msg_box_2 = new QGroupBox(centralwidget);
        msg_box_2->setObjectName(QString::fromUtf8("msg_box_2"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(msg_box_2->sizePolicy().hasHeightForWidth());
        msg_box_2->setSizePolicy(sizePolicy);
        msg_box_2->setMinimumSize(QSize(200, 0));
        gridLayout = new QGridLayout(msg_box_2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        msg_2 = new QTextEdit(msg_box_2);
        msg_2->setObjectName(QString::fromUtf8("msg_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Ignored);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(msg_2->sizePolicy().hasHeightForWidth());
        msg_2->setSizePolicy(sizePolicy1);
        msg_2->setMinimumSize(QSize(0, 50));

        gridLayout->addWidget(msg_2, 0, 0, 1, 1);

        male = new QPushButton(msg_box_2);
        male->setObjectName(QString::fromUtf8("male"));

        gridLayout->addWidget(male, 1, 0, 1, 1);

        female = new QPushButton(msg_box_2);
        female->setObjectName(QString::fromUtf8("female"));

        gridLayout->addWidget(female, 2, 0, 1, 1);

        thing = new QPushButton(msg_box_2);
        thing->setObjectName(QString::fromUtf8("thing"));

        gridLayout->addWidget(thing, 3, 0, 1, 1);

        no_genus = new QPushButton(msg_box_2);
        no_genus->setObjectName(QString::fromUtf8("no_genus"));

        gridLayout->addWidget(no_genus, 4, 0, 1, 1);

        abbr_1 = new QPushButton(msg_box_2);
        abbr_1->setObjectName(QString::fromUtf8("abbr_1"));

        gridLayout->addWidget(abbr_1, 5, 0, 1, 1);


        gridLayout_8->addWidget(msg_box_2, 0, 1, 2, 1);

        msg_box = new QGroupBox(centralwidget);
        msg_box->setObjectName(QString::fromUtf8("msg_box"));
        sizePolicy.setHeightForWidth(msg_box->sizePolicy().hasHeightForWidth());
        msg_box->setSizePolicy(sizePolicy);
        msg_box->setMinimumSize(QSize(200, 0));
        gridLayout1 = new QGridLayout(msg_box);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        msg = new QTextEdit(msg_box);
        msg->setObjectName(QString::fromUtf8("msg"));
        sizePolicy1.setHeightForWidth(msg->sizePolicy().hasHeightForWidth());
        msg->setSizePolicy(sizePolicy1);
        msg->setMinimumSize(QSize(0, 50));

        gridLayout1->addWidget(msg, 0, 0, 1, 2);

        w_name = new QPushButton(msg_box);
        w_name->setObjectName(QString::fromUtf8("w_name"));

        gridLayout1->addWidget(w_name, 1, 0, 1, 1);

        w_nomen = new QPushButton(msg_box);
        w_nomen->setObjectName(QString::fromUtf8("w_nomen"));

        gridLayout1->addWidget(w_nomen, 1, 1, 1, 1);

        w_verb = new QPushButton(msg_box);
        w_verb->setObjectName(QString::fromUtf8("w_verb"));

        gridLayout1->addWidget(w_verb, 2, 0, 1, 1);

        w_adj = new QPushButton(msg_box);
        w_adj->setObjectName(QString::fromUtf8("w_adj"));

        gridLayout1->addWidget(w_adj, 2, 1, 1, 1);

        w_adv = new QPushButton(msg_box);
        w_adv->setObjectName(QString::fromUtf8("w_adv"));

        gridLayout1->addWidget(w_adv, 3, 0, 1, 1);

        w_pronomen = new QPushButton(msg_box);
        w_pronomen->setObjectName(QString::fromUtf8("w_pronomen"));

        gridLayout1->addWidget(w_pronomen, 3, 1, 1, 1);

        w_quesword = new QPushButton(msg_box);
        w_quesword->setObjectName(QString::fromUtf8("w_quesword"));

        gridLayout1->addWidget(w_quesword, 4, 0, 1, 2);

        w_interj = new QPushButton(msg_box);
        w_interj->setObjectName(QString::fromUtf8("w_interj"));

        gridLayout1->addWidget(w_interj, 5, 0, 1, 1);

        w_prep = new QPushButton(msg_box);
        w_prep->setObjectName(QString::fromUtf8("w_prep"));

        gridLayout1->addWidget(w_prep, 5, 1, 1, 1);

        w_interj2 = new QPushButton(msg_box);
        w_interj2->setObjectName(QString::fromUtf8("w_interj2"));

        gridLayout1->addWidget(w_interj2, 6, 0, 1, 2);

        w_adv_best = new QPushButton(msg_box);
        w_adv_best->setObjectName(QString::fromUtf8("w_adv_best"));

        gridLayout1->addWidget(w_adv_best, 7, 0, 1, 2);

        abbr_2 = new QPushButton(msg_box);
        abbr_2->setObjectName(QString::fromUtf8("abbr_2"));

        gridLayout1->addWidget(abbr_2, 8, 0, 1, 2);


        gridLayout_8->addWidget(msg_box, 0, 2, 2, 1);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setTabShape(QTabWidget::Rounded);
        Talk = new QWidget();
        Talk->setObjectName(QString::fromUtf8("Talk"));
        gridLayout2 = new QGridLayout(Talk);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        textedit_talk = new QTextEdit(Talk);
        textedit_talk->setObjectName(QString::fromUtf8("textedit_talk"));
        textedit_talk->setEnabled(true);
        QFont font;
        font.setKerning(false);
        textedit_talk->setFont(font);
        textedit_talk->setReadOnly(true);

        gridLayout2->addWidget(textedit_talk, 0, 0, 1, 3);

        lineedit_talk = new QLineEdit(Talk);
        lineedit_talk->setObjectName(QString::fromUtf8("lineedit_talk"));

        gridLayout2->addWidget(lineedit_talk, 1, 0, 1, 1);

        normalscreen = new QPushButton(Talk);
        normalscreen->setObjectName(QString::fromUtf8("normalscreen"));

        gridLayout2->addWidget(normalscreen, 1, 1, 1, 1);

        pushButton = new QPushButton(Talk);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout2->addWidget(pushButton, 1, 2, 1, 1);

        tabWidget->addTab(Talk, QString());
        Learn = new QWidget();
        Learn->setObjectName(QString::fromUtf8("Learn"));
        gridLayout_2 = new QGridLayout(Learn);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        learnbar = new QProgressBar(Learn);
        learnbar->setObjectName(QString::fromUtf8("learnbar"));
        learnbar->setEnabled(true);
        learnbar->setValue(0);

        gridLayout_2->addWidget(learnbar, 1, 0, 1, 2);

        pushButton_learn = new QPushButton(Learn);
        pushButton_learn->setObjectName(QString::fromUtf8("pushButton_learn"));

        gridLayout_2->addWidget(pushButton_learn, 2, 1, 1, 1);

        lineedit_learn = new QLineEdit(Learn);
        lineedit_learn->setObjectName(QString::fromUtf8("lineedit_learn"));

        gridLayout_2->addWidget(lineedit_learn, 2, 0, 1, 1);

        textedit_learn = new QTextEdit(Learn);
        textedit_learn->setObjectName(QString::fromUtf8("textedit_learn"));
        textedit_learn->setEnabled(true);
        textedit_learn->setFont(font);
        textedit_learn->setReadOnly(true);

        gridLayout_2->addWidget(textedit_learn, 0, 0, 1, 2);

        tabWidget->addTab(Learn, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_6 = new QGridLayout(tab);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        scrollArea = new QScrollArea(tab);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setFrameShadow(QFrame::Plain);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 420, 449));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(scrollAreaWidgetContents->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents->setSizePolicy(sizePolicy2);
        gridLayout_3 = new QGridLayout(scrollAreaWidgetContents);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));

        gridLayout_3->addLayout(gridLayout_4, 0, 0, 1, 6);

        verticalScrollBar = new QScrollBar(scrollAreaWidgetContents);
        verticalScrollBar->setObjectName(QString::fromUtf8("verticalScrollBar"));
        verticalScrollBar->setOrientation(Qt::Vertical);

        gridLayout_3->addWidget(verticalScrollBar, 0, 6, 1, 1);

        line = new QFrame(scrollAreaWidgetContents);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(line, 1, 0, 1, 7);

        label_2 = new QLabel(scrollAreaWidgetContents);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_3->addWidget(label_2, 2, 0, 1, 2);

        flowchart_input = new QLineEdit(scrollAreaWidgetContents);
        flowchart_input->setObjectName(QString::fromUtf8("flowchart_input"));

        gridLayout_3->addWidget(flowchart_input, 2, 2, 1, 1);

        compute_output = new QPushButton(scrollAreaWidgetContents);
        compute_output->setObjectName(QString::fromUtf8("compute_output"));

        gridLayout_3->addWidget(compute_output, 2, 3, 1, 2);

        refresh_chart = new QPushButton(scrollAreaWidgetContents);
        refresh_chart->setObjectName(QString::fromUtf8("refresh_chart"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(refresh_chart->sizePolicy().hasHeightForWidth());
        refresh_chart->setSizePolicy(sizePolicy3);

        gridLayout_3->addWidget(refresh_chart, 2, 5, 1, 2);

        line_2 = new QFrame(scrollAreaWidgetContents);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(line_2, 3, 0, 1, 7);

        label_3 = new QLabel(scrollAreaWidgetContents);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_3->addWidget(label_3, 4, 0, 1, 2);

        flowchart_view = new QLabel(scrollAreaWidgetContents);
        flowchart_view->setObjectName(QString::fromUtf8("flowchart_view"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(flowchart_view->sizePolicy().hasHeightForWidth());
        flowchart_view->setSizePolicy(sizePolicy4);

        gridLayout_3->addWidget(flowchart_view, 4, 2, 1, 5);

        label_4 = new QLabel(scrollAreaWidgetContents);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_3->addWidget(label_4, 5, 0, 1, 2);

        flowchart_edit = new QLineEdit(scrollAreaWidgetContents);
        flowchart_edit->setObjectName(QString::fromUtf8("flowchart_edit"));

        gridLayout_3->addWidget(flowchart_edit, 5, 2, 1, 5);

        label = new QLabel(scrollAreaWidgetContents);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_3->addWidget(label, 6, 0, 1, 2);

        flowchart_fact_pk = new QSpinBox(scrollAreaWidgetContents);
        flowchart_fact_pk->setObjectName(QString::fromUtf8("flowchart_fact_pk"));
        sizePolicy3.setHeightForWidth(flowchart_fact_pk->sizePolicy().hasHeightForWidth());
        flowchart_fact_pk->setSizePolicy(sizePolicy3);
        flowchart_fact_pk->setMaximum(999999999);

        gridLayout_3->addWidget(flowchart_fact_pk, 6, 2, 1, 1);

        plus = new QPushButton(scrollAreaWidgetContents);
        plus->setObjectName(QString::fromUtf8("plus"));
        sizePolicy3.setHeightForWidth(plus->sizePolicy().hasHeightForWidth());
        plus->setSizePolicy(sizePolicy3);

        gridLayout_3->addWidget(plus, 7, 0, 1, 1);

        minus = new QPushButton(scrollAreaWidgetContents);
        minus->setObjectName(QString::fromUtf8("minus"));
        sizePolicy3.setHeightForWidth(minus->sizePolicy().hasHeightForWidth());
        minus->setSizePolicy(sizePolicy3);

        gridLayout_3->addWidget(minus, 7, 1, 1, 2);

        flowchart_fact_edit = new QPushButton(scrollAreaWidgetContents);
        flowchart_fact_edit->setObjectName(QString::fromUtf8("flowchart_fact_edit"));
        sizePolicy3.setHeightForWidth(flowchart_fact_edit->sizePolicy().hasHeightForWidth());
        flowchart_fact_edit->setSizePolicy(sizePolicy3);

        gridLayout_3->addWidget(flowchart_fact_edit, 7, 3, 1, 1);

        flowchart_fact_delete = new QPushButton(scrollAreaWidgetContents);
        flowchart_fact_delete->setObjectName(QString::fromUtf8("flowchart_fact_delete"));
        sizePolicy3.setHeightForWidth(flowchart_fact_delete->sizePolicy().hasHeightForWidth());
        flowchart_fact_delete->setSizePolicy(sizePolicy3);

        gridLayout_3->addWidget(flowchart_fact_delete, 7, 4, 1, 2);

        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout_6->addWidget(scrollArea, 0, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        Database = new QWidget();
        Database->setObjectName(QString::fromUtf8("Database"));
        gridLayout_7 = new QGridLayout(Database);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        label_14 = new QLabel(Database);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout_7->addWidget(label_14, 0, 0, 1, 1);

        subject = new QLineEdit(Database);
        subject->setObjectName(QString::fromUtf8("subject"));

        gridLayout_7->addWidget(subject, 0, 1, 1, 1);

        label_12 = new QLabel(Database);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_7->addWidget(label_12, 0, 2, 1, 1);

        adverbs = new QLineEdit(Database);
        adverbs->setObjectName(QString::fromUtf8("adverbs"));
        sizePolicy3.setHeightForWidth(adverbs->sizePolicy().hasHeightForWidth());
        adverbs->setSizePolicy(sizePolicy3);

        gridLayout_7->addWidget(adverbs, 0, 3, 1, 1);

        label_15 = new QLabel(Database);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout_7->addWidget(label_15, 1, 0, 1, 1);

        verb = new QLineEdit(Database);
        verb->setObjectName(QString::fromUtf8("verb"));

        gridLayout_7->addWidget(verb, 1, 1, 1, 1);

        label_11 = new QLabel(Database);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_7->addWidget(label_11, 1, 2, 1, 1);

        questionword = new QLineEdit(Database);
        questionword->setObjectName(QString::fromUtf8("questionword"));
        sizePolicy3.setHeightForWidth(questionword->sizePolicy().hasHeightForWidth());
        questionword->setSizePolicy(sizePolicy3);

        gridLayout_7->addWidget(questionword, 1, 3, 1, 1);

        label_13 = new QLabel(Database);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_7->addWidget(label_13, 2, 0, 1, 1);

        object = new QLineEdit(Database);
        object->setObjectName(QString::fromUtf8("object"));

        gridLayout_7->addWidget(object, 2, 1, 1, 1);

        label_16 = new QLabel(Database);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout_7->addWidget(label_16, 2, 2, 1, 1);

        extra = new QLineEdit(Database);
        extra->setObjectName(QString::fromUtf8("extra"));
        sizePolicy3.setHeightForWidth(extra->sizePolicy().hasHeightForWidth());
        extra->setSizePolicy(sizePolicy3);

        gridLayout_7->addWidget(extra, 2, 3, 1, 1);

        frame = new QFrame(Database);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Plain);
        gridLayout_5 = new QGridLayout(frame);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        horizontalSpacer = new QSpacerItem(166, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer, 0, 0, 1, 1);

        matchingfacts = new QPushButton(frame);
        matchingfacts->setObjectName(QString::fromUtf8("matchingfacts"));

        gridLayout_5->addWidget(matchingfacts, 0, 1, 1, 1);

        allfacts = new QPushButton(frame);
        allfacts->setObjectName(QString::fromUtf8("allfacts"));
        sizePolicy4.setHeightForWidth(allfacts->sizePolicy().hasHeightForWidth());
        allfacts->setSizePolicy(sizePolicy4);

        gridLayout_5->addWidget(allfacts, 0, 2, 1, 1);

        tableView = new QTableView(frame);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        gridLayout_5->addWidget(tableView, 1, 0, 1, 3);


        gridLayout_7->addWidget(frame, 3, 0, 1, 4);

        tabWidget->addTab(Database, QString());

        gridLayout_8->addWidget(tabWidget, 0, 0, 2, 1);

        freehalWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(freehalWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 986, 22));
        menuDatei = new QMenu(menubar);
        menuDatei->setObjectName(QString::fromUtf8("menuDatei"));
        menuNeu = new QMenu(menuDatei);
        menuNeu->setObjectName(QString::fromUtf8("menuNeu"));
        menuExtras = new QMenu(menubar);
        menuExtras->setObjectName(QString::fromUtf8("menuExtras"));
        menuHilfe = new QMenu(menubar);
        menuHilfe->setObjectName(QString::fromUtf8("menuHilfe"));
        menuVerbindung = new QMenu(menubar);
        menuVerbindung->setObjectName(QString::fromUtf8("menuVerbindung"));
        freehalWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(freehalWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        freehalWindow->setStatusBar(statusbar);
        QWidget::setTabOrder(lineedit_talk, pushButton);

        menubar->addAction(menuDatei->menuAction());
        menubar->addAction(menuVerbindung->menuAction());
        menubar->addAction(menuExtras->menuAction());
        menubar->addAction(menuHilfe->menuAction());
        menuDatei->addAction(menuNeu->menuAction());
        menuDatei->addSeparator();
        menuDatei->addAction(actionBeenden);
        menuNeu->addAction(actionGespr_ch);
        menuExtras->addAction(actionEinstellungen);
        menuExtras->addSeparator();
        menuExtras->addAction(fullscreen);
        menuHilfe->addAction(actionInfo);
        menuVerbindung->addAction(actionNeue_Verbindung_herstellen);

        retranslateUi(freehalWindow);
        QObject::connect(actionBeenden, SIGNAL(triggered()), freehalWindow, SLOT(close()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(freehalWindow);
    } // setupUi

    void retranslateUi(QMainWindow *freehalWindow)
    {
        freehalWindow->setWindowTitle(QApplication::translate("freehalWindow", "Connect to artificial intelligence server", 0, QApplication::UnicodeUTF8));
        actionGespr_ch->setText(QApplication::translate("freehalWindow", "Talk", 0, QApplication::UnicodeUTF8));
        actionDatenbank->setText(QApplication::translate("freehalWindow", "Database", 0, QApplication::UnicodeUTF8));
        actionBeenden->setText(QApplication::translate("freehalWindow", "Exit", 0, QApplication::UnicodeUTF8));
        actionVorf_hrmodus->setText(QApplication::translate("freehalWindow", "Vorf\303\274hrmodus", 0, QApplication::UnicodeUTF8));
        actionBildschirm_l_schen->setText(QApplication::translate("freehalWindow", "Bildschirm l\303\266schen", 0, QApplication::UnicodeUTF8));
        actionDatenbank_editieren->setText(QApplication::translate("freehalWindow", "Datenbank editieren", 0, QApplication::UnicodeUTF8));
        actionOnlinedatenbank_laden->setText(QApplication::translate("freehalWindow", "Onlinedatenbank laden", 0, QApplication::UnicodeUTF8));
        actionIm_WWW_Surfen->setText(QApplication::translate("freehalWindow", "Im WWW surfen", 0, QApplication::UnicodeUTF8));
        actionOfflinemodus->setText(QApplication::translate("freehalWindow", "Offlinemodus", 0, QApplication::UnicodeUTF8));
        actionWikipedia_Artikel_Informationen_laden->setText(QApplication::translate("freehalWindow", "Wikipedia-Artikel-Informationen laden", 0, QApplication::UnicodeUTF8));
        actionWikipedia_Artikel_Informationen_speichern->setText(QApplication::translate("freehalWindow", "Wikipedia-Artikel-Informationen speichern", 0, QApplication::UnicodeUTF8));
        actionInfo->setText(QApplication::translate("freehalWindow", "Info", 0, QApplication::UnicodeUTF8));
        actionEinstellungen->setText(QApplication::translate("freehalWindow", "Preferences", 0, QApplication::UnicodeUTF8));
        fullscreen->setText(QApplication::translate("freehalWindow", "Presentation mode", 0, QApplication::UnicodeUTF8));
        actionSemantisches_Netz->setText(QApplication::translate("freehalWindow", "Semantisches Netz", 0, QApplication::UnicodeUTF8));
        actionNeue_Verbindung_herstellen->setText(QApplication::translate("freehalWindow", "Connect to an AI", 0, QApplication::UnicodeUTF8));
        action_scan_a_text->setText(QApplication::translate("freehalWindow", "einen Text scannen", 0, QApplication::UnicodeUTF8));
        msg_box_2->setTitle(QApplication::translate("freehalWindow", "FreeHAL needs some information", 0, QApplication::UnicodeUTF8));
        male->setText(QApplication::translate("freehalWindow", "male", 0, QApplication::UnicodeUTF8));
        female->setText(QApplication::translate("freehalWindow", "female", 0, QApplication::UnicodeUTF8));
        thing->setText(QApplication::translate("freehalWindow", "neuter", 0, QApplication::UnicodeUTF8));
        no_genus->setText(QApplication::translate("freehalWindow", "no gender", 0, QApplication::UnicodeUTF8));
        abbr_1->setText(QApplication::translate("freehalWindow", "Abort", 0, QApplication::UnicodeUTF8));
        msg_box->setTitle(QApplication::translate("freehalWindow", "FreeHAL needs some information", 0, QApplication::UnicodeUTF8));
        w_name->setText(QApplication::translate("freehalWindow", "Name", 0, QApplication::UnicodeUTF8));
        w_nomen->setText(QApplication::translate("freehalWindow", "Noun", 0, QApplication::UnicodeUTF8));
        w_verb->setText(QApplication::translate("freehalWindow", "Verb", 0, QApplication::UnicodeUTF8));
        w_adj->setText(QApplication::translate("freehalWindow", "Adjective", 0, QApplication::UnicodeUTF8));
        w_adv->setText(QApplication::translate("freehalWindow", "Adverb", 0, QApplication::UnicodeUTF8));
        w_pronomen->setText(QApplication::translate("freehalWindow", "Pronoun", 0, QApplication::UnicodeUTF8));
        w_quesword->setText(QApplication::translate("freehalWindow", "Question word / Conjunction", 0, QApplication::UnicodeUTF8));
        w_interj->setText(QApplication::translate("freehalWindow", "Interjection", 0, QApplication::UnicodeUTF8));
        w_prep->setText(QApplication::translate("freehalWindow", "Preposition", 0, QApplication::UnicodeUTF8));
        w_interj2->setText(QApplication::translate("freehalWindow", "General Word (e.g. greeting)", 0, QApplication::UnicodeUTF8));
        w_adv_best->setText(QApplication::translate("freehalWindow", "An adverb of time or place", 0, QApplication::UnicodeUTF8));
        abbr_2->setText(QApplication::translate("freehalWindow", "Abort", 0, QApplication::UnicodeUTF8));
        textedit_talk->setHtml(QApplication::translate("freehalWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'DejaVu Sans'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        normalscreen->setText(QApplication::translate("freehalWindow", " Exit ", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("freehalWindow", "Talk", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(Talk), QApplication::translate("freehalWindow", "Talk", 0, QApplication::UnicodeUTF8));
        pushButton_learn->setText(QApplication::translate("freehalWindow", "Learn", 0, QApplication::UnicodeUTF8));
        textedit_learn->setHtml(QApplication::translate("freehalWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'DejaVu Sans'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(Learn), QApplication::translate("freehalWindow", "Learn", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("freehalWindow", "Input text:", 0, QApplication::UnicodeUTF8));
        compute_output->setText(QApplication::translate("freehalWindow", "Answer again!", 0, QApplication::UnicodeUTF8));
        refresh_chart->setText(QApplication::translate("freehalWindow", "Refresh", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("freehalWindow", "Output fact:", 0, QApplication::UnicodeUTF8));
        flowchart_view->setText(QString());
        label_4->setText(QApplication::translate("freehalWindow", "Edit fact:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("freehalWindow", "Fact no:", 0, QApplication::UnicodeUTF8));
        plus->setText(QApplication::translate("freehalWindow", "+", 0, QApplication::UnicodeUTF8));
        minus->setText(QApplication::translate("freehalWindow", "-", 0, QApplication::UnicodeUTF8));
        flowchart_fact_edit->setText(QApplication::translate("freehalWindow", "Save fact", 0, QApplication::UnicodeUTF8));
        flowchart_fact_delete->setText(QApplication::translate("freehalWindow", "Delete fact", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("freehalWindow", "Chart", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("freehalWindow", "Subjekt:", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("freehalWindow", "Adv. B.", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("freehalWindow", "Verb:", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("freehalWindow", "Fragewort:", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("freehalWindow", "Objekt:", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("freehalWindow", "Zusatz:", 0, QApplication::UnicodeUTF8));
        matchingfacts->setText(QApplication::translate("freehalWindow", "passende Fakten", 0, QApplication::UnicodeUTF8));
        allfacts->setText(QApplication::translate("freehalWindow", "alle Fakten", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(Database), QApplication::translate("freehalWindow", "Database", 0, QApplication::UnicodeUTF8));
        menuDatei->setTitle(QApplication::translate("freehalWindow", "File", 0, QApplication::UnicodeUTF8));
        menuNeu->setTitle(QApplication::translate("freehalWindow", "New", 0, QApplication::UnicodeUTF8));
        menuExtras->setTitle(QApplication::translate("freehalWindow", "Tools", 0, QApplication::UnicodeUTF8));
        menuHilfe->setTitle(QApplication::translate("freehalWindow", "Help", 0, QApplication::UnicodeUTF8));
        menuVerbindung->setTitle(QApplication::translate("freehalWindow", "Connection", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class freehalWindow: public Ui_freehalWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FREEHAL_H
