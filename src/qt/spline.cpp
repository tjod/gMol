/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2013 by TJ O'Donnell and
C  Copyright (C) 2005-2013 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#ifdef __APPLE__
#include <sys/malloc.h>
#else
#include <malloc.h>
#endif
#include <math.h>

#include "spline.h"
void Spline(float M[4][4], float Mfact, float t,
    float x3, float y3, float z3, float x2, float y2, float z2, float x1, float y1, float z1, float x, float y, float z,
    float *qx, float *qy, float *qz, float *qw) {
  float t2, t3;
/*
   Do Rational Bspline, but assuming w=1 for all x,y,z.
*/
  t2=t*t;
  t3=t2*t;
  *qx = ((t3*M[0][0] + t2*M[0][1] + t*M[0][2] + M[0][3])*x3
      +  (t3*M[1][0] + t2*M[1][1] + t*M[1][2] + M[1][3])*x2
      +  (t3*M[2][0] + t2*M[2][1] + t*M[2][2] + M[2][3])*x1
      +  (t3*M[3][0] + t2*M[3][1] + t*M[3][2] + M[3][3])*x )/Mfact;
  *qy = ((t3*M[0][0] + t2*M[0][1] + t*M[0][2] + M[0][3])*y3
      +  (t3*M[1][0] + t2*M[1][1] + t*M[1][2] + M[1][3])*y2
      +  (t3*M[2][0] + t2*M[2][1] + t*M[2][2] + M[2][3])*y1
      +  (t3*M[3][0] + t2*M[3][1] + t*M[3][2] + M[3][3])*y )/Mfact;
  *qz = ((t3*M[0][0] + t2*M[0][1] + t*M[0][2] + M[0][3])*z3
      +  (t3*M[1][0] + t2*M[1][1] + t*M[1][2] + M[1][3])*z2
      +  (t3*M[2][0] + t2*M[2][1] + t*M[2][2] + M[2][3])*z1
      +  (t3*M[3][0] + t2*M[3][1] + t*M[3][2] + M[3][3])*z )/Mfact;
  *qw = ((t3*M[0][0] + t2*M[0][1] + t*M[0][2] + M[0][3])
      +  (t3*M[1][0] + t2*M[1][1] + t*M[1][2] + M[1][3])
      +  (t3*M[2][0] + t2*M[2][1] + t*M[2][2] + M[2][3])
      +  (t3*M[3][0] + t2*M[3][1] + t*M[3][2] + M[3][3]))/Mfact;
 
  return;
}

float *makeSpline (float *inbuff, int np, int nt, int stype, bool extrapolate=true) {
  float *x, *y, *z;
  float qx, qy, qz, qw;
  int i,j;
  int nout;
  float *outbuff;
  int i0, iend, iskip, istart;
  int jend;
  float t,dt;
  //int n = np;
 
  float MCR[4][4] = {{-1,2,-1,0}, {3,-5,0,2},  {-3,4,1,0}, {1,-1,0,0}};
  float MCRfact = 2.;
  float MBs[4][4] = {{-1,3,-3,1}, {3,-6,0,4}, {-3,3,3,1}, {1,0,0,0}};
  float MBsfact = 6.;
  float MBz[4][4] = {{-1,3,-3,1}, {3,-6,3,0}, {-3,3,0,0}, {1,0,0,0}};
  float MBzfact = 1.;
 
  x = (float *) calloc(np+2, sizeof(float));
  y = (float *) calloc(np+2, sizeof(float));
  z = (float *) calloc(np+2, sizeof(float));
  i=0;
  istart=0;
  if (extrapolate) istart = 1;
  for (i=istart,j=0; i<=istart+np; ++i,j+=3) {
    x[i] = inbuff[j];
    y[i] = inbuff[j+1];
    z[i] = inbuff[j+2];
  }
 
  if(nt < 0 || nt > 100) {
    fprintf(stderr, "unreasonable nt value given: using 6\n");
    nt=6;
  }
  dt=1./(nt+1);
 
  int iout = 0;
  if(stype == Bezier) {
    nout = 1 + np*(nt+1);
    iend=np;
    i0 = 1;
    iskip = 3;
  } else {
    i0 = 3;
    iskip = 1;
    if (extrapolate) {
      x[np+1]=2*x[np]-x[np-1];
      y[np+1]=2*y[np]-y[np-1];
      z[np+1]=2*z[np]-z[np-1];
      x[0]  =2*x[1]-x[2];
      y[0]  =2*y[1]-y[2];
      z[0]  =2*z[1]-z[2];
      nout = 1+(np-1)*(nt+1);
      iend=np+1;
    } else {
      //n = np-2;
      nout = 1+(np-1)*(nt+1);
      iend=np+1;
      istart = 0;
    }
  }
  outbuff = (float *) calloc(3*(nout+1), sizeof(float));
  outbuff[  iout] = nout;
  outbuff[++iout] = 2; // assume line output
  outbuff[++iout] = 3;

  for (i=i0; i<=iend; i+=iskip) {
    t = 0.0;
    if(i == iend) {
      jend=nt+2;
    } else {
      jend=nt+1;
    }
    for (j=0; j<jend; ++j) {
      if(stype == Bspline) {
        Spline(MBs, MBsfact, t,
          x[i-3], y[i-3], z[i-3],
          x[i-2], y[i-2], z[i-2],
          x[i-1], y[i-1], z[i-1],
          x[i  ], y[i  ], z[i  ],
          &qx, &qy, &qz, &qw);
    } else if(stype == Bezier) {
      Spline(MBz, MBzfact, t,
        x[i  ], y[i  ], z[i  ],
        x[i+1], y[i+1], z[i+1],
        x[i+2], y[i+2], z[i+2],
        x[i+3], y[i+3], z[i+3],
        &qx, &qy, &qz, &qw);
    } else if(stype == CatRom) {
      Spline(MCR, MCRfact, t,
        x[i-3], y[i-3], z[i-3],
        x[i-2], y[i-2], z[i-2],
        x[i-1], y[i-1], z[i-1],
        x[i  ], y[i  ], z[i  ],
        &qx, &qy, &qz, &qw);
    }
    outbuff[++iout] = qx/qw;
    outbuff[++iout] = qy/qw;
    outbuff[++iout] = qz/qw;
    t=t+dt;
    }
  }
  if(stype == Bezier) {
    outbuff[++iout] = x[iend+1];
    outbuff[++iout] = y[iend+1];
    outbuff[++iout] = z[iend+1];
  }
  free(x);
  free(y);
  free(z);
  return outbuff;
}

extern "C" void aaxis_(float c[4], float qa[4], float qb[4], float matrix[4][4], float imatrix[4][4], int *use3);
void matmul(float imatrix[4][4], float circle[][3], float tcircle[][3], int nsides);
float *wrap(float c[][3], float guide[][3], int n, float xrad, float yrad, int ldm2,  int nsides) {
 
  float dangle, angle;
  float qa[4], qb[4], qc[4];
  float matrix[4][4], imatrix[4][4];
  float  circle[MAXSIDES+1][3];
  float tcircle[MAXSIDES+1][3];
  int i,j;

  if (nsides > MAXSIDES) nsides = MAXSIDES;
  dangle=2.*PI/nsides;
  angle=0.;
  angle=0.;
  qa[3] = 1; qb[3] = 1; qc[3] = 1;
  for (i=0; i<nsides; ++i) {
    circle[i][0]=cos(angle)*xrad;
    circle[i][1]=sin(angle)*yrad;
    circle[i][2]=0.;
    angle=angle+dangle;
  }
  circle[nsides][0] = circle[0][0];
  circle[nsides][1] = circle[0][1];
  circle[nsides][2] = circle[0][2];

  //fprintf(stderr, "%d,%d,%d\n", n*(nsides+1),7,ldm2);
  //if (guide) fprintf(stderr, "%d,%d,%d\n", n*2,0,3);
  float *outbuff = (float *)calloc(3 + n*(nsides+1)*6 + 6, sizeof(float));
  int nout = 0;
  outbuff[nout++] = n*(nsides+1);
  outbuff[nout++] = 7;
  outbuff[nout++] = ldm2;
  int use3 = 0;
  for (i=0; i<n; ++i) {
    qa[0] = c[i][0]; qa[1] = c[i][1]; qa[2] = c[i][2];
    if (i+1 == n) {
      // extrapolate off the end of chain
      qb[0] = 2*c[i][0] - c[i-1][0];
      qb[1] = 2*c[i][1] - c[i-1][1];
      qb[2] = 2*c[i][2] - c[i-1][2];
/*
    } else if (i > 0) {
      qb[0] = c[i][0] + c[i+1][0] - c[i-1][0];
      qb[1] = c[i][1] + c[i+1][1] - c[i-1][1];
      qb[2] = c[i][2] + c[i+1][2] - c[i-1][2];
*/
    } else {
      qb[0] = c[i+1][0];
      qb[1] = c[i+1][1];
      qb[2] = c[i+1][2];
    }
    if (guide) {
      qc[0] = guide[i][0];
      qc[1] = guide[i][1];
      qc[2] = guide[i][2];
      use3 = 2;
    }
    aaxis_(qa,qb,qc,matrix,imatrix,&use3);
    matmul(imatrix,circle,tcircle,nsides+1);
    for (j=0; j<nsides+1; ++j) {
      outbuff[nout++] = tcircle[j][0];
      outbuff[nout++] = tcircle[j][1];
      outbuff[nout++] = tcircle[j][2];
      outbuff[nout++] = tcircle[j][0] - qa[0];
      outbuff[nout++] = tcircle[j][1] - qa[1];
      outbuff[nout++] = tcircle[j][2] - qa[2];

/*
      fprintf(stderr,"%f,%f,%f,%f,%f,%f\n",
        tcircle[j][0], tcircle[j][1], tcircle[j][2],
        tcircle[j][0] - qa[0], tcircle[j][1] - qa[1], tcircle[j][2] - qa[2]); //normal
*/
    }

    if (guide) {
      //fprintf(stderr,"%f,%f,%f\n%f,%f,%f\n",
      //  tcircle[0][0], tcircle[0][1], tcircle[0][2],
      //  guide[i][0], guide[i][1], guide[i][2]);
    } else {
      qc[0] = tcircle[0][0] + qb[0] - qa[0];
      qc[1] = tcircle[0][1] + qb[1] - qa[1];
      qc[2] = tcircle[0][2] + qb[2] - qa[2];
      use3 = 2;
    }
/*
    qc[0] = tcircle[nsides][0];
    qc[1] = tcircle[nsides][1];
    qc[2] = tcircle[nsides][2];
*/
  }
 
/*
  fprintf(stderr,"%d,%d,%d\n",1,8,3);
  fprintf(stderr,"%d,%d,%d\n",nsides+1,n,0);
*/
  outbuff[nout++] = 1; outbuff[nout++] = 8; outbuff[nout++] = 3;
  outbuff[nout++] = nsides+1; outbuff[nout++] = n; outbuff[nout++] = 0;
  return outbuff;
}

float xangle(float a0[3], float a[3], float b0[3],float b[3]) {

  float dot, alen, blen;
  //Compute the angle between two vectors
  dot = 0;
  alen = 0;
  blen = 0;
  for (int i=0; i<3; ++i) {
    dot = dot + (a[i]-a0[i])*(b[i]-b0[i]);
    alen =  alen + pow(a[i]-a0[i],2);
    blen =  blen + pow(b[i]-b0[i],2);
  }
  return acos( dot / sqrt(alen*blen) );
}

void matmul(float t[4][4], float v[][3], float q[][3], int n) {

  int i,j,k;
  float anew[4];
  float aold[4];

//   Multiply n elements of vector v by matrix t yielding q.
  for (i=0; i<n; ++i) {

//   set up 4-d vector copy of input v
    for (k=0; k<3; ++k) {
      aold[k]=v[i][k];
    }
    aold[3]=1.;

//   do the 4x4 matrix multiply to get new 4-d vector
    for (j=0; j<4; ++j) {
      anew[j]=0.0;
      for (k=0; k<4; ++k) {
        anew[j] += aold[k]*t[j][k];  //t is from fortran, so (k,j) is [j][k]
      }
    }

//   normalize and copy new into output q
    for (k=0; k<3; ++k) {
      q[i][k]=anew[k]/anew[3];
    }

  }
  return;
}
void normalize(float C[3]) {
  float clen = sqrt(C[0]*C[0] + C[1]*C[1] + C[2]*C[2]);
  C[0] = C[0]/clen;
  C[1] = C[1]/clen;
  C[2] = C[2]/clen;
}
void cross(float C[3], float A[3], float B[3]) {
/* C = A x B */
  C[0] = A[1]*B[2] - A[2]*B[1];
  C[1] = A[2]*B[0] - A[0]*B[2];
  C[2] = A[0]*B[1] - A[1]*B[0];
  normalize(C);
  return;
}
float vangle(float a[3], float b[3]) {
  float alen = sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
  float blen = sqrt(b[0]*b[0] + b[1]*b[1] + b[2]*b[2]);
  float dot = a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
  return acos(dot / (alen*blen));
}
/*
int main (int argc, char **argv) {
  int n,l1,l2;
  fscanf(stdin, "%d%*[ ,]%d%*[ ,]%d", &n, &l1, &l2);
  float *inbuff = (float *) calloc(n*3, sizeof(float));
  for (int i=0,j=0; i<n; ++i,j+=3) {
    fscanf(stdin, "%f%*[ ,]%f%*[ ,]%f", &inbuff[j], &inbuff[j+1],&inbuff[j+2]);
  }
  float *outbuff = makeSpline (inbuff, n, 6, CatRom);
  //float *outbuff = makeSpline (inbuff, n, 6, Bspline);
  //float *outbuff = makeSpline (inbuff, n, 6, Bezier);
  int nout = outbuff[0];
  int l1out = outbuff[1];
  int l2out = outbuff[2];
  printf("%d,%d,%d\n", nout, l1out, l2out);
  for (int i=0,j=3; i<nout; ++i,j+=3) {
    fprintf(stdout, "%f,%f,%f\n", outbuff[j], outbuff[j+1],outbuff[j+2]);
  }
  free (outbuff);
}
*/
