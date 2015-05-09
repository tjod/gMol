TEMPLATE += app
CONFIG += console
QT += sql core widgets opengl
isEqual(QT_MAJOR_VERSION, 5): {
  cache()
  QMAKE_CXXFLAGS += -DQT5
  QT +=	webkitwidgets
} else {
  QT +=	webkit
}
macx {
  CONFIG -= app_bundle
  QMAKE_MAC_SDK = macosx10.9
}
INCLUDEPATH  += ../ ../ChemDb
HEADERS += \
    types.h \
    ../db.h \
    ../ChemDb/dbchem.h \
    ../atom_properties.h \
    ../atom_values.h

SOURCES += main.cpp \
   ../db.cpp \
   ../ChemDb/dbchem.cpp

@#include(deployment.pri)
#qtcAddDeployment()
