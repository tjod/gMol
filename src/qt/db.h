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
#define FILTER_CARTOON    7  // CA and O "guide" atoms
#define FILTER_ATOM       8  // One chosen atom
#define FILTER_HETATM     9  // All HETATM as in pdb file
#define FILTER_MOLECULE  10  // Most atoms, but not waters
#define FILTER_SURFACE   25  // no waters, no hetatm

#define NOCHAIN '\0'
#define NOATOM -1
#define NOMOL -1
#define NORESNUM INT_MAX // need to allow "any" integer value, including negative, according to PDB; but we'll disallow this one value

#define HYDROGEN_NONE -1
#define HYDROGEN_HIDE 0
#define HYDROGEN_SHOW 1
#define HYDROGEN_COUNT -2 // very special case used as param in iterAtoms.  not used in db actually

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
    Db();
    ~Db();

    static QList<selectionFilter> filters;
    static QList<selectionFilter> getFilters();
    static selectionFilter getFilter(int filterid);

    static QSqlDatabase open();
    static QSqlDatabase openDbFile(QString fileName);
    static bool openExisting(QString filename);
    static bool attachResidue();
    static bool save(QString file);
    static int numRows(QSqlQuery);
    static bool tellError(QSqlQuery &, QString);
    static bool tellError(QSqlQuery &);
    static bool deleteFromTables(int imol);

    static int readPDB(QString pdb, QString name);
    static int readPDB(QString filename);
    static void checkResidues(int molid);
    static int getMolIdFromInchiKey(QString);
    static float molCenter(int imol, unsigned int resnum, char chain, int filter, float *midpoint, float *sizes, float *avg);
    static void molBounds(int imol, unsigned int resnum, char chain, int filter, float *min, float *max, float *avg);

    static int chainNumRes(int imol, char chain);
    static QSqlQuery iterChainCounts(int imol, char chain);
    static QSqlQuery iterChainCounts(int imol);
    static QHash<QString, int> nextChainCounts(QSqlQuery);

    static bool gramspSave(QString);
    static void grampsSaveEmpty();
    static QStringList getGrampsSave();
private:
    static int itemId;
    static int maxItemId();

#ifdef PDBREADER
    static int symbolToNumber(QString symbol);
    static void addAtom(QSqlQuery q, int molid, int aidx, int resnum, QString resnam, char altLoc, char icode, int atnum,
                        float x, float y, float z, QString name, char chain, int hetatm);
    static bool addProperty(int molid, QString name, QString value, int irecord);
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

class propertyQuery : public QSqlQuery {
public:
    propertyQuery();
    ~propertyQuery();

 /* let these be public, to make getting easy.
  * setting would be harmless, meaningless and ineffective.
  */
    bool valid;
    int imol;
    QString name;
    QString text;
    int order;

    bool next();
    bool iter(int);
};

class molQuery : public QSqlQuery {
public:
    molQuery();
    ~molQuery();

    bool valid;
    int imol;
    QString type;
    QString filename;
    QString title;
    int nresidue;
    int natoms;
    int nbonds;
    int model;

    bool next();
    bool get(int);
    bool getInFile(int);
    bool getByFile();

};

class atomQuery : public QSqlQuery {
public:
    atomQuery();
    ~atomQuery();

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
    int nfound;
    float frange;

    bool next();

    bool near(int imol, char chain, float *xyzw, float range);
    bool iterNear(int atomid, float range);
    bool get(int imol, int iatom);
    bool iter(int imol, int resnum, char chain, int filter, int hydrogens);
    int  hcount(int, int, char, int);
    int count();    
    QString getResnam(int imol, int resnum, char chain);
    static QString atomClause(int qmol, int qresnum, char qchain, int qfilter, int qhydrogens);      

private:
    int findAtomIdNear(int, char, float *, float);
    QString atomSql   (int qmol, int qresnum, char qchain, int qfilter, int qhydrogens);
};

class triangleQuery : public QSqlQuery {
public:
    triangleQuery();
    ~triangleQuery();

    bool valid;
    int vid;
    int tid;

    bool iter(int);
    bool next();
    static int count(int);
};
class vertexQuery : public QSqlQuery {
public:
    vertexQuery();
    ~vertexQuery();

    bool valid;
    float x;
    float y;
    float z;
    float nx;
    float ny;
    float nz;
    float vprop; // computed charge or distance property of this point
    int vid;
    int atnum;

    bool iter(int, int, int, char, int, int, int, float);
    bool next();
    bool first();
    bool get();
    //static int count(int);
    int count();
};

class bondQuery : public QSqlQuery {
public:
    bondQuery();
    ~bondQuery();

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

      bool iter(int, int, char, int, int);
      bool next();
      bool first();
      bool get();
      //static int count(int, unsigned int, char, int, int);
      int count();

private:
      static QString bondSql(int imol, int resnum, char chain, int filter, int hydrogens);
};

class chainQuery : public QSqlQuery {
public:
    chainQuery();
    ~chainQuery();

      bool valid;
      float x;
      float y;
      float z;
      int resnum;
      QString name;
      char sstype;

      bool iter(int, char, int);
      bool next();
      static int numRes(int, char);
      //static std::string getChainSS(int molid, char chain, int resnum);  
      int count();
};

class treeQuery : public QSqlQuery {
public:

    treeQuery();
    ~treeQuery();

    bool next();
    bool iter();
    bool iter(QString);
    bool iterRestore();
    bool getRow(int itemid);
    bool getRow(QString grampsName);
    bool getSibling(int parentid, char sibchain);

    static bool createTable();
    bool newRow(int parentid, int imol, int iatom, QString grampsname, QString rowname,
               int resnum, char chain, int ignore, int style, int colorBy, int filter,
               int hue, int saturation, int value, int alpha, int hydrogens, int mainSide, bool checked);
    static bool isRow(int imol, char chain, QString rowname);
    static bool isRow(int imol, char chain, int iatom);
    bool updateColor(int itemid, QColor color);
    bool updateColorBy(int itemid, int colorBy);
    bool updateIgnore(int itemid, int ignore);
    bool updateChecked(int itemid, int checked);
    bool updateStyle(int itemid, int style);
    bool updateHydrogens(int itemid, int hydrogens);
    bool updateMainSide(int itemid, int mainSide, int filter);
 
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

class paramQuery : public QSqlQuery {
public:
    
    paramQuery();
    ~paramQuery();
    
    bool next();
    bool iter(QString group, QString kind);
    bool iter(QString kind);
    bool iter();
    bool update(QString akind, QString agroup, QString aname, QString avalue);
    
    QString pkind;
    QString pgroup;
    QString pname;
    QString pvalue;
    bool valid;
};

#endif
