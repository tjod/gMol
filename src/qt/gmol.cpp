/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2013 by TJ O'Donnell and
C  Copyright (C) 2005-2013 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/

#include "gmol.h"
#include "db.h"
#include "mainwindow.h"

extern MainWindow * mainWindow; // need to access some widgets
/* this object is exposed to JavaScript for all pages loaded into webView */
Gmol::Gmol(QObject *parent)
    : QObject(parent)
{
}

Gmol::~Gmol()
{
}

QList <QVariant>Gmol::getChildren(int parentid) {
  QList <QVariant>children;
  QSqlQuery query;
  query.prepare("Select * From tree Where parentid = ?");
  query.addBindValue(parentid);
  if (query.exec()) {
    while(query.next()) {
      QMap <QString, QVariant>child;
      QSqlRecord r = query.record();
      for (int i=0; i<r.count(); ++i) {
         child[r.fieldName(i)] = r.value(i);
      }
      child["children"] = getChildren(r.value("itemid").toInt());
      children.append(child);
    }
  }
  return children;
}
QList <QVariant>Gmol::getTree() {
  return getChildren(0);
}

int Gmol::getMolIdFromInchiKey(const QString key) {
  return Db::getMolIdFromInchiKey(key);
}

int Gmol::readMolFromUrl(const QString url) {
    mainWindow->openUrl(url, true);
    return 1;
}

void Gmol::addMol(const int imol) {
    emit molAdded(imol); // signal to be connected to javascript function
}

/*
int Gmol::readMolFromFileName(const QString fileName) {
    QString ext = QFileInfo(fileName).suffix();
    mainWindow->chemWidget->addMolToDb(fileName, ext);
    return 1;
}

int Gmol::readMolFromFileElement(const QWebElement fileElement) {
    QString fileName = fileElement.attribute("value");
    QStringList anames = fileElement.attributeNames();
    qDebug() << anames << fileElement.attribute("type") << fileElement.attribute("name") << fileElement.attribute("value");
    QString ext = QFileInfo(fileName).suffix();
    mainWindow->chemWidget->addMolToDb(fileName, ext);
    return 1;
}
*/

QMap <QString, QVariant>Gmol::atest() {
  QMap <QString, QVariant>result;
  result["success"] = true;
  result["message"] = "hello";
  result["code"] = 12;

  QList <QVariant> things;
  things << "abc" << -9 << "def" << 16 << "ghi";

  QMap <QString, QVariant> items;
  items["cup"] = "mug";
  items["dish"] = "plate";
  items["utensil"] = "fork";
  things.append(items);

  things.append(true);

  result["things"] = things;
  result["fval"] = 1.4;
  return result;
}
