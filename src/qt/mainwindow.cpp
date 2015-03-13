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
#include "mainwindow.h"
#include "gramps.h"
#include "controls.h"
#include "web.h"
#ifdef CHEM
#include "chem.h"
#endif

extern MainWindow * mainWindow;
extern QString gmolLib;
extern QString grampsLib;
MainWindow::MainWindow()
{
    saveFile = "";

    glWidget = new GLWidget;
    glWidget->resize(500,500);
    setCentralWidget(glWidget);
/*
    connect(this, SIGNAL(visibilityChanged(QWindow::Visibility)),
            this,   SLOT(visibilityChanged(QWindow::Visibility)));
*/
    createActions();
    createMenus();
    createDockWindows();

    connect(this, SIGNAL(restore()), chemWidget, SLOT(restore()));
    connect(cmdInput,  SIGNAL(returnPressed()),   this,  SLOT(processCmd()));
    connect(cmdOutput, SIGNAL(selectionChanged()), this, SLOT(select()));
    connect(controlWidget, SIGNAL(device(int,int,double)), glWidget, SLOT(applyDevice(int,int,double)));
#ifdef CHEM
    connect(chemWidget,  SIGNAL(cmdReady(const QString)),  this, SLOT(doCmd(const QString)));
    connect(chemWidget,  SIGNAL(msgReady(const QString)),  this, SLOT(showStatus(const QString)));
    connect(chemWidget,  SIGNAL(fatal(const QString)),     this, SLOT(fatal(const QString)));
    connect(chemWidget,  SIGNAL(textWebPage(const QString, const QString)),
                     this, SLOT(showTextWebPage(const QString, const QString)));
    tabifyDockWidget(logDock, chemDock);
#endif
    logDock->hide();
    tabifyDockWidget(logDock, controlDock);
    controlDock->hide();

    //QDockWidget *grampsDock = openUrl("file:///"+gmolLib+"/js/gramps/index.html", false);
    grampsDock = openUrl("file:///"+gmolLib+"/extjs/gmol/index-dbg.html", false);
    grampsView = (WebWidget*)grampsDock->widget();
    connect(glWidget, SIGNAL(mousePressed(const QPoint &,const QPoint &,Qt::MouseButton)),
            this,     SLOT(       gotPick(const QPoint &,const QPoint &,Qt::MouseButton)));
    connect(glWidget,  SIGNAL(doubleClick(const QPoint &,const QPoint &,Qt::MouseButton)),
            this,     SLOT(gotDoubleClick(const QPoint &,const QPoint &,Qt::MouseButton)));
    connect(glWidget, SIGNAL(advanceDone()), this, SLOT(advanceDone()));
    grampsDock->hide();

    helpDock = openUrl("file:///" + gmolLib + "/index.html", false);
    helpView = (WebWidget*)helpDock->widget();
    // don't let the title change as page changes
    helpView->disconnect(SIGNAL(loadFinished(bool))); // was connected in webViewConnections
    helpDock->setWindowTitle(tr("Help"));
    //connect(helpDock, SIGNAL(visibilityChanged(bool)), this, SLOT(dockVisible(bool)));

    QFile *startPage = new QFile(QDir::homePath() + "/gmol.html");
    if (startPage->exists()) openUrl(startPage->fileName(), false);

     // don't allow drop of files to overwrite these
    grampsView->setAcceptDrops(false);
    helpView->setAcceptDrops(false);
}

void MainWindow::gotPick(const QPoint &globalP, const QPoint &p, Qt::MouseButton b) {
    grampsPick gp = grampsView->gramps->pickProcess(p, b);
    gp.button = b;
    if (gp.objid > 0) {
        glWidget->setCursor(Qt::ArrowCursor);
        if (b == Qt::RightButton) { // mac two finger click gesture
            chemWidget->gotPick(globalP, gp);
        } else if (b == Qt::LeftButton) { // mac one finger click
            chemWidget->showPick(globalP, gp);
        }
    }
}
void MainWindow::gotDoubleClick(const QPoint &globalP, const QPoint &p, Qt::MouseButton b) {
    grampsPick gp = grampsView->gramps->doubleClickProcess(p, b);
    gp.button = b;
    chemWidget->gotDoubleClick(globalP, gp);
}

QString MainWindow::showDock(QDockWidget *dock) {
    QString title = "";
    dock->show();
    dock->raise();
    if (dock == logDock) {
        cmdInput->setFocus();
    } else if (dock  == helpDock) {
        WebWidget *view = (WebWidget*)dock->widget();
        title = view->title();
    } else if (dock  == controlDock) {
    } else if (dock  == chemDock) {
    } else {
        WebWidget *view = (WebWidget*)dock->widget();
        title = view->title();
    }
    return title;
}

void MainWindow::showdock(bool b) {
    QAction *act = static_cast<QAction *>(QObject::sender());
    //qDebug() << act->text() << act->parentWidget() << act->associatedWidgets();
    QDockWidget *dock = static_cast<QDockWidget *>(act->parentWidget());
    QString title = dock->windowTitle();
    if (b) {
        title = showDock(dock);
    } else {
//        if (dock->isFloating()) {
//            dock->setFloating(false);
//            //dock->showMinimized();
//        }
    }
    //showStatus(title);
    //qDebug() << "showdock" << title << b;
}

void MainWindow::addMenuItem(QDockWidget *dock, QString tip) {
    QAction *act = dock->toggleViewAction();
    tabMenu->addAction(act);
    act->setToolTip(tip);
    connect(act, SIGNAL(toggled(bool)), this, SLOT(showdock(bool)));
}

void MainWindow::createDockWindows() {
    logDock = new QDockWidget("Log", this);
    logDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

//  command is composite widget for output and input
    cmdWidget = new QWidget(logDock);
    cmdOutput = new QPlainTextEdit(cmdWidget);
    cmdInput  = new QLineEdit(cmdWidget);
    QVBoxLayout * cmdLayout = new QVBoxLayout;
    cmdLayout->addWidget(cmdOutput);
    cmdLayout->addWidget(cmdInput);
    cmdLayout->setContentsMargins(0,0,0,0);
    cmdLayout->setSpacing(0);
    cmdOutput->setReadOnly(true);
    cmdOutput->setFocusProxy(cmdInput);
    cmdWidget->setLayout(cmdLayout);
    cmdOutput->setFont( QFont("Courier", 12) );
    cmdOutput->setMinimumWidth(450);

    logDock->setWidget(cmdWidget);
    addDockWidget(Qt::LeftDockWidgetArea, logDock);
    addMenuItem(logDock, tr("Toggle command log"));
    //connect(logDock, SIGNAL(visibilityChanged(bool)), this, SLOT(dockVisible(bool)));

// control widget
    controlDock = new QDockWidget(tr("Control"), this);
    controlWidget = new Controls(controlDock);
    controlWidget->setupConnections();
    controlDock->setWidget(controlWidget);
    addDockWidget(Qt::LeftDockWidgetArea, controlDock);
    addMenuItem(controlDock, tr("Toggle control devices"));
    //connect(controlDock, SIGNAL(visibilityChanged(bool)), this, SLOT(dockVisible(bool)));

// status bar
    QAction *act = tabMenu->addAction(tr("Status")); //, this, SLOT(toggleStatus(bool)));
    act->setCheckable(true);
    act->setChecked(false);
    statusBar()->hide();
    connect(act, SIGNAL(toggled(bool)), this, SLOT(toggleStatus(bool)));

// subsumed into help; see doHelp(QAction)
    demoView = NULL;

    chemDock = NULL;
    chemWidget = NULL;
#ifdef CHEM
    Db::open();
    chemDock = new QDockWidget(tr("gMol"), this);
    chemWidget = new ChemWidget(chemDock);
    chemDock->setWidget(chemWidget);
    addDockWidget(Qt::LeftDockWidgetArea, chemDock);
    addMenuItem(chemDock, tr("Toggle gMol tree control"));
    chemDock->setMinimumWidth(450);
    //connect(chemDock, SIGNAL(visibilityChanged(bool)), this, SLOT(dockVisible(bool)));
#endif
}
//void MainWindow::dockVisible(bool visible) {
//    QDockWidget *dock = static_cast<QDockWidget *>(QObject::sender());
//    //qDebug() << "dock visible: " << visible << " floating:" << dock->isFloating();
//    if (dock->isFloating() && !visible) {
//        dock->setFloating(false);
//        //dock->widget()->hide();
//    }
//}

void MainWindow::createActions()
{
    openAct = new QAction(tr("Open..."), this);
    //openAct->setShortcut(tr("Ctrl+O"));
    openAct->setStatusTip(tr("Open and display a file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    openUrlAct = new QAction(tr("Open URL..."), this);
    //openUrlAct->setShortcut(tr("Ctrl+U"));
    openUrlAct->setStatusTip(tr("Open and display a URL"));
    connect(openUrlAct, SIGNAL(triggered()), this, SLOT(openUrl()));

    saveAct = new QAction(tr("Save"), this);
    //saveAct->setShortcut(tr("Ctrl+S"));
    saveAct->setStatusTip(tr("Save session file"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save As"), this);
    //saveAsAct->setShortcut(tr("Ctrl+S"));
    saveAsAct->setStatusTip(tr("Save session file"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveas()));

    snapAct = new QAction(tr("Snap"), this);
    //snapAct->setShortcut(tr("Ctrl+P"));
    snapAct->setStatusTip(tr("Snap graphics window and save to disk"));
    connect(snapAct, SIGNAL(triggered()), this, SLOT(snap()));

    resetAct = new QAction(tr("Reset"), this);
    //resetAct->setShortcut(tr("Ctrl+E"));
    resetAct->setStatusTip(tr("Reset view"));
    connect(resetAct, SIGNAL(triggered()), this, SLOT(reset()));

    restartAct = new QAction(tr("Restart"), this);
    //restartAct->setShortcut(tr("Ctrl+R"));
    restartAct->setStatusTip(tr("Restart"));
    connect(restartAct, SIGNAL(triggered()), this, SLOT(restart()));

    exitAct = new QAction(tr("Exit"), this);
    exitAct->setStatusTip(tr("Exit the application"));
    //exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(openUrlAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(snapAct);
    fileMenu->addAction(resetAct);
    fileMenu->addAction(restartAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    tabMenu = menuBar()->addMenu(tr("View"));

    helpMenu = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction("gMol")->setStatusTip(tr("Welcome to gramps"));
    helpMenu->addAction("Gramps")->setStatusTip(tr("Gramps command documentation"));
    helpMenu->addAction("Demos")->setStatusTip(tr("Introduction to the demos"));
    connect(helpMenu, SIGNAL(triggered(QAction *)), this, SLOT(doHelp(QAction *)));
}

void MainWindow::loadHelp() {
    helpView->loadUrl(QUrl("file:///" + gmolLib + "/index.html"));
}

void MainWindow::doHelp(QAction *action)
{
  if (action->text() == "gMol") {
    loadHelp();
    showDock(helpDock);
  } else if (action->text() == "Gramps") {
    helpView->loadUrl(QUrl("file:///" + grampsLib + "/html/index.html"));
    showDock(helpDock);
  } else if (action->text() == "Demos") {
    QString demo = "file:///" + grampsLib + "/demos/index.html";
    if (demoView) {
        //demoView->load(demo);
        QWebHistory *history = demoView->history();
        history->goToItem(history->itemAt(0));
    } else {
        demoDock = openUrl(demo, false);
        demoView = (WebWidget *) demoDock->widget();
        demoView->setAcceptDrops(false);
    }
    showDock(demoDock);
  }
  showStatus(helpView->title());
}

void MainWindow::close()
{
    glWidget->setTimer(false);
    save();

    exit(0);
//    int ret = QMessageBox::question(this, tr("exit gramps"), tr("Really exit?"),
//                         QMessageBox::Yes | QMessageBox::No);
//    if (ret == QMessageBox::Yes) {
//      exit(0);
//    }
}

bool MainWindow::save()
{
    QString afile = "";
    if (this->saveFile.isEmpty()) {
        afile = saveas();
        if (!afile.isEmpty()) {
            this->saveFile = afile;
            Db::openExisting(this->saveFile); // switch to this file
        }
    } else {
        afile = saveas(this->saveFile);
    }
    return !afile.isEmpty();
}

QString MainWindow::saveas()
{
    static QString mypath;
    if (mypath.isNull()) mypath = QDir::homePath() + "/savedSession.gdb3";
    QString fileName = QFileDialog::getSaveFileName(this, "gMol saved session file", mypath, tr("Session (*.gdb3)"));
    if (fileName.isEmpty()) return "";
    return saveas(fileName);
}
QString MainWindow::saveas(QString fileName) {
    chemWidget->updateCheckedRows();
    if (fileName == this->saveFile) return fileName;

    if (!fileName.isEmpty()) {
        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return "";
        }

        file.remove();
        if (Db::save(fileName)) {
            showStatus("saved " + this->saveFile);
        } else {
            return "";
        }
    }
    return fileName;
}

void MainWindow::openFile(QString fileName) {
    if (fileName.isNull()) return;
    QString ext = QFileInfo(fileName).suffix();
    if (ext == "html" || ext == "htm") {
      //helpView->loadUrl(QUrl("file:///" + fileName));
      openUrl("file:///" + fileName, true);
    } else if (ext == "pix" || ext == "qix" || ext == "cyl" || ext == "lab") {
      doCmd("get '" + fileName + "'");
    } else if (ext == "gdb3") {
        if (Db::openExisting(fileName)) {
            this->saveFile = fileName;
            emit restore(); // calls ChemWidget::restore
        }
#ifdef CHEM
    } else {
      chemWidget->addMolToDb(fileName, ext);
      //chemWidget->open(fileName);
      chemDock->raise();
#endif
    }
}

void MainWindow::open()
{
    static QString mypath;
    if (mypath.isNull()) mypath = QDir::homePath();
    QString fileName = QFileDialog::getOpenFileName(this, "Read file", mypath,
#ifdef CHEM
     tr(" Mols (*.sdf *.mol *.pdb *.mol2 *.smi);; gMol session (*.gdb3);; Gramps(*.pix *.qix *.cyl *.lab);; HTML (*.html *.htm);; All Files (*)"));
#else
     tr(" Gramps(*.pix *.qix *.cyl *.lab);; HTML (*.html *.htm);; All Files (*)"));
#endif
    openFile(fileName);
    mypath = QFileInfo(fileName).path();
}

void MainWindow::openUrl() {
    //QString init = helpView->url().toString();
    //QString init = "http://sourceforge.net/projects/sci3d/";
    QString init = "http://www.rcsb.org/pdb/files/3hiv.pdb";
    //QString init = "http://www.gnova.com/";
    bool ok;
    QString url = QInputDialog::getText(this, tr("Enter a URL for website or file"),
                  tr("Enter URL, for example:\n - www.google.com\n - http://sourceforge.net/projects/sci3d/\n - http://www.rcsb.org/pdb/files/4hhb.pdb\n - /home/users/joe/x.html\n - C:\\Documents and Settings\\joe\\My Documents\\x.html\n - file:///home/users/pam/z.html"), QLineEdit::Normal, init, &ok);
    if (ok) openUrl(url, true);
}

QDockWidget * MainWindow::openUrl(QString url, bool canDelete)
{
    QDockWidget *webDock = NULL;
    WebWidget *webView = NULL;
    //QUrl Url = QUrl(url);
    QUrl Url = QUrl::fromUserInput(url);
    //qDebug() << url;
    //qDebug() << Url.scheme() << Url.host() << Url.path();
    bool allowAccess = false; // access to gramps and gmol javascript objects
    if (Url.scheme() == "file") {
      allowAccess = true;
#ifdef WIN32
      // handle windows \ in files
      QFileInfo fileUrl = QFileInfo(Url.path().mid(1)); // all but first / in path
      //qDebug() << fileUrl.absoluteFilePath();
      Url.setUrl("file:///"+fileUrl.absoluteFilePath());
      //QUrl Url = QUrl(fileUrl.absoluteFilePath());
#endif
    }

    //if (Url.scheme().isEmpty()) Url.setScheme("http");
    if(Url.isValid()) {
      webDock = new QDockWidget("web", this);
      webView = new WebWidget(webDock, canDelete, allowAccess);
      webView->setMinimumWidth(450);
      webView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
      webDock->setWidget(webView);
      addDockWidget(Qt::LeftDockWidgetArea, webDock);
      addMenuItem(webDock, tr("Toggle"));
      //webDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetClosable);
      webViewConnections(webView, allowAccess);
      tabifyDockWidget(logDock, webDock);
      webView->loadUrl(Url);
    } else {
      QMessageBox::information(this, tr("URL"), Url.errorString());
    }
    return webDock;
}
void MainWindow::showTextWebPage(const QString text, const QString title) {
    QDockWidget *webDock = new QDockWidget("web", this);
    bool canDelete = true;
    bool allowAccess = false;
    WebWidget *webView = new WebWidget(webDock, canDelete, allowAccess);
    webView->setMinimumWidth(450);
    webView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    webDock->setWidget(webView);
    webDock->setWindowTitle(title);
    addDockWidget(Qt::LeftDockWidgetArea, webDock);
    addMenuItem(webDock, tr("Toggle"));
    //webDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetClosable);
    webViewConnections(webView, allowAccess);
    tabifyDockWidget(logDock, webDock);
    QString content = "<header><title>"+title+"</title></header>"+text;
    //qDebug() << content << content.toHtmlEscaped();
    webView->setHtml(content);
}

void MainWindow::webViewConnections(WebWidget *view, bool allowAccess) {
    connect(view,  SIGNAL(msgReady(const QString)),  this, SLOT(showStatus(const QString)));
    connect(view,  SIGNAL(cmdReady(const QString)),  this, SLOT(doCmd(const QString)));
    if (chemWidget != NULL) {
        connect(view,  SIGNAL(molReady(int)),  chemWidget, SLOT(molReady(int)));
        connect(view,     SIGNAL(readMol(const QString, const QString)),
                chemWidget, SLOT(readMol(const QString, const QString)));
    }
    if (allowAccess) {
        connect(view->gramps,  SIGNAL(cmdReady(const QString, bool)),  this, SLOT(doCmd(const QString, bool)));
    }
}

void MainWindow::snap()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Image file", gmolLib, tr("Images (*.jpg *.bmp *.tiff *.ppm)"));

    if (!fileName.isEmpty()) {
        // snap screen to file
        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }
        file.remove();

        doCmd("snap \"" + fileName + "\"");
    }
}
void MainWindow::reset() {
    doCmd("reset/rot/tran/leave LWorld");
}
void MainWindow::restart() {
    int ret = QMessageBox::question(this, tr("restart gramps"), tr("Really restart?"),
                         QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
      doCmd("restart");
      loadHelp();
    }
}

void MainWindow::processCmd() {
   doCmd(cmdInput->text());

}
QString MainWindow::doCmd(const QString cmd) {
  doCmd(cmd, false);
  return cmd; 
}
QString MainWindow::doCmd(const QString cmd, bool advwait) {
   //statusBar()->clearMessage();
   cmdOutput->appendPlainText(">"+cmd);
   int i = do_( cmd.toLocal8Bit().data(), cmd.length());
/*
   int i=do_( cmd.toLocal8Bit().data(), cmd.length());
   if (i == 0) {
     glWidget->setTimer(false);
   } else if (i < 0) {
     glWidget->setTimer(true);
   } else {
     glWidget->doAdvance();
   }
*/
   glWidget->updateGL();
   if(i)glWidget->doAdvance(advwait);
   cmdInput->clear();
   cmdOutput->moveCursor(QTextCursor::End);
   cmdOutput->ensureCursorVisible();
   return cmd;
}

void MainWindow::advanceDone() {
    // be sure mouse/dials stay attached.
    doCmd("usemouse LWorld", false);
}

void MainWindow::showStatus(const QString msg) {
    statusBar()->showMessage(msg);
}

void MainWindow::toggleStatus(bool hidden) {
    if (hidden) {
        statusBar()->show();
    } else {
        statusBar()->hide();
    }
}

void MainWindow::fatal(const QString msg) {
    QMessageBox::critical(this, tr("Unable to continue"), msg);
    close();
}
void MainWindow::select() {
  cmdInput->setText(cmdOutput->textCursor().selectedText());
  //cmdOutput->copy();
}

/*
void MainWindow::visibilityChanged(QWindow::Visibility visibility) {
    qDebug() << visibility << isFullScreen();
}
*/

