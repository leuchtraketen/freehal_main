#ifndef QA_H
#define QA_H

#define NOT_HEADER_ONLY 1
#define USE_CXX 1
#include "../hal2009.h"

#include <iostream>
#include <QtGui/QMainWindow>
using namespace std;

namespace Ui
{
    class QAClass;
}

class QA : public QMainWindow
{
    Q_OBJECT

public:
    QA(QWidget *parent = 0);
    ~QA();

private:
    Ui::QAClass *ui;

private slots:
    void on_check_clicked();
signals:
    void signalAppendToOutput(QString);
    void signalUpdateProgressbar(int);

friend void hal2009_handle_signal(void*);
friend void* qa_check(void*);
};

#endif // QA_H
