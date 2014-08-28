/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
#include <openbabel/mol.h>
#include <openbabel/obconversion.h>
#include <openbabel/grid.h>
#include <openbabel/griddata.h>
#include <string.h>

using namespace std;
using namespace OpenBabel;

extern "C" int readgrid_(float *vdata, int *nx, int *ny, int *nz,
  float *x0, float *y0, float *z0, float *xx, float *yy, float *zz,
  char *file, int fsize) {

  float dx,dy,dz;
  int gsize;
  float v, vmin, vmax, vavg;
  int navg = 0;

  OBMol mol;
  OBConversion conv;
  conv.SetInFormat("cube");
 
  string fname = file;
  int blank = fname.find(" ");
  //printf("%d,'%s'\n", blank, fname.substr(0,blank).c_str());
  conv.ReadFile(&mol, fname.substr(0,blank).c_str());
  //cout << mol.NumAtoms() << " atoms." << endl;
  if (mol.HasData(OBGenericDataType::GridData)) {
    vector<OBGenericData*> grids = mol.GetAllData(OBGenericDataType::GridData);
    OBGridData *grid = dynamic_cast<OBGridData *> (grids[0]);
    gsize = grid->GetNumberOfPoints();
    grid->GetNumberOfPoints(*nx, *ny, *nz);
    vector3 origin = grid->GetOriginVector();
    *x0 = origin[0]; *y0 = origin[1]; *z0 = origin[2];
    vector3 maxv = grid->GetMaxVector();
    *xx = maxv[0]; *yy = maxv[1]; *zz = maxv[2];
    dx=(*xx-*x0)/(*nx-1);
    dy=(*yy-*y0)/(*ny-1);
    dz=(*zz-*z0)/(*nz-1);
    printf("%s %d=%d*%d*%d\n", grid->GetAttribute().c_str(), gsize, *nx, *ny, *nz);
    printf("%f %f\n", grid->GetMinValue(), grid->GetMaxValue());
    printf("%f,%f,%f\n", *x0,*y0,*z0);
    printf("%f,%f,%f\n", *xx,*yy,*zz);
    printf("%f,%f,%f\n", dx,dy,dz);
 
    //vdata = (float *)calloc(gsize, sizeof(float));
/* this is for fortran, so reverse sense of slowest/fastest moving dimensions */
    //for (int i=0; i<*nx; ++i) {
    vmin = grid->GetValue(0,0,0);
    vmax = vmin;
    for (int i=0; i<*nz; ++i) {
      for (int j=0; j<*ny; ++j) {
        //for (int k=0; k<*nz; ++k) {
        for (int k=0; k<*nx; ++k) {
          //*vdata++ = grid->GetValue(i,j,k);
          v = grid->GetValue(k,j,i);
          if (v < 1e30 && v > -1e30) {
            if (v < vmin) vmin = v;
            if (v > vmax) vmax = v;
            ++navg;
            vavg += v;
          } else {
            v = vmax * 1000; // just a guess
          }
          *vdata++ = v;
        }
      }
    }
  }
  vavg = vavg/navg;
  printf("min/avg/max = %f/%f/%f\n", vmin, vavg, vmax);
}
 

/* try to make this a main program to call iso routines....maybe someday */
/*
  int nx0,ny0,nz0,nxx,nyy,nzz,nvert,ntri;
  float isoval;
#define MAXTRI 200000
#define MAXVERT MAXTRI*3
  float vlist[MAXVERT][2][3];
  float tlist[MAXTRI][3];
  float *parea, *pvol;
  int *tags;
  int ntags = gsize;
  tags  = (int *)calloc(ntags, sizeof(float));
  parea = (float *)calloc(ntags, sizeof(float));
  pvol  = (float *)calloc(ntags, sizeof(float));
  bool volcomp,normcomp;
  float volume;
  volcomp = true;
  normcomp = true;
  isoval = 1600;
  int mvert = MAXVERT;
  int mtri = MAXTRI;

  intsect_(&isoval, vdata, &nx, &ny, &nz,
       &nx0, &ny0, &nz0, &nxx, &nyy, &nzz, &nvert, &ntri,
       vlist, &mvert, &x0, &y0, &z0, &dx, &dy, &dz,
       &volume, &pvol, &volcomp, &normcomp, tags, &ntags);

  vsort_(&nvert, &ntri, vlist, &mvert, tlist);

  int ounit = 9;
  int outfile = 1;
  float scale = 1;
  float volume,area;
  interp_(vlist, &nvert, &ntri, &nx, &ny, &nz,
       &nx0, &ny0, &nz0, &nxx, &nyy, &nzz,
       vdata, &isoval(i), &x0, &y0, &z0, &dx, &dy, &dz, &scale, &area,
       &parea, &volcomp, &normcomp, tags, &ntags, ounit, &outfile, &tm);

*/
