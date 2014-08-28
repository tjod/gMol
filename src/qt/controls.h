/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2013 by TJ O'Donnell and
C  Copyright (C) 2005-2013 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include "ui_controls.h"

#define NDIALS 8
class Controls : public QWidget, private Ui::Controls
{
    Q_OBJECT

public:
    Controls(QWidget *parent = 0);
    ~Controls();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

public slots:
    void setupConnections();
    void dial1(int val);
    void dial2(int val);
    void dial3(int val);
    void dial4(int val);
    void dial5(int val);
    void dial6(int val);
    void dial7(int val);
    void dial8(int val);
    void slide1(int val);
    void slide2(int val);
    void slide3(int val);
    void slide4(int val);
    void slide5(int val);
    void slide6(int val);
    void slide7(int val);
    void slide8(int val);
    void spin1(double val);
    void spin2(double val);
    void spin3(double val);
    void spin4(double val);
    void spin5(double val);
    void spin6(double val);
    void spin7(double val);
    void spin8(double val);
    void setValue(int dev, float rval, bool state);

signals:
    void device(int type, int devnum, double val);

protected:

private:
    void gdial(int ndial, gDial * dial, int val);
    void enable(QWidget *dev, QWidget *label, bool state);
};

#endif
