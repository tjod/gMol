/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2013 by TJ O'Donnell and
C  Copyright (C) 2005-2013 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
#include <QApplication>
#include "mainwindow.h"
#include "atom_properties.h"
#include "gmolApp.h"

MainWindow * mainWindow;  // others need this and its widgets
QString gmolLib;
QString grampsLib;

int main(int argc, char *argv[]) {
    gmolApp *app = new gmolApp(argc, argv);
    app->setOrganizationName("gNova");
    app->setApplicationName("gMol");

    // QCoreApplication::addLibraryPath("/opt/local/libexec/qt4/share/plugins/");
    // qDebug() << "Actual search paths:" << QCoreApplication::libraryPaths();
    // qDebug() << QSqlDatabase::drivers();

    gmolLib   = getenv("gmol_lib");
    grampsLib = getenv("gramps_lib");
#ifdef __APPLE__
// look in app bundle resources, but don't reset gmolLib or grampsLib in setenv
    QDir appdir = QCoreApplication::applicationDirPath();
    appdir.cdUp();
    QString reldir = appdir.absolutePath();
    if (gmolLib.isNull()) gmolLib   = reldir + "/Resources";
    if (grampsLib.isNull()) {
        grampsLib = reldir + "/Resources/gramps";
        setenv("gramps_lib", grampsLib.toLocal8Bit().data(), 1);
    }
//    qDebug() << QCoreApplication::applicationDirPath();
//    qDebug() << gmolLib;
//    qDebug() << grampsLib;
#endif

    qDebug() << "Supports SSL:" << QSslSocket::supportsSsl();
    // qDebug() << "OpenSSL Build Version:" << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << "OpenSSL Runtime Version:" << QSslSocket::sslLibraryVersionString();

    // Force the global network configuration to use modern TLS
    QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
    // sslConfig.setProtocol(QSsl::TlsV1_2OrLater); // Forces TLS 1.2 or TLS 1.3
    sslConfig.setProtocol(QSsl::TlsV1_2); // Forces TLS 1.2
    QSslConfiguration::setDefaultConfiguration(sslConfig);

    qDebug() << "Supports SSL:" << QSslSocket::supportsSsl();
    // qDebug() << "OpenSSL Build Version:" << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << "OpenSSL Runtime Version:" << QSslSocket::sslLibraryVersionString();


    mainWindow = new MainWindow;
    mainWindow->show();
    mainWindow->activateWindow();
    mainWindow->raise();  // wasn't always being raised on osx
    if (argc > 0 && argv[1]) {
        mainWindow->openFile(argv[1]);
    } else {
        mainWindow->setWindowTitle("gMol v1.85");
    }
    QObject::connect(app, SIGNAL(aboutToQuit()), mainWindow, SLOT(save()));

    // Load QSS from a resource path or local file path
    QFile file(gmolLib+"/gmol.qss");  // Use ":/" prefix if using a .qrc file
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QString styleSheet = QLatin1String(file.readAll());
        app->setStyleSheet(styleSheet);
        file.close();
    }

    return app->exec();
}
