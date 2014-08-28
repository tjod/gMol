/****************************************************************************
** Meta object code from reading C++ file 'web.h'
**
** Created: Wed Jul 23 16:37:57 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "web.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'web.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WebWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,
      29,   10,   10,   10, 0x05,
      47,   10,   10,   10, 0x05,
      63,   61,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      92,   88,   10,   10, 0x0a,
     106,   10,   10,   10, 0x0a,
     125,   10,   10,   10, 0x0a,
     162,  155,   10,   10, 0x0a,
     196,   10,   10,   10, 0x0a,
     225,   10,   10,   10, 0x0a,
     251,   10,   10,   10, 0x0a,
     268,   10,   10,   10, 0x0a,
     280,   10,   10,   10, 0x0a,
     292,   10,   10,   10, 0x0a,
     311,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_WebWidget[] = {
    "WebWidget\0\0cmdReady(QString)\0"
    "msgReady(QString)\0molReady(int)\0,\0"
    "readMol(QString,QString)\0url\0loadUrl(QUrl)\0"
    "insertJavaScript()\0interceptLink(QNetworkReply*)\0"
    "link,,\0showLink(QString,QString,QString)\0"
    "pageFinished(QNetworkReply*)\0"
    "download(QNetworkRequest)\0pageLoaded(bool)\0"
    "deleteTab()\0loadStart()\0openLinkInNewTab()\0"
    "openLink(QUrl)\0"
};

void WebWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        WebWidget *_t = static_cast<WebWidget *>(_o);
        switch (_id) {
        case 0: _t->cmdReady((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->msgReady((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->molReady((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 3: _t->readMol((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->loadUrl((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 5: _t->insertJavaScript(); break;
        case 6: _t->interceptLink((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 7: _t->showLink((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 8: _t->pageFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 9: _t->download((*reinterpret_cast< const QNetworkRequest(*)>(_a[1]))); break;
        case 10: _t->pageLoaded((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->deleteTab(); break;
        case 12: _t->loadStart(); break;
        case 13: _t->openLinkInNewTab(); break;
        case 14: _t->openLink((*reinterpret_cast< QUrl(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData WebWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject WebWidget::staticMetaObject = {
    { &QWebView::staticMetaObject, qt_meta_stringdata_WebWidget,
      qt_meta_data_WebWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &WebWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *WebWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *WebWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WebWidget))
        return static_cast<void*>(const_cast< WebWidget*>(this));
    return QWebView::qt_metacast(_clname);
}

int WebWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWebView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void WebWidget::cmdReady(const QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void WebWidget::msgReady(const QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void WebWidget::molReady(const int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void WebWidget::readMol(const QString _t1, const QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
