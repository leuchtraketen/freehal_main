/****************************************************************************
** Meta object code from reading C++ file 'freehal_db.h'
**
** Created: Sun Aug 23 20:23:17 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "freehal_db.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'freehal_db.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FactModel[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

       0        // eod
};

static const char qt_meta_stringdata_FactModel[] = {
    "FactModel\0"
};

const QMetaObject FactModel::staticMetaObject = {
    { &QAbstractTableModel::staticMetaObject, qt_meta_stringdata_FactModel,
      qt_meta_data_FactModel, 0 }
};

const QMetaObject *FactModel::metaObject() const
{
    return &staticMetaObject;
}

void *FactModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FactModel))
        return static_cast<void*>(const_cast< FactModel*>(this));
    return QAbstractTableModel::qt_metacast(_clname);
}

int FactModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractTableModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_Fact[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
       6,    5,    5,    5, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Fact[] = {
    "Fact\0\0delete_fact()\0"
};

const QMetaObject Fact::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Fact,
      qt_meta_data_Fact, 0 }
};

const QMetaObject *Fact::metaObject() const
{
    return &staticMetaObject;
}

void *Fact::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Fact))
        return static_cast<void*>(const_cast< Fact*>(this));
    return QObject::qt_metacast(_clname);
}

int Fact::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: delete_fact(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_FreeHAL_DB[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      39,   11,   11,   11, 0x0a,
      57,   11,   11,   11, 0x08,
      82,   11,   11,   11, 0x08,
     114,   11,   11,   11, 0x08,
     141,   11,   11,   11, 0x08,
     167,   11,   11,   11, 0x08,
     191,   11,   11,   11, 0x08,
     218,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FreeHAL_DB[] = {
    "FreeHAL_DB\0\0on_matchingfacts_clicked()\0"
    "menuClick(QPoint)\0on_extra_returnPressed()\0"
    "on_questionword_returnPressed()\0"
    "on_adverbs_returnPressed()\0"
    "on_object_returnPressed()\0"
    "on_verb_returnPressed()\0"
    "on_subject_returnPressed()\0"
    "on_allfacts_clicked()\0"
};

const QMetaObject FreeHAL_DB::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_FreeHAL_DB,
      qt_meta_data_FreeHAL_DB, 0 }
};

const QMetaObject *FreeHAL_DB::metaObject() const
{
    return &staticMetaObject;
}

void *FreeHAL_DB::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FreeHAL_DB))
        return static_cast<void*>(const_cast< FreeHAL_DB*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int FreeHAL_DB::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_matchingfacts_clicked(); break;
        case 1: menuClick((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 2: on_extra_returnPressed(); break;
        case 3: on_questionword_returnPressed(); break;
        case 4: on_adverbs_returnPressed(); break;
        case 5: on_object_returnPressed(); break;
        case 6: on_verb_returnPressed(); break;
        case 7: on_subject_returnPressed(); break;
        case 8: on_allfacts_clicked(); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
