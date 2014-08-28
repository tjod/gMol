/*
I
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2013 by TJ O'Donnell and
C  Copyright (C) 2005-2013 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information.
C *************************************************************
*/
#include "gmolApp.h"
#include "db.h"
#include "mainwindow.h"
#include <QFileOpenEvent>
#include <QDebug>
gmolApp::gmolApp(int &argc, char **argv) :
    QApplication(argc, argv)
{
}
gmolApp::~gmolApp() {
}
extern MainWindow *mainWindow;
bool gmolApp::event(QEvent *event)
{
    if (event->type() == QEvent::FileOpen) {
        QFileOpenEvent *fileOpenEvent = static_cast<QFileOpenEvent*>(event);
        mainWindow->openFile(fileOpenEvent->file());
        return true;
    }
    return QApplication::event(event);
}
