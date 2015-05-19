/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2013 by TJ O'Donnell and
C  Copyright (C) 2005-2013 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QtGui>
#include "glwidget.h"
#include "controls.h"
#include "gramps.h"
#include "web.h"
#include "chem.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

/* gramps.cpp needs these */
    GLWidget * glWidget;
    QPlainTextEdit * cmdOutput;
    QLineEdit * cmdInput;
    Controls * controlWidget;

/* web.cpp needs this for gmol javascript object */
    ChemWidget * chemWidget;

/* gmol.cpp uses this to get pdb files from the web */
    QDockWidget *openUrl(QString url, bool canDelete);
signals:
    void pickReady(const QPoint &p, QString name, float *xyzw);
    void doubleClickReady(const QPoint &p, QString name, float *xyzw);
    void restore();
    //void textWebPage(const QString);

public slots:
    void showStatus(const QString);
    void gotPick       (const QPoint &, const QPoint &, const Qt::MouseButton);
    void gotDoubleClick(const QPoint &, const QPoint &, const Qt::MouseButton);
    void openFile(QString);
    // not really a slot
    bool grampsSave(QString);

private slots:
    void fatal(const QString);
    void open();
    bool save();
    QString saveas();
    QString saveas(QString fileName);
    void openUrl();
    void showTextWebPage(const QString, const QString );

    void snap();
    void reset();
    void restart();
    void loadHelp();
    void close();
    void select();
    void doHelp(QAction *action);
    void processCmd();
    QString doCmd(const QString);
    QString doCmd(const QString, bool);
    void showdock(bool);
    void advanceDone();
    void toggleStatus(bool);
    //void visibilityChanged(QWindow::Visibility);
    //void dockVisible(bool);

private:
    QMenu *fileMenu;
    QMenu *helpMenu;
    QMenu *tabMenu;
    //QMenu *demoMenu;
    QAction *openAct;
    QAction *openUrlAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *snapAct;
    QAction *resetAct;
    QAction *restartAct;
    QAction *exitAct;
    //void demoIntro();
    void loadFile(const QString &fileName);

    void createDockWindows();
    void createActions();
    void createMenus();
    void webViewConnections(WebWidget*, bool allowAccess);

    QString saveFile;

    QDockWidget * chemDock;
    QWidget * cmdWidget;
    QDockWidget * logDock;
    QDockWidget * helpDock;
    WebWidget * helpView;
    WebWidget * demoView;
    QDockWidget * demoDock;
    QDockWidget * controlDock;
    QDockWidget *grampsDock;
    WebWidget *grampsView;
    QDockWidget *surfDock;
    WebWidget *surfView;

    void setTimer(bool state);
    QTimer * continuousUpdater;
    void addMenuItem(QDockWidget *dock, QString tip);
    QString showDock(QDockWidget *);
    //void resizeEvent(QResizeEvent *);
};

#endif
