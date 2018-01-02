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
#include <QtOpenGL>
#include <QDebug>

#include <math.h>

#include "glwidget.h"
#include "gramps.h"
#include "controls.h"

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    continuousUpdater = new QTimer;
    connect(continuousUpdater, SIGNAL(timeout()), this, SLOT(doAdvance()));
    connect(this, SIGNAL(device(int,int,double)), this, SLOT(applyDevice(int,int,double)));
    connect(this, SIGNAL(loopAdvance(bool)),      this, SLOT(doAdvance(bool)), Qt::DirectConnection);
}

GLWidget::~GLWidget()
{
    delete continuousUpdater;
    makeCurrent();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(300, 300);
}

QSize GLWidget::sizeHint() const
{
    return QSize(750, 750);
}

void GLWidget::initializeGL()
{
    udinit_();
    g0pinit_();
    startup_();
}

void GLWidget::paintGL()
{
    int force = 1;
    g0redraw_(&force);
}

void GLWidget::resizeGL(int width, int height)
{
    int zero = 0;
    g0wset_(&zero, &zero, &width, &height, &zero);
    glViewport(0,0, width, height);
    //qDebug() << width << height;
    //qDebug() << windowState() << topLevelWidget()->isFullScreen() << isFullScreen();
}

void GLWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    // connected to mainwindow doubleClickProcess
    emit doubleClick(event->globalPos(), event->pos(), event->button());
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    int update_display = 0;
    int flag = 0;
    g0hires_(&flag);
#ifdef QT5
    QPoint delta = event->pixelDelta();
    if (delta == QPoint(0,0)) delta = event->angleDelta();    
    double dx = delta.x() / 2.0;
    double dy = delta.y() / 2.0;
    if (dx) update_display += setDevice(MOUSS,   2,  dx); //x2
    if (dy) update_display += setDevice(MOUSS, 4+2,  dy); //y2
    //    QPoint delta = event->angleDelta();
    //    double dx = delta.x();
    //    double dy = delta.y();
    //    if (dx) update_display += setDevice(MOUSS,   2,  dx); //x2
    //    if (dy) update_display += setDevice(MOUSS, 4+2,  dy); //y2
    if (update_display) {
        emit device(0,0,0);
    }
#else
    double delta = event->delta();
    if (delta) {
        if (event->orientation() == Qt::Vertical) {
            //qDebug() << "delta" << delta;
            update_display += setDevice(MOUSS, 4+2,  delta); //y2
        } else if (event->orientation() == Qt::Horizontal) {
            update_display += setDevice(MOUSS,   2,  delta); //x2
        }
        if (update_display) {
            emit device(0,0,0);
        }
    }
#endif
//    QGLWidget::wheelEvent(event);
    event->accept();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    int flag = 0;
    g0hires_(&flag);
    lastPos = event->pos();
    emit mousePressed(event->globalPos(), event->pos(), event->button()); // connected to mainwindow pickProcess
    QGLWidget::mousePressEvent(event);
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    int flag = 1;
    g0hires_(&flag);
    emit device(0,0,0);
    event->type();  // just to avoid compiler warning about non-use of event
    QGLWidget::mouseReleaseEvent(event);  //Dont forget to pass on the event to parent
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int update_display = 0;
    double dx = (event->x() - lastPos.x());
    double dy = (event->y() - lastPos.y());

    if (event->buttons() & Qt::LeftButton) {
      if (dx) update_display += setDevice(MOUSS,   1,  dx); // x1
      if (dy) update_display += setDevice(MOUSS, 4+1, -dy); // y1

    } else if (event->buttons() & Qt::RightButton) {
      if (dx) update_display += setDevice(MOUSS,   3,  dx); // x3
      if (dy) update_display += setDevice(MOUSS, 4+3, -dy); // y3
    }
    lastPos = event->pos();
    if (update_display) {
      emit device(0,0,0);
    }
    QGLWidget::mouseMoveEvent(event);
}

int GLWidget::setDevice(int type, int devnum, double val) {

  float fval;
 
  if (type == MOUSS) {
    float mdiv;
#ifdef __APPLE__
    if (Qt::MetaModifier == QApplication::keyboardModifiers())
#else
    if (Qt::ControlModifier == QApplication::keyboardModifiers())
#endif
    {
      mdiv = 10000; // finer control
    } else {
      mdiv = 1000;
    }
    fval = val / mdiv;
  } else {
    fval = val;
  }
  return update_(&type, &devnum, &fval, &fval);

}

void GLWidget::applyDevice(int type, int devnum, double val) {

  if (type == 0 || setDevice(type, devnum, val)) {
      int flag = 1;
      g0redraw_(&flag);
      updateGL();
  }

}

void GLWidget::doAdvance() {
  doAdvance(false);
}
void GLWidget::doAdvance(bool advwait) {
   int flag = 0;
   int i=contupd_(&flag);
   //qDebug() << "advance" << i;
   flag = 1;
   g0redraw_(&flag);
   updateGL();
   advsnap_(); // output snap image if necessary
   if (i == 0) {
     setTimer(false);
     emit advanceDone();
   } else {
     //if (i < 0) setTimer(true);
     setTimer(true);
   }
 /* be sure adv finished before returning.
    this is needed to ensure subsequent
    commands, say to setup next animation sequence
    are not prematurely carried out,  e.g.
    in javascript calls to gramps.cmd()
 */
   if (advwait && i > 0) {
     //this->disconnect(SIGNAL(device(int,int,double)));
     emit loopAdvance(advwait);
   }
/*
   while (i > 0) {
     doAdvance();
   }
*/
}

void GLWidget::setTimer(bool state) {
  if ( !continuousUpdater) {
    qWarning() <<  "no timer available!";
    return;
  }
  if (state) {
    if (!continuousUpdater->isActive()) {
      //fprintf(stderr, "start timer\n");
      continuousUpdater->start();
    }
  } else {
    if (continuousUpdater->isActive()) {
      //fprintf(stderr, "stop timer\n");
      continuousUpdater->stop();
    }
  }
}
