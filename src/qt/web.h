/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2013 by TJ O'Donnell and
C  Copyright (C) 2005-2013 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
#ifndef WEB_H
#define WEB_H

#include <QtNetwork>
#ifdef QT5
#include <QtWebKitWidgets>
#include <QtWidgets>
#else
#include <QtGui>
#include <QtWebKit>
#endif
#include <QMainWindow>
#include "gramps.h"
#include "gmol.h"

class WebWidget : public QWebView
{
    Q_OBJECT

public:
    WebWidget(QWidget *parent = 0, bool canDelete = false, bool allowAccess = false);
    ~WebWidget();
    Gramps * gramps;
    Gmol * gmol;
    bool canDelete;
    bool canReload;

public slots:
    void loadUrl(const QUrl & url);
    void insertJavaScript();
    void interceptLink(QNetworkReply *);
    void showLink(const QString & link, const QString &, const QString &);
    void pageFinished(QNetworkReply *);
    void download(const QNetworkRequest);
    void pageLoaded(bool);
    void deleteTab();
    void loadStart();
    void openLinkInNewTab();
    void openLink(QUrl);
    //void addMol(const int);

signals:
    void cmdReady(const QString);
    void msgReady(const QString);
    void molReady(const int);
    void readMol(const QString, const QString);

protected:

private:
    //QDockWidget * dock;
    QUrl urlLoading;
    void contextMenuEvent ( QContextMenuEvent * ev );
    bool handlePDB(QNetworkReply *reply, QString header, QString path);
    bool handleSDF(QNetworkReply *reply, QString header, QString path);
};

#endif
