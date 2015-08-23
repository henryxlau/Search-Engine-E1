/****************************************************************************
** Meta object code from reading C++ file 'SearchEngine.h'
**
** Created: Wed May 7 10:35:15 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "SearchEngine.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SearchEngine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SearchEngine[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      28,   13,   13,   13, 0x08,
      41,   36,   13,   13, 0x08,
      70,   13,   13,   13, 0x08,
      90,   36,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SearchEngine[] = {
    "SearchEngine\0\0enterSearch()\0close()\0"
    "item\0pageSelect(QListWidgetItem*)\0"
    "searchResultOrder()\0showAd(QListWidgetItem*)\0"
};

void SearchEngine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SearchEngine *_t = static_cast<SearchEngine *>(_o);
        switch (_id) {
        case 0: _t->enterSearch(); break;
        case 1: _t->close(); break;
        case 2: _t->pageSelect((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 3: _t->searchResultOrder(); break;
        case 4: _t->showAd((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SearchEngine::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SearchEngine::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SearchEngine,
      qt_meta_data_SearchEngine, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SearchEngine::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SearchEngine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SearchEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SearchEngine))
        return static_cast<void*>(const_cast< SearchEngine*>(this));
    return QWidget::qt_metacast(_clname);
}

int SearchEngine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
