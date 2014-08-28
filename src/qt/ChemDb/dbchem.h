/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
#ifndef CHEMDB_H
#define CHEMDB_H

#include <QtGui>
#include <QtSql>
#include <QSqlDatabase>
#ifdef OBCHEM
#include <openbabel/mol.h>
#include <openbabel/obconversion.h>
#endif
#include "db.h"

#define TR(a) QObject::tr(a)


//class ChemDb : public QSqlDatabase
namespace ChemDb {

  QSqlDatabase open(QString dbname, bool create);
  void close(QSqlDatabase db);
  int getMolid();
  int getAtomid();
  //int molid=0;
  //int atid=0;

  bool readDb();
  bool createTmpTables();
  bool createTables();
  bool createAtomIndexes();
  bool createBondIndexes();
  bool dropIndexes();
  bool reportError(QSqlQuery);
  //bool addFlags(int);
  void  getVersion();
  void  setPragmas();
  bool addSecondaryStructure(int);
#ifdef OBCHEM
  int readFile(QString, QString);
  bool addMolecule(OpenBabel::OBMol &, QString, QString, QString, QString, QString, QString, int, int);
  bool addAtoms(OpenBabel::OBMol &);
  bool addBonds(OpenBabel::OBMol &);
  bool addProperty(OpenBabel::OBMol &);
  void protein(OpenBabel::OBMol &);
  int gen3D(OpenBabel::OBMol &, bool);
#endif
};

#endif
