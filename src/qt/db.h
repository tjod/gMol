/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2013 by TJ O'Donnell and
C  Copyright (C) 2005-2013 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
#ifndef DB_H
#define DB_H

#include <QVariant>
#include <QSqlDatabase>
#include <QtSql>
#include <QSqlQuery>
#include <QTemporaryFile>
#include <QColor>

// items returned by getTreeRow
#define ITEM_ID 0
#define ITEM_PARENTID 1
#define ITEM_IMOL 2
#define ITEM_IATOM 3
#define ITEM_GRAMPSNAME 4
#define ITEM_ROWNAME 5
#define ITEM_RESNUM 6
#define ITEM_CHAIN 7
#define ITEM_IGNORE 8
#define ITEM_STYLE 9
#define ITEM_COLORBY 10
#define ITEM_FILTER 11
#define ITEM_HUE 12
#define ITEM_SATURATION 13
#define ITEM_VALUE 14
#define ITEM_ALPHA 15
#define ITEM_HYDROGENS 16
#define ITEM_MAINSIDE 17
#define ITEM_CHECKED 18

// these  must correspond to the id column in the table filters in init.gdb3
#define FILTER_NONE       0  // None filtered, so all
#define FILTER_CHAIN      1  // All but water
#define FILTER_WATER      2  // Water
#define FILTER_MAIN       3  // Main chain atoms, not CA
#define FILTER_SIDE       4  // Side chain atoms
#define FILTER_RESIDUE    5  // One chosen residue
#define FILTER_TRACE      6  // CA atoms
#define FILTER_CARTOON    7  // All main chain atoms
#define FILTER_ATOM       8  // One chosen atom
#define FILTER_HETATM     9  // All HETATM as in pdb file
#define FILTER_MOLECULE  10  // Most atoms, but not waters

#define NOCHAIN '\0'
#define NOATOM -1

#define HYDROGEN_NONE -1
#define HYDROGEN_HIDE 0
#define HYDROGEN_SHOW 1
#define HYDROGEN_COUNT -2 // very special case used as param in iterAtoms.  not used in db actually

struct molRecord {
  bool valid;
  int imol;
  QString type;
  QString filename;
  QString title;
  int nresidue;
  int natoms;
  int nbonds;
  int model;
};

// struct chainRecord {
//   bool valid;
//   char name;
//   int nres;
//   int nhet;
//   int nwater;
//   int nmain;
//   int nside;
//   int ntrace;

// };

 struct chainCoordRecord {
   bool valid;
   float x;
   float y;
   float z;
   int resnum;
   QString name;
   char sstype;
 };

 struct atomRecord {
   bool valid;
   int atid;
   int molid;
   int resnum;
   QString resnam;
   char altLoc;
   char icode;
   int atnum;
   float x;
   float y;
   float z;
   float fcharge;
   float pcharge;
   QString name;
   char chain;
   int hetatm;
 };

 struct residueRecord {
   bool valid;
   int  number;
   QString name;
 };

 struct bondRecord {
   bool valid;
   float ax;
   float ay;
   float az;
   int anum;
   int ares;
   char achain;
   float acharge;
   int ahetatm;
   float bx;
   float by;
   float bz;
   int bnum;
   int bres;
   char bchain;
   float bcharge;
   int bhetatm;
 };

 struct treeRow {
   int itemId;
   int parentId;
   int imol;
   int iatom;
   QString grampsName;
   QString rowname;
   int resnum;
   char chain;
   int ignore;
   int style;
   int colorBy;
   int filter;
   QColor color;
   int hydrogens;
   int mainSide;
   int checked;
   bool valid;
 };

 struct triRecord {
   bool valid;
   int vid;
   int tid;
 };
 struct vtxRecord {
   bool valid;
   float x;
   float y;
   float z;
   float nx;
   float ny;
   float nz;
   float value;
   int vid;
   int atnum;
 };

struct selectionFilter {
    int id;
    int ordering;
    QString appearance;
    QString menu;
    QString sql;
    QString tip;
};

class Db {
public:
    static QList<selectionFilter> filters;

    Db();
    static QSqlDatabase open();
    static QSqlDatabase openDbFile(QString fileName);
    static bool openExisting(QString filename);
    static bool attachResidue();
    static bool save(QString file);
    static int numRows(QSqlQuery);
    static bool tellError(QSqlQuery &, QString);
    static bool deleteFromTables(int imol);

    static int readPDB(QString pdb, QString name);
    static int readPDB(QString filename);
    static void checkResidues(int molid);
    static int getMolIdFromInchiKey(QString);
    static int molNumRes(int imol);
    static QString molResName(int imol, int resnum, char chain);
    static QSqlQuery iterMols();
    static molRecord nextMol(QSqlQuery);
    static float molCenter(int imol, unsigned int resnum, char chain, int filter, float *center, float *sizes);
    static QString molTitle(int imol);
    static QSqlQuery iterMolsByFile();
    static QSqlQuery iterMolsinFile(int imol);
    static int molNumAtoms(int imol);
    static void molBounds(int imol, unsigned int resnum, char chain, int filter, float *min, float *max);

    static atomRecord findAtomNear(int imol, char chain, float *xyzw, float range);
    static atomRecord getAtom(int imol, int iatom);
    static int getAtomResNum(int imol, int iatom);
    static char getAtomChain(int imol, int iatom);
    static QSqlQuery iterAtoms(int imol, int resnum, char chain, int filter, int hydrogens);
    static atomRecord nextAtom(QSqlQuery);
    static QSqlQuery iterBonds(int imol, int resnum, char chain, int filter, int hydrogens);
    static bondRecord nextBond(QSqlQuery);

    static bool createTreeTable();
    static int newTreeRow(treeRow &row);
    static treeRow getTreeRow(int itemid);
    static treeRow getTreeRow(QString grampsName);
    static treeRow getTreeSibling(int parentid, char sibchain);
    static bool isTreeRow(int imol, char chain, QString rowname);
    static bool isTreeRow(int imol, char chain, int iatom);
    static bool updateTreeColor(int itemid,  QColor color);
    static bool updateTreeColorBy(int itemid,  int colorBy);
    static bool updateTreeHydrogens(int itemid,  int hydrogens);
    static bool updateTreeMainSide(int itemid, int mainSide, int filter);
    static bool updateTreeIgnore(int itemid,  int ignore);
    static bool updateTreeStyle(int itemid,  int style);
    static bool updateTreeChecked(int itemid, int checked);
    static QSqlQuery iterTreeRows();
    static QSqlQuery IterTreeRowsToRestore();
    static treeRow nextTreeRow(QSqlQuery query);

    static int chainNumRes(int imol, char chain);
    static QSqlQuery iterChainCoords(int imol, char chain, int filter);
    static chainCoordRecord nextChainCoord(QSqlQuery);
    static std::string getChainSS(int molid, char chain, int resnum);
    static QSqlQuery iterChainCounts(int imol, char chain);
    static QSqlQuery iterChainCounts(int imol);
    static QHash<QString, int> nextChainCounts(QSqlQuery);

    static int countVertices(int itemid);
    static QSqlQuery iterVertices(int itemid, int imol, int resnum, char chain, int filter, int hydrogen, int colorBy, float nearAtom);
    static vtxRecord nextVertex(QSqlQuery);
    static QSqlQuery iterTriangles(int itemid);
    static triRecord nextTriangle(QSqlQuery);

private:
    static int itemId;
    static int maxItemId();

    static bool tellError(QSqlQuery &);

    static int molNumBonds(int imol);
    static molRecord getMol(int);

    //  static QSqlQuery iterResidues(int imol, int resnum, char chain, int filter);
    //  residueRecord nextResidue(QSqlQuery);

    //int resCount(int imol, char chain, int *Ntrace, int *Nwater, int *Nhet, int *Nmain, int *Nside);
    static int findAtomIdNear(int imol, char chain, float *xyzw, float range);
    static QString getAtomResName(int imol, int iatom);
    static void getAtomCoord(int imol, int iatom, float *center);
    static QString getAtomName(int imol, int iatom);

    static QList<selectionFilter> getFilters();
    static selectionFilter getFilter(int filterid);
#ifdef PDBREADER
    static int symbolToNumber(QString symbol);
    static void addAtom(QSqlQuery q, int molid, int aidx, int resnum, QString resnam, char altLoc, char icode, int atnum,
                        float x, float y, float z, QString name, char chain, int hetatm);
    static bool addProperty(int molid, QMap<QString,QString>property);
    static void newMolecule(int molid);
    static void updateMolecule(int molid, QString file, int natom, QString title);
    static void updateMoleculeFile(int molid, QString file);
    static void addBonds(int mol1, int molid);
    static void addBond(QSqlQuery q, int molid, int aid, int bid, int bo);
    static int processAtom(QString s, QSqlQuery q, int molid);
    static int processHetatm(QString s, QSqlQuery q, int molid);
    static void processTer(QString s);
    static void processSsbond(QString s, QSqlQuery q, int atid);
    static void processConect(QString s, QSqlQuery q, int molid);
    static int readPDB(std::istream& is, QString filename, int fsize);
#endif
};
#endif
