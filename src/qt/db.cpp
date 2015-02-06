/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2013 by TJ O'Donnell and
C  Copyright (C) 2005-2013 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
#include <algorithm>
#include "db.h"
#include "chem.h"
#include "mainwindow.h"
#ifdef PDBREADER
#include <fstream>
#include <iostream>
#include <sstream>
#endif
#include "ChemDb/dbchem.h"
#include <QDebug>
#include <QColor>
#include <QSqlDriver>
#include <QSqlError>

extern QString gmolLib;
int Db::itemId = 0; //static
QList<selectionFilter> Db::filters = QList<selectionFilter>();
Db::Db() {
}

QSqlDatabase Db::open() {
    QTemporaryFile *tmpFile = new QTemporaryFile();
    tmpFile->open(); // create the file
    tmpFile->setAutoRemove(false);
    tmpFile->close();
#ifdef __APPLE__
    // QFile::copy misses the icon (resource fork?)
    QProcess copyfile;
    copyfile.start("/bin/cp", QStringList() << gmolLib + "/init.gdb3" << tmpFile->fileName());
    copyfile.waitForStarted();
    copyfile.waitForFinished();
#else
//    QFile::remove(tmpFile->fileName());
//    if (!QFile::copy(gmolLib + "/init.gdb3", tmpFile->fileName())) {
//        qDebug() << "can't copy init.gdb3 to" << tmpFile->fileName();
//    }
    QFile  srcFile(QDir::fromNativeSeparators(gmolLib) + "/init.gdb3"),
          destFile(tmpFile->fileName());
    if (srcFile.open(QIODevice::ReadOnly) &&
         destFile.open(QIODevice::WriteOnly))
    {
         QByteArray data = srcFile.readAll();
         destFile.write(data);
    }
    srcFile.close();
    destFile.close();
#endif
    return openDbFile(tmpFile->fileName());
}
QSqlDatabase Db::openDbFile(QString fileName) {
#ifdef PDBREADER
    QSqlDatabase db = ChemDb::open(fileName, true);
#else
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(fileName);
    db.open();
    //qDebug() << (db.driver())->hasFeature(QSqlDriver::QuerySize);
#endif
    if (db.isOpen()) {
        QSqlQuery query;
        itemId = 0;
        attachResidue();
        query.exec("PRAGMA foreign_keys = ON");
        query.exec("Pragma foreign_keys");
        query.next();
        qDebug() << QSqlDatabase::database().databaseName() << "DB:open(): foreign keys" << query.value(0).toInt();
        filters = getFilters();
    } else {
        qDebug() << "can't open database";
    }
    return db;
}

bool Db::tellError(QSqlQuery &query) {
    return tellError(query,"");
}
bool Db::tellError(QSqlQuery &query, QString msg) {
    QSqlDatabase db = QSqlDatabase::database();
    //qDebug() << query.lastError();
    qDebug() << msg << query.lastQuery();
    qDebug() << TR("error: ") << db.lastError().number() << db.lastError().text();
    return false;
}
bool Db::save(QString file) {
    //QSqlDatabase db = QSqlDatabase::database();
#ifdef __APPLE__
    // QFile::copy misses the icon (resource fork?)
    QProcess copyfile;
    copyfile.start("/bin/cp", QStringList() << QSqlDatabase::database().databaseName() << file);
    copyfile.waitForStarted();
    copyfile.waitForFinished();
    return true;
#else
    return QFile::copy(QSqlDatabase::database().databaseName(), file);
#endif
}
bool Db::openExisting(QString filename) {
    //qDebug() << "restore" << filename;
    QSqlDatabase olddb = QSqlDatabase::database();
    olddb.commit();
    olddb.close();
    //QStringList conns = olddb.connectionNames();
    //qDebug() << conns;
    QString conn = olddb.defaultConnection;
    //qDebug() << conn;
    //delete olddb;
    QSqlDatabase::removeDatabase(conn);
    QSqlDatabase db = openDbFile(filename);
    itemId = maxItemId();
    return db.isOpen();
}

bool Db::createTreeTable() {
    QSqlQuery query;
    if (!query.exec("Create Table main.tree (itemid Integer Primary Key, parentid Integer, imol Integer, iatom Integer, grampsname Text, rowname Text, resnum integer, chain Char(1), ignore Integer, style Integer, colorBy Integer, filter Integer, hue Integer, saturation Integer, value Integer, hydrogens Integer, mainSide Integer, checked Integer, Foreign key (imol) References molecule (molid) On Delete Cascade)")) return tellError(query);
    return true;
}


bool Db::attachResidue() {
    QSqlQuery query;
//#ifdef __APPLE__
    QString resdir = gmolLib;
//#else
//    QString resdir = ".";
//#endif
    if (!query.exec("Attach Database '" + resdir + "/residue.db' As residue")) {
        tellError(query);
        return false;
    } else {
        return true;
    }
}

bool Db::updateTreeColor(int itemid, QColor color) {
    QSqlQuery query;
    query.prepare("Update main.tree Set hue=?, saturation=?, value=? Where itemid=?");
    query.addBindValue((int)color.hue());
    query.addBindValue((int)color.saturation());
    query.addBindValue((int)color.value());
    query.addBindValue((int)itemid);
    if (!query.exec()) {
        tellError(query);
        return false;
    } else {
        return true;
    }
}
bool Db::updateTreeColorBy(int itemid, int colorBy) {
    QSqlQuery query;
    query.prepare("Update main.tree Set colorBy=? Where itemid=?");
    query.addBindValue((int)colorBy);
    query.addBindValue((int)itemid);
    if (!query.exec()) {
        tellError(query);
        return false;
    } else {
        return true;
    }
}
bool Db::updateTreeIgnore(int itemid, int ignore) {
    QSqlQuery query;
    query.prepare("Update main.tree Set ignore=? Where itemid=?");
    query.addBindValue((int)ignore);
    query.addBindValue((int)itemid);
    if (!query.exec()) {
        tellError(query);
        return false;
    } else {
        return true;
    }
}
bool Db::updateTreeChecked(int itemid, int checked) {
    QSqlQuery query;
    query.prepare("Update main.tree Set checked=? Where itemid=?");
    query.addBindValue(checked);
    query.addBindValue((int)itemid);
    if (!query.exec()) {
        tellError(query);
        return false;
    } else {
        return true;
    }
}
bool Db::updateTreeStyle(int itemid, int style) {
    QSqlQuery query;
    query.prepare("Update main.tree Set style=? Where itemid=?");
    query.addBindValue((int)style);
    query.addBindValue((int)itemid);
    if (!query.exec()) {
        tellError(query);
        return false;
    } else {
        return true;
    }
}
bool Db::updateTreeHydrogens(int itemid, int hydrogens) {
    QSqlQuery query;
    query.prepare("Update main.tree Set hydrogens=? Where itemid=?");
    query.addBindValue((int)hydrogens);
    query.addBindValue((int)itemid);
    if (!query.exec()) {
        tellError(query);
        return false;
    } else {
        return true;
    }
}
bool Db::updateTreeMainSide(int itemid, int mainSide, int filter) {
    QSqlQuery query;
    query.prepare("Update main.tree Set mainSide=?, filter=? Where itemid=?");
    query.addBindValue((int)mainSide);
    query.addBindValue((int)filter);
    query.addBindValue((int)itemid);
    if (!query.exec()) {
        tellError(query);
        return false;
    } else {
        return true;
    }
}

std::string Db::getChainSS(int molid, char chain, int numres) {
    QSqlQuery query;
    query.prepare("Select resnum, type From main.secondary_structure Where molid=? And chain=? Order by resnum");
    query.addBindValue((int)molid);
    query.addBindValue((QString)chain);
    if (query.exec()) {
        std::string sstype = std::string(numres+1, '\0');
        sstype.insert(0, numres+1, '\0');
        while (query.next()) {
            int resnum = query.value(0).toInt();
            char type  = query.value(1).toString().toLocal8Bit().data()[0];
            sstype[resnum] = type;
        }
        return sstype;
    } else {
        tellError(query);
        return NULL;
    }
}

int Db::maxItemId() {
    int itemid = 0;
    QSqlQuery query;
    query.prepare("Select max(itemid) From main.tree");
    if (!query.exec()) tellError(query);
    query.next();
    itemid = query.value(0).toInt();
    return itemid;
}

//  int getTreeRowId(int molid) {
//    int itemid = 0;
//    QSqlQuery query;
//    query.prepare("Select itemid From main.tree Where imol=? And parentid > 0");
//    query.addBindValue((int)molid);
//    if (!query.exec()) tellError(query);
//    query.next();
//    itemid = query.value(0).toInt();
//    return itemid;
//  }

treeRow Db::getTreeRow(int itemid) {
    QSqlQuery query;
    query.prepare("Select itemid, parentid, imol, iatom, grampsname, rowname, resnum, chain, ignore, style, colorBy, filter, hue, saturation, value, hydrogens, mainSide, checked From main.tree Where itemid=?");
    query.addBindValue((int)itemid);
    if (!query.exec()) tellError(query);
    return nextTreeRow(query);
}
treeRow Db::getTreeRow(QString grampsName) {
    QSqlQuery query;
    query.prepare("Select itemid, parentid, imol, iatom, grampsname, rowname, resnum, chain, ignore, style, colorBy, filter, hue, saturation, value, hydrogens, mainSide, checked From main.tree Where grampsname=?");
    query.addBindValue((QString)grampsName);
    if (!query.exec()) tellError(query);
    return nextTreeRow(query);
}
treeRow Db::getTreeSibling(int parentid, char sibchain) {
    QSqlQuery query;
    query.prepare("Select itemid, parentid, imol, iatom, grampsname, rowname, resnum, chain, ignore, style, colorBy, filter, hue, saturation, value, hydrogens, mainSide, checked From main.tree Where parentid=? and chain=?");
    query.addBindValue(parentid);
    query.addBindValue((QString)sibchain);
    if (!query.exec()) tellError(query);
    return nextTreeRow(query);
}

int Db::newTreeRow(treeRow &row) {
    QSqlQuery query;
    ++itemId;
    query.prepare("Insert Into main.tree (itemid, parentid, imol, iatom, grampsname, rowname, resnum, chain, ignore, style, colorBy, filter, hue, saturation, value, hydrogens, mainSide, checked) Values (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    query.addBindValue((int)itemId);
    query.addBindValue((int)row.parentId);
    query.addBindValue((int)row.imol);
    query.addBindValue((int)row.iatom);
    query.addBindValue(row.grampsName);
    query.addBindValue(row.rowname);
    query.addBindValue((int)row.resnum);
    query.addBindValue((QString)row.chain);
    query.addBindValue((int)row.ignore);
    query.addBindValue((int)row.style);
    query.addBindValue((int)row.colorBy);
    query.addBindValue((int)row.filter);
    query.addBindValue((int)row.color.hue());
    query.addBindValue((int)row.color.saturation());
    query.addBindValue((int)row.color.value());
    query.addBindValue((int)row.hydrogens);
    query.addBindValue((int)row.mainSide);
    query.addBindValue((int)row.checked);
    if (!query.exec()) tellError(query);
    return itemId;
}

bool Db::isTreeRow(int imol, char chain, QString rowname) {
    QSqlQuery query;
    query.prepare("Select itemid From main.tree Where imol=? And chain=? And rowname=?");
    query.addBindValue((int)imol);
    query.addBindValue((QString)chain);
    query.addBindValue((QString)rowname);
    if (!query.exec()) tellError(query);
    bool retval = query.next();
    //qDebug() << "isTreeRow" << imol << chain << rowname << retval;
    return retval;
}

bool Db::isTreeRow(int imol, char chain, int iatom) {
    QSqlQuery query;
    query.prepare("Select itemid From main.tree Where imol=? And chain=? And iatom=?");
    query.addBindValue((int)imol);
    query.addBindValue((QString)chain);
    query.addBindValue((int)iatom);
    if (!query.exec()) tellError(query);
    bool retval = query.next();
    //qDebug() << "isTreeRow" << imol << chain << iatom << retval;
    return retval;
}

QString Db::molTitle(int imol) {
    return getMol(imol).title;
}

int Db::molNumBonds(int imol) {
    return getMol(imol).nbonds;
}

int Db::molNumAtoms(int imol) {
    return getMol(imol).natoms;
}

int Db::molNumRes(int imol) {
    return getMol(imol).nresidue;
}

QString Db::molResName(int imol, int resnum, char chain) {
    QString name;
#ifdef DEBUG
    qDebug() << "Db::molResName";
#endif
    QSqlQuery query;
    query.prepare("Select Distinct resnam From main.atom Where molid=? And resnum=? And chain=?");
    query.addBindValue(imol);
    query.addBindValue(resnum);
    query.addBindValue((QString)chain);
    if (query.exec()) {
        query.next();
        name = query.value(0).toString();
    } else {
        tellError(query);
    }
    return name;
}

int Db::chainNumRes(int imol, char chain) {
    int nres = 0;
#ifdef DEBUG
    qDebug() << "Db::chainNumRes";
#endif
    QSqlQuery query;
    query.prepare("Select max(resnum) From main.atom Where molid=? And chain=?");
    query.addBindValue((int)imol);
    query.addBindValue((QString)chain);
    if (query.exec()) {
        query.next();
        nres = query.value(0).toInt();
    } else {
        tellError(query);
    }
    return nres;
}

int Db::numRows(QSqlQuery query) {
    /* workaround for sqlite (numRows not implemented in driver) to get number of rows in a query result.
      should be called before first result is retrieved, since this method reset result set to before
      the first result
  */
    int current = query.at();
    if (query.last()) {
        int nrows = query.at() + 1;
        query.seek(current);
        return nrows;
    } else {
        return 0;
    }
}
/* These iter* methods prepare a SQL statement for use in a while (query->next()) type loop
     in the manner of an iterator
  */
QSqlQuery Db::iterMols() {
    QSqlQuery query;
    query.prepare("Select molid,file,title,type,nresidue,natoms,nbonds,model From molecule Order By molid Asc, file");
    if (!query.exec()) tellError(query);
    return query;
}
QSqlQuery Db::iterMolsByFile() {
    QSqlQuery query;
    query.prepare("Select min(molid),file,title,type,nresidue,natoms,nbonds,model From molecule Group By file Order By molid Asc");
    if (!query.exec()) tellError(query);
    return query;
}
QSqlQuery Db::iterMolsinFile(int imol) {
    QSqlQuery query;
    query.prepare("Select molid,file,title,type,nresidue,natoms,nbonds,model From main.molecule Where file=(Select file From main.molecule Where molid=?) Order By molid Asc");
    query.addBindValue(imol);
    if (!query.exec()) tellError(query);
    return query;
}
molRecord Db::getMol(int imol) {
    QSqlQuery query;
    query.prepare("Select molid,file,title,type,nresidue,natoms,nbonds,model From main.molecule Where molid=?");
    query.addBindValue(imol);
    if (!query.exec()) tellError(query);
    return nextMol(query);
}
molRecord Db::nextMol(QSqlQuery qmol) {
    molRecord mol;
    if (qmol.next()) {
        mol.imol     = qmol.value(0).toInt();
        mol.filename = qmol.value(1).toString();
        mol.title    = qmol.value(2).toString();
        mol.type     = qmol.value(3).toString();
        mol.nresidue = qmol.value(4).toInt();
        mol.natoms   = qmol.value(5).toInt();
        mol.nbonds   = qmol.value(6).toInt();
        mol.model    = qmol.value(7).toInt();
        mol.valid    = true;
    } else {
        mol.valid = false;
    }
    return mol;
}
QSqlQuery Db::iterTreeRows() {
    QSqlQuery query;
    query.prepare("Select itemid, parentid, imol, iatom, grampsname, rowname, resnum, chain, ignore, style, colorBy, filter, hue, saturation, value, hydrogens, mainSide, checked From main.tree Order by itemid");
    if (!query.exec()) tellError(query);
    return query;
}
QSqlQuery Db::IterTreeRowsToRestore() {
    // find items needing to be drawn on restore
    QSqlQuery query;
    query.prepare("Select itemid, parentid, imol, iatom, grampsname, rowname, resnum, chain, ignore, style, colorBy, filter, hue, saturation, value, hydrogens, mainSide, checked From main.tree Where style != 0 Order by itemid");
    if (!query.exec()) tellError(query);
    return query;
}
treeRow Db::nextTreeRow(QSqlQuery query) {
    treeRow row;
    if (query.next()) {
        row.itemId      = query.value(ITEM_ID).toInt();
        row.parentId    = query.value(ITEM_PARENTID).toInt();
        row.imol        = query.value(ITEM_IMOL).toInt();
        row.iatom       = query.value(ITEM_IATOM).toInt();
        row.grampsName  = query.value(ITEM_GRAMPSNAME).toString();
        row.rowname     = query.value(ITEM_ROWNAME).toString();
        row.resnum      = query.value(ITEM_RESNUM).toInt();
        row.chain       = query.value(ITEM_CHAIN).toString().toLocal8Bit().data()[0];
        row.ignore      = query.value(ITEM_IGNORE).toInt();
        row.style       = query.value(ITEM_STYLE).toInt();
        row.colorBy     = query.value(ITEM_COLORBY).toInt();
        row.filter      = query.value(ITEM_FILTER).toInt();
        int hue   = query.value(ITEM_HUE).toInt();
        int sat   = query.value(ITEM_SATURATION).toInt();
        int val   = query.value(ITEM_VALUE).toInt();
        if (hue == 0.0 && sat == 0.0 && val == 0.0) {
            row.color   = QColor();
        } else {
            row.color   = QColor::fromHsv(hue,sat,val);
        }
        row.hydrogens = query.value(ITEM_HYDROGENS).toInt();
        row.mainSide  = query.value(ITEM_MAINSIDE).toInt();
        row.checked   = query.value(ITEM_CHECKED).toInt();
        row.valid = true;
    } else {
        row.valid = false;
    }
    return row;
}
/*
  QSqlQuery iterChains(int imol) {
    QSqlQuery query;
    query.prepare("Select Distinct chain From main.atom Where molid=?");
    query.addBindValue(imol);
    if (!query.exec()) tellError(query);
    return query;
  }
*/

QList<selectionFilter> Db::getFilters() {
    QList<selectionFilter> flist;
    QSqlQuery query;
    if (query.prepare("Select id,ordering,appearance,menu,sql,tip from filters order by ordering")) {
        if (!query.exec()) tellError(query);
    }
    while (query.next()) {
        selectionFilter f;
        f.id   = query.value(0).toInt();
        f.ordering = query.value(1).toInt();
        f.appearance = query.value(2).toString();
        f.menu = query.value(3).toString();
        f.sql  = query.value(4).toString();
        f.tip  = query.value(5).toString();
        flist.append(f);
    }
    return flist;
}

 selectionFilter Db::getFilter(int filterid) {
     if (filters.count() == 0) filters = getFilters();
     for (int i=0; i<filters.count(); ++i) {
         if (filters[i].id == filterid) return filters[i];
     }
     return filters[0];
 }

QSqlQuery Db::iterChainCounts(int imol) {
    return iterChainCounts(imol, '\0');
}

QSqlQuery Db::iterChainCounts(int imol, char chain) {
    QString pattern = "Select chain, '%1' name, Count(Distinct(resnum)) From atom Where molid=" + QString::number(imol) + " %2";
    if (chain != '\0') {
        pattern += " And chain='" + QString(chain) + "'";
    } else {
        pattern += " Group by chain";
    }
    QString sql = pattern.arg("nres", "");
    QSqlQuery query;
    query.exec("Select id,menu,sql From filters");
    while (query.next()) {
        //int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        QString filterClause = " And (" + query.value(2).toString() + ")";
        sql += " Union " + pattern.arg(name,filterClause);
    }
    query.finish();
    sql += " Order By chain";
    //qDebug() << sql;
    if (query.prepare(sql) && query.exec()) {
        ;
    } else {
        tellError(query);
    }
    return query;
}
QHash<QString, int> Db::nextChainCounts(QSqlQuery qchain) {
    QHash<QString, int> chainCounts;
    //int nfilters = filters.count();
    char lastChain = '\0';
//    for (int i=0; i < filters.count(); ++i) {
//        if (!qchain.next()) break;
    while (qchain.next()) {
        char chain = qchain.value(0).toString().toLocal8Bit().data()[0];
        if (lastChain == '\0') {
            chainCounts.insert("chain", chain); // char to int - careful
            lastChain = chain;
        } else if (chain != lastChain) {
            qchain.previous(); // new chain, backup one
            break;
        }
        QString name = qchain.value(1).toString();
        int count = qchain.value(2).toInt();
        chainCounts.insert(name, count);
    }
    return chainCounts;
}

QSqlQuery Db::iterAtoms(int imol, int resnum, char chain, int filter, int hydrogens) {
#ifdef DEBUG
    qDebug() << "Db::iterAtoms";
#endif
    QString sql = "Select molid,atid,resnum,resnam,altLoc,icode,atnum,x,y,z,fcharge,pcharge,name,chain,hetatm \
            From main.atom Where molid=" + QString::number(imol);
            if (chain != NOCHAIN) sql += " And chain='" + QString(chain) + "'";
    if (resnum) sql += " And resnum=" + QString::number(resnum);
    if (hydrogens == HYDROGEN_HIDE) {
        sql += " And atnum>1";
    } else if (hydrogens == HYDROGEN_COUNT) {
        // very special case used once to determine hcount
        sql += " And atnum=1";
    }
    //if (filter)  sql += " And flag=" + QString::number(filter);
    if (filter) {
        QString filterClause = getFilter(filter).sql;
        //qDebug() << filterClause;
        sql += " And (" + filterClause + ")";
    }
    QSqlQuery query;
    if (!query.prepare(sql)) tellError(query);
    if (!query.exec()) tellError(query);
    return query;
}
atomRecord Db::nextAtom(QSqlQuery qatom) {
    atomRecord atom;
    if (qatom.next()) {
        atom.molid   = qatom.value(0).toInt();
        atom.atid    = qatom.value(1).toInt();
        atom.resnum  = qatom.value(2).toInt();
        atom.resnam  = qatom.value(3).toString();
        atom.altLoc  = qatom.value(4).toString().toLocal8Bit().data()[0];
        atom.icode   = qatom.value(5).toString().toLocal8Bit().data()[0];
        atom.atnum   = qatom.value(6).toInt();
        atom.x       = qatom.value(7).toDouble();
        atom.y       = qatom.value(8).toDouble();
        atom.z       = qatom.value(9).toDouble();
        atom.fcharge = qatom.value(10).toDouble();
        atom.pcharge = qatom.value(11).toDouble();
        atom.name    = qatom.value(12).toString();
        atom.chain   = qatom.value(13).toString().toLocal8Bit().data()[0];
        atom.hetatm  = qatom.value(14).toInt();
        atom.valid = true;
    } else {
        atom.valid = false;
    }
    return atom;
}
atomRecord Db::getAtom(int imol, int iatom) {
    QString sql = "Select molid,atid,resnum,resnam,altLoc,icode,atnum,x,y,z,fcharge,pcharge,name,chain,hetatm From main.atom Where molid=? And atid=?";
    QSqlQuery query;
    if (!query.prepare(sql)) tellError(query);
    query.addBindValue(imol);
    query.addBindValue(iatom);
    if (!query.exec()) tellError(query);
    return nextAtom(query);
}

//  QSqlQuery Db::iterResidues(int imol, int resnum, char chain, int filter) {
//#ifdef DEBUG
//    qDebug() << "Db::iterResidues";
//#endif
//    QString sql = "Select resnum,resnam From main.atom Where molid=?";
//    if (chain != NOCHAIN) sql += " And chain=?";
//    if (resnum)  sql += " And resnum=?";
//    if (filter)  sql += " And flag=?";
//    QSqlQuery query;
//    query.prepare(sql);
//    query.addBindValue(imol);
//    if (chain != NOCHAIN) query.addBindValue(QString(chain));
//    if (resnum)  query.addBindValue(resnum);
//    if (filter)  {
//      query.addBindValue(filter);
//    }
//    if (!query.exec()) tellError(query);
//    return query;
//  }
//  residueRecord Db::nextResidue(QSqlQuery qres) {
//    residueRecord residue;
//    if (qres.next()) {
//      residue.number = qres.value(0).toInt();
//      residue.name   = qres.value(1).toString();
//      residue.valid = true;
//    } else {
//      residue.valid = false;
//    }
//    return residue;
//  }

QSqlQuery Db::iterBonds(int imol, int resnum, char chain, int filter, int hydrogens) {
#ifdef DEBUG
    qDebug() << "Db::iterBonds";
#endif
    //QString sql = "Select atom.x,atom.y,atom.z,atom.atnum,atom.resnum,atom.chain,atom.hetatm, b.x,b.y,b.z,b.atnum,b.resnum,b.chain,b.hetatm From main.bond Join main.atom a On (atom.atid=aid And atom.molid=bond.molid) Join main.atom b On (b.atid=bid And b.molid=bond.molid) Where bond.molid=(Select ifnull(model, molid) From main.molecule Where molid=?) And atom.atnum>? And b.atnum>?";
    //QString sql = "Select atom.x,atom.y,atom.z,atom.atnum,atom.resnum,atom.chain,atom.pcharge,atom.hetatm, b.x,b.y,b.z,b.atnum,b.resnum,b.chain,b.pcharge,b.hetatm From (Select aid,bid From main.bond Where bond.molid=(Select Case When type='pdb' Then model Else molid End From main.molecule Where molid=?)) as bond Join main.atom a On atom.atid=aid Join main.atom b On b.atid=bid And atom.molid=b.molid And atom.molid=? And atom.atnum>? And b.atnum>?";
    QString sql = "Select a.x,a.y,a.z,a.atnum,a.resnum,a.chain,a.pcharge,a.hetatm, \
            b.x,b.y,b.z,b.atnum,b.resnum,b.chain,b.pcharge,b.hetatm \
            From atom a Join atom b On (aid=a.atid And bid=b.atid And a.molid=b.molid) \
            Join (Select Distinct atom.molid,aid,bid From \
                  (Select molid,aid,bid From bond \
                   Where molid=(Select Case When type='pdb' Then model Else molid End From molecule Where molid=?)) bond \
                  Join atom Using(molid) Where (atid=aid Or atid=bid)";
                  if (chain != NOCHAIN) sql += " And atom.chain='" + QString(chain) + "'";
            if (resnum) sql += " And atom.resnum=" + QString::number(resnum);
    if (filter) {
        QString filterClause = getFilter(filter).sql;
        //qDebug() << filterClause;
        sql += " And (" + filterClause + ")";
    }
    sql += ") Where a.molid=b.molid And a.molid=" + QString::number(imol);
    if (hydrogens == HYDROGEN_HIDE) sql += " And a.atnum>1 And b.atnum>1";

    QSqlQuery query;
    if (!query.prepare(sql)) {
        //qDebug() << sql;
        tellError(query, sql);
    }
    query.bindValue(0,imol);
    if (!query.exec()) tellError(query, sql);
    return query;
}
bondRecord Db::nextBond(QSqlQuery qbond) {
    bondRecord bond;
    if (qbond.next()) {
        bond.ax     = qbond.value(0).toDouble();
        bond.ay     = qbond.value(1).toDouble();
        bond.az     = qbond.value(2).toDouble();
        bond.anum   = qbond.value(3).toInt();
        bond.ares   = qbond.value(4).toInt();
        bond.achain = qbond.value(5).toString().toLocal8Bit().data()[0];
        bond.acharge =qbond.value(6).toDouble();
        bond.ahetatm = qbond.value(7).toInt();
        bond.bx     = qbond.value(8).toDouble();
        bond.by     = qbond.value(9).toDouble();
        bond.bz     = qbond.value(10).toDouble();
        bond.bnum   = qbond.value(11).toInt();
        bond.bres   = qbond.value(12).toInt();
        bond.bchain = qbond.value(13).toString().toLocal8Bit().data()[0];
        bond.bcharge= qbond.value(14).toDouble();
        bond.bhetatm= qbond.value(15).toInt();
        bond.valid = true;
    } else {
        bond.valid = false;
    }
    return bond;
}

QSqlQuery Db::iterChainCoords(int imol, char chain, int filter) {
#ifdef DEBUG
    qDebug() << "Db::iterChainCoords";
#endif
    QSqlQuery query;
    QString sql = "Select x,y,z,resnum,name From main.atom Where molid=? And chain=? And atom.hetatm=0";
    if (filter) {
        QString filterClause = getFilter(filter).sql;
        //qDebug() << filterClause;
        sql += " And " + filterClause;
    }
    sql += " Order By resnum";
    if (!query.prepare(sql)) {
        tellError(query);
    } else {
        query.addBindValue(imol);
        query.addBindValue(QString(chain));
        if (!query.exec()) tellError(query);
    }
    return query;
}
chainCoordRecord Db::nextChainCoord(QSqlQuery qchain) {
    chainCoordRecord chainCoord;
    if (qchain.next()) {
        chainCoord.x      = qchain.value(0).toDouble();
        chainCoord.y      = qchain.value(1).toDouble();
        chainCoord.z      = qchain.value(2).toDouble();
        chainCoord.resnum = qchain.value(3).toInt();
        chainCoord.name   = qchain.value(4).toString();
        chainCoord.valid = true;
    } else {
        chainCoord.valid = false;
    }
    return chainCoord;
}

QSqlQuery Db::iterTriangles(int itemid) {
    QSqlQuery query;
    if (!query.prepare("Select tid,vid From triangle Where itemid=? Order By tid")) tellError(query);
    query.addBindValue(itemid);
    if (!query.exec()) tellError(query);
    return query;
}
triRecord Db::nextTriangle(QSqlQuery qtri) {
    triRecord tri;
    if (qtri.next()) {
        tri.tid  = qtri.value(0).toDouble();
        tri.vid  = qtri.value(1).toDouble();
        tri.valid = true;
    } else {
        tri.valid = false;
    }
    return tri;
}

int Db::countVertices(int itemid) {
    QSqlQuery q;
    q.prepare("Select count(*) from vertex Where itemid=?");
    q.addBindValue(itemid);
    if (q.exec()) {
        q.next();
        return q.value(0).toInt();
    } else {
        tellError(q);
        return -1;
    }
}
QSqlQuery Db::iterVertices(int itemid, int imol, int /*resnum*/, char /*chain*/, int /*filter*/, int /*hydrogen*/, int colorBy, float nearAtom) {
    QSqlQuery query;
    QSqlQuery q;
    //if (!query.prepare("Select x,y,z,nx,ny,nz,atid,atnum From triangle Join atom On atid Where itemid=? order by tid")) tellError(query);
    QString sql;
    if (colorBy == COLOR_BY_CHARGE) {
        sql.sprintf("Select vid,atnum,sum(pcharge*abs(pcharge)/(dx*dx+dy*dy+dz*dz)) e2,vx,vy,vz,nx,ny,nz From (Select pcharge, vid, 0 atnum, v.x vx, v.y vy, v.z vz, nx, ny, nz, a.x-v.x dx, a.y-v.y dy, a.z-v.z dz From atom a Join vertex v On (itemid=%d And molid=%d) ) Group By vid,atnum,vx,vy,vz,nx,ny,vz Order By vid", itemid, imol);
    } else if (colorBy == COLOR_BY_ATOMS) {
        sql.sprintf("Select vid,atnum,0.0,v.x,v.y,v.z,v.nx,v.ny,v.nz from main.vertex v join (select atid,atnum from main.atom where molid=%i) using (atid) where itemid=%i", imol, itemid);
        // this will (slowly-ish) compute actual closest atom to vertex, rather than dbsurf's estimate of min(atomid) of vertex's several atom generators
        //      q.exec("Create Index vx On vertex(itemid,x,y,z)");
        //      if (chain == NOCHAIN) {
        //          sql.sprintf("Select vid,atnum,(dx*dx+dy*dy+dz*dz)d2,x,y,z,nx,ny,nz From (Select v.rowid As vid,a.atnum,a.x-v.x As dx,a.y-v.y As dy,a.z-v.z As dz,v.x,v.y,v.z,nx,ny,nz From atom a,vertex v Where (itemid=%d And molid=%d And dx Between -(%3.2f) and (%3.2f) And dy Between -(%3.2f) and (%3.2f) And dz Between -(%3.2f) and (%3.2f))) Where (dx*dx+dy*dy+dz*dz) < %3.2f Order By vid,d2",itemid, imol, nearAtom, nearAtom, nearAtom, nearAtom, nearAtom, nearAtom, nearAtom*nearAtom);
        //      } else {
        //          sql.sprintf("Select vid,atnum,(dx*dx+dy*dy+dz*dz)d2,x,y,z,nx,ny,nz From (Select v.rowid As vid,a.atnum,a.x-v.x As dx,a.y-v.y As dy,a.z-v.z As dz,v.x,v.y,v.z,nx,ny,nz From atom a,vertex v Where (itemid=%d And molid=%d And chain='%c' And dx Between -(%3.2f) and (%3.2f) And dy Between -(%3.2f) and (%3.2f) And dz Between -(%3.2f) and (%3.2f))) Where (dx*dx+dy*dy+dz*dz) < %3.2f Order By vid,d2",itemid, imol, chain, nearAtom, nearAtom, nearAtom, nearAtom, nearAtom, nearAtom, nearAtom*nearAtom);
        //      }
        //      //qDebug() << sql;
    } else {
        sql.sprintf("Select vid,0,0.0,x,y,z,nx,ny,nz From vertex Where itemid=%d Order By vid", itemid);
    }
    if (!query.exec(sql)) tellError(query);
    if (nearAtom > 0.0) q.exec("Drop Index If Exists vx");
    return query;
}
vtxRecord Db::nextVertex(QSqlQuery qv) {
    int vid;
    if (qv.isValid()) {
        vid = qv.value(0).toInt();
    } else {
        // must be on first record
        vid = 0;
    }
    vtxRecord vtx;
    while (qv.next()) {
        vtx.vid   = qv.value(0).toInt();
        if (vtx.vid == vid) continue;  // want next greater vertex id
        if (vtx.vid > vid+1) qDebug() << "skipped vertex at" << vid << vtx.vid;
        vtx.atnum = qv.value(1).toInt();
        vtx.value = qv.value(2).toDouble();
        vtx.x     = qv.value(3).toDouble();
        vtx.y     = qv.value(4).toDouble();
        vtx.z     = qv.value(5).toDouble();
        vtx.nx    = qv.value(6).toDouble();
        vtx.ny    = qv.value(7).toDouble();
        vtx.nz    = qv.value(8).toDouble();
        vtx.valid = true;
        return vtx;
    }
    vtx.valid = false;
    return vtx;
}

float Db::molCenter(int imol, unsigned int resnum, char chain, int filter, /* out */ float *center, /* out */ float *sizes) {
#ifdef DEBUG
    qDebug() << "Db::molCenter";
#endif
    float cmin[3], cmax[3];
    molBounds(imol, resnum, chain, filter, cmin, cmax);
    center[0] = (cmax[0] + cmin[0]) / 2.0;
    center[1] = (cmax[1] + cmin[1]) / 2.0;
    center[2] = (cmax[2] + cmin[2]) / 2.0;
    sizes[0]  = (cmax[0] - cmin[0]) / 2.0;
    sizes[1]  = (cmax[1] - cmin[1]) / 2.0;
    sizes[2]  = (cmax[2] - cmin[2]) / 2.0;
    float size = std::max(std::max(cmax[0] - cmin[0], cmax[1] - cmin[1]), cmax[2] - cmin[2]) / 2.0;
    return size;
}

void Db::molBounds(int imol, unsigned int resnum, char chain, int filter, float min[3], float max[3]) {
#ifdef DEBUG
    qDebug() << "Db::molBounds";
#endif
    QString sql = "Select max(x), min(x), max(y), min(y), max(z), min(z) From main.atom Where molid=?";
    if (chain != NOCHAIN) sql += " And chain=?";
    if (resnum)  sql += " And resnum=?";
    //if (filter)  sql += " And flag=?";
    if (filter) {
        QString filterClause = getFilter(filter).sql;
        //qDebug() << filterClause;
        sql += " And " + filterClause;
    }
    QSqlQuery query;
    query.prepare(sql);
    query.addBindValue(imol);
    if (chain != NOCHAIN) query.addBindValue(QString(chain));
    if (resnum)  query.addBindValue(resnum);
//    if (filter)  {
//        //query.addBindValue(filter);
//        query.addBindValue(filter);
//    }
    if (!query.exec()) tellError(query);
    query.next();
    max[0] = query.value(0).toDouble(); min[0] = query.value(1).toDouble();
    max[1] = query.value(2).toDouble(); min[1] = query.value(3).toDouble();
    max[2] = query.value(4).toDouble(); min[2] = query.value(5).toDouble();
}


bool Db::deleteFromTables(int imol) {
    QSqlQuery query;
    query.exec("Pragma foreign_keys");
    query.next();
#ifdef DEBUG
    qDebug() << "deleteFromTables: foreign keys" << query.value(0).toInt();
#endif
    /*  These deletes from atom and bond will be unecesary
      when Cascade delete work in sqlite with foreign keys in version>3.6.19
  */
    /*
    query.prepare("Delete From main.bond Where molid=?");
    query.addBindValue(imol);
    if (!query.exec()) return tellError(query);
    return true;
    query.prepare("Delete From main.bond Where molid In (Select molid From main.molecule where model=?)");
    query.addBindValue(imol);
    if (!query.exec()) tellError(query);

    query.prepare("Delete From main.atom Where molid=?");
    query.addBindValue(imol);
    if (!query.exec()) tellError(query);
    query.prepare("Delete From main.atom Where molid In (Select molid From main.molecule Where model=?)");
    query.addBindValue(imol);
    if (!query.exec()) tellError(query);

    query.prepare("Delete From main.property Where molid=?");
    query.addBindValue(imol);
    if (!query.exec()) tellError(query);

    query.prepare("Delete From main.secondary_structure Where molid=?");
    query.addBindValue(imol);
    if (!query.exec()) tellError(query);

    query.prepare("Delete From main.triangle Where itemid In (Select itemid From main.tree Where imol=?)");
    query.addBindValue(imol);
    if (!query.exec()) tellError(query);

    query.prepare("Delete From main.tree Where imol=?");
    query.addBindValue(imol);
    if (!query.exec()) tellError(query);
    query.prepare("Delete From main.tree Where imol In (Select molid From main.molecule where model=?)");
    query.addBindValue(imol);
    if (!query.exec()) tellError(query);
*/

    query.prepare("Delete From main.molecule Where molid=? Or model=?");
    query.addBindValue(imol);
    query.addBindValue(imol);
    if (query.exec()) {
        ChemDb::dropIndexes();
        ChemDb::createAtomIndexes();
        ChemDb::createBondIndexes();
        query.exec("vacuum");
        return true;
    } else {
        tellError(query);
        return false;
    }
}

atomRecord Db::findAtomNear(int imol, char chain, float *xyzw, float range=2.5) {
    int atomid = findAtomIdNear(imol,chain,xyzw,range);
    return getAtom(imol,atomid);
}
int Db::findAtomIdNear(int imol, char chain, float *xyzw, float range=1.0) {

#ifdef DEBUG
    qDebug() << "Db::findAtomNear";
#endif
    if (range > 100.0) return NOATOM; // can't recurse forever!
    int atid = 0;
    QSqlQuery query;
    //query.prepare("Select atid,x-? As dx,y-? As dy,z-? As dz From main.atom Where molid = ? and (chain is null Or chain = ?) And x Between ? And ? And y Between ? And ? And z Between ? And ? Order By (dx*dx+dy*dy+dz*dz) Desc");
    query.prepare("Select atid,x-? As dx,y-? As dy,z-? As dz From main.atom Where molid = ? And x Between ? And ? And y Between ? And ? And z Between ? And ? Order By (dx*dx+dy*dy+dz*dz) Desc");
    query.addBindValue(xyzw[0]); query.addBindValue(xyzw[1]); query.addBindValue(xyzw[2]);
    query.addBindValue(imol);
    //query.addBindValue((QString)chain);
    query.addBindValue(xyzw[0] - range); query.addBindValue(xyzw[0] + range);
    query.addBindValue(xyzw[1] - range); query.addBindValue(xyzw[1] + range);
    query.addBindValue(xyzw[2] - range); query.addBindValue(xyzw[2] + range);
    int nfound = 0;
    if (query.exec()) {
        while (query.next()) {
            atid = query.value(0).toInt();
            ++nfound;
        }
    } else {
        tellError(query);
    }
    //qDebug() << nfound << " near atoms.";
    if (nfound == 0) return findAtomIdNear(imol, chain, xyzw, range*2);
    return atid;
}

//  QString getAtomResName(int imol, int iatom) {
//#ifdef DEBUG
//    qDebug() << "Db::getAtomResName" << imol << iatom;
//#endif
//    QString name;
//    QSqlQuery query;
//    query.prepare("Select resnam From main.atom Where molid=? And atid=?");
//    query.addBindValue(imol);
//    query.addBindValue(iatom);
//    if (query.exec()) {
//      query.next();
//      name = query.value(0).toString();
//    } else {
//      tellError(query);
//    }
//    return name;
//  }

int Db::getAtomResNum(int imol, int iatom) {
#ifdef DEBUG
    qDebug() << "Db::getAtomResNum" << imol << iatom;
#endif
    return getAtom(imol,iatom).resnum;
}

char Db::getAtomChain(int imol, int iatom) {
#ifdef DEBUG
    qDebug() << "Db::getAtomChain" << imol << iatom;
#endif
    return getAtom(imol,iatom).chain;
}

//  void getAtomCoord(int imol, int iatom, float *center) {
//#ifdef DEBUG
//    qDebug() << "Db::getAtomCoord" << imol << iatom;
//#endif
//    QSqlQuery query;
//    query.prepare("Select x,y,z From main.atom Where molid=? And atid=?");
//    query.addBindValue(imol);
//    query.addBindValue(iatom);
//    if (query.exec()) {
//      query.next();
//      center[0] = query.value(0).toDouble();
//      center[1] = query.value(1).toDouble();
//      center[2] = query.value(2).toDouble();
//    } else {
//      tellError(query);
//    }
//  }

QString Db::getAtomName(int imol, int iatom) {
#ifdef DEBUG
    qDebug() << "Db::getAtomName" << imol << iatom;
#endif
    return getAtom(imol,iatom).name;
}

int Db::getMolIdFromInchiKey(QString inchikey) {
    int id = -1;
    QSqlQuery query;
    query.prepare("Select molid From main.molecule Where inchikey=?");
    query.addBindValue((QString)inchikey);
    if (query.exec()) {
        if (query.next()) {
            id = query.value(0).toInt();
        } else {
            id = 0;
        }
    } else {
        //tellError(query);
        id = -1;
    }
    return id;
}

#ifdef PDBREADER
int Db::symbolToNumber(QString symbol) {
    static QString symbols = " HHeLiBe B C N O FNeNaMgAlSi P SClAr KCaScTi VCrMnFeCoNiCuZnGaGeAsSeBrKrRbSr YZrNbMoTcRuRhPdAgCdInSnSbTe IXeCsBaLaCePrNdPmSmEuGdTbDyHoErTmYbLuHfTa WReOsIrPtAuHgTlPbBiPoAtRnFrRaAcThPa UNpPuAmCmBkCfEsFmMdNoLrRfDbSgBhHsMtDsRg";
    QString s = symbol;
    if (!s.startsWith(" ")) s.replace(1,1,symbol.toLower()[1]);
    return symbols.indexOf(s)/2 + 1;
}

void Db::addAtom(QSqlQuery q, int molid, int aidx, int resnum, QString resnam, char altLoc, char icode, int atnum, float x, float y, float z, QString name, char chain, int hetatm) {
    //insertAtom.prepare("Insert Into main.atom (molid, atid, resnum, resnam, atnum, x, y, z, name, chain, hetatm)) Values (?,?,?,?,?,?,?,?,?,?,?)");
    q.addBindValue((int)molid);
    q.addBindValue((int)aidx);
    q.addBindValue((int)resnum);
    q.addBindValue(resnam.trimmed());
    q.addBindValue((QString)altLoc);
    q.addBindValue((QString)icode);
    q.addBindValue((int)atnum);
    q.addBindValue((float)x);
    q.addBindValue((float)y);
    q.addBindValue((float)z);
    q.addBindValue(name.trimmed());
    q.addBindValue((QString)chain);
    q.addBindValue((int)hetatm);
    if (!q.exec()) tellError(q);
}

int Db::processAtom(QString s, QSqlQuery q, int molid) {
    int aidx = 0;
    QString name;
    QString resnam;
    QString symbol;
    QString charge;
    //float occupancy;
    //float tempFactor;
    char icode = '\0';
    char chain = '\0';
    char altLoc = '\0';
    int resnum = 0;
    float x,y,z;
    //sscanf(s.toLocal8Bit().data(), "ATOM  %5d %4s %3s %1c%4d   %8f%8f%8f%*6s%*6s%2s", &aidx, name, resnam, &chain, &resnum, &x, &y, &z, symbol);
    //sscanf(s.toLocal8Bit().data(), "ATOM  %5d %4c%c%3c %c%4d%c   %8f%8f%8f%6f%6f          %2c%2c", &aidx, name, &altLoc, resnam, &chain, &resnum, &icode, &x, &y, &z, &occupancy, &tempFactor, symbol, charge);
    aidx       = s.mid( 7-1,5).toInt();
    name       = s.mid(13-1,4);
    altLoc     = s.mid(17-1,1).toLocal8Bit().data()[0];
    resnam     = s.mid(18-1,3);
    chain      = s.mid(22-1,1).toLocal8Bit().data()[0];
    resnum     = s.mid(23-1,4).toInt();
    icode      = s.mid(27-1,1).toLocal8Bit().data()[0];
    x          = s.mid(31-1,8).toFloat();
    y          = s.mid(39-1,8).toFloat();
    z          = s.mid(47-1,8).toFloat();
    //occupancy  = s.mid(55-1,6).toFloat();
    //tempFactor = s.mid(61-1,6).toFloat();
    symbol     = s.mid(77-1,2);
    charge     = s.mid(79-1,2);
    int atnum=symbolToNumber(symbol);
    //qDebug() << aidx << name << symbol << atnum << resnam << altLoc << icode << chain << resnum << x << y << z;
    int hetatm = 0;
    if (s.startsWith("HETATM")) hetatm = 1;
    addAtom(q, molid, aidx, resnum, resnam, altLoc, icode, atnum, x, y, z, name, chain, hetatm);
    return aidx;
}
int Db::processHetatm(QString s, QSqlQuery q, int molid) {
    return processAtom(s,q,molid);
}
/*
    int aidx = 0;
    char name[5];
    char resnam[4];
    char symbol[3];
    char chain;
    int resnum = 0;
    float x,y,z;
    sscanf(s.toLocal8Bit().data(), "HETATM%5d %4s %3s %1c%4d   %8f%8f%8f%*6s%*6s%2s", &aidx, name, resnam, &chain, &resnum, &x, &y, &z, symbol);
    int atnum = symbolToNumber(symbol);
    addAtom(q, molid, atid+aidx, resnum, resnam, atnum, x, y, z, name, chain);
    //qDebug() << "HETATM " << aidx << name << resnam << chain << resnum << x << y << z;
  }
*/
/*
  void processTer(QString s) {
  }
*/
void Db::addBond(QSqlQuery q, int molid, int aid, int bid, int bo) {
    // insertBond.prepare("Insert Into main.bond (molid, aid, bid, bo) Values (?,?,?,?)");
    q.addBindValue((int)molid);
    q.addBindValue((int)aid);
    q.addBindValue((int)bid);
    q.addBindValue((int)bo);
    if (!q.exec()) tellError(q);
}
void Db::processSsbond(QString s, QSqlQuery q, int molid) {
    int snum = 0;
    int aidx = 0;
    int bidx = 0;
    char aresnam[4];
    char bresnam[4];
    char achain;
    char bchain;
    sscanf(s.toLocal8Bit().data(), "SSBOND%3d %3s %c %4d    %3s %c %4d", &snum, aresnam, &achain, &aidx, bresnam, &bchain, &bidx);
    addBond(q, molid, aidx, bidx, 1);
    qDebug() << "SSBOND " << aresnam << achain << aidx << bresnam << bchain << bidx;
}
void Db::processConect(QString s, QSqlQuery q, int molid) {
    int aidx = 0;
    int bidx = 0;
    int cidx = 0;
    int didx = 0;
    int eidx = 0;
    //sscanf(s.toLocal8Bit().data(), "CONECT%5d%5d%5d%5d%5d", &aidx, &bidx, &cidx, &didx, &eidx);
    aidx = s.mid( 6,5).toInt();
    bidx = s.mid(11,5).toInt();
    cidx = s.mid(16,5).toInt();
    didx = s.mid(21,5).toInt();
    eidx = s.mid(26,5).toInt();
    //qDebug() << "CONECT " << aidx << bidx << cidx << didx << eidx;
    // pdb CONECT description says fields must be in increasing atom # order
    if (aidx) {
        if (aidx<bidx) addBond(q, molid, aidx, bidx, 1);
        if (aidx<cidx) addBond(q, molid, aidx, cidx, 1);
        if (aidx<didx) addBond(q, molid, aidx, didx, 1);
        if (aidx<eidx) addBond(q, molid, aidx, eidx, 1);
    }
}

int Db::readPDB(QString pdb, QString name) {
    // read from string
    std::string spdb = pdb.toStdString();
    std::istringstream is(spdb);
    return readPDB(is, name, pdb.length());
}
int Db::readPDB(QString filename) {
    // read from file
    std::filebuf fb;
    if (!fb.open(filename.toLocal8Bit().data(), std::ios::in)) {
        qDebug() << TR("can not open file") << filename;
        return false;
    }
    int fsize = QFile(filename).size();
    std::istream is(&fb);
    return readPDB(is, filename, fsize);
}

int Db::readPDB(std::istream& is, QString filename, int fsize) {
    // read from stream
#ifdef DEBUG
    printf ("Reading 0 at 1 of %d\n", fsize);
    fflush(stdout);
#endif
    int natom = 0;
    int molid = ChemDb::getMolid();  // last used molid in db
    //int atid  = ChemDb::getAtomid(); // last used atomid in db
    ++molid;
#ifdef DEBUG
    qDebug() << "next molid =" << molid;
#endif
    QSqlQuery query;
    QSqlQuery insertAtom;
    QSqlQuery insertBond;
    insertAtom.prepare("Insert Into main.atom (molid, atid, resnum, resnam, altLoc, icode, atnum, x, y, z, name, chain, hetatm) Values (?,?,?,?,?,?,?,?,?,?,?,?,?)");
    //query.exec("Create Temporary Table tmpbond (aid integer, bid integer, bo integer)");
    query.exec("Create Table tmpbond (molid Integer, aid integer, bid integer, bo integer)");
    insertBond.prepare("Insert Into tmpbond (molid, aid, bid, bo) Values (?,?,?,?)");
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();
    ChemDb::dropIndexes();
    //QTextStream in(&file);
    QString title;
#ifdef DEBUG
    qDebug() << "addAtoms";
#endif
    int imodel = 0;
#define MAXLINE 120
    char line[MAXLINE];
    int mol1;
    newMolecule(molid);
    QMap<QString, QString> property;
    while (is.getline(line, MAXLINE)) {
        QString s = line;
        if (s.startsWith("ATOM  ")) {
            processAtom(s, insertAtom, molid);
            ++natom;
            if (imodel == 0 && natom % 100 == 0) {
                //int atloc = is.tellg()*0.5;
                //printf("Atoms %d at %d of %d\n", natom, atloc, fsize); fflush(stdout);
                //fflush(stdout);
            }
        } else if (s.startsWith("HETATM")) {
            processHetatm(s, insertAtom, molid);
            ++natom;
        } else if (s.startsWith("TITLE ")) {
            if (!title.isNull()) title += " ";
            title += s.mid(10).trimmed();
        } else if (s.startsWith("TER   ")) {
            //processTer(s);
        } else if (s.startsWith("CONECT")) {
            processConect(s, insertBond, molid);
        } else if (s.startsWith("MODEL")) {
            if (imodel > 0) newMolecule(molid);
        } else if (s.startsWith("ENDMDL")) {
            ++imodel;
            int atloc = is.tellg()*0.5;
            printf("Model %d at %d of %d\n", imodel, atloc, fsize); fflush(stdout);
            if (imodel == 1) {
                mol1 = molid;
            } else {
                title.clear();
                QTextStream(&title) << "Model" << imodel;
            }
            updateMolecule(molid, QFileInfo(filename).fileName(), natom, title);
            //atid += pdbatom;
            natom = 0;
            ++molid;
        } else if (s.startsWith("END   ") || s == "END") {
            if (natom > 0) {
                property["TITLE"] = title;
                updateMolecule(molid, QFileInfo(filename).fileName(), natom, title);
                addProperty(molid, property);
                ChemDb::addSecondaryStructure(molid);
                mol1 = molid;
                ++molid;
            }
        } else {
            QString pname = s.mid(0,6);
            if (property.contains(pname)) {
                property[pname] += "\n" + s.mid(6);
            } else {
                property[pname] = s.mid(6);
            }
        }
    }
    db.commit();
    //printf ("Read %d at %d of %d\n", molid-mol1, fsize/2, fsize);
    //fflush(stdout);
#ifdef DEBUG
    printf ("Indexing %d at %d of %d\n", 0, int(fsize*0.5), fsize);
    fflush(stdout);
    qDebug() << "create atom indexes";
#endif
    ChemDb::createAtomIndexes();
#ifdef DEBUG
    printf ("Bonds %d at %d of %d\n", 0, int(fsize*0.6), fsize);
    fflush(stdout);
#endif
    addBonds(mol1, molid);
#ifdef DEBUG
    qDebug() << "create bond indexes";
#endif
    ChemDb::createBondIndexes();

    //checkResidues(mol1);
#ifdef DEBUG
    printf ("File %d at %d of %d\n", 0, int(fsize*0.7), fsize);
    fflush(stdout);
#endif
    return mol1;
}


void Db::addBonds(int mol1, int molid) {
#ifdef DEBUG
    qDebug() << "addBonds" << mol1 << molid;
#endif
    // add bonds as stored in residue.db, attached when db was opened
    QSqlQuery query;
    //query.prepare("Insert Into tmpbond (molid,aid,bid,bo) Select atom.molid,atom.atid,nbr.atid,bo From main.atom Join main.atom nbr On (atom.molid=nbr.molid And atom.chain=nbr.chain And atom.resnum=nbr.resnum And atom.icode=nbr.icode) Join residue.bondname On (atom.name=atom1 And nbr.name=atom2 And atom.resnam=bondname.resnam) Where atom.molid=?");
    bool ok = query.prepare("Insert Into tmpbond (molid,aid,bid,bo) Select molid,aid,bid,bo From (Select atom.molid, atom.atid aid, atom.resnam, atom.name atom1, nbr.atid bid, nbr.name atom2 From atom Join atom nbr Using (molid,chain,resnum,icode,altloc)) Join bondname Using (resnam,atom1,atom2) Where molid=?");
#ifdef DEBUG
    qDebug() << "tmpbond" << mol1 << ok;
#endif
    if (!ok) tellError(query);
    query.addBindValue(mol1);
    if (!query.exec()) tellError(query);
    query.finish();

    // add res(n)C-res(n+1)N residue-residue bonds
    query.prepare("Insert Into tmpbond Select atom.molid,atom.atid,nbr.atid,1 From main.atom Join main.atom nbr On atom.molid=nbr.molid And atom.chain=nbr.chain And nbr.resnum=atom.resnum+1 Where atom.molid=? And atom.name='C' And nbr.name='N' And nbr.hetatm=0");
    query.addBindValue(mol1);
    if (!query.exec()) tellError(query);
    query.finish();

    // add res(n)O3'-res(n+1)P residue-residue bonds; account for insertion code
    query.prepare("Insert Into tmpbond Select atom.molid,atom.atid,nbr.atid,1 From main.atom Join main.atom nbr On atom.molid=nbr.molid And atom.chain=nbr.chain And nbr.resnum=atom.resnum+1 Where atom.molid=? And atom.name='O3''' And nbr.name='P' and abs(atom.x-nbr.x) < 2 and abs(atom.y-nbr.y) < 2 and abs(atom.z-nbr.z) < 2");
    query.addBindValue(mol1);
    if (!query.exec()) tellError(query);
    query.finish();

    query.prepare("Insert Into tmpbond Select atom.molid,atom.atid,nbr.atid,1 From main.atom Join main.atom nbr On atom.molid=nbr.molid And atom.chain=nbr.chain And nbr.resnum=atom.resnum Where atom.molid=? And atom.name='O3''' And nbr.name='P' and abs(atom.x-nbr.x) < 2 and abs(atom.y-nbr.y) < 2 and abs(atom.z-nbr.z) < 2");
    query.addBindValue(mol1);
    if (!query.exec()) tellError(query);
    query.finish();

    // now copy unique records From tmpbond into bond;  CONECT and info From residues can conflict, so choose larger bond order
#ifdef DEBUG
    qDebug() << "finalBonds for molids" << mol1 << " to " << molid-1;
#endif
    ok = query.prepare("Insert Into bond (molid,aid,bid,bo) Select molid,aid,bid,Max(bo) From tmpbond Where molid Between ? And ? Group By molid,aid,bid");
    if (ok) {
        query.addBindValue(mol1);
        query.addBindValue(molid-1);
        if (!query.exec()) tellError(query);
    } else {
        tellError(query);
    }
    query.finish();

    // add bond count to molecule table
    query.prepare("Update main.molecule Set nbonds=(Select count(rowid) from main.bond Where molid=?) Where molid=?");
    query.addBindValue(mol1);
    query.addBindValue(mol1);
    query.exec();

    // record parent model for multi-model pdb files
    query.prepare("Update main.molecule Set model=? Where molid>=?");
    query.addBindValue(mol1);
    query.addBindValue(mol1);
    query.exec();

    query.exec("Delete from tmpbond");
}

void Db::newMolecule(int molid) {
    // make dummy entry to accomodate foreign key of atoms and bonds
    QSqlQuery query;
    query.prepare("Insert into main.molecule (molid) Values (?)");
    query.addBindValue((int)molid);
    query.exec();
}

void Db::updateMoleculeFile(int molid, QString file) {
    QSqlQuery query;
    query.prepare("Update main.molecule Set file=? Where molid=?");
    query.addBindValue(file);
    query.addBindValue(molid);
    if (!query.exec()) tellError(query);
}
void Db::updateMolecule(int molid, QString file, int natom, QString title) {
    QSqlQuery query;
    /*
    // remove dummy entry
    query.prepare("Delete from main.molecule Where molid=?");
    query.addBindValue((int)molid);
    query.exec();
*/
#ifdef DEBUG
    qDebug() << "updateMolecule" << molid << title;
#endif
    //    query.prepare("Insert Into main.molecule (molid, file, type, title, natoms, nresidue, nbonds) Values (?,?,?,?,?,(Select Count(Distinct chain||resnum) From main.atom Where molid=?),(Select Count(aid) From main.bond Where molid=?))");
    //    query.addBindValue((int)molid);
    //    query.addBindValue(file);
    //    query.addBindValue(QString("pdb"));
    //    query.addBindValue((QString)title);
    //    query.addBindValue((int)natom);
    //    query.addBindValue((int)molid);
    //    query.addBindValue((int)molid);
    query.prepare("Update main.molecule Set file=?, type=?, title=?, natoms=?, nresidue=(Select Count(Distinct chain||resnum) From main.atom Where molid=?) Where molid=? ");
    query.addBindValue(file);
    query.addBindValue(QString("pdb"));
    query.addBindValue((QString)title);
    query.addBindValue((int)natom);
    query.addBindValue((int)molid);
    query.addBindValue((int)molid);
    if (!query.exec()) tellError(query);
    //qDebug() << query.boundValues();
#ifdef DEBUG
    qDebug() << "addFlags";
#endif
    //ChemDb::addFlags(molid);
#ifdef DEBUG
    qDebug() << natom << " atoms.";
#endif
}

bool Db::addProperty(int molid, QMap<QString,QString> property) {
    qDebug() << "Properties: " << property.keys();
    QSqlQuery query;
    query.prepare("Insert Into property (molid, name, value) Values (?,?,?)");
    //QMapIterator<QString,QString> it(property);
    QMap<QString,QString>::const_iterator it = property.constBegin();
    while (it != property.constEnd()) {
        query.addBindValue((int)molid);
        query.addBindValue(it.key().trimmed());
        query.addBindValue(it.value());
        if (!query.exec()) return false;
        ++it;
    }
    return true;
}

#endif
// end PDBREADER
void Db::checkResidues(int molid) {
    QSqlQuery query;
    //query.prepare("Select Distinct resnam From main.atom Where molid=? Except Select Distinct res From residue.atom");
    query.prepare("Select Distinct resnam From main.atom Where resnam Is Not Null And molid=? Except Select title From residue.molecule");
    query.addBindValue((int)molid);
    query.exec();
    while (query.next()) {
        qDebug() << "unknown residue" << query.value(0).toString() << " ";
    }
}
