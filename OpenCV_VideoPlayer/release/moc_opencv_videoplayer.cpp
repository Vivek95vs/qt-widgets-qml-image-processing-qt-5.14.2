/****************************************************************************
** Meta object code from reading C++ file 'opencv_videoplayer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../opencv_videoplayer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'opencv_videoplayer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_OpenCV_VideoPlayer_t {
    QByteArrayData data[30];
    char stringdata0[465];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OpenCV_VideoPlayer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OpenCV_VideoPlayer_t qt_meta_stringdata_OpenCV_VideoPlayer = {
    {
QT_MOC_LITERAL(0, 0, 18), // "OpenCV_VideoPlayer"
QT_MOC_LITERAL(1, 19, 14), // "updateRequired"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 15), // "viewPortChanged"
QT_MOC_LITERAL(4, 51, 23), // "OpenCV_Player_ViewPort*"
QT_MOC_LITERAL(5, 75, 8), // "viewPort"
QT_MOC_LITERAL(6, 84, 15), // "videoUrlChanged"
QT_MOC_LITERAL(7, 100, 8), // "videoUrl"
QT_MOC_LITERAL(8, 109, 22), // "videoFrameCountChanged"
QT_MOC_LITERAL(9, 132, 15), // "videoFrameCount"
QT_MOC_LITERAL(10, 148, 18), // "frameNumberChanged"
QT_MOC_LITERAL(11, 167, 11), // "frameNumber"
QT_MOC_LITERAL(12, 179, 20), // "playbackStateChanged"
QT_MOC_LITERAL(13, 200, 13), // "PlaybackState"
QT_MOC_LITERAL(14, 214, 13), // "playbackState"
QT_MOC_LITERAL(15, 228, 22), // "inputResulationChanged"
QT_MOC_LITERAL(16, 251, 15), // "inputResulation"
QT_MOC_LITERAL(17, 267, 13), // "videoFinished"
QT_MOC_LITERAL(18, 281, 11), // "setViewPort"
QT_MOC_LITERAL(19, 293, 11), // "newViewPort"
QT_MOC_LITERAL(20, 305, 11), // "setVideoUrl"
QT_MOC_LITERAL(21, 317, 11), // "newVideoUrl"
QT_MOC_LITERAL(22, 329, 14), // "setFrameNumber"
QT_MOC_LITERAL(23, 344, 14), // "newFrameNumber"
QT_MOC_LITERAL(24, 359, 16), // "setPlaybackState"
QT_MOC_LITERAL(25, 376, 33), // "OpenCV_VideoPlayer::PlaybackS..."
QT_MOC_LITERAL(26, 410, 16), // "newPlaybackState"
QT_MOC_LITERAL(27, 427, 12), // "StoppedState"
QT_MOC_LITERAL(28, 440, 12), // "PlayingState"
QT_MOC_LITERAL(29, 453, 11) // "PausedState"

    },
    "OpenCV_VideoPlayer\0updateRequired\0\0"
    "viewPortChanged\0OpenCV_Player_ViewPort*\0"
    "viewPort\0videoUrlChanged\0videoUrl\0"
    "videoFrameCountChanged\0videoFrameCount\0"
    "frameNumberChanged\0frameNumber\0"
    "playbackStateChanged\0PlaybackState\0"
    "playbackState\0inputResulationChanged\0"
    "inputResulation\0videoFinished\0setViewPort\0"
    "newViewPort\0setVideoUrl\0newVideoUrl\0"
    "setFrameNumber\0newFrameNumber\0"
    "setPlaybackState\0OpenCV_VideoPlayer::PlaybackState\0"
    "newPlaybackState\0StoppedState\0"
    "PlayingState\0PausedState"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OpenCV_VideoPlayer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       6,  106, // properties
       1,  130, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06 /* Public */,
       3,    1,   75,    2, 0x06 /* Public */,
       6,    1,   78,    2, 0x06 /* Public */,
       8,    1,   81,    2, 0x06 /* Public */,
      10,    1,   84,    2, 0x06 /* Public */,
      12,    1,   87,    2, 0x06 /* Public */,
      15,    1,   90,    2, 0x06 /* Public */,
      17,    0,   93,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      18,    1,   94,    2, 0x0a /* Public */,
      20,    1,   97,    2, 0x0a /* Public */,
      22,    1,  100,    2, 0x0a /* Public */,
      24,    1,  103,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, QMetaType::QUrl,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, QMetaType::QSize,   16,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 4,   19,
    QMetaType::Void, QMetaType::QUrl,   21,
    QMetaType::Void, QMetaType::Int,   23,
    QMetaType::Void, 0x80000000 | 25,   26,

 // properties: name, type, flags
       5, 0x80000000 | 4, 0x0049510b,
       7, QMetaType::QUrl, 0x00495103,
       9, QMetaType::Int, 0x00495001,
      16, QMetaType::QSize, 0x00495001,
      11, QMetaType::Int, 0x00495103,
      14, 0x80000000 | 13, 0x0049510b,

 // properties: notify_signal_id
       1,
       2,
       3,
       6,
       4,
       5,

 // enums: name, alias, flags, count, data
      13,   13, 0x0,    3,  135,

 // enum data: key, value
      27, uint(OpenCV_VideoPlayer::StoppedState),
      28, uint(OpenCV_VideoPlayer::PlayingState),
      29, uint(OpenCV_VideoPlayer::PausedState),

       0        // eod
};

void OpenCV_VideoPlayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OpenCV_VideoPlayer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateRequired(); break;
        case 1: _t->viewPortChanged((*reinterpret_cast< OpenCV_Player_ViewPort*(*)>(_a[1]))); break;
        case 2: _t->videoUrlChanged((*reinterpret_cast< QUrl(*)>(_a[1]))); break;
        case 3: _t->videoFrameCountChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->frameNumberChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->playbackStateChanged((*reinterpret_cast< PlaybackState(*)>(_a[1]))); break;
        case 6: _t->inputResulationChanged((*reinterpret_cast< QSize(*)>(_a[1]))); break;
        case 7: _t->videoFinished(); break;
        case 8: _t->setViewPort((*reinterpret_cast< OpenCV_Player_ViewPort*(*)>(_a[1]))); break;
        case 9: _t->setVideoUrl((*reinterpret_cast< QUrl(*)>(_a[1]))); break;
        case 10: _t->setFrameNumber((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->setPlaybackState((*reinterpret_cast< OpenCV_VideoPlayer::PlaybackState(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< OpenCV_Player_ViewPort* >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< OpenCV_Player_ViewPort* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (OpenCV_VideoPlayer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OpenCV_VideoPlayer::updateRequired)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (OpenCV_VideoPlayer::*)(OpenCV_Player_ViewPort * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OpenCV_VideoPlayer::viewPortChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (OpenCV_VideoPlayer::*)(QUrl );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OpenCV_VideoPlayer::videoUrlChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (OpenCV_VideoPlayer::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OpenCV_VideoPlayer::videoFrameCountChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (OpenCV_VideoPlayer::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OpenCV_VideoPlayer::frameNumberChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (OpenCV_VideoPlayer::*)(PlaybackState );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OpenCV_VideoPlayer::playbackStateChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (OpenCV_VideoPlayer::*)(QSize );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OpenCV_VideoPlayer::inputResulationChanged)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (OpenCV_VideoPlayer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OpenCV_VideoPlayer::videoFinished)) {
                *result = 7;
                return;
            }
        }
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< OpenCV_Player_ViewPort* >(); break;
        }
    }

#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<OpenCV_VideoPlayer *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< OpenCV_Player_ViewPort**>(_v) = _t->viewPort(); break;
        case 1: *reinterpret_cast< QUrl*>(_v) = _t->videoUrl(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->videoFrameCount(); break;
        case 3: *reinterpret_cast< QSize*>(_v) = _t->inputResulation(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->frameNumber(); break;
        case 5: *reinterpret_cast< PlaybackState*>(_v) = _t->playbackState(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<OpenCV_VideoPlayer *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setViewPort(*reinterpret_cast< OpenCV_Player_ViewPort**>(_v)); break;
        case 1: _t->setVideoUrl(*reinterpret_cast< QUrl*>(_v)); break;
        case 4: _t->setFrameNumber(*reinterpret_cast< int*>(_v)); break;
        case 5: _t->setPlaybackState(*reinterpret_cast< PlaybackState*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject OpenCV_VideoPlayer::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_OpenCV_VideoPlayer.data,
    qt_meta_data_OpenCV_VideoPlayer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *OpenCV_VideoPlayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OpenCV_VideoPlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OpenCV_VideoPlayer.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int OpenCV_VideoPlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void OpenCV_VideoPlayer::updateRequired()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void OpenCV_VideoPlayer::viewPortChanged(OpenCV_Player_ViewPort * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void OpenCV_VideoPlayer::videoUrlChanged(QUrl _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void OpenCV_VideoPlayer::videoFrameCountChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void OpenCV_VideoPlayer::frameNumberChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void OpenCV_VideoPlayer::playbackStateChanged(PlaybackState _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void OpenCV_VideoPlayer::inputResulationChanged(QSize _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void OpenCV_VideoPlayer::videoFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
