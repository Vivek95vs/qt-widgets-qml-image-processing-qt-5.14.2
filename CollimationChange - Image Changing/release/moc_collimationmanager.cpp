/****************************************************************************
** Meta object code from reading C++ file 'collimationmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../collimationmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'collimationmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CollimationManager_t {
    QByteArrayData data[33];
    char stringdata0[357];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CollimationManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CollimationManager_t qt_meta_stringdata_CollimationManager = {
    {
QT_MOC_LITERAL(0, 0, 18), // "CollimationManager"
QT_MOC_LITERAL(1, 19, 15), // "cropRectChanged"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 18), // "collimationChanged"
QT_MOC_LITERAL(4, 55, 18), // "imageLoadedChanged"
QT_MOC_LITERAL(5, 74, 12), // "loadRawImage"
QT_MOC_LITERAL(6, 87, 8), // "filePath"
QT_MOC_LITERAL(7, 96, 5), // "width"
QT_MOC_LITERAL(8, 102, 6), // "height"
QT_MOC_LITERAL(9, 109, 16), // "resetCollimation"
QT_MOC_LITERAL(10, 126, 11), // "startResize"
QT_MOC_LITERAL(11, 138, 11), // "handleIndex"
QT_MOC_LITERAL(12, 150, 6), // "startX"
QT_MOC_LITERAL(13, 157, 6), // "startY"
QT_MOC_LITERAL(14, 164, 10), // "imageWidth"
QT_MOC_LITERAL(15, 175, 11), // "imageHeight"
QT_MOC_LITERAL(16, 187, 12), // "updateResize"
QT_MOC_LITERAL(17, 200, 8), // "currentX"
QT_MOC_LITERAL(18, 209, 8), // "currentY"
QT_MOC_LITERAL(19, 218, 9), // "endResize"
QT_MOC_LITERAL(20, 228, 9), // "startMove"
QT_MOC_LITERAL(21, 238, 10), // "updateMove"
QT_MOC_LITERAL(22, 249, 7), // "endMove"
QT_MOC_LITERAL(23, 257, 9), // "startDraw"
QT_MOC_LITERAL(24, 267, 10), // "updateDraw"
QT_MOC_LITERAL(25, 278, 7), // "endDraw"
QT_MOC_LITERAL(26, 286, 8), // "cropRect"
QT_MOC_LITERAL(27, 295, 12), // "originalSize"
QT_MOC_LITERAL(28, 308, 3), // "fid"
QT_MOC_LITERAL(29, 312, 12), // "collimationX"
QT_MOC_LITERAL(30, 325, 12), // "collimationY"
QT_MOC_LITERAL(31, 338, 8), // "hasImage"
QT_MOC_LITERAL(32, 347, 9) // "imageData"

    },
    "CollimationManager\0cropRectChanged\0\0"
    "collimationChanged\0imageLoadedChanged\0"
    "loadRawImage\0filePath\0width\0height\0"
    "resetCollimation\0startResize\0handleIndex\0"
    "startX\0startY\0imageWidth\0imageHeight\0"
    "updateResize\0currentX\0currentY\0endResize\0"
    "startMove\0updateMove\0endMove\0startDraw\0"
    "updateDraw\0endDraw\0cropRect\0originalSize\0"
    "fid\0collimationX\0collimationY\0hasImage\0"
    "imageData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CollimationManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       7,  142, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x06 /* Public */,
       3,    0,   85,    2, 0x06 /* Public */,
       4,    0,   86,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
       5,    3,   87,    2, 0x02 /* Public */,
       9,    0,   94,    2, 0x02 /* Public */,
      10,    5,   95,    2, 0x02 /* Public */,
      16,    2,  106,    2, 0x02 /* Public */,
      19,    0,  111,    2, 0x02 /* Public */,
      20,    4,  112,    2, 0x02 /* Public */,
      21,    2,  121,    2, 0x02 /* Public */,
      22,    0,  126,    2, 0x02 /* Public */,
      23,    4,  127,    2, 0x02 /* Public */,
      24,    2,  136,    2, 0x02 /* Public */,
      25,    0,  141,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int,    6,    7,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QReal, QMetaType::QReal, QMetaType::QReal, QMetaType::QReal,   11,   12,   13,   14,   15,
    QMetaType::Void, QMetaType::QReal, QMetaType::QReal,   17,   18,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QReal, QMetaType::QReal, QMetaType::QReal, QMetaType::QReal,   12,   13,   14,   15,
    QMetaType::Void, QMetaType::QReal, QMetaType::QReal,   17,   18,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QReal, QMetaType::QReal, QMetaType::QReal, QMetaType::QReal,   12,   13,   14,   15,
    QMetaType::Void, QMetaType::QReal, QMetaType::QReal,   17,   18,
    QMetaType::Void,

 // properties: name, type, flags
      26, QMetaType::QRectF, 0x00495001,
      27, QMetaType::QSizeF, 0x00095401,
      28, QMetaType::Double, 0x00095401,
      29, QMetaType::Double, 0x00495001,
      30, QMetaType::Double, 0x00495001,
      31, QMetaType::Bool, 0x00495001,
      32, QMetaType::QString, 0x00495001,

 // properties: notify_signal_id
       0,
       0,
       0,
       1,
       1,
       2,
       2,

       0        // eod
};

void CollimationManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CollimationManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->cropRectChanged(); break;
        case 1: _t->collimationChanged(); break;
        case 2: _t->imageLoadedChanged(); break;
        case 3: _t->loadRawImage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 4: _t->resetCollimation(); break;
        case 5: _t->startResize((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2])),(*reinterpret_cast< qreal(*)>(_a[3])),(*reinterpret_cast< qreal(*)>(_a[4])),(*reinterpret_cast< qreal(*)>(_a[5]))); break;
        case 6: _t->updateResize((*reinterpret_cast< qreal(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2]))); break;
        case 7: _t->endResize(); break;
        case 8: _t->startMove((*reinterpret_cast< qreal(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2])),(*reinterpret_cast< qreal(*)>(_a[3])),(*reinterpret_cast< qreal(*)>(_a[4]))); break;
        case 9: _t->updateMove((*reinterpret_cast< qreal(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2]))); break;
        case 10: _t->endMove(); break;
        case 11: _t->startDraw((*reinterpret_cast< qreal(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2])),(*reinterpret_cast< qreal(*)>(_a[3])),(*reinterpret_cast< qreal(*)>(_a[4]))); break;
        case 12: _t->updateDraw((*reinterpret_cast< qreal(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2]))); break;
        case 13: _t->endDraw(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CollimationManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CollimationManager::cropRectChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CollimationManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CollimationManager::collimationChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CollimationManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CollimationManager::imageLoadedChanged)) {
                *result = 2;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<CollimationManager *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QRectF*>(_v) = _t->cropRect(); break;
        case 1: *reinterpret_cast< QSizeF*>(_v) = _t->originalSize(); break;
        case 2: *reinterpret_cast< double*>(_v) = _t->fid(); break;
        case 3: *reinterpret_cast< double*>(_v) = _t->collimationX(); break;
        case 4: *reinterpret_cast< double*>(_v) = _t->collimationY(); break;
        case 5: *reinterpret_cast< bool*>(_v) = _t->hasImage(); break;
        case 6: *reinterpret_cast< QString*>(_v) = _t->imageData(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject CollimationManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CollimationManager.data,
    qt_meta_data_CollimationManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CollimationManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CollimationManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CollimationManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CollimationManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 7;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void CollimationManager::cropRectChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void CollimationManager::collimationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void CollimationManager::imageLoadedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
