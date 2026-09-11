QMAKE_CXXFLAGS     += -DOBCHEM -DPDBREADER -DDEBUG

INCLUDEPATH  += ../ ../ChemDb
QT += sql \
      opengl \
      network
isEqual(QT_MAJOR_VERSION, 5): {
  cache()
  QMAKE_CXXFLAGS += -DQT5
  QT +=	webkitwidgets
} else {
  QT +=	webkit
}
win32 {
 CONFIG += console
}
HEADERS = \
  ../db.h \
  ../ChemDb/dbchem.h \
  ../atom_properties.h \
  ../atom_values.h
SOURCES  = dbsurf.cpp \
   ../db.cpp \
   ../ChemDb/dbchem.cpp
unix:!macx {
    LIBS         += -L/usr/local/gfortran/lib/ -lgfortran
    fortran.commands = /usr/bin/gfortran -c ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}
}
win32 {
 fortran.commands = gfortran -c ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}
 LIBS         += -lgfortran
}
macx {
  QMAKE_MACOSX_DEPLOYMENT_TARGET = 12.0
  fortran.commands = /opt/local/bin/gfortran -c ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}
  QMAKE_CXXFLAGS     += -std=c++11
  CONFIG -= app_bundle
  INCLUDEPATH  += /usr/local/include/openbabel3/ ../
  QMAKE_MAC_SDK = macosx
  QMAKE_LFLAGS += -Wl,-rpath,@executable_path/../Frameworks
  LIBS         += -L/opt/local/lib/gcc15 -lgfortran \
                  -L/usr/local/lib/      -lopenbabel
}
FORTRAN_SOURCES += isolib.F
fortran.output = ${QMAKE_FILE_BASE}.o
fortran.input = FORTRAN_SOURCES
QMAKE_EXTRA_COMPILERS += fortran
# install
target.path = dbsurf
sources.files = dbsurf.pro
sources.path = dbsurf
INSTALLS += target sources

OTHER_FILES += \
    dbsurf \
    isosrf.F \
    isolib.F
