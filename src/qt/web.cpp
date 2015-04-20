/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2013 by TJ O'Donnell and
C  Copyright (C) 2005-2013 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
#include "web.h"
#include "gramps.h"
#include "gmol.h"
#include "mainwindow.h"
#include <QObject>
#include <QDockWidget>

extern MainWindow * mainWindow; // need to access some widgets
WebWidget::WebWidget(QWidget *parent, bool canDelete, bool allowAccess)
    : QWebView(parent)
{
    gramps = new Gramps();
    gmol = new Gmol();
    this->canDelete = canDelete;
    canReload = true;  // until set otherwise for pdb and sdf loaded from url
    //dock = parent;
    connect(this,  SIGNAL(loadFinished(bool)),  this, SLOT(pageLoaded(bool)));
    page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    page()->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    QNetworkAccessManager *manager = page()->networkAccessManager();
    connect(manager, SIGNAL(finished(QNetworkReply *)),  this, SLOT(pageFinished(QNetworkReply *)));
    page()->setForwardUnsupportedContent(true);
    connect(page(),  SIGNAL(unsupportedContent(QNetworkReply *)),  this, SLOT(interceptLink(QNetworkReply *)));
    connect(page(),  SIGNAL(linkHovered(const QString &, const QString &, const QString &)),  this, SLOT(showLink(const QString &, const QString &, const QString &)));
    pageAction(QWebPage::OpenLinkInNewWindow)->setText("Open in new tab");
    connect(pageAction(QWebPage::OpenLinkInNewWindow),SIGNAL(triggered()),this,SLOT(openLinkInNewTab()));
    connect(page(),  SIGNAL(linkClicked(QUrl)),  this, SLOT(openLink(QUrl)));
    //connect(page(),  SIGNAL(loadStarted()),  this, SLOT(loadStart()));
    connect(page(),  SIGNAL(downloadRequested(const QNetworkRequest)),  this, SLOT(download(const QNetworkRequest)));
    if (allowAccess) connect(page()->mainFrame(),  SIGNAL(javaScriptWindowObjectCleared()),  this, SLOT(insertJavaScript()));
    //qDebug() << page()->supportedContentTypes();
}

WebWidget::~WebWidget()
{
  delete gramps;
  //gmol->disconnect();
  delete gmol;
}

void WebWidget::insertJavaScript() {
  //qDebug() << "frame loading..." << gramps;
  page()->mainFrame()->addToJavaScriptWindowObject("gramps", gramps);
  page()->mainFrame()->addToJavaScriptWindowObject("gmol", gmol);
  if (mainWindow->chemWidget != NULL) {
      connect(mainWindow->chemWidget, SIGNAL(molAdded(const int)), gmol, SLOT(addMol(const int)));
  }
}

void WebWidget::loadUrl(const QUrl & url) {
  emit msgReady(url.path()); // connected to mainWindow->statusBar
  //qDebug() << url.path() << url.toString();
  setHtml("Loading " + url.toString());
  //QWebView::load(url);
  load(url);
}
void WebWidget::download(const QNetworkRequest request) {
  qDebug() << "download request" << request.url();
}
void WebWidget::showLink(const QString & link, const QString & , const QString &) {
    emit msgReady(link);
}

void WebWidget::interceptLink(QNetworkReply * reply)
{
  QUrl url = reply->url();
  if (url.scheme() == "gramps") {
    emit cmdReady(url.path()); // connected to gramps->cmd
  } else {
      //QString spdb = QString(reply->readAll());
      //qDebug() << spdb;
//      QString path = (reply->request()).url().path();
//      if (path.endsWith(".pdb")) {
//          reply->request().setHeader(QNetworkRequest::ContentTypeHeader, "chemical/x-pdb");
//      } else if (path.endsWith(".sdf") || path.endsWith(".mol")) {
//          reply->request().setHeader(QNetworkRequest::ContentTypeHeader, "chemical/x-mdl-sdfile");
//      }
    //emit msgReady(reply->errorString()); // connected to mainWindow->statusBar
    //emit msgReady(reply->header(QNetworkRequest::ContentTypeHeader).toString()); // connected to mainWindow->statusBar
    //QString s100 = reply->read(100);
    //emit msgReady(s100); // connected to mainWindow->statusBar
    //qDebug() << "unsupported content header:" << reply->header(QNetworkRequest::ContentTypeHeader).toString();
    //this->reload();
    //qDebug() << s100;
  }
}

bool WebWidget::handlePDB(QNetworkReply *reply, QString header, QString path) {
    //qDebug() << "got PDB file from" << path;
    QString spdb = "";
    if (header.isEmpty() || header == "application/download") {
        spdb = QString(reply->readAll());
        //QString title =  QFileInfo(path).fileName();
        //setHtml("<header><title>"+title+"</title></header><pre>"+spdb.toHtmlEscaped());
    } else {
        spdb = page()->mainFrame()->toPlainText();
        //page()->history()->back();
    }
    if (reply->error()) {
        this->page()->mainFrame()->evaluateJavaScript("document.body.innerHTML += '" + reply->errorString() + "'");        
    } else {
        if (spdb.length() > 0) {
            int imol = Db::readPDB(spdb, QFileInfo(path).fileName());
            emit molReady(imol); // signals ChemWidget molReady
            canReload = false;  // reload causes new model of existing mol
            deleteTab();            
            return true;
        }
    }
    return false;
}
bool WebWidget::handleSDF(QNetworkReply *reply, QString header, QString path) {
    QString smol = "";
    if (header.isEmpty()) {
        smol = QString(reply->readAll());
        //QString title =  QFileInfo(path).fileName();
        //setHtml("<header><title>"+title+"</title></header><pre>"+smol);
    } else {
        smol = page()->mainFrame()->toPlainText();
        //page()->history()->back();
    }
    if (reply->error()) {
         this->page()->mainFrame()->evaluateJavaScript("document.body.innerHTML += '" + reply->errorString() + "'");
    } else {
        if (smol.length() > 0) {
            QString tmplate = QDir::tempPath() + "/" + QFileInfo(path).fileName();
            QTemporaryFile *tmpFile = new QTemporaryFile(tmplate);
            tmpFile->setAutoRemove(false); // needed outside this scope
            //qDebug() << "mol/sdf file from" << path << "to" << tmpFile->fileName();
            if ( tmpFile->open() )
            {
                QTextStream stream( tmpFile );
                stream << smol;
                tmpFile->close();
                emit readMol(tmpFile->fileName(), "sdf"); // calls ChemWidget::addMolToDb
                canReload = false;  // reload causes new instance of existing mol
                deleteTab();                
                return true;
            } else {
                qDebug() << "can't open file" << tmpFile->fileName();
                emit msgReady("can't open file " + tmpFile->fileName());
            }
        }
    }
    return false;
}
void WebWidget::pageFinished(QNetworkReply * reply) {
  QString header = reply->header(QNetworkRequest::ContentTypeHeader).toString();
  //emit msgReady(header); // connected to mainWindow->statusBar
  if (reply->error() > 0) emit msgReady(reply->errorString());
  QString path = (reply->request()).url().path();
  if(header == "application/download" && reply->hasRawHeader("Content-Disposition") && reply->rawHeaderList().contains("Content-Disposition")) {
       QString dispo = reply->rawHeader("Content-Disposition");
       path = dispo.section("filename=",1,1);
       qDebug() << "Download " << path;
  }
  //qDebug() << path << header;
  if (header == "chemical/x-pdb" || path.endsWith(".pdb"))
  {
      if (handlePDB(reply, header, path)) {
          //parentWidget()->lower();
          //parentWidget()->hide();
          //mainWindow->chemDock->raise();
      }
  }
  else if ( header == "chemical/x-mdl-sdfile"  || path.endsWith(".sdf") ||
            header == "chemical/x-mdl-molfile" || path.endsWith(".mol") )
  {
      if (handleSDF(reply, header, path)) {
          //parentWidget()->lower();
          //parentWidget()->hide();
          //mainWindow->chemDock->raise();
      }
  }
  reply->deleteLater();
}

void WebWidget::deleteTab() {
    this->deleteLater();
    parentWidget()->deleteLater();
}

void WebWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QWebHitTestResult r = page()->mainFrame()->hitTestContent(event->pos());
    QMenu menu(this);
    if (!r.linkUrl().isEmpty()) {
        QMenu menu(this);
        menu.addAction(pageAction(QWebPage::OpenLinkInNewWindow));
        pageAction(QWebPage::OpenLinkInNewWindow)->setData(r.linkUrl().toString());
        menu.addSeparator();
        menu.addAction(pageAction(QWebPage::DownloadLinkToDisk));
        // Add link to bookmarks...
        menu.addSeparator();
        menu.addAction(pageAction(QWebPage::CopyLinkToClipboard));
        //if (page()->settings()->testAttribute(QWebSettings::DeveloperExtrasEnabled))
        //    menu.addAction(pageAction(QWebPage::InspectElement));
        menu.exec(mapToGlobal(event->pos()));
        return;
    }
    if (this->canDelete) {
        // canDelete is false for pages gmol loads (help, etc.); true for those user loads
      menu.addAction(pageAction(QWebPage::Back));
      if (canReload) menu.addAction(pageAction(QWebPage::Reload));
      menu.addAction(pageAction(QWebPage::Copy));
      menu.addAction(tr("Delete Tab"), this, SLOT(deleteTab()));
      menu.addAction(pageAction(QWebPage::InspectElement));
      menu.exec(mapToGlobal(event->pos()));
    } else {
      QWebView::contextMenuEvent(event);
    }
}
void WebWidget::openLinkInNewTab() {
    QAction *act = static_cast<QAction *>(QObject::sender());
    //qDebug() << act->data();
    mainWindow->openUrl(act->data().toString(), true);
    triggerPageAction(QWebPage::Stop);
}
void WebWidget::openLink(QUrl url) {
    //qDebug() << url.toString();
    // could intercept pdb/sdf and other file links here?
    emit msgReady(url.path()); // connected to mainWindow->statusBar
    //setHtml("Loading " + url.toString());
    //qDebug() << "link clicked" << url.toString() << page() << QObject::sender();
    //QWebHitTestResult r = page()->mainFrame()->hitTestContent(this->pos());
    //qDebug() << r.linkTargetFrame() << r.linkUrl() << r.linkElement().toPlainText();
    if (url.path().endsWith(".pdb") || url.path().endsWith(".sdf") || url.path().endsWith(".mol")) {
        //QDockWidget *dock = mainWindow->openUrl(url.toString(), true);
        mainWindow->openUrl(url.toString(), true);
        triggerPageAction(QWebPage::Stop);
    } else {
       load(url);
    }
}
void WebWidget::pageLoaded(bool ok) {
    //WebWidget *view = static_cast<WebWidget *>(QObject::sender());
    //QDockWidget *dock = static_cast<QDockWidget *>(view->parentWidget());
    if (ok) {
      //dock->setWindowTitle(view->title().left(9));
      QString title = this->title().split(QRegExp("\\s+"))[0];
      if (title.length() == 0) {
        QString path = this->url().path();
        title = QFileInfo(path).fileName();
      }
      parentWidget()->setWindowTitle(title);
      //tabifyDockWidget(logDock, dock);
//      dock->show();
//      dock->raise();
    } else {
      //dock->setWindowTitle("unknown");
    }
}
void WebWidget::loadStart() {
    qDebug() << "load started" << page() <<  QObject::sender();
    //QWebHitTestResult r = page()->mainFrame()->hitTestContent(this->pos());
    //qDebug() << r.linkTargetFrame() << r.linkUrl() << r.linkElement().toPlainText();
}
