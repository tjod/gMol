/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2013 by TJ O'Donnell and
C  Copyright (C) 2005-2013 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
#ifndef GRAMPS_H
#define GRAMPS_H

#include <QObject>
#include <QStringList>
#include <QMouseEvent>

struct grampsPick {
  int objid;
  Qt::MouseButton button;
  QString name;
  float xyzw[4];
};

class Gramps : public QObject
{
    Q_OBJECT

public:
    Gramps(QObject *parent = 0);
    ~Gramps();
    static int idFromName(const QString);
    grampsPick pickProcess       (const QPoint & p, Qt::MouseButton b);
    grampsPick doubleClickProcess(const QPoint & p, Qt::MouseButton b);

public slots:
    QString cmd(const QString);

signals:
    void cmdReady(const QString, bool);
    void msgReady(const QString);

protected:

private:
    grampsPick fromPick(QPoint);
};

/*
These extern functions allow communication to and from
gramps C and Fortran functions.
*/

// qtgramps calls these
extern "C" void udinit_();
extern "C" void g0pinit_();
extern "C" void startup_();
extern "C" int  g0redraw_(int *force);
extern "C" void g0wset_(int *x0, int *y0, int *xsize, int *ysize, int *tellws);
extern "C" int  do_(char *cmd, int cmdlen);
extern "C" int  domore_();
extern "C" void  settimer_(int state);
extern "C" int  contupd_(int *do_it);
extern "C" void advsnap_();
extern "C" int update_(int *type, int *ndial, float *xval, float *xinc);
extern "C" int setdevval_(int *dev, float *rval, int *state);
extern "C" int g0hires_(int *flag);
extern "C" int g0refill_(int *ojbid);
//extern "C" int g0nrml_(float [3], float[3], float[3], float[3]);
extern "C" int getname_(int *ojbid, char *name, int len);
extern "C" int idname_(const char *name, int len);
//extern "C" int pickProcess(int currx, int curry, Qt::MouseButton b);
extern "C" void setkeys_(int *);

/* to send pix "files" */
extern "C" int getmem_(const char *name, float *data, int *size, int namlen);
extern "C" int getmemmore_(const char *name, float *data, int *size, int namlen);
extern "C" int getmemdone_(const char *name, float *data, int *size, int namlen);

// from g0csubs
extern "C" int g0pickprocess(int currx, int curry, float *xyzw);
extern "C" void qtext(double x, double y, double z, char *string, int slen);
extern "C" void snapscreen(char * filename, int width, int height, const uchar* rgbbuff);

// for get *.html
//extern "C" int geturl_(char *url, int ulen);

#define DIALS  1
#define DIAL1_BIT 0
#define MOUSS  256
#define MOUS1_BIT  8
#define SLIDES 65536
#define SLIDE1_BIT 16
#define SPINS  16777216
#define SPIN1_BIT 24

#define PICKWIDTH 5

// gramps call these
extern "C" void updategl_();
extern "C" void setwinpos_(int *x, int *w, int *y, int *h);
extern "C" void qwrite_(char * line, int llen);
extern "C" void setpickmatrix_();
extern "C" void setpickcursor_();
extern "C" int cd_(char *dir, int clem);
#endif
