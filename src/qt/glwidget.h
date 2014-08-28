/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2013 by TJ O'Donnell and
C  Copyright (C) 2005-2013 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QObject>
#include <QGLWidget>

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

public slots:
    int setDevice(int type, int devnum, double val);
    void applyDevice(int type, int devnum, double val);
    void doAdvance();
    void doAdvance(bool);
    void setTimer(bool);

signals:
    void device(int type, int devnum, double val);
    void loopAdvance(bool);
    void mousePressed(const QPoint &,const QPoint &,Qt::MouseButton);
    void doubleClick(const QPoint &,const QPoint &,Qt::MouseButton);
    void advanceDone();
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    QPoint lastPos;
    QTimer *continuousUpdater;

};

#endif
