#include "defs.h"
#include "FreeHALQt.h"
#include "unistd.h"

#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>

#include <QInputDialog>
#define BOOST_THREAD_DYN_DLL 1
#define BOOST_THREAD_DYN_LINK 1
#include <boost/thread.hpp>

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

#ifndef linux
# ifndef __APPLE__
#  include <windows.h>
# endif
#endif


#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>

using boost::shared_ptr;


double*                     JELIZA_PROGRESS;
FreeHALWindow*               main_window;
Dialog1*                    dialog_options;
Dialog3*                    dialog_help;
Dialog4*                    dialog_changelog;
Dialog5*                    dialog_startup;
Dialog8*                    dialog_view;
ConnectDialog*              dialog_connection;
Helper*                     helper;
FlowChart*                  fc;
QMutex                      log_mutex;
int                         NOTHING_TO_DO = 0;
int                         ANSWER_TO_DO = 5;
string*                     global_ques;
int                         count_loop;
bool                        global_fertig = false;
bool                        startedUp = false;
string                      must_be_logged;
QProcess*                   kernel_process;
bool                        can_do_exit = false;
std::string                 database_engine = "sqlite";

bool offline_mode = false;
bool www_surf_mode = true;
freehal::string ki_name;
freehal::string version;

bool                        is_perl = 0;

auto_ptr<QString> freehalthread1_talking;

FreeHALWindow::FreeHALWindow(QWidget *parent)
: QMainWindow(parent)
{
	if (QSystemTrayIcon::isSystemTrayAvailable()) {
    	createActions();
    	createTrayIcon();
	    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            	this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
	             
	    QIcon icon("img/icon.png");
	    trayIcon->setIcon(icon);
     	
    	trayIcon->show();
    	
    	connect(&(*helper), SIGNAL(sig_show_in_tray_icon(QString)),
    			this, SLOT(show_in_tray_icon(QString)));
	}
}

void freehal::display_sentence(freehal::string s) {
    emit helper->signalAnswer(QString((s.ref() + "<br><a name=\"anch\" id=\"anch\">&nbsp;</a>").c_str()));
    emit helper->signalTalkingScrollEnd(QString("anch"));

}

void FreeHALWindow::hideStartupDialog() {
    dialog_startup->hide();
    main_window->showWindowNormal();
}

freehal::string ascii(QString qs) {
    qs = qs.replace(QString("Ã¼"), QString("ue"));
	qs = qs.replace(QString("Ã"), QString("ss"));
	qs = qs.replace(QString("Ã¤"), QString("ae"));
	qs = qs.replace(QString("ü"), QString("ue"));
	qs = qs.replace(QString("ß"), QString("ss"));
	qs = qs.replace(QString("ä"), QString("ae"));
	qs = qs.replace(QString("ö"), QString("oe"));
	qs = qs.replace(QString("Ü"), QString("Ue"));
	qs = qs.replace(QString("Ä"), QString("Ae"));
	qs = qs.replace(QString("Ö"), QString("Oe"));
	qs = qs.replace(QString("Ã¢ÂÂ"), QString("\""));
	qs = qs.replace(QString("Ã¢ÂÂ"), QString("\""));
	qs = qs.replace(QString("&lt;/br&gt;"), QString(" "));
	qs = qs.replace(QString(":"), QString("Fjnbvfghnbvcfdghbvcfgnbcxdfghcfg"));
	qs = qs.replace(QString("Fjnbvfghnbvcfdghbvcfgnbcxdfghcfg"), QString("::"));
	std::wstring ws(qs.toStdWString());
	freehal::string ques;
	for (unsigned int x = 0; x < ws.size(); x++) {
	    unsigned long y = ws[x];
	    cout << ws[x] << " " << y << endl;
	    if (y == 228) {
	        ques += "ae";
	    }
	    else if (y == 196) {
	        ques += "Ae";
	    }
	    else if (y == 252) {
	        ques += "ue";
	    }
	    else if (y == 220) {
	        ques += "Ue";
	    }
	    else if (y == 246) {
	        ques += "oe";
	    }
	    else if (y == 214) {
	        ques += "Oe";
	    }
	    else if (y == 223) {
	        ques += "ss";
	    }

        else {
            char array[2];
            array[0] = y;
            array[1] = '\0';
	        ques += array;
	    }
	}


    ques = ques.toASCII();
    
    return ques;
}
void FreeHALWindow::on_pushButton_clicked() {
    QString qs = user_interface_main_window->lineEdit->displayText();
    freehal::string ques = ascii(qs);
    user_interface_main_window->lineEdit->setText(QString());
    if (ques.get_lower().contains("windows") && ques.get_lower().contains("gut")) {
        main_window->showWindowEE();
    }

    freehal::comm_send("QUESTION:" + ques);
}

void FreeHALWindow::on_compute_output_clicked() {
    QString qs = user_interface_main_window->flowchart_input->displayText();
    freehal::string ques = ascii(qs);

    freehal::comm_send("QUESTION:" + ques);
}

void FreeHALWindow::on_flowchart_fact_delete_clicked() {
    QString qs;
    qs.setNum(user_interface_main_window->flowchart_fact_pk->value());

    freehal::comm_send("DELETE:FACT:PK:" + qs.toStdString());
}

void FreeHALWindow::on_pushButton_learn_clicked() {
    QString qs = user_interface_main_window->lineEdit->displayText();
    freehal::string ques = ascii(qs);
    user_interface_main_window->lineEdit->setText(QString());
    ques = "learn:" + ques;
    if (ques.get_lower().contains("windows") && ques.get_lower().contains("gut")) {
        main_window->showWindowEE();
    }

    freehal::comm_send("QUESTION:" + ques);
}

void FreeHALWindow::on_lineEdit_returnPressed() {
    on_pushButton_clicked();
}

void Helper::exitNow() {
        int ret = QMessageBox::warning(main_window, tr("FreeHAL"),
            tr("Should FreeHAL continue in the background?"),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No);

        if (ret == QMessageBox::No) {
    	freehal::comm_send("EXIT:EXIT");
	}
    delete dialog_options;
    delete dialog_help;
    delete dialog_changelog;
    delete dialog_startup;
    delete dialog_view;
    std::exit(0);
}


void check_new_version() {
    // dont't check. It does not make sense currently
    return;

    while (!version.size()) {
            freehal::msleep(1000);
    }

    freehal::string new_online(
            freehal::download(
            std::string("http://developer.freehal.org/down/version") + (is_perl?"_perl":"")));
    new_online = new_online.replace("r", "");
    new_online = new_online.replace("rev", "");
    new_online = new_online.replace(".", "");
    new_online.strip();

    if (new_online != version) {
        emit helper->signalNewVersionOnline(QString(new_online.ref().c_str()));
    }
}

void Helper::slotNewVersionOnline(QString new_online) {
    if (new_online.size())
        QMessageBox::warning(main_window, tr("Update"),
                "There is an update available.\n"
                "The version number of the update is: Revision " + new_online + "\n\n"
                "You can download it at our project website:\n"
                "http://www.freehal.org/\n\n",
                QMessageBox::Yes,
                QMessageBox::Yes);
}

int windows_invoke_runner() {
    for (int i = 0; i < 10; ++i) {
        if (!std::system(("runner " + freehal::get_lang_str().ref() + " " + database_engine).c_str()))
            return 0;
        if (!std::system(("runner.exe " + freehal::get_lang_str().ref() + " " + database_engine).c_str()))
            return 0;
        if (!std::system((".\\runner " + freehal::get_lang_str().ref() + " " + database_engine).c_str()))
            return 0;
        freehal::msleep(1000);
    }

    return 1;
}

void make_connection() {
    dialog_connection->exec();
    if (dialog_connection->result() == QDialog::Rejected) {
        std::exit(0);
    }

    database_engine = dialog_connection->from->database_engine->currentText().toStdString();
    if (database_engine == "disk (traditional)")
        database_engine = "sqlite";
    if (database_engine.size() < 3 || (database_engine != "ram" && database_engine != "disk" && database_engine != "sqlite"))
        database_engine = "disk";

    string ip = freehal::string(dialog_connection->from->ip->displayText().toStdString()).get_strip().ref();

    main_window->showWindowNormal();

    QFile booted_file("booted");
    booted_file.remove();

    if (ip == "127.0.0.1" && dialog_connection->from->start_kernel->isChecked()) {

#ifdef linux
    std::system("rm booted");
    
    ifstream i_1("hal2009-server");
    if (i_1) {
    	boost::thread t_v(boost::bind(std::system, ("./hal2009-server " + freehal::get_lang_str().ref()).c_str() ));
        cout << "./hal2009-server " << freehal::get_lang_str().ref() << " " << database_engine << endl;
    }
    else {
    	boost::thread t_v(boost::bind(std::system, ("hal2009-server " + freehal::get_lang_str().ref()).c_str() ));
        cout << "hal2009-server " << freehal::get_lang_str().ref() << " " << database_engine << endl;
    }
#else
# if defined(__APPLE__)
    std::system("rm booted");

    ifstream i_1("hal2009-server");
    if (i_1) {
    	boost::thread t_v(boost::bind(std::system, ("./hal2009-server " + freehal::get_lang_str().ref()).c_str() ));
        cout << "./hal2009-server " << freehal::get_lang_str().ref() << " " << database_engine << endl;
    }
    else {
    	boost::thread t_v(boost::bind(std::system, ("hal2009-server " + freehal::get_lang_str().ref()).c_str() ));
        cout << "hal2009-server " << freehal::get_lang_str().ref() << " " << database_engine << endl;
    }
# else
    std::system("del booted");

	boost::thread t_v(windows_invoke_runner);
        cout << "runner " << freehal::get_lang_str().ref() << " " << database_engine << endl;
# endif
#endif

		can_do_exit = true;
    } else {
        emit helper->everythingReady();
    }

	boost::thread t_v(check_new_version);
    boost::thread t_w(boost::bind(freehal::comm_init_client, ip, true, ip == "127.0.0.1" && dialog_connection->from->start_kernel->isChecked()));

    freehal::comm_send("HELLO:.");
}

int main(int argc, char* argv[]) {
    freehalthread1_talking.reset(new QString());


#ifdef JELIZA_DEBUG
# ifndef __APPLE__
#  ifndef linux
    if (AllocConsole()) {
        freopen("CON", "w", stdout);
        freopen("CON", "w", stderr);
    }
#  endif
# endif
#endif

    double JELIZA_PROGRESS2 = 0.0;
    JELIZA_PROGRESS = &JELIZA_PROGRESS2;

    int argc_for_qt = 0;
    char* argv_for_qt[1];
    char first_of_argv[2];
    argv_for_qt[0] = first_of_argv;
    QApplication app(argc_for_qt, argv_for_qt);
    
    QString locale = QLocale::system().name();
    freehal::string s(locale.toStdString());
    freehal::strings parts;
    util::split(s, "_", parts);
    locale = parts[0].ref().c_str();
    QTranslator translator;
    translator.load(QString("freehal_en"));
    translator.load(QString("freehal_") + locale);

    cout << locale.toStdString() << endl;
    app.installTranslator(&translator);

    helper = new Helper();

    main_window = new FreeHALWindow;
    dialog_options = new Dialog1;
    dialog_help = new Dialog3;
    dialog_changelog = new Dialog4;
    dialog_startup = new Dialog5;
    dialog_view = new Dialog8;
    dialog_connection = new ConnectDialog;


    Ui::freehalWindow* user_interface_main_window = new Ui::freehalWindow();
    user_interface_main_window->setupUi(main_window);
    main_window->user_interface_main_window = user_interface_main_window;

    fc = new FlowChart(NULL); // user_interface_main_window->scrollAreaWidgetContents);
    fc->user_interface_main_window = main_window->user_interface_main_window;
    //fc->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //user_interface_main_window->scrollAreaWidgetContents->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //user_interface_main_window->scrollAreaWidgetContents->setGeometry(fc->geometry());
    /*
QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::White);
    scrollArea->setWidget(fc);
    user_interface_main_window->gridLayout_2->addWidget(scrollArea, 0, 0, 1, 1);
    */
    user_interface_main_window->gridLayout_4->addWidget(fc, 0, 0, 1, 1);

    helper->connect(fc, SIGNAL(ask_again()),
             main_window, SLOT(on_compute_output_clicked()));


    Ui::ConnectDialog* user_interface_connection_dialog = new Ui::ConnectDialog();
    user_interface_connection_dialog->setupUi(dialog_connection);
    dialog_connection->from = user_interface_connection_dialog;

    Ui::Dialog* user_interface_dialog = new Ui::Dialog();
    user_interface_dialog->setupUi(dialog_options);
    main_window->user_interface_dialog = user_interface_dialog;
    dialog_options->user_interface_dialog = user_interface_dialog;
    dialog_options->user_interface_main_window = main_window->user_interface_main_window;

    Ui::HelpDialog* user_interface_dialog_help = new Ui::HelpDialog();
    user_interface_dialog_help->setupUi(dialog_help);
    dialog_help->user_interface_dialog = user_interface_dialog_help;
    dialog_help->user_interface_main_window = main_window->user_interface_main_window;
    
    Ui::ViewDialog* user_interface_dialog_view = new Ui::ViewDialog();
    user_interface_dialog_view->setupUi(dialog_view);
    dialog_view->user_interface_dialog = user_interface_dialog_view;
    dialog_view->user_interface_main_window = main_window->user_interface_main_window;

    Ui::ChangelogDialog* user_interface_dialog_changelog= new Ui::ChangelogDialog();
    user_interface_dialog_changelog->setupUi(dialog_changelog);
    dialog_changelog->user_interface_dialog = user_interface_dialog_changelog;
    dialog_changelog->user_interface_main_window = main_window->user_interface_main_window;
    dialog_changelog->init_changelog();


    helper->connect(&(*helper), SIGNAL(signalLogAppend(QString)),
             main_window->user_interface_main_window->textEdit_2, SLOT(append(QString)));
    helper->connect(&(*helper), SIGNAL(signalLogClear(QString)),
             main_window->user_interface_main_window->textEdit_2, SLOT(setText(QString)));
    helper->connect(&(*helper), SIGNAL(signalLogScrollEnd(QString)),
             main_window->user_interface_main_window->textEdit_2, SLOT(scrollToAnchor(QString)));

    helper->connect(&(*helper), SIGNAL(signalAnswer(QString)),
             main_window->user_interface_main_window->textEdit, SLOT(setHtml(QString)));
    helper->connect(&(*helper), SIGNAL(signalTalkingScrollEnd(QString)),
             main_window->user_interface_main_window->textEdit, SLOT(scrollToAnchor(QString)));

    helper->connect(&(*helper), SIGNAL(signalUpdateStatusbar(QString)),
            main_window->user_interface_main_window->statusbar, SLOT(showMessage(QString)));
    helper->connect(main_window->user_interface_main_window->statusbar, SIGNAL(messageChanged(QString)),
            main_window, SLOT(slotStatusbarUpdated(QString)));
    helper->connect(&(*helper), SIGNAL(signalChangeWindowTitle(QString)),
            main_window, SLOT(setWindowTitle(QString)));
    helper->connect(&(*helper), SIGNAL(signalChangeHelpWindowText1(QString)),
            dialog_help->user_interface_dialog->label, SLOT(setText(QString)));
    helper->connect(&(*helper), SIGNAL(signalChangeHelpWindowText2(QString)),
            dialog_help->user_interface_dialog->label_2, SLOT(setText(QString)));
    helper->connect(&(*helper), SIGNAL(signalMsg(QString)),
            main_window->user_interface_main_window->msg, SLOT(setHtml(QString)));
    helper->connect(&(*helper), SIGNAL(signalMsgGenus(QString)),
            main_window->user_interface_main_window->msg_2, SLOT(setHtml(QString)));

    helper->connect(&(*helper), SIGNAL(signalMsgNounOrNot(QString)),
            main_window->user_interface_main_window->msg_3, SLOT(setHtml(QString)));

    helper->connect(&(*helper), SIGNAL(signalShowMsgWindow()),
            &(*helper), SLOT(showMsgWindow()));
    helper->connect(&(*helper), SIGNAL(signalHideMsgWindow()),
            &(*helper), SLOT(hideMsgWindow()));

    helper->connect(&(*helper), SIGNAL(signalShowMsgGenusWindow()),
            &(*helper), SLOT(showMsgGenusWindow()));
    helper->connect(&(*helper), SIGNAL(signalHideMsgGenusWindow()),
            &(*helper), SLOT(hideMsgGenusWindow()));

    helper->connect(&(*helper), SIGNAL(signalShowMsgNounOrNotWindow()),
            &(*helper), SLOT(showMsgNounOrNotWindow()));
    helper->connect(&(*helper), SIGNAL(signalHideMsgNounOrNotWindow()),
            &(*helper), SLOT(hideMsgNounOrNotWindow()));

    helper->connect(&(*helper), SIGNAL(everythingReady()),
            &(*helper), SLOT(slotEverythingReady()));

    helper->connect(dialog_view->user_interface_dialog->weiter, SIGNAL(released()),
    		&(*helper), SLOT(close_view()));
    		
    helper->connect(&(*helper), SIGNAL(signalNewVersionOnline(QString)),
    		&(*helper), SLOT(slotNewVersionOnline(QString)));
    		
    app.connect(&app, SIGNAL(aboutToQuit()),
             &(*helper), SLOT(exitNow()));

    main_window->user_interface_main_window->lineEdit->setEnabled(false);
    main_window->user_interface_main_window->textEdit->setEnabled(false);
    main_window->user_interface_main_window->pushButton->setEnabled(false);
    main_window->user_interface_main_window->textEdit->setHtml(QString("Please wait..."));

    main_window->user_interface_main_window->lineEdit->setFocus();
        
    make_connection();

    return app.exec();
}

void FreeHALWindow::setVisible(bool visible) {
    minimizeAction->setEnabled(visible);
    maximizeAction->setEnabled(!isMaximized());
    restoreAction->setEnabled(isMaximized() || !visible);
    QWidget::setVisible(visible);
}

void FreeHALWindow::closeEvent(QCloseEvent *event) {
    if (trayIcon->isVisible() && QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::information(this, tr("Systray"),
                                 tr("FreeHAL will continue running; You can stop the whole program by clicking at the FreeHAL icon in the system tray"));
        hide();
        event->ignore();
    }
}

void FreeHALWindow::show_in_tray_icon(QString s) {
	if (!QSystemTrayIcon::isSystemTrayAvailable()) {
		return;
	}
	QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(
                        QSystemTrayIcon::Information);
	s = s.left(40);
	s += QString(" ...");
        main_window->trayIcon->showMessage(QString("Answer"), s, icon);
}

void FreeHALWindow::iconActivated(QSystemTrayIcon::ActivationReason reason) {
	if (!QSystemTrayIcon::isSystemTrayAvailable()) {
		return;
	}
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
    case QSystemTrayIcon::MiddleClick:
    	if (isMinimized() or !isVisible()) {
    		showNormal();
    	}
    	else {
    		hide();
		}
        break;
    default:
        ;
    }
}

void FreeHALWindow::createActions() {
	if (!QSystemTrayIcon::isSystemTrayAvailable()) {
		return;
	}
	
    minimizeAction = new QAction(tr("M&inimize"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("&Exit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void FreeHALWindow::createTrayIcon() {
	if (!QSystemTrayIcon::isSystemTrayAvailable()) {
		return;
	}
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}

void Helper::close_view() {
	dialog_view->hide();
	make_connection();
}

void Helper::reconnect() {
	make_connection();
}

void FreeHALWindow::on_actionNeue_Verbindung_herstellen_triggered() {
    make_connection();
}

void FreeHALWindow::showWindowNormal() {
    hide();
    user_interface_main_window->normalscreen->hide();
    user_interface_main_window->menubar->show();
    user_interface_main_window->msg_box->hide();
    user_interface_main_window->msg_box_2->hide();
    user_interface_main_window->msg_box_3->hide();

    main_window->setWindowFlags(0);
    main_window->resize(700, 550);

    showNormal();
}
void FreeHALWindow::showWindowFullscreen() {
    hide();

    user_interface_main_window->textEdit_2->hide();
    user_interface_main_window->menubar->hide();
    user_interface_main_window->msg_box->hide();
    user_interface_main_window->msg_box_2->hide();
    user_interface_main_window->msg_box_3->hide();

    main_window->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    main_window->move(0,0);
    showMaximized();

    user_interface_main_window->normalscreen->show();
}

void FreeHALWindow::showWindowEE() {
    hide();

    Dialog6* dialog = new Dialog6();

    Ui::EasterEgg* user_interface_dialog = new Ui::EasterEgg();
    user_interface_dialog->setupUi(dialog);
    dialog->user_interface_dialog = user_interface_dialog;
    dialog->user_interface_main_window = main_window->user_interface_main_window;

    dialog->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    dialog->move(0,0);
    dialog->showMaximized();
}

void FreeHALWindow::on_normalscreen_clicked() {
    dialog_options->close();
    main_window->showWindowNormal();
}
void FreeHALWindow::on_fullscreen_triggered() {
    dialog_options->close();
    main_window->showWindowFullscreen();
}


int AKTUELLE_PROZENT = 0;

void FreeHALWindow::on_actionGespr_ch_triggered() {
    main_window->user_interface_main_window->textEdit->setHtml(QString());
    freehalthread1_talking.reset(new QString());
    freehal::comm_send("CLEAR:TALKING");
}
void Dialog1::on_clear_screen_clicked() {
    main_window->user_interface_main_window->textEdit->setHtml(QString());
    freehalthread1_talking.reset(new QString());
    freehal::comm_send("CLEAR:TALKING");
}

void FreeHALWindow::on_actionEinstellungen_triggered() {
    dialog_options->show();
}

void Dialog5::updateStatusBar(float fl) {
    QString send;

    stringstream sst;
    string s;
    sst << fl;
    sst >> s;

    send = QString(QString(s.c_str()) + "%");

    emit signalUpdateStatusbar(send);
}

void FreeHALWindow::slotStatusbarUpdated(QString) {
    waitForStatusbar = false;
}

void Dialog1::on_mode_www_toggled() {
    www_surf_mode = !www_surf_mode;
    freehal::comm_send(string("WWW_SURF_MODE:") + ((www_surf_mode) ? "1" : "0"));
}
bool freehal::use_online_db = true;
void Dialog1::on_access_online_db_toggled() {
    freehal::use_online_db = !freehal::use_online_db;
    if (freehal::use_online_db) {
        freehal::comm_send("USE_ONLINE_DB:YES");
    }
    else {
        freehal::comm_send("USE_ONLINE_DB:NO");
    }
}

bool not_change_mode = false;

void Dialog1::on_mode_offline_stateChanged(int state) {
    if (not_change_mode) return;
    offline_mode = state == Qt::Checked ? 1 : 0;
    freehal::comm_send(string("OFFLINE_MODE:") + ((offline_mode) ? "1" : "0"));
}

void Dialog1::on_speech_stateChanged(int state) {
    if (not_change_mode) return;
    freehal::speech_activated = state == Qt::Checked ? 1 : 0;
    freehal::comm_send(string("SPEECH_MODE:") + ((freehal::speech_activated) ? "1" : "0"));
}

void Dialog1::on_proxy_ok_clicked() {
    freehal::comm_send("PROXY:" + dialog_options->user_interface_dialog->proxy->displayText().toStdString()
                               + ":" + dialog_options->user_interface_dialog->proxy_port->displayText().toStdString() );
}

void FreeHALWindow::on_actionInfo_triggered() {
    dialog_help->show();
}
void Dialog3::on_changelog_clicked() {
    dialog_changelog->show();
}

void Dialog4::init_changelog() {
    freehal::string all;

    ifstream i("gpl.txt");
    freehal::string temp;
    while (i) {
        getline(i, temp.ref());
        temp.strip();

        all += temp;
        all += "\n";
    }

    user_interface_dialog->te_changelog->setText(QString(all.ref().c_str()));
}

void FreeHALWindow::on_w_verb_clicked() {
    freehal::comm_send("HERE_IS_WORD_TYPE:1");
    cout << "HERE_IS_WORD_TYPE:1" << endl;
    main_window->user_interface_main_window->msg_box->hide();
}
void FreeHALWindow::on_w_nomen_clicked() {
    freehal::comm_send("HERE_IS_WORD_TYPE:2");
    cout << "HERE_IS_WORD_TYPE:2" << endl;
    main_window->user_interface_main_window->msg_box->hide();
}
void FreeHALWindow::on_w_interj_clicked() {
    freehal::comm_send("HERE_IS_WORD_TYPE:7");
    cout << "HERE_IS_WORD_TYPE:7" << endl;
    main_window->user_interface_main_window->msg_box->hide();
}
void FreeHALWindow::on_w_interj2_clicked() {
    freehal::comm_send("HERE_IS_WORD_TYPE:7");
    cout << "HERE_IS_WORD_TYPE:7" << endl;
    main_window->user_interface_main_window->msg_box->hide();
}
void FreeHALWindow::on_w_name_clicked() {
    freehal::comm_send("HERE_IS_WORD_TYPE:2");
    cout << "HERE_IS_WORD_TYPE:2" << endl;
    main_window->user_interface_main_window->msg_box->hide();
}
void FreeHALWindow::on_w_adj_clicked() {
    freehal::comm_send("HERE_IS_WORD_TYPE:3");
    cout << "HERE_IS_WORD_TYPE:3" << endl;
    main_window->user_interface_main_window->msg_box->hide();
}
void FreeHALWindow::on_w_adv_clicked() {
    freehal::comm_send("HERE_IS_WORD_TYPE:3");
    cout << "HERE_IS_WORD_TYPE:3" << endl;
    main_window->user_interface_main_window->msg_box->hide();
}
void FreeHALWindow::on_w_pronomen_clicked() {
    freehal::comm_send("HERE_IS_WORD_TYPE:4");
    cout << "HERE_IS_WORD_TYPE:4" << endl;
    main_window->user_interface_main_window->msg_box->hide();
}
void FreeHALWindow::on_w_quesword_clicked() {
    freehal::comm_send("HERE_IS_WORD_TYPE:5");
    cout << "HERE_IS_WORD_TYPE:5" << endl;
    main_window->user_interface_main_window->msg_box->hide();
}
void FreeHALWindow::on_w_prep_clicked() {
    freehal::comm_send("HERE_IS_WORD_TYPE:6");
    cout << "HERE_IS_WORD_TYPE:6" << endl;
    main_window->user_interface_main_window->msg_box->hide();
}
void FreeHALWindow::on_w_adv_best_clicked() {
    freehal::comm_send("HERE_IS_WORD_TYPE:3");
    cout << "HERE_IS_WORD_TYPE:3" << endl;
    main_window->user_interface_main_window->msg_box->hide();
}

void FreeHALWindow::on_male_clicked() {
    freehal::comm_send("HERE_IS_GENUS:1");
    cout << "HERE_IS_GENUS:1" << endl;
    main_window->user_interface_main_window->msg_box_2->hide();
}

void FreeHALWindow::on_female_clicked() {
    freehal::comm_send("HERE_IS_GENUS:2");
    cout << "HERE_IS_GENUS:2" << endl;
    main_window->user_interface_main_window->msg_box_2->hide();
}

void FreeHALWindow::on_thing_clicked() {
    freehal::comm_send("HERE_IS_GENUS:3");
    cout << "HERE_IS_GENUS:3" << endl;
    main_window->user_interface_main_window->msg_box_2->hide();
}

void FreeHALWindow::on_no_genus_clicked() {
    freehal::comm_send("HERE_IS_GENUS:3");
    cout << "HERE_IS_GENUS:3" << endl;
    main_window->user_interface_main_window->msg_box_2->hide();
}

void FreeHALWindow::on_is_noun_clicked() {
    freehal::comm_send("HERE_IS_NOUN_OR_NOT:1");
    cout << "HERE_IS_NOUN_OR_NOT:1" << endl;
    main_window->user_interface_main_window->msg_box_3->hide();
}

void FreeHALWindow::on_not_is_noun_clicked() {
    freehal::comm_send("HERE_IS_NOUN_OR_NOT:2");
    cout << "HERE_IS_NOUN_OR_NOT:2" << endl;
    main_window->user_interface_main_window->msg_box_3->hide();
}

void FreeHALWindow::on_abbr_1_clicked() {
    freehal::comm_send("HERE_IS_GENUS:EXIT");
    cout << "HERE_IS_GENUS:EXIT" << endl;
//    main_window->user_interface_main_window->msg_box_2->hide();
}
void FreeHALWindow::on_abbr_3_clicked() {
    freehal::comm_send("HERE_IS_NOUN_OR_NOT:EXIT");
    cout << "HERE_IS_NOUN_OR_NOT:EXIT" << endl;
//    main_window->user_interface_main_window->msg_box_3->hide();
}
void FreeHALWindow::on_abbr_2_clicked() {
    freehal::comm_send("HERE_IS_WORD_TYPE:EXIT");
    cout << "HERE_IS_WORD_TYPE:EXIT" << endl;
//    main_window->user_interface_main_window->msg_box->hide();
}

void Helper::showMsgWindow() {
    main_window->user_interface_main_window->msg_box->show();
}
void Helper::hideMsgWindow() {
    main_window->user_interface_main_window->msg_box->hide();
}

void Helper::showMsgGenusWindow() {
    main_window->user_interface_main_window->msg_box_2->show();
}
void Helper::hideMsgGenusWindow() {
    main_window->user_interface_main_window->msg_box_2->hide();
}

void Helper::showMsgNounOrNotWindow() {
    main_window->user_interface_main_window->msg_box_3->show();
}
void Helper::hideMsgNounOrNotWindow() {
    main_window->user_interface_main_window->msg_box_3->hide();
}

void Helper::slotEverythingReady() {
    main_window->user_interface_main_window->lineEdit->setEnabled(true);
    main_window->user_interface_main_window->textEdit->setEnabled(true);
    main_window->user_interface_main_window->pushButton->setEnabled(true);
    main_window->user_interface_main_window->textEdit->setHtml(QString());
}

FlowChart::FlowChart(QWidget* w)
: QWidget(w) {
}

int lastX = 0, lastY = 0;
int diffX = 0, diffY = 0;
struct ClickPositionItem** clickPositions = (struct ClickPositionItem**)calloc(10000*sizeof(struct ClickPositionItem*), 1);
void FlowChart::mousePressEvent(QMouseEvent *e) {
    try {

    cout << "Button pressed (" << e->x() << ", " << e->y() << ")." << endl;
    if (e->button() == Qt::LeftButton) {
        if (e->x() <= this->size().width() - 20) {
            lastX = e->x();
            lastY = e->y();
        }
    }

    if (e->button() == Qt::LeftButton) {
        if (e->x() > this->size().width() - 20) {

            if (e->y() > 50) {
                diffY -= 20;
            }
            else {
                diffY += 20;
            }
            this->repaint();

            int z;
            for (z = 0; z < 3; ++z) {
                if (e->y() > 50) {
                    diffY -= 30;
                }
                else {
                    diffY += 30;
                }
                this->repaint();
            }

            if (e->y() >50) {
                diffY -= 20;
            }
            else {
                diffY += 20;
            }
            this->repaint();
        }
    }

    } catch (std::bad_alloc b) {
        cout << "Bad allocation in mousePressEvent." << endl;
    }
}

void FreeHALWindow::on_refresh_chart_clicked() {
    fc->repaint();
}

void FlowChart::mouseReleaseEvent(QMouseEvent *e) {
    try {
    cout << "Button released (" << e->x() << ", " << e->y() << ")." << endl;
    if (e->button() == Qt::LeftButton) {
        if (e->x() <= this->size().width() - 20) {
            if (e->x() - lastX > 10 || e->y() - lastY > 10 || e->x() - lastX < -10 || e->y() - lastY < -10) {
                diffX += e->x() - lastX;
                diffY += e->y() - lastY;

                cout << "Repainting (" << (e->x() - lastX > 10) << ", " << (e->y() - lastY > 10) << ", " << (e->x() - lastX < -10) << ", " << (e->y() - lastY < -10) << ")..." << endl;
                this->repaint();
        }

            else {
                this->user_interface_main_window->flowchart_fact_pk->setValue(0);
                int y = e->y() - diffY;
                for (int l = 0; l < 9999 && clickPositions[l]; ++l) {
                    if (clickPositions[l]) {
                        cout << "[from " << clickPositions[l]->beginY << " to " << clickPositions[l]->endY << "]" << endl;
                        if (y >= clickPositions[l]->beginY && y <= clickPositions[l]->endY) {
    //                        if (clickPositions[l]->prop == QString("pk    ")) {

                                this->user_interface_main_window->flowchart_fact_pk->setValue(QString::fromStdString(std::string(clickPositions[l]->value)).toInt(NULL, 10));

                                break;
    //                        }
                        }
                    }
                }
            }

            lastX = 0;
            lastY = 0;
        }
    }

    if (e->button() != Qt::LeftButton) {
        e->ignore();
        return;
    }
    } catch (std::bad_alloc b) {
        cout << "Bad allocation in mouseReleaseEvent." << endl;
    }
}

void FlowChart::paintEvent(QPaintEvent *event) {
    try {
    QPainter painter(this);

    for (int l = 0; l < 9999 && clickPositions[l]; ++l) {
        if (clickPositions[l]) {
            if (clickPositions[l]->prop)
                free(clickPositions[l]->prop);
            if (clickPositions[l]->value)
                free(clickPositions[l]->value);
            free(clickPositions[l]);
            clickPositions[l] = 0;
        }
    }

    int pos = 0;

    ifstream log("flowchart.log");
    string line;
    int margin_top = 10;
    int margin_between = 10;
    int padding = 10;
    int text_height = 15;
    int num_of_boxes = 0;
    int box_width = 500;
    int timeout = 5000;
    while (log && timeout >= 0) {
        --timeout;
        getline(log, line);
        while (line.size() && (line[line.size()-1] == '\r' || line[line.size()-1] == '\n')) {
            line[line.size()-1] == '\0';
        }

        if (line == "begin box") {
            if (num_of_boxes >= 1) {
                painter.drawLine(diffX+10+box_width/2+5,  diffY+margin_top,   diffX+10+box_width/2+5, diffY+margin_top+margin_between*2);
                painter.drawLine(diffX+10+box_width/2-5,  diffY+margin_top,   diffX+10+box_width/2-5, diffY+margin_top+margin_between*2);

                painter.drawLine(diffX+10+box_width/2+10, diffY+margin_top+margin_between*1.5, diffX+10+box_width/2, diffY+margin_top+margin_between*2.5);
                painter.drawLine(diffX+10+box_width/2-10, diffY+margin_top+margin_between*1.5, diffX+10+box_width/2, diffY+margin_top+margin_between*2.5);

                margin_top += margin_between*3.5;
            }

            int num_text_lines = 0;
            while (log) {
                getline(log, line);
                while (line.size() && (line[line.size()-1] == '\r' || line[line.size()-1] == '\n')) {
                    line[line.size()-1] == '\0';
                }

                if (line == "end box") {
                    break;
                }

                // Entries of a "box"

                if (line.substr(0, 4) == "draw") {
                    int height = (2*padding) + (text_height*num_text_lines);
                    painter.drawRect(diffX+10, diffY+margin_top, box_width, height);
                    margin_top += padding;
                }
                else if (line.substr(0, 12) == "setinput    ") {
                    user_interface_main_window->flowchart_input->setText(QString::fromStdString(line.substr(12)));
                }
                else if (line.substr(0, 12) == "bckgrndcolr ") {
                    painter.setBrush(QBrush(("#" + line.substr(12, 6)).c_str()));
                }
                else if (line.substr(0, 12) == "bordercolor ") {
                    painter.setPen(QColor(("#" + line.substr(12, 6)).c_str()));
                }
                else if (line.substr(0, 12) == "linesoftext ") {
                    sscanf(line.substr(12).c_str(), "%d", &num_text_lines);
                }
                else if (line.substr(0, 12) == "linkcontent ") {
                    painter.setPen(QColor(("#" + line.substr(12, 6)).c_str()));
                    QString prop;
                    QString value;
                    painter.drawText(diffX+10+padding, diffY+margin_top+padding, QString::fromStdString(line.substr(12+6+1)));
                    getline(log, line);
                    prop  = QString::fromStdString(line.substr(12, 6)); // property
                    value = QString::fromStdString(line.substr(12+6+1)); // value

                    if (pos < 999) {
                        clickPositions[pos] = (struct ClickPositionItem*)calloc(sizeof(struct ClickPositionItem), 1);
                        clickPositions[pos]->prop  = strdup(prop .toStdString().c_str());
                        clickPositions[pos]->value = strdup(value.toStdString().c_str());
                        clickPositions[pos]->beginY = margin_top;
                        clickPositions[pos]->endY = margin_top + text_height;
                        ++pos;
                    }

                    margin_top += text_height;
                }
                else if (line.substr(0, 12) == "textcontent ") {
                    painter.setPen(QColor(("#" + line.substr(12, 6)).c_str()));
                    painter.drawText(diffX+10+padding, diffY+margin_top+padding, QString::fromStdString(line.substr(12+6+1)));
                    margin_top += text_height;
                }
            }
            margin_top += padding;
            margin_top += margin_between;
            ++num_of_boxes;
        }
    }

/*
    painter.setBrush(QBrush("#c56c00"));
    painter.drawRect(10, 15, 90, 60);

    painter.setBrush(QBrush("#1ac500"));
    painter.drawRect(130, 15, 90, 60);

    painter.setBrush(QBrush("#539e47"));
    painter.drawRect(250, 15, 90, 60);

    painter.setBrush(QBrush("#004fc5"));
    painter.drawRect(10, 105, 90, 60);

    painter.setBrush(QBrush("#c50024"));
    painter.drawRect(130, 105, 90, 60);

    painter.setBrush(QBrush("#9e4757"));
    painter.drawRect(250, 105, 90, 60);

    painter.setBrush(QBrush("#5f3b00"));
    painter.drawRect(10, 195, 90, 60);

    painter.setBrush(QBrush("#4c4c4c"));
    painter.drawRect(130, 195, 90, 60);

    painter.setBrush(QBrush("#785f36"));
    painter.drawRect(250, 195, 90, 60);
*/
    } catch (std::bad_alloc b) {
        cout << "Bad allocation in paintEvent." << endl;
    }

}

void freehal::comm_new(freehal::string s) {
    try {
    if (s.contains("ĹOG")) {
        cout << s.ref() << endl;
    }
    s = s.replace(":::", "::");
    s = s.replace("::", "----");
    s = s.replace(";", ",");
    s = s.replace(":", ";");

    freehal::strings parts;
    util::split(s, ";", parts);

    for (unsigned int x = 0; x < parts.size(); x++) {
        parts[x] = parts[x].replace("----", ":");
    }


    if (parts.size() >= 2) {
        if (s.contains("PERL")) {
            is_perl = 1;
        }
        if (s.contains("DISPLAY")) {
            QString qs(s.ref().c_str()+8);
            qs = qs.replace(";", ":");
            freehal::display_sentence(freehal::string(qs.toStdString()));
        }
        if (s.contains("DELETED")) {
            emit fc->ask_again();
        }
        if (s.contains("SPEAK")) {
            speak(parts[1].ref());
            emit helper->sig_show_in_tray_icon(QString(parts[1].ref().c_str()));
        }
        if (s.contains("LOG")) {
            static long int size = 0;
            QString qs(s.ref().c_str()+6);
            size += qs.size();
            qs = qs.replace("\n", "<br/>");
            qs = qs.replace("<br/>", "\n");

            if (size > 100000) {
                emit helper->signalLogClear("");
                size = 0;
            }

            static long int g = 0;
            emit helper->signalLogAppend(qs); // + QString("<a name=\"line") + g + QString("\">&nbsp;</a>"));
            //emit helper->signalLogScrollEnd(QString("line") + g);
            ++g;
        }
        if (s.contains("HERE_IS_DB_STRING")) {
        }
        if (s.contains("HERE_IS_ONLINE_DB_STRING")) {
        }
        if (s.contains("IMPORT_DB_SUCCESS")) {
        }
        if (s.contains("GET_WORD_TYPE")) {
/// <br>0. Abbruch. Keine Antwort.<br>1. Verb (z.B. sagen)<br>2. Name und Nomen (z.B Haus)<br>3. Adjektiv, Adverb oder adverbiale Bestimmung (z.B. schnell; immer, ueberall, gestern)<br>4. Pronomen (z.B er, sie, es)<br>5. Fragewort (z.B. warum, wie) oder Konjunktion<br>6. Praeposition (z.B. auf, an, unter)<br><br>Gib bitte die Nummer ein.

            emit helper->signalMsg(QString(("Which part of speech is \"" + parts[1].ref() + "\"?\n\nWelche Wortart hat \"" + parts[1].ref() + "\"?").c_str()));
            emit helper->signalShowMsgWindow();
        }
        if (s.contains("GET_GENUS")) {
            emit helper->signalMsgGenus(QString(("Which gender has the noun \"" + parts[1].ref() + "\"?\n\nWelches grammatische Geschlecht (Genus) hat das Wort \"" + parts[1].ref() + "\"? Nennt man es \"der " + parts[1].ref() + "\", \"die " + parts[1].ref() + "\" oder \"das " + parts[1].ref() + "\"?").c_str()));
            emit helper->signalShowMsgGenusWindow();
        }
        if (s.contains("NAME")) {
            ki_name.set(parts[1]);
            emit helper->signalChangeWindowTitle(QApplication::translate("freehalWindow", ki_name.ref().c_str(), 0, QApplication::UnicodeUTF8));
            emit helper->signalChangeHelpWindowText1(dialog_help->user_interface_dialog->label->text().replace("ABC", ki_name.ref().c_str()));
            emit helper->signalChangeHelpWindowText2(dialog_help->user_interface_dialog->label_2->text().replace("ABC", ki_name.ref().c_str()));
        }
        if (s.contains("JELIZA_FULL_VERSION")) {
            version.set(parts[1]);
        }
        if (s.contains("READY")) {
            emit helper->everythingReady();
        }
        if (s.contains("OFFLINE_MODE")) {
            stringstream sst;
            sst << parts[1].ref();
            sst >> offline_mode;
            not_change_mode = true;
            dialog_options->user_interface_dialog->mode_offline->setCheckState(offline_mode ? Qt::Checked : Qt::Unchecked);
            not_change_mode = false;
        }
        if (s.contains("SPEECH_MODE")) {
            stringstream sst;
            sst << parts[1].ref();
            sst >> freehal::speech_activated;
            not_change_mode = true;
            dialog_options->user_interface_dialog->speech->setCheckState(freehal::speech_activated ? Qt::Checked : Qt::Unchecked);
            not_change_mode = false;
        }
    }
    } catch (std::bad_alloc b) {
        cout << "Bad allocation in comm_new." << endl;
    }

}

