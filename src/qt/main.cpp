/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2013 by TJ O'Donnell and
C  Copyright (C) 2005-2013 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
#include <QApplication>
#include "mainwindow.h"
#include "atom_properties.h"
#include "gmolApp.h"

MainWindow * mainWindow; // others need this and its widgets
QString gmolLib;
QString grampsLib;

int main(int argc, char *argv[])
{
    gmolApp *app = new gmolApp(argc, argv);
    app->setOrganizationName("gNova");
    app->setApplicationName("gMol");
#ifdef __APPLE__
    QDir appdir = QCoreApplication::applicationDirPath();
    appdir.cdUp();
    QString reldir = appdir.absolutePath();
    gmolLib   = reldir + "/Resources";
    grampsLib = reldir + "/Resources/gramps";
    setenv("gramps_lib", grampsLib.toLocal8Bit().data(), 1);
    qDebug() << QCoreApplication::applicationDirPath();
#else
    gmolLib   = getenv("gmol_lib");
    grampsLib = getenv("gramps_lib");
#endif
    mainWindow = new MainWindow;
    mainWindow->show();
    mainWindow->activateWindow();
    mainWindow->raise(); // wasn't always being raised on osx
    mainWindow->setWindowTitle("gMol v1.8");
    if (argc > 0) {
        mainWindow->openFile(argv[1]);
    }
    QObject::connect(app, SIGNAL(aboutToQuit()), mainWindow, SLOT(save()));
    return app->exec();
}
