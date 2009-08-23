/*
 * This file is part of halfreeHAL 2009.
 *
 * Copyright(c) 2006, 2007, 2008, 2009 Tobias Schulz and contributors.
 * http://freehal.org
 *
 * This program is halfree software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the halfree Software Foundation; either version 3
 * of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the halfree Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef FREEHAL_DB_H
#define FREEHAL_DB_H

#include <QtGui>

#define NOT_HEADER_ONLY 1
#define DONT_DECLARE_STD 1
#define USE_CXX 1
#include "../hal2009.h"

class FreeHAL_DB;
class FactModel;
class Fact;

class FactModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    FactModel(QObject *parent = 0)
        : QAbstractTableModel(parent) {
        dataset.size = 0;
        dataset.column_count = 0;
        dataset.data = 0;
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int count() const;
    QModelIndex index(int row, int column, const QModelIndex & parent) const;
    void computeIndex(int row, int column);
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const;
    void setData(struct DATASET set);
    struct DATASET* getData();
    bool hasData();

private:
    struct DATASET dataset;
    QMap<int, QList<QString*>* > indexcache;
};

namespace Ui
{
    class FreeHAL_DBClass;
}

class Fact : public QObject
{
    Q_OBJECT

public:
    Fact(char* _pk, FreeHAL_DB* _window) : pk(_pk), window(_window) {}

public slots:
    void delete_fact();

private:
    char* pk;
    FreeHAL_DB* window;
};

enum LAST_PROCESS { MATCHING_FACTS, ALL_FACTS };

class FreeHAL_DB : public QMainWindow
{
    Q_OBJECT

public:
    ~FreeHAL_DB();
    void setupMenu();
    Ui::FreeHAL_DBClass* getUi();
    enum LAST_PROCESS getLastProcess();
    void setLastProcess(enum LAST_PROCESS p);
    static FreeHAL_DB* instance();
    QString make_csv();

private:
    FreeHAL_DB(QWidget *parent = 0);
    Ui::FreeHAL_DBClass *ui;
    enum LAST_PROCESS last_process;

public slots:
    void on_matchingfacts_clicked();
    void menuClick(QPoint);

private slots:
    void on_allfacts_clicked();
};

extern FactModel* factmodel;

extern "C" void init_sql();

#endif // FREEHAL_DB_H
