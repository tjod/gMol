/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2013 by TJ O'Donnell and
C  Copyright (C) 2005-2013 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
#ifndef GDIAL_H
#define GDIAL_H

#include <QObject>
#include <QDial>
#include "gramps.h"

class gDial : public QDial
{
    Q_OBJECT

public:
    gDial(QWidget *parent = 0);
    ~gDial();
    int lastval;

private:

protected:
/*
    void focusOutEvent(QFocusEvent * e);
    void leaveEvent(QEvent * e);
*/

public slots:
/*
    void Zero();
*/

signals:
};
#endif
