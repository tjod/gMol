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
#include "controls.h"
#include "gdial.h"
#include "ui_controls.h"
#include "gramps.h"

Controls::Controls(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);
}

Controls::~Controls()
{
}

void Controls::setupConnections()
{
    connect(dial_1, SIGNAL(valueChanged(int)), this, SLOT(dial1(int)));
    connect(dial_2, SIGNAL(valueChanged(int)), this, SLOT(dial2(int)));
    connect(dial_3, SIGNAL(valueChanged(int)), this, SLOT(dial3(int)));
    connect(dial_4, SIGNAL(valueChanged(int)), this, SLOT(dial4(int)));
    connect(dial_5, SIGNAL(valueChanged(int)), this, SLOT(dial5(int)));
    connect(dial_6, SIGNAL(valueChanged(int)), this, SLOT(dial6(int)));
    connect(dial_7, SIGNAL(valueChanged(int)), this, SLOT(dial7(int)));
    connect(dial_8, SIGNAL(valueChanged(int)), this, SLOT(dial8(int)));
    connect(verticalSlider_1, SIGNAL(valueChanged(int)), this, SLOT(slide1(int)));
    connect(verticalSlider_2, SIGNAL(valueChanged(int)), this, SLOT(slide2(int)));
    connect(verticalSlider_3, SIGNAL(valueChanged(int)), this, SLOT(slide3(int)));
    connect(verticalSlider_4, SIGNAL(valueChanged(int)), this, SLOT(slide4(int)));
    connect(horizontalSlider_5, SIGNAL(valueChanged(int)), this, SLOT(slide5(int)));
    connect(horizontalSlider_6, SIGNAL(valueChanged(int)), this, SLOT(slide6(int)));
    connect(horizontalSlider_7, SIGNAL(valueChanged(int)), this, SLOT(slide7(int)));
    connect(horizontalSlider_8, SIGNAL(valueChanged(int)), this, SLOT(slide8(int)));
    connect(horizontalSlider_5, SIGNAL(valueChanged(int)), this, SLOT(slide5(int)));
    connect(doubleSpinBox_1, SIGNAL(valueChanged(double)), this, SLOT(spin1(double)));
    connect(doubleSpinBox_2, SIGNAL(valueChanged(double)), this, SLOT(spin2(double)));
    connect(doubleSpinBox_3, SIGNAL(valueChanged(double)), this, SLOT(spin3(double)));
    connect(doubleSpinBox_4, SIGNAL(valueChanged(double)), this, SLOT(spin4(double)));
    connect(doubleSpinBox_5, SIGNAL(valueChanged(double)), this, SLOT(spin5(double)));
    connect(doubleSpinBox_6, SIGNAL(valueChanged(double)), this, SLOT(spin6(double)));
    connect(doubleSpinBox_7, SIGNAL(valueChanged(double)), this, SLOT(spin7(double)));
    connect(doubleSpinBox_8, SIGNAL(valueChanged(double)), this, SLOT(spin8(double)));
}

QSize Controls::minimumSizeHint() const
{
    return QSize(300, 460);
}

QSize Controls::sizeHint() const
{
    return QSize(300, 460);
}

/*
void Controls::gdial(int ndial, gDial *dial, int val) {
    double rval = val / 100.
    emit device(DIALS,ndial,rval);
}
void Controls::dial1(int val)    { gdial(1,dial_1,val); }
void Controls::dial2(int val)    { gdial(2,dial_2,val); }
void Controls::dial3(int val)    { gdial(3,dial_3,val); }
void Controls::dial4(int val)    { gdial(4,dial_4,val); }
*/
void Controls::dial1(int val)    { emit device(DIALS, 1,(double)val/100.); }
void Controls::dial2(int val)    { emit device(DIALS, 2,(double)val/100.); }
void Controls::dial3(int val)    { emit device(DIALS, 3,(double)val/100.); }
void Controls::dial4(int val)    { emit device(DIALS, 4,(double)val/100.); }
void Controls::dial5(int val)    { emit device(DIALS, 5,(double)val/100.); }
void Controls::dial6(int val)    { emit device(DIALS, 6,(double)val/100.); }
void Controls::dial7(int val)    { emit device(DIALS, 7,(double)val/100.); }
void Controls::dial8(int val)    { emit device(DIALS, 8,(double)val/100.); }
void Controls::slide1(int val)   { emit device(SLIDES,1,(double)val/100.); }
void Controls::slide2(int val)   { emit device(SLIDES,2,(double)val/100.); }
void Controls::slide3(int val)   { emit device(SLIDES,3,(double)val/100.); }
void Controls::slide4(int val)   { emit device(SLIDES,4,(double)val/100.); }
void Controls::slide5(int val)   { emit device(SLIDES,5,(double)val/100.); }
void Controls::slide6(int val)   { emit device(SLIDES,6,(double)val/100.); }
void Controls::slide7(int val)   { emit device(SLIDES,7,(double)val/100.); }
void Controls::slide8(int val)   { emit device(SLIDES,8,(double)val/100.); }
void Controls::spin1(double val) { emit device(SPINS,1,val); }
void Controls::spin2(double val) { emit device(SPINS,2,val); }
void Controls::spin3(double val) { emit device(SPINS,3,val); }
void Controls::spin4(double val) { emit device(SPINS,4,val); }
void Controls::spin5(double val) { emit device(SPINS,5,val); }
void Controls::spin6(double val) { emit device(SPINS,6,val); }
void Controls::spin7(double val) { emit device(SPINS,7,val); }
void Controls::spin8(double val) { emit device(SPINS,8,val); }

void Controls::setValue(int dev, float rval, bool state) {
  int nbit = 0;
  for (int i=0; i<32; ++i) {
    if (1<<i == dev) {
      nbit = i;
      break;
    }
  }

  if (nbit >= SPIN1_BIT) {
    //fprintf(stderr, "SPIN %d", nbit - SPIN1_BIT + 1);
    switch (nbit - SPIN1_BIT + 1) {
    case 1: enable(doubleSpinBox_1, label_v1, state); doubleSpinBox_1->setValue(rval); break;
    case 2: enable(doubleSpinBox_2, label_v2, state); doubleSpinBox_2->setValue(rval); break;
    case 3: enable(doubleSpinBox_3, label_v3, state); doubleSpinBox_3->setValue(rval); break;
    case 4: enable(doubleSpinBox_4, label_v4, state); doubleSpinBox_4->setValue(rval); break;
    case 5: enable(doubleSpinBox_5, label_v5, state); doubleSpinBox_5->setValue(rval); break;
    case 6: enable(doubleSpinBox_6, label_v6, state); doubleSpinBox_6->setValue(rval); break;
    case 7: enable(doubleSpinBox_7, label_v7, state); doubleSpinBox_7->setValue(rval); break;
    case 8: enable(doubleSpinBox_8, label_v8, state); doubleSpinBox_8->setValue(rval); break;
    default: break;
    }
  } else if (nbit >= SLIDE1_BIT) {
    //fprintf(stderr, "SLIDE %d", nbit - SLIDE1_BIT + 1);
    switch (nbit - SLIDE1_BIT + 1) {
    case 1: enable(verticalSlider_1,   label_s1, state); verticalSlider_1->setValue(int(rval*100)); break;
    case 2: enable(verticalSlider_2,   label_s2, state); verticalSlider_2->setValue(int(rval*100)); break;
    case 3: enable(verticalSlider_3,   label_s3, state); verticalSlider_3->setValue(int(rval*100)); break;
    case 4: enable(verticalSlider_4,   label_s4, state); verticalSlider_4->setValue(int(rval*100)); break;
    case 5: enable(horizontalSlider_5, label_s5, state); horizontalSlider_5->setValue(int(rval*100)); break;
    case 6: enable(horizontalSlider_6, label_s6, state); horizontalSlider_6->setValue(int(rval*100)); break;
    case 7: enable(horizontalSlider_7, label_s7, state); horizontalSlider_7->setValue(int(rval*100)); break;
    case 8: enable(horizontalSlider_8, label_s8, state); horizontalSlider_8->setValue(int(rval*100)); break;
    default: break;
    }
  } else if (nbit >= MOUS1_BIT) {
    //fprintf(stderr, "MOUSE %d", nbit - MOUS1_BIT + 1);
  } else if (nbit >= DIAL1_BIT) {
    //fprintf(stderr, "DIAL %d", nbit - DIAL1_BIT + 1);
    switch (nbit - DIAL1_BIT + 1) {
    case 1: enable(dial_1, label_d1, state); dial_1->setValue(int(rval*100)); break;
    case 2: enable(dial_2, label_d2, state); dial_2->setValue(int(rval*100)); break;
    case 3: enable(dial_3, label_d3, state); dial_3->setValue(int(rval*100)); break;
    case 4: enable(dial_4, label_d4, state); dial_4->setValue(int(rval*100)); break;
    case 5: enable(dial_5, label_d5, state); dial_5->setValue(int(rval*100)); break;
    case 6: enable(dial_6, label_d6, state); dial_6->setValue(int(rval*100)); break;
    case 7: enable(dial_7, label_d7, state); dial_7->setValue(int(rval*100)); break;
    case 8: enable(dial_8, label_d8, state); dial_8->setValue(int(rval*100)); break;
    default: break;
    }
  }
}

void Controls::enable(QWidget *dev, QWidget *label, bool state) {
  dev->setEnabled(state);
  label->setEnabled(state);
  label->setAutoFillBackground(state);
}
