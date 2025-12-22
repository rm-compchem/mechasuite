/****************************************************************************
** Meta object code from reading C++ file 'multichoicedialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "multichoicedialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'multichoicedialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MultiChoiceDialog_t {
    QByteArrayData data[8];
    char stringdata0[132];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MultiChoiceDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MultiChoiceDialog_t qt_meta_stringdata_MultiChoiceDialog = {
    {
QT_MOC_LITERAL(0, 0, 17), // "MultiChoiceDialog"
QT_MOC_LITERAL(1, 18, 19), // "on_okbutton_clicked"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 23), // "on_cancelbutton_clicked"
QT_MOC_LITERAL(4, 63, 20), // "on_chb1_stateChanged"
QT_MOC_LITERAL(5, 84, 5), // "state"
QT_MOC_LITERAL(6, 90, 20), // "on_chb2_stateChanged"
QT_MOC_LITERAL(7, 111, 20) // "on_chb3_stateChanged"

    },
    "MultiChoiceDialog\0on_okbutton_clicked\0"
    "\0on_cancelbutton_clicked\0on_chb1_stateChanged\0"
    "state\0on_chb2_stateChanged\0"
    "on_chb3_stateChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MultiChoiceDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x08 /* Private */,
       3,    0,   40,    2, 0x08 /* Private */,
       4,    1,   41,    2, 0x08 /* Private */,
       6,    1,   44,    2, 0x08 /* Private */,
       7,    1,   47,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    5,

       0        // eod
};

void MultiChoiceDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MultiChoiceDialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_okbutton_clicked(); break;
        case 1: _t->on_cancelbutton_clicked(); break;
        case 2: _t->on_chb1_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_chb2_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_chb3_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MultiChoiceDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_MultiChoiceDialog.data,
    qt_meta_data_MultiChoiceDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MultiChoiceDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MultiChoiceDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MultiChoiceDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int MultiChoiceDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
