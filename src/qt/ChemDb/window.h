/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QTableView>
#include <QSqlQuery>

class Window : public QMainWindow
{
    Q_OBJECT

public:
  Window();

private:
  QMainWindow *window;
  QMenu *fileMenu;
  QMenu *tableMenu;
  QTableView *tableData;
  QSqlTableModel *model;
  QString createMenus();

private slots:
  void dbclose();
  void sql();
  void setTable(QString);
};

#endif
