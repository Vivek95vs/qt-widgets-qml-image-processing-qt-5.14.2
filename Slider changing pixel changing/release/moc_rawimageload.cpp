/****************************************************************************
** Meta object code from reading C++ file 'rawimageload.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../rawimageload.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rawimageload.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RawImageLoad_t {
    QByteArrayData data[17];
    char stringdata0[216];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RawImageLoad_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RawImageLoad_t qt_meta_stringdata_RawImageLoad = {
    {
QT_MOC_LITERAL(0, 0, 12), // "RawImageLoad"
QT_MOC_LITERAL(1, 13, 18), // "imageLoadedChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 16), // "histogramUpdated"
QT_MOC_LITERAL(4, 50, 18), // "windowWidthChanged"
QT_MOC_LITERAL(5, 69, 18), // "windowLevelChanged"
QT_MOC_LITERAL(6, 88, 12), // "imageUpdated"
QT_MOC_LITERAL(7, 101, 9), // "loadImage"
QT_MOC_LITERAL(8, 111, 14), // "setWindowWidth"
QT_MOC_LITERAL(9, 126, 5), // "width"
QT_MOC_LITERAL(10, 132, 14), // "setWindowLevel"
QT_MOC_LITERAL(11, 147, 5), // "level"
QT_MOC_LITERAL(12, 153, 11), // "imageLoaded"
QT_MOC_LITERAL(13, 165, 13), // "histogramData"
QT_MOC_LITERAL(14, 179, 12), // "QVector<int>"
QT_MOC_LITERAL(15, 192, 11), // "windowWidth"
QT_MOC_LITERAL(16, 204, 11) // "windowLevel"

    },
    "RawImageLoad\0imageLoadedChanged\0\0"
    "histogramUpdated\0windowWidthChanged\0"
    "windowLevelChanged\0imageUpdated\0"
    "loadImage\0setWindowWidth\0width\0"
    "setWindowLevel\0level\0imageLoaded\0"
    "histogramData\0QVector<int>\0windowWidth\0"
    "windowLevel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RawImageLoad[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       4,   66, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,
       3,    0,   55,    2, 0x06 /* Public */,
       4,    0,   56,    2, 0x06 /* Public */,
       5,    0,   57,    2, 0x06 /* Public */,
       6,    0,   58,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
       7,    0,   59,    2, 0x02 /* Public */,
       8,    1,   60,    2, 0x02 /* Public */,
      10,    1,   63,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,   11,

 // properties: name, type, flags
      12, QMetaType::Bool, 0x00495001,
      13, 0x80000000 | 14, 0x00495009,
      15, QMetaType::Int, 0x00495103,
      16, QMetaType::Int, 0x00495103,

 // properties: notify_signal_id
       0,
       1,
       2,
       3,

       0        // eod
};

void RawImageLoad::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RawImageLoad *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->imageLoadedChanged(); break;
        case 1: _t->histogramUpdated(); break;
        case 2: _t->windowWidthChanged(); break;
        case 3: _t->windowLevelChanged(); break;
        case 4: _t->imageUpdated(); break;
        case 5: _t->loadImage(); break;
        case 6: _t->setWindowWidth((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->setWindowLevel((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RawImageLoad::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RawImageLoad::imageLoadedChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (RawImageLoad::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RawImageLoad::histogramUpdated)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (RawImageLoad::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RawImageLoad::windowWidthChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (RawImageLoad::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RawImageLoad::windowLevelChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (RawImageLoad::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RawImageLoad::imageUpdated)) {
                *result = 4;
                return;
            }
        }
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<int> >(); break;
        }
    }

#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<RawImageLoad *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->imageLoaded(); break;
        case 1: *reinterpret_cast< QVector<int>*>(_v) = _t->histogramData(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->windowWidth(); break;
        case 3: *reinterpret_cast< int*>(_v) = _t->windowLevel(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<RawImageLoad *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 2: _t->setWindowWidth(*reinterpret_cast< int*>(_v)); break;
        case 3: _t->setWindowLevel(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject RawImageLoad::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_RawImageLoad.data,
    qt_meta_data_RawImageLoad,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RawImageLoad::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RawImageLoad::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RawImageLoad.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QQuickImageProvider"))
        return static_cast< QQuickImageProvider*>(this);
    return QObject::qt_metacast(_clname);
}

int RawImageLoad::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void RawImageLoad::imageLoadedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void RawImageLoad::histogramUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void RawImageLoad::windowWidthChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void RawImageLoad::windowLevelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void RawImageLoad::imageUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
