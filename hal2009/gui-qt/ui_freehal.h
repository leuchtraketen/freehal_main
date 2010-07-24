/********************************************************************************
** Form generated from reading UI file 'freehal.ui'
**
** Created: Sat Jul 24 11:36:21 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FREEHAL_H
#define UI_FREEHAL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QScrollBar>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableView>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_freehalWindow
{
public:
    QAction *actionNewTalk;
    QAction *actionBeenden;
    QAction *actionInfo;
    QAction *actionEinstellungen;
    QAction *actionFullscreen;
    QAction *actionConnect;
    QAction *actionReconnect;
    QAction *actionStop_Server;
    QAction *actionStart_Server;
    QAction *actionCompile_Server;
    QAction *actionUpdate_SVN;
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
    QVBoxLayout *verticalLayout;
    QTextEdit *textedit_talk;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout;
    QComboBox *lineedit_talk;
    QPushButton *normalscreen;
    QPushButton *pushButton;
    QWidget *Learn;
    QVBoxLayout *verticalLayout_2;
    QTextEdit *textedit_learn;
    QFrame *frame_3;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *lineedit_learn;
    QPushButton *pushButton_learn;
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
    QLabel *label_6;
    QSpacerItem *horizontalSpacer;
    QPushButton *matchingfacts;
    QPushButton *allfacts;
    QLabel *label_5;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *double_facts;
    QTableView *tableView;
    QFrame *db_search_edit_frame;
    QGridLayout *gridLayout_9;
    QLabel *label_7;
    QLineEdit *db_search_edit;
    QPushButton *db_search_save;
    QMenuBar *menubar;
    QMenu *menuDatei;
    QMenu *menuExtras;
    QMenu *menuHilfe;
    QMenu *menuVerbindung;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *freehalWindow)
    {
        if (freehalWindow->objectName().isEmpty())
            freehalWindow->setObjectName(QString::fromUtf8("freehalWindow"));
        freehalWindow->resize(986, 539);
        freehalWindow->setMaximumSize(QSize(16777215, 16000000));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/ui/ui/icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        freehalWindow->setWindowIcon(icon);
        freehalWindow->setAnimated(false);
        freehalWindow->setDockNestingEnabled(true);
        freehalWindow->setUnifiedTitleAndToolBarOnMac(false);
        actionNewTalk = new QAction(freehalWindow);
        actionNewTalk->setObjectName(QString::fromUtf8("actionNewTalk"));
        actionBeenden = new QAction(freehalWindow);
        actionBeenden->setObjectName(QString::fromUtf8("actionBeenden"));
        actionInfo = new QAction(freehalWindow);
        actionInfo->setObjectName(QString::fromUtf8("actionInfo"));
        actionEinstellungen = new QAction(freehalWindow);
        actionEinstellungen->setObjectName(QString::fromUtf8("actionEinstellungen"));
        actionFullscreen = new QAction(freehalWindow);
        actionFullscreen->setObjectName(QString::fromUtf8("actionFullscreen"));
        actionConnect = new QAction(freehalWindow);
        actionConnect->setObjectName(QString::fromUtf8("actionConnect"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/ui/ui/connect_creating.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionConnect->setIcon(icon1);
        actionReconnect = new QAction(freehalWindow);
        actionReconnect->setObjectName(QString::fromUtf8("actionReconnect"));
        actionReconnect->setIcon(icon1);
        actionStop_Server = new QAction(freehalWindow);
        actionStop_Server->setObjectName(QString::fromUtf8("actionStop_Server"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/ui/ui/media-playback-stop.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionStop_Server->setIcon(icon2);
        actionStart_Server = new QAction(freehalWindow);
        actionStart_Server->setObjectName(QString::fromUtf8("actionStart_Server"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/ui/ui/media-playback-start.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionStart_Server->setIcon(icon3);
        actionCompile_Server = new QAction(freehalWindow);
        actionCompile_Server->setObjectName(QString::fromUtf8("actionCompile_Server"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/ui/ui/document-properties.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionCompile_Server->setIcon(icon4);
        actionUpdate_SVN = new QAction(freehalWindow);
        actionUpdate_SVN->setObjectName(QString::fromUtf8("actionUpdate_SVN"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/ui/ui/view-refresh.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionUpdate_SVN->setIcon(icon5);
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
        verticalLayout = new QVBoxLayout(Talk);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        textedit_talk = new QTextEdit(Talk);
        textedit_talk->setObjectName(QString::fromUtf8("textedit_talk"));
        textedit_talk->setEnabled(true);
        QFont font;
        font.setKerning(false);
        textedit_talk->setFont(font);
        textedit_talk->setReadOnly(true);

        verticalLayout->addWidget(textedit_talk);

        frame_2 = new QFrame(Talk);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::NoFrame);
        frame_2->setFrameShadow(QFrame::Plain);
        horizontalLayout = new QHBoxLayout(frame_2);
#ifndef Q_OS_MAC
        horizontalLayout->setSpacing(6);
#endif
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lineedit_talk = new QComboBox(frame_2);
        lineedit_talk->setObjectName(QString::fromUtf8("lineedit_talk"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lineedit_talk->sizePolicy().hasHeightForWidth());
        lineedit_talk->setSizePolicy(sizePolicy2);
        lineedit_talk->setEditable(true);
        lineedit_talk->setMaxVisibleItems(20);
        lineedit_talk->setInsertPolicy(QComboBox::NoInsert);
        lineedit_talk->setFrame(true);

        horizontalLayout->addWidget(lineedit_talk);

        normalscreen = new QPushButton(frame_2);
        normalscreen->setObjectName(QString::fromUtf8("normalscreen"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(normalscreen->sizePolicy().hasHeightForWidth());
        normalscreen->setSizePolicy(sizePolicy3);

        horizontalLayout->addWidget(normalscreen);

        pushButton = new QPushButton(frame_2);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        sizePolicy3.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy3);

        horizontalLayout->addWidget(pushButton);


        verticalLayout->addWidget(frame_2);

        tabWidget->addTab(Talk, QString());
        Learn = new QWidget();
        Learn->setObjectName(QString::fromUtf8("Learn"));
        verticalLayout_2 = new QVBoxLayout(Learn);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        textedit_learn = new QTextEdit(Learn);
        textedit_learn->setObjectName(QString::fromUtf8("textedit_learn"));
        textedit_learn->setEnabled(true);
        textedit_learn->setFont(font);
        textedit_learn->setReadOnly(true);

        verticalLayout_2->addWidget(textedit_learn);

        frame_3 = new QFrame(Learn);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setFrameShape(QFrame::NoFrame);
        frame_3->setFrameShadow(QFrame::Plain);
        horizontalLayout_2 = new QHBoxLayout(frame_3);
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lineedit_learn = new QComboBox(frame_3);
        lineedit_learn->setObjectName(QString::fromUtf8("lineedit_learn"));
        lineedit_learn->setEditable(true);
        lineedit_learn->setMaxVisibleItems(20);
        lineedit_learn->setInsertPolicy(QComboBox::NoInsert);

        horizontalLayout_2->addWidget(lineedit_learn);

        pushButton_learn = new QPushButton(frame_3);
        pushButton_learn->setObjectName(QString::fromUtf8("pushButton_learn"));
        sizePolicy3.setHeightForWidth(pushButton_learn->sizePolicy().hasHeightForWidth());
        pushButton_learn->setSizePolicy(sizePolicy3);

        horizontalLayout_2->addWidget(pushButton_learn);


        verticalLayout_2->addWidget(frame_3);

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
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 461, 399));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(scrollAreaWidgetContents->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents->setSizePolicy(sizePolicy4);
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
        sizePolicy2.setHeightForWidth(flowchart_view->sizePolicy().hasHeightForWidth());
        flowchart_view->setSizePolicy(sizePolicy2);

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
        label_6 = new QLabel(frame);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_5->addWidget(label_6, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(166, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer, 0, 1, 1, 2);

        matchingfacts = new QPushButton(frame);
        matchingfacts->setObjectName(QString::fromUtf8("matchingfacts"));

        gridLayout_5->addWidget(matchingfacts, 0, 3, 1, 1);

        allfacts = new QPushButton(frame);
        allfacts->setObjectName(QString::fromUtf8("allfacts"));
        sizePolicy2.setHeightForWidth(allfacts->sizePolicy().hasHeightForWidth());
        allfacts->setSizePolicy(sizePolicy2);

        gridLayout_5->addWidget(allfacts, 0, 4, 1, 1);

        label_5 = new QLabel(frame);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_5->addWidget(label_5, 1, 0, 1, 2);

        horizontalSpacer_2 = new QSpacerItem(119, 22, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_3, 1, 3, 1, 1);

        double_facts = new QPushButton(frame);
        double_facts->setObjectName(QString::fromUtf8("double_facts"));

        gridLayout_5->addWidget(double_facts, 1, 4, 1, 1);

        tableView = new QTableView(frame);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        gridLayout_5->addWidget(tableView, 2, 0, 1, 5);

        db_search_edit_frame = new QFrame(frame);
        db_search_edit_frame->setObjectName(QString::fromUtf8("db_search_edit_frame"));
        db_search_edit_frame->setFrameShape(QFrame::StyledPanel);
        db_search_edit_frame->setFrameShadow(QFrame::Raised);
        gridLayout_9 = new QGridLayout(db_search_edit_frame);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        label_7 = new QLabel(db_search_edit_frame);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_9->addWidget(label_7, 0, 0, 1, 1);

        db_search_edit = new QLineEdit(db_search_edit_frame);
        db_search_edit->setObjectName(QString::fromUtf8("db_search_edit"));

        gridLayout_9->addWidget(db_search_edit, 0, 1, 1, 1);

        db_search_save = new QPushButton(db_search_edit_frame);
        db_search_save->setObjectName(QString::fromUtf8("db_search_save"));
        sizePolicy3.setHeightForWidth(db_search_save->sizePolicy().hasHeightForWidth());
        db_search_save->setSizePolicy(sizePolicy3);

        gridLayout_9->addWidget(db_search_save, 0, 2, 1, 1);


        gridLayout_5->addWidget(db_search_edit_frame, 3, 0, 1, 5);


        gridLayout_7->addWidget(frame, 3, 0, 1, 4);

        tabWidget->addTab(Database, QString());

        gridLayout_8->addWidget(tabWidget, 0, 0, 2, 1);

        freehalWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(freehalWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 986, 21));
        menuDatei = new QMenu(menubar);
        menuDatei->setObjectName(QString::fromUtf8("menuDatei"));
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
        toolBar = new QToolBar(freehalWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setIconSize(QSize(16, 16));
        freehalWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuDatei->menuAction());
        menubar->addAction(menuVerbindung->menuAction());
        menubar->addAction(menuExtras->menuAction());
        menubar->addAction(menuHilfe->menuAction());
        menuDatei->addAction(actionNewTalk);
        menuDatei->addSeparator();
        menuDatei->addAction(actionBeenden);
        menuExtras->addAction(actionEinstellungen);
        menuExtras->addSeparator();
        menuExtras->addAction(actionFullscreen);
        menuHilfe->addAction(actionInfo);
        menuVerbindung->addAction(actionConnect);
        menuVerbindung->addAction(actionReconnect);
        menuVerbindung->addAction(actionStop_Server);
        menuVerbindung->addAction(actionStart_Server);
        menuVerbindung->addAction(actionCompile_Server);
        menuVerbindung->addAction(actionUpdate_SVN);
        toolBar->addAction(actionReconnect);
        toolBar->addAction(actionStart_Server);
        toolBar->addAction(actionStop_Server);
        toolBar->addSeparator();
        toolBar->addAction(actionUpdate_SVN);
        toolBar->addAction(actionCompile_Server);

        retranslateUi(freehalWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(freehalWindow);
    } // setupUi

    void retranslateUi(QMainWindow *freehalWindow)
    {
        freehalWindow->setWindowTitle(QApplication::translate("freehalWindow", "Connect to artificial intelligence server", 0, QApplication::UnicodeUTF8));
        actionNewTalk->setText(QApplication::translate("freehalWindow", "New Talk", 0, QApplication::UnicodeUTF8));
        actionBeenden->setText(QApplication::translate("freehalWindow", "Exit", 0, QApplication::UnicodeUTF8));
        actionInfo->setText(QApplication::translate("freehalWindow", "Info", 0, QApplication::UnicodeUTF8));
        actionEinstellungen->setText(QApplication::translate("freehalWindow", "Preferences", 0, QApplication::UnicodeUTF8));
        actionFullscreen->setText(QApplication::translate("freehalWindow", "Presentation mode", 0, QApplication::UnicodeUTF8));
        actionConnect->setText(QApplication::translate("freehalWindow", "Connect", 0, QApplication::UnicodeUTF8));
        actionConnect->setShortcut(QApplication::translate("freehalWindow", "F6", 0, QApplication::UnicodeUTF8));
        actionReconnect->setText(QApplication::translate("freehalWindow", "Reconnect", 0, QApplication::UnicodeUTF8));
        actionReconnect->setShortcut(QApplication::translate("freehalWindow", "F5", 0, QApplication::UnicodeUTF8));
        actionStop_Server->setText(QApplication::translate("freehalWindow", "Stop Server", 0, QApplication::UnicodeUTF8));
        actionStop_Server->setShortcut(QApplication::translate("freehalWindow", "Ctrl+D", 0, QApplication::UnicodeUTF8));
        actionStart_Server->setText(QApplication::translate("freehalWindow", "Start Server", 0, QApplication::UnicodeUTF8));
        actionStart_Server->setShortcut(QApplication::translate("freehalWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionCompile_Server->setText(QApplication::translate("freehalWindow", "Compile Server", 0, QApplication::UnicodeUTF8));
        actionCompile_Server->setShortcut(QApplication::translate("freehalWindow", "Ctrl+G", 0, QApplication::UnicodeUTF8));
        actionUpdate_SVN->setText(QApplication::translate("freehalWindow", "Update SVN", 0, QApplication::UnicodeUTF8));
        actionUpdate_SVN->setShortcut(QApplication::translate("freehalWindow", "Ctrl+U", 0, QApplication::UnicodeUTF8));
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
"</style></head><body style=\" font-family:'Sans'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'DejaVu Sans'; font-size:10pt;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        lineedit_talk->clear();
        lineedit_talk->insertItems(0, QStringList()
         << QString()
        );
        normalscreen->setText(QApplication::translate("freehalWindow", " Exit ", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("freehalWindow", "Talk", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(Talk), QApplication::translate("freehalWindow", "Talk", 0, QApplication::UnicodeUTF8));
        textedit_learn->setHtml(QApplication::translate("freehalWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'DejaVu Sans'; font-size:10pt;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        lineedit_learn->clear();
        lineedit_learn->insertItems(0, QStringList()
         << QString()
        );
        pushButton_learn->setText(QApplication::translate("freehalWindow", "Learn", 0, QApplication::UnicodeUTF8));
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
        label_14->setText(QApplication::translate("freehalWindow", "Subject:", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("freehalWindow", "Adverbs:", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("freehalWindow", "Verb:", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("freehalWindow", "Question word:", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("freehalWindow", "Object:", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("freehalWindow", "Extra:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("freehalWindow", "Query:", 0, QApplication::UnicodeUTF8));
        matchingfacts->setText(QApplication::translate("freehalWindow", "matching facts", 0, QApplication::UnicodeUTF8));
        allfacts->setText(QApplication::translate("freehalWindow", "all facts", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("freehalWindow", "General database queries:", 0, QApplication::UnicodeUTF8));
        double_facts->setText(QApplication::translate("freehalWindow", "double facts", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("freehalWindow", "Edit fact:", 0, QApplication::UnicodeUTF8));
        db_search_save->setText(QApplication::translate("freehalWindow", "Save fact", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(Database), QApplication::translate("freehalWindow", "Database", 0, QApplication::UnicodeUTF8));
        menuDatei->setTitle(QApplication::translate("freehalWindow", "File", 0, QApplication::UnicodeUTF8));
        menuExtras->setTitle(QApplication::translate("freehalWindow", "Tools", 0, QApplication::UnicodeUTF8));
        menuHilfe->setTitle(QApplication::translate("freehalWindow", "Help", 0, QApplication::UnicodeUTF8));
        menuVerbindung->setTitle(QApplication::translate("freehalWindow", "Server", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("freehalWindow", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class freehalWindow: public Ui_freehalWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FREEHAL_H
