/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
#include <QApplication>

#include "window.h"
#include "dbchem.h"
#ifdef PDBREADER
#include "db.h"
#endif
QString gmolLib;

void usage() {
  fprintf(stderr, "usage: chemdb dbfile [-f] [[-t type] infile.ext]...\n");
  fprintf(stderr, "       -f forces use of openBabel for pdb files\n");
  fprintf(stderr, "       without -t, type is guessed from .ext\n");
  fprintf(stderr, "       [[-t type] infile.ext] may repeat\n");
  fprintf(stderr, "       without infile, dbfile tables are displayed in new window\n");
  exit(0);
}

int getargs(int narg, int argc, char *argv[], QString &file, QString &type) {
  int n = narg;
  type = "";
  if (argc > n+2 && !strncmp(argv[n+1], "-t", 2)) {
    type = argv[n+2];
    n += 2;
  }
  if (argc > n+1) {
    file = argv[n+1];
    if (type.size() == 0) {
      type = QFileInfo(file).suffix();
    }
    ++n;
    return n;
  }
  return argc;
}

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  QString dbfile;
  QString infile;
  QString type;
  int molid;

#ifdef __APPLE__
  gmolLib = getenv("gmol_lib");
  if (gmolLib.length() == 0) {
    QDir appdir = QCoreApplication::applicationDirPath();
    appdir.cdUp();
    QString reldir = appdir.absolutePath();
    gmolLib = reldir + "/Resources";
  }
#else
    gmolLib = getenv("gmol_lib");
#endif

  if (argc < 2) usage();
  dbfile = argv[1];
  if (dbfile.startsWith("-")) usage();
  bool useOB = false;
  int narg = 0;
  if (argc > 2 && !strncmp(argv[2],"-f", 2)) {
    useOB = true;
    ++narg;
  }
  if (argc > narg+2) {
    QSqlDatabase db = ChemDb::open(dbfile, true);
    qDebug() << ChemDb::getMolid()  << "current molecules.";
    //qDebug() << ChemDb::getAtomid() << "current atoms.";
    ++narg;
    ChemDb::dropIndexes();
#ifdef PDBREADER
    if (!useOB) qDebug() << "attach residue" << Db::attachResidue();
#endif
    while ((narg = getargs(narg, argc, argv, infile, type)) < argc) {
      qDebug() << type;
#ifdef PDBREADER
      if (!useOB && type == "pdb") molid = Db::readPDB(infile); else
#endif
      molid = ChemDb::readFile(infile, type);
    }
    ChemDb::createAtomIndexes();
    ChemDb::createBondIndexes();
    db.close();

  } else {
    ChemDb::open(dbfile, false);
    qDebug() << ChemDb::getMolid()  << "current molecules.";
    Window *window = new Window();
    window->show();
    qDebug() << window;
    return app.exec();
  }

  return molid;
}
