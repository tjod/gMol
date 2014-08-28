##QMAKE_CXXFLAGS     += -DOBCHEM -DDEBUG
QMAKE_CXXFLAGS     += -DDEBUG -DPDBREADER
INCLUDEPATH  += ../ ../ChemDb
QT += sql \
		opengl \
		network
isEqual(QT_MAJOR_VERSION, 5): {
cache()
QT +=	webkitwidgets
} else {
QT +=	webkit
}
win32 {
 CONFIG += console
}
HEADERS       = ../db.h \
		../ChemDb/dbchem.h \
                ../atom_properties.h
SOURCES       = dbsurf.cpp \
                ../db.cpp \
                ../ChemDb/dbchem.cpp
unix {
LIBS         += -L/usr/local/gfortran/lib/ -lgfortran
 fortran.commands = /usr/bin/gfortran -c ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}
}
win32 {
 fortran.commands = gfortran -c ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}
 LIBS         += -lgfortran
}
macx {
 fortran.commands = /usr/local/bin/gfortran -c ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}
 CONFIG -= app_bundle
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
