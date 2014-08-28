/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Wed Jul 23 16:37:59 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      25,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,   12,   11,   11, 0x05,
      57,   12,   11,   11, 0x05,
      97,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     107,   11,   11,   11, 0x0a,
     130,  127,   11,   11, 0x0a,
     169,  127,   11,   11, 0x0a,
     215,   11,   11,   11, 0x0a,
     233,   11,   11,   11, 0x08,
     248,   11,   11,   11, 0x08,
     260,   11,  255,   11, 0x08,
     275,   11,  267,   11, 0x08,
     293,  284,  267,   11, 0x08,
     309,   11,   11,   11, 0x08,
     319,   11,   11,   11, 0x08,
     326,   11,   11,   11, 0x08,
     334,   11,   11,   11, 0x08,
     344,   11,   11,   11, 0x08,
     355,   11,   11,   11, 0x08,
     363,   11,   11,   11, 0x08,
     379,  372,   11,   11, 0x08,
     396,   11,   11,   11, 0x08,
     409,   11,  267,   11, 0x08,
     426,  424,  267,   11, 0x08,
     446,   11,   11,   11, 0x08,
     461,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0p,name,xyzw\0"
    "pickReady(QPoint,QString,float*)\0"
    "doubleClickReady(QPoint,QString,float*)\0"
    "restore()\0showStatus(QString)\0,,\0"
    "gotPick(QPoint,QPoint,Qt::MouseButton)\0"
    "gotDoubleClick(QPoint,QPoint,Qt::MouseButton)\0"
    "openFile(QString)\0fatal(QString)\0"
    "open()\0bool\0save()\0QString\0saveas()\0"
    "fileName\0saveas(QString)\0openUrl()\0"
    "snap()\0reset()\0restart()\0loadHelp()\0"
    "close()\0select()\0action\0doHelp(QAction*)\0"
    "processCmd()\0doCmd(QString)\0,\0"
    "doCmd(QString,bool)\0showdock(bool)\0"
    "advanceDone()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->pickReady((*reinterpret_cast< const QPoint(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< float*(*)>(_a[3]))); break;
        case 1: _t->doubleClickReady((*reinterpret_cast< const QPoint(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< float*(*)>(_a[3]))); break;
        case 2: _t->restore(); break;
        case 3: _t->showStatus((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->gotPick((*reinterpret_cast< const QPoint(*)>(_a[1])),(*reinterpret_cast< const QPoint(*)>(_a[2])),(*reinterpret_cast< const Qt::MouseButton(*)>(_a[3]))); break;
        case 5: _t->gotDoubleClick((*reinterpret_cast< const QPoint(*)>(_a[1])),(*reinterpret_cast< const QPoint(*)>(_a[2])),(*reinterpret_cast< const Qt::MouseButton(*)>(_a[3]))); break;
        case 6: _t->openFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->fatal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->open(); break;
        case 9: { bool _r = _t->save();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 10: { QString _r = _t->saveas();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 11: { QString _r = _t->saveas((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 12: _t->openUrl(); break;
        case 13: _t->snap(); break;
        case 14: _t->reset(); break;
        case 15: _t->restart(); break;
        case 16: _t->loadHelp(); break;
        case 17: _t->close(); break;
        case 18: _t->select(); break;
        case 19: _t->doHelp((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 20: _t->processCmd(); break;
        case 21: { QString _r = _t->doCmd((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 22: { QString _r = _t->doCmd((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 23: _t->showdock((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 24: _t->advanceDone(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::pickReady(const QPoint & _t1, QString _t2, float * _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::doubleClickReady(const QPoint & _t1, QString _t2, float * _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MainWindow::restore()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
