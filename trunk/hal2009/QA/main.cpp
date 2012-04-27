#include <QtGui/QApplication>
#include "qa.h"

int main(int argc, char *argv[])
{
    sql_engine = (char*)calloc(512 + 1, 1);
    strcpy(sql_engine, "sqlite");


    QApplication a(argc, argv);
    QA w;
    w.show();
    return a.exec();
}
