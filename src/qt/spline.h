/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
#define CatRom  1
#define Bezier  2
#define Bspline 3
#define PI 3.141592654
#define MAXSIDES 40

float *makeSpline (float *inbuff, int np, int nt, int stype, bool extrapolate);
float *wrap(float c[][3], float g[][3], int n, float xrad, float yrad, int ldm2, int nsides);
void cross(float C[3], float A[3], float B[3]);
float vangle(float a[3], float b[3]);
