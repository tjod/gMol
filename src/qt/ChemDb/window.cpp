/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
#include <QtGui>
#include <QMenuBar>
#include "window.h"
//#include "chemdb.h"

Window::Window() {

  window = new QMainWindow;
  tableData = new QTableView();
  QString table = createMenus();

  model = new QSqlTableModel;
  model->setEditStrategy(QSqlTableModel::OnManualSubmit);
  tableData->setModel(model);

  setCentralWidget(tableData);
  setTable(table);

}

void Window::sql() {
}
void Window::dbclose() {
  delete(model);
  delete(tableData);
  close();
}

void Window::setTable(QString table) {
  QSqlDatabase db = QSqlDatabase::database();
  setWindowTitle(db.databaseName() + ":" + table);
  model->setTable(table);
  model->select();
}

QString Window::createMenus() {

  fileMenu  = menuBar()->addMenu(tr("File"));
  tableMenu = menuBar()->addMenu(tr("Tables"));
  fileMenu->addAction(tr("SQL"), this, SLOT(sql()));
  fileMenu->addAction(tr("Exit"), this, SLOT(dbclose()));
  QSqlDatabase db = QSqlDatabase::database();
  QSignalMapper *signalMapper = new QSignalMapper(tableMenu);
  connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(setTable(QString)));
/*
  QStringList tables = db.tables(QSql::Tables);
  for (int i=0; i<tables.size(); ++i) {
  }
*/
  QSqlQuery query;
  query.exec("Select name from sqlite_master where type ='table'");
  while (query.next()) {
    QString tableName = query.value(0).toString();
    signalMapper->setMapping(tableMenu->addAction(tableName, signalMapper, SLOT(map())), tableName);
  }
  query.first();
  QString firstTable = query.value(0).toString();
  return firstTable;
}
