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
FreeHALWindow*              main_window;
Dialog1*                    dialog_options;
Dialog3*                    dialog_help;
Dialog4*                    dialog_changelog;
Dialog5*                    dialog_startup;
Dialog8*                    dialog_view;
ConnectDialog*              dialog_connection;
MoreInfo*                   dialog_moreinfo;
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
    emit helper->signalAnswerTalk(QString((s.ref() + "<br><a name=\"anch\" id=\"anch\">&nbsp;</a>").c_str()));
    emit helper->signalTalkingScrollEndTalk(QString("anch"));
}

void FreeHALWindow::hideStartupDialog() {
    dialog_startup->hide();
    main_window->showWindowNormal();
}

const char* check_config (freehal::string name, const char* _default) {
    ifstream i_1("config.txt");
    ifstream i_2("../config.txt");
    ifstream i_3("../../config.txt");
    
    const char* config_file = "config.txt";
    ifstream* i = &i_1;
    if (i_1.is_open()) {
        i = &i_1;
    }
    if (i_2.is_open()) {
        i = &i_2;
        config_file = "../config.txt";
    }
    if (i_3.is_open()) {
        i = &i_3;
        config_file = "../../config.txt";
    }
    
    if (i && i->is_open()) {
        freehal::string temp;
        while (*i) {
            getline(*i, temp.ref());
            temp.strip();
            
            temp = temp.replace(" =", "=");
            temp = temp.replace("= ", "=");

            if (temp.contains(name + "=")) {
                freehal::strings parts;
                util::split(temp, "=", parts);
                
                if (parts.size() >= 2) {
                    return (parts[1].ref().c_str());
                }
            }
        }
    }
    i_1.close();
    i_2.close();
    i_3.close();
    
    ofstream o(config_file, ios::app);
    o << name.ref() << " = " << _default << endl;
    o.close();
    
    return _default;
}

const char* set_config (freehal::string name, const char* _default) {
    ifstream i_1("config.txt");
    ifstream i_2("../config.txt");
    ifstream i_3("../../config.txt");
    
    const char* config_file = "config.txt";
    ifstream* i = &i_1;
    if (i_1.is_open()) {
        i = &i_1;
    }
    if (i_2.is_open()) {
        i = &i_2;
        config_file = "../config.txt";
    }
    if (i_3.is_open()) {
        i = &i_3;
        config_file = "../../config.txt";
    }
    
    if (!i->is_open()) {
        ofstream o("config.txt", ios::app);
        o.close();
        ifstream i_4("config.txt");
        i = &i_4;
        config_file = "config.txt";
    }
    
    freehal::string file_content;
    freehal::string temp;
    while (*i) {
        getline(*i, temp.ref());
        
        if (!temp.contains(name + "=") && !temp.contains(name + " =") && temp.size() > 2) {
            file_content += temp;
            file_content += "\n";
        }
    }
    i->close();
    
    ofstream o("config.txt");
    o << file_content.ref();
    o << name.ref() << " = " << _default << endl;
    o.close();
    
    return _default;
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

int time_needed_to_learn = 1;
int time_to_learn_begin  = 0;
int time_to_learn_end    = 0;

void thread_learn_progress_bar(int max) {
    for (int i = 0; i < max*20; ++i) {
        emit helper->signalSetTimeToLearnElapsed(i);
        freehal::msleep(50);
    }
    freehal::msleep(1000);
    emit helper->signalSetTimeToLearnElapsed(0);
    freehal::msleep(10000);
    emit helper->signalSetTimeToLearnElapsed(0);
}

void FreeHALWindow::button_talk(QString command, QLineEdit* lineedit) {
    QString qs = lineedit->displayText();
    freehal::string ques = ascii(qs);
    lineedit->setText(QString());
    if (ques.get_lower().contains("windows") && ques.get_lower().contains("gut")) {
        main_window->showWindowEE();
    }

    freehal::comm_send(command.toStdString().c_str() + ques);
}

void FreeHALWindow::on_pushButton_clicked() {
    button_talk("TALK:", user_interface_main_window->lineedit_talk);
}

void FreeHALWindow::on_pushButton_learn_clicked() {
    button_talk("LEARN:", user_interface_main_window->lineedit_learn);

    boost::thread progress(boost::bind(thread_learn_progress_bar, main_window->user_interface_main_window->learnbar->maximum()));
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

void FreeHALWindow::on_flowchart_fact_edit_clicked() {
    QString qs;
    qs.setNum(user_interface_main_window->flowchart_fact_pk->value());
    QString replacement = main_window->user_interface_main_window->flowchart_edit->text();
    main_window->user_interface_main_window->flowchart_edit->setText("Please wait...");

    freehal::comm_send("REPLACE:PROFACT:PK:" + qs.toStdString() + ":BY:" + replacement.toStdString());
}

void FreeHALWindow::on_lineedit_talk_returnPressed() {
    on_pushButton_clicked();
}

void FreeHALWindow::on_lineedit_learn_returnPressed() {
    on_pushButton_learn_clicked();
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
    if (database_engine == "semtree")
        database_engine = "disk";
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
    dialog_moreinfo = new MoreInfo;


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

    Ui::ChangelogDialog* user_interface_dialog_changelog = new Ui::ChangelogDialog();
    user_interface_dialog_changelog->setupUi(dialog_changelog);
    dialog_changelog->user_interface_dialog = user_interface_dialog_changelog;
    dialog_changelog->user_interface_main_window = main_window->user_interface_main_window;
    dialog_changelog->init_changelog();

    Ui::MoreInfo* user_interface_dialog_moreinfo = new Ui::MoreInfo();
    user_interface_dialog_moreinfo->setupUi(dialog_moreinfo);
    dialog_moreinfo->user_interface_dialog = user_interface_dialog_moreinfo;
    dialog_moreinfo->user_interface_main_window = main_window->user_interface_main_window;

// log
    /*
    helper->connect(&(*helper), SIGNAL(signalLogAppend(QString)),
             main_window->user_interface_main_window->textedit_talk_2, SLOT(append(QString)));
    helper->connect(&(*helper), SIGNAL(signalLogClear(QString)),
             main_window->user_interface_main_window->textedit_talk_2, SLOT(setText(QString)));
    helper->connect(&(*helper), SIGNAL(signalLogScrollEnd(QString)),
             main_window->user_interface_main_window->textedit_talk_2, SLOT(scrollToAnchor(QString)));
    */

    helper->connect(&(*helper), SIGNAL(signalAnswerTalk(QString)),
             main_window->user_interface_main_window->textedit_talk, SLOT(setHtml(QString)));
    helper->connect(&(*helper), SIGNAL(signalTalkingScrollEndTalk(QString)),
             main_window->user_interface_main_window->textedit_talk, SLOT(scrollToAnchor(QString)));
    helper->connect(&(*helper), SIGNAL(signalAnswerLearn(QString)),
             main_window->user_interface_main_window->textedit_talk, SLOT(setHtml(QString)));
    helper->connect(&(*helper), SIGNAL(signalTalkingScrollEndLearn(QString)),
             main_window->user_interface_main_window->textedit_talk, SLOT(scrollToAnchor(QString)));
    helper->connect(&(*helper), SIGNAL(signalAnswerTalk(QString)),
             main_window->user_interface_main_window->textedit_learn, SLOT(setHtml(QString)));
    helper->connect(&(*helper), SIGNAL(signalTalkingScrollEndTalk(QString)),
             main_window->user_interface_main_window->textedit_learn, SLOT(scrollToAnchor(QString)));
    helper->connect(&(*helper), SIGNAL(signalAnswerLearn(QString)),
             main_window->user_interface_main_window->textedit_learn, SLOT(setHtml(QString)));
    helper->connect(&(*helper), SIGNAL(signalTalkingScrollEndLearn(QString)),
             main_window->user_interface_main_window->textedit_learn, SLOT(scrollToAnchor(QString)));

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
    helper->connect(&(*helper), SIGNAL(signalMoreInfo(QStringList)),
            &(*helper), SLOT(slotMoreInfo(QStringList)));


    helper->connect(&(*helper), SIGNAL(signalShowMsgWindow()),
            &(*helper), SLOT(showMsgWindow()));
    helper->connect(&(*helper), SIGNAL(signalHideMsgWindow()),
            &(*helper), SLOT(hideMsgWindow()));

    helper->connect(&(*helper), SIGNAL(signalShowMsgGenusWindow()),
            &(*helper), SLOT(showMsgGenusWindow()));
    helper->connect(&(*helper), SIGNAL(signalHideMsgGenusWindow()),
            &(*helper), SLOT(hideMsgGenusWindow()));

    helper->connect(&(*helper), SIGNAL(everythingReady()),
            &(*helper), SLOT(slotEverythingReady()));

    helper->connect(dialog_view->user_interface_dialog->weiter, SIGNAL(released()),
    		&(*helper), SLOT(close_view()));
    		
    helper->connect(&(*helper), SIGNAL(signalNewVersionOnline(QString)),
    		&(*helper), SLOT(slotNewVersionOnline(QString)));

    helper->connect(main_window->user_interface_main_window->verticalScrollBar, SIGNAL(valueChanged(int)),
                    fc, SLOT(setY(int)));
    helper->connect(fc, SIGNAL(setFactText(QString)),
                    main_window->user_interface_main_window->flowchart_edit, SLOT(setText(QString)));

    helper->connect(&(*helper), SIGNAL(signalSetTimeToLearn(int)),
            main_window->user_interface_main_window->learnbar, SLOT(setMaximum(int)));
    helper->connect(&(*helper), SIGNAL(signalSetTimeToLearnElapsed(int)),
            main_window->user_interface_main_window->learnbar, SLOT(setValue(int)));
    		
    app.connect(&app, SIGNAL(aboutToQuit()),
             &(*helper), SLOT(exitNow()));

    main_window->user_interface_main_window->lineedit_talk->setEnabled(false);
    main_window->user_interface_main_window->textedit_talk->setEnabled(false);
    main_window->user_interface_main_window->pushButton->setEnabled(false);
    main_window->user_interface_main_window->textedit_talk->setHtml(QString("Please wait..."));
    main_window->user_interface_main_window->learnbar->hide();

    main_window->user_interface_main_window->lineedit_talk->setFocus();
    
    offline_mode  = !(0 == strcmp(check_config(freehal::string("online"), "1"), "1"));
    www_surf_mode =  (0 == strcmp(check_config(freehal::string("use-www"), "1"), "1"));
    dialog_options->user_interface_dialog->mode_offline->setCheckState(offline_mode ? Qt::Checked : Qt::Unchecked);
    dialog_options->user_interface_dialog->mode_www->setCheckState(www_surf_mode ? Qt::Checked : Qt::Unchecked);

    int limit_amount_of_answers = (0 == strcmp(check_config(freehal::string("limit-amount-of-answers"), "1"), "1"));
    dialog_options->user_interface_dialog->limit_amount_of_answers->setCheckState(limit_amount_of_answers ? Qt::Checked : Qt::Unchecked);

    init_db_tool();
    main_window->setupMenu();
    
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

    main_window->setWindowFlags(0);
    main_window->resize(700, 550);

    showNormal();
}
void FreeHALWindow::showWindowFullscreen() {
    hide();

    // user_interface_main_window->textedit_talk_2->hide();
    user_interface_main_window->menubar->hide();
    user_interface_main_window->msg_box->hide();
    user_interface_main_window->msg_box_2->hide();

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
    main_window->user_interface_main_window->textedit_talk->setHtml(QString());
    freehalthread1_talking.reset(new QString());
    freehal::comm_send("CLEAR:TALKING");
}
void Dialog1::on_clear_screen_clicked() {
    main_window->user_interface_main_window->textedit_talk->setHtml(QString());
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
    set_config(freehal::string("use-www"), ((www_surf_mode) ? "1" : "0"));
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
    set_config(freehal::string("online"), ((!offline_mode) ? "1" : "0"));
}

void Dialog1::on_speech_stateChanged(int state) {
    if (not_change_mode) return;
    freehal::speech_activated = state == Qt::Checked ? 1 : 0;
    freehal::comm_send(string("SPEECH_MODE:") + ((freehal::speech_activated) ? "1" : "0"));
    set_config(freehal::string("speech"), ((freehal::speech_activated) ? "1" : "0"));
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

void MoreInfo::on_w_v_clicked() {
    freehal::comm_send("HERE_IS_WORD_TYPE:1");
    cout << "HERE_IS_WORD_TYPE:1" << endl;
    this->close();
}
void MoreInfo::on_w_n_clicked() {
    this->user_interface_dialog->box_genus->show();
}
void MoreInfo::on_g_male_clicked() {
    freehal::comm_send("HERE_IS_WORD_TYPE:2:m");
    cout << "HERE_IS_WORD_TYPE:2,m" << endl;
    this->close();
}
void MoreInfo::on_g_female_clicked() {
    freehal::comm_send("HERE_IS_WORD_TYPE:2:f");
    cout << "HERE_IS_WORD_TYPE:2,f" << endl;
    this->close();
}
void MoreInfo::on_g_neuter_clicked() {
    freehal::comm_send("HERE_IS_WORD_TYPE:2:s");
    cout << "HERE_IS_WORD_TYPE:2,s" << endl;
    this->close();
}
void MoreInfo::on_g_unknown_clicked() {
    freehal::comm_send("HERE_IS_WORD_TYPE:2:q");
    cout << "HERE_IS_WORD_TYPE:2,q" << endl;
    this->close();
}
void MoreInfo::on_w_a_clicked() {
    freehal::comm_send("HERE_IS_WORD_TYPE:3");
    cout << "HERE_IS_WORD_TYPE:3" << endl;
    this->close();
}
void MoreInfo::on_w_i_clicked() {
    freehal::comm_send("HERE_IS_WORD_TYPE:7");
    cout << "HERE_IS_WORD_TYPE:7" << endl;
    this->close();
}
void MoreInfo::on_w_c_clicked() {
    freehal::comm_send("HERE_IS_WORD_TYPE:5");
    cout << "HERE_IS_WORD_TYPE:5" << endl;
    this->close();
}
void MoreInfo::on_w_p_clicked() {
    freehal::comm_send("HERE_IS_WORD_TYPE:6");
    cout << "HERE_IS_WORD_TYPE:6" << endl;
    this->close();
}

void MoreInfo::on_buttonBox_clicked(QAbstractButton* button)
{
    freehal::comm_send("HERE_IS_WORD_TYPE:EXIT");
    cout << "HERE_IS_WORD_TYPE:EXIT" << endl;
    this->close();
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

void FreeHALWindow::on_abbr_1_clicked() {
    freehal::comm_send("HERE_IS_GENUS:EXIT");
    cout << "HERE_IS_GENUS:EXIT" << endl;
    main_window->user_interface_main_window->msg_box_2->hide();
}
void FreeHALWindow::on_abbr_2_clicked() {
    freehal::comm_send("HERE_IS_WORD_TYPE:EXIT");
    cout << "HERE_IS_WORD_TYPE:EXIT" << endl;
    main_window->user_interface_main_window->msg_box->hide();
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

void Helper::slotEverythingReady() {
    main_window->user_interface_main_window->lineedit_talk->setEnabled(true);
    main_window->user_interface_main_window->textedit_talk->setEnabled(true);
    main_window->user_interface_main_window->pushButton->setEnabled(true);
    main_window->user_interface_main_window->textedit_talk->setHtml(QString());
}

FlowChart::FlowChart(QWidget* w)
: QWidget(w) {
}

int lastX = 0, lastY = 0, lastY_scrollbar = 0;
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

void FlowChart::setY(int y) {
    diffY -= y - lastY_scrollbar;
    lastY_scrollbar = y;
    this->repaint();
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
                this->user_interface_main_window->flowchart_edit->setText(QString());
                this->user_interface_main_window->flowchart_view->setText(QString());

                int y = e->y() - diffY;
                for (int l = 0; l < 9999 && clickPositions[l]; ++l) {
                    if (clickPositions[l]) {
                        cout << "[from " << clickPositions[l]->beginY << " to " << clickPositions[l]->endY << "]" << endl;
                        if (y >= clickPositions[l]->beginY && y <= clickPositions[l]->endY) {
                            this->user_interface_main_window->flowchart_fact_pk->setValue(QString::fromStdString(std::string(clickPositions[l]->value)).toInt(NULL, 10));
                            this->user_interface_main_window->flowchart_view->setText(QString::fromStdString(std::string(clickPositions[l]->text)));

                            break;
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
            if (clickPositions[l]->text)
                free(clickPositions[l]->text);
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
                    QString text;
                    {
                        QStringList textList = QString::fromStdString(line.substr(12+6+1)).split(": ");
                        if (textList.size() >= 2) {
                            text = textList.at(1);
                        }
                    }
                    painter.drawText(diffX+10+padding, diffY+margin_top+padding, QString::fromStdString(line.substr(12+6+1)));
                    getline(log, line);
                    prop  = QString::fromStdString(line.substr(12, 6)); // property
                    value = QString::fromStdString(line.substr(12+6+1)); // value

                    if (pos < 999) {
                        clickPositions[pos] = (struct ClickPositionItem*)calloc(sizeof(struct ClickPositionItem), 1);
                        clickPositions[pos]->prop  = strdup(prop .toStdString().c_str());
                        clickPositions[pos]->value = strdup(value.toStdString().c_str());
                        clickPositions[pos]->text  = strdup(text .toStdString().c_str());
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

    main_window->user_interface_main_window->verticalScrollBar->setMinimum(0);
    main_window->user_interface_main_window->verticalScrollBar->setPageStep(margin_top-this->size().height());
    main_window->user_interface_main_window->verticalScrollBar->setMaximum(margin_top);

    } catch (std::bad_alloc b) {
        cout << "Bad allocation in paintEvent." << endl;
    }

}

void freehal::comm_new(freehal::string s) {
    try {
    if (s.contains("LOG")) {
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
        if (s.contains("LEARNED")) {
            if (s.ref()[8] != '0') {
                time_to_learn_end = (int)time(NULL);
                time_needed_to_learn = (time_needed_to_learn + (time_to_learn_end - time_to_learn_begin))/2;
                cout << "Time needed to learn: " << time_needed_to_learn << " sec" << endl;
                emit helper->signalSetTimeToLearn(time_needed_to_learn*20);
            }

            QString qs(s.ref().c_str()+10);
            qs = qs.replace(";", ":");
            qs = qs.replace("lt,", "lt;");
            qs = qs.replace("gt,", "gt;");
            emit helper->signalAnswerLearn(QString((qs.toStdString() + "<br><a name=\"anch\" id=\"anch\">&nbsp;</a>").c_str()));
            emit helper->signalTalkingScrollEndLearn(QString("anch"));
            emit helper->signalSetTimeToLearnElapsed(0);
        }
        if (s.contains("DELETED")) {
            emit fc->ask_again();
        }
        if (s.contains("PROFACT")) {
            emit fc->setFactText(QString::fromStdString(parts[1].ref()));
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
        if (s.contains("CSV_RESULT")) {
            clear_dataset();
            factmodel->setData(make_dataset());


            ifstream csv_data;
            if (!csv_data.is_open()) {
                csv_data.open("_csv-data.txt", ios::in);
            }
            if (!csv_data.is_open()) {
                csv_data.open("../_csv-data.txt", ios::in);
            }
            if (!csv_data.is_open()) {
                csv_data.open("../../_csv-data.txt", ios::in);
            }
            if (csv_data.is_open()) {
                while (csv_data) {
                    std::string line;
                    getline(csv_data, line);

                    add_to_dataset(factmodel->getData(), line.c_str());
                }
                csv_data.close();
            }


            factmodel->updateData();
        }
        if (s.contains("HERE_IS_DB_STRING")) {
        }
        if (s.contains("HERE_IS_ONLINE_DB_STRING")) {
        }
        if (s.contains("IMPORT_DB_SUCCESS")) {
        }
        if (s.contains("GET_WORD_TYPE")) {
/// <br>0. Abbruch. Keine Antwort.<br>1. Verb (z.B. sagen)<br>2. Name und Nomen (z.B Haus)<br>3. Adjektiv, Adverb oder adverbiale Bestimmung (z.B. schnell; immer, ueberall, gestern)<br>4. Pronomen (z.B er, sie, es)<br>5. Fragewort (z.B. warum, wie) oder Konjunktion<br>6. Praeposition (z.B. auf, an, unter)<br><br>Gib bitte die Nummer ein.
            if (0 == strcmp(check_config(freehal::string("more-info"), "dialog"), "frame")) {
                emit helper->signalMsg(QString(("Which part of speech is \"" + parts[1].ref() + "\"?\n\nWelche Wortart hat \"" + parts[1].ref() + "\"?").c_str()));
                emit helper->signalShowMsgWindow();
            }
            else {
                QStringList list;
                list.push_back(QString("Which part of speech is the following word?\nWelche Wortart hat das folgende Wort?"));
                list.push_back(QString::fromStdString(parts[1].ref()));
                list.push_back(parts.size() >= 3 ? QString::fromStdString(parts[2].ref()) : "");
                list.push_back(parts.size() >= 4 ? QString::fromStdString(parts[3].ref()) : "");
                emit helper->signalMoreInfo(list);
            }
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

void Helper::slotMoreInfo(QStringList list) {
    QString text = list.takeFirst();
    QString word = list.takeFirst();
    QString source = list.takeFirst();
    QString sentence = list.takeFirst();

    dialog_moreinfo->user_interface_dialog->i_text->setText(text);
    dialog_moreinfo->user_interface_dialog->i_word->setText(word);
    dialog_moreinfo->user_interface_dialog->i_source->setText(source);
    dialog_moreinfo->user_interface_dialog->i_sentence->setText(sentence);
    dialog_moreinfo->user_interface_dialog->box_genus->hide();
    dialog_moreinfo->show();
}

void FreeHALWindow::on_flowchart_fact_pk_valueChanged(int i)
{
    if (i) {
        QString qs;
        qs.setNum(i);
        main_window->user_interface_main_window->flowchart_edit->setText("Please wait...");

        freehal::comm_send("GET:PROFACT:PK:" + qs.toStdString());
    }
}

void FreeHALWindow::on_plus_clicked()
{
   QString text = this->user_interface_main_window->flowchart_view->text();
   freehal::comm_send("QUESTION:/SCORE +10 " + text.toStdString());
}

void FreeHALWindow::on_minus_clicked()
{
   QString text = this->user_interface_main_window->flowchart_view->text();
   freehal::comm_send("QUESTION:/SCORE -10 " + text.toStdString());
}


void Dialog1::on_ask_dialog_toggled(bool checked)
{
    set_config(freehal::string("more-info"), ((checked) ? "dialog" : "frame"));
}

void Dialog1::on_ask_frame_toggled(bool checked)
{
    set_config(freehal::string("more-info"), ((checked) ? "frame" : "dialog"));
}

void Dialog1::on_limit_amount_of_answers_toggled(bool checked)
{
    set_config(freehal::string("limit-amount-of-answers"), ((checked) ? "1" : "0"));
}




///////////////////////////////////////////////////////////////////////////////
// Database Tab (DB Tool)

FactModel* factmodel;

void init_db_tool()
{
    factmodel = new FactModel();
    main_window->user_interface_main_window->tableView->setModel(factmodel);
}

int FactModel::count() const
{
    return (dataset->size);
}

int FactModel::rowCount(const QModelIndex &parent) const
{
    return count();
}

int FactModel::columnCount(const QModelIndex &parent) const
{
    return 6;
}

QVariant FactModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= count())
        return QVariant();

    if (role == Qt::DisplayRole)
        return index.internalPointer()?QString((char*)index.internalPointer()):QString("");
    else
        return QVariant();
}

QStringList stem(QStringList list) {
    for (int u = 0; u < list.size(); ++u) {
        QString word(list[u]);

        // Delete articles...
        {
            if (word.size() >= 6) {
                word.replace(QRegExp("^ein", Qt::CaseInsensitive), "*");
                word.replace(QRegExp("ein",  Qt::CaseInsensitive), "ei*");
                word.replace(QRegExp("ein$", Qt::CaseInsensitive), "*");
            }
            word.replace(QRegExp("^(der|die|das|den|dem|des)$", Qt::CaseInsensitive), "");

            if (word.size() <= 3) {
                continue;
            }

            // That's the stemming algorithm
            if (word.contains(QRegExp("((ein)|(ie.)|(tes))$", Qt::CaseInsensitive))) {
                continue;
            }

            if (word.contains(QRegExp("ei.$", Qt::CaseInsensitive))) {
                continue;
            }

            word.replace(QRegExp("ern$", Qt::CaseInsensitive), "*");
            word.replace(QRegExp("e[mnrs]$", Qt::CaseInsensitive), "*");
            word.replace(QRegExp("e$", Qt::CaseInsensitive), "*");
            if (word.contains(QRegExp("[bdfghklmnrt]s$", Qt::CaseInsensitive))) {
                word.replace(QRegExp("s$", Qt::CaseInsensitive), "*");
            }
            word.replace(QRegExp("est$", Qt::CaseInsensitive), "*");
            word.replace(QRegExp("e[nr]$", Qt::CaseInsensitive), "*");
            if (word.contains(QRegExp("[bdfghklmnt]st$", Qt::CaseInsensitive))) {
                if (word.size() > 5) {
                    word.replace(QRegExp("st$", Qt::CaseInsensitive), "*");
                }
            }
            if (word.contains(QRegExp("(end|ung)$", Qt::CaseInsensitive))) {
                word.replace(QRegExp("(end|ung)$", Qt::CaseInsensitive), "*");

                if (word.contains(QRegExp("ig$", Qt::CaseInsensitive)) && !word.contains(QRegExp("eig$", Qt::CaseInsensitive))) {
                    word.replace(QRegExp("ig$", Qt::CaseInsensitive), "*");
                }
            }
            if (word.contains(QRegExp("(ig|ik|isch)$", Qt::CaseInsensitive)) && !word.contains(QRegExp("e(ig|ik|isch)$", Qt::CaseInsensitive))) {
                word.replace(QRegExp("((ig|ik|isch)$", Qt::CaseInsensitive), "*");
            }
            word.replace(QRegExp("(lich|heit)$", Qt::CaseInsensitive), "*");
            word.replace(QRegExp("(lich|ig)?keit$", Qt::CaseInsensitive), "*");

        }
    }
    QString str = list.join(" ");
    str.replace(QRegExp("[*]\\s/", Qt::CaseInsensitive), "*");
    str.replace(QRegExp("\\s[*]/", Qt::CaseInsensitive), "*");
    str.replace(QRegExp("^\\s+/", Qt::CaseInsensitive), "");
    str.replace(QRegExp("\\s+$/", Qt::CaseInsensitive), "");
    list = str.split(" ");
    return list;
}

QModelIndex FactModel::index(int row, int column, const QModelIndex & parent = QModelIndex()) const
{
    if (indexcache.contains(row)) {
        if (indexcache.value(row)->value(column)) {
            if (indexcache.value(row)->value(column)->size()) {
                QString* ref = indexcache.value(row)->at(column);
                QModelIndex index = createIndex(row, column, (void*)(ref->toStdString().c_str()));

                return index;
            }
        }
    }
    return createIndex(row, column, 0);
}

void FactModel::computeIndex(int row, int column)
{
    if (column < 0)
        return;
    int raw_column = -1;
    switch (column) {
        case 0:     raw_column = 6;     break;
        case 1:     raw_column = 1;     break;
        case 2:     raw_column = 2;     break;
        case 3:     raw_column = 3;     break;
        case 4:     raw_column = 4;     break;
        case 5:     raw_column = 7;     break;
    }
    if (raw_column < 0)
        return;

    QString ref;

    if (dataset->data) {
        if (dataset->data[row]) {
            if (dataset->data[row][raw_column]) {
                char str[128];
                strncpy(str, dataset->data[row][raw_column], 127);
                ref = QString(str);
            }
        }
    }

    if (column == 5) {
        if (ref.contains("1")) {
            ref = "true     ";
        }
        else if (ref.contains("5")) {
            ref = "maybe    ";
        }
        else {
            ref = "false    ";
        }
    }

    if (column == 1) {
        QStringList modal_verbs;
        if (ref.size() >= 6) {
            if (ref[ref.size()-5+0] == '1') {
                modal_verbs.push_back(" / must");
            }
            if (ref[ref.size()-5+1] == '1') {
                modal_verbs.push_back(" / want");
            }
            if (ref[ref.size()-5+2] == '1') {
                modal_verbs.push_back(" / can");
            }
            if (ref[ref.size()-5+3] == '1') {
                modal_verbs.push_back(" / may");
            }
            if (ref[ref.size()-5+4] == '1') {
                modal_verbs.push_back(" / should");
            }
        }


        ref.replace("0", "");
        ref.replace("1", "");
        ref += modal_verbs.join("");
    }

    if (ref.contains("NULL"))
        ref.clear();

    while (ref.size() <= 10) {
        ref += " ";
    }

    QList<QString*>* l = indexcache.value(row);
    if (l) {
        (*l)[column]->append(ref);
        printf("col: %d\tref:%s\n", column, ref.toStdString().c_str());
    }
}

QVariant FactModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0: return QString("File");
            case 1: return QString("Verb");
            case 2: return QString("Subject");
            case 3: return QString("Object");
            case 4: return QString("Adverbs");
            case 5: return QString("Truth");
            default: return QString("");
        }
    }
    else {
        char* pk = "?";
        if (dataset->data) {
            if (dataset->data[section]) { // row
                pk = (char*)(dataset->data[section][0]); // column
            }
        }
        return QString(pk);
    }
}

struct DATASET* FactModel::getData() {
    return dataset;
}

bool FactModel::hasData() {
    return dataset->size ? 1 : 0;
}

void FactModel::setData(struct DATASET* set) {
    dataset = set;
}

void FactModel::updateData() {
    beginRemoveRows(QModelIndex(), 0, count()-1);
    endRemoveRows();
    this->removeRows(0, this->rowCount(QModelIndex()), QModelIndex());

    QList<QList<QString*>*> values = indexcache.values();
    for (int i = 0; i < values.size(); ++i) {
        if (values.at(i)) {
            for (int j = 0; j < values.at(i)->size(); ++j) {
                if (values.at(i)->at(j)) {
                    delete(values.at(i)->at(j));
                }
            }

            delete(values.at(i));
        }
    }
    indexcache.clear();


    int row = 0;
    while (row < dataset->size) {
        QList<QString*>* l = new QList<QString*>;
        for (int z = 0; z <= 11; ++z) {
            l->push_back(new QString());
        }
        indexcache.insert(row, l);

        int col = 0;
        while (col < 7) {
            computeIndex(row, col);
            ++col;
        }
        ++row;
    }

    if (dataset->size) {
        beginInsertRows(QModelIndex(), 0, count()-1);
        endInsertRows();
        printf("Rows: %d\n", count());
    }
}

void Fact::delete_fact() {
    char pkey[LINE_SIZE];
    strncpy(pkey, pk, LINE_SIZE-1);
    freehal::comm_send("DELETE:FACT:PK:" + freehal::string(pkey));

    /*
    int v_value = main_window->user_interface_main_window->tableView->verticalScrollBar()->value();
    if (window->getLastProcess() == MATCHING_FACTS) {
        window->on_matchingfacts_clicked();
    }
    main_window->user_interface_main_window->tableView->verticalScrollBar()->setValue(v_value);
    */
}

enum LAST_PROCESS FreeHALWindow::getLastProcess() {
    return lastprocess;
}

void FreeHALWindow::setLastProcess(enum LAST_PROCESS p) {
    lastprocess = p;
}

void FreeHALWindow::setupMenu()
{
    user_interface_main_window->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    this->connect(user_interface_main_window->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(menuClick(QPoint)));
}

void FreeHALWindow::menuClick(QPoint p) {
    QModelIndex index = user_interface_main_window->tableView->indexAt(p);

    //if (index.column() == 0) {
    if (factmodel->hasData()) {
        Fact fact((char*)(factmodel->getData()->data[index.row()][0]), this);
        Fact* fact_ref = &fact;

        QAction* deleteAct = new QAction(tr("&Delete"), this);
        deleteAct->setStatusTip(tr("Delete from database and .pro file"));
        connect(deleteAct, SIGNAL(triggered()), fact_ref, SLOT(delete_fact()));

        QMenu menu(this);
        menu.addAction(deleteAct);
        p.setY(p.y()+40);
        p.setX(p.x()+50);
        menu.exec(user_interface_main_window->tableView->mapToGlobal(p));

        printf("pk: %s\n", (char*)(factmodel->getData()->data[index.row()][0]));
    }
}

QString FreeHALWindow::make_csv()
{
    bool must = false;
    bool want = false;
    bool can = false;
    bool may = false;
    bool should = false;

    QString verbs_string = user_interface_main_window->verb->text();
    QStringList verbs = verbs_string.split(" ");
    for (int i = 0; i < verbs.size(); ++i) {
        if (!QString(verbs[i]).contains("|")) {
            QString verb(verbs[i]);

            if (verb.contains("must") || verb.contains("muss") || verb.contains("muess")) {
                must = true;
                verbs.removeAt(i);
                --i;
                continue;
            }
            if (verb.contains("want") || verb.contains("will") || verb.contains("woll")) {
                want = true;
                verbs.removeAt(i);
                --i;
                continue;
            }
            if (verb.contains("kann") || verb.contains("koenn") || verb.contains("can")) {
                can = true;
                verbs.removeAt(i);
                --i;
                continue;
            }
            if (verb.contains("may") || verb.contains("darf") || verb.contains("duerf")) {
                may = true;
                verbs.removeAt(i);
                --i;
                continue;
            }
            if (verb.contains("should") || verb.contains("soll")) {
                should = true;
                verbs.removeAt(i);
                --i;
                continue;
            }

            verb.replace(QRegExp("enen$", Qt::CaseInsensitive), "");
            verb.replace(QRegExp("en$", Qt::CaseInsensitive), "");
            verb.replace(QRegExp("st$", Qt::CaseInsensitive), "");
            verb.replace(QRegExp("t$", Qt::CaseInsensitive), "");
            verb.replace(QRegExp("n$", Qt::CaseInsensitive), "");
            verb.replace(QRegExp("e$", Qt::CaseInsensitive), "");

            if (verb == "bi" || verb == "is" || verb == "sind" || verb.contains("heis")) {
                verb = "bin|bist|ist|sind|seid|sein|heisst|heisse|heissen";
            }
            else if (verb == "gib" || verb == "geb") {
                verb = "gebe|geben|gibst|gib|gibt|gebt";
            }
            else {
                QStringList possibilities;
                possibilities.push_back(verb + "enen");
                possibilities.push_back(verb + "en");
                possibilities.push_back(verb + "st");
                possibilities.push_back(verb + "t");
                possibilities.push_back(verb + "n");
                possibilities.push_back(verb + "e");
                possibilities.push_back(verb);
                verb = possibilities.join("|");
            }

            verbs[i] = verb;
        }
    }
    QString verb = verbs.join(" ").toLower();
    if (!verbs_string.size()) {
        verb = "";
    }

    QString subject = stem(user_interface_main_window->subject->text().split(" ")).join(" ").toLower();
    QString object = stem(user_interface_main_window->object->text().split(" ")).join(" ").toLower();
    QString adverbs = user_interface_main_window->adverbs->text().toLower();
    QString questionword = user_interface_main_window->questionword->text().toLower();
    QString extra = user_interface_main_window->extra->text().toLower();
    QString context = "default";

    if (subject.size() == 0 && object.size()) {
        subject = object;
        object.clear();
    }

    if (questionword.contains("wie") || questionword.contains("how")) {
        context = "q_how";
    }
    else if (questionword.contains("welch")) {
        context = "q_what_prep";
    }
    else if (questionword.contains("woher")) {
        context = "q_from_where";
    }
    else if (questionword.contains("wo") || questionword.contains("where")) {
        context = "q_where";
    }
    else if (questionword.contains("wer") || questionword.contains("who")) {
        context = "q_who";
    }
    else if (questionword.contains("was") || questionword.contains("what")) {
        if (user_interface_main_window->verb->text().contains("bist")) { // <-- must be "ui->verb->text()", not "verb" !!!
            context = "q_what_exactly";
        }
        else {
            context = "q_what_weakly";
        }
    }
    else if (extra.contains("enumall")) {
        context = "enum_all";
    }

    QString csv;
    csv += verb.size() ? verb : "0";
    csv += "^";
    csv += subject.size() ? subject : "0";
    csv += "^";
    csv += object.size() ? object : "0";
    csv += "^";
    csv += adverbs.size() ? adverbs : "0";
    csv += "^";
    csv += extra.size() ? extra : "0";
    csv += "^";
    csv += context;
    csv += "^0^"; // <- primary key
    csv +=   want ? "1" : "0";
    csv += "^";
    csv +=   must ? "1" : "0";
    csv += "^";
    csv +=    can ? "1" : "0";
    csv += "^";
    csv +=    may ? "1" : "0";
    csv += "^";
    csv += should ? "1" : "0";
    return csv;
}

void* send_csv_request(char* csv_request) {
    freehal::comm_send("CSV:" + freehal::string(csv_request));
    cout << endl << csv_request << endl << endl;
}

void FreeHALWindow::on_matchingfacts_clicked()
{
    setLastProcess(MATCHING_FACTS);
    QString csv = make_csv();

    char* csv_request = strdup(csv.toStdString().c_str());
    boost::thread t_v(boost::bind(send_csv_request, csv_request));
}


void FreeHALWindow::on_allfacts_clicked()
{
    setLastProcess(ALL_FACTS);
    QString csv = make_csv();
    csv += "^";
    csv += "everything";

    char* csv_request = strdup(csv.toStdString().c_str());
    boost::thread t_v(boost::bind(send_csv_request, csv_request));
}

void FreeHALWindow::on_subject_returnPressed()
{
    on_allfacts_clicked();
}

void FreeHALWindow::on_verb_returnPressed()
{
    on_matchingfacts_clicked();
}

void FreeHALWindow::on_object_returnPressed()
{
    on_matchingfacts_clicked();
}

void FreeHALWindow::on_adverbs_returnPressed()
{
    on_matchingfacts_clicked();
}

void FreeHALWindow::on_questionword_returnPressed()
{
    on_matchingfacts_clicked();
}

void FreeHALWindow::on_extra_returnPressed()
{
    on_matchingfacts_clicked();
}

void FreeHALWindow::on_double_facts_clicked()
{
    setLastProcess(MATCHING_FACTS);
    QString csv;
    csv += "0^0^0^0^0^double_facts^0^0^0^0^0^0";

    char* csv_request = strdup(csv.toStdString().c_str());
    boost::thread t_v(boost::bind(send_csv_request, csv_request));
}

void clear_dataset() {
    struct DATASET* set = factmodel->getData();
    if (set && set->data) {
        int a;
        for (a = 0; a < set->size; ++a) {
            if (set->data[a]) {
                int b;
                for (b = 0; b < set->column_count; ++b) {
                    free(set->data[a][b]);
                    set->data[a][b] = 0;
                }
            }
            free(set->data[a]);
            set->data[a] = 0;
        }

        free(set->data);
        set->data = 0;
    }
}

struct DATASET* make_dataset() {
    struct DATASET* set = (struct DATASET*)malloc(sizeof(struct DATASET));
    set->size = 0;
    set->column_count = 8;
    set->data = (char***)calloc(sizeof(char*), max_amount_of_answers_in_db_tool+1);
    set->err  = 0;

    int record = 0;
    int record_item = 0;
    int record_item_pos = 0;
    set->data[record] = (char**)calloc(sizeof(char*), 100);
    set->data[record][record_item] = (char*)calloc(sizeof(char), 100);

    return set;
}
void add_to_dataset(struct DATASET* set, const char* csv) {

    if (set->size >= max_amount_of_answers_in_db_tool) {
        return;
    }
    if (strstr(csv, "^^^") && strstr(csv, "^^^") < csv+10) {
        return;
    }

    cout << csv << endl;

    int length;
    int record = set->size;
    int record_item = 0;
    int record_item_pos = 0;
    set->data[record] = (char**)calloc(sizeof(char*), 100);
    set->data[record][record_item] = (char*)calloc(sizeof(char), 100);
    for (length = 0; csv[length]; ++length) {
        if (record_item_pos < 99 && record_item < 8) {
            if (csv[length] == '|') {
                ++record;
                set->data[record] = (char**)calloc(sizeof(char*), 100);
                record_item = 0;
                record_item_pos = 0;
                set->data[record][record_item] = (char*)calloc(sizeof(char), 100);
                continue;
            }
            if (csv[length] == '^') {
                record_item_pos = 0;
                ++record_item;
                set->data[record][record_item] = (char*)calloc(sizeof(char), 100);
                continue;
            }


            set->data[record][record_item][record_item_pos] = csv[length];

            if (set->data[record][record_item][record_item_pos] == ';') {
                set->data[record][record_item][record_item_pos] = ':';
            }

            ++record_item_pos;
        }
        else if (csv[length] == '|') {
            ++record;
            set->data[record] = (char**)calloc(sizeof(char*), 100);
            record_item = 0;
            record_item_pos = 0;
            set->data[record][record_item] = (char*)calloc(sizeof(char), 100);
            continue;
        }
    }

    ++(set->size);
}

