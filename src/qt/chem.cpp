//C *************************************************************
//C This file contains source code for the gMol computer program
//C  Copyright (C) 1981-2013 by TJ O'Donnell and
//C  Copyright (C) 2005-2013 gNova, Inc.
//C It is unlawful to modify or remove this copyright notice.
//C See the accompanying LICENSE file for further information.
//C *************************************************************

#include "chem.h"
#include "atom_properties.h"
#include "atom_values.h" // only once per app
#include "mainwindow.h"
#include <QObject>
#include <QDebug>
#include <QMenu>
#ifdef QT5
#include <QtWidgets>
#endif
#include "cpk.xpm"
#include "charge.xpm"

#include <string>
#include <ios>
#include <ctime>

extern QString gmolLib;

ChemWidget::ChemWidget(QDockWidget *parent)
    : QTreeWidget(parent)
{   
    hydrogenDefault = HYDROGEN_HIDE;
    //pickedAtom.valid = false;
    setAnimated(true);
    //molMenu.reset(new QMenu(this));
    //pickMenu = new QMenu(tr("Pick"), this);
    srand (time(NULL));
    
    QStringList header;
    setColumnCount(3);
    header << tr("Molecule") << tr("Color") << tr("Name");
    setHeaderLabels(header);
    //setColumnHidden(2,true);
    //hideColumn(2);
    //setSortingEnabled(true);
    //sortByColumn(1, Qt::AscendingOrder);
    rootMol = invisibleRootItem();
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(clicked(QTreeWidgetItem *, int)));
    connect(this, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(changed(QTreeWidgetItem *, int)));
    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(doubleClicked(QTreeWidgetItem*,int)));
    //connect(this, SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT(pressed(QTreeWidgetItem *, int)));
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(showContextMenu(const QPoint &)));
    
    //Db::open();
    //pickedAtom = atomQuery::atomQuery();
    dbproc = new QProcess();
    connect(dbproc, SIGNAL(finished(int)), this, SLOT(molReady(int)));
    connect(dbproc, SIGNAL(readyReadStandardOutput()), this, SLOT(readProc()));
    surfproc = new QProcess();
    connect(surfproc, SIGNAL(finished(int)), this, SLOT(surfReady(int)));
    connect(surfproc, SIGNAL(readyReadStandardOutput()), this, SLOT(readProc()));
    progress = new QProgressDialog();
    
    treeQuery::createTable();
}

ChemWidget::~ChemWidget()
{
}

QTreeWidgetItem* ChemWidget::getGrampsItem(QString name) {
    QList<QTreeWidgetItem*> t = findItems(name, Qt::MatchFixedString|Qt::MatchCaseSensitive|Qt::MatchRecursive, NAME_COLUMN);
    if (t.count() > 0) return t[0];
    else return NULL;
}
bool ChemWidget::getCurrentRow(QTreeWidgetItem *item) {
    return getCurrentRow(item->type());
}

bool ChemWidget::getCurrentRow(int itemid) {
    if (currentRow.itemId == itemid && currentRow.valid) {
        ;
    } else {
        currentRow.getRow(itemid);
    }
    return currentRow.valid;
}

int ChemWidget::setItemsFromPick(QTreeWidgetItem *item, grampsPick gp) {
    //setCurrentItem(item);
    int itemid = item->type();
    getCurrentRow(itemid);
    if (currentRow.iatom == NOATOM) {
        float range = 1.5;
        pickedAtom.near(currentRow.imol,currentRow.chain, gp.xyzw, range);
        //qDebug() << pickedAtom.nfound << "atoms nearby";
        pickedAtom.finish(); // there may be more found than this near(est)
    } else {
        pickedAtom.get(currentRow.imol, currentRow.iatom); //Db::getAtom(currentRow.imol,currentRow.iatom);
    }
    return itemid;
}

void ChemWidget::showPick(const QPoint & /*globalP*/, grampsPick gp) {
    // single click on a gramps molecule object; just show its path and coord
    QTreeWidgetItem *item = getGrampsItem(gp.name);
    if (item) {
        setItemsFromPick(item, gp);
        //QString name = makePickedAtomName(true);
        //QStringList path = pickPath(name, item);
        //QStringList path = pickPath("", item);
        // let's show the gramps picked coord here instead of the picked (nearest) atom
        //emit msgReady(path.join(":")+xyz);
    }
    //if (idname_("scube",5)) emit cmdReady("forget scube");            
    QString xyz;    
    QTextStream(&xyz) << gp.name << "@@" << gp.xyzw[0] << "," << gp.xyzw[1] << "," << gp.xyzw[2]; // << "; " << pickedAtom.nfound << " atoms within " << pickedAtom.frange << QChar(0x212B);
    emit msgReady(xyz);    
}

QStringList ChemWidget::pickPath(QString name, QTreeWidgetItem *item) {
    QStringList path;
    if (!name.isNull() && !name.isEmpty()) path.prepend(name);
    QTreeWidgetItem *p = item;
    while (p) {
        path.prepend(p->text(0));
        p = p->parent();
    }
    return path;
}

QString ChemWidget::processPick(QTreeWidgetItem* item, grampsPick /*gp*/) {
    QString name = makePickedAtomName(true);
    QStringList path = pickPath(name, item);
    QString xyz;
    QTextStream(&xyz) << "@" << pickedAtom.x << "," << pickedAtom.y << "," << pickedAtom.z << "; " << pickedAtom.nfound << " atoms within " << pickedAtom.frange << QChar(0x212B);
    emit msgReady(path.join(":")+xyz);
    //if (idname_("scube",5)) emit cmdReady("forget scube");
    //QString cmd;
    //QTextStream(&cmd) << "showpick scube " << gp.xyzw[0] << "," << gp.xyzw[1] << "," << gp.xyzw[2] << "," << pickedAtom.frange;
    //emit cmdReady(cmd);
    return path[0];
}

void ChemWidget::gotPick(const QPoint &globalP, grampsPick gp) {
    // right-click or ctrl-click Mac
    // got a pick of a gramps molecule object with name and location
    QTreeWidgetItem *item = getGrampsItem(gp.name);
    if (item) {
        setCurrentItem(item);
        setItemsFromPick(item, gp);
        QString path = processPick(item, gp);
        showPickMenu(globalP, item, path);
        //showNear();
    }
}
void ChemWidget::showNear() {
    float data[9] = {0.,0.,1.};
    if (idname_(PICKNAME, strlen(PICKNAME))) {
        QString cmd = "forget ";
        cmd += PICKNAME;
        emit cmdReady(cmd);
    }
    int err = getMem(PICKNAME, data, 3, strlen(PICKNAME));
    if (err) return;
    data[0] = 2; data[1] = 2; data[2] = 3;
    for (atom_query.iterNear(pickedAtom.molid, pickedAtom.atid, 3.0); atom_query.next(); ) {
#ifdef DEBUG
        qDebug() << pickedAtom.atid << pickedAtom.resnam << pickedAtom.resnum << pickedAtom.name << atom_query.atid << atom_query.resnam << atom_query.resnum << atom_query.name;
#endif
        data[3] = pickedAtom.x; data[4] = pickedAtom.y; data[5] = pickedAtom.z;
        data[6] = atom_query.x; data[7] = atom_query.y; data[8] = atom_query.z;
        err = getMemMore("", data, 9, 0);
    }
    err = getMemDone("", NULL, 0, 0);
}

void ChemWidget::gotDoubleClick(const QPoint & /*globalP*/, grampsPick gp) {
    // double clicked on gramps object
    QTreeWidgetItem *item = getGrampsItem(gp.name);
    if (item) {
        setCurrentItem(item);
        setItemsFromPick(item, gp);
        zoomMol(FILTER_ATOM);                
        processPick(item, gp);
    } else {
        // double clicked on background
        if (cycleZoom() > 0) {
            float midpoint[3];
            float sizes[3];
            float avg[3];
            molCenter(currentRow.filter, midpoint, sizes, avg);
            QStringList path = pickPath(QString(""), currentItem());
            QString xyz;
            QTextStream(&xyz) << "@" << avg[0] << "," << avg[1] << "," << avg[2];
            emit msgReady(path.join(":")+xyz);
        }
    }
}

QString ChemWidget::makePickedAtomName(bool withRes) {
    QString aname = "";
    if (pickedAtom.valid) {
        if (pickedAtom.resnum != NORESNUM) {
            if (withRes) {
                QTextStream(&aname) << pickedAtom.resnam << pickedAtom.resnum << ":";
            }
            QTextStream(&aname) << pickedAtom.name;
            if (pickedAtom.altLoc != ' ') QTextStream(&aname) << "(" << pickedAtom.altLoc << ")";
        } else {
            QTextStream(&aname) << pickedAtom.name << "#" << pickedAtom.atid;
        }
    }
    return aname;
}

void ChemWidget::addColorMenu(QMenu *colorMenu, int current_color) {
    QActionGroup *g = new QActionGroup(colorMenu);

    QAction *act = colorMenu->addAction(tr("Random"), this, SLOT(colorMolByRandom()));
    act->setCheckable(false);
    //if (current_color == COLOR_BY_SOLID) act->setChecked(true);
    g->addAction(act);

    act = colorMenu->addAction(tr("Choose"), this, SLOT(colorMolByChoose()));
    act->setCheckable(false);
    //if (current_color == COLOR_BY_SOLID) act->setChecked(true);
    g->addAction(act);

    colorMenu->addSeparator();

    act = colorMenu->addAction(tr("Solid"), this, SLOT(colorMolBySolid()));
    act->setCheckable(true);
    if (current_color == COLOR_BY_SOLID) act->setChecked(true);
    g->addAction(act);

    act = colorMenu->addAction(tr("Atom types"), this, SLOT(colorMolByAtom()));
    act->setCheckable(true);
    if (current_color == COLOR_BY_ATOMS) act->setChecked(true);
    g->addAction(act);

    act = colorMenu->addAction(tr("Charge"), this, SLOT(colorMolByCharge()));
    act->setCheckable(true);
    if (current_color == COLOR_BY_CHARGE) act->setChecked(true);
    g->addAction(act);

    colorMenu->addSeparator();

    act = colorMenu->addAction(tr("Transparent"));
    act->setCheckable(true);
    QColor c = currentRow.color;
    //qDebug() << currentRow.rowname << c.alpha();
    if (c.alpha() < 255) {
        act->setChecked(true);
        //act->setData(255);
    } else {
        act->setChecked(false);
        //act->setData(128);
    }
    connect(act, SIGNAL(triggered(bool)), this, SLOT(colorMolTransparent(bool)));
    //g->addAction(act);
}

void ChemWidget::addHydrogenMenu(QMenu *styleMenu, int hydrogens) {
    if (hydrogens == HYDROGEN_NONE) return;
    QAction *actHydro = new QAction(tr("Hydrogens"), styleMenu);
    actHydro->setCheckable(true);
    actHydro->setChecked((hydrogens == HYDROGEN_SHOW) ? true:false);
    //actHydro->setDisabled(true);
    styleMenu->addAction(actHydro);
    connect(actHydro, SIGNAL(triggered(bool)), this, SLOT(showHydrogens(bool)));
}

void ChemWidget::addMainSideMenu(QMenu *styleMenu, int mainSide) {
    QAction *actMainSide = new QAction(tr("Main+Side"), styleMenu);
    actMainSide->setCheckable(true);
    actMainSide->setChecked(mainSide);
    //actHydro->setDisabled(true);
    styleMenu->addAction(actMainSide);
    connect(actMainSide, SIGNAL(triggered(bool)), this, SLOT(showMainSide(bool)));
}

void ChemWidget::addStyleMenu(QMenu *styleMenu, int current_style, const char *slot) {
    QActionGroup *g = new QActionGroup(styleMenu);

    QAction *act = styleMenu->addAction(tr("Sticks"), this, slot);
    act->setCheckable(true);
    if (current_style == STYLE_CYLINDERS) act->setChecked(true);
    act->setData(STYLE_CYLINDERS);
    g->addAction(act);

    act = styleMenu->addAction(tr("Ball/stick"), this, slot);
    act->setCheckable(true);
    if (current_style == STYLE_STICKSANDCOLOREDHET) act->setChecked(true);
    act->setData(STYLE_STICKSANDCOLOREDHET);
    g->addAction(act);

    act = styleMenu->addAction(tr("Lines"), this, slot);
    act->setCheckable(true);
    if (current_style == STYLE_LINES) act->setChecked(true);
    act->setData(STYLE_LINES);
    g->addAction(act);

    act = styleMenu->addAction(tr("vdW"), this, slot);
    act->setCheckable(true);
    if (current_style == STYLE_VDW) act->setChecked(true);
    act->setData(STYLE_VDW);
    g->addAction(act);

    act = styleMenu->addAction(tr("Sphere"), this, slot);
    act->setCheckable(true);
    if (current_style == STYLE_SPHERE) act->setChecked(true);
    act->setData(STYLE_SPHERE);
    g->addAction(act);
}

void ChemWidget::addAtomStyleMenu(QMenu *styleMenu, int current_style, const char *slot) {
  QActionGroup *g = new QActionGroup(styleMenu);

  QAction *act = styleMenu->addAction(tr("vdW"), this, slot);
  act->setCheckable(true);
  if (current_style == STYLE_ATOM_VDW) act->setChecked(true);
  act->setData(STYLE_ATOM_VDW);
  g->addAction(act);

  act = styleMenu->addAction(tr("Sphere"), this, slot);
  act->setCheckable(true);
  if (current_style == STYLE_ATOM_SPHERE) act->setChecked(true);
  act->setData(STYLE_ATOM_SPHERE);
  g->addAction(act);

  act = styleMenu->addAction(tr("Ball"), this, slot);
  act->setCheckable(true);
  if (current_style == STYLE_ATOM_BALL) act->setChecked(true);
  act->setData(STYLE_ATOM_BALL);
  g->addAction(act);

  return;

  // these cause whole residue to be drawn.  interesting, but ...
  act = styleMenu->addAction(tr("Sticks"), this, slot);
  act->setCheckable(true);
  if (current_style == STYLE_CYLINDERS) act->setChecked(true);
  act->setData(STYLE_CYLINDERS);
  g->addAction(act);

  act = styleMenu->addAction(tr("Ball/stick"), this, slot);
  act->setCheckable(true);
  if (current_style == STYLE_STICKSANDCOLOREDHET) act->setChecked(true);
  act->setData(STYLE_STICKSANDCOLOREDHET);
  g->addAction(act);

  act = styleMenu->addAction(tr("Lines"), this, slot);
  act->setCheckable(true);
  if (current_style == STYLE_LINES) act->setChecked(true);
  act->setData(STYLE_LINES);
  g->addAction(act);
}

void ChemWidget::addSurfStyleMenu(QMenu *styleMenu, int current_style, const char *slot) {
  QActionGroup *g = new QActionGroup(styleMenu);

  QAction *act = styleMenu->addAction(tr("Molecule"), this, slot);
  act->setCheckable(true);
  if (current_style == STYLE_SURF_MOL) act->setChecked(true);
  act->setData(STYLE_SURF_MOL);
  g->addAction(act);

  act = styleMenu->addAction(tr("Accessible"), this, slot);
  act->setCheckable(true);
  if (current_style == STYLE_SURF_WATER) act->setChecked(true);
  act->setData(STYLE_SURF_WATER);
  g->addAction(act);
  
  act = styleMenu->addAction(tr("Custom"), this, slot);
  act->setCheckable(true);
  if (current_style == STYLE_SURF_CUSTOM) act->setChecked(true);
  act->setData(STYLE_SURF_CUSTOM);
  g->addAction(act);
}

void ChemWidget::addCartoonStyleMenu(QMenu *styleMenu, int current_style, const char *slot) {
  QActionGroup *g = new QActionGroup(styleMenu);

  QAction *act = styleMenu->addAction(tr("Thin"), this, slot);
  act->setCheckable(true);
  if (current_style == STYLE_CARTOON_THIN) act->setChecked(true);
  act->setData(STYLE_CARTOON_THIN);
  g->addAction(act);

  act = styleMenu->addAction(tr("Thick"), this, slot);
  act->setCheckable(true);
  if (current_style == STYLE_CARTOON_THICK) act->setChecked(true);
  act->setData(STYLE_CARTOON_THICK);
  g->addAction(act);
}

void ChemWidget::createContextMenu(QTreeWidgetItem *item, int filter, QMenu *menu, const char *slot, bool highlight) {
  int imol = currentRow.imol;
  int grampsID = Gramps::idFromName(currentRow.grampsName);
  //qDebug() << currentRow.rowname << grampsID << item->childCount() << filter;

  QAction *zoomAction = menu->addAction(tr("Zoom"), this, SLOT(zoomMol()));
  zoomAction->setData(filter);
  QAction *centerAction = menu->addAction(tr("Center"), this, SLOT(centerMol()));
  centerAction->setData(filter);
  QAction *midRangeAction = menu->addAction(tr("Midrange"), this, SLOT(midRangeMol()));
  midRangeAction->setData(filter);

  QAction *highlightAction = menu->addAction(tr("Highlight"), this, SLOT(highlightMol()));
  highlightAction->setData(filter);
  if (highlight == false || grampsID == 0) {
      highlightAction->setDisabled(true);
  }
  if (NULL == slot) return;

//  picked atom, or atom row selected
  if (filter == FILTER_ATOM) {
      bool hasRow = treeQuery::isRow(currentRow.imol, currentRow.chain, pickedAtom.atid);
      if (hasRow) {
          QMenu *colorMenu = menu->addMenu(tr("Color"));
          addColorMenu(colorMenu, currentRow.colorBy);
      }
      QMenu *styleMenu = menu->addMenu(tr("Draw"));
      addAtomStyleMenu(styleMenu, currentRow.style, slot);
      menu->addAction(tr("Hbonds"), this, SLOT(showNear()));
      if (hasRow) {
          menu->addAction(tr("Hide"), this, SLOT(hideMol()));
          menu->addAction(tr("Ignore"), this, SLOT(ignoreMol()));
      }
      return;
  }

  if (item->childCount() == 0) {
    //QMenu *surfMenu = menu->addMenu(tr("Surface"));
    //addSurfStyleMenu(surfMenu, slot);
    // actual gramps objects can be redrawn
    if (filter != FILTER_RESIDUE) {
      QMenu *colorMenu = menu->addMenu(tr("Color"));
      addColorMenu(colorMenu, currentRow.colorBy);
    }
    QMenu *styleMenu = menu->addMenu(tr("Draw"));
    if (currentRow.filter == FILTER_WATER) {
      addStyleMenu(styleMenu, currentRow.style, slot);
    } else if (filter == FILTER_RESIDUE) {
      addStyleMenu(styleMenu, STYLE_NONE, slot); // add residue option
    } else if (currentRow.style == STYLE_SURF_MOL || currentRow.style == STYLE_SURF_WATER || currentRow.style == STYLE_SURF_CUSTOM) {
      addSurfStyleMenu(styleMenu, currentRow.style, slot);
    } else if (currentRow.style == STYLE_CARTOON_THIN || currentRow.style == STYLE_CARTOON_THICK) {
      addCartoonStyleMenu(styleMenu, currentRow.style, slot);
    } else {
      if (currentRow.filter != FILTER_TRACE) {
          addHydrogenMenu(styleMenu, currentRow.hydrogens);
          styleMenu->addSeparator();
      }
      //if (currentRow.resnum > 0 || Db::filters[filter].appearance == "T") {
      if (currentRow.resnum != NORESNUM) {
          addMainSideMenu(styleMenu, currentRow.mainSide);
          styleMenu->addSeparator();
      }
      addStyleMenu(styleMenu, currentRow.style, slot);
    }
    QString resLabel;
    //QTextStream(&resLabel) << Db::getAtomResName(currentRow.imol, pickedAtom.atid) << currentRow.resnum;
    QTextStream(&resLabel) << pickedAtom.resnam << currentRow.resnum;
    if (filter != FILTER_RESIDUE || treeQuery::isRow(currentRow.imol, currentRow.chain, resLabel)) {
        menu->addAction(tr("Hide"), this, SLOT(hideMol()));
        menu->addAction(tr("Ignore"), this, SLOT(ignoreMol()));
    }
  }

  if (currentRow.style == STYLE_SURF_MOL || currentRow.style == STYLE_SURF_WATER || currentRow.style == STYLE_SURF_CUSTOM) return;
  int nresidues = mol_query.nresidue;
  if (!item->parent()) {
    // root level
    menu->addAction(tr("Delete"), this, SLOT(deleteMol()));
  } else if (imol && nresidues < 2 && currentRow.style == STYLE_NONE) {
    //  multi mol files can delete single mol, but only put delete menu on mol level (style=none)
    menu->addAction(tr("Delete"), this, SLOT(deleteMol()));
//} else { don't allow deleting chain, LIG, HOH, etc.
  }

  if (item->childCount()) { // containers, e.g. chains, mols in multi-mol sdf, or entire molecule
      QMenu *colorMenu = menu->addMenu(tr("Color"));
      addColorMenu(colorMenu, currentRow.colorBy);
      if (!item->parent() && nresidues < 2) {
          // not for multi-mol file
      } else {
          // surface, filters for entire "file" (root level) when a protein
          menu->addSeparator();
          QAction *act = menu->addAction(tr("Surface"), this, SLOT(addSurfRow()));
          if (treeQuery::isRow(imol,currentRow.chain,"Surface")) act->setDisabled(true);
          if (currentRow.chain != NOCHAIN) addFilterMenus(menu);
      }
  }

}

void ChemWidget::addFilterMenus(QMenu *menu) {
    int imol = currentRow.imol;
    char chain = currentRow.chain;
    QSqlQuery qchain = Db::iterChainCounts(imol, chain);
    QHash<QString, int> counts = Db::nextChainCounts(qchain);

    for (int i=0; i<Db::filters.count(); ++i) {
        if (Db::filters[i].appearance != "M") continue;
        QString name = Db::filters[i].menu;
        int n = counts[name];
        QString rowname = name;
        QString menuname = name;
        QTextStream(&menuname) << " (" << counts[name] << ")";
        QAction *act = menu->addAction(menuname, this, SLOT(addFilterRow()));
        if (n == 0 || treeQuery::isRow(imol, chain, rowname)) act->setDisabled(true);
        //int filterid = Db::filters[i].id;
        act->setData(i);
        //act->setToolTip(counts["name"] + " " + Db::filters[i].tip);
        //menu->setToolTip(counts["name"] + " " + Db::filters[i].tip);
    }
}

void ChemWidget::addFilterRow() {
    int idx = static_cast<QAction *>(QObject::sender())->data().toInt();
    QString name = Db::filters[idx].menu;
    int filter = Db::filters[idx].id;
    char chain = currentRow.chain;
    int imol   = currentRow.imol;
    int resnum = currentRow.resnum;
    //QHash<QString,int> counts = Db::nextChainCounts(Db::iterChainCounts(imol, chain));
    QString rowname = name;
    //QTextStream(&rowname) << name << " (" << counts[name] << ")";
    QTreeWidgetItem *parentItem = currentItem();
    QColor color = currentRow.color;
    if (!color.isValid()) {
        color = COLOR_DEFAULT;
    }
    int style = STYLE_DEFAULT;
    int colorby = COLOR_BY_ATOMS;
    if (name == "Cartoon") {
        style = STYLE_CARTOON_THICK;
        colorby = COLOR_BY_SOLID;
    } else if (name == "Trace") {
        style = STYLE_CYLINDERS;
        colorby = COLOR_BY_SOLID;
    } else if (name == "GLY") {
        style = STYLE_SPHERE;
    }
    setCurrentItem(addMolRow(parentItem, rowname, resnum, Qt::Checked, imol, NOATOM, chain, name, style, colorby, color, filter));
    expandItem(parentItem);
    currentRow.ignore = 1; // so as to prevent the forget command from being issued
    styleMol(style);
}

//void ChemWidget::showRootMenu(const QPoint &pos) {
//    QMenu menu(tr("Molecules"), this);
//    menu.addAction("Molecules");
//    QString filename = "";
//    QSqlQuery qmol = Db::iterMols();
//    for (molRecord mol = Db::nextMol(qmol); mol.valid; mol = Db::nextMol(qmol)) {
//        if (filename != mol.filename) {
//            menu.addSeparator();
//            filename = mol.filename;
//            QStringList parts = filename.split('.');
//            if (parts.count() > 2) {
//                parts.removeLast();
//            }
//            menu.addAction(parts.join("."));
//            menu.addSeparator();
//        }
//        QString s;
//        QTextStream(&s) << mol.imol << "." << mol.title.left(30);
//        menu.addAction(s);
//    }
//    menu.exec(pos);
//}

void ChemWidget::showContextMenu(const QPoint &pos) {
  // when an object is picked in the tree window
  //QMenu menu(tr("Molecule"), this);
  molMenu.reset(new QMenu(tr("Molecule"), this));
  QTreeWidgetItem *item = itemAt(pos);
  if (!item) return;
  int itemid = item->type();
  getCurrentRow(itemid);
  QString label = item->text(0);

//  char chain = Chain(item);
//  if (chain != NOCHAIN) QTextStream(&label) << " " << chain;

  molMenu->addAction(label, this, SLOT(infoMol()));
  molMenu->addSeparator();
  //int filter = (currentRow.iatom == NOATOM) ? FILTER_NONE : FILTER_ATOM;
  int filter = currentRow.filter;
  if (filter == FILTER_ATOM) {
      // simulate this atom being picked; for molCenter
      pickedAtom.get(currentRow.imol, currentRow.iatom);
  }
  createContextMenu(item, filter, molMenu.data(), SLOT(styleMol()), true);
  molMenu->exec(viewport()->mapToGlobal(pos));
}

void ChemWidget::showPickMenu(const QPoint &p, QTreeWidgetItem *item, QString root) {
    // when an object is picked in the gramps gl window
    if (!item) return;
    //QMenu menu(tr("Pick"), this);
    pickMenu.reset(new QMenu(tr("Pick"), this));
    QString label;
    if (currentRow.chain != NOCHAIN) {
        QTextStream(&label) << root << ":" << currentRow.chain;
    } else {
        QString mtitle = currentRow.rowname + ":" + (mol_query.get(currentRow.imol) ? mol_query.title : "");
        if (mtitle.size() > 20) mtitle = mtitle.left(20) + "...";
        //QString mtitle = currentRow.rowname;
        QTextStream(&label) << mtitle;
    }
    pickMenu->addAction(label, this, SLOT(infoMol()));
    pickMenu->addSeparator();

    QString itemLabel;
    itemLabel = item->text(0);
    QMenu *itemMenu = pickMenu->addMenu(itemLabel);
    int filter = (currentRow.iatom == NOATOM) ? currentRow.filter : FILTER_ATOM;
    createContextMenu(item, filter, itemMenu, SLOT(styleMol()), true);

    if (currentRow.iatom == NOATOM && pickedAtom.valid) {
        // let's add more menu items to allow for more graphics objects to be
        // created that might apply to this pick, e.g. a residue, an atom,
        // or any of the defined selections (not yet implemented).
        int resnum = pickedAtom.resnum;
        QString spacer = "  ";
        QString indent = spacer + QChar(0x2514) + QChar(0x2500);
        //if (currentRow.resnum == 0 && resnum) {
        if (resnum != NORESNUM && currentRow.resnum != resnum) {
            // no residue for this item, could want to add the atom's residue, or water.
            QString resLabel;
            //QTextStream(&resLabel) << Db::getAtomResName(currentRow.imol, pickedAtom.atid) << resnum;
            QTextStream(&resLabel) << pickedAtom.resnam << resnum;
            if (currentRow.chain == NOCHAIN || treeQuery::isRow(currentRow.imol, currentRow.chain, resLabel)) {
                QMenu *resMenu = pickMenu->addMenu(indent + resLabel);
                createContextMenu(item, FILTER_RESIDUE, resMenu, NULL, true);
            } else {
                QMenu *resMenu = pickMenu->addMenu(indent + resLabel);
                createContextMenu(item, FILTER_RESIDUE, resMenu, SLOT(addResidue()), false);
            }
            indent = spacer + indent;
        }
        const char* slot;
        if (currentRow.chain == NOCHAIN || treeQuery::isRow(currentRow.imol, currentRow.chain, pickedAtom.atid)) {
            // atom row exists
            slot = NULL;  // just allow Zoom, Center
        } else {
            // atom row could be added
            slot = SLOT(addAtom());
        }
        //QMenu *atomMenu = menu.addMenu(indent + "Atom " + Db::getAtomName(currentRow.imol, pickedAtom.atid));
        QString aname = makePickedAtomName(false);
        //if (pickedAtom.resnum == 0) QTextStream(&aname) << "#" << pickedAtom.atid;
        QMenu *atomMenu = pickMenu->addMenu(indent + tr("Atom ") + aname);
        createContextMenu(item, FILTER_ATOM, atomMenu, slot, false);
    }

    pickMenu->exec(p);
}

void ChemWidget::updateColorIcon(QTreeWidgetItem *item, QColor color, int colorBy) {
    if (item->childCount() > 0) return;
    QColor fcolor = QColor(color.red(), color.green(), color.blue()); // no transparency
    if (color != COLOR_NONE) item->setBackground(COLOR_COLUMN, QBrush(fcolor));
    if (colorBy == COLOR_BY_SOLID) {
        QPixmap px = QPixmap();
        px.fill(fcolor);
        item->setIcon(COLOR_COLUMN, QIcon(px));
    } else if (colorBy == COLOR_BY_ATOMS) {
        item->setIcon(COLOR_COLUMN, QIcon(QPixmap(cpk)));
    } else if (colorBy == COLOR_BY_CHARGE) {
        item->setIcon(COLOR_COLUMN, QIcon(QPixmap(charge)));
    }
}

void ChemWidget::setMolColor(QTreeWidgetItem *item, QColor color, int colorBy) {
    int itemid = item->type();
    if (color != COLOR_NONE) {
        currentRow.updateColor(itemid, color);
        //currentRow.getRow((itemid); //currentRow.color = color;
    }
    if (colorBy != COLOR_BY_NONE) {
        currentRow.updateColorBy(itemid, colorBy);
        //currentRow.getRow(itemid); //currentRow.colorBy = colorBy;
    }
    //  treeRow arow = Db::getTreeRow(itemid);
    //  if (arow.ignore == 0) updateColorIcon(item, color, colorBy);
    updateColorIcon(item, color, colorBy);
}

void ChemWidget::colorMolByAtom() {
    QTreeWidgetItem *item = currentItem();
    colorMolBy(item, COLOR_BY_ATOMS);
}

void ChemWidget::colorMolByCharge() {
    QTreeWidgetItem *item = currentItem();
    colorMolBy(item, COLOR_BY_CHARGE);
}

void ChemWidget::colorMolBySolid() {
    QTreeWidgetItem *item = currentItem();
    colorMolBy(item, COLOR_BY_SOLID);
}

void ChemWidget::colorMolByRandom() {
    QTreeWidgetItem *item = currentItem();
    QColor color = nextColor(currentRow.color);
    colorMol(item, color);
}

void ChemWidget::colorMolByChoose() {
    QTreeWidgetItem *item = currentItem();
    QColor color = QColorDialog::getColor();
    colorMol(item, color);
}

void ChemWidget::colorMolBy(QTreeWidgetItem *item, int colorby) {
    if (item->childCount()) {
        for (int i=0; i< item->childCount(); ++i) {
            QTreeWidgetItem *child = item->child(i);
            colorMolBy(child, colorby);
        }
    }
    getCurrentRow(item);
    setCurrentItem(item);
    setMolColor(item, COLOR_NONE, colorby);
    if (item->checkState(MOL_COLUMN) != Qt::Unchecked) styleMol(STYLE_CURRENT);
}
void ChemWidget::colorMol(QTreeWidgetItem *item, QColor color) {
    if (item->childCount()) {
        for (int i=0; i< item->childCount(); ++i) {
            QTreeWidgetItem *child = item->child(i);
            colorMol(child, color);
        }
    }
    getCurrentRow(item);
    setMolColor(item, color, COLOR_BY_NONE);
    applyColor(currentRow.grampsName, color);
}

QColor ChemWidget::applyColor(QString grampsName, QColor color) {
    QColor acolor;
    if (color.isValid()) {
        acolor = color;
    } else {
        acolor = nextColor(color);
    }
    QString cmd;
    QTextStream(&cmd) << "color " << grampsName << " h," << (acolor.hue() + 240)%360 << " s," <<
                         acolor.saturation()/256. << " v," << acolor.value()/256.;
    emit cmdReady(cmd);
    return acolor;
}

void ChemWidget::colorMolTransparent(bool makeTransparent) {
    //if (makeTransparent) {};
    //QAction *act = static_cast<QAction *>(QObject::sender());
    //qDebug() << "got " <<  act->data();
    //int alpha = act->data().toInt();
    int alpha;
    float transp;
    if (makeTransparent) {
        alpha = 128;
        transp = 0.5;
    } else {
        alpha = 255;
        transp = 0.0;
    }
    //qDebug() << currentRow.rowname << currentRow.color;
    QColor color = currentRow.color;
    color.setAlpha(alpha);
    //qDebug() << currentRow.rowname << currentRow.color;
    QString cmd;
    QTextStream(&cmd) << "inten " << currentRow.grampsName << " t," << transp;
    cmdReady(cmd);
    currentRow.updateColor(currentRow.itemId, color);
    //currentRow.getRow(currentRow.itemId);
}
int ChemWidget::cycleZoom()
{
    // cycle through tree items after double click on background graphics area
    // zooming in on each.
    int itemid = 0;
    if (rootMol->childCount() == 0) return itemid; // no items
    QTreeWidgetItem *item = currentItem();
    if (item) {
        if (item->parent()) {
            if (zoomFilter == FILTER_ATOM && currentRow.iatom == NOATOM) {
                ; // last zoom was to atom, so zoom to currentItem presumably containing this atom
            } else {
            // up to parents
                item = item->parent();
            }
        } else {
            // until a whole molecule, then each molecule in a cycle
            int ichild = rootMol->indexOfChild(item);
            item->setCheckState(MOL_COLUMN, Qt::Checked); // don't let's have an infinite loop
            while (true) {
                // go to next molecule ...
                ichild = (ichild+1) % rootMol->childCount();
                item = rootMol->child(ichild);
                // but only if its checked (visible)
                if (item->checkState(MOL_COLUMN) == Qt::Checked) break;
            }
        }
        itemid = item->type();
    } else {
        // no current item, so start with first molecule
        item = rootMol->child(0);
        itemid = item->type();
    }
    setCurrentItem(item);
    itemid = item->type();
    getCurrentRow(itemid);
    zoomMol(currentRow.filter);
    return itemid;
}

void ChemWidget::zoomMol() {
    int filter = static_cast<QAction *>(QObject::sender())->data().toInt();
    zoomMol(filter);
}
void ChemWidget::zoomMol(int filter) {
    float size = centerMol(filter, false);
    QString cmd;
    QTextStream(&cmd) << "zoom 4," << size;
    emit cmdReady(cmd);
    zoomFilter = filter;
}

float ChemWidget::molCenter(int filter,  float *midpoint,  float *sizes, float *avg) {
    float size = 5;
    int afilter = filter;
    if (afilter == FILTER_ATOM)
    {
        midpoint[0] = pickedAtom.x;
        midpoint[1] = pickedAtom.y;
        midpoint[2] = pickedAtom.z;
        avg[0] = pickedAtom.x;
        avg[1] = pickedAtom.y;
        avg[2] = pickedAtom.z;
        sizes[0] = 1.0;
        sizes[1] = 1.0;
        sizes[2] = 1.0;
        size = 1.0;
    } else {
        int resnum;
        char chain;
        if (afilter == FILTER_RESIDUE) {
            if (currentRow.iatom == NOATOM) {
                resnum = pickedAtom.resnum;
                chain = pickedAtom.chain;
            } else {
                atom_query.get(currentRow.imol, currentRow.iatom);
                resnum = atom_query.resnum;
                chain  = atom_query.chain;
            }
            afilter = FILTER_NONE; // FILTER_RESIDUE not used in db, select using resnum and chain
        } else {
            resnum = currentRow.resnum;
            chain  = currentRow.chain;
        }
        size = Db::molCenter(currentRow.imol, resnum, chain, afilter,  midpoint,  sizes, avg);
    }
    return size;
}

void ChemWidget::centerMol() {
    int filter = static_cast<QAction *>(QObject::sender())->data().toInt();
    centerMol(filter, false);
}
void ChemWidget::midRangeMol() {
    int filter = static_cast<QAction *>(QObject::sender())->data().toInt();
    centerMol(filter, true);
}
float ChemWidget::centerMol(int filter, bool midrange) {
    //QString gramps = currentRow.grampsName;
    QString cmd;
    float midpoint[3];
    float sizes[3];
    float avg[3];
    float size = molCenter(filter,  midpoint,  sizes, avg);
    if (midrange) {
        QTextStream(&cmd) << "center " << -midpoint[0] << "," << -midpoint[1] << "," << -midpoint[2];
    } else {
        QTextStream(&cmd) << "center " << -avg[0] << "," << -avg[1] << "," << -avg[2];        
    }
    emit cmdReady(cmd);
    return std::max(size, (float)5.);
}

void ChemWidget::highlightMol() {
    int filter = static_cast<QAction *>(QObject::sender())->data().toInt();
    QString gramps = currentRow.grampsName;
    if (filter == FILTER_RESIDUE) {
        gramps = encodeMolName(pickedAtom.molid, pickedAtom.chain, pickedAtom.resnum,"");
    }
    if (Gramps::idFromName(gramps) == 0) return;
    QString cmd;
    QTextStream(&cmd) << "highlight " << gramps;
    emit cmdReady(cmd);
}

void ChemWidget::infoMol() {
  // when a user clicks the mol name
  // treeRow currentRow has been retrieved from db

  //QTreeWidgetItem *item = currentItem();
  //int itemid = item->type();
  //if (item->parent()) item->parent()->setSelected(true);

  QString msg;
  //QTextStream(&msg) << item->text(0);
  QTextStream(&msg) << currentRow.rowname;
  QString title;
  QString filename;
  QString style = "\
  <style>\
    td\
      {border-left:1px solid grey;\
       border-top:1px solid grey;}\
    table\
      {border-right:1px solid grey;\
       border-bottom:1px solid grey;}\
  </style>";
  QString props = style + "<table>";
  if (currentRow.imol) {
	  mol_query.get(currentRow.imol);
	  QStringList fparts = mol_query.filename.split(".");
	 if (fparts.size() > 2) fparts.removeLast();
	 filename = fparts.join(".");
	 
	 title = mol_query.title; // currentRow.rowname;
	 props += "<caption><b>" + title + "</b></caption><tr><th>name</th><th>value</th></tr>";
     for ( property_query.iter(currentRow.imol); property_query.next(); ) {
         props += "<tr><td>" + property_query.name.toHtmlEscaped() + "</td><td>" + property_query.text.toHtmlEscaped() + "</td></tr>";
	 }
	 props += "</table>";
  } else {
    title = "NOMOL";
	 props = "no properties";
	 filename = "Unknown";
  }
  QTextStream(&msg) << ":" << title;
  QTextStream(&msg) << ":mol#" << currentRow.imol;
  QTextStream(&msg) << ":chain" << currentRow.chain;
  QTextStream(&msg) << ":residue" << currentRow.resnum;
  if (currentRow.imol) QTextStream(&msg) << "/" <<  mol_query.nresidue;

  QTextStream(&msg) << ":" << currentRow.grampsName;
  if (currentRow.ignore) QTextStream(&msg) << ":ignore";
  emit msgReady(msg);
  emit textWebPage(props, filename);
  
}

void ChemWidget::addAtom() {
    int style = static_cast<QAction *>(QObject::sender())->data().toInt();
    addAtom(style);
}
void ChemWidget::addAtom(int drawstyle) {
    QTreeWidgetItem *item = currentItem();
    QColor color = COLOR_NONE;
    char chain     = currentRow.chain;
    int imol       = currentRow.imol;
    int filter     = FILTER_ATOM; //currentRow.filter;
    int iatom      = pickedAtom.atid; //currentRow.iatom;
    //int colorBy    = currentRow.colorBy;
    int colorBy    = COLOR_BY_ATOMS;
    //int resnum = Db::getAtomResNum(imol, iatom);
    QString text = makePickedAtomName(true);
    QString suffix;
    //QString atomname = pickedAtom.name; //Db::getAtomName(imol, iatom);
    //QTextStream(&text) << Db::getAtomResName(imol, iatom) << resnum << ":" << atomname;
    if (pickedAtom.resnum != NORESNUM) {
        suffix = pickedAtom.name;
    } else {
        suffix = text;
    }
    setCurrentItem(addMolRow(item->parent(), text, pickedAtom.resnum, Qt::Unchecked, imol, iatom, chain,  suffix, drawstyle, colorBy, color, filter));
    styleMol(drawstyle);
}

void ChemWidget::addResidue() {
    // add a single residue after mouse pick
    int style = static_cast<QAction *>(QObject::sender())->data().toInt();
    addResidue(style);
}
void ChemWidget::addResidue(int drawstyle) {
    // add a single residue after mouse pick
    QTreeWidgetItem *item = currentItem();
    treeQuery parentRow = treeQuery();
    parentRow.getRow( item->parent()->type() );
    QColor color = parentRow.color;
    char chain     = currentRow.chain;
    int imol       = currentRow.imol;
    int filter     = currentRow.filter;
    int colorBy    = COLOR_BY_ATOMS;
    if (filter == FILTER_WATER) {
        //filter = FILTER_WATER;
    } else if (filter == FILTER_ATOM) {
        //filter = FILTER_ATOM;
    } else {
        if (pickedAtom.resnam == "HOH" || pickedAtom.resnam == "WAT") {
            filter = FILTER_WATER;
        } else {
            // add side chain to picked trace, main, surface, or other sibling
            filter = FILTER_SIDE;
            // could be main+side, selectable on mainSideMenu
            //filter = FILTER_CHAIN;
        }
    }
    QTreeWidgetItem *parentItem;
    //QString text;
    QString rowname;
    QTextStream(&rowname) <<  pickedAtom.resnam << pickedAtom.resnum;
    if (chain == pickedAtom.chain) {
        parentItem = item->parent(); // the chain
    } else {
        int parentid;
        if (chain == NOCHAIN) {
            // picked via non-chain item, like surface of entire mol
            parentItem = item->parent(); // the molecule
            parentid = parentItem->type();
        } else {
            parentItem = item->parent(); // the chain
            parentid = parentItem->parent()->type(); // the molecule
        }
        chain = pickedAtom.chain; // allow drawing of atoms in "other" chain to this item
        treeQuery sibling = treeQuery();
        sibling.getSibling(parentid, chain);  // the other chain
        parentItem = getGrampsItem(sibling.grampsName); // parent item of other chain
    }
    setCurrentItem(addMolRow(parentItem, rowname, pickedAtom.resnum, Qt::Unchecked, imol, NOATOM, chain, "", drawstyle, colorBy, color, filter));
    styleMol(drawstyle);
}

void ChemWidget::showHydrogens(bool state) {
    QTreeWidgetItem *item = currentItem();
    int itemid = item->type();
    int hstate = (state == true) ? HYDROGEN_SHOW : HYDROGEN_HIDE;
    currentRow.updateHydrogens(itemid, hstate);
    //treeQuery.getRow(itemid);
    styleMol(currentRow.style);
}
void ChemWidget::showMainSide(bool state) {
    QTreeWidgetItem *item = currentItem();
    int mainSide;
    int filter;
    if (state) {
        mainSide = 1;
        filter = FILTER_CHAIN; // all atoms, i.e. main+side
    } else {
        mainSide = 0;
        filter = FILTER_SIDE;
    }
    int itemid = item->type();
    currentRow.updateMainSide(itemid, mainSide, filter);
    //currentRow.getRow(itemid);
    styleMol(currentRow.style);
}
void ChemWidget::styleMol() {
    int style = static_cast<QAction *>(QObject::sender())->data().toInt();
    styleMol(style);
}
void ChemWidget::styleMol(int drawstyle) {
    //qDebug() << selectionMode();
    QTreeWidgetItem *item = currentItem();
    if (item == NULL) return;
    bool sameStyle = true;
    if (drawstyle != STYLE_CURRENT) {
        int itemid = item->type();
        currentRow.updateStyle(itemid, drawstyle);
        //currentRow.getRow(itemid);
        sameStyle = false;
    }
    if (!item->parent()) return; // a first-level molecule (gramps group)

    if (currentRow.style == STYLE_NONE) {
        // a group
    } else {
        if (currentRow.ignore == 0) {
            QString cmd = "forget " + currentRow.grampsName;
            emit cmdReady(cmd);
        }
        QString molnam;
        if (currentRow.iatom != NOATOM && (currentRow.style == STYLE_ATOM_VDW || currentRow.style == STYLE_ATOM_SPHERE || currentRow.style == STYLE_ATOM_BALL)) {
            molnam = drawOneAtom(item);
        } else  if (currentRow.style == STYLE_SURF_MOL || currentRow.style == STYLE_SURF_WATER || currentRow.style == STYLE_SURF_CUSTOM) {
            if (sameStyle && currentRow.ignore == 0) {
                drawSurface(currentRow.itemId);
                molnam = currentRow.grampsName;
            } else {
                makeSurface();
                //colorMolTransparent(true);
                return;
            }
        } else {
            molnam = drawMol(item);
        }
        QString pname;
        if (!molnam.isNull()) {
            //treeRow parentRow = Db::getTreeRow(item->parent()->type());
            treeQuery parentRow = treeQuery();
            parentRow.getRow(currentRow.parentId);
#ifdef DEBUG
            qDebug() << item->type() << currentRow.itemId << currentRow.grampsName << molnam << parentRow.grampsName << Gramps::idFromName(parentRow.grampsName);
#endif
            insertOrGroup(molnam, parentRow);
        }
    }

    item->setCheckState(MOL_COLUMN, Qt::Checked);
    blockSignals(true); // just change check boxes, don't act on children
    adjustParents(item);
    blockSignals(false);
}

void ChemWidget::insertOrGroup(QString molnam, treeQuery parentRow) {
    //
    QString cmd;
    if (Gramps::idFromName(parentRow.grampsName) == 0) {
        cmd = "group " + molnam + "," + molnam + " " + parentRow.grampsName;
        emit cmdReady(cmd);
        treeQuery grandParent = treeQuery();
        grandParent.getRow(parentRow.parentId);
        cmd = "insert " + parentRow.grampsName + " " + grandParent.grampsName;
        emit cmdReady(cmd);
    } else {
        cmd = "insert " + molnam + " " + parentRow.grampsName;
        emit cmdReady(cmd);
    }
}

void ChemWidget::hideMol() {
    QTreeWidgetItem *item = currentItem();
    item->setCheckState(MOL_COLUMN, Qt::Unchecked);
}

void ChemWidget::ignoreMol() {
    QTreeWidgetItem *item = currentItem();
    if (item->childCount() == 0  && !currentRow.ignore) {
        QString cmd = "forget " + currentRow.grampsName;
        emit cmdReady(cmd);
    }
    int itemid = item->type();
    currentRow.updateIgnore(itemid, true);
    //currentRow.getRow(itemid); //currentRow.ignore = 1;
    item->setCheckState(MOL_COLUMN, Qt::Unchecked);
    blockSignals(true); // just change check boxes, don't act on children
    adjustParents(item);
    blockSignals(false);
}

void ChemWidget::deleteMol() {
    QTreeWidgetItem *item = currentItem();
    item->setCheckState(MOL_COLUMN, Qt::Unchecked);
    adjustParents(item);
    if (!currentRow.ignore) {
        QString cmd = "forget " + currentRow.grampsName;
        emit cmdReady(cmd);
    }
    Db::deleteFromTables(currentRow.imol);
    delete item;
}

void ChemWidget::doubleClicked(QTreeWidgetItem *item, int /* col */) {
    //qDebug() << "doubleClicked " << actions();
    // double clicked on tree
    //if (col > MOL_COLUMN) return;
    int itemid = item->type();
    getCurrentRow(itemid);
    if (currentRow.iatom != NOATOM) {
        // for centerMol to use
        pickedAtom.get(currentRow.imol,currentRow.iatom);
    }
    zoomMol(currentRow.filter);
}
//void ChemWidget::pressed(QTreeWidgetItem *item, int col) {
//  qDebug() << "pressed" << item->checkState(MOL_COLUMN);
//}

void ChemWidget::clicked(QTreeWidgetItem *item, int /* col */) {
  setCurrentItem(item);
//  if (col > MOL_COLUMN) {
//      return;
//  }
  int itemid = item->type();
  getCurrentRow(itemid);
  Qt::CheckState state = item->checkState(MOL_COLUMN);
  if (state == Qt::Checked) {
    if (currentRow.style == STYLE_NONE) { // group
      showOneItem(item); // changed currentRow and setcurrentItem()
    }
    if (currentRow.ignore) styleMol(currentRow.style);
  }
}

void ChemWidget::changed(QTreeWidgetItem *item, int col) {
  if (col > MOL_COLUMN)  return;  // not interested in other columns
  int itemid = item->type();
  getCurrentRow(itemid);
  if (currentRow.ignore) return;
  Qt::CheckState state = item->checkState(MOL_COLUMN);
  blankOrDeblank(state, currentRow.grampsName);
  if (state == Qt::Unchecked) {
      //if (item->childCount() == 0) item->setCheckState(MOL_COLUMN, Qt::PartiallyChecked);
      item->setCheckState(MOL_COLUMN, Qt::PartiallyChecked);
  }
  blockSignals(true); // just change check boxes, don't act on children
  adjustChildren(item, state);
  adjustParents(item);
  blockSignals(false);
}

void ChemWidget::blankOrDeblank(Qt::CheckState state, QString grampsName) {
    QString cmd;  // goal is to generate appropriate gramps command
  if (state == Qt::Checked) {
    QTextStream(&cmd) << "deblank " << grampsName;
  } else if (state == Qt::Unchecked) {
    QTextStream(&cmd) << "blank " << grampsName;
  } else {
  }
  if (!cmd.isNull()) emit cmdReady(cmd);
}

void ChemWidget::showOneItem(QTreeWidgetItem *item) {
    //  used when an (group) item is clicked to open that item and show the first item.
    //   this is useful when a molecule in an sd file is clicked, but the model has not
    //   yet been shown.

  expandItem(item);
  int nChecked = 0;
  for (int i=0; i< item->childCount(); ++i) {
    QTreeWidgetItem *child = item->child(i);
    if (child->checkState(MOL_COLUMN) == Qt::Checked) ++nChecked;
  }
  if (nChecked) {
  // let the checked item(s) be shown - no need to interfere
  } else {
  int itemid = item->type();
  currentRow.updateIgnore(itemid, false);
  // show the first item
    for (int i=0; i< item->childCount(); ++i) {
      QTreeWidgetItem *child = item->child(i);
      if (child->checkState(MOL_COLUMN) == Qt::Unchecked) {
        child->setCheckState(MOL_COLUMN, Qt::Checked);
        setCurrentItem(child);
        int childItemId = child->type();
        getCurrentRow(childItemId);
        return;
      }
    }
  }
}

void ChemWidget::adjustParents(QTreeWidgetItem *item) {
  QTreeWidgetItem *parentItem = adjustParent(item);
  if (parentItem) adjustParents(parentItem);
  return;
}

QTreeWidgetItem *ChemWidget::adjustParent(QTreeWidgetItem *item) {
  // adjust check marks of parentItem when those of child(ren) change
  QTreeWidgetItem *parentItem = item->parent();
  if (parentItem) {
    int nChecked = 0;
    int nUnchecked = 0;
    int nPartial = 0;
    for (int i=0; i< parentItem->childCount(); ++i) {
      QTreeWidgetItem *child = parentItem->child(i);
      if (child->checkState(MOL_COLUMN) == Qt::Checked) ++nChecked;
      if (child->checkState(MOL_COLUMN) == Qt::Unchecked) ++nUnchecked;
      if (child->checkState(MOL_COLUMN) == Qt::PartiallyChecked) ++nPartial;
    }
    if (nChecked > 0) {
      parentItem->setCheckState(MOL_COLUMN, Qt::Checked);
    } else if (nPartial > 0) {
      parentItem->setCheckState(MOL_COLUMN, Qt::PartiallyChecked);
    } else {
      parentItem->setCheckState(MOL_COLUMN, Qt::Unchecked);
    }

//    if (nChecked == nChildren) {
//      parentItem->setCheckState(MOL_COLUMN, Qt::Checked);
//    } else if (nUnchecked == nChildren) {
//      parentItem->setCheckState(MOL_COLUMN, Qt::Unchecked);
//    } else {
//      parentItem->setCheckState(MOL_COLUMN, Qt::PartiallyChecked);
//    }

  }
  return parentItem;
}

int ChemWidget::adjustChildren(QTreeWidgetItem *item, Qt::CheckState state) {

//   make children conform to parentItem's check state.
//   PartiallyChecked means it's blanked for now, but will blank/deblank with parentItem.

  int nChecked = 0;
  for (int i=0; i< item->childCount(); ++i) {
    QTreeWidgetItem *child = item->child(i);
    //qDebug() << child->text(0) << child->checkState(MOL_COLUMN);
    //adjustChildren(child, child->checkState(MOL_COLUMN));
    int kidsChecked = adjustChildren(child, state);
    if (state == Qt::Unchecked) {
      if (child->checkState(MOL_COLUMN) == Qt::Checked) child->setCheckState(MOL_COLUMN, Qt::PartiallyChecked);
      if (child->checkState(MOL_COLUMN) == Qt::Checked && kidsChecked == 0) child->setCheckState(MOL_COLUMN, Qt::Unchecked);
    } else if (state == Qt::Checked) {
      if (child->checkState(MOL_COLUMN) == Qt::PartiallyChecked || kidsChecked) child->setCheckState(MOL_COLUMN, Qt::Checked);
    } else {
    }
    if (child->checkState(MOL_COLUMN) == Qt::Checked) ++nChecked;
  }
  return nChecked;
}

QTreeWidgetItem * ChemWidget::setCheckedRows() {
    QTreeWidgetItem *first = NULL;
    QTreeWidgetItemIterator it(this);
    while (*it) {
        int itemid = (*it)->type();
        treeQuery t = treeQuery();
        t.getRow(itemid);
        if (first == NULL && t.checked == Qt::Checked) first = *it;
        (*it)->setCheckState(MOL_COLUMN, (Qt::CheckState)t.checked);
        if (t.ignore == 0 && t.checked != Qt::Checked) {
            emit cmdReady("blank " + t.grampsName);
#ifdef DEBUG
            qDebug() << "blank" << t.grampsName << itemid;
#endif
        }
        ++it;
    }
    return first;
}

void ChemWidget::updateCheckedRows() {
    QTreeWidgetItemIterator it(this);
    while (*it) {
        int itemid = (*it)->type();
        Qt::CheckState state = (*it)->checkState(MOL_COLUMN);
        //qDebug() << itemid << state;
        currentRow.updateChecked(itemid, state);
        ++it;
    }
}

void ChemWidget::readMol(const QString filename, const QString fmt) {
    addMolToDb(filename, fmt);
}

int ChemWidget::addMolToDb(QString filename, QString fmt) {
#ifdef __APPLE__
  //QString prog = gmolLib + "/dbchem";
  QString prog = QCoreApplication::applicationDirPath() + "/dbchem";
#else
  QString prog = "./dbchem";
#endif
  QStringList arguments;
  //db.close();
  //int fsize = QFileInfo(filename).size();
#ifdef PDBREADER
  if (fmt == "pdb") {
    progress->setWindowTitle(QFileInfo(filename).fileName());
    QString label;
    QTextStream(&label) << tr("Reading...");
    progress->setLabelText(label);
    int imol = Db::readPDB(filename);
    open(imol, true);
    emit molAdded(imol);
    progress->cancel();
  } else {
#endif
      //QSqlDatabase db = QSqlDatabase::database()
    QString dbfile = QSqlDatabase::database().databaseName();
    if (fmt.isNull() || fmt.size() == 0) {
      arguments << dbfile << filename;
      emit msgReady("empty file format");
    } else {
      //QProcess dbproc;
      //while (dbproc.waitForFinished()) {}
      //QSqlQuery query;
      //query.exec("Pragma foreign_keys");
      //query.next();
      //qDebug() << "readMol: foreign keys" << query.value(0).toInt();
      progress->setWindowTitle(QFileInfo(filename).fileName());
      QString label;
      QTextStream(&label) << tr("Reading...");
      progress->setLabelText(label);
      arguments << dbfile;
      //if (fsize < 1000000) arguments << "-f"; // force use of openBabel
      arguments << "-t" << fmt << filename;
      //arguments << dbfile << filename;
      dbproc->start(prog, arguments);
#ifdef DEBUG
      qDebug() << "started" << dbproc << prog << arguments;
#endif
    }
#ifdef PDBREADER
  }
#endif
  //db.open();
  return 0;
}

void ChemWidget::readProc() {
  QStringList msg;
  if (dbproc->state() == QProcess::Running) {
    msg = QString(dbproc->readAllStandardOutput()).split(' ');
  } else if (surfproc->state() == QProcess::Running) {
    msg = QString(surfproc->readAllStandardOutput()).split(' ');
  }
  // Proc outputs 
  // type # at nnnn of NNNN
  // e.g. Model # at nnnn of NNNN
  if (msg.size() < 6) return;

  QString type = msg[0];
  int mnum  = msg[1].toInt();
  QString label;
  if (mnum > 0) {
    QTextStream(&label) << type << " " << mnum;
    int atloc = msg[3].toInt();
    int fsize = msg[5].toInt();
    progress->setMaximum(fsize);
    progress->setValue(atloc);
    progress->setLabelText(label);
  } else if (mnum == 0) {
    QTextStream(&label) << type << "...";
    int atloc = msg[3].toInt();
    int fsize = msg[5].toInt();
    progress->setMaximum(fsize);
    progress->setValue(atloc);
    progress->setLabelText(label);
  }
}

void ChemWidget::molReady(int imol) {
  //qDebug() << "molReady" << imol;
  if (imol) {
    //QSqlQuery query;
    //query.exec("Pragma foreign_keys");
    //query.next();
    //qDebug() << "molReady: foreign keys" << query.value(0).toInt();
    //Db::createTreeTable();
    //progress->cancel();
    progress->setLabelText(tr("Preparing view"));
    progress->setMaximum(100);
    progress->setValue(80);
    open(imol, true);
    emit molAdded(imol);
    //Db::attachResidue();
    progress->cancel();
    parentWidget()->show();
    parentWidget()->raise();
  } else {
      emit msgReady("no molecules read from file");
  }
}

//#include "chem_cartoon.cpp"
//#include "chem_chain.cpp"
//#include "chem_surface.cpp"

void ChemWidget::restore() {
    // redraw entries in the database table tree, creating TreeWidget items and gramps objects.

 // create new query items for this newly opened db
	atom_query = atomQuery();
	pickedAtom = atomQuery();
	mol_query = molQuery();
	property_query = propertyQuery();
    currentRow = treeQuery();
    
    restoreOrientation();

    // top level molQuery for scope of this restore loop
    molQuery mquery = molQuery();
    for (mquery.getByFile(); mquery.next(); ) {
#ifdef DEBUG
        qDebug() << "Restore #" << mquery.imol << mquery.title << mquery.filename;
#endif
		// the standard items opened when file is read
        open(mquery.imol, false);
	}
	// items added during the session; surfaces, residues, atoms, cartoons, etc.
    for (currentRow.iterRestore(); currentRow.next(); ) {
        if (currentRow.checked == Qt::Unchecked) {
            currentRow.ignore = 1; // no need to redraw if hidden; pretend igonored.
        }
		int id = Gramps::idFromName(currentRow.grampsName);
		if (id == 0) {
#ifdef DEBUG
			qDebug() << "Restore #" << currentRow.itemId << currentRow.rowname << currentRow.grampsName;
#endif
			QTreeWidgetItem *item = getGrampsItem(currentRow.grampsName);
			if (item) {
				// has a row entry, just draw the gramps object, unless it's ignored
				if (currentRow.ignore == 0) {
#ifdef DEBUG
					qDebug() << "item #" << item->type();
#endif
                    currentRow.ignore = 1;  // else we think it's already been drawn (not ignored)                  
					setCurrentItem(item);
					styleMol(currentRow.style);
					setMolColor(item, currentRow.color, currentRow.colorBy);
				} else {
					item->setCheckState(MOL_COLUMN, Qt::Unchecked);
				}
			} else {
				// user-added items need a row entry and possibly a gramps object
                treeQuery parentRow = treeQuery();
                parentRow.getRow(currentRow.parentId);
				QTreeWidgetItem *parentItem = getGrampsItem(parentRow.grampsName);
				if (parentItem) {
					currentRow.ignore = currentRow.ignore;
                    Qt::CheckState cstate = (Qt::CheckState) currentRow.checked;
					setCurrentItem(makeMolRow(parentItem, currentRow.grampsName, currentRow.rowname, cstate)); //Qt::Checked));
					setMolColor(currentItem(), currentRow.color, currentRow.colorBy);
#ifdef DEBUG
					qDebug() << "parent" << parentRow.rowname << currentRow.ignore;
#endif
					if (currentRow.ignore == 0) {
						currentRow.ignore = 1;  // so as to prevent the forget command from being issued
						if (currentRow.style == STYLE_SURF_WATER || currentRow.style == STYLE_SURF_MOL || currentRow.style == STYLE_SURF_CUSTOM) {
							// no need to re-compute surface, just draw from db tables
							drawSurface(currentRow.itemId);
							insertOrGroup(currentRow.grampsName, parentRow);
                            //color.setAlpha(128);
                            //setMolColor(currentItem(), currentRow.color, currentRow.colorBy);
                            applyColor(currentRow.grampsName, currentRow.color);
                            if (currentRow.color.alpha() == 255) {
                                colorMolTransparent(false);
                            } else {
                                colorMolTransparent(true);
                            }
						} else {
							styleMol(currentRow.style);
						}
					} else {
                        currentRow.ignore = 1;
						// objects was ignored, so don't draw it and uncheck its row
						currentItem()->setCheckState(MOL_COLUMN, Qt::Unchecked);
					}
				} else {
					qDebug() << tr("can't locate parent item") << parentRow.grampsName;
				}
			}
		} // end of id=0: gramps objects that don't exist yet
	} // end of iterRestore
	
    //restoreOrientation();
    setCurrentItem(setCheckedRows());         
}
void ChemWidget::restoreOrientation() {
    QStringList commands = Db::getGrampsSave();
    if (commands.length() > 0) {
        //emit cmdReady("pause");
        for (int i=0; i<commands.length(); ++i) {
            emit cmdReady(commands[i]); //.replace("$1", "50"));
        }
        //emit cmdReady("continue");
    } else {
        cycleZoom(); // will cause cycleZoom pick first molecule
    }
    emit cmdReady("usemouse LWorld");
}

//int ChemWidget::open(QString filename) {
// read db for molecular structure(s) from filename.

int ChemWidget::open(int molid, bool centerZoom) {
    //   read db for molecular structure(s) for molid;
    //     create rows in the tree
    //     to represent the whole file. it's components may be separate molecules
    //     or chains/residues/LIG/HOH for proteins.
    
    QTreeWidgetItem *parentItem = rootMol; // the invisible root of the tree where all mols are rooted
    int err = 0;
    float midpoint[3] = {0,0,0};
    float sizes[3];
    float avg[3];
    float size = 1.0;
    QString cmd; // gramps group command to emit when done
    QString fname; // used outside loop to report possible error
    
    Db::checkResidues(molid);
    
    // file may contain multiple molecules, so iterMolsInFile/nextMol returns all mols that have same file as this molid
    int amol=0; // count # of mols, in order to deal with first mol as a special case
    QColor color = COLOR_DEFAULT;
    color = nextColor(color);
    //QSqlQuery qmol = Db::iterMolsinFile(molid);
    //for (molRecord mol = Db::nextMol(qmol); mol.valid; mol = Db::nextMol(qmol)) {
    for (mol_query.getInFile(molid); mol_query.next(); ) {
        fname = mol_query.filename; // used outside loop to report possible error
        QStringList fparts = fname.split("."); // tmpfile from web.cpp may have third part name we don't show
        if (fparts.count() > 2) fparts.removeLast();
        QString fshort = fparts.join(".");
        QString rownam = mol_query.title;
        if (rownam.isNull() || rownam.length() == 0) QTextStream(&rownam) << tr("Molecule #") << mol_query.imol;
        //bool protein = (Db::molNumRes(mol.imol) > 1);
        bool pdb = (mol_query.type.compare("pdb", Qt::CaseInsensitive) == 0);
        if (amol == 0) {
            // first mol
            size = Db::molCenter(mol_query.imol, NORESNUM, NOCHAIN, FILTER_NONE, midpoint, sizes, avg);
            char chain = NOCHAIN;
            if (pdb && mol_query.nresidue == 1) {
                //atomRecord a = Db::getAtom(mol.imol, 1);
                atom_query.get(mol_query.imol, 1);
                if (atom_query.valid) chain = atom_query.chain;
            }
            parentItem = addMolRow(rootMol, fshort, NORESNUM, Qt::Checked, mol_query.imol, NOATOM, chain, fparts[0], STYLE_NONE, COLOR_BY_NONE, COLOR_DEFAULT, FILTER_NONE);
            QString MOLname = currentRow.grampsName;
            if (mol_query.nresidue > 1) {
                QList<QString>gnames = addChainsRow(parentItem, mol_query.imol);
                //addMolRow(parentItem, "Surface", NORESNUM, Qt::Unchecked, mol.imol, NOCHAIN, "surface", STYLE_SURF_MOL, COLOR_NONE, FILTER_MAIN+FILTER_SIDE);
                //QTextStream(&cmd) << "," << MOLname; // finish off command with group name
                if (gnames.count() > 0) {
                    QTextStream(&cmd) << "group " << gnames.first() << "," << gnames.last() << "," << MOLname;
                } else {
                    err = -1;
                }
            } else {
                int style = STYLE_DEFAULT;
                //if (Db::molNumAtoms(mol.imol) > 200) style = STYLE_LINES;
                if (mol_query.natoms > 200) style = STYLE_LINES;
                // STYLE_NONE for this group parent row
                QTreeWidgetItem *molRow = addMolRow(parentItem, rownam, NORESNUM, Qt::Checked, mol_query.imol, NOATOM, NOCHAIN, "", STYLE_NONE, COLOR_BY_NONE, color, FILTER_NONE);
                QString molnam = currentRow.grampsName;
                QTreeWidgetItem *modelRow = addMolRow(molRow, "Model", NORESNUM, Qt::Checked, mol_query.imol, NOATOM, NOCHAIN, "model", style, COLOR_BY_ATOMS, color, FILTER_NONE);
                QString modelnam = drawMol(modelRow);
                //addMolRow(molRow, "Surface", NORESNUM, Qt::Unchecked, mol.imol, NOCHAIN, "surface", STYLE_SURF_MOL, COLOR_NONE, FILTER_NONE);
                QTextStream(&cmd) << "group " << modelnam << "," << modelnam << "," << molnam;
                emit cmdReady(cmd);
                emit cmdReady("rebuild " + modelnam);  // workaround for odd bug that prevents first obj from being shown
                cmd.clear();
                QTextStream(&cmd) << "group " << molnam << "," << molnam << "," << MOLname;
            }
            
        } else {
            // extra mols are added to tree, but not yet sent to gramps
            int style = STYLE_DEFAULT;
            //if (Db::molNumAtoms(mol.imol) > 100) style = STYLE_LINES;
            if (mol_query.natoms > 100) style = STYLE_LINES;
            color.setHsv(color.hue() + 30, color.saturation(), color.value());
            QTreeWidgetItem *molRow = addMolRow(parentItem, rownam, NORESNUM, Qt::Unchecked, mol_query.imol, NOATOM, NOCHAIN, "", STYLE_NONE, COLOR_BY_NONE, color, FILTER_NONE); // the group for this  mol
            addMolRow(molRow, "Model", NORESNUM, Qt::Unchecked, mol_query.imol, NOATOM, NOCHAIN, "model", style, COLOR_BY_ATOMS, color, FILTER_NONE);
            //addMolRow(molRow, "Surface", NORESNUM, Qt::Unchecked, mol.imol, NOCHAIN, "surface", STYLE_SURF_MOL, COLOR_NONE, FILTER_NONE);
        }
        ++amol;
    }
    
    setAnimated(false); // expand generates warnings unless animation off
    expandItem(parentItem);
    //setFirstItemColumnSpanned(parentItem,true);
    setAnimated(true);
    if (err == 0) {
        emit cmdReady(cmd);
        if (centerZoom) {
            cmd.clear();
            QTextStream(&cmd) << "center " << -avg[0] << "," << -avg[1] << "," << -avg[2];
            emit cmdReady(cmd);
            cmd.clear();
            QTextStream(&cmd) << "zoom 4," << size;
            emit cmdReady(cmd);
        }
    } else {
        //qDebug() << tr("error #") << err << tr("while reading") << filename;
        QString msg;
        QTextStream(&msg) << tr("error #") << err << tr("while processing") << fname;
        emit msgReady(msg);
    }
    resizeColumnToContents(1);
    resizeColumnToContents(0);
    return err;
}

QString ChemWidget::grampsSafe(QString name) {
  // What's in a name?
  // well, gramps doesn't allow blank or comma in a name, but anything else is fine.
  name.replace(" ","_");
  name.replace(",","_");
  if (name.length() > 20) return name.left(20) + "...";
  return name;
}

QString ChemWidget::encodeMolName(int imol, char chain, int resnum, QString suffix) {
//  construct a unique, yet informative name to be used for gramps object
  QString mname;
  QTextStream(&mname) << "mol" << imol;
  if (chain != NOCHAIN && chain != ' ') {
  // a chain
    QTextStream(&mname) << "." << chain;
  }
  if (resnum != NORESNUM) {
    // a residue in that chain 
    QString resnam  = atom_query.getResnam(imol, resnum, chain);
    QTextStream(&mname) << "." << resnam;
    //if (resnam != "HOH") QTextStream(&mname) << resnum << ".";
    QTextStream(&mname) << resnum << ".";
    if (suffix.length()) {
        QTextStream(&mname) << suffix;
    } else {
        QTextStream(&mname) << "Side" ;
    }

  } else {
    if (suffix.length()) {
     // a whole molecule or chain, for a group name
      QTextStream(&mname) << "." << suffix;
    } else {
      // a molecule (say from a sdf/molfile)
      if (imol) QTextStream(&mname) << "." << mol_query.title << "." << imol;
    }
  }
  return grampsSafe(mname);
}

int ChemWidget::drawBond(bondQuery bond, bool pairs, int colorBy, float *spec) {
  int err = 0;

//     draw the selected bonds, colored perhaps.  bool pairs lets me know whether
//     this object was started with a continuous, move-draw-move-draw-line of nbonds,
//     or if I should draw each bond/half-bond explicitely as a new single line.

  int size;
  float coord[6];

  coord[0] = bond.ax; coord[1] = bond.ay; coord[2] = bond.az;
  if (pairs) {
    bool halfs = false;
    if (colorBy == COLOR_BY_ATOMS) {
      setAtomColor(bond.anum);
      if (bond.anum != bond.bnum) halfs = true;  // half bonds of atom colors
    } else if (colorBy == COLOR_BY_CHARGE) {
      setChargeColor(bond.acharge);
      if (bond.acharge != bond.bcharge) halfs = true;
    }
    if (bond.ares != bond.bres) halfs = true;
    size = 3; err = getMemMore("", spec, size, 0);
    if (halfs) {
      coord[3] = bond.ax + (bond.bx - bond.ax)/2.;
      coord[4] = bond.ay + (bond.by - bond.ay)/2.;
      coord[5] = bond.az + (bond.bz - bond.az)/2.;
      // draw to the one half
      size = 6; err = getMemMore("", coord, size, 0);
      // draw from other atom to the half
      if (colorBy == COLOR_BY_ATOMS) {
          setAtomColor(bond.bnum);
      } else if (colorBy == COLOR_BY_CHARGE) {
        setChargeColor(bond.bcharge);
     }
      size = 3; err = getMemMore("", spec, size, 0);
      coord[0] = bond.bx; coord[1] = bond.by; coord[2] = bond.bz;
    } else {
      coord[3] = bond.bx; coord[4] = bond.by; coord[5] = bond.bz;
    }
  } else {
    coord[3] = bond.bx; coord[4] = bond.by; coord[5] = bond.bz;
  }
  size = 6; err = getMemMore("", coord, size, 0);

  return err;
}

QString ChemWidget::drawOneAtom(QTreeWidgetItem *item) {
    QString grampsName  = currentRow.grampsName;
    int resnum     = currentRow.resnum;
    char chain     = currentRow.chain;
    int imol       = currentRow.imol;
    int style      = currentRow.style;
    int colorBy    = currentRow.colorBy;
    int iatom      = currentRow.iatom;
    int hydrogens  = currentRow.hydrogens;
    int filter     = FILTER_ATOM; // currentRow.filter;
    QColor color   = currentRow.color;
    float radius;
    switch (style) {
    case (STYLE_ATOM_SPHERE):
        radius = ATOM_SPHERE_RADIUS;
        break;
    case (STYLE_ATOM_BALL):
        radius = ATOM_BALL_RADIUS;
        break;
    case (STYLE_ATOM_VDW):
        radius = ATOM_VDW_RADIUS;
        break;
    }
    int err = spheres(imol, iatom, grampsName, resnum, chain, colorBy, 0, filter, hydrogens, radius, ATOM_VDW_RADIUS);
    if (err) {
        grampsName.clear();
    } else {
        int itemid = item->type();
        //currentRow.updateIgnore(itemid, false);
        currentRow.updateIgnore(itemid, false);
        //currentRow.getRow(itemid); //currentRow.ignore = 0;         
        color = applyColor(grampsName, color);
        setMolColor(item, color, colorBy);
        //updateColorIcon(item, color, colorBy);
    }
    return grampsName;
}

QString ChemWidget::drawMol(QTreeWidgetItem *item) {
  QString grampsName  = currentRow.grampsName;
  int resnum     = currentRow.resnum;
  char chain     = currentRow.chain;
  int imol       = currentRow.imol;
  int style      = currentRow.style;
  int colorBy    = currentRow.colorBy;
  int hydrogens  = currentRow.hydrogens;
  int filter     = currentRow.filter;
  QColor color   = currentRow.color;

  // special case for water
  //if (HAS_BIT(filter, FILTER_WATER) && style == STYLE_STICKSANDCOLOREDHET) style = STYLE_BALLANDSTICK;
  if (filter == FILTER_WATER && style == STYLE_STICKSANDCOLOREDHET) style = STYLE_BALLANDSTICK;

  int err = 0;
  //if (HAS_BIT(filter, FILTER_TRACE)) {
  if (filter == FILTER_TRACE || filter == FILTER_CARTOON) {
    switch (style) {
    case (STYLE_LINES):
      err = drawChain(imol, grampsName, chain, DRAW_OPEN+DRAW_DRAW+DRAW_CLOSE, 0, 0, false);
      break;
    case (STYLE_VDW):
      err = drawChain(imol, grampsName, chain, DRAW_OPEN+DRAW_DRAW+DRAW_CLOSE,Atom::radius[6], 0, false);
      break;
    case (STYLE_SPHERE):
      err = drawChain(imol, grampsName, chain, DRAW_OPEN+DRAW_DRAW+DRAW_CLOSE,CA_RADIUS, 0, false);
      break;
    case (STYLE_BALLANDSTICK):
      err  = drawChain(imol, grampsName, chain, DRAW_OPEN+DRAW_DRAW, 0, 3*BOND_RADIUS, false);
      err += drawChain(imol, grampsName, chain,           DRAW_DRAW+DRAW_CLOSE,CA_RADIUS, 0, false);
      break;
    case (STYLE_STICKSANDCOLOREDHET):
      err  = drawChain(imol, grampsName, chain, DRAW_OPEN+DRAW_DRAW, 0, 3*BOND_RADIUS, false);
      err += drawChain(imol, grampsName, chain,           DRAW_DRAW+DRAW_CLOSE,CA_RADIUS, 0, false);
      break;
    case (STYLE_CARTOON_THIN):
      err  = drawCartoon(imol, grampsName, chain, DRAW_OPEN+DRAW_DRAW+DRAW_CLOSE, RIBBON_WIDTH, 0);
      break;
    case (STYLE_CARTOON_THICK):
      err  = drawCartoon(imol, grampsName, chain, DRAW_OPEN+DRAW_DRAW+DRAW_CLOSE, RIBBON_WIDTH, COIL_RADIUS);
      break;
    default:
      // cylinders, but include sphere caps
      err  = drawChain(imol, grampsName, chain, DRAW_OPEN+DRAW_DRAW, 0, 3*BOND_RADIUS, false);
      err += drawChain(imol, grampsName, chain,           DRAW_DRAW+DRAW_CLOSE,3*BOND_RADIUS, 0, true);
      break;
    }
    //if (colorBy == COLOR_BY_SOLID && !color.isValid()) color = COLOR_CARBON;
    setMolColor(item, color, colorBy);


// lines(imol, grampsName, resnum, chain, colorBy, cyl, balls, filter, hydrogens)
  } else {
      switch (style) {
      case (STYLE_LINES):
          err = lines(imol, grampsName, resnum, chain, colorBy, false, COLOR_BY_NONE, filter, hydrogens);
          break;
      case (STYLE_CYLINDERS):
          err = lines(imol, grampsName, resnum, chain, colorBy, true, COLOR_BY_NONE, filter, hydrogens);
          break;
      case (STYLE_BALLANDSTICK):
          err = lines(imol, grampsName, resnum, chain, colorBy, true, COLOR_BY_SOLID, filter, hydrogens);
          break;
      case (STYLE_STICKSANDCOLOREDHET):
          err = lines(imol, grampsName, resnum, chain, COLOR_BY_SOLID, true, colorBy, filter, hydrogens);
          //if (colorBy == COLOR_BY_ATOMS || colorBy == COLOR_BY_CHARGE) color = COLOR_CARBON;
          break;
      case (STYLE_ATOM_VDW): // case of waters
      case (STYLE_VDW):
          err = spheres(imol, NOATOM, grampsName, resnum, chain, colorBy, 0, filter, hydrogens, ATOM_VDW_RADIUS, ATOM_VDW_RADIUS);
          break;
      case (STYLE_SPHERE):
          err = spheres(imol, NOATOM, grampsName, resnum, chain, colorBy, 0, filter, hydrogens, ATOM_SPHERE_RADIUS, ATOM_SPHERE_RADIUS);
          break;
      case (STYLE_ATOM_SPHERE): // case of waters
          err = spheres(imol, NOATOM, grampsName, resnum, chain, colorBy, 0, filter, hydrogens, ATOM_SPHERE_RADIUS, ATOM_VDW_RADIUS);
          break;
      case (STYLE_ATOM_BALL): // case of waters
          err = spheres(imol, NOATOM, grampsName, resnum, chain, colorBy, 0, filter, hydrogens, ATOM_BALL_RADIUS, ATOM_VDW_RADIUS);
          break;
      case (STYLE_NONE):
          break;
      }
  }

  if (err) {
    grampsName.clear();
  } else {
    int itemid = item->type();
    //currentRow.updateIgnore(itemid, false);
    currentRow.updateIgnore(itemid, false);
    //currentRow.getRow(itemid); //currentRow.ignore = 0;
    color = applyColor(grampsName, color);
    setMolColor(item, color, colorBy);
    //updateColorIcon(item, color, colorBy);
  }

//  if (color.isValid()) {
//    applyColor(item, color);
//  }

  return grampsName;
}

int ChemWidget::drawDummy(const QString molnam) {
  //float data[7] = {1,5,0,0.,0.,0.,1.};
  float data[3] = {0,0,0};
  int err = 0;
  if (molnam.length()) {
    err = getMem(molnam.toLocal8Bit().data(), data, sizeof(data)/sizeof(float), molnam.length());
    if (err) return err;
    err = getMemDone("", NULL, 0, 0);
  } else {
    err = 1;
  }
  return err;
}

int ChemWidget::lines(int imol, const QString molnam, unsigned int resnum, char chain, int colorBy, bool cyl, int balls, int filter, int hydrogens) {

    //  draw bonds as lines or cylinders for selected atoms in mol.
    //   make new gramps object named molnam, unless molnam is empty
    //   limit to residue resnum, unless resnum != NORESNUM
    //   include all residues in chain if chain not blank or null
    //   color half-bonds by atom/cpk color if colored
    //   use cylinders if cyl
    //   include spheres if balls

    float radius = BOND_RADIUS;
    float spec[3] = {0,1,radius};
    int size = 0;
    int err = 0;
    if (molnam.length()) {
        size = 3;
        err = getMem(molnam.toLocal8Bit().data(), spec, size, molnam.length());
        if (err) return err;
    }

    bool chaintest = (chain != NOCHAIN);
    bool pairs;
    //  bool both = false; // filter allows either atom in bond to match
    //if (HAS_BIT(filter, FILTER_MAIN)) {
    //  if (filter == FILTER_MAIN) {
    //    // main chain filter wants both atoms in the bond to be in the main chain
    //    both = true;
    //  }

    bondQuery bond;
    bond.iter(imol, resnum, chain, filter, hydrogens);
    int nbonds = bond.count();

    spec[0] = nbonds * 2; // coordinate count, each bond having 2 atoms
    spec[1] = 0; // alternating lines: move-draw-move-draw-...
    spec[2] = 3;
    if (cyl) spec[1] = 1; // alternating lines as cylinders
    if (colorBy != COLOR_BY_SOLID || chaintest || resnum != NORESNUM) {
        pairs = true;
        spec[0] = 2; // will draw bond pairs explicitly
    } else {
        pairs = false;
        err = getMemMore("", spec, size, 0);
    }

    for ( bond.first(); bond.valid; bond.next() ) { //= Db::nextBond(qbond); bond.valid; bond = Db::nextBond(qbond)) {
        err += drawBond(bond, pairs, colorBy, spec);
    }

    if (cyl) {
        float brad = radius;
        float hrad = radius;
        if (balls != COLOR_BY_NONE) brad = radius * 2;
        //spheres(imol, "", resnum, chain, colored, 0, brad, hrad);
        if (balls == COLOR_BY_ATOMS) {
            // uncolored H atoms
            //setAtomColor(1); // color them white anyway
            spheres(imol, NOATOM, "", resnum, chain, COLOR_BY_HATOM,  2, filter, hydrogens, brad, hrad);
            // colored hetero atoms
            spheres(imol, NOATOM, "", resnum, chain, COLOR_BY_HETERO, 2, filter, hydrogens, brad, hrad);
        } else if (balls == COLOR_BY_CHARGE) {
            //spheres(imol, "", resnum, chain, COLOR_BY_NONE,   2, filter, hydrogens, brad, hrad);
            spheres(imol, NOATOM, "", resnum, chain, COLOR_BY_CHARGE, 2, filter, hydrogens, brad, hrad);
        } else {
            spheres(imol, NOATOM, "", resnum, chain, colorBy, 0, filter, hydrogens, brad, hrad);
        }
    }

    if (molnam.length()) err = getMemDone("", NULL, 0, 0);
    return 0;
}

void ChemWidget::setAtomColor(int atnum) {
  // insert a color "command" into the gramps object for this atomic number
  float color[6] = {0,4,0,0.,0.,1.};
  color[3] = Atom::color[atnum][0];
  color[4] = Atom::color[atnum][1];
  color[5] = Atom::color[atnum][2];
  getMemMore("", color, 6, 0);
}

void ChemWidget::setChargeColor(float charge) {
  // insert a color "command" into the gramps object for this atomic number
  float color[6] = {0,4,0,0.,0.,1.};
  QColor colr = COLOR_CARBON;
  float hue=colr.hue(), sat=colr.saturation(), val=colr.value();
  if (charge < 0) {
      hue = Atom::color[8][0]; //red
      sat = 4 * -charge;
      val = 1;
  } else {
       hue = Atom::color[7][0]; //blue
       sat = 4 * charge;
       val = 1;
  }
  if (sat > 1) sat = 1;
  color[3] = hue;
  color[4] = sat;
  color[5] = val;
  getMemMore("", color, 6, 0);
}

int ChemWidget::drawAtom(atomQuery atom, int colorBy, float radius, float Hradius, int hetero) {
//     draw atoms, but selectively depending on their hetero status.
//     used for STICKSANDCOLOREDHET style.

  if (hetero == 0 || (hetero == 1 && atom.atnum != 6) ) {
    return drawAtom(atom, colorBy, radius, Hradius);
  } else if (hetero == 2) {
    if (colorBy == COLOR_BY_HETERO) {
      if (atom.atnum == 1) return 0;
      return drawAtom(atom, colorBy, radius, Hradius);
    } else if (colorBy == COLOR_BY_CHARGE) {
      return drawAtom(atom, colorBy, radius, Hradius);
    } else if (colorBy == COLOR_BY_HATOM){
      if (atom.atnum == 1) return drawAtom(atom, colorBy, radius, Hradius);
    }
  }
  return 0;
}

int ChemWidget::drawAtom(atomQuery atom, int colorBy, float radius, float Hradius) {

//   color sphere by atom/cpk or charge color
//   use radius supplied, or vdw radii if 0.0
//   use Hradius for H atoms, or vdw radius if ATOM_VDW_RADIUS (==0.0)
  
  float spdata[7] = {1,5,0,0.,0.,0.,1.};
  int size = 7;
  if (colorBy == COLOR_BY_CHARGE) {
      setChargeColor(atom.pcharge);
  } else if (colorBy == COLOR_BY_HATOM && 1 == atom.atnum) {
      ;
  } else if (colorBy == COLOR_BY_SOLID) {
      ;
  } else {
      setAtomColor(atom.atnum);
  }
  spdata[3] = atom.x; spdata[4] = atom.y; spdata[5] = atom.z;
  if (1 == atom.atnum) {
    spdata[6] = Hradius;
  } else {
    spdata[6] = radius;
  }
  if (spdata[6] <= ATOM_VDW_RADIUS) {
    spdata[6] = Atom::radius[atom.atnum];
  }
  return getMemMore("", spdata, size, 0);
}

int ChemWidget::spheres(int imol, int iatom, const QString molnam, int resnum, char chain, int colorBy,
  int hetero, int filter, int hydrogens, float radius = ATOM_VDW_RADIUS, float Hradius = ATOM_VDW_RADIUS) {

//   draw spheres for atoms in mol
//   make new gramps object named molnam, unless molnam is empty
//   limit to residue resnum, unless resnum  == NORESNUM
//   include all residues in chain if chain not null and not blank
//   color sphere by atom/cpk color if colored
//   use radius supplied, or vdw radii if ATOM_VDW_RADIUS (==0.0)
//   use Hradius for H atoms, or vdw radius if ATOM_VDW_RADIUS (==0.0)

  int err = 0;
  int size;
  if (molnam.length()) { 
    float spec[3] = {0,0,1};
    size = 3;
    err = getMem(molnam.toLocal8Bit().data(), spec, size, molnam.length());
    if (err) return err;
  }
  if (iatom == NOATOM) {
      // no particular atom, so iterate over all atoms in mol/residue/chain/filter
      for (atom_query.iter(imol, resnum, chain, filter, hydrogens); atom_query.next(); ) {
          err += drawAtom(atom_query, colorBy, radius, Hradius, hetero);
      }
  } else {
      atom_query.get(imol, iatom);
      err = drawAtom(atom_query, colorBy, radius, Hradius, hetero);
  }

  //qDebug() << "spheres err " << err << "resnum " << resnum;
  if (molnam.length()) getMemDone("", NULL, 0, 0);
  return err;
}

//QTreeWidgetItem * ChemWidget::getTreeRow(int itemid) {
//  QTreeWidgetItemIterator it(this);
//  while (*it) {
//    if ((*it)->type() == itemid) {
//#ifdef DEBUG
//      qDebug() << (*it)->type() << (*it)->text(NAME_COLUMN);
//#endif
//      return (*it);
//    }
//    ++it;
//  }
//  return 0;
//}

void ChemWidget::makeNewCurrentRow(QString rowname, int rootType, QString grampsName, int resnum,
                                   Qt::CheckState state, int imol, int iatom, char chain, int drawstyle,
                                   int colorBy, QColor color, int filter, int hydrogens) {

    // remember pieces of data for later use when redrawing or other user-driven operations on this row.
    // rowname displayed in tree
    /*
    currentRow.rowname = rowname;
    // gramps object name
    currentRow.grampsName = grampsName;
    // the residue number, if any (0 says no residues)
    currentRow.resnum = resnum;
    // the chain if any
    currentRow.chain = chain;
    // the molecule unique id number
    currentRow.imol = imol;
    // the parent of this row;
    currentRow.parentId = rootType;
    // the ignore state
    currentRow.ignore = (state==Qt::Unchecked);
    // drawing style
    currentRow.style = drawstyle;
    // colored by
    currentRow.colorBy = colorBy;
    // color
    currentRow.color = color;
    // filters some atoms
    currentRow.filter = filter;
    // draw hydrogens or not
    currentRow.hydrogens = hydrogens;
    // draw main chain atoms along with side chain atoms for single residue
    currentRow.mainSide = 0;
    // tree.atid, if row "belongs" to an atom
    currentRow.iatom = iatom;
    // is this row's checkbox checked
    currentRow.checked = state;
    */
    int mainSide = 0;
    int ignore = (state==Qt::Unchecked);

    currentRow.newRow(rootType, imol, iatom, grampsName, rowname,
    resnum, chain, ignore, drawstyle, colorBy, filter,
    color.hue(), color.saturation(), color.value(), color.alpha(), hydrogens, mainSide, state);
}

QTreeWidgetItem * ChemWidget::addMolRow(QTreeWidgetItem *root, QString rowname, int resnum,
                                        Qt::CheckState state, int imol, int iatom, char chain,
                                        QString suffix, int drawstyle, int colorBy, QColor color, int filter)
{
  
  // add a row to the tree to represent a gramps group, or a gramps object with some type
  //   of drawing.

  QString grampsName = encodeMolName(imol, chain, resnum, suffix);
  QTreeWidgetItem *item = getGrampsItem(grampsName);
  //QList<QTreeWidgetItem*>item = findItems(grampsName, Qt::MatchFixedString|Qt::MatchCaseSensitive|Qt::MatchRecursive, NAME_COLUMN);
  if (item) return item;

  currentRow.getRow(grampsName);
  if (currentRow.valid) {
      // already have a db entry with this name (when restoring from session file)
  } else {
      int hcount = atom_query.hcount(imol, resnum, chain, filter);
      int hydrogens = (hcount == 0) ? HYDROGEN_NONE : hydrogenDefault;
      // store this row info in db
      makeNewCurrentRow(rowname, root->type(), grampsName, resnum, state, imol, iatom, chain, drawstyle, colorBy, color, filter, hydrogens);
  }
  return makeMolRow(root, grampsName, rowname, state);
}
QTreeWidgetItem * ChemWidget::makeMolRow(QTreeWidgetItem *root, QString grampsName, QString rowname, Qt::CheckState state) {
    blockSignals(true); // don't carry out the action for this
    QList<QString> items;
    items << rowname << " " << grampsName;
    // "borrow" use of type() attribute of QTreeWidgetItem to use for itemid in db and currentRow
    QTreeWidgetItem *row = new QTreeWidgetItem(items, currentRow.itemId);
    row->setCheckState(MOL_COLUMN, state);
    root->addChild(row);
    row->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    blockSignals(false);
    return row;
}

QColor ChemWidget::nextColor(QColor color) {
    QColor newColor;
    newColor.setHsv(rand()%360, 200 + rand()%55 , 200 + rand()%55);
    if (color.isValid()) {
        while (abs(color.hue() - newColor.hue()) < 50) {
            //newColor.setHsv(color.hue()+60, color.saturation(), color.value());
            newColor.setHsv(rand()%360, 200 + rand()%55 , 200 + rand()%55);
        }
    }
    return newColor;
}

void ChemWidget::dumpout(float *data, int size) {
  for (int i=0; i<size; i+=3) {
     fprintf (dfp, "%f,%f,%f\n", data[i], data[i+1], data[i+2]);
  }
}

//#define MEMDEBUG 1
int ChemWidget::getMem    (const char *name, float *data, int size, int namlen) {
  int s = size;
#ifdef PIXTMP
  dfp = fopen(name, "w");
  if (size)dumpout(data,s);
  dfname = name;
  return 0;
#else
  int err = getmem_(name,data,&s,namlen);
  if (err) {
    QString msg;
    QTextStream(&msg) << "getMem" << name << " size=" << size << " error=" << err;
    emit msgReady(msg);
  }
#ifdef MEMDEBUG
  dfp = fopen(name, "w");
  if (size)dumpout(data,s);
#endif
  getMemErr = err;
  return err;
#endif
}

int ChemWidget::getMemMore(const char *name, float *data, int size, int namlen) {
  int s = size;
#ifdef PIXTMP
  if (size)dumpout(data,s);
  return 0;
#else
#ifdef MEMDEBUG
  if (size)dumpout(data,s);
#endif
  if (getMemErr) return getMemErr;
  int err = getmemmore_(name,data,&s,namlen);
  if (err) {
    QString msg;
    QTextStream(&msg) << "getMemMore" << name << " size=" << size << " error=" << err;
    emit msgReady(msg);
  }
  getMemErr = err;
  return err;
#endif
}

int ChemWidget::getMemDone(const char *name, float *data, int size, int namlen) {
  int s = size;
#ifdef PIXTMP
  if (size)dumpout(data,s);
  fclose(dfp);
  QString cmd;
  QTextStream(&cmd) << "get/name=" << dfname << " " << dfname;
  emit cmdReady(cmd);
  return 0;
#else
#ifdef MEMDEBUG
  if (size)dumpout(data,s);
  fclose(dfp);
#endif
  if (getMemErr) return getMemErr;
  int err = getmemdone_(name,data,&s,namlen);
  if (err) {
    QString msg;
    QTextStream(&msg) << "getMemDone error=" << err;
    emit msgReady(msg);
  }
  getMemErr = err;
  return err;
#endif
}


//bool ChemWidget::event(QEvent* event)
//{
//    qDebug() << event->type();

////    const QMimeData* mimeData = event->mimeData();

////    // check for our needed mime type, here a file or a list of files
////    if (mimeData->hasUrls())
////    {
////        QStringList pathList;
////        QList<QUrl> urlList = mimeData->urls();

////        // extract the local paths of the files
////        for (int i = 0; i < urlList.size() && i < 32; ++i)
////        {
////            pathList.append(urlList.at(i).toLocalFile());
////        }

////        // call a function to open the files
////        //openFiles(pathList);
////        qDebug() << pathList;
////    }
//    return QTreeWidget::event(event);
//}
