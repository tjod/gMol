/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
dbprobe.cpp - This program will create a grid around a molecule
              and compute a distance function to be used to define a molecular
              surface.  
*/
#include "atom_properties.h"
#include "atom_values.h"
#include "db.h"
#include "dbchem.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <QApplication>
#include <QDebug>

QString gmolLib;

extern "C" void intsect_(float *, float *, int *, int *, int *,
     int *, int *, int *, int *, int *, int *, int *, int *,
     float *, int *, float *, float *, float *, float *, float *, float *,
     float *, bool *, int *, int *);

extern "C" void interp_(float *, int *, int *, int *, int *,
     float *, float *, float *, float *, float *, float *, float *, float *, float *, float *,
     bool *, bool *, int *, int *, int *, int *, float *);
extern "C" void unit_(float *);

#define rProbe 1.4
#define Cmax 100.0
#define BOHR (1.0/0.5291771)
#define PIX 1
#define QIX 2
#define DB  3
#define CUBE  4
QSqlQuery *addQuery; // used in addtri, setup in main
bool computeGrid(float *grid, int *tags, atomQuery atom, float step, int xDim, int yDim, int zDim, float min[3]) {
    //float vdwR = etab.GetVdwRad(a->GetAtomicNum());
    float vdwR = Atom::radius[atom.atnum];
    float  R = pow((vdwR + rProbe), 2);
    float vR = pow(vdwR, 2);
    float width = 2*vdwR + rProbe;
    // examine neighboring grids
    int ix = floor((atom.x - width - min[0]) / step);
    int iy = floor((atom.y - width - min[1]) / step);
    int iz = floor((atom.z - width - min[2]) / step);
    int xx = floor((atom.x + width - min[0]) / step)+1;
    int yy = floor((atom.y + width - min[1]) / step)+1;
    int zz = floor((atom.z + width - min[2]) / step)+1;
    //fprintf (stderr,"%d,%d,%d\n", xx-ix, yy-iy, zz-iz);
    for (int k=(iz<0?0:iz); k<(zz>zDim?zDim:zz); ++k) {
      float gz = min[2] + k*step;
      for (int j=(iy<0?0:iy); j<(yy>yDim?yDim:yy); ++j) {
        float gy = min[1] + j*step;
        for (int i=(ix<0?0:ix); i<(xx>xDim?xDim:xx); ++i) {
          float gx = min[0] + i*step;
          float d = pow(atom.x-gx,2) + pow(atom.y-gy,2) + pow(atom.z-gz,2);
          float v;
/*
          if (d > R) {
            continue;
          } else if (d < vR) {
            v = Cmax;
          } else {
            v = Cmax * (R-d)/(R-vR);
          }
*/
          v = Cmax * (R-d)/(R-vR);
/*
          v = sqrt(sqrt(d) / vdwR); // soft sphere (rij/r0)** -2
*/
// c style: to be used when writing cube file
          //int ijk = i*zDim*yDim + j*zDim + k;
          //int ijk = (i*yDim + j)*zDim + k;
// fortran style: to be used when calling intsect_/interp_
          int ijk = (k*yDim + j)*xDim + i;
/*
          if (ijk > xDim*yDim*zDim) {
            qDebug() << "ACK!" << i << j << k << ijk;
            exit(-1);
          }
*/
          //grid[ijk] += v;
          //grid[ijk] = std::max(grid[ijk], v);
          //tags[ijk] = atom.atid;
          if (tags[ijk] == 0 || v > grid[ijk]) {
            grid[ijk] = v;
            tags[ijk] = atom.atid;
          } 
        }
      }
     }
    //fprintf(stderr, "%d,%d,%d\n", ix,iy,iz);

  return true;
}

void makeGrid(int imol, float step, float padding, char chain, int resnum, int filter, int hydrogen, int outtype, float isoval) {
  float *fgrid;
  int *tags;
  float min[3], max[3], avg[3];
  int xDim, yDim, zDim, xyzDim;

  Db::molBounds(imol, resnum, chain, filter, min, max, avg);
  qDebug() << min[0] << min[1] << min[2] << max[0] << max[1] << max[2];

  min[0] -= padding; max[0] += padding;
  min[1] -= padding; max[1] += padding;
  min[2] -= padding; max[2] += padding;
/*
 make min a multiple of step size to help align grids from
 separate moleulces/runs
*/
  min[0] = int(min[0] / step) * step;
  min[1] = int(min[1] / step) * step;
  min[2] = int(min[2] / step) * step;

  xDim = (max[0] - min[0]) / step + 0.5;
  yDim = (max[1] - min[1]) / step + 0.5;
  zDim = (max[2] - min[2]) / step + 0.5;
  xyzDim = xDim * yDim * zDim;
  qDebug() << xDim << yDim << zDim << min[0] << min[1] << min[2] << min[0]+step*xDim << min[1]+step*yDim << min[2]+step*zDim;
  if (outtype == CUBE) {
    printf("dbsurf\ndbsurf\n");
    printf("%5d  %10.6f  %10.6f  %10.6f\n", 0, BOHR*min[0], BOHR*min[1], BOHR*min[2]);
    printf("%5d  %10.6f  %10.6f  %10.6f\n", xDim, BOHR*step, 0.0, 0.0);
    printf("%5d  %10.6f  %10.6f  %10.6f\n", yDim, 0.0, BOHR*step,  0.0);
    printf("%5d  %10.6f  %10.6f  %10.6f\n", zDim, 0.0, 0.0, BOHR*step);
  }

  fgrid = (float *)calloc(xyzDim, sizeof(float));
  tags  = (int *)  calloc(xyzDim, sizeof(int));
  int natom = 0;
  //QSqlQuery qatom = Db::iterAtoms(imol, resnum, chain, filter, hydrogen);
  //QSqlQuery qatom = Db::iterAtoms(imol, 0, chain, filter, hydrogen);
  //for (atomRecord atom = Db::nextAtom(qatom); atom.valid; atom = Db::nextAtom(qatom)) {
  atomQuery atom_query = atomQuery();
  for (atom_query.iter(imol, resnum, chain, filter, hydrogen); atom_query.next(); ) {  
     computeGrid(fgrid, tags, atom_query, step, xDim, yDim, zDim, min);
     ++natom;
  }
  qDebug() << natom << "atoms.";

  if (outtype == CUBE) {
    for (int i=0; i < xyzDim; i += 6) {
      int j=i;
      while (j < i+6 && j < xyzDim) printf(" %12.5g", fgrid[j++]);
      printf("\n");
    }

  } else if (outtype) {
    float *vlist;
    int nx0 = 1;
    int ny0 = 1;
    int nz0 = 1;
    int nxx = xDim;
    int nyy = yDim;
    int nzz = zDim;
    int nvert, ntri;
    float volume;
    bool normcomp = true;
    bool volcomp = true;
    int ntags = xyzDim;

/*
#define MAXTRI 50000
#define MAXVERT (3*MAXTRI)
    int mvert = MAXVERT;
    float vlist[3*2*MAXVERT];
*/
    // just a guess at MAXVERT
    int mvert = std::max(25000,xyzDim*2);
    vlist = (float *)calloc(mvert*3*2, sizeof(float));

    printf("Surface 0 at 10 of 100\n"); fflush(stdout);
    intsect_(&isoval, fgrid, &xDim, &yDim, &zDim,
     &nx0, &ny0, &nz0, &nxx, &nyy, &nzz, &nvert, &ntri,
     vlist, &mvert, &min[0], &min[1], &min[2], &step, &step, &step,
     &volume, &volcomp, tags, &ntags);
    qDebug() << "vertices =" << nvert << "of max" << mvert;
    qDebug() << "volume =" << volume;
    float scale = 1.0;
    float area;
    float tm[9]; //(3,3) fortran
    int ounit = 6; // fortran stdout
    unit_(tm);
    printf("Surface 0 at 30 of 100\n"); fflush(stdout);
    interp_(vlist, &nvert, &xDim, &yDim, &zDim,
     fgrid, &isoval, &min[0], &min[1], &min[2], &step, &step, &step, &scale, &area,
     &volcomp, &normcomp, tags, &ntags, &ounit, &outtype, tm);
    qDebug() << "area =" << area;
    free(vlist);
  }

  free(fgrid);
  free(tags);
  printf("Surface 0 at 60 of 100\n"); fflush(stdout);
  return;
}

/* these addtri and addtrinml could be called from interp */
extern "C" void addtri_(int *iatom, int *itri, float *x, float *y, float *z) {
  addQuery->bindValue(0,*iatom);
  addQuery->bindValue(1,*itri);
  addQuery->bindValue(2,*x);
  addQuery->bindValue(3,*y);
  addQuery->bindValue(4,*z);
  addQuery->exec();
}
extern "C" void addtrinml_(int *iatom, int *itri, float *x, float *y, float *z, float *nx, float *ny, float *nz) {
  addQuery->bindValue(0,*iatom);
  addQuery->bindValue(1,*itri);
  addQuery->bindValue(2,*x);
  addQuery->bindValue(3,*y);
  addQuery->bindValue(4,*z);
  addQuery->bindValue(5,*nx);
  addQuery->bindValue(6,*ny);
  addQuery->bindValue(7,*nz);
  addQuery->exec();
}

void usage() {
    std::cerr << "Usage: dbsurf <dbname> [options]" << std::endl;
    std::cerr << "Options:          Description:" << std::endl;
    std::cerr << "  -i              itemid in tree table" << std::endl;
    std::cerr << "  -v              contour value" << std::endl;
    std::cerr << "  -n              assign nearest atom (-db only)" << std::endl;
    std::cerr << "  -cube           output cube file" << std::endl;
    std::cerr << "  -db             output triangles to database" << std::endl;
    std::cerr << "  -pix            output triangles to stdout" << std::endl;
    std::cerr << "  -qix            output triangles to qix file" << std::endl;
    std::cerr << "  -s <stepsize>   step size" << std::endl;
    std::cerr << "  -p <padding>    padding" << std::endl;
    exit(-1);
}

int main(int argc,char **argv)
{

  QApplication app(argc, argv); // needed to get SQLITE plugin to work

  //char *program_name = argv[0];
  std::string option;
  std::string outfile;
  QSqlDatabase db;
  float step, padding;
  QString dbname;
  int itemid = 0;
  int imol = 0;
  char chain = NOCHAIN;
  int resnum = 0;
  int filter = 0;
  int hydrogen = 0;
  int outtype = 0;

  float isoval = 100.;
  step    = 0.5;
  padding = 5.0;

  if (argc < 2) {
    usage();

  } else {
    db = QSqlDatabase::addDatabase("QSQLITE");
    dbname = argv[1];
    db.setDatabaseName(dbname);
    if (!db.open()) {
      qDebug() << "can't open database" << dbname;
      usage();
    }
    QSqlQuery q;
    //q.exec("Pragma foreign_keys");
    //q.next();
    //qDebug() << "dbsurf open: foreign keys" << q.value(0).toInt();
    q.exec("Pragma foreign_keys = ON");


    for (int i = 2; i < argc; i++) {
      option = argv[i];
      
      if ((option == "-i") && (argc > (i+1))) {
        itemid = atoi(argv[++i]);

      } else if ((option == "-v") && (argc > (i+1))) {
        isoval = atof(argv[++i]);

      } else if ((option == "-pix")) {
        outtype = PIX;

      } else if ((option == "-qix")) {
        outtype = QIX;

      } else if ((option == "-db")) {
        outtype = DB;
        qDebug() << "output to" << dbname;

      } else if ((option == "-cube")) {
        outtype = CUBE;

      } else if ((option == "-s") && (argc > (i+1))) {
        step = atof(argv[++i]);

      } else if ((option == "-p") && (argc > (i+1))) {
        padding = atof(argv[++i]);

      }
    }
    
  }

  if (itemid) {
    treeQuery item;
    item.getRow(itemid);
    chain = item.chain;
    imol = item.imol;
    filter = item.filter;
    resnum = item.resnum;
    hydrogen = item.hydrogens;
  }
  if (outtype == DB) {
    QSqlQuery q;
    q.exec("Create Table If Not Exists vertex (itemid Integer, vid Integer, atid Integer, x Real, y Real, z Real, nx Real, ny Real, nz Real, Unique (itemid,vid) Foreign Key (itemid) References tree (itemid) On Delete Cascade)");
    q.exec("Create Table If Not Exists triangle (itemid Integer, atid Integer, tid Integer, vid Integer, Foreign Key (itemid) References tree (itemid) On Delete Cascade)");
    q.exec("Create /*Temporary*/ Table If Not Exists new_triangle (atid Integer, tid Integer, x Real, y Real, z Real, nx Real, ny Real, nz Real)");
    q.exec("Delete From new_triangle");

    /* addQuery will be used by addtri */
    addQuery = new QSqlQuery();
    qDebug() << addQuery->prepare("Insert Into new_triangle (atid,tid,x,y,z,nx,ny,nz) Values (?,?,?,?,?,?,?,?)");
    db.transaction();
  }

  qDebug() << imol << chain << resnum << filter;
  makeGrid(imol, step, padding, chain, resnum, filter, hydrogen, outtype, isoval);
  if (outtype == DB) {
    QSqlQuery q;
    q.exec("Drop Table If Exists uniq_vertices");
    q.exec("Create Temporary Table uniq_vertices As Select Distinct x,y,z,nx,ny,nz From new_triangle");
    q.prepare("Delete From vertex Where itemid=?");
    q.bindValue(0,itemid);
    q.exec();
    q.finish();

    //if (!q.prepare("Insert Into vertex (itemid,vid,x,y,z,nx,ny,nz) Select ?,rowid,x,y,z,nx,ny,nz From uniq_vertices")) 
    q.exec("Create Index uniq_v_xyz On uniq_vertices(x,y,z,nx,ny,nz)");
    q.exec("Create Index new_t_xyz  On uniq_vertices(x,y,z,nx,ny,nz)");
    if (!q.prepare("Insert Into vertex (itemid,vid,atid,x,y,z,nx,ny,nz) select ?,vid,min(atid),x,y,z,nx,ny,nz from (Select uniq_vertices.rowid vid,atid,x,y,z,nx,ny,nz From new_triangle Natural Join uniq_vertices) group by vid"))
        Db::tellError(q, "prepare");
    q.bindValue(0,itemid);
    if (!q.exec()) Db::tellError(q, "exec");

    q.prepare("Delete From triangle Where itemid=?"); q.bindValue(0,itemid); q.exec();
    q.exec("Drop Index If Exists main.triangle_itemid");
    q.prepare("Insert Into triangle (itemid,atid,tid,vid) Select ?,atid,tid,uniq_vertices.rowid From new_triangle Natural Join uniq_vertices");
    q.bindValue(0,itemid);
    q.exec();
    //qDebug() << "create triangle index" << q.exec("Create Index main.triangle_itemid On triangle (itemid)");
    db.commit();
    db.close();
  }


  fprintf (stderr, "itemid=%d\n", itemid); // communicate id back to calling proc
  return(1);
}
