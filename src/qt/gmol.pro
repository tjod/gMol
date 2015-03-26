#QMAKE_CXXFLAGS += -DCHEM -DDEBUG
QMAKE_CXXFLAGS += -DCHEM  -DPDBREADER
QMAKE_CFLAGS_DEBUG += -DMEMWRITE -DDEBUG
QMAKE_CFLAGS_RELEASE += -DMEMWRITE
QMAKE_INFO_PLIST = Info.plist
QT += opengl \
    network \
    sql
isEqual(QT_MAJOR_VERSION, 5): {
   cache()
   QMAKE_CXXFLAGS += -DQT5
   QT += uitools \
    widgets \
    webkitwidgets
} else {
   QT += webkit
}
CONFIG += glu
    #debug
HEADERS = glwidget.h \
    controls.h \
    web.h \
    gdial.h \
    chem.h \
    db.h \
    atom_properties.h \
    gramps.h \
    gmol.h \
    cpk.xpm \
    mainwindow.h \
    gmolApp.h \
    atom_values.h
SOURCES = glwidget.cpp \
    controls.cpp \
    web.cpp \
    gramps.cpp \
    gmol.cpp \
    gdial.cpp \
    chem.cpp \
    chem_cartoon.cpp \
    chem_surface.cpp \
    chem_chain.cpp \
    ./ChemDb/dbchem.cpp \
    spline.cpp \
    mainwindow.cpp \
    main.cpp \
    gmolApp.cpp \
    db.cpp
FORMS += controls.ui
unix:!macx: {
    LIBS += ./qtgramps.so \
         -lGLU
}
win32 { 
    LIBS += -L./ \
        -lqtgramps \
        $$QMAKE_LIBS_OPENGL \
        -lgfortran
    
    # OBJECTS      +=  qtgramps.a
    CONFIG += console
    CONFIG -= debug
}
macx {
   INCLUDEPATH += /opt/local/include
   LIBS += ../gramps/qtgramps.a \
    -L/usr/local/gfortran/lib \
    -lgfortran
   ICON = gmol.icns
   QMAKE_CXXFLAGS += -D__APPLE__
   QMAKE_MAC_SDK = macosx10.9
}

# install
target.path = gmol
sources.files = gmol.pro
sources.path = gmol
INSTALLS += target \
    sources

OTHER_FILES += \
    Info.plist
