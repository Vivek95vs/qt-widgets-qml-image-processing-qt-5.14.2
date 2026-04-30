/****************************************************************************
** Meta object code from reading C++ file 'SocketClient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../SocketClient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SocketClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SocketClient_t {
    QByteArrayData data[19];
    char stringdata0[221];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SocketClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SocketClient_t qt_meta_stringdata_SocketClient = {
    {
QT_MOC_LITERAL(0, 0, 12), // "SocketClient"
QT_MOC_LITERAL(1, 13, 16), // "ipAddressChanged"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 11), // "portChanged"
QT_MOC_LITERAL(4, 43, 17), // "connectionChanged"
QT_MOC_LITERAL(5, 61, 12), // "dataReceived"
QT_MOC_LITERAL(6, 74, 4), // "data"
QT_MOC_LITERAL(7, 79, 13), // "errorOccurred"
QT_MOC_LITERAL(8, 93, 5), // "error"
QT_MOC_LITERAL(9, 99, 11), // "onConnected"
QT_MOC_LITERAL(10, 111, 14), // "onDisconnected"
QT_MOC_LITERAL(11, 126, 11), // "onReadyRead"
QT_MOC_LITERAL(12, 138, 15), // "connectToServer"
QT_MOC_LITERAL(13, 154, 20), // "disconnectFromServer"
QT_MOC_LITERAL(14, 175, 8), // "sendData"
QT_MOC_LITERAL(15, 184, 11), // "receiveData"
QT_MOC_LITERAL(16, 196, 9), // "ipAddress"
QT_MOC_LITERAL(17, 206, 4), // "port"
QT_MOC_LITERAL(18, 211, 9) // "connected"

    },
    "SocketClient\0ipAddressChanged\0\0"
    "portChanged\0connectionChanged\0"
    "dataReceived\0data\0errorOccurred\0error\0"
    "onConnected\0onDisconnected\0onReadyRead\0"
    "connectToServer\0disconnectFromServer\0"
    "sendData\0receiveData\0ipAddress\0port\0"
    "connected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SocketClient[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       3,   92, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06 /* Public */,
       3,    0,   75,    2, 0x06 /* Public */,
       4,    0,   76,    2, 0x06 /* Public */,
       5,    1,   77,    2, 0x06 /* Public */,
       7,    1,   80,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   83,    2, 0x08 /* Private */,
      10,    0,   84,    2, 0x08 /* Private */,
      11,    0,   85,    2, 0x08 /* Private */,

 // methods: name, argc, parameters, tag, flags
      12,    0,   86,    2, 0x02 /* Public */,
      13,    0,   87,    2, 0x02 /* Public */,
      14,    1,   88,    2, 0x02 /* Public */,
      15,    0,   91,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    6,
    QMetaType::Void, QMetaType::QString,    8,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    6,
    QMetaType::QByteArray,

 // properties: name, type, flags
      16, QMetaType::QString, 0x00495103,
      17, QMetaType::Int, 0x00495103,
      18, QMetaType::Bool, 0x00495001,

 // properties: notify_signal_id
       0,
       1,
       2,

       0        // eod
};

void SocketClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SocketClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ipAddressChanged(); break;
        case 1: _t->portChanged(); break;
        case 2: _t->connectionChanged(); break;
        case 3: _t->dataReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 4: _t->errorOccurred((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->onConnected(); break;
        case 6: _t->onDisconnected(); break;
        case 7: _t->onReadyRead(); break;
        case 8: _t->connectToServer(); break;
        case 9: _t->disconnectFromServer(); break;
        case 10: _t->sendData((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 11: { QByteArray _r = _t->receiveData();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SocketClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SocketClient::ipAddressChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SocketClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SocketClient::portChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SocketClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SocketClient::connectionChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SocketClient::*)(const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SocketClient::dataReceived)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (SocketClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SocketClient::errorOccurred)) {
                *result = 4;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<SocketClient *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->ipAddress(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->port(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->connected(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<SocketClient *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setIpAddress(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setPort(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject SocketClient::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_SocketClient.data,
    qt_meta_data_SocketClient,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SocketClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SocketClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SocketClient.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SocketClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void SocketClient::ipAddressChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SocketClient::portChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SocketClient::connectionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void SocketClient::dataReceived(const QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SocketClient::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
