#QMAKE_CXXFLAGS     += -DOBCHEM -DDEBUG
QMAKE_CXXFLAGS     += -DOBCHEM -DDEBUG -DPDBREADER
QT += sql \
		opengl \
		network
isEqual(QT_MAJOR_VERSION, 5): {
cache()
QMAKE_CXXFLAGS += -DQT5
QT +=  webkitwidgets
} else {
QT += webkit
}
unix:!macx {
 CONFIG += debug
 INCLUDEPATH  += /usr/include/openbabel-2.0 ../
 #INCLUDEPATH  += /Users/tj/Documents/openbabel/include/openbabel-2.0 ../
}
macx {
 INCLUDEPATH  += /Users/tj/Documents/openbabel/include/openbabel-2.0 ../
 #INCLUDEPATH  += /usr/local/include/openbabel-2.0 ../
}
win32 {
CONFIG += console
 INCLUDEPATH  += /c/openbabel-2.3.2/build/install_dir/include/openbabel-2.0 ../
}
macx {
 QMAKE_CXXFLAGS     += -D__APPLE__
 CONFIG -= app_bundle
 QMAKE_MAC_SDK = macosx10.9
}
HEADERS       = dbchem.h \
                ../db.h \
                window.h
SOURCES       = main.cpp \
                window.cpp \
                ../db.cpp \
                dbchem.cpp
macx {
 LIBS         += ${HOME}/Documents/openbabel/lib/libopenbabel.a /opt/local/lib/libz.a
 #LIBS         += -L${HOME}/Documents/openbabel/lib -l openbabel /opt/local/lib/libz.a
}
unix:!macx {
 LIBS         += -L/usr/local/lib/ -lopenbabel
 #LIBS         += -L${HOME}/Documents/openbabel/lib/ -lopenbabel
}
win32 {
 LIBS         += -L"'c:/openbabel-2.3.2/build/install_dir/bin'" -lopenbabel
}
# install
target.path = dbchem
sources.files = dbchem.pro
sources.path = dbchem
INSTALLS += target sources
