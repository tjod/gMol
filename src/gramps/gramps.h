/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
/* Must correspond with values in initvar.f */
#define XMOUSS 256
#define MOUSS 256
#define YMOUSS 262144
#define DIALS  1
#define SLIDERS 268435456

#define PICKWIDTH 5

void snapscreen_(char *filename, int width, int height, const unsigned char *rgbbuff);
void display_(int *forpick);
void grout_(char *tmpline, int tmplen);
void g0redraw_(int *display_changed);
int do_(char *gr_cmd, int cmdlen);
int get_pick_wait_();
int contupd_(int *do_it);
void advsnap_();
int update_(int *type, int *ndial, float *xval, float *xinc);
void g0hires_(int *flag);
int g0pickprocess(int mx, int my, float (*xyzw)[4]);
void g0pick_(int *zhit, int *mx, int *my, float (*xyzw)[4]);
void g0wset_(int *x0, int *y0, int *x, int *y, int *flag);
void setfrom_(int *outflag);
void udinit_();
void g0pinit_();
void startup_();
