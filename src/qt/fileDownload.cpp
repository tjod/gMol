#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QDebug>
#include "QtCore/qdir.h"
#include "QtCore/qtemporaryfile.h"
#include "fileDownload.h"
#include "mainwindow.h"
extern MainWindow * mainWindow;

FileDownloader::FileDownloader (const QUrl &url) {

        QNetworkRequest request(url);
        // Note: Qt 5.15 defaults to ManualRedirectPolicy; enable redirection if needed:
        // request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, true);

        reply = manager.get(request);
        connect(reply, &QNetworkReply::readyRead, this, &FileDownloader::onReadyRead);
        connect(reply, &QNetworkReply::finished, this, &FileDownloader::onFinished);
        connect(reply, &QNetworkReply::downloadProgress, this, &FileDownloader::onProgress);
}

    void FileDownloader::onReadyRead() {
        return;
    }

    void FileDownloader::onProgress(qint64 bytesReceived, qint64 bytesTotal) {
        qDebug() << "Downloaded:" << bytesReceived << "of" << bytesTotal << "bytes";
    }

    void FileDownloader::onFinished() {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Download failed:" << reply->errorString();
        } else {
            QString path = reply->url().path();
            QString tmplate = QDir::tempPath() + "/XXXXXX_" + QFileInfo(path).fileName();
            //QString suffix = QFileInfo(path).suffix(); // used as file type (e.g. .pdb .sdf)
            QTemporaryFile *tmpFile = new QTemporaryFile(tmplate);
            if ( tmpFile->open() ) {
                QTextStream stream( tmpFile );
                stream << reply->readAll();
                tmpFile->setAutoRemove(false); // needed outside this scope
                qDebug() <<  tmpFile->fileName() << " download completed successfully.";
                mainWindow->openFile(tmpFile->fileName());
            }   else {
                qDebug() << "Failed to open file for writing.";
            }
        }
        reply->deleteLater();
    }
