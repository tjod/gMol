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
    ;
}
Db::~Db(){
    ;
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

int Db::maxItemId() {
    int itemid = 0;
    QSqlQuery query;
    query.prepare("Select max(itemid) From tree");
    if (!query.exec()) tellError(query);
    query.next();
    itemid = query.value(0).toInt();
    return itemid;
}

int Db::numRows(QSqlQuery query) {
//      workaround for sqlite (numRows not implemented in driver) to get number of rows in a query result.
//      should be called before first result is retrieved, since this method reset result set to before
//      the first result

    int current = query.at();
    if (query.last()) {
        int nrows = query.at() + 1;
        query.seek(current);
        return nrows;
    } else {
        return 0;
    }
}

void Db::grampsSaveEmpty() {
    QSqlQuery("Delete From gramps_save");
}

bool Db::gramspSave(QString grampsOutput) {
    QSqlQuery query;
    query.prepare("Insert Into gramps_save (command) Values (?)");
    query.addBindValue(grampsOutput);
    return query.exec();
}
QStringList Db::getGrampsSave() {
    QStringList commands;
    QSqlQuery query = QSqlQuery("Select command from gramps_save Order by iorder");
    if (query.exec()) {
        while (query.next()) {
            commands << query.value(0).toString();
        }
    }
    return commands;
}

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
    return iterChainCounts(imol, NOCHAIN);
}

QSqlQuery Db::iterChainCounts(int imol, char chain) {
    QString pattern = "Select chain, '%1' name, Count(Distinct(resnum)) From atom Where molid=" + QString::number(imol) + " %2";
    if (chain != NOCHAIN) {
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
    char lastChain = NOCHAIN;
//    for (int i=0; i < filters.count(); ++i) {
//        if (!qchain.next()) break;
    while (qchain.next()) {
        char chain = qchain.value(0).toString().toLocal8Bit().data()[0];
        if (lastChain == NOCHAIN) {
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

float Db::molCenter(int imol, unsigned int resnum, char chain, int filter, /* out */ float *midpoint, /* out */ float *sizes, /* out */ float *avg) {
#ifdef DEBUG
    qDebug() << "Db::molCenter";
#endif
    float cmin[3], cmax[3];
    molBounds(imol, resnum, chain, filter, cmin, cmax, avg);
    midpoint[0] = (cmax[0] + cmin[0]) / 2.0;
    midpoint[1] = (cmax[1] + cmin[1]) / 2.0;
    midpoint[2] = (cmax[2] + cmin[2]) / 2.0;
    sizes[0]  = (cmax[0] - cmin[0]) / 2.0;
    sizes[1]  = (cmax[1] - cmin[1]) / 2.0;
    sizes[2]  = (cmax[2] - cmin[2]) / 2.0;
    float size = std::max(std::max(cmax[0] - cmin[0], cmax[1] - cmin[1]), cmax[2] - cmin[2]) / 2.0;
    return size;
}

void Db::molBounds(int imol, unsigned int resnum, char chain, int filter, float min[3], float max[3], float avg[3]) {
#ifdef DEBUG
    qDebug() << "Db::molBounds";
#endif
    QString sql = "Select max(x), min(x), max(y), min(y), max(z), min(z), avg(x), avg(y), avg(z) From atom Where molid=?";
    if (chain != NOCHAIN) sql += " And chain=?";
    if (resnum != NORESNUM)  sql += " And resnum=?";
    if (filter) {
        QString filterClause = getFilter(filter).sql;
        sql += " And " + filterClause;
    }
    QSqlQuery query;
    query.prepare(sql);
    query.addBindValue(imol);
    if (chain != NOCHAIN)   query.addBindValue(QString(chain));
    if (resnum != NORESNUM) query.addBindValue(resnum);

    if (!query.exec()) tellError(query);
    query.next();
    max[0] = query.value(0).toFloat(); min[0] = query.value(1).toFloat();
    max[1] = query.value(2).toFloat(); min[1] = query.value(3).toFloat();
    max[2] = query.value(4).toFloat(); min[2] = query.value(5).toFloat();
    avg[0] = query.value(6).toFloat();
    avg[1] = query.value(7).toFloat();
    avg[2] = query.value(8).toFloat();    
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
    query.prepare("Delete From bond Where molid=?");
    query.addBindValue(imol);
    if (!query.exec()) return tellError(query);
    return true;
    query.prepare("Delete From bond Where molid In (Select molid From molecule where model=?)");
    query.addBindValue(imol);
    if (!query.exec()) tellError(query);

    query.prepare("Delete From atom Where molid=?");
    query.addBindValue(imol);
    if (!query.exec()) tellError(query);
    query.prepare("Delete From atom Where molid In (Select molid From molecule Where model=?)");
    query.addBindValue(imol);
    if (!query.exec()) tellError(query);

    query.prepare("Delete From property Where molid=?");
    query.addBindValue(imol);
    if (!query.exec()) tellError(query);

    query.prepare("Delete From secondary_structure Where molid=?");
    query.addBindValue(imol);
    if (!query.exec()) tellError(query);

    query.prepare("Delete From triangle Where itemid In (Select itemid From tree Where imol=?)");
    query.addBindValue(imol);
    if (!query.exec()) tellError(query);

    query.prepare("Delete From tree Where imol=?");
    query.addBindValue(imol);
    if (!query.exec()) tellError(query);
    query.prepare("Delete From tree Where imol In (Select molid From molecule where model=?)");
    query.addBindValue(imol);
    if (!query.exec()) tellError(query);
*/

    query.prepare("Delete From molecule Where molid=? Or model=?");
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

int Db::getMolIdFromInchiKey(QString inchikey) {
    int id = -1;
    QSqlQuery query;
    query.prepare("Select molid From molecule Where inchikey=?");
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
    //insertAtom.prepare("Insert Into atom (molid, atid, resnum, resnam, atnum, x, y, z, name, chain, hetatm)) Values (?,?,?,?,?,?,?,?,?,?,?)");
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
    char chain = NOCHAIN;
    char altLoc = '\0';
    int resnum = NORESNUM;
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
  void processTer(QString s) {
  }
*/
void Db::addBond(QSqlQuery q, int molid, int aid, int bid, int bo) {
    // insertBond.prepare("Insert Into bond (molid, aid, bid, bo) Values (?,?,?,?)");
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

int Db::readPDB(std::istream& is, QString filename, int fsize=0) {
    // read from stream
    if (fsize){} // get rid of warning
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
    insertAtom.prepare("Insert Into atom (molid, atid, resnum, resnam, altLoc, icode, atnum, x, y, z, name, chain, hetatm) Values (?,?,?,?,?,?,?,?,?,?,?,?,?)");
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
    int iline = 0;
    int imodel = 0;
#define MAXLINE 120
    char line[MAXLINE];
    int mol1 = 0;
    newMolecule(molid);
    while (is.getline(line, MAXLINE)) {
        QString s = line;
        ++iline;
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
            addProperty(molid, s.mid(0,6), s.mid(6), iline);
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
            //int atloc = is.tellg()*0.5;
            //printf("Model %d at %d of %d\n", imodel, atloc, fsize); fflush(stdout);
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
                updateMolecule(molid, QFileInfo(filename).fileName(), natom, title);
                ChemDb::addSecondaryStructure(molid);
                mol1 = molid;
                ++molid;
            }
        } else {
            addProperty(molid, s.mid(0,6), s.mid(6), iline);
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
    //query.prepare("Insert Into tmpbond (molid,aid,bid,bo) Select atom.molid,atom.atid,nbr.atid,bo From atom Join atom nbr On (atom.molid=nbr.molid And atom.chain=nbr.chain And atom.resnum=nbr.resnum And atom.icode=nbr.icode) Join residue.bondname On (atom.name=atom1 And nbr.name=atom2 And atom.resnam=bondname.resnam) Where atom.molid=?");
    //bool ok = query.prepare("Insert Into tmpbond (molid,aid,bid,bo) Select molid,aid,bid,bo From (Select atom.molid, atom.atid aid, atom.resnam, atom.name atom1, nbr.atid bid, nbr.name atom2 From atom Join atom nbr Using (molid,chain,resnum,icode,altloc)) Join bondname Using (resnam,atom1,atom2) Where molid=?");
    bool ok = query.prepare("Insert Into tmpbond (molid,aid,bid,bo) Select molid,aid,bid,bo From (Select atom.molid, atom.atid aid, atom.resnam, atom.name atom1, nbr.atid bid, nbr.name atom2 From atom Join atom nbr Using (molid,chain,resnum,icode) Where atom.altloc=nbr.altloc or atom.altloc=' ' or nbr.altloc=' ') Join bondname Using (resnam,atom1,atom2) Where molid=?");
#ifdef DEBUG
    qDebug() << "tmpbond" << mol1 << ok;
#endif
    if (!ok) tellError(query);
    query.addBindValue(mol1);
    if (!query.exec()) tellError(query);
    query.finish();

    // add res(n)C-res(n+1)N residue-residue bonds
    query.prepare("Insert Into tmpbond Select atom.molid,atom.atid,nbr.atid,1 From atom Join atom nbr On atom.molid=nbr.molid And atom.chain=nbr.chain And nbr.resnum=atom.resnum+1 Where atom.molid=? And atom.name='C' And nbr.name='N' And nbr.hetatm=0");
    query.addBindValue(mol1);
    if (!query.exec()) tellError(query);
    query.finish();

    // add res(n)O3'-res(n+1)P residue-residue bonds; account for insertion code
    query.prepare("Insert Into tmpbond Select atom.molid,atom.atid,nbr.atid,1 From atom Join atom nbr On atom.molid=nbr.molid And atom.chain=nbr.chain And nbr.resnum=atom.resnum+1 Where atom.molid=? And atom.name='O3''' And nbr.name='P' and abs(atom.x-nbr.x) < 2 and abs(atom.y-nbr.y) < 2 and abs(atom.z-nbr.z) < 2");
    query.addBindValue(mol1);
    if (!query.exec()) tellError(query);
    query.finish();

    query.prepare("Insert Into tmpbond Select atom.molid,atom.atid,nbr.atid,1 From atom Join atom nbr On atom.molid=nbr.molid And atom.chain=nbr.chain And nbr.resnum=atom.resnum Where atom.molid=? And atom.name='O3''' And nbr.name='P' and abs(atom.x-nbr.x) < 2 and abs(atom.y-nbr.y) < 2 and abs(atom.z-nbr.z) < 2");
    query.addBindValue(mol1);
    if (!query.exec()) tellError(query);
    query.finish();

    // now copy unique records From tmpbond into bond;  CONECT and info From residues can conflict, so choose larger bond order
#ifdef DEBUG
    qDebug() << "finalBonds for molids" << mol1 << " to " << molid-1;
#endif
    ok = query.prepare("Insert Into bond (molid,aid,bid,bo) Select Distinct molid,aid,bid,Max(bo) From tmpbond Where molid Between ? And ? Group By molid,aid,bid");
    if (ok) {
        query.addBindValue(mol1);
        query.addBindValue(molid-1);
        if (!query.exec()) tellError(query);
    } else {
        tellError(query);
    }
    query.finish();

    // add bond count to molecule table
    query.prepare("Update molecule Set nbonds=(Select count(rowid) from bond Where molid=?) Where molid=?");
    query.addBindValue(mol1);
    query.addBindValue(mol1);
    query.exec();

    // record parent model for multi-model pdb files
    query.prepare("Update molecule Set model=? Where molid>=?");
    query.addBindValue(mol1);
    query.addBindValue(mol1);
    query.exec();

    query.exec("Delete from tmpbond");
}

void Db::newMolecule(int molid) {
    // make dummy entry to accomodate foreign key of atoms and bonds
    QSqlQuery query;
    query.prepare("Insert into molecule (molid) Values (?)");
    query.addBindValue((int)molid);
    query.exec();
}

void Db::updateMoleculeFile(int molid, QString file) {
    QSqlQuery query;
    query.prepare("Update molecule Set file=? Where molid=?");
    query.addBindValue(file);
    query.addBindValue(molid);
    if (!query.exec()) tellError(query);
}
void Db::updateMolecule(int molid, QString file, int natom, QString title) {
    QSqlQuery query;
#ifdef DEBUG
    qDebug() << "updateMolecule" << molid << title;
#endif
    query.prepare("Update molecule Set file=?, type=?, title=?, natoms=?, nresidue=(Select Count(Distinct chain||resnum) From atom Where molid=?) Where molid=? ");
    query.addBindValue(file);
    query.addBindValue(QString("pdb"));
    query.addBindValue((QString)title);
    query.addBindValue((int)natom);
    query.addBindValue((int)molid);
    query.addBindValue((int)molid);
    if (!query.exec()) tellError(query);
#ifdef DEBUG
    qDebug() << "addFlags";
#endif
#ifdef DEBUG
    qDebug() << natom << " atoms.";
#endif
}

bool Db::addProperty(int molid, QString name, QString value, int irecord) {
    QSqlQuery query;
    query.prepare("Insert Into property (molid, name, value, record) Values (?,?,?,?)");
    query.addBindValue((int)molid);
    query.addBindValue(name.trimmed());
    query.addBindValue(value);
    query.addBindValue(irecord);
    return query.exec();
}

#endif
// end PDBREADER
void Db::checkResidues(int molid) {
    QSqlQuery query;
    //query.prepare("Select Distinct resnam From atom Where molid=? Except Select Distinct res From residue.atom");
    query.prepare("Select Distinct resnam From atom Where resnam Is Not Null And molid=? Except Select title From residue.molecule");
    query.addBindValue((int)molid);
    query.exec();
    while (query.next()) {
        qDebug() << "unknown residue" << query.value(0).toString() << " ";
    }
}

propertyQuery::propertyQuery() {}
propertyQuery::~propertyQuery() {}

bool propertyQuery::iter(int qmol) {
    if (prepare("Select molid,name,value,record from property where molid=? order by record asc")) {
        addBindValue(qmol);
        if (exec()) valid = true; // don't call next() for this iter method
    } else {
        Db::tellError(*this);
        valid = false;
    }
    return false;
}
bool propertyQuery::next() {
    if(QSqlQuery::next()) {
        imol  = value(0).toInt();
        name  = value(1).toString();
        text  = value(2).toString();
        order = value(3).toInt();
        valid = true;
    } else {
        valid = false;
    }
    return valid;
}

molQuery::molQuery() {;}
molQuery::~molQuery() {;}

bool molQuery::get(int qmol) {
    // get record for input qmol
    if (prepare("Select molid,file,title,type,nresidue,natoms,nbonds,model From molecule Where molid=?")) {
        addBindValue(qmol);
        if (exec()) {
            valid = next();
            this->finish();
        }
    } else {
        Db::tellError(*this);
        valid = false;
    }
    return valid;
}

bool molQuery::getInFile(int qmol) {
    // get all molecules in the same file as input qmol
    if (prepare("Select molid,file,title,type,nresidue,natoms,nbonds,model From molecule Where file=(Select file From molecule Where molid=?) Order By molid Asc")) {
        addBindValue(qmol);
        if (exec()) valid = true; // don't call next for this iter method
    } else {
        Db::tellError(*this);
        valid = false;
    }
    return valid;
}

bool molQuery::getByFile() {
    // iterate over all molecules (actually just first mol of the file), grouped by file they came from
    if (prepare("Select min(molid),file,title,type,nresidue,natoms,nbonds,model From molecule Group By file Order By molid Asc")) {
        if (exec()) valid = true; // don't call next for this iter method
    } else {
        Db::tellError(*this);
        valid = false;
    }
    return valid;
}

bool molQuery::next() {
    if (QSqlQuery::next()) {
        imol     = value(0).toInt();
        filename = value(1).toString();
        title    = value(2).toString();
        type     = value(3).toString();
        nresidue = value(4).toInt();
        natoms   = value(5).toInt();
        nbonds   = value(6).toInt();
        model    = value(7).toInt();
        valid    = true;
    } else {
        valid = false;
    }
    return valid;
}

atomQuery::atomQuery() {
    molid   = NOMOL;
    atid    = NOATOM;
    resnum  = NORESNUM;
    resnam  = "";
    altLoc  = '\0';
    icode   = '\0';
    atnum   = NOATOM;
    x       = 0.0;
    y       = 0.0;
    z       = 0.0;
    fcharge = 0.0;
    pcharge = 0.0;
    name    = "";
    chain   = NOCHAIN;
    hetatm  = NOATOM;
    nfound  = 0;
    frange   = 0.0;
    valid = true;
}
atomQuery::~atomQuery() {;}

int atomQuery::count() {
    return Db::numRows(*this);
}
bool atomQuery::get(int qmol, int qatom) {
    //atomRecord Db::getAtom(int imol, int iatom) {
    QString sql = "Select molid,atid,resnum,resnam,altLoc,icode,atnum,x,y,z,fcharge,pcharge,name,chain,hetatm From atom Where molid=? And atid=?";
    if (prepare(sql)) {
        addBindValue(qmol);
        addBindValue(qatom);
        if (exec()) {
            valid = next();
            this->finish();
        }
    } else {
        Db::tellError(*this);
        valid = false;
    }
    return valid;
}

bool atomQuery::iter(int qmol, int qresnum=NORESNUM, char qchain=NOCHAIN, int qfilter=0, int qhydrogens=HYDROGEN_NONE) {
    //QSqlQuery Db::iterAtoms(int imol, int resnum, char chain, int filter, int hydrogens) {
#ifdef DEBUG
    qDebug() << "Db::iterAtoms";
#endif
    QString sql = atomSql(qmol, qresnum, qchain, qfilter, qhydrogens);
    if (prepare(sql) && exec()) {
        valid = true; // don't call next for this iter method
    } else {
        Db::tellError(*this);
        valid = false;
    }
    return valid;
}
QString atomQuery::atomSql(int qmol, int qresnum, char qchain, int qfilter, int qhydrogens) {
    QString sql = "Select molid,atid,resnum,resnam,altLoc,icode,atnum,x,y,z,fcharge,pcharge,name,chain,hetatm";
    sql += " From atom Where " + atomClause(qmol, qresnum, qchain, qfilter, qhydrogens);
    return sql;
}
QString atomQuery::atomClause(int qmol, int qresnum, char qchain, int qfilter, int qhydrogens) {
    
    QString sql = "molid=" + QString::number(qmol);
    if (qchain != NOCHAIN)   sql += " And chain='" + QString(qchain) + "'";
    if (qresnum != NORESNUM) sql += " And resnum=" + QString::number(qresnum);
    if (qhydrogens == HYDROGEN_HIDE) {
        sql += " And atnum>1";
    } else if (qhydrogens == HYDROGEN_COUNT) {
        // very special case used once to determine hcount
        sql += " And atnum=1";
    }
    if (qfilter) {
        QString filterClause = Db::getFilter(qfilter).sql;
        sql += " And (" + filterClause + ")";
    }
    return sql;
}

QString atomQuery::getResnam(int imol, int resnum, char chain) {
#ifdef DEBUG
    qDebug() << "Db::molResName";
#endif
    QString name = "";
    QSqlQuery query;
    if (query.prepare("Select Distinct resnam From atom Where molid=? And resnum=? And chain=?")) {
        query.addBindValue(imol);
        query.addBindValue(resnum);
        query.addBindValue((QString)chain);
        if (query.exec()) {
            query.next();
            name = query.value(0).toString();
            //this->finish();
            return name;
        }
    }
    Db::tellError(query);
    return name;
}

bool atomQuery::near(int qmol, char qchain, float *xyzw, float range=2.5) {
    int atomid = findAtomIdNear(qmol,qchain,xyzw,range);
    if (atomid != NOATOM) get(qmol,atomid); // will set valid
    //this->finish();
    return valid;
}

int atomQuery::findAtomIdNear(int qmol, char qchain, float *xyzw, float range) {
#ifdef DEBUG
    qDebug() << "Db::findAtomNear";
#endif
    if (range > 50.0) return NOATOM; // can't recurse forever!
    QSqlQuery query;
    //query.prepare("Select atid,x-? As dx,y-? As dy,z-? As dz From atom Where molid = ? and (chain is null Or chain = ?) And x Between ? And ? And y Between ? And ? And z Between ? And ? Order By (dx*dx+dy*dy+dz*dz) Desc");
    QString sql = "Select atid,x-? As dx,y-? As dy,z-? As dz From atom Where molid = ? And x Between ? And ? And y Between ? And ? And z Between ? And ? Order By (dx*dx+dy*dy+dz*dz) Desc";
    if (!query.prepare(sql)) return false;
    query.addBindValue(xyzw[0]); query.addBindValue(xyzw[1]); query.addBindValue(xyzw[2]);
    query.addBindValue(qmol);
    //query.addBindValue((QString)chain);
    query.addBindValue(xyzw[0] - range); query.addBindValue(xyzw[0] + range);
    query.addBindValue(xyzw[1] - range); query.addBindValue(xyzw[1] + range);
    query.addBindValue(xyzw[2] - range); query.addBindValue(xyzw[2] + range);
    nfound = 0;
    frange = range;
    int atomid = NOATOM;
    if (query.exec()) {
        while (query.next()) {
            atomid = query.value(0).toInt();
            ++nfound;
        }
    } else {
        Db::tellError(query);
        return NOATOM;
    }
    //qDebug() << nfound << " near atoms.";
    if (nfound == 0) return findAtomIdNear(qmol, qchain, xyzw, range*2);
    return atomid;
}
bool atomQuery::iterNear(int atomid, float range=2.5) {
    QString sql = "With a As (Select atid,molid,chain,resnam,resnum,name,x,y,z,? range From atom Where atid=?), \
      nearNonBonded As (Select a.atid root, b.atid nearid From atom b join a Where a.atid != b.atid \
      And Case a.molid = b.molid When a.chain = b.chain Then a.resnum != b.resnum End \
      And (Case When a.name In (Select name from donor) Then b.name In (Select name from acceptor) \
                When a.name In (Select name from acceptor) Then b.name In (Select name from donor) End) \
      And b.x Between a.x-range And a.x+range \
      And b.y Between a.y-range And a.y+range \
      And b.z Between a.z-range And a.z+range Except Select aid,bid From bond) \
      Select molid,atid,resnum,resnam,altLoc,icode,atnum,x,y,z,fcharge,pcharge,name,chain,hetatm \
      From atom Join nearNonBonded On (nearid=atid)";
    //qDebug() << sql;
    if (!prepare(sql)) return false;
    addBindValue(range);
    addBindValue(atomid);
    if (exec()) {
        valid = true; // don't call next for this iter method
    } else {
        Db::tellError(*this);
        valid = false;
    }
    return valid;
}

int atomQuery::hcount(int qmol, int qresnum, char qchain, int qfilter) {
    int hcount = -1;
    QSqlQuery query;
    QString sql = "Select count(*) From (" + atomSql(qmol, qresnum, qchain, qfilter, HYDROGEN_COUNT) + ") hcount";
    if (query.exec(sql) && query.next()) {
        hcount = query.value(0).toInt();
    }
    //this->finish();
    return hcount;
}

bool atomQuery::next() {
    if (QSqlQuery::next()) {
        molid   = value(0).toInt();
        atid    = value(1).toInt();
        QVariant restmp = value(2);
        resnum  = (restmp.isNull()) ? NORESNUM : restmp.toInt();
        resnam  = value(3).toString();
        altLoc  = value(4).toString().toLocal8Bit().data()[0];
        icode   = value(5).toString().toLocal8Bit().data()[0];
        atnum   = value(6).toInt();
        x       = value(7).toDouble();
        y       = value(8).toDouble();
        z       = value(9).toDouble();
        fcharge = value(10).toDouble();
        pcharge = value(11).toDouble();
        name    = value(12).toString();
        chain   = value(13).toString().toLocal8Bit().data()[0];
        hetatm  = value(14).toInt();
        valid = true;
    } else {
        valid = false;
    }
    return valid;
}

triangleQuery::triangleQuery() {}
triangleQuery::~triangleQuery() {}

bool triangleQuery::iter(int itemid) {
    if (prepare("Select tid,vid From triangle Where itemid=? Order By tid,tvid")) {
        addBindValue(itemid);
        if (exec()) valid = true;
    } else {
        Db::tellError(*this);
    }
    return valid;
}
bool triangleQuery::next() {
    if (QSqlQuery::next()) {
        tid  = value(0).toInt();
        vid  = value(1).toInt();
        valid = true;
    } else {
        valid = false;
    }
    return valid;
}
int triangleQuery::count(int itemid) {
    QSqlQuery q;
    q.prepare("Select count(*) from triangle Where itemid=?");
    q.addBindValue(itemid);
    if (q.exec()) {
        q.next();
        return q.value(0).toInt();
    } else {
        Db::tellError(q);
        return -1;
    }
}

vertexQuery::vertexQuery() {}
vertexQuery::~vertexQuery() {}

bool vertexQuery::iter(int itemid, int imol, int /*resnum*/, char /*chain*/, int /*filter*/, int /*hydrogen*/, int colorBy, float /*nearAtom*/) {
    QString sql;
    if (colorBy == COLOR_BY_CHARGE) {
        sql.sprintf("Select vid,atnum,sum(pcharge*abs(pcharge)/(dx*dx+dy*dy+dz*dz)) e2,vx,vy,vz,nx,ny,nz From (Select pcharge, vid, 0 atnum, v.x vx, v.y vy, v.z vz, nx, ny, nz, a.x-v.x dx, a.y-v.y dy, a.z-v.z dz From atom a Join vertex v On (itemid=%d And molid=%d) ) Group By vid,atnum,vx,vy,vz,nx,ny,vz Order By vid", itemid, imol);
    } else if (colorBy == COLOR_BY_ATOMS) {
        sql.sprintf("Select vid,atnum,0.0,v.x,v.y,v.z,v.nx,v.ny,v.nz From vertex v Join (Select atid,atnum From atom Where molid=%i) Using (atid) Where itemid=%i Order By vid", imol, itemid);
        // this will (slowly-ish) compute actual closest atom to vertex, rather than dbsurf's estimate of min(atomid) of vertex's several atom generators
        //      q.exec("Create Index vx On vertex(itemid,x,y,z)");
        //      if (chain == NOCHAIN) {
        //          sql.sprintf("Select vid,atnum,(dx*dx+dy*dy+dz*dz)d2,x,y,z,nx,ny,nz From (Select v.rowid As vid,a.atnum,a.x-v.x As dx,a.y-v.y As dy,a.z-v.z As dz,v.x,v.y,v.z,nx,ny,nz From atom a,vertex v Where (itemid=%d And molid=%d And dx Between -(%3.2f) and (%3.2f) And dy Between -(%3.2f) and (%3.2f) And dz Between -(%3.2f) and (%3.2f))) Where (dx*dx+dy*dy+dz*dz) < %3.2f Order By vid,d2",itemid, imol, nearAtom, nearAtom, nearAtom, nearAtom, nearAtom, nearAtom, nearAtom*nearAtom);
        //      } else {
        //          sql.sprintf("Select vid,atnum,(dx*dx+dy*dy+dz*dz)d2,x,y,z,nx,ny,nz From (Select v.rowid As vid,a.atnum,a.x-v.x As dx,a.y-v.y As dy,a.z-v.z As dz,v.x,v.y,v.z,nx,ny,nz From atom a,vertex v Where (itemid=%d And molid=%d And chain='%c' And dx Between -(%3.2f) and (%3.2f) And dy Between -(%3.2f) and (%3.2f) And dz Between -(%3.2f) and (%3.2f))) Where (dx*dx+dy*dy+dz*dz) < %3.2f Order By vid,d2",itemid, imol, chain, nearAtom, nearAtom, nearAtom, nearAtom, nearAtom, nearAtom, nearAtom*nearAtom);
        //      }
        //      //qDebug() << sql;
        //if (nearAtom > 0.0) exec("Drop Index If Exists vx");
    } else {
        sql.sprintf("Select vid,0,0.0,x,y,z,nx,ny,nz From vertex Where itemid=%d Order By vid", itemid);
    }
    if (exec(sql)) {
        valid = true;
    } else {
        Db::tellError(*this);
    }
    //qDebug() << sql;
    return valid;
}

bool vertexQuery::next() {
    if (QSqlQuery::next()) {
        valid = get();
    } else {
        valid = false;
    }
    return valid;
}
bool vertexQuery::get() {
    if (isActive() && isValid()) {
        vid   = value(0).toInt();
        atnum = value(1).toInt();
        vprop = value(2).toDouble();
        x     = value(3).toDouble();
        y     = value(4).toDouble();
        z     = value(5).toDouble();
        nx    = value(6).toDouble();
        ny    = value(7).toDouble();
        nz    = value(8).toDouble();
        valid = true;
    } else {
        valid = false;
    }
    return valid;
}
int vertexQuery::count() {
    return Db::numRows(*this);
}
bool vertexQuery::first() {
    QSqlQuery::first();
    return get();
}
/*
int vertexQuery::count(int itemid) {
    QSqlQuery q;
    q.prepare("Select count(*) from vertex Where itemid=?");
    q.addBindValue(itemid);
    if (q.exec()) {
        q.next();
        int n = q.value(0).toInt();
        return n;
    } else {
        Db::tellError(q);
        return -1;
    }
}
*/
bondQuery::bondQuery() {}
bondQuery::~bondQuery() {}

bool bondQuery::iter(int imol, int resnum, char chain, int filter, int hydrogens) {

    //QSqlQuery Db::iterBonds(int imol, int resnum, char chain, int filter, int hydrogens) {
    QString sql = bondSql(imol, resnum, chain, filter, hydrogens);
    //qDebug() << sql;
    if (prepare(sql)) {
        //addBindValue(imol);
        addBindValue(imol);
        if (exec()) valid = true;
    } else {
        //qDebug() << sql;
        valid = false;
        Db::tellError(*this, sql);
    }
    return valid;
}

QString bondQuery::bondSql(int /*imol*/, int resnum, char chain, int filter, int hydrogens) {
#ifdef DEBUG
    qDebug() << "Db::iterBonds";
#endif
/*
    QString bonds = "With bonds As (Select molid,aid,bid From bond \
       Where molid=(Select Case When type='pdb' Then model Else molid End From molecule Where molid=?))";

    QString atomBonds = ", atomBonds As (Select Distinct atom.molid,aid,bid From atom Join bonds \
       Using(molid) Where (atid=aid Or atid=bid)";
    if (chain != NOCHAIN) atomBonds += " And atom.chain='" + QString(chain) + "'";
    if (resnum != NORESNUM)           atomBonds += " And atom.resnum=" + QString::number(resnum);
    if (filter) {
        atomBonds += " And (" + Db::getFilter(filter).sql + ")";
    }
    atomBonds += ")";

    QString heart = " Select a.x,a.y,a.z,a.atnum,a.resnum,a.chain,a.pcharge,a.hetatm, \
                             b.x,b.y,b.z,b.atnum,b.resnum,b.chain,b.pcharge,b.hetatm \
          From atom a Join atom b On (aid=a.atid And bid=b.atid And a.molid=b.molid) Join atomBonds";

    QString sql = bonds + atomBonds + heart + " Where a.molid=b.molid And a.molid=?";
*/

    QString sql = "With mol As (Select molid, Case When type='pdb' Then model Else molid End As bondmol From molecule Where molid=?)"; 
    sql += ", atoms As (Select * From atom Join mol Using (molid)";
    QStringList atom_clauses = QStringList();
    if (chain != NOCHAIN) atom_clauses << "atom.chain='" + QString(chain) + "'";
    if (resnum!= NORESNUM)atom_clauses << "atom.resnum=" + QString::number(resnum);
    if (filter)           atom_clauses << "(" + Db::getFilter(filter).sql + ")";
    if (atom_clauses.size() > 0) sql += " Where " + atom_clauses.join(" And ");
    sql += ") ";
    sql += "Select Distinct a.x,a.y,a.z,a.atnum,a.resnum,a.chain,a.pcharge,a.hetatm, \
            b.x,b.y,b.z,b.atnum,b.resnum,b.chain,b.pcharge,b.hetatm \
            From atom a Join atoms b Using (molid) Join bond On (bond.molid=bondmol) \
            Where aid=a.atid And bid=b.atid";
/*
    QString sql = "Select a.x,a.y,a.z,a.atnum,a.resnum,a.chain,a.pcharge,a.hetatm, \
            b.x,b.y,b.z,b.atnum,b.resnum,b.chain,b.pcharge,b.hetatm \
            From atom a Join atom b On (aid=a.atid And bid=b.atid And a.molid=b.molid) \
            Join (Select Distinct atom.molid,aid,bid From \
                  (Select molid,aid,bid From bond \
                   Where molid=(Select Case When type='pdb' Then model Else molid End From molecule Where molid=?)) bond \
                  Join atom Using(molid) Where (atid=aid Or atid=bid)";
    if (chain != NOCHAIN) sql += " And atom.chain='" + QString(chain) + "'";
    if (resnum != NORESNUM) sql += " And atom.resnum=" + QString::number(resnum);
    if (filter) {
        sql += " And (" + Db::getFilter(filter).sql + ")";
    }
    sql += ") Where a.molid=b.molid And a.molid=?";
    */

    if (hydrogens == HYDROGEN_HIDE) sql += " And a.atnum>1 And b.atnum>1";
    return sql;
}
bool bondQuery::next() {
    if (QSqlQuery::next()) {
        valid = get();
    } else {
        valid = false;
    }
    return valid;
}
bool bondQuery::first() {
    QSqlQuery::first();
    return get();
}
bool bondQuery::get() {
    if (isActive() && isValid()) {
        ax      = value(0).toDouble();
        ay      = value(1).toDouble();
        az      = value(2).toDouble();
        anum    = value(3).toInt();
        ares    = value(4).toInt();
        achain  = value(5).toString().toLocal8Bit().data()[0];
        acharge = value(6).toDouble();
        ahetatm = value(7).toInt();
        bx      = value(8).toDouble();
        by      = value(9).toDouble();
        bz      = value(10).toDouble();
        bnum    = value(11).toInt();
        bres    = value(12).toInt();
        bchain  = value(13).toString().toLocal8Bit().data()[0];
        bcharge = value(14).toDouble();
        bhetatm = value(15).toInt();
        valid = true;
    } else {
        valid = false;
    }
    return valid;
}

int bondQuery::count() {
    return Db::numRows(*this);
}
/*
int bondQuery::count(int imol, unsigned int resnum, char chain, int filter, int hydrogens) {
    QString sql = "Select count(*) From (" + bondSql(imol, resnum, chain, filter, hydrogens) + ") bcount";
    QSqlQuery q;
    int nbond = -1;
    if (q.prepare(sql)) {
        q.addBindValue(imol);
        q.addBindValue(imol);
        if (q.exec()) {
            q.next();
            nbond = q.value(0).toInt();
        } else {
            Db::tellError(q);
        }
    } else {
        Db::tellError(q);
    }
    return nbond;
}
*/
chainQuery::chainQuery() {}
chainQuery::~chainQuery() {}

bool chainQuery::iter(int imol, char chain, int filter) {
    //QSqlQuery Db::iterChainCoords(int imol, char chain, int filter) {
#ifdef DEBUG
    qDebug() << "Db::iterChainCoords";
#endif
    QString filterClause = Db::getFilter(filter).sql;
    QString sql = "With doubles as (Select molid, chain, resnum, count(name) From atom";
    sql += " Where " + filterClause;
    sql += " Group By molid,resnum,chain";
    if (filter == FILTER_CARTOON) {
        sql += " Having count(distinct name)=2) "; // expect CA and O per residue
    } else {
        sql += ") "; // CA and P per residue; allow both
    }
    
    sql += "Select Distinct x,y,z,resnum,name,coalesce(type,'?') From atom Left Join secondary_structure Using (molid,chain,resnum) Join doubles Using (molid,chain,resnum) Where molid=? And chain=?";
    sql += " And " + filterClause;
    // only one altloc allowed (group) and make 'O' come last per residue (order)
    sql += " Group By resnum,name Order By resnum,name";
    //qDebug() << sql;
    if (prepare(sql)) {
        addBindValue(imol);
        addBindValue(QString(chain));
        if (exec()) valid = true;
    } else {
        Db::tellError(*this);
        valid = false;
    }
    return valid;
}
int chainQuery::count() {
    return Db::numRows(*this);
}
bool chainQuery::next() {
    //chainCoordRecord Db::nextChainCoord(QSqlQuery qchain) {
    if (QSqlQuery::next()) {
        x      = value(0).toDouble();
        y      = value(1).toDouble();
        z      = value(2).toDouble();
        QVariant restmp = value(3);
        resnum = (restmp.isNull()) ? NORESNUM : restmp.toInt();
        name   = value(4).toString();
        sstype = value(5).toString().toLocal8Bit().data()[0];
        valid = true;
    } else {
        valid = false;
    }
    return valid;
}

int chainQuery::numRes(int imol, char chain) {
    //int Db::chainNumRes(int imol, char chain) {
    int nres = -1;
#ifdef DEBUG
    qDebug() << "Db::chainNumRes";
#endif
    QSqlQuery query;
    //query.prepare("Select max(resnum) From atom Where molid=? And chain=?");
    //query.prepare("Select max(resnum)-min(resnum)+1 From atom Where molid=? And chain=?");   
    query.prepare("Select count(distinct(resnum)) From atom Where molid=? And chain=?");    
    query.addBindValue((int)imol);
    query.addBindValue((QString)chain);
    if (query.exec() && query.next()) {
        nres = query.value(0).toInt();
    } else {
        Db::tellError(query);
    }
    return nres;
}
/*
std::string chainQuery::getChainSS(int molid, char chain, int numres) {
    QSqlQuery query;
    query.prepare("Select resnum, type From secondary_structure Where molid=? And chain=? Order by resnum");
    query.addBindValue((int)molid);
    query.addBindValue((QString)chain);
    if (query.exec()) {
        std::string sstype = std::string(numres+1, '?');
        //sstype.insert(0, numres+1, '?');
        while (query.next()) {
            QVariant restmp = query.value(0);
            int resnum = (restmp.isNull()) ? NORESNUM : restmp.toInt();
            char type  = query.value(1).toString().toLocal8Bit().data()[0];
            if (resnum != NORESNUM) sstype[resnum] = type;
        }
        return sstype;
    } else {
        Db::tellError(query);
        return NULL;
    }
}
*/

treeQuery::treeQuery() {}
treeQuery::~treeQuery() {}

bool treeQuery::createTable() {
    QSqlQuery query;
    if (query.exec("Create Table tree (itemid Integer Primary Key, parentid Integer, imol Integer, iatom Integer, grampsname Text, rowname Text, resnum integer, chain Char(1), ignore Integer, style Integer, colorBy Integer, filter Integer, hue Integer, saturation Integer, value Integer, alpha Integer, hydrogens Integer, mainSide Integer, checked Integer, Foreign key (imol) References molecule (molid) On Delete Cascade)")) {
        return true;
    } else {
        return Db::tellError(query);
    }
}
//  int getTreeRowId(int molid) {
//    int itemid = 0;
//    QSqlQuery query;
//    query.prepare("Select itemid From tree Where imol=? And parentid > 0");
//    query.addBindValue((int)molid);
//    if (!query.exec()) tellError(query);
//    query.next();
//    itemid = query.value(0).toInt();
//    return itemid;
//  }

bool treeQuery::iter() {
    if (prepare("Select itemid, parentid, imol, iatom, grampsname, rowname, resnum, chain, ignore, style, colorBy, filter, hue, saturation, value, alpha, hydrogens, mainSide, checked From tree Order by itemid")
            && exec()) {
        valid = true;
    } else {
        Db::tellError(*this);
        valid = false;
    }
    return valid;
}
bool treeQuery::iterRestore() {
    // find items needing to be drawn on restore
    if (prepare("Select itemid, parentid, imol, iatom, grampsname, rowname, resnum, chain, ignore, style, colorBy, filter, hue, saturation, value, alpha, hydrogens, mainSide, checked From tree Where style != 0 Order by itemid")
            && exec()) {
        valid = true;
    } else {
        valid = Db::tellError(*this);
    }
    return valid;
}
bool treeQuery::next() {
    if (QSqlQuery::next()) {
        itemId      = value(ITEM_ID).toInt();
        parentId    = value(ITEM_PARENTID).toInt();
        imol        = value(ITEM_IMOL).toInt();
        iatom       = value(ITEM_IATOM).toInt();
        grampsName  = value(ITEM_GRAMPSNAME).toString();
        rowname     = value(ITEM_ROWNAME).toString();
        QVariant restmp = value(ITEM_RESNUM);
        resnum      = (restmp.isNull()) ? NORESNUM : restmp.toInt();
        chain       = value(ITEM_CHAIN).toString().toLocal8Bit().data()[0];
        ignore      = value(ITEM_IGNORE).toInt();
        style       = value(ITEM_STYLE).toInt();
        colorBy     = value(ITEM_COLORBY).toInt();
        filter      = value(ITEM_FILTER).toInt();
        int hue   = value(ITEM_HUE).toInt();
        int sat   = value(ITEM_SATURATION).toInt();
        int val   = value(ITEM_VALUE).toInt();
        int alpha = value(ITEM_ALPHA).toInt();
        if (hue == 0.0 && sat == 0.0 && val == 0.0) {
            color   = QColor();
        } else {
            color   = QColor::fromHsv(hue,sat,val,alpha);
            //qDebug() << "current alpha " << itemId << rowname << alpha << color;
        }
        hydrogens = value(ITEM_HYDROGENS).toInt();
        mainSide  = value(ITEM_MAINSIDE).toInt();
        checked   = value(ITEM_CHECKED).toInt();
        valid = true;
    } else {
        valid = false;
    }
    return valid;
}

bool treeQuery::getRow(int itemid) {
    if (prepare("Select itemid, parentid, imol, iatom, grampsname, rowname, resnum, chain, ignore, style, colorBy, filter, hue, saturation, value, alpha, hydrogens, mainSide, checked From tree Where itemid=?")) {
        addBindValue((int)itemid);
        if (exec()) {
            next();
            this->finish();
            return true;
        }
    }
    valid = Db::tellError(*this);
    return valid;
}
bool treeQuery::getRow(QString grampsName) {
    if (prepare("Select itemid, parentid, imol, iatom, grampsname, rowname, resnum, chain, ignore, style, colorBy, filter, hue, saturation, value, alpha, hydrogens, mainSide, checked From tree Where grampsname=?")) {
        addBindValue((QString)grampsName);
        if (exec()) {
            next();
            this->finish();
            return true;
        }
    }
    valid = Db::tellError(*this);
    return valid;
}
bool treeQuery::getSibling(int parentid, char sibchain) {
    if (prepare("Select itemid, parentid, imol, iatom, grampsname, rowname, resnum, chain, ignore, style, colorBy, filter, hue, saturation, value, alpha, hydrogens, mainSide, checked From tree Where parentid=? and chain=?")) {
        addBindValue(parentid);
        addBindValue((QString)sibchain);
        if (exec()) {
            next();
            this->finish();
            return true;
        }
    }
    valid = Db::tellError(*this);
    return valid;
}

bool treeQuery::newRow(int parentid, int imol, int iatom, QString grampsname, QString rowname,
                      int resnum, char chain, int ignore, int style, int colorBy, int filter,
                      int hue, int saturation, int cvalue, int alpha, int hydrogens, int mainSide, bool checked) {
    QSqlQuery query;
    if (query.prepare("Insert Into tree (parentid, imol, iatom, grampsname, rowname, resnum, chain, ignore, style, colorBy, filter, hue, saturation, value, alpha, hydrogens, mainSide, checked) Values (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)")) {
        query.addBindValue((int)parentid);
        query.addBindValue((int)imol);
        query.addBindValue((int)iatom);
        query.addBindValue((QString)grampsname);
        query.addBindValue((QString)rowname);
        if (resnum == NORESNUM) {
            query.addBindValue(QVariant::Int);
        } else {
            query.addBindValue((int)resnum);
        }
        query.addBindValue((QString)chain);
        query.addBindValue((int)ignore);
        query.addBindValue((int)style);
        query.addBindValue((int)colorBy);
        query.addBindValue((int)filter);
        query.addBindValue((int)hue);
        query.addBindValue((int)saturation);
        query.addBindValue((int)cvalue);
        query.addBindValue((int)alpha);
        query.addBindValue((int)hydrogens);
        query.addBindValue((int)mainSide);
        query.addBindValue((int)checked);
        if (query.exec()) {
            query.exec("Select last_insert_rowid()");
            query.next();
            int rowid = query.value(0).toInt();
            return getRow(rowid);
        }
    }
    return Db::tellError(query);
}

bool treeQuery::isRow(int imol, char chain, QString rowname) {
    QSqlQuery query;
    if (query.prepare("Select itemid From tree Where imol=? And chain=? And rowname=?")) {
        query.addBindValue((int)imol);
        query.addBindValue((QString)chain);
        query.addBindValue((QString)rowname);
        if (query.exec()) return query.next();
    }
    return Db::tellError(query);
}

bool treeQuery::isRow(int imol, char chain, int iatom) {
    QSqlQuery query;
    if (query.prepare("Select itemid From tree Where imol=? And chain=? And iatom=?")) {
        query.addBindValue((int)imol);
        query.addBindValue((QString)chain);
        query.addBindValue((int)iatom);
        if (query.exec()) return query.next();
    }
    return Db::tellError(query);
}

bool treeQuery::updateColor(int itemid, QColor newcolor) {
    QSqlQuery query;
    if (query.prepare("Update tree Set hue=?, saturation=?, value=?, alpha=? Where itemid=?")) {
        query.addBindValue((int)newcolor.hue());
        query.addBindValue((int)newcolor.saturation());
        query.addBindValue((int)newcolor.value());
        query.addBindValue((int)newcolor.alpha());
        query.addBindValue((int)itemid);
        if (query.exec()) {
            color = newcolor; 
            return true;
        }
    }
    return Db::tellError(query);
}
bool treeQuery::updateColorBy(int itemid, int newcolorBy) {
    QSqlQuery query;
    if (query.prepare("Update tree Set colorBy=? Where itemid=?")) {
        query.addBindValue((int)newcolorBy);
        query.addBindValue((int)itemid);
        if (query.exec()) {
            colorBy = newcolorBy;
            return true;
        }
    }
    return Db::tellError(query);
}
bool treeQuery::updateIgnore(int itemid, int newignore) {
    QSqlQuery query;
    if (query.prepare("Update tree Set ignore=? Where itemid=?")) {
        query.addBindValue((int)newignore);
        query.addBindValue((int)itemid);
        if (query.exec()) {
            ignore = newignore;
            return true;
        }
    }
    return Db::tellError(query);
}
bool treeQuery::updateChecked(int itemid, int newchecked) {
    QSqlQuery query;
    if (query.prepare("Update tree Set checked=? Where itemid=?")) {
        query.addBindValue(newchecked);
        query.addBindValue((int)itemid);
        if (query.exec()) {
            checked = newchecked;
            return true;
        }
    }
    return Db::tellError(query);
}
bool treeQuery::updateStyle(int itemid, int newstyle) {
    QSqlQuery query;
    if (query.prepare("Update tree Set style=? Where itemid=?")) {
        query.addBindValue((int)newstyle);
        query.addBindValue((int)itemid);
        if (query.exec()) {
            style = newstyle;
            return true;
        }
    }
    return Db::tellError(query);
}
bool treeQuery::updateHydrogens(int itemid, int newhydrogens) {
    QSqlQuery query;
    if (query.prepare("Update tree Set hydrogens=? Where itemid=?")) {
        query.addBindValue((int)newhydrogens);
        query.addBindValue((int)itemid);
        if (query.exec()) {
            hydrogens = newhydrogens;
            return true;
        }
    }
    return Db::tellError(query);
}
bool treeQuery::updateMainSide(int itemid, int newmainSide, int newfilter) {
    QSqlQuery query;
    if (query.prepare("Update tree Set mainSide=?, filter=? Where itemid=?")) {
        query.addBindValue((int)newmainSide);
        query.addBindValue((int)newfilter);
        query.addBindValue((int)itemid);
        if (query.exec()) {
            mainSide = newmainSide;
            filter = newfilter;
            return true;
        }
    }
    return Db::tellError(query);
}

paramQuery::paramQuery() {}
paramQuery::~paramQuery() {}

bool paramQuery::iter(QString kind) {
    QString sql = "Select pkind,pgroup,pname,pvalue from params Where pkind = ? order by pkind, pgroup";
    if (prepare(sql)) {
        addBindValue(kind);
        if (exec()) {
            valid = true;
        } else {
            valid = false;
        }
    } else {
        Db::tellError(*this);
        valid = false;
    }
    return valid;
}
bool paramQuery::iter() {
    QString sql = "Select pkind,pgroup,pname,pvalue from params order by pkind, pgroup";
    if (prepare(sql)) {
        if (exec()) {
            valid = true;
        } else {
            valid = false;
        }
    } else {
        Db::tellError(*this);
        valid = false;
    }
    return valid;
}

bool paramQuery::iter(QString group, QString kind) {
    QString sql = "Select pkind,pgroup,pname,pvalue from params Where pkind=? and pgroup=? order by pkind, pgroup";
    if (prepare(sql)) {
        addBindValue(kind);
        addBindValue(group);
        if (exec()) {
            valid = true;
        } else {
            valid = false;
        }
    } else {
        Db::tellError(*this);
        valid = false;
    }
    return valid;
}
bool paramQuery::next() {
    if (QSqlQuery::next()) {
        pkind  = value(0).toString();
        pgroup = value(1).toString();
        pname  = value(2).toString();
        pvalue = value(3).toString();
        valid = true;
    } else {
        valid = false;
    }
    return valid;
}
bool paramQuery::update(QString akind, QString agroup, QString aname, QString avalue) {
    QSqlQuery q;
    q.prepare("Update params Set pvalue=? Where pkind=? And pgroup=? And pname=?");
    q.addBindValue(avalue);    
    q.addBindValue(akind);
    q.addBindValue(agroup);
    q.addBindValue(aname);
    if (!q.exec()) {
        Db::tellError(q);
        return false;
    }
    return true;
}
