/****************************************************************************
** Meta object code from reading C++ file 'BackendManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../BackendManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BackendManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BackendManager_t {
    QByteArrayData data[51];
    char stringdata0[707];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BackendManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BackendManager_t qt_meta_stringdata_BackendManager = {
    {
QT_MOC_LITERAL(0, 0, 14), // "BackendManager"
QT_MOC_LITERAL(1, 15, 23), // "connectionStatusChanged"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 6), // "status"
QT_MOC_LITERAL(4, 47, 13), // "errorOccurred"
QT_MOC_LITERAL(5, 61, 7), // "message"
QT_MOC_LITERAL(6, 69, 16), // "feedbackReceived"
QT_MOC_LITERAL(7, 86, 4), // "data"
QT_MOC_LITERAL(8, 91, 18), // "motorStatusChanged"
QT_MOC_LITERAL(9, 110, 9), // "motorType"
QT_MOC_LITERAL(10, 120, 15), // "positionUpdated"
QT_MOC_LITERAL(11, 136, 8), // "position"
QT_MOC_LITERAL(12, 145, 14), // "connectToBoard"
QT_MOC_LITERAL(13, 160, 9), // "ipAddress"
QT_MOC_LITERAL(14, 170, 4), // "port"
QT_MOC_LITERAL(15, 175, 19), // "disconnectFromBoard"
QT_MOC_LITERAL(16, 195, 10), // "resetMotor"
QT_MOC_LITERAL(17, 206, 14), // "resetAllMotors"
QT_MOC_LITERAL(18, 221, 13), // "startAutoMode"
QT_MOC_LITERAL(19, 235, 13), // "emergencyStop"
QT_MOC_LITERAL(20, 249, 11), // "motorEnable"
QT_MOC_LITERAL(21, 261, 12), // "motorDisable"
QT_MOC_LITERAL(22, 274, 14), // "servoAutoStart"
QT_MOC_LITERAL(23, 289, 10), // "velocities"
QT_MOC_LITERAL(24, 300, 15), // "targetPositions"
QT_MOC_LITERAL(25, 316, 13), // "servoAutoStop"
QT_MOC_LITERAL(26, 330, 20), // "encoderPositionReset"
QT_MOC_LITERAL(27, 351, 14), // "resetPositions"
QT_MOC_LITERAL(28, 366, 10), // "alarmReset"
QT_MOC_LITERAL(29, 377, 14), // "bootParameters"
QT_MOC_LITERAL(30, 392, 20), // "handleBoardConnected"
QT_MOC_LITERAL(31, 413, 23), // "handleBoardDisconnected"
QT_MOC_LITERAL(32, 437, 16), // "handleBoardError"
QT_MOC_LITERAL(33, 454, 5), // "error"
QT_MOC_LITERAL(34, 460, 18), // "handleFeedbackData"
QT_MOC_LITERAL(35, 479, 16), // "connectionStatus"
QT_MOC_LITERAL(36, 496, 9), // "MotorType"
QT_MOC_LITERAL(37, 506, 11), // "TubeLateral"
QT_MOC_LITERAL(38, 518, 16), // "TubeLongitudinal"
QT_MOC_LITERAL(39, 535, 12), // "TubeVertical"
QT_MOC_LITERAL(40, 548, 20), // "TubeVerticalRotation"
QT_MOC_LITERAL(41, 569, 22), // "TubeHorizontalRotation"
QT_MOC_LITERAL(42, 592, 16), // "DetectorVertical"
QT_MOC_LITERAL(43, 609, 12), // "DetectorTilt"
QT_MOC_LITERAL(44, 622, 10), // "ModeSelect"
QT_MOC_LITERAL(45, 633, 8), // "NullMode"
QT_MOC_LITERAL(46, 642, 11), // "AutoSetMode"
QT_MOC_LITERAL(47, 654, 10), // "ManualMode"
QT_MOC_LITERAL(48, 665, 14), // "MotorDirection"
QT_MOC_LITERAL(49, 680, 9), // "ClockWise"
QT_MOC_LITERAL(50, 690, 16) // "CounterClockwise"

    },
    "BackendManager\0connectionStatusChanged\0"
    "\0status\0errorOccurred\0message\0"
    "feedbackReceived\0data\0motorStatusChanged\0"
    "motorType\0positionUpdated\0position\0"
    "connectToBoard\0ipAddress\0port\0"
    "disconnectFromBoard\0resetMotor\0"
    "resetAllMotors\0startAutoMode\0emergencyStop\0"
    "motorEnable\0motorDisable\0servoAutoStart\0"
    "velocities\0targetPositions\0servoAutoStop\0"
    "encoderPositionReset\0resetPositions\0"
    "alarmReset\0bootParameters\0"
    "handleBoardConnected\0handleBoardDisconnected\0"
    "handleBoardError\0error\0handleFeedbackData\0"
    "connectionStatus\0MotorType\0TubeLateral\0"
    "TubeLongitudinal\0TubeVertical\0"
    "TubeVerticalRotation\0TubeHorizontalRotation\0"
    "DetectorVertical\0DetectorTilt\0ModeSelect\0"
    "NullMode\0AutoSetMode\0ManualMode\0"
    "MotorDirection\0ClockWise\0CounterClockwise"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BackendManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       1,  188, // properties
       3,  192, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  124,    2, 0x06 /* Public */,
       4,    1,  127,    2, 0x06 /* Public */,
       6,    1,  130,    2, 0x06 /* Public */,
       8,    2,  133,    2, 0x06 /* Public */,
      10,    2,  138,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    2,  143,    2, 0x0a /* Public */,
      15,    0,  148,    2, 0x0a /* Public */,
      16,    1,  149,    2, 0x0a /* Public */,
      17,    0,  152,    2, 0x0a /* Public */,
      18,    0,  153,    2, 0x0a /* Public */,
      19,    0,  154,    2, 0x0a /* Public */,
      20,    1,  155,    2, 0x0a /* Public */,
      21,    1,  158,    2, 0x0a /* Public */,
      22,    3,  161,    2, 0x0a /* Public */,
      25,    1,  168,    2, 0x0a /* Public */,
      26,    2,  171,    2, 0x0a /* Public */,
      28,    1,  176,    2, 0x0a /* Public */,
      29,    0,  179,    2, 0x0a /* Public */,
      30,    0,  180,    2, 0x08 /* Private */,
      31,    0,  181,    2, 0x08 /* Private */,
      32,    1,  182,    2, 0x08 /* Private */,
      34,    1,  185,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QVariantMap,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    9,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::Double,    9,   11,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   13,   14,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int, QMetaType::QVariantList, QMetaType::QVariantList,    9,   23,   24,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int, QMetaType::QVariantList,    9,   27,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   33,
    QMetaType::Void, QMetaType::QByteArray,    7,

 // properties: name, type, flags
      35, QMetaType::Bool, 0x00495001,

 // properties: notify_signal_id
       0,

 // enums: name, alias, flags, count, data
      36,   36, 0x0,    7,  207,
      44,   44, 0x0,    3,  221,
      48,   48, 0x0,    2,  227,

 // enum data: key, value
      37, uint(BackendManager::TubeLateral),
      38, uint(BackendManager::TubeLongitudinal),
      39, uint(BackendManager::TubeVertical),
      40, uint(BackendManager::TubeVerticalRotation),
      41, uint(BackendManager::TubeHorizontalRotation),
      42, uint(BackendManager::DetectorVertical),
      43, uint(BackendManager::DetectorTilt),
      45, uint(BackendManager::NullMode),
      46, uint(BackendManager::AutoSetMode),
      47, uint(BackendManager::ManualMode),
      49, uint(BackendManager::ClockWise),
      50, uint(BackendManager::CounterClockwise),

       0        // eod
};

void BackendManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BackendManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connectionStatusChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->errorOccurred((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->feedbackReceived((*reinterpret_cast< const QVariantMap(*)>(_a[1]))); break;
        case 3: _t->motorStatusChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->positionUpdated((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 5: _t->connectToBoard((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->disconnectFromBoard(); break;
        case 7: _t->resetMotor((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->resetAllMotors(); break;
        case 9: _t->startAutoMode(); break;
        case 10: _t->emergencyStop(); break;
        case 11: _t->motorEnable((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->motorDisable((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->servoAutoStart((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QVariantList(*)>(_a[2])),(*reinterpret_cast< const QVariantList(*)>(_a[3]))); break;
        case 14: _t->servoAutoStop((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->encoderPositionReset((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QVariantList(*)>(_a[2]))); break;
        case 16: _t->alarmReset((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->bootParameters(); break;
        case 18: _t->handleBoardConnected(); break;
        case 19: _t->handleBoardDisconnected(); break;
        case 20: _t->handleBoardError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 21: _t->handleFeedbackData((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (BackendManager::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BackendManager::connectionStatusChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (BackendManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BackendManager::errorOccurred)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (BackendManager::*)(const QVariantMap & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BackendManager::feedbackReceived)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (BackendManager::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BackendManager::motorStatusChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (BackendManager::*)(int , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BackendManager::positionUpdated)) {
                *result = 4;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<BackendManager *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->connectionStatus(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject BackendManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_BackendManager.data,
    qt_meta_data_BackendManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *BackendManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BackendManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BackendManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int BackendManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 22)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 22;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void BackendManager::connectionStatusChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BackendManager::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void BackendManager::feedbackReceived(const QVariantMap & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void BackendManager::motorStatusChanged(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void BackendManager::positionUpdated(int _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
