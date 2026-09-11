#CC = /opt/local/bin/gcc
#FC = /opt/local/bin/gfortran

QMAKE_CXXFLAGS += -DCHEM  -DPDBREADER
QMAKE_CXXFLAGS_DEBUG += -DDEBUG
#QMAKE_CXXFLAGS_RELEASE
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
#   # Query the system for the active macOS SDK path
#       QMAKE_MAC_SDK = macosx

#       # Ensure qmake passes the SDK path to the compiler
#       QMAKE_CFLAGS += -isysroot $$QMAKE_MAC_SDK_PATH
#       QMAKE_CXXFLAGS += -isysroot $$QMAKE_MAC_SDK_PATH
#       QMAKE_OBJECTIVE_CFLAGS += -isysroot $$QMAKE_MAC_SDK_PATH
#       QMAKE_LFLAGS += -isysroot $$QMAKE_MAC_SDK_PATH
   QMAKE_LFLAGS += -Wl,-rpath,@executable_path/../Frameworks
   INCLUDEPATH += /opt/local/include
   #INCLUDEPATH += /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1
   LIBS += ../gramps/qtgramps.a \
    -L/opt/local/lib/gcc15 -lgfortran \
    -framework GLUT -framework OPENGL
   ICON = gmol.icns
   QMAKE_CXXFLAGS += -D__APPLE__
   #QMAKE_MAC_SDK = macosx13.1
   QMAKE_MACOSX_DEPLOYMENT_TARGET = 12.7
   QMAKE_CXXFLAGS += -Wno-gnu-inline-cpp-without-extern
}

#

# install
target.path = gmol
sources.files = gmol.pro
sources.path = gmol
INSTALLS += target \
    sources

OTHER_FILES += \
    Info.plist
    gmol.icns

RESOURCES +=

DISTFILES += \
    gmol.qss
