#include <math.h>
#include "chem.h"
int ChemWidget::drawSurface(int itemid) {
  float CoulombK = 10;
  treeRow item = Db::getTreeRow(itemid);
  QString name = item.grampsName;
  QColor color = item.color;
  float nearAtom = 0;
  int ldm1 = 7;
  int size = 6;
  if (item.colorBy == COLOR_BY_SOLID) {
    nearAtom = 0.0;
    ldm1 = 7; size = 6;  // 3 coords and 3 normals
  } else {
    if (item.colorBy == COLOR_BY_ATOMS) {
      nearAtom = 3.2;
    } else {
      nearAtom = 0.0;
    }
    ldm1 = 10; size = 9; // 3 coords, 3 normals and 3 colors
  }
  QSqlQuery qvtx = Db::iterVertices(itemid, item.imol, item.resnum, item.chain, item.filter, item.hydrogens, currentRow.colorBy, nearAtom);
  int nvtx = Db::countVertices(itemid);
  float buff[9] = {nvtx,ldm1,2};
  int err = getMem(name.toLocal8Bit().data(), buff, 3, name.length());
  if (err) {
    qDebug() << "can't create surface for item" << itemid;
  } else {
    nvtx = 0;
    float hue, sat;
    for (vtxRecord vtx = Db::nextVertex(qvtx); vtx.valid; vtx = Db::nextVertex(qvtx)) {
      buff[0] = vtx.x;  buff[1] = vtx.y;  buff[2] = vtx.z;
      buff[3] = vtx.nx; buff[4] = vtx.ny; buff[5] = vtx.nz;
      if (item.colorBy == COLOR_BY_ATOMS) {
        if (vtx.atnum < 1) qDebug() << tr("unknown atom at vertex") << nvtx+1;
        buff[6] = Atom::color[vtx.atnum][0]; buff[7] = Atom::color[vtx.atnum][1]; buff[8] = Atom::color[vtx.atnum][2];
      } else if (item.colorBy == COLOR_BY_CHARGE) {
        if (vtx.value < 0) {
          hue = Atom::color[8][0]; // red
          sat = CoulombK * sqrt(-vtx.value);
        } else {
          hue = Atom::color[7][0]; // blue
          sat = CoulombK * sqrt(vtx.value);
        }
        buff[6] = hue;
        buff[7] = sat > 1 ? 1 : sat;
        buff[8] = 1;
      }
      err += getMemMore("", buff, size, 0);
      ++nvtx;
      if (err) { qDebug() << tr("drawSurface vertex error after ") << nvtx; break; }
    }
    QSqlQuery qtri = Db::iterTriangles(itemid);
    int ntri = Db::numRows(qtri)/3;
    buff[0] = ntri; buff[1] = 8; buff[2] = -1;
    qDebug() << tr("drawSurface triangle count") + " = " << ntri;
    err += getMemMore("", buff, 3, 0);
    ntri=0;
    for (triRecord tri = Db::nextTriangle(qtri); tri.valid; tri = Db::nextTriangle(qtri)) {
      buff[0] = tri.vid;
      tri = Db::nextTriangle(qtri); 
      buff[1] = tri.vid;
      tri = Db::nextTriangle(qtri); 
      buff[2] = tri.vid;
      err += getMemMore("", buff, 3, 0);
      ++ntri;
      if (err) { qDebug() << tr("drawSurface triangle error after ") << ntri; break; }
    }
    qDebug() << tr("surface ready") << itemid << ntri << tr("triangles") + ".";
    err += getMemDone("", NULL, 0, 0);
  }
  if (err == 0) {
    QString cmd = "intensity " + name + " t,0.5";
    cmdReady(cmd);
    if (color.isValid()) applyColor(item.grampsName, color);
  }
  return err;
}

void ChemWidget::surfReady(int exitcode) {
  QString errout = QString(surfproc->readAllStandardError());
  QStringList msg = errout.split('\n');
  if (exitcode) qDebug() << errout;
  int itemid = msg[msg.size()-2].split("=")[1].toInt(); // last line itemid=%d
  //db.open();
  progress->setMaximum(100);
  progress->setValue(50);
  //QSqlQuery query;
  //query.exec("Pragma foreign_keys");
  //query.next();
  //qDebug() << "surfaceReady: foreign keys" << query.value(0).toInt();
  int err = drawSurface(itemid);
  progress->cancel();
  if (err) {
  } else {
    //QString cmd;
    Db::updateTreeIgnore(itemid, 0);
    treeRow t = Db::getTreeRow(itemid);
    QTreeWidgetItem *item = getGrampsItem(t.grampsName);
    if (item) item->setCheckState(MOL_COLUMN, Qt::Checked);
    insertOrGroup(t.grampsName, Db::getTreeRow(t.parentId));
  }
  //currentRow.ignore = 0;
  //Db::updateTreeIgnore(itemid, 0);
}

void ChemWidget::makeSurface() {
  int style = currentRow.style;
  //addSurfRow(STYLE_SURF_MOL);
#ifdef __APPLE__
  //QString prog = gmolLib + "/dbsurf";
  //qDebug() << QCoreApplication::applicationDirPath();
  QString prog = QCoreApplication::applicationDirPath() + "/dbsurf";
#else
  QString prog = "./dbsurf";
#endif
  QStringList arguments;
  arguments << QSqlDatabase::database().databaseName() << "-db" << "-i" << QString::number(currentRow.itemId);
  QString title;
  if (style == STYLE_SURF_MOL) {
    arguments << "-v" << QString::number(90.) << "-s" << "0.75";  
    title = "Molecular surface";
  } else if (style == STYLE_SURF_WATER) {
    arguments << "-v" << QString::number(20.) << "-s" << "0.75";  
    title = "Water-accessible surface";
  } else {
    //  try to avoid this before calling drawSurface; even avoid putting it into ContextMenu
    qDebug() << tr("unknown surface style") << style;
    return;
  }

  qDebug() << prog << arguments;
  QSqlDatabase::database().commit();
  progress->setWindowTitle(title);
  progress->setLabelText(tr("Computing surface"));
  surfproc->start(prog, arguments);
  progress->setMaximum(100);
  progress->setValue(1);
}

void ChemWidget::addSurfRow() {
  QTreeWidgetItem *parent = currentItem();
  QColor color = currentRow.color;
  //treeRow parentRow = Db::getTreeRow(parent->type());
  //color = parentRow.color;
  if (!color.isValid()) color = COLOR_DEFAULT;
  char chain     = currentRow.chain;
  int imol       = currentRow.imol;
  //int filter     = currentRow.filter;
  int filter = FILTER_NONE; // all atoms
  int resnum = currentRow.resnum;
  int numres = Db::molNumRes(imol);
  if (numres > 1) filter = FILTER_MOLECULE; // not waters
  setCurrentItem(addMolRow(parent, "Surface", resnum, Qt::Checked, imol, NOATOM, chain, "surface", STYLE_SURF_WATER, COLOR_BY_SOLID, color, filter));
  expandItem(parent);
  currentRow.ignore = 1; // so as to prevent the forget command from being issued
  //makeSurface(currentRow.style);
  styleMol(STYLE_SURF_WATER);
  setMolColor(currentItem(), color, COLOR_BY_SOLID);
  currentRow.ignore = 0;
}
