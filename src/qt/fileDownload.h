#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QDebug>

class FileDownloader : public QObject {
    Q_OBJECT
public:
    FileDownloader(const QUrl &url);

private slots:
    void onReadyRead();
    void onProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onFinished();
private:
    QNetworkAccessManager manager;
    QNetworkReply *reply;
    QTemporaryFile *tmpFile;
};
