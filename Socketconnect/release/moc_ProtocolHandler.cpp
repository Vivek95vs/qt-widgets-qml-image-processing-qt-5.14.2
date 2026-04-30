/****************************************************************************
** Meta object code from reading C++ file 'ProtocolHandler.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../ProtocolHandler.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ProtocolHandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ProtocolHandler_t {
    QByteArrayData data[26];
    char stringdata0[356];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ProtocolHandler_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ProtocolHandler_t qt_meta_stringdata_ProtocolHandler = {
    {
QT_MOC_LITERAL(0, 0, 15), // "ProtocolHandler"
QT_MOC_LITERAL(1, 16, 13), // "clientChanged"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 15), // "feedbackUpdated"
QT_MOC_LITERAL(4, 47, 7), // "message"
QT_MOC_LITERAL(5, 55, 20), // "displayCountsChanged"
QT_MOC_LITERAL(6, 76, 17), // "errorCodesChanged"
QT_MOC_LITERAL(7, 94, 23), // "connectionStatusChanged"
QT_MOC_LITERAL(8, 118, 9), // "connected"
QT_MOC_LITERAL(9, 128, 15), // "feedbackDisplay"
QT_MOC_LITERAL(10, 144, 19), // "processReceivedData"
QT_MOC_LITERAL(11, 164, 4), // "data"
QT_MOC_LITERAL(12, 169, 16), // "attemptReconnect"
QT_MOC_LITERAL(13, 186, 14), // "bootParameters"
QT_MOC_LITERAL(14, 201, 12), // "startThreads"
QT_MOC_LITERAL(15, 214, 11), // "stopThreads"
QT_MOC_LITERAL(16, 226, 6), // "client"
QT_MOC_LITERAL(17, 233, 13), // "SocketClient*"
QT_MOC_LITERAL(18, 247, 13), // "displayCount1"
QT_MOC_LITERAL(19, 261, 13), // "displayCount2"
QT_MOC_LITERAL(20, 275, 13), // "displayCount3"
QT_MOC_LITERAL(21, 289, 13), // "displayCount4"
QT_MOC_LITERAL(22, 303, 13), // "displayCount5"
QT_MOC_LITERAL(23, 317, 13), // "displayCount6"
QT_MOC_LITERAL(24, 331, 13), // "displayCount7"
QT_MOC_LITERAL(25, 345, 10) // "errorCodes"

    },
    "ProtocolHandler\0clientChanged\0\0"
    "feedbackUpdated\0message\0displayCountsChanged\0"
    "errorCodesChanged\0connectionStatusChanged\0"
    "connected\0feedbackDisplay\0processReceivedData\0"
    "data\0attemptReconnect\0bootParameters\0"
    "startThreads\0stopThreads\0client\0"
    "SocketClient*\0displayCount1\0displayCount2\0"
    "displayCount3\0displayCount4\0displayCount5\0"
    "displayCount6\0displayCount7\0errorCodes"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ProtocolHandler[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       9,   86, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06 /* Public */,
       3,    1,   70,    2, 0x06 /* Public */,
       5,    0,   73,    2, 0x06 /* Public */,
       6,    0,   74,    2, 0x06 /* Public */,
       7,    1,   75,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   78,    2, 0x08 /* Private */,
      10,    1,   79,    2, 0x08 /* Private */,
      12,    0,   82,    2, 0x08 /* Private */,

 // methods: name, argc, parameters, tag, flags
      13,    0,   83,    2, 0x02 /* Public */,
      14,    0,   84,    2, 0x02 /* Public */,
      15,    0,   85,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    8,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,   11,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      16, 0x80000000 | 17, 0x0049510b,
      18, QMetaType::Int, 0x00495001,
      19, QMetaType::Int, 0x00495001,
      20, QMetaType::Int, 0x00495001,
      21, QMetaType::Int, 0x00495001,
      22, QMetaType::Int, 0x00495001,
      23, QMetaType::Int, 0x00495001,
      24, QMetaType::Int, 0x00495001,
      25, QMetaType::QString, 0x00495001,

 // properties: notify_signal_id
       0,
       2,
       2,
       2,
       2,
       2,
       2,
       2,
       3,

       0        // eod
};

void ProtocolHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ProtocolHandler *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->clientChanged(); break;
        case 1: _t->feedbackUpdated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->displayCountsChanged(); break;
        case 3: _t->errorCodesChanged(); break;
        case 4: _t->connectionStatusChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->feedbackDisplay(); break;
        case 6: _t->processReceivedData((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 7: _t->attemptReconnect(); break;
        case 8: _t->bootParameters(); break;
        case 9: _t->startThreads(); break;
        case 10: _t->stopThreads(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ProtocolHandler::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ProtocolHandler::clientChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ProtocolHandler::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ProtocolHandler::feedbackUpdated)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ProtocolHandler::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ProtocolHandler::displayCountsChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ProtocolHandler::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ProtocolHandler::errorCodesChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ProtocolHandler::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ProtocolHandler::connectionStatusChanged)) {
                *result = 4;
                return;
            }
        }
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< SocketClient* >(); break;
        }
    }

#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<ProtocolHandler *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< SocketClient**>(_v) = _t->client(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->displayCount1(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->displayCount2(); break;
        case 3: *reinterpret_cast< int*>(_v) = _t->displayCount3(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->displayCount4(); break;
        case 5: *reinterpret_cast< int*>(_v) = _t->displayCount5(); break;
        case 6: *reinterpret_cast< int*>(_v) = _t->displayCount6(); break;
        case 7: *reinterpret_cast< int*>(_v) = _t->displayCount7(); break;
        case 8: *reinterpret_cast< QString*>(_v) = _t->errorCodes(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<ProtocolHandler *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setClient(*reinterpret_cast< SocketClient**>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject ProtocolHandler::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ProtocolHandler.data,
    qt_meta_data_ProtocolHandler,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ProtocolHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProtocolHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ProtocolHandler.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ProtocolHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 9;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void ProtocolHandler::clientChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ProtocolHandler::feedbackUpdated(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ProtocolHandler::displayCountsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void ProtocolHandler::errorCodesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void ProtocolHandler::connectionStatusChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
