/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2013 by TJ O'Donnell and
C  Copyright (C) 2005-2013 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
#ifdef QT5
#include <QtWidgets>
#endif

#include "gdial.h"

gDial::gDial(QWidget *parent) : QDial(parent)
{
    lastval = 0;
    //connect(this, SIGNAL(sliderReleased()), this, SLOT(Zero()));
}

gDial::~gDial()
{
}

/*
void gDial::Zero()
{
    setValue(0);
}

void  gDial::focusOutEvent(QFocusEvent * e) {
    setValue(0);
    update();
}

void  gDial::leaveEvent(QEvent * e) {
    setValue(0);
    clearFocus();
}
*/
