/****************************************************************************
** Meta object code from reading C++ file 'cellview.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../isingModel/cellview.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cellview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_CellScene_t {
    uint offsetsAndSizes[2];
    char stringdata0[10];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CellScene_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CellScene_t qt_meta_stringdata_CellScene = {
    {
        QT_MOC_LITERAL(0, 9)   // "CellScene"
    },
    "CellScene"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CellScene[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

Q_CONSTINIT const QMetaObject CellScene::staticMetaObject = { {
    QMetaObject::SuperData::link<QGraphicsScene::staticMetaObject>(),
    qt_meta_stringdata_CellScene.offsetsAndSizes,
    qt_meta_data_CellScene,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CellScene_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<CellScene, std::true_type>
    >,
    nullptr
} };

void CellScene::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject *CellScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CellScene::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CellScene.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsScene::qt_metacast(_clname);
}

int CellScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_stringdata_CellView_t {
    uint offsetsAndSizes[2];
    char stringdata0[9];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CellView_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CellView_t qt_meta_stringdata_CellView = {
    {
        QT_MOC_LITERAL(0, 8)   // "CellView"
    },
    "CellView"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CellView[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

Q_CONSTINIT const QMetaObject CellView::staticMetaObject = { {
    QMetaObject::SuperData::link<QGraphicsView::staticMetaObject>(),
    qt_meta_stringdata_CellView.offsetsAndSizes,
    qt_meta_data_CellView,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CellView_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<CellView, std::true_type>
    >,
    nullptr
} };

void CellView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject *CellView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CellView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CellView.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsView::qt_metacast(_clname);
}

int CellView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_stringdata_CellViewSettingWidget_t {
    uint offsetsAndSizes[6];
    char stringdata0[22];
    char stringdata1[14];
    char stringdata2[1];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CellViewSettingWidget_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CellViewSettingWidget_t qt_meta_stringdata_CellViewSettingWidget = {
    {
        QT_MOC_LITERAL(0, 21),  // "CellViewSettingWidget"
        QT_MOC_LITERAL(22, 13),  // "saveRequested"
        QT_MOC_LITERAL(36, 0)   // ""
    },
    "CellViewSettingWidget",
    "saveRequested",
    ""
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CellViewSettingWidget[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   20,    2, 0x06,    1 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject CellViewSettingWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CellViewSettingWidget.offsetsAndSizes,
    qt_meta_data_CellViewSettingWidget,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CellViewSettingWidget_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<CellViewSettingWidget, std::true_type>,
        // method 'saveRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void CellViewSettingWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CellViewSettingWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->saveRequested(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CellViewSettingWidget::*)();
            if (_t _q_method = &CellViewSettingWidget::saveRequested; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
    (void)_a;
}

const QMetaObject *CellViewSettingWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CellViewSettingWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CellViewSettingWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CellViewSettingWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void CellViewSettingWidget::saveRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
namespace {
struct qt_meta_stringdata_ParameterSettingWidget_t {
    uint offsetsAndSizes[2];
    char stringdata0[23];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_ParameterSettingWidget_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_ParameterSettingWidget_t qt_meta_stringdata_ParameterSettingWidget = {
    {
        QT_MOC_LITERAL(0, 22)   // "ParameterSettingWidget"
    },
    "ParameterSettingWidget"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_ParameterSettingWidget[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

Q_CONSTINIT const QMetaObject ParameterSettingWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ParameterSettingWidget.offsetsAndSizes,
    qt_meta_data_ParameterSettingWidget,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_ParameterSettingWidget_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ParameterSettingWidget, std::true_type>
    >,
    nullptr
} };

void ParameterSettingWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject *ParameterSettingWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ParameterSettingWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ParameterSettingWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ParameterSettingWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_stringdata_CellItem_t {
    uint offsetsAndSizes[44];
    char stringdata0[9];
    char stringdata1[12];
    char stringdata2[1];
    char stringdata3[5];
    char stringdata4[13];
    char stringdata5[6];
    char stringdata6[12];
    char stringdata7[5];
    char stringdata8[12];
    char stringdata9[6];
    char stringdata10[12];
    char stringdata11[11];
    char stringdata12[9];
    char stringdata13[11];
    char stringdata14[12];
    char stringdata15[6];
    char stringdata16[10];
    char stringdata17[10];
    char stringdata18[6];
    char stringdata19[7];
    char stringdata20[11];
    char stringdata21[9];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CellItem_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CellItem_t qt_meta_stringdata_CellItem = {
    {
        QT_MOC_LITERAL(0, 8),  // "CellItem"
        QT_MOC_LITERAL(9, 11),  // "stepChanged"
        QT_MOC_LITERAL(21, 0),  // ""
        QT_MOC_LITERAL(22, 4),  // "step"
        QT_MOC_LITERAL(27, 12),  // "setCellWidth"
        QT_MOC_LITERAL(40, 5),  // "width"
        QT_MOC_LITERAL(46, 11),  // "setInterval"
        QT_MOC_LITERAL(58, 4),  // "msec"
        QT_MOC_LITERAL(63, 11),  // "setMaxCount"
        QT_MOC_LITERAL(75, 5),  // "count"
        QT_MOC_LITERAL(81, 11),  // "startUpdate"
        QT_MOC_LITERAL(93, 10),  // "stopUpdate"
        QT_MOC_LITERAL(104, 8),  // "initCell"
        QT_MOC_LITERAL(113, 10),  // "updateCell"
        QT_MOC_LITERAL(124, 11),  // "setParamKbT"
        QT_MOC_LITERAL(136, 5),  // "value"
        QT_MOC_LITERAL(142, 9),  // "setParamJ"
        QT_MOC_LITERAL(152, 9),  // "setMethod"
        QT_MOC_LITERAL(162, 5),  // "index"
        QT_MOC_LITERAL(168, 6),  // "Method"
        QT_MOC_LITERAL(175, 10),  // "Metropolis"
        QT_MOC_LITERAL(186, 8)   // "HeatBath"
    },
    "CellItem",
    "stepChanged",
    "",
    "step",
    "setCellWidth",
    "width",
    "setInterval",
    "msec",
    "setMaxCount",
    "count",
    "startUpdate",
    "stopUpdate",
    "initCell",
    "updateCell",
    "setParamKbT",
    "value",
    "setParamJ",
    "setMethod",
    "index",
    "Method",
    "Metropolis",
    "HeatBath"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CellItem[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       1,  105, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   80,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    1,   83,    2, 0x0a,    3 /* Public */,
       6,    1,   86,    2, 0x0a,    5 /* Public */,
       8,    1,   89,    2, 0x0a,    7 /* Public */,
      10,    0,   92,    2, 0x0a,    9 /* Public */,
      11,    0,   93,    2, 0x0a,   10 /* Public */,
      12,    0,   94,    2, 0x0a,   11 /* Public */,
      13,    0,   95,    2, 0x0a,   12 /* Public */,
      14,    1,   96,    2, 0x0a,   13 /* Public */,
      16,    1,   99,    2, 0x0a,   15 /* Public */,
      17,    1,  102,    2, 0x0a,   17 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   15,
    QMetaType::Void, QMetaType::Double,   15,
    QMetaType::Void, QMetaType::Int,   18,

 // enums: name, alias, flags, count, data
      19,   19, 0x2,    2,  110,

 // enum data: key, value
      20, uint(CellItem::Method::Metropolis),
      21, uint(CellItem::Method::HeatBath),

       0        // eod
};

Q_CONSTINIT const QMetaObject CellItem::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CellItem.offsetsAndSizes,
    qt_meta_data_CellItem,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CellItem_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<CellItem, std::true_type>,
        // method 'stepChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const int, std::false_type>,
        // method 'setCellWidth'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const int, std::false_type>,
        // method 'setInterval'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const int, std::false_type>,
        // method 'setMaxCount'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'startUpdate'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'stopUpdate'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'initCell'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateCell'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setParamKbT'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const double &, std::false_type>,
        // method 'setParamJ'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const double &, std::false_type>,
        // method 'setMethod'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const int, std::false_type>
    >,
    nullptr
} };

void CellItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CellItem *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->stepChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->setCellWidth((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->setInterval((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->setMaxCount((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->startUpdate(); break;
        case 5: _t->stopUpdate(); break;
        case 6: _t->initCell(); break;
        case 7: _t->updateCell(); break;
        case 8: _t->setParamKbT((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 9: _t->setParamJ((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 10: _t->setMethod((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CellItem::*)(const int );
            if (_t _q_method = &CellItem::stepChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *CellItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CellItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CellItem.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QGraphicsItem"))
        return static_cast< QGraphicsItem*>(this);
    return QObject::qt_metacast(_clname);
}

int CellItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void CellItem::stepChanged(const int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
