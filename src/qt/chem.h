/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2013 by TJ O'Donnell and
C  Copyright (C) 2005-2013 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
#ifndef CHEM_H
#define CHEM_H

#include <QDockWidget>
#ifdef QT5
#include <QtWidgets>
#else
#include <QtGui>
#endif
#include <QSqlQuery>
#include "db.h"
#include "gramps.h"

#define NAME_COLUMN  2
#define COLOR_COLUMN 1
#define MOL_COLUMN 0

#define COLOR_BY_NONE      0
#define COLOR_BY_SOLID     1
#define COLOR_BY_ATOMS     2
#define COLOR_BY_HETERO    3
#define COLOR_BY_HATOM     4
#define COLOR_BY_CHARGE    5
#define COLOR_BY_RESIDUE   8

#define STYLE_CURRENT 0
#define STYLE_NONE 0
#define STYLE_HYDROGEN            1
#define STYLE_LINES               2
#define STYLE_CYLINDERS           3
#define STYLE_BALLANDSTICK        4
#define STYLE_VDW                 5
#define STYLE_SPHERE              6
#define STYLE_STICKSANDCOLOREDHET 7
//#define STYLE_COLOREDSTICKSANDHET 8

#define STYLE_SURF_MOL            9
#define STYLE_SURF_WATER          10
#define STYLE_SURF_CUSTOM         17

#define STYLE_CARTOON_THIN        11
#define STYLE_CARTOON_THICK       12

#define STYLE_ATOM_VDW            13
#define STYLE_ATOM_SPHERE         14
#define STYLE_ATOM_BALL           15

#define STYLE_DUMMY               16
#define STYLE_DEFAULT             STYLE_STICKSANDCOLOREDHET

#define BOND_RADIUS 0.15
//#define CA_RADIUS 2.2
//#define CA_RADIUS Atom::radius[6]
#define CA_RADIUS 4*BOND_RADIUS
#define ATOM_SPHERE_RADIUS 4*BOND_RADIUS
#define ATOM_BALL_RADIUS   2*BOND_RADIUS
#define ATOM_VDW_RADIUS  0.0
#define CA_DIST 3.8
#define DRAW_DRAW 0
#define DRAW_OPEN 1
#define DRAW_CLOSE 2

#define RIBBON_WIDTH 1.25
#define COIL_WIDTH 1.25
#define HELIX_OFFSET 2.0
#define COIL_RADIUS 0.20

#define COLOR_DEFAULT QColor::fromHsv(150,255,255)
//#define COLOR_NONE    QColor::QColor()
#define COLOR_NONE    QColor()
#define COLOR_CARBON  QColor::fromHsv(0,0,160)
#define COLOR_WATER   QColor::fromHsv(180,255,255)

//#define HAS_BIT(flag, FILTER_BIT) (flag&&((flag&FILTER_BIT)==flag))

#define PICKNAME "DISTANCE"

//   Q_DECLARE_METATYPE(QTreeWidgetItem*);
class ChemWidget : public QTreeWidget
{
    Q_OBJECT

public:
    ChemWidget(QDockWidget *parent = 0);
    ~ChemWidget();
    int addMolToDb(QString fileName, QString fmt);
    //QTreeWidgetItem * getTreeRow(int itemid);
    void showOneItem(QTreeWidgetItem *);
    void updateCheckedRows();
    void gotPick(const QPoint &, grampsPick gp);
    void gotDoubleClick(const QPoint &, grampsPick gp);
    void showPick(const QPoint &globalP, grampsPick gp);

public slots:
    void clicked(QTreeWidgetItem *, int);
    void readMol(const QString, const QString);
private slots:
    void doubleClicked(QTreeWidgetItem *, int);
    //void pressed(QTreeWidgetItem *, int);
    void changed(QTreeWidgetItem *, int);
    void showContextMenu(const QPoint &);
    void showHydrogens(bool);
    void showMainSide(bool);
    void addSurfRow();
    void makeSurface();
    void addSurface(int);
    void addFilterRow();
    void styleMol();
    void addResidue();
    void addAtom();
    void hideMol();
    void ignoreMol();
    void deleteMol();
    void infoMol();
    void colorMolByRandom();
    void colorMolByChoose();
    void colorMolBySolid();
    void colorMolByAtom();
    void colorMolByCharge();
    void colorMolTransparent(bool);
    void zoomMol();
    void highlightMol();
    void centerMol();
    void midRangeMol();
    void molReady(int);
    void surfReady(int);
    void readProc();
    void restore();
    void showNear();    

signals:
    void cmdReady(const QString);
    void msgReady(const QString);
    void molAdded(const int);
    void fatal(const QString);
    void textWebPage(const QString, const QString);

protected:
    //bool event(QEvent *event);
private:
    QProcess *dbproc;
    QProcess *surfproc;
    QProgressDialog *progress;
    QTreeWidgetItem *rootMol;
    int hydrogenDefault;
    treeQuery currentRow;
    int zoomFilter;
    QScopedPointer<QMenu> molMenu;
    QScopedPointer<QMenu> pickMenu;

    bool getCurrentRow(QTreeWidgetItem *item);
    bool getCurrentRow(int itemid);
    QTreeWidgetItem *setCheckedRows();
    //void showRootMenu(const QPoint &);
    QString makePickedAtomName(bool withRes);
    int setItemsFromPick(QTreeWidgetItem *item, grampsPick gp);
    QStringList pickPath(QString name, QTreeWidgetItem *item);
    QString processPick(QTreeWidgetItem* item, grampsPick gp);    
    int open(int molid, bool centerZoom);
    QString drawMol(QTreeWidgetItem *);
    QString drawOneAtom(QTreeWidgetItem *);
    int drawDummy(const QString mname);
    int lines(int imol, const QString mname, unsigned int rindex, char chain, int colorBy, bool cyl, int balls, int filter, int hydrogens);
    int spheres(int imol, int iatom, const QString molnam, int resnum, char chain, int colorBy, int hetero,
      int filter, int hydrogens, float radius, float Hradius);
    void normal(float[3], float[3], float[3], float[3]);
    int drawBond(bondQuery bond, bool pairs, int colorBy, float *spec);
    int drawAtom(atomQuery atom, int colorBy, float radius, float Hradius);
    int drawAtom(atomQuery atom, int colorBy, float radius, float Hradius, int hetero);
    int sendChain(int nca, float *coord, float cradius, float radius, int spline);
    int drawRibbon(int imol, QString molnam, char cname, int mode, float sradius, float cradius);
    int makeRibbon(int nguide, float *guide_minus, float *guide_plus, float *guide_zero, std::string sstype, int hires, float radius, float width, bool endcaps, bool showUnknown);
    int sendRibbon(float spline_minus[][3], float spline_plus[][3], int slength, float thick, float width, int ntaper, int edges);
    int sendArrow(float spline_minus[][3],  float spline_plus[][3], int slength, float width);
    int sendBase(float spline_minus[][3],  float spline_plus[][3], float width);
    int sendCoil(float *spline, float *guide, int offset, int slength, float xradius, float yradius);
    int drawChain(int imol, QString molnam, char cname, int mode, float sradius, float cradius, bool endcaps);
    int drawCartoon(int imol, QString molnam, char cname, int mode, float sradius, float cradius);
    int drawSurface(int itemid);
    QString grampsSafe(QString name);
    QString encodeMolName(int imol, char chain, int rindex, QString suffix);
    void createContextMenu(QTreeWidgetItem *, int, QMenu *, const char* slot, bool highlight);
    void showPickMenu(const QPoint &, QTreeWidgetItem *, QString name);
    void addColorMenu(QMenu *, int colorBy);
    void addAtomStyleMenu(QMenu *styleMenu, int current_style, const char *slot);
    void addSurfStyleMenu(QMenu *styleMenu, int current_style, const char *slot);
    void addCartoonStyleMenu(QMenu *styleMenu, int current_style, const char *slot);
    void addHydrogenMenu(QMenu *styleMenu, int hydrogens);
    void addMainSideMenu(QMenu *styleMenu, int mainSide);
    void addStyleMenu(QMenu *styleMenu, int current_style, const char *slot);
    void addFilterMenus(QMenu *menu);
    void blankOrDeblank(Qt::CheckState state, QString grampsName);
    QString atomName(int);
    void setAtomColor(int atnum);
    void setAtomColor(int atnum[], int natom);
    void setChargeColor(float charge);
    void setMolColor(QTreeWidgetItem*, QColor, int);
    QColor nextColor(QColor color);
    QColor applyColor(QString, QColor);
    void colorMolBy(QTreeWidgetItem *item, int colorby);
    void colorMol(QTreeWidgetItem *item, QColor color);
    int resCount(int imol, char cname, int *ntrace, int *nwater, int *nhet, int *nmain, int *nside);
    QList<QString> addChainsRow(QTreeWidgetItem *root, int imol);
    void makeNewCurrentRow(QString rowname, int rootType, QString grampsName, int resnum,
                           Qt::CheckState state, int imol, int iatom, char chain, int drawstyle,
                           int colorBy, QColor color, int filter, int hydrogens);
    QTreeWidgetItem * addMolRow(QTreeWidgetItem *root, QString rowname,
     int rindex, Qt::CheckState state, int imol, int iatom, char chain, QString suffix, int style, int colorBy, QColor color, int filter);
    QTreeWidgetItem * makeMolRow(QTreeWidgetItem *root, QString grampsName, QString rowname, Qt::CheckState state);
    //QTreeWidgetItem *addWaterRow(QTreeWidgetItem *root, int imol, unsigned int rindex, char cname, int nwaters);

    int adjustChildren(QTreeWidgetItem *, Qt::CheckState);
    void adjustParents(QTreeWidgetItem *);
    QTreeWidgetItem *adjustParent(QTreeWidgetItem *);
    void updateColorIcon(QTreeWidgetItem *, QColor, int);
    void insertOrGroup(QString molnam, treeQuery treeParent);
    QTreeWidgetItem *getGrampsItem(QString name);

    void restoreOrientation();
    float centerMol(int filter, bool midrange);
    void zoomMol(int filter);
    int cycleZoom();
    float molCenter(int, float*midpoint, float *sizes, float *avg);
    void addResidue(int);
    void styleMol(int);
    void addAtom(int);

    int getMemErr;
    int getMem    (const char *name, float *data, int size, int namlen);
    int getMemMore(const char *name, float *data, int size, int namlen);
    int getMemDone(const char *name, float *data, int size, int namlen);
    void dumpout(float *, int);
    FILE *dfp;
    QString dfname;

    propertyQuery property_query;
    molQuery mol_query;
    atomQuery atom_query;
    atomQuery pickedAtom;
};

#endif
