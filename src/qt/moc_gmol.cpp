/****************************************************************************
** Meta object code from reading C++ file 'gmol.h'
**
** Created: Wed Jul 9 11:46:07 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "gmol.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gmol.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Gmol[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
       6,    5,    5,    5, 0x05,

 // slots: signature, parameters, type, tag, flags
      43,    5,   20,    5, 0x0a,
      67,    5,   51,    5, 0x0a,
      81,    5,   77,    5, 0x0a,
     111,    5,   77,    5, 0x0a,
     135,    5,    5,    5, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Gmol[] = {
    "Gmol\0\0molAdded(int)\0QMap<QString,QVariant>\0"
    "atest()\0QList<QVariant>\0getTree()\0int\0"
    "getMolIdFromInchiKey(QString)\0"
    "readMolFromUrl(QString)\0addMol(int)\0"
};

void Gmol::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Gmol *_t = static_cast<Gmol *>(_o);
        switch (_id) {
        case 0: _t->molAdded((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 1: { QMap<QString,QVariant> _r = _t->atest();
            if (_a[0]) *reinterpret_cast< QMap<QString,QVariant>*>(_a[0]) = _r; }  break;
        case 2: { QList<QVariant> _r = _t->getTree();
            if (_a[0]) *reinterpret_cast< QList<QVariant>*>(_a[0]) = _r; }  break;
        case 3: { int _r = _t->getMolIdFromInchiKey((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 4: { int _r = _t->readMolFromUrl((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 5: _t->addMol((*reinterpret_cast< const int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Gmol::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Gmol::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Gmol,
      qt_meta_data_Gmol, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Gmol::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Gmol::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Gmol::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Gmol))
        return static_cast<void*>(const_cast< Gmol*>(this));
    return QObject::qt_metacast(_clname);
}

int Gmol::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void Gmol::molAdded(const int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
