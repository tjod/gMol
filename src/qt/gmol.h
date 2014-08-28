/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2013 by TJ O'Donnell and
C  Copyright (C) 2005-2013 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
#ifndef GMOL_H
#define GMOL_H

#include <QObject>
#ifdef QT5
#include <QtWebKitWidgets>
#endif
//#include "chem.h"
#include "db.h"

class Gmol : public QObject
{
    Q_OBJECT

public:
    Gmol(QObject *parent = 0);
    ~Gmol();

private:
    QList <QVariant> getChildren(int parentid);

public slots:
    QMap <QString, QVariant> atest();
    QList <QVariant> getTree();
    int getMolIdFromInchiKey(const QString);
    int readMolFromUrl(const QString);
    //int readMolFromFileName(const QString);
    //int readMolFromFileElement(const QWebElement);
    void addMol(const int);

signals:
    void molAdded(const int);

protected:

private:
};
#endif
