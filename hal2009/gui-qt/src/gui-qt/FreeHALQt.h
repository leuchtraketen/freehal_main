#include "defs.h"

#define BOOST_THREAD_DYN_DLL 1
#define BOOST_THREAD_DYN_LINK 1

/*
 * This file is part of FreeHAL 2009.
 *
 * Copyright(c) 2006, 2007, 2008, 2009 Tobias Schulz and contributors.
 * http://freehal.org
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/


#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <queue>
#include <sstream>

#include <string>
#include <list>

#include <time.h>
#include <vector>
#include <unistd.h>
#include <getopt.h>

#include <algorithm>

#include <sys/time.h>

#include <stdlib.h>




#include <QtGui/QApplication>
#include <QtGui/QPushButton>
#include <QtGui/QGraphicsView>

#include <QtGui/QApplication>
#include <QtGui/QPainter>
#include <QtUiTools/QUiLoader>
#include <QtGui/QMainWindow>
#include <QtGui/QWidget>
#include <QtCore/QFile>
#include <QtCore/QMutexLocker>
#include <QtCore/QMutex>
#include <QtCore/QtCore>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtGui/QPainter>
#include <QtGui/QScrollArea>
#include <QtGui/QSystemTrayIcon>
#include <QtGui/QWidget>
#include <QtGui/QCloseEvent>
#include <QtGui/QStyle>

#ifndef QT_NO_OPENGL
    #include <QtOpenGL/QGLWidget>
#endif
//#define QT_NO_OPENGL

#include "ui_freehal.h"
#include "ui_options.h"
#include "ui_help.h"
#include "ui_changelog.h"
#include "ui_startup.h"
#include "ui_ee.h"
#include "ui_connect.h"
#include "ui_view.h"
#include "ui_more-info.h"

using namespace std;

extern auto_ptr<QString> freehalthread1_talking;

void make_connection(int);
void check_new_version();
int windows_invoke_runner();
int windows_update_svn();
int windows_compile_runner();
int windows_stop_runner();
int linux_update_svn();
int linux_compile_runner();
int linux_stop_runner();
int linux_invoke_runner();
int invoke_runner();
int compile_runner();
int stop_runner();
int update_svn();


// for DB Tool
class FactModel;
extern FactModel* factmodel;
enum LAST_PROCESS { MATCHING_FACTS, ALL_FACTS };

namespace freehal {
	void display_sentence(freehal::string);
	void comm_new(freehal::string);
        void log(std::string);
}

void make_connection(int);


class FreeHALWindow : public QMainWindow {
    Q_OBJECT

public:
    bool waitForStatusbar;

    void setVisible(bool);

    FreeHALWindow(QWidget *parent = 0);

    Ui::freehalWindow* user_interface_main_window;
    Ui::Dialog* user_interface_dialog;

     QAction *minimizeAction;
     QAction *maximizeAction;
     QAction *restoreAction;
     QAction *quitAction;

     QSystemTrayIcon *trayIcon;
     QMenu *trayIconMenu;

    void showWindowNormal();
    void showWindowFullscreen();
    void showWindowEE();


     void createIconGroupBox();
     void createMessageGroupBox();
     void createActions();
     void createTrayIcon();
     
     void setRenderingSystem();
    
protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void hideStartupDialog();
    void slotStatusbarUpdated(QString);
    void on_normalscreen_clicked();
    void on_fullscreen_triggered();
    void on_pushButton_clicked();
    void on_pushButton_learn_clicked();
    void button_talk(QString command, QComboBox* lineedit);
    void on_refresh_chart_clicked();
    void on_compute_output_clicked();
    void on_flowchart_fact_delete_clicked();
    void on_flowchart_fact_edit_clicked();
    void on_actionGespr_ch_triggered();
    void on_actionEinstellungen_triggered();
    void on_actionInfo_triggered();
    void on_actionConnect_triggered();
    void on_actionReconnect_triggered();
    void on_actionStop_Server_triggered();
    void on_actionStart_Server_triggered();
    void on_actionCompile_Server_triggered();
    void on_actionUpdate_SVN_triggered();

    void on_w_verb_clicked();
    void on_w_nomen_clicked();
    void on_w_name_clicked();
    void on_w_adj_clicked();
    void on_w_adv_clicked();
    void on_w_pronomen_clicked();
    void on_w_quesword_clicked();
    void on_w_prep_clicked();
    void on_w_adv_best_clicked();
    void on_w_interj_clicked();
    void on_w_interj2_clicked();

    void on_male_clicked();
    void on_female_clicked();
    void on_thing_clicked();
    void on_no_genus_clicked();

    void on_abbr_1_clicked();
    void on_abbr_2_clicked();

    void iconActivated(QSystemTrayIcon::ActivationReason);
    void show_in_tray_icon(QString);

    void slotEnable();

    void on_db_search_save_clicked();
    void on_plus_clicked();
    void on_minus_clicked();
    void on_flowchart_fact_pk_valueChanged(int );

// DB Tool



public:
    void setupMenu();
    Ui::freehalWindow* getDbToolUi();
    enum LAST_PROCESS getLastProcess();
    void setLastProcess(enum LAST_PROCESS p);
    QString make_csv();

private:
    enum LAST_PROCESS lastprocess;

public slots:
    void on_matchingfacts_clicked();
    void menuClick(QPoint);

private slots:
    void on_extra_returnPressed();
    void on_questionword_returnPressed();
    void on_adverbs_returnPressed();
    void on_object_returnPressed();
    void on_verb_returnPressed();
    void on_subject_returnPressed();
    void on_allfacts_clicked();

    void on_double_facts_clicked();

};

class Dialog1 : public QDialog {
    Q_OBJECT

public:
    Ui::freehalWindow* user_interface_main_window;
    Ui::Dialog* user_interface_dialog;

public slots:
//    void on_fullscreen_clicked();
    void on_ask_frame_toggled(bool checked);
    void on_ask_dialog_toggled(bool checked);
    void on_limit_amount_of_answers_toggled(bool checked);

    void on_clear_screen_clicked();
    void on_proxy_ok_clicked();
    void on_mode_www_toggled();
    void on_mode_offline_stateChanged(int);
    void on_speech_stateChanged(int);
    void on_access_online_db_toggled();

    friend void freehal::comm_new(freehal::string);
};

class Dialog3 : public QDialog {
    Q_OBJECT

public:
    Ui::freehalWindow* user_interface_main_window;
    Ui::HelpDialog* user_interface_dialog;

public slots:
    void on_changelog_clicked();
};

class Dialog4 : public QDialog {
    Q_OBJECT

public:
    Ui::freehalWindow* user_interface_main_window;
    Ui::ChangelogDialog* user_interface_dialog;

    void init_changelog();
};

class Dialog5 : public QDialog {
    Q_OBJECT

public:
    void updateStatusBar(float fl);

    Ui::freehalWindow* user_interface_main_window;
    Ui::Startup* user_interface_dialog;

signals:
    void signalUpdateStatusbar(QString);

};

class Dialog6 : public QDialog {
    Q_OBJECT

public:
    void updateStatusBar(float fl);

    Ui::freehalWindow* user_interface_main_window;
    Ui::EasterEgg* user_interface_dialog;
};

class Dialog8 : public QDialog {
    Q_OBJECT

public:
    void updateStatusBar(float fl);

    Ui::freehalWindow* user_interface_main_window;
    Ui::ViewDialog* user_interface_dialog;
};

class Helper : public QObject {
    Q_OBJECT

    friend void freehal::display_sentence(freehal::string);
    friend void freehal::comm_new(freehal::string);
    friend void freehal::log(std::string);
    friend void make_connection(int);
    friend void check_new_version();
    friend void FreeHALWindow::on_actionConnect_triggered();
    friend void client(std::string, bool, bool);
    friend void thread_learn_progress_bar(int max);


public slots:
    void exitNow();
    void slotMoreInfo(QStringList);
    void showMsgWindow();
    void hideMsgWindow();
    void showMsgGenusWindow();
    void hideMsgGenusWindow();
    void slotEverythingReady();
    void close_view();
    void reconnect();
    void slotNewVersionOnline(QString);


signals:
    void signalNewVersionOnline(QString);

    void signalTalkingScrollEndTalk(QString);
    void signalAnswerTalk(QString);
    void signalTalkingScrollEndLearn(QString);
    void signalAnswerLearn(QString);
    void signalUpdateStatusbar(QString);
    void signalUpdateProgressbar(int);
    void signalHideProgressbar();
    void signalShowProgressbar();
    void signalLogAppend(QString);
    void signalLogClear(QString);
    void signalLogScrollEnd(QString);
    void signalChangeWindowTitle(QString);
    void signalChangeHelpWindowText1(QString);
    void signalChangeHelpWindowText2(QString);
    void signalMsg(QString);
    void signalMoreInfo(QStringList);
    void signalMsgGenus(QString);
    void signalShowMsgWindow();
    void signalHideMsgWindow();
    void signalShowMsgGenusWindow();
    void signalHideMsgGenusWindow();
    void signalReconnect();
    void everythingReady();
    void sig_show_in_tray_icon(QString);
    void signalSetTimeToLearn(int);
    void signalSetTimeToLearnElapsed(int);
    void signalEnable();
};


class ConnectDialog : public QDialog {
    Q_OBJECT

public:
    Ui::ConnectDialog* from;
};


class MoreInfo : public QDialog {
    Q_OBJECT

public:
    Ui::freehalWindow* user_interface_main_window;
    Ui::MoreInfo* user_interface_dialog;

public slots:
    void on_w_v_clicked();
    void on_w_n_clicked();
    void on_w_a_clicked();
    void on_w_i_clicked();
    void on_w_c_clicked();
    void on_w_p_clicked();

    void on_g_male_clicked();
    void on_g_female_clicked();
    void on_g_neuter_clicked();
    void on_g_unknown_clicked();

private slots:
    void on_buttonBox_clicked(QAbstractButton* button);
};


class DummyQObject : public QObject {
    Q_OBJECT
};

struct ClickPositionItem {
    int beginY;
    int endY;
    char* prop;
    char* value;
    char* text;
};

class FlowChart : public QWidget {
    Q_OBJECT

    friend void freehal::comm_new(freehal::string);

  public:
    FlowChart(QWidget*);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    Ui::freehalWindow* user_interface_main_window;

  signals:
    void ask_again();
    void setFactText_Chart(QString);
    void setFactText_DB(QString);

  protected:
    void paintEvent(QPaintEvent *event);
  public slots:
    void setY(int);
};




///////////////////////////////////////////////////////////////////////////////
// Database Tab (DB Tool)

const int max_amount_of_answers_in_db_tool = 100000;

void init_db_tool();

struct DATASET {
    char*** data;
    long int size;
    long int column_count;
    long int timeout;
    int err;
};

void clear_dataset();
struct DATASET* make_dataset();
void add_to_dataset(struct DATASET* set, const char* csv);

#define MAX_CLAUSES 10
#define LINE_SIZE 4096
#define FACT struct RECORD
#define SHORTFACT struct RECORD_short
#define factsize sizeof(FACT)+32
struct RECORD;
struct RECORD {
    char verb[LINE_SIZE];
    char subjects[LINE_SIZE];
    char objects[LINE_SIZE];
    char adverbs[LINE_SIZE];
    char from[LINE_SIZE];
    char extra[LINE_SIZE];
    char questionword[LINE_SIZE];
    char context[LINE_SIZE];
    char pkey[LINE_SIZE];
    int prio;
    void* clauses[MAX_CLAUSES];
    int num_clauses;
    short type;
    int hash_clauses;
    double truth;

    short verb_flag_want;
    short verb_flag_must;
    short verb_flag_can;
    short verb_flag_may;
    short verb_flag_should;

    short everything_q;
};

class FactModel;
class Fact;

class FactModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    FactModel(QObject *parent = 0)
        : QAbstractTableModel(parent) {

        setData(make_dataset());
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int count() const;
    QModelIndex index(int row, int column, const QModelIndex & parent) const;
    void computeIndex(int row, int column);
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const;
    void updateData();
    void setData(struct DATASET* set);
    struct DATASET* getData();
    bool hasData();

private:
    struct DATASET* dataset;
    QMap<int, QStringList > indexcache;
};

class Fact : public QObject
{
    Q_OBJECT

public:
    Fact(char* _pk, FreeHALWindow* _window) : pk(_pk), window(_window) {}

public slots:
    void edit_fact();
    void delete_fact();

private:
    char* pk;
    FreeHALWindow* window;
};


extern "C" void init_sql();


