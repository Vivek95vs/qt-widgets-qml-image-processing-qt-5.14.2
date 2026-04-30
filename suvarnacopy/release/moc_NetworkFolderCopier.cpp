/****************************************************************************
** Meta object code from reading C++ file 'NetworkFolderCopier.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../NetworkFolderCopier.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NetworkFolderCopier.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NetworkFolderCopier_t {
    QByteArrayData data[28];
    char stringdata0[325];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetworkFolderCopier_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetworkFolderCopier_t qt_meta_stringdata_NetworkFolderCopier = {
    {
QT_MOC_LITERAL(0, 0, 19), // "NetworkFolderCopier"
QT_MOC_LITERAL(1, 20, 8), // "finished"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 5), // "error"
QT_MOC_LITERAL(4, 36, 12), // "errorMessage"
QT_MOC_LITERAL(5, 49, 8), // "progress"
QT_MOC_LITERAL(6, 58, 7), // "message"
QT_MOC_LITERAL(7, 66, 15), // "fileCopyStarted"
QT_MOC_LITERAL(8, 82, 8), // "fileName"
QT_MOC_LITERAL(9, 91, 4), // "size"
QT_MOC_LITERAL(10, 96, 16), // "fileCopyProgress"
QT_MOC_LITERAL(11, 113, 11), // "bytesCopied"
QT_MOC_LITERAL(12, 125, 10), // "totalBytes"
QT_MOC_LITERAL(13, 136, 16), // "fileCopyFinished"
QT_MOC_LITERAL(14, 153, 7), // "success"
QT_MOC_LITERAL(15, 161, 16), // "directoryCreated"
QT_MOC_LITERAL(16, 178, 4), // "path"
QT_MOC_LITERAL(17, 183, 22), // "authenticationRequired"
QT_MOC_LITERAL(18, 206, 11), // "networkPath"
QT_MOC_LITERAL(19, 218, 9), // "startCopy"
QT_MOC_LITERAL(20, 228, 10), // "sourcePath"
QT_MOC_LITERAL(21, 239, 8), // "destPath"
QT_MOC_LITERAL(22, 248, 8), // "username"
QT_MOC_LITERAL(23, 257, 8), // "password"
QT_MOC_LITERAL(24, 266, 6), // "domain"
QT_MOC_LITERAL(25, 273, 6), // "cancel"
QT_MOC_LITERAL(26, 280, 20), // "handleAuthentication"
QT_MOC_LITERAL(27, 301, 23) // "retryWithAuthentication"

    },
    "NetworkFolderCopier\0finished\0\0error\0"
    "errorMessage\0progress\0message\0"
    "fileCopyStarted\0fileName\0size\0"
    "fileCopyProgress\0bytesCopied\0totalBytes\0"
    "fileCopyFinished\0success\0directoryCreated\0"
    "path\0authenticationRequired\0networkPath\0"
    "startCopy\0sourcePath\0destPath\0username\0"
    "password\0domain\0cancel\0handleAuthentication\0"
    "retryWithAuthentication"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetworkFolderCopier[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x06 /* Public */,
       3,    1,   90,    2, 0x06 /* Public */,
       5,    1,   93,    2, 0x06 /* Public */,
       7,    2,   96,    2, 0x06 /* Public */,
      10,    2,  101,    2, 0x06 /* Public */,
      13,    2,  106,    2, 0x06 /* Public */,
      15,    1,  111,    2, 0x06 /* Public */,
      17,    1,  114,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      19,    5,  117,    2, 0x0a /* Public */,
      19,    4,  128,    2, 0x2a /* Public | MethodCloned */,
      19,    3,  137,    2, 0x2a /* Public | MethodCloned */,
      19,    2,  144,    2, 0x2a /* Public | MethodCloned */,
      25,    0,  149,    2, 0x0a /* Public */,
      26,    1,  150,    2, 0x08 /* Private */,
      27,    0,  153,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::LongLong,    8,    9,
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,   11,   12,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,    8,   14,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, QMetaType::QString,   18,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   20,   21,   22,   23,   24,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   20,   21,   22,   23,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   20,   21,   22,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   20,   21,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   18,
    QMetaType::Void,

       0        // eod
};

void NetworkFolderCopier::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetworkFolderCopier *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: _t->error((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->progress((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->fileCopyStarted((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 4: _t->fileCopyProgress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 5: _t->fileCopyFinished((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 6: _t->directoryCreated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->authenticationRequired((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->startCopy((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5]))); break;
        case 9: _t->startCopy((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 10: _t->startCopy((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 11: _t->startCopy((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 12: _t->cancel(); break;
        case 13: _t->handleAuthentication((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 14: _t->retryWithAuthentication(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NetworkFolderCopier::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkFolderCopier::finished)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NetworkFolderCopier::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkFolderCopier::error)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (NetworkFolderCopier::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkFolderCopier::progress)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (NetworkFolderCopier::*)(const QString & , qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkFolderCopier::fileCopyStarted)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (NetworkFolderCopier::*)(qint64 , qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkFolderCopier::fileCopyProgress)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (NetworkFolderCopier::*)(const QString & , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkFolderCopier::fileCopyFinished)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (NetworkFolderCopier::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkFolderCopier::directoryCreated)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (NetworkFolderCopier::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkFolderCopier::authenticationRequired)) {
                *result = 7;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NetworkFolderCopier::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_NetworkFolderCopier.data,
    qt_meta_data_NetworkFolderCopier,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NetworkFolderCopier::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetworkFolderCopier::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkFolderCopier.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NetworkFolderCopier::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void NetworkFolderCopier::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void NetworkFolderCopier::error(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void NetworkFolderCopier::progress(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void NetworkFolderCopier::fileCopyStarted(const QString & _t1, qint64 _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void NetworkFolderCopier::fileCopyProgress(qint64 _t1, qint64 _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void NetworkFolderCopier::fileCopyFinished(const QString & _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void NetworkFolderCopier::directoryCreated(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void NetworkFolderCopier::authenticationRequired(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
