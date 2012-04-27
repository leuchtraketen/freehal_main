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

#include "freehal_db.h"
#include "ui_freehal_db.h"

extern "C" struct DATASET cxxhal2009_get_csv(char* csv_request);

FactModel* factmodel;

FreeHAL_DB::FreeHAL_DB(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::FreeHAL_DBClass)
{
    ui->setupUi(this);

    factmodel = new FactModel();
    ui->tableView->setModel(factmodel);
}

FreeHAL_DB* FreeHAL_DB::instance() {
    static FreeHAL_DB* instance_ref = 0;
    if (!instance_ref) {
        instance_ref = new FreeHAL_DB();
    }
    return instance_ref;
}

FreeHAL_DB::~FreeHAL_DB()
{
    delete ui;
}

int FactModel::count() const
{
    return (dataset.size);
}

int FactModel::rowCount(const QModelIndex &parent) const
{
    return count();
}

int FactModel::columnCount(const QModelIndex &parent) const
{
    return 7;
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
    int column_true = column;
    switch (column_true) {
        case 6:     column_true = 0;     break;
        case 2:     column_true = 1;     break;
        case 1:     column_true = 2;     break;
        case 11:    column_true = 3;     break;
        case 3:     column_true = 4;     break;
        case 4:     column_true = 5;     break;
        case 7:     column_true = 6;     break;
        case 5:     column_true = -1;    break;
        case 8:     column_true = -1;    break;
        case 9:     column_true = -1;    break;
        case 10:    column_true = -1;    break;
    }

    if (column_true < 0)
        return;

    QString ref;

    if (dataset.data) {
        if (dataset.data[row]) {
            if (dataset.data[row][column==11?1:column]) {
                char str[128];
                strncpy(str, dataset.data[row][column==11?1:column], 127);
                ref = QString(str);
                while (ref.size() <= 8) {
                    ref += " ";
                }
            }
        }
    }

    if (column == 7) {
        if (ref.contains("1")) {
            ref = "true     ";
        }
        else {
            ref = "wrong    ";
        }
    }

    if (column == 1 && ref.size() >= 6) {
        ref.resize(ref.size()-5);
        while (ref.size() <= 8) {
            ref += " ";
        }
    }

    if (column == 11 && ref.size() >= 6) {
        QStringList modal_verbs;
        if (ref[ref.size()-5+0] == '1') {
            modal_verbs.push_back("must / muessen");
        }
        if (ref[ref.size()-5+1] == '1') {
            modal_verbs.push_back("want / wollen");
        }
        if (ref[ref.size()-5+2] == '1') {
            modal_verbs.push_back("can / koennen");
        }
        if (ref[ref.size()-5+3] == '1') {
            modal_verbs.push_back("may / duerfen");
        }
        if (ref[ref.size()-5+4] == '1') {
            modal_verbs.push_back("should / soll(t)en");
        }
        ref = modal_verbs.join(", ");
    }

    if (ref.contains("NULL"))
        ref.clear();

    /*if (!indexcache.contains(row)) {
        QList<QString*>* l = new QList<QString*>;
        for (int z = 0; z <= 11; ++z) {
            l->push_back(0);
        }
        indexcache.insert(row, l);
    }
    QList<QString*>* l = indexcache.value(row);
    (*l)[column_true] = new QString(ref);
    indexcache.insert(row, l);
    */

    QList<QString*>* l = indexcache.value(row);
    if (l) {
        (*l)[column_true]->append(ref);
        printf("col: %d\tcol_true: %d\tref:%s\n", column, column_true, ref.toStdString().c_str());
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
            case 1: return QString("Subject");
            case 2: return QString("Verben");
            case 3: return QString("Modalverben");
            case 4: return QString("Object");
            case 5: return QString("Adverbs");
            case 6: return QString("Truth");
            default: return QString("");
        }
    }
    else {
        char* pk = "?";
        if (dataset.data) {
            if (dataset.data[section]) { // row
                pk = (char*)(dataset.data[section][0]); // column
            }
        }
        return QString(pk);
    }
}

struct DATASET* FactModel::getData() {
    return &dataset;
}

bool FactModel::hasData() {
    return dataset.size ? 1 : 0;
}

void FactModel::setData(struct DATASET set) {
    beginRemoveRows(QModelIndex(), 0, count()-1);
    endRemoveRows();

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

    dataset = set;
    int row = 0;
    while (row < dataset.size) {
        QList<QString*>* l = new QList<QString*>;
        for (int z = 0; z <= 11; ++z) {
            l->push_back(new QString());
        }
        indexcache.insert(row, l);

        int col = 1;
        while (col <= 11) {
            computeIndex(row, col);
            ++col;
        }
        ++row;
    }

    if (dataset.size) {
        beginInsertRows(QModelIndex(), 0, count()-1);
        endInsertRows();
        printf("Rows: %d\n", count());
    }
}

void Fact::delete_fact() {
    struct RECORD r;
    strncpy(r.pkey, pk, LINE_SIZE-1);
    printf("deleting fact: %s\n", r.pkey);
    sql_begin();
    char* source = sql_del_record(&r);
    if (source) {
        fact_delete_from_source(source);
        free(source);
    }
    sql_end();
    printf("success.\n");

    int v_value = window->getUi()->tableView->verticalScrollBar()->value();
    if (window->getLastProcess() == MATCHING_FACTS) {
        window->on_matchingfacts_clicked();
    }
    window->getUi()->tableView->verticalScrollBar()->setValue(v_value);
}

Ui::FreeHAL_DBClass* FreeHAL_DB::getUi() {
    return ui;
}

enum LAST_PROCESS FreeHAL_DB::getLastProcess() {
    return lastprocess;
}

void FreeHAL_DB::setLastProcess(enum LAST_PROCESS p) {
    lastprocess = p;
}

void FreeHAL_DB::setupMenu()
{
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    this->connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(menuClick(QPoint)));
}

void FreeHAL_DB::menuClick(QPoint p) {
    QModelIndex index = ui->tableView->indexAt(p);

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
        menu.exec(ui->tableView->mapToGlobal(p));

        printf("pk: %s\n", (char*)(factmodel->getData()->data[index.row()][0]));
    }
}

QString FreeHAL_DB::make_csv()
{
    bool must = false;
    bool want = false;
    bool can = false;
    bool may = false;
    bool should = false;

    QStringList verbs = ui->verb->text().split(" ");
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

    QString subject = stem(ui->subject->text().split(" ")).join(" ").toLower();
    QString object = stem(ui->object->text().split(" ")).join(" ").toLower();
    QString adverbs = ui->adverbs->text().toLower();
    QString questionword = ui->questionword->text().toLower();
    QString extra = ui->extra->text().toLower();
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
        if (ui->verb->text().contains("bist")) { // <-- must be "ui->verb->text()", not "verb" !!!
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

void FreeHAL_DB::on_matchingfacts_clicked()
{
    setLastProcess(MATCHING_FACTS);
    QString csv = make_csv();

    char* csv_request = strdup(csv.toStdString().c_str());
    struct DATASET set = cxxhal2009_get_csv(csv_request);
    free(csv_request);
    factmodel->setData(set);
}


void FreeHAL_DB::on_allfacts_clicked()
{
    setLastProcess(ALL_FACTS);
    QString csv = make_csv();
    csv += "^";
    csv += "everything";

    char* csv_request = strdup(csv.toStdString().c_str());
    struct DATASET set = cxxhal2009_get_csv(csv_request);
    free(csv_request);
    factmodel->setData(set);
}

void FreeHAL_DB::on_subject_returnPressed()
{
    on_allfacts_clicked();
}

void FreeHAL_DB::on_verb_returnPressed()
{
    on_matchingfacts_clicked();
}

void FreeHAL_DB::on_object_returnPressed()
{
    on_matchingfacts_clicked();
}

void FreeHAL_DB::on_adverbs_returnPressed()
{
    on_matchingfacts_clicked();
}

void FreeHAL_DB::on_questionword_returnPressed()
{
    on_matchingfacts_clicked();
}

void FreeHAL_DB::on_extra_returnPressed()
{
    on_matchingfacts_clicked();
}
