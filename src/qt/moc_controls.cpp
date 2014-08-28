/****************************************************************************
** Meta object code from reading C++ file 'controls.h'
**
** Created: Wed Jul 9 11:46:01 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "controls.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'controls.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Controls[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      27,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      26,   10,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      49,    9,    9,    9, 0x0a,
      72,   68,    9,    9, 0x0a,
      83,   68,    9,    9, 0x0a,
      94,   68,    9,    9, 0x0a,
     105,   68,    9,    9, 0x0a,
     116,   68,    9,    9, 0x0a,
     127,   68,    9,    9, 0x0a,
     138,   68,    9,    9, 0x0a,
     149,   68,    9,    9, 0x0a,
     160,   68,    9,    9, 0x0a,
     172,   68,    9,    9, 0x0a,
     184,   68,    9,    9, 0x0a,
     196,   68,    9,    9, 0x0a,
     208,   68,    9,    9, 0x0a,
     220,   68,    9,    9, 0x0a,
     232,   68,    9,    9, 0x0a,
     244,   68,    9,    9, 0x0a,
     256,   68,    9,    9, 0x0a,
     270,   68,    9,    9, 0x0a,
     284,   68,    9,    9, 0x0a,
     298,   68,    9,    9, 0x0a,
     312,   68,    9,    9, 0x0a,
     326,   68,    9,    9, 0x0a,
     340,   68,    9,    9, 0x0a,
     354,   68,    9,    9, 0x0a,
     383,  368,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Controls[] = {
    "Controls\0\0type,devnum,val\0"
    "device(int,int,double)\0setupConnections()\0"
    "val\0dial1(int)\0dial2(int)\0dial3(int)\0"
    "dial4(int)\0dial5(int)\0dial6(int)\0"
    "dial7(int)\0dial8(int)\0slide1(int)\0"
    "slide2(int)\0slide3(int)\0slide4(int)\0"
    "slide5(int)\0slide6(int)\0slide7(int)\0"
    "slide8(int)\0spin1(double)\0spin2(double)\0"
    "spin3(double)\0spin4(double)\0spin5(double)\0"
    "spin6(double)\0spin7(double)\0spin8(double)\0"
    "dev,rval,state\0setValue(int,float,bool)\0"
};

void Controls::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Controls *_t = static_cast<Controls *>(_o);
        switch (_id) {
        case 0: _t->device((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 1: _t->setupConnections(); break;
        case 2: _t->dial1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->dial2((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->dial3((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->dial4((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->dial5((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->dial6((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->dial7((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->dial8((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->slide1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->slide2((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->slide3((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->slide4((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->slide5((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->slide6((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->slide7((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->slide8((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->spin1((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 19: _t->spin2((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 20: _t->spin3((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 21: _t->spin4((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 22: _t->spin5((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 23: _t->spin6((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 24: _t->spin7((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 25: _t->spin8((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 26: _t->setValue((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Controls::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Controls::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Controls,
      qt_meta_data_Controls, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Controls::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Controls::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Controls::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Controls))
        return static_cast<void*>(const_cast< Controls*>(this));
    return QWidget::qt_metacast(_clname);
}

int Controls::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 27)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 27;
    }
    return _id;
}

// SIGNAL 0
void Controls::device(int _t1, int _t2, double _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
