/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2013 by TJ O'Donnell and
C  Copyright (C) 2005-2013 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information.
C *************************************************************
*/
#ifndef GMOLAPP_H
#define GMOLAPP_H
#include <QApplication>
#include <QFileOpenEvent>
#ifdef QT5
#include <QtWidgets>
#endif
#include <QObject>
#include <QWidget>

class gmolApp : public QApplication
{
    Q_OBJECT

public:
    gmolApp( int & argc, char ** argv );
    ~gmolApp();

private:

protected:
    bool event(QEvent *event);

signals:
    void restore();
};
#endif // GMOLAPP_H
