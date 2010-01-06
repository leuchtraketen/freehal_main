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

void make_connection();
void check_new_version();

namespace freehal {
	void display_sentence(freehal::string);
	void comm_new(freehal::string);
        void log(std::string);
}

void make_connection();


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
    void button_talk(QString command, QLineEdit* lineedit);
    void on_refresh_chart_clicked();
    void on_compute_output_clicked();
    void on_flowchart_fact_delete_clicked();
    void on_flowchart_fact_edit_clicked();
    void on_lineedit_talk_returnPressed();
    void on_lineedit_learn_returnPressed();
    void on_actionGespr_ch_triggered();
    void on_actionEinstellungen_triggered();
    void on_actionInfo_triggered();
    void on_actionNeue_Verbindung_herstellen_triggered();

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


private slots:
    void on_plus_clicked();
    void on_minus_clicked();
    void on_flowchart_fact_pk_valueChanged(int );
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
    friend void make_connection();
    friend void check_new_version();
    friend void FreeHALWindow::on_actionNeue_Verbindung_herstellen_triggered();
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
    void setFactText(QString);

  protected:
    void paintEvent(QPaintEvent *event);
  public slots:
    void setY(int);
};

