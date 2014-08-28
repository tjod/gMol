/****************************************************************************
** Meta object code from reading C++ file 'chem.h'
**
** Created: Wed Jul 9 11:46:05 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "chem.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ChemWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      33,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      30,   11,   11,   11, 0x05,
      48,   11,   11,   11, 0x05,
      62,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      79,   77,   11,   11, 0x0a,
     109,   77,   11,   11, 0x0a,
     134,   77,   11,   11, 0x08,
     170,   77,   11,   11, 0x08,
     200,   11,   11,   11, 0x08,
     224,   11,   11,   11, 0x08,
     244,   11,   11,   11, 0x08,
     263,   11,   11,   11, 0x08,
     276,   11,   11,   11, 0x08,
     290,   11,   11,   11, 0x08,
     305,   11,   11,   11, 0x08,
     316,   11,   11,   11, 0x08,
     329,   11,   11,   11, 0x08,
     339,   11,   11,   11, 0x08,
     349,   11,   11,   11, 0x08,
     361,   11,   11,   11, 0x08,
     373,   11,   11,   11, 0x08,
     383,   11,   11,   11, 0x08,
     402,   11,   11,   11, 0x08,
     421,   11,   11,   11, 0x08,
     439,   11,   11,   11, 0x08,
     456,   11,   11,   11, 0x08,
     475,   11,   11,   11, 0x08,
     485,   11,   11,   11, 0x08,
     500,   11,   11,   11, 0x08,
     512,   11,   11,   11, 0x08,
     526,   11,   11,   11, 0x08,
     541,   11,   11,   11, 0x08,
     552,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ChemWidget[] = {
    "ChemWidget\0\0cmdReady(QString)\0"
    "msgReady(QString)\0molAdded(int)\0"
    "fatal(QString)\0,\0clicked(QTreeWidgetItem*,int)\0"
    "readMol(QString,QString)\0"
    "doubleClicked(QTreeWidgetItem*,int)\0"
    "changed(QTreeWidgetItem*,int)\0"
    "showContextMenu(QPoint)\0showHydrogens(bool)\0"
    "showMainSide(bool)\0addSurfRow()\0"
    "makeSurface()\0addFilterRow()\0styleMol()\0"
    "addResidue()\0addAtom()\0hideMol()\0"
    "ignoreMol()\0deleteMol()\0infoMol()\0"
    "colorMolByRandom()\0colorMolByChoose()\0"
    "colorMolBySolid()\0colorMolByAtom()\0"
    "colorMolByCharge()\0zoomMol()\0"
    "highlightMol()\0centerMol()\0molReady(int)\0"
    "surfReady(int)\0readProc()\0restore()\0"
};

void ChemWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ChemWidget *_t = static_cast<ChemWidget *>(_o);
        switch (_id) {
        case 0: _t->cmdReady((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->msgReady((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->molAdded((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 3: _t->fatal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->clicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->readMol((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 6: _t->doubleClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->changed((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->showContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 9: _t->showHydrogens((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->showMainSide((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->addSurfRow(); break;
        case 12: _t->makeSurface(); break;
        case 13: _t->addFilterRow(); break;
        case 14: _t->styleMol(); break;
        case 15: _t->addResidue(); break;
        case 16: _t->addAtom(); break;
        case 17: _t->hideMol(); break;
        case 18: _t->ignoreMol(); break;
        case 19: _t->deleteMol(); break;
        case 20: _t->infoMol(); break;
        case 21: _t->colorMolByRandom(); break;
        case 22: _t->colorMolByChoose(); break;
        case 23: _t->colorMolBySolid(); break;
        case 24: _t->colorMolByAtom(); break;
        case 25: _t->colorMolByCharge(); break;
        case 26: _t->zoomMol(); break;
        case 27: _t->highlightMol(); break;
        case 28: _t->centerMol(); break;
        case 29: _t->molReady((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 30: _t->surfReady((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 31: _t->readProc(); break;
        case 32: _t->restore(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ChemWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ChemWidget::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_ChemWidget,
      qt_meta_data_ChemWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ChemWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ChemWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ChemWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ChemWidget))
        return static_cast<void*>(const_cast< ChemWidget*>(this));
    return QTreeWidget::qt_metacast(_clname);
}

int ChemWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 33)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 33;
    }
    return _id;
}

// SIGNAL 0
void ChemWidget::cmdReady(const QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ChemWidget::msgReady(const QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ChemWidget::molAdded(const int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ChemWidget::fatal(const QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
