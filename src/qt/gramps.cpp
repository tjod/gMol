/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2013 by TJ O'Donnell and
C  Copyright (C) 2005-2013 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
#include "mainwindow.h"
#include <QtOpenGL>
#ifdef QT5
#include <QtWebKitWidgets>
#else
#include <QtWebKit/QtWebKit>
#endif
#include <QDebug>
#include <QStringList>
#include <GL/glu.h>

extern MainWindow * mainWindow; // need to access some widgets from these global extern "C" functions.
static QStringList qbuffer; // filled by qwrite_ (called by gramps to output), cleared and returned by this->cmd

/* this object is exposed to JavaScript for all pages loaded into webView */
Gramps::Gramps(QObject *parent)
    : QObject(parent)
{
}

Gramps::~Gramps()
{
}

QString Gramps::cmd(const QString str) {
   qbuffer.clear();
   /* connected to mainWindow->doCmd, which call do_, which calls qwrite_ (see below)
      which adds gramps command output to qbuffer */
   emit cmdReady(str, true);
   return qbuffer.join("\n");
}

static int chosen_cursor = 0;
static int currx;
static int curry;

grampsPick Gramps::pickProcess(const QPoint &p, Qt::MouseButton b) {
/* Adjust curry: gl 0,0 is lower left, qt 0,0 is upper left.
   Store current mouse press location in global currx, curry.
   so that call to setpickprocess via g0pickprocess and display will know these values
*/  grampsPick gp;
    if (b == Qt::LeftButton || b == Qt::RightButton) {
        chosen_cursor = 2;
    }
    if (chosen_cursor) {
        gp = fromPick(p);
        chosen_cursor = 0;
    }
    return gp;

}
grampsPick Gramps::doubleClickProcess(const QPoint &p, Qt::MouseButton b) {
    grampsPick gp;
    if (b != Qt::RightButton) {
        chosen_cursor = 2;
        gp =  fromPick(p);
        chosen_cursor = 0;
    }
    return gp;
}
grampsPick Gramps::fromPick(QPoint p) {
    grampsPick gp;
    gp.objid = 0;
    int height = mainWindow->glWidget->height();
    currx = p.x();
    curry = height - p.y();
    gp.objid = g0pickprocess(currx, curry, gp.xyzw);
    if (gp.objid > 0) {
        char name[41];
        int len = getname_(&(gp.objid), name, 40);
        name[len] = '\0';
        gp.name = QString(name);
    } else {
        gp.name = "";
    }
    return gp;
}

//static
int Gramps::idFromName(const QString name) {
    const char *gname;
    QByteArray b = name.toLocal8Bit();
    gname = b.constData();
    return idname_(gname, name.size());
}

/* interface between fortran gramps functions and variables
   and qtgramps widgets and objects */

void setkeys_(int * /*k*/) {
    return;
}

void setpickmatrix_() {
// only if in pick mode (set by pick_ calling setpickcursor_)
  //if (chosen_cursor == PICK_CURSOR) {
  if (chosen_cursor) {
   GLint viewport[4];
   glGetIntegerv(GL_VIEWPORT,viewport);
   //fprintf(stderr,"pick matrix %d %d\n", currx, curry);
   gluPickMatrix(currx,curry,PICKWIDTH,PICKWIDTH,viewport);
  }
}
void setpickcursor_() {
  chosen_cursor = 1;
  mainWindow->glWidget->setCursor(Qt::PointingHandCursor);
}

void setwinpos_(int *x, int *w, int *y, int *h) {

  if(x||y){}; // get rid of warnings about unused args
// how to resize only glWidget and have mainWindow adjust?
  QSize delta = QSize(*w, *h) - mainWindow->glWidget->size() + mainWindow->size();
  mainWindow->resize(delta);
}

void qwrite_(char * line, int llen) {
  QString qstr = QString::fromLocal8Bit(line,llen);
  mainWindow->cmdOutput->appendPlainText(qstr);
  if (qstr.startsWith("?")) {
    mainWindow->statusBar()->showMessage(qstr);
  }
  qbuffer += qstr;

}

void updategl_() {
  mainWindow->glWidget->updateGL();
}

void qtext(double x, double y, double z, char *string, int slen) {

 QString astring = QString::fromLocal8Bit(string,slen);
// qDebug() << x << y << z << astring;
 mainWindow->glWidget->renderText(x,y,z,astring);
}

void settimer_(int state) {
  if (state == 0) {
    mainWindow->glWidget->setTimer(false);
  } else {
    mainWindow->glWidget->setTimer(true);
  }
}

int setdevval_(int *dev, float *rval, int *state) {
  //qDebug() << mainWindow;
  if (mainWindow == NULL) return 0;
  if (*state == 0) {
    mainWindow->controlWidget->setValue(*dev, *rval, false);
  } else {
    mainWindow->controlWidget->setValue(*dev, *rval, true);
  }
  return 1;
}

void snapscreen(char *filename, int width, int height, const uchar *rgbbuff) {
  QImage screen = QImage(rgbbuff, width, height, width*3, QImage::Format_RGB888);
  screen.mirrored().save(filename,0,100);
}
