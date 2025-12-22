/****************************************************************************
** Meta object code from reading C++ file 'qglbegin.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.15)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "qglbegin.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qglbegin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.15. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QGLBegin_t {
    QByteArrayData data[8];
    char stringdata0[95];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QGLBegin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QGLBegin_t qt_meta_stringdata_QGLBegin = {
    {
QT_MOC_LITERAL(0, 0, 8), // "QGLBegin"
QT_MOC_LITERAL(1, 9, 14), // "atoms_selected"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 16), // "atoms_deselected"
QT_MOC_LITERAL(4, 42, 12), // "comboRequest"
QT_MOC_LITERAL(5, 55, 12), // "data_changed"
QT_MOC_LITERAL(6, 68, 13), // "playmovieslot"
QT_MOC_LITERAL(7, 82, 12) // "playfreqslot"

    },
    "QGLBegin\0atoms_selected\0\0atoms_deselected\0"
    "comboRequest\0data_changed\0playmovieslot\0"
    "playfreqslot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QGLBegin[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,
       3,    0,   45,    2, 0x06 /* Public */,
       4,    0,   46,    2, 0x06 /* Public */,
       5,    0,   47,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   48,    2, 0x0a /* Public */,
       7,    0,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QGLBegin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QGLBegin *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->atoms_selected(); break;
        case 1: _t->atoms_deselected(); break;
        case 2: _t->comboRequest(); break;
        case 3: _t->data_changed(); break;
        case 4: _t->playmovieslot(); break;
        case 5: _t->playfreqslot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (QGLBegin::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QGLBegin::atoms_selected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (QGLBegin::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QGLBegin::atoms_deselected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (QGLBegin::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QGLBegin::comboRequest)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (QGLBegin::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QGLBegin::data_changed)) {
                *result = 3;
                return;
            }
        }
    }
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject QGLBegin::staticMetaObject = { {
    QMetaObject::SuperData::link<QOpenGLWidget::staticMetaObject>(),
    qt_meta_stringdata_QGLBegin.data,
    qt_meta_data_QGLBegin,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QGLBegin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QGLBegin::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QGLBegin.stringdata0))
        return static_cast<void*>(this);
    return QOpenGLWidget::qt_metacast(_clname);
}

int QGLBegin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QOpenGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void QGLBegin::atoms_selected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void QGLBegin::atoms_deselected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void QGLBegin::comboRequest()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void QGLBegin::data_changed()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
