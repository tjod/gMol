/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2013 by TJ O'Donnell and
C  Copyright (C) 2005-2013 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
#include "dbchem.h"
#include "db.h"
#include <fstream>
#include <ios>
//using namespace std;
// Foreign Keys are not implemented in sqlite3 until 3.6.19
// The fk defs here will parse ok with earlier verions, but the fk constraints will not operate

namespace ChemDb {
  int molid = 0;
  //int atid = 0;
}
QSqlDatabase ChemDb::open(QString dbname, bool create) {
  //name = dbname;
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(dbname);
  db.open();
  getVersion();
  //setPragmas();
  //if (!defineResidues()) return;
  if (!create) return db;
  //if (!createTmpTables()) reportError(query);//"temp tables");
  molid = getMolid();
  if (molid) {
    //atid = getAtomid();
  } else {
    createTables();
    molid = 0;
    //atid = 0;
  }
  return db;
}

void ChemDb::getVersion() {
  QSqlQuery query;
  if (query.exec("Select sqlite_version()")) {
    query.next();
    QString version = query.value(0).toString();
    qDebug() << "sqlite version" << version;
  }
}
void ChemDb::setPragmas() {
  QSqlQuery query;
  printf("synchronous %d\n", query.exec("Pragma synchronous=0"));
  printf("temp_store %d\n",  query.exec("Pragma temp_store=2"));
  printf("foreign_keys%d\n",  query.exec("Pragma foreign_keys=ON"));
}

int ChemDb::getMolid() {
// get the maximum molid used in this db
  QSqlQuery query;
  if (query.exec("Select max(molid) From molecule")) { 
    query.next();
    int mid = query.value(0).toInt();
    return mid;
  }
  return 0;
}

bool ChemDb::reportError(QSqlQuery q) {
  QSqlDatabase db = QSqlDatabase::database();
  qDebug() << q.lastQuery();
  qDebug() << TR("error: ") << db.lastError().number() << db.lastError().text();
  return false;
}
/*
int ChemDb::getAtomid() {
// get the maximum atid used in this db
  QSqlQuery query;
  if (query.exec("Select max(atid) From atom")) { 
    query.next();
    int aid = query.value(0).toInt();
    return aid;
  }
  return 0;
}
*/

bool ChemDb::createTables() {
// create tables for a new database
  QSqlQuery query;
  if (!query.exec("Create Table If Not Exists molecule (molid Integer Primary Key, file Text, type Text, title Text, smiles Text, inchi Text, inchikey Text, nresidue Integer, natoms Integer, nbonds Integer, model Integer)")) return reportError(query);
  if (!query.exec("Create Table If Not Exists property (molid Integer, name Text, value Text, Foreign key (molid) References molecule (molid) On Delete Cascade)")) return reportError(query);
  if (!query.exec("Create Table If Not Exists secondary_structure (molid Integer, chain Char(1), resnum Integer, type Char(1), Foreign key (molid) References molecule (molid) On Delete Cascade)")) return reportError(query);
  if (!query.exec("Create Table If Not Exists atom (atid Integer Not Null, molid Integer Not Null, resnum Integer, resnam Text, altLoc Char(1), icode Char(1), atnum Integer, x Real, y Real, z Real, fcharge Integer, pcharge Real, name Text, chain Char(1), hetatm Integer, Primary Key (molid,atid), Foreign Key (molid) References molecule (molid) On Delete Cascade);")) return reportError(query);
  if (!query.exec("Create Table If Not Exists bond (molid Integer Not Null, aid Integer Not Null, bid Integer Not Null, bo Integer, Foreign key (molid,aid) References atom (molid,atid) On Delete Cascade, Foreign key (molid,bid) References atom (molid,atid) On Delete Cascade)")) return reportError(query);
  if (!query.exec("Create Table If Not Exists gramps_save (iorder Integer Primary Key, command Text)")) return reportError(query);  
  return true;
}
 
bool ChemDb::createBondIndexes() {
// helps to speed up searches
    //qDebug() << "createBondIndexes";
  QSqlQuery query;
  if (!query.exec("Create Index main.bond_molid On bond (molid)")) return reportError(query);
  if (!query.exec("Create Index main.bond_aid On bond (aid)"))     return reportError(query);
  if (!query.exec("Create Index main.bond_bid On bond (bid)"))     return reportError(query);
  return true;
}
bool ChemDb::createAtomIndexes() {
    //qDebug() << "createAtomIndexes";
  QSqlQuery query;
  if (!query.exec("Create Index main.atom_id On atom (molid,atid)")) return reportError(query);
  if (!query.exec("Create Index main.atom_res On atom (molid,chain,resnum,icode,altLoc)")) return reportError(query);
  if (!query.exec("Create Index main.atom_name On atom (name)"))     return reportError(query);
  if (!query.exec("Create Index main.atom_resnum On atom (resnum)")) return reportError(query);
  //if (!query.exec("Create Index main.atom_x On atom (x)"))           return reportError(query);
  //if (!query.exec("Create Index main.atom_y On atom (y)"))           return reportError(query);
  //if (!query.exec("Create Index main.atom_z On atom (z)"))           return reportError(query);
  return true;
}
 
bool ChemDb::dropIndexes() {
// helps to speed up inserts
    //qDebug() << "dropIndexes";
  QSqlQuery query;
  if (!query.exec("Drop Index If Exists main.bond_molid"))  return reportError(query);
  if (!query.exec("Drop Index If Exists main.bond_aid"))    return reportError(query);
  if (!query.exec("Drop Index If Exists main.bond_bid"))    return reportError(query);
  if (!query.exec("Drop Index If Exists main.atom_id"))     return reportError(query);
  if (!query.exec("Drop Index If Exists main.atom_name"))   return reportError(query);
  if (!query.exec("Drop Index If Exists main.atom_resnum")) return reportError(query);
  if (!query.exec("Drop Index If Exists main.atom_res"))    return reportError(query);
  return true;
}

//bool ChemDb::addFlags(int molid) {
//  QSqlQuery query;
//  query.prepare("Update atom Set flag=? Where molid=? And name In ('N', 'C', 'O', 'OXT', 'CA', 'H', 'HA', 'OP1', 'P', 'OP2', 'OP3', 'O5''', 'C5''', 'H5''', 'H5''''', 'H3''', 'H4''', 'C4''', 'C3''', 'O3''') And flag=?");
//  query.bindValue(0, FILTER_MAIN); query.bindValue(1, molid); query.bindValue(2, FILTER_ATOM);
//  if (!query.exec()) return reportError(query);

//  query.prepare("Update atom Set flag=? Where molid=? And flag=?");
//  query.bindValue(0, FILTER_SIDE); query.bindValue(1, molid); query.bindValue(2, FILTER_ATOM);
//  if (!query.exec()) return reportError(query);

//  query.prepare("Update atom Set flag=? Where molid=? And name In ('CA', 'P') And flag=?");
//  query.bindValue(0, FILTER_TRACE); query.bindValue(1, molid); query.bindValue(2, FILTER_MAIN);
//  if (!query.exec()) return reportError(query);

//  query.prepare("Update atom Set flag=? Where molid=? And resnam='HOH'");
//  query.bindValue(0, FILTER_WATER); query.bindValue(1, molid);
//  if (!query.exec()) return reportError(query);
//  return true;
//}

bool ChemDb::addSecondaryStructure(int molid) {
  QSqlQuery insert;
  QSqlQuery select;
  QChar sstype;
  select.prepare("Select name,value from main.property Where molid=? And name in ('HELIX', 'SHEET', 'TURN')");
  select.addBindValue((int)molid);
  select.exec();
  insert.prepare("Insert Or Replace Into main.secondary_structure (molid, chain, resnum, type) Values (?,?,?,?)");
  while (select.next()) {
     QString name = select.value(0).toString();
     QString value = select.value(1).toString();
     sstype = name.data()[0];
     QStringList items = value.split("\n");
     QChar chain;
     int initres;
     int endres;
     for (int i=0; i<items.size(); ++i) {
       QString rec = items.at(i);
       if (sstype == 'S') {
         chain = rec.at(15);
         initres = rec.mid(16,4).toInt();
         //endres = rec.mid(27,4).toInt()-1;
         endres = rec.mid(27,4).toInt();
       } else if (sstype == 'H') {
         chain = rec.at(13);
         initres = rec.mid(15,4).toInt();
         //endres = rec.mid(27,4).toInt()-1;
         endres = rec.mid(27,4).toInt();
       } else if (sstype == 'T') {
         chain = rec.at(13);
         initres = rec.mid(14,4).toInt();
         endres = rec.mid(25,4).toInt();
       }
       for (int j=initres; j<=endres; ++j) {
         insert.addBindValue((int)molid);
         insert.addBindValue(chain);
         insert.addBindValue((int)j);
         insert.addBindValue(sstype);
         if (!insert.exec()) return reportError(insert);
       }
     }
  }
  return true;
}

#ifdef OBCHEM
// OBCHEM is used to remove code when building qtgramps, since it can read PDB files on it's own
//  as of 12/2009 that feature is disabled in qtgramps.
using namespace OpenBabel;
int ChemDb::readFile(QString file, QString type) {
// read molecule file and add to database
  QSqlDatabase db = QSqlDatabase::database();
  //dropIndexes();
  int nmols = 0;
/*
  std::filebuf fb;
  if (!fb.open(file.toLocal8Bit().data(), std::ios::in)) {
    qDebug() << TR("can not open file") << file;
    return false;
  }
  int fsize = QFile(file).size();
  std::istream is(&fb);
*/
  std::ifstream is;
  is.open(file.toLocal8Bit().data(), std::ifstream::in|std::ifstream::binary);
  if (!is.good()) {
    qDebug() << TR("can not open file") << file;
    return false;
  }
  int fsize = QFile(file).size();
  OBMol mol;
  OBConversion conv;
  conv.SetInFormat(type.toLocal8Bit().data());
  db.transaction();
  //dropIndexes();
#ifdef DEBUG
  printf ("Reading 0 at 1 of %d", fsize);
#endif
  fflush(stdout);
  while (conv.Read(&mol, &is)) {
    if (mol.Empty()) break;
    QString title = mol.GetTitle();
    mol.SetTitle(""); // otherwise title gets into smiles
    int nres = mol.NumResidues();
#ifdef DEBUG
    qDebug() << title << nres << "residues";
#endif
    if (nres > 1) {
      //protein(mol);
    } else {
      gen3D(mol, false);
    }
    QString smiles;
    QString inchi;
    QString inchikey;
    ++nmols;
    //if (nmols > 1) {
      if (type.toLower() == "pdb") {
        printf("Model");
        if (nmols > 1) {
          QTextStream(&title) << "Model" << nmols;
        } else {
        }
      } else {
        printf("Molecule");
      }
      int atloc = is.tellg()*0.5;
      printf (" %d at %d of %d\n", nmols, atloc, fsize);
      fflush(stdout);
    //}
    if (nres < 10) {
      OBConversion conv;
      conv.SetOutFormat("can");
      smiles = QString::fromStdString(conv.WriteString(&mol,true));
      conv.SetOutFormat("inchi");
      inchi = QString::fromStdString(conv.WriteString(&mol,true));
      conv.SetOptions("K", OBConversion::OUTOPTIONS);
      inchikey = QString::fromStdString(conv.WriteString(&mol,true));
    }
    if (addMolecule(mol, file, type, smiles, inchi, inchikey, title, nres, nmols)) {
#ifdef DEBUG
      qDebug() << "add flags";
#endif
      //if (!addFlags(molid)) return false;

    } else {
      break;
    }
  }
  if(!is.eof()) {
    qDebug() << TR("more data remains in file");
  }
  printf ("Read %d at %d of %d\n", nmols, int(fsize*0.5), fsize);
  fflush(stdout);
  is.close();
  //createAtomIndexes();
  //createBondIndexes();
  db.commit();

/*
#ifdef DEBUG
  qDebug() << "create indexes";
#endif
  createIndexes();
*/

  return molid;
}

bool ChemDb::addProperty(OBMol &mol) {
  QSqlQuery query;
  query.prepare("Insert Into property (molid, name, value) Values (?,?,?)");
  std::vector<OBGenericData*>::iterator k;
  std::vector<OBGenericData*> vdata = mol.GetData();
  for (k = vdata.begin();k != vdata.end();++k) {
     if ((*k)->GetDataType() == OBGenericDataType::PairData) {
       query.addBindValue((int)molid);
       query.addBindValue(QString::fromStdString((*k)->GetAttribute()));
       query.addBindValue(QString::fromStdString(((OBPairData*)(*k))->GetValue()));
       if (!query.exec()) return reportError(query);
     } else {
       //cerr << (*k)->GetDataType() << endl;
     }
  }
  return true;
}

bool ChemDb::addMolecule(OBMol &mol, QString file, QString type, QString smiles, QString inchi, QString inchikey, QString title, int nres, int nmols) {
// add all the necessary rows to the necessary tables for this molecule
  QSqlQuery query;
  query.prepare("Insert Into molecule (molid, file, type, title, smiles, inchi, inchikey, nresidue, natoms, nbonds, model) Values (?,?,?,?,?,?,?,?,?,?,?)");
  ++molid;
  query.addBindValue((int)molid);
  QString fname = QFileInfo(file).fileName();
  query.addBindValue(fname);
  query.addBindValue(type);
  if (title.isNull() || title.size() == 0) {
    title = QFileInfo(file).baseName();
  }
  query.addBindValue(title);
  if (smiles.isNull() || smiles.size() == 0) {
      query.addBindValue(QVariant(QVariant::String));
      //query.addBindValue(NULL);
  } else {
    query.addBindValue(smiles);
  }
  if (inchi.isNull() || inchi.size() == 0) {
      query.addBindValue(QVariant(QVariant::String));
      //query.addBindValue(NULL);
  } else {
    query.addBindValue(inchi);
  }
  if (inchikey.isNull() || inchikey.size() == 0) {
      query.addBindValue(QVariant(QVariant::String));
      //query.addBindValue(NULL);
  } else {
    query.addBindValue(inchikey);
  }
  query.addBindValue((int)nres);
  query.addBindValue((int)mol.NumAtoms());
  query.addBindValue((int)mol.NumBonds());
  query.addBindValue((int)(molid-nmols+1)); // "parent" model
#ifdef DEBUG
  qDebug() << "add molecule";
  qDebug() << mol.NumConformers() << "conformers.";
#endif
  if (!query.exec())        return reportError(query);//"add molecule");
#ifdef DEBUG
  qDebug() << "add property";
#endif
  if (!addProperty(mol))    return false;
#ifdef DEBUG
  qDebug() << "add secondary structure";
#endif
  if (!addSecondaryStructure(molid))    return false;
#ifdef DEBUG
  qDebug() << "add atom";
#endif
  if (!addAtoms(mol))       return false;
#ifdef DEBUG
  qDebug() << "add bond";
#endif
  if (!addBonds(mol))       return false;
  return true;
}

bool ChemDb::addAtoms(OBMol &mol) {
// add the atom rows to the atom table
  QString sql;
  int nres = mol.NumResidues();
  if (nres > 1) {
    sql = "Insert Into atom (molid, atid, atnum, x, y, z, fcharge, pcharge, name, resnum, chain, resnam, hetatm) Values (?,?,?,?,?,?,?,?,?,?,?,?,?)";
  } else {
    sql = "Insert Into atom (molid, atid, atnum, x, y, z, fcharge, pcharge, name) Values (?,?,?,?,?,?,?,?,?)";
  }
  QSqlQuery query;
  query.prepare(sql);
  //QSqlQuery tmpquery;
  //tmpquery.exec("Delete From atom_ob");
  //tmpquery.prepare("Insert Into atom_ob (atid, aidx) Values (?,?)");
  for (OBMolAtomIter atom(mol) ; atom; ++atom) {
    //++atid;
    query.addBindValue((int)molid);
    query.addBindValue((int)atom->GetIdx());
    int resnum = NORESNUM;
    QString resnam;
    QString atomnam;
    QString chain;
    int atnum = atom->GetAtomicNum();
    query.addBindValue((int)atnum);
    query.addBindValue((float)atom->x());
    query.addBindValue((float)atom->y());
    query.addBindValue((float)atom->z());
    query.addBindValue((int)atom->GetFormalCharge());
    query.addBindValue((float)atom->GetPartialCharge());
    OBResidue* res = atom->GetResidue();
    if (res) {
      atomnam = QString::fromStdString(res->GetAtomID(&*atom)).trimmed();
    }
    if (nres > 1) {
      query.addBindValue(atomnam);
      resnum = res->GetNum();
      query.addBindValue((int)resnum);
      chain = (QString)res->GetChain();
/*
      if (chain == " ") {
        //query.addBindValue(QVariant(QVariant::String));
        query.addBindValue(NULL);
      } else {
        query.addBindValue(chain);
      }
*/
      query.addBindValue(chain);
      resnam = QString::fromStdString(res->GetName());
      query.addBindValue(resnam);
      int flag = 0;
      if (res->IsHetAtom(&*atom)) flag = 1;
      query.addBindValue((int)flag);
    } else {
      if (atomnam.isNull()) atomnam = etab.GetSymbol(atnum);
      query.addBindValue(atomnam);
    }
    if (!query.exec()) return reportError(query);//"exec addAtoms");

    //tmpquery.addBindValue((int)atid);
    //tmpquery.addBindValue((int)atom->GetIdx());
    //if (!tmpquery.exec()) return reportError(query);//"tmp exec");
  }
  return true;
}

bool ChemDb::addBonds(OBMol &mol) {
// add the bonds rows to the bond table
  QSqlQuery query;
  query.prepare("Insert Into bond (molid, aid, bid, bo) Values (?,?,?,?)");
  for (OBMolBondIter bond(&mol) ; bond; ++bond) {
    int aidx = bond->GetBeginAtomIdx();
    int bidx = bond->GetEndAtomIdx();
    query.addBindValue((int)molid);
    query.addBindValue((int)aidx);
    query.addBindValue((int)bidx);
    query.addBindValue((int)bond->GetBondOrder());
    if (!query.exec()) return reportError(query);//"exec addBonds");
  }
  return true;
}

void ChemDb::protein(OBMol &pmol) {
// for debugging pdb files
  //qDebug() << pmol->NumAtoms() << " atoms.";
  //qDebug() << pmol->NumResidues() << " residues.";
  // show new residues only
  //  can be also done using
  //  select chain,resnam,resnum,name From atom Where resnam Not In (Select title from residue.molecule);
  //     when residue.db is attached.
  int i=0;
  for (OBResidueIter res(pmol) ; res; ++res) {
    ++i;
    if (res->GetResKey() < OBResidueIndex::NDP) continue;
    std::cerr <<i<<","<<res->GetName() <<","<<res->GetNum()<<","<<res->GetNumString()
            <<","<<res->GetChain()<<","<<res->GetIdx()<<","<<res->GetResKey()<<std::endl;
  }
  i = 0;
  for (OBMolAtomIter atom(pmol) ; atom; ++atom) {
    ++i;
    OBResidue* res = atom->GetResidue();
    if (res->GetResKey() < OBResidueIndex::NDP) continue;
    if (!res->IsHetAtom(&*atom)) continue;
    std::cerr << i << " " << res->GetName() << res->GetNum() << "(" << res->GetIdx() << ")" <<
      res->GetChain() << " " << res->IsHetAtom(&*atom) <<
      res->GetAtomProperty(&*atom, OBResidueAtomProperty::ALPHA_CARBON) <<
      res->GetAtomID(&*atom) << std::endl;
  }
  std::vector<OBGenericData*> pairData = pmol.GetAllData(OBGenericDataType::PairData);
  for (std::vector<OBGenericData*>::iterator data = pairData.begin(); data != pairData.end(); ++data) {
    OBPairData *pd = static_cast<OBPairData*>(*data);
    std::string attr = pd->GetAttribute();
    std::string value = pd->GetValue();
    qDebug() << QString::fromStdString(attr) << QString::fromStdString(value);
      
    if (attr == "HELIX") {
      qDebug() << QString::fromStdString(attr) << QString::fromStdString(value);
    } else if (attr == "SHEET") {
      qDebug() << QString::fromStdString(attr) << QString::fromStdString(value);
    }
  }

}
#include <openbabel/builder.h>
#include <openbabel/forcefield.h>
int ChemDb::gen3D(OBMol &pmol, bool force=false) {

  // make up 3D coords for this molecule
  if (force) pmol.SetDimension(0);
  if (pmol.GetDimension() > 0) return 0;
#ifdef DEBUG
  qDebug() << "gen3D";
#endif
  OBBuilder builder;
  pmol.AddHydrogens();
  builder.Build(pmol);
/*
  // optimize/minimize the enery of the coordinates
  OBForceField* pFF = OBForceField::FindForceField("MMFF94");

  if (!pFF) return -1;
  pFF->SetLogFile(&cerr);
  pFF->SetLogLevel(OBFF_LOGLVL_LOW);
  if (!pFF->Setup(pmol)) {
    qDebug() << "cannot setup force field";
    return -2;
  }
  //QMessageBox::information(this, TR("3D structure generation"), TR("Generating 3D structure..."));
  pFF->ConjugateGradients(500);
  pFF->GetCoordinates(pmol);
  //mbox.hide();
*/
  return 0;
}
#endif
// end of ifdef OBCHEM
