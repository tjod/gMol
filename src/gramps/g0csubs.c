/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#ifdef QT
// for text
#include "glutbitmap.h"
#else
#endif

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gramps.h"

/* G0 routines that used to be in g0subs.f */

#define MAXTPAT 9 
int trpat_(float *transparency) {
int i;
int pattern;
int nbits;
#ifdef GLES
  //printf("%f\n", *transparency);
  glSampleCoverage(*transparency, GL_TRUE);
  return;
#endif

static GLubyte mask[128]; /* 4 8-bit-bytes is 32 bits and 32*4 = 128 */

/* 1024 bits in 32 by 32 mask */
 if (*transparency <= 0.0) {
 }
 pattern = *transparency*MAXTPAT + 0.99;
 if (pattern < 0) pattern = 0;
 if (pattern > MAXTPAT) pattern = MAXTPAT;
 switch (pattern) {

 case 0:
  glDisable(GL_POLYGON_STIPPLE);
  return(0);
/*
#include "patterns.h"
 case 9: mask = mask9; break;
 case 8: mask = mask8; break;
 case 7: mask = mask7; break;
 case 6: mask = mask6; break;
 case 5: mask = mask5; break;
 case 4: mask = mask4; break;
 case 3: mask = mask3; break;
 case 2: mask = mask2; break;
 case 1: mask = mask1; break;
*/
 case 10:
  for (i=0; i<128; ) {
   mask[i++] = 0x81; mask[i++] = 0x81; mask[i++] = 0x81; mask[i++] = 0x81;
   mask[i++] = 0x24; mask[i++] = 0x24; mask[i++] = 0x24; mask[i++] = 0x24;
   mask[i++] = 0x42; mask[i++] = 0x42; mask[i++] = 0x42; mask[i++] = 0x42;
   mask[i++] = 0x18; mask[i++] = 0x18; mask[i++] = 0x18; mask[i++] = 0x18;
  }
  break;

 case 9:
  for (i=0; i<128; ) {
   mask[i++] = 0x81; mask[i++] = 0x81; mask[i++] = 0x81; mask[i++] = 0x81;
   mask[i++] = 0x26; mask[i++] = 0x26; mask[i++] = 0x26; mask[i++] = 0x26;
   mask[i++] = 0x62; mask[i++] = 0x62; mask[i++] = 0x62; mask[i++] = 0x62;
   mask[i++] = 0x18; mask[i++] = 0x18; mask[i++] = 0x18; mask[i++] = 0x18;
  }
  break;

 case 8:
  for (i=0; i<128; ) {
   mask[i++] = 0x1A; mask[i++] = 0x1A; mask[i++] = 0x1A; mask[i++] = 0x1A;
   mask[i++] = 0x52; mask[i++] = 0x52; mask[i++] = 0x52; mask[i++] = 0x52;
   mask[i++] = 0xA1; mask[i++] = 0xA1; mask[i++] = 0xA1; mask[i++] = 0xA1;
   mask[i++] = 0x25; mask[i++] = 0x25; mask[i++] = 0x25; mask[i++] = 0x25;
  }
  break;

 case 7:
  for (i=0; i<128; ) {
   mask[i++] = 0xAA; mask[i++] = 0xAA; mask[i++] = 0xAA; mask[i++] = 0xAA;
   mask[i++] = 0x51; mask[i++] = 0x51; mask[i++] = 0x51; mask[i++] = 0x51;
   mask[i++] = 0xAA; mask[i++] = 0xAA; mask[i++] = 0xAA; mask[i++] = 0xAA;
   mask[i++] = 0x15; mask[i++] = 0x15; mask[i++] = 0x15; mask[i++] = 0x15;
  }
  break;

 case 6:
  for (i=0; i<128; ) {
   mask[i++] = 0xAA; mask[i++] = 0xAA; mask[i++] = 0xAA; mask[i++] = 0xAA;
   mask[i++] = 0x55; mask[i++] = 0x55; mask[i++] = 0x55; mask[i++] = 0x55;
  }
  nbits = 512;
  break;

 case 5:
  for (i=0; i<128; ) {
   mask[i++] = 0x55; mask[i++] = 0x55; mask[i++] = 0x55; mask[i++] = 0x55;
   mask[i++] = 0xAA; mask[i++] = 0xAA; mask[i++] = 0xAA; mask[i++] = 0xAA;
  }
  nbits = 512;
  break;

 case 4:
  for (i=0; i<128; ) {
   mask[i++] = 0xAA; mask[i++] = 0xAA; mask[i++] = 0xAA; mask[i++] = 0xAA;
   mask[i++] = 0x57; mask[i++] = 0x57; mask[i++] = 0x57; mask[i++] = 0x57;
   mask[i++] = 0xAA; mask[i++] = 0xAA; mask[i++] = 0xAA; mask[i++] = 0xAA;
   mask[i++] = 0x75; mask[i++] = 0x75; mask[i++] = 0x75; mask[i++] = 0x75;
  }
  break;

 case 3:
  for (i=0; i<128; ) {
   mask[i++] = 0x7A; mask[i++] = 0x7A; mask[i++] = 0x7A; mask[i++] = 0x7A;
   mask[i++] = 0x57; mask[i++] = 0x57; mask[i++] = 0x57; mask[i++] = 0x57;
   mask[i++] = 0xA7; mask[i++] = 0xA7; mask[i++] = 0xA7; mask[i++] = 0xA7;
   mask[i++] = 0x75; mask[i++] = 0x75; mask[i++] = 0x75; mask[i++] = 0x75;
  }
  break;

 case 2:
  for (i=0; i<128; ) {
  // mask[i++] = 0xFF; mask[i++] = 0xFF; mask[i++] = 0xFF; mask[i++] = 0xFF;
  // mask[i++] = 0x66; mask[i++] = 0x66; mask[i++] = 0x66; mask[i++] = 0x66;
  // mask[i++] = 0x66; mask[i++] = 0x66; mask[i++] = 0x66; mask[i++] = 0x66;
  // mask[i++] = 0xFF; mask[i++] = 0xFF; mask[i++] = 0xFF; mask[i++] = 0xFF;
   mask[i++] = 0x77; mask[i++] = 0xFF; mask[i++] = 0x77; mask[i++] = 0xFF;
   mask[i++] = 0xCB; mask[i++] = 0xCB; mask[i++] = 0xCB; mask[i++] = 0xCB;
   mask[i++] = 0xBC; mask[i++] = 0xBC; mask[i++] = 0xBC; mask[i++] = 0xBC;
   mask[i++] = 0xFF; mask[i++] = 0x77; mask[i++] = 0xFF; mask[i++] = 0x77;
  }
  break;

 case 1:
  for (i=0; i<128; ) {
   mask[i++] = 0xFF; mask[i++] = 0xFF; mask[i++] = 0xFF; mask[i++] = 0xFF;
   mask[i++] = 0xCB; mask[i++] = 0xCB; mask[i++] = 0xCB; mask[i++] = 0xCB;
   mask[i++] = 0xBC; mask[i++] = 0xBC; mask[i++] = 0xBC; mask[i++] = 0xBC;
   mask[i++] = 0xFF; mask[i++] = 0xFF; mask[i++] = 0xFF; mask[i++] = 0xFF;
  }
  break;

 }

 glEnable(GL_POLYGON_STIPPLE);
 glPolygonStipple(mask);
 return nbits;

}

rayshow_(name, options, namelen, optlen)
char *name;
char *options;
int namelen;
int optlen;
{
char cmd[256];
char file[256];
char opt[256];
strncpy(file, name, namelen);
strncpy(opt, options, optlen);
file[namelen]='\0';
opt[optlen]='\0';

sprintf(cmd, "time rayt %s %s | getiris\n", opt, file);
printf("%s", cmd);
system(cmd);
}

psplot_(name, color, namelen)
char *name;
int *color;
int namelen;
{
char cmd[256];
char file[256];
strncpy(file, name, namelen);
file[namelen]='\0';

switch (*color)
        {
case 0: sprintf(cmd, "lp -ocooked %s\n", file);
        break;
case 1: sprintf(cmd, "lpr -Pcs100_paper %s;sleep 1;lpq -Pcs100_paper\n", file);
        break;
case 2: sprintf(cmd, "lpr -Pcs100_trans %s;sleep 1;lpq -Pcs100_trans\n", file);
        break;
        }
printf("%s", cmd);
system(cmd);
}

void commout_(char *string) {
/* output to command window */
}

savewin_(name, nlen, namlen)
char *name;
int namlen;
int *nlen;
{

 char filename[256];

#ifdef ImageMagick
#include <GL/glx.h>
 char cmd[256];
 GLXDrawable xwin = glXGetCurrentDrawable();
 sprintf(cmd, "import -window 0x%x %.*s\0",xwin, *nlen, name);
 system(cmd);
#endif
 GLint viewport[4];
 unsigned char *rgbbuf;
 int width, height;
 FILE *fp;
 glGetIntegerv(GL_VIEWPORT, viewport);
 width  = viewport[2];
 height = viewport[3];
 glPixelStorei(GL_PACK_ALIGNMENT, 1);
 rgbbuf = (unsigned char *)calloc(3*width*height, sizeof(unsigned char)); 
 if (NULL == rgbbuf) {
  fprintf(stderr, "can't allocate rgb buffer\n");
  return;
 }
 glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, rgbbuf);
 sprintf(filename, "%.*s\0", *nlen, name);
#if defined QT
 snapscreen(filename, width, height, rgbbuf); 
#else
 fp = fopen(filename, "wb");
 if (NULL == fp) {
  fprintf(stderr, "can't open %s\n", filename);
  return;
 }
#if defined libjpeg
#include <jpeglib.h>
 struct jpeg_compress_struct jpg;
 struct jpeg_error_mgr jerr;
 jpg.err = jpeg_std_error(&jerr);
 jpeg_create_compress(&jpg);
 jpeg_stdio_dest(&jpg, fp);
 jpg.image_width = width;      /* image width and height, in pixels */
 jpg.image_height = height;
 jpg.input_components = 3;     /* # of color components per pixel */
 jpg.in_color_space = JCS_RGB; /* colorspace of input image */
 jpeg_set_defaults(&jpg);
 jpeg_set_quality(&jpg, 100, FALSE);
 jpeg_start_compress(&jpg, TRUE);
 JSAMPROW row_pointer[1];        /* pointer to a single row */
 int row_stride = width * 3;   /* JSAMPLEs per row in image_buffer */
 while (jpg.next_scanline < jpg.image_height) {
     row_pointer[0] = &rgbbuf[(jpg.image_height-1 - jpg.next_scanline) * row_stride];
     jpeg_write_scanlines(&jpg, row_pointer, 1);
 }
 jpeg_finish_compress(&jpg);
 jpeg_destroy_compress(&jpg);
#else
 fwrite(rgbbuf,sizeof(unsigned char),width*height*3,fp);
#endif
 fclose(fp);
#endif
 free(rgbbuf);
}

int g0texture_(int *iobj) {
	glCallList(*iobj);
}
int g0texture_init__(int *iobj) {
	static unsigned char roygbiv[8][3] =
	{
		{0x3f, 0x00, 0x3F}, /*Dark violet*/
		{0x7f, 0x00, 0x7f}, /*Violet*/
		{0xbf, 0x00, 0xbf}, /*Indigo*/
		{0x00, 0x00, 0xff}, /*Blue*/
		{0x00, 0xff, 0x00}, /*Green*/
		{0xff, 0xff, 0x00}, /*Yellow*/
		{0xff, 0x7f, 0x00}, /*Orange*/
		{0xff, 0x00, 0x00}, /*Red*/
	};

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_1D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glNewList(*iobj, GL_COMPILE);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage1D(GL_TEXTURE_1D, 0, 3, 8, 0,
		       	GL_RGB, GL_UNSIGNED_BYTE, roygbiv);

	return(1);
}
g0texcoord1f_(float *c) {
	glTexCoord1f(*c);
}

void g0colormaterial_(int *flag) {
static int state=-1;

 if (*flag == state) return;
 if (*flag == 1) {
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  state = 1;
 } else {
  glDisable(GL_COLOR_MATERIAL);
  state = 0;
 }

}

#if defined __APPLE__
void g0light_init_ () {
#elif defined GFORTRAN
void g0light_init_ () {
#else
void g0light_init__ () {
#endif

 /* setup lighting, etc */

 int zero=0;
 glEnable(GL_LIGHTING);
 glDisable(GL_COLOR_MATERIAL);
 g0colormaterial_(&zero);
}

void g0initfog_() {
 glEnable(GL_FOG);                 // Turn Fog on
 glFogi(GL_FOG_MODE, GL_LINEAR);   // Which fog equation do I use?
// glFogi(GL_FOG_MODE, GL_EXP);   // Which fog equation do I use?
// glFogi(GL_FOG_MODE, GL_EXP2);   // Which fog equation do I use?
 glHint(GL_FOG_HINT, GL_NICEST);
}
void setfog_(GLfloat *fog_start, GLfloat *fog_end, GLfloat fogColor[4]) {
// fprintf(stderr, "fog_start %f, fog_end %f\n", fog_start, fog_end);
// GLfloat fogColor[4] = {0.0, 0.0, 0.5, 1.0};
 glFogf(GL_FOG_START, *fog_start);       // How far away does the fog start
 glFogf(GL_FOG_END,   *fog_end);         // How far away does the fog stop
 glFogfv(GL_FOG_COLOR, fogColor);
}

#if defined __APPLE__
void g0line_width_(float *v) { glLineWidth(*v); }
#elif defined GFORTRAN
void g0line_width_(float *v) { glLineWidth(*v); }
#else
void g0line_width__(float *v) { glLineWidth(*v); }
#endif

setsmooth_(int *flag) {
	if (*flag == 0) {
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POINT_SMOOTH);
//		glDisable(GL_POLYGON_SMOOTH);
	} else {
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POINT_SMOOTH);
//		glEnable(GL_POLYGON_SMOOTH);
	}
}

g0normalization_(int *type) {
/* GL_RESCALE_NORMAL works for isotropic scaling (e.g. sphere primitive)*/
/* GL_NORMALIZE works for any scaling  (e.g. cylinder primitive)*/
 if (*type == 1) {
  glDisable(GL_NORMALIZE);
#ifndef MINGW32
  glEnable(GL_RESCALE_NORMAL);
#endif
 } else if (*type == 2) {
#ifndef MINGW32
  glDisable(GL_RESCALE_NORMAL);
#endif
  glEnable(GL_NORMALIZE);
 } else {
  glDisable(GL_NORMALIZE);
#ifndef MINGW32
  glDisable(GL_RESCALE_NORMAL);
#endif
 }
}

void lmdef_(int *ilit, float *r, float *g, float *b,
 float *iamb, float *idiff, float *ispec)
{
 GLenum lnames[] =
  {GL_LIGHT0,GL_LIGHT1,GL_LIGHT2,GL_LIGHT3,
   GL_LIGHT4,GL_LIGHT5,GL_LIGHT6,GL_LIGHT7};
 GLfloat zaxis[] = { 0.0, 0.0, 100.0, 0.0 };
 GLfloat rgba[4];

 rgba[0] = *r * (*iamb); rgba[1] = *g * (*iamb); rgba[2] = *b * (*iamb);
 glLightfv(lnames[*ilit],GL_AMBIENT, rgba);
 rgba[0] = *r * (*idiff); rgba[1] = *g * (*idiff); rgba[2] = *b * (*idiff);
 glLightfv(lnames[*ilit],GL_DIFFUSE, rgba);
 rgba[0] = *r * (*ispec); rgba[1] = *g * (*ispec); rgba[2] = *b * (*ispec);
 glLightfv(lnames[*ilit],GL_SPECULAR, rgba);

 glLightfv(lnames[*ilit],GL_POSITION, zaxis);
}

void lmbind_(int *ilit, int *flag) {
 GLenum lnames[] =
  {GL_LIGHT0,GL_LIGHT1,GL_LIGHT2,GL_LIGHT3,
   GL_LIGHT4,GL_LIGHT5,GL_LIGHT6,GL_LIGHT7};
 if (*flag) {
  glEnable(lnames[*ilit]);
 } else {
  glDisable(lnames[*ilit]);
 }
}

g0material_ (float *red, float *green, float *blue, float *alpha,
 float *ambient, float *diffuse, float *specular, float *emissive,
 float *shiny, int *backface, float bfcolor[])  {

 GLfloat tmp[4];
 tmp[3] = 1.0 - *alpha;
 GLfloat ared, agreen, ablue;
 GLfloat aambi, adiff, aspec;
 int aface;
 if (*backface == 1) {
  aface = GL_BACK;
  ared = bfcolor[0];
  agreen = bfcolor[1];
  ablue = bfcolor[2];
  aambi = *ambient;
  adiff = *diffuse;
  aspec = *specular;
 } else if (*backface == -1) {
  aface = GL_BACK;
  ared = *red;
  agreen = *green;
  ablue = *blue;
  aambi = *ambient*bfcolor[0];
  adiff = *diffuse*bfcolor[1];
  aspec = *specular*bfcolor[2];
 } else {
  aface = GL_FRONT;
  ared = *red;
  agreen = *green;
  ablue = *blue;
  aambi = *ambient;
  adiff = *diffuse;
  aspec = *specular;
 /* kludge for lines */
  tmp[0] = ared  *(aambi+adiff+aspec)*0.75;
  tmp[1] = agreen*(aambi+adiff+aspec)*0.75;
  tmp[2] = ablue *(aambi+adiff+aspec)*0.75;
  glColor4fv(tmp);
/* end of kludge for lines */
 }

 if (aambi >= 0) {
  tmp[0] = ared*aambi;
  tmp[1] = agreen*aambi;
  tmp[2] = ablue*aambi;
  glMaterialfv(aface, GL_AMBIENT, tmp);
 }

 if (adiff >= 0) {
  tmp[0] = ared*adiff;
  tmp[1] = agreen*adiff;
  tmp[2] = ablue*adiff;
  glMaterialfv(aface, GL_DIFFUSE, tmp);
 }

 if (aspec >= 0) {
  tmp[0] = ared*aspec;
  tmp[1] = agreen*aspec;
  tmp[2] = ablue*aspec;
  glMaterialfv(aface, GL_SPECULAR, tmp);
 }

 if (*emissive >= 0) {
  tmp[0] = ared*(*emissive);
  tmp[1] = agreen*(*emissive);
  tmp[2] = ablue*(*emissive);
  glMaterialfv(aface, GL_EMISSION, tmp);
 }

 if (*shiny >= 0) {
  glMaterialf(aface, GL_SHININESS, *shiny*128.);
 }

}

g0blendf_(int *source, int *dest) {
 GLenum funcs[] = {GL_ZERO, GL_ONE, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR,
  GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA,
  GL_ONE_MINUS_DST_ALPHA, GL_SRC_ALPHA_SATURATE,
  GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR};
  if (*source < 0 || *dest < 0) {
 	glDisable(GL_BLEND);
  } else {
#ifdef GLES
	glEnable(GL_SAMPLE_ALPHA_TO_MASK);
#endif
	glEnable(GL_BLEND);
	glBlendFunc(funcs[*source], funcs[*dest]);
  }
//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void g0drawtext_(float *x, float *y, float *z, char *string, int *ifont, int slen) {

  int i;
  int width;
  void *ffont[] = {
   GLUT_BITMAP_HELVETICA_18,
   GLUT_BITMAP_HELVETICA_12,
   GLUT_BITMAP_HELVETICA_10,
   GLUT_BITMAP_TIMES_ROMAN_24,
   GLUT_BITMAP_TIMES_ROMAN_10,
   GLUT_BITMAP_9_BY_15,
   GLUT_BITMAP_8_BY_13,
  };

  glDisable(GL_LIGHTING);
//#ifdef QT
//  qtext(*x,*y,*z,string,slen);
//#else
  //glPushMatrix();
  //glTranslatef(*x, *y, *z);
  int jfont = *ifont;
  if (jfont < 0  || jfont > 6) jfont = 0;
  glRasterPos3f(*x, *y, *z);
  for (i=0; i<slen; i++) {
    glutBitmapCharacter(ffont[jfont], string[i]);
  }
  //glPopMatrix();
//#endif
  glEnable(GL_LIGHTING);
}

#if defined __APPLE__
void g0shading_model_(int *type) {
#elif defined GFORTRAN
void g0shading_model_(int *type) {
#else
void g0shading_model__(int *type) {
#endif

 if (*type == 0) {
  glShadeModel(GL_FLAT);
 } else if (*type == 1) {
  glShadeModel(GL_SMOOTH);
 }
}

#ifdef __APPLE__
int g0read_z_buff_(int xlo, int ylo, int xhi, int yhi, int *zbuf) {
#else
int g0read_z_buff__(int xlo, int ylo, int xhi, int yhi, int *zbuf) {
#endif

 int width, height;
 width = xhi-xlo+1;
 height = yhi-ylo+1;
 glReadPixels(xlo, ylo, width, height, GL_DEPTH_COMPONENT, GL_INT, zbuf);
 return(width*height);
}

#if defined __APPLE__
int g0read_rgb_buff_(int *xlo, int *ylo, int *xhi, int *yhi, int *zbuf) {
#elif defined GFORTRAN
int g0read_rgb_buff_(int *xlo, int *ylo, int *xhi, int *yhi, int *zbuf) {
#else
int g0read_rgb_buff__(int *xlo, int *ylo, int *xhi, int *yhi, int *zbuf) {
#endif

 int width, height;
 width = *xhi-*xlo+1;
 height = *yhi-*ylo+1;
 glReadPixels(*xlo, *ylo, width, height, GL_RGB, GL_INT, zbuf);
 return(width*height);
}

#if defined __APPLE__
void g0set_matrix_mode_view_() {
#elif defined GFORTRAN
void g0set_matrix_mode_view_() {
#else
void g0set_matrix_mode_view__() {
#endif
 glMatrixMode(GL_MODELVIEW);
}

#if defined __APPLE__
void g0set_matrix_mode_projection_() {
#elif defined GFORTRAN
void g0set_matrix_mode_projection_() {
#else
void g0set_matrix_mode_projection__() {
#endif
 glMatrixMode(GL_PROJECTION);
}

/* wrappers for old SGI GL functions */

void makeob_(int *ob) {
 //fprintf(stderr, "NewList %d;", *ob);
 glNewList(*ob, GL_COMPILE);
}

void closeo_() {
 //fprintf(stderr, "EndList\n");
 glEndList();
}

void viewpo_(int *xlo, int *xhi, int *ylo, int *yhi) {
// fprintf(stderr, "viewport %d %d %d %d\n", *xlo, *ylo, *xhi, *yhi);
 int width  = *xhi - *xlo + 1;
 int height = *yhi - *ylo + 1;
 glViewport(*xlo, *ylo, width, height);
}

void pushma_() {
  glPushMatrix();
}

void popmat_() {
  glPopMatrix();
}

void popnam_() {
 //fprintf(stderr, "PopName\n");
 glPopName();
}
 
void pushnam_(GLuint *ob) {
 //fprintf(stderr, "PushName %d\n", *ob);
 glPushName(*ob);
}

void loadnam_(GLuint *ob) {
 fprintf(stderr, "LoadName %d\n", *ob);
 glLoadName(*ob);
}


#if defined __APPLE__
void identity_matrix_() {
#elif defined GFORTRAN
void identity_matrix_() {
#else
void identity_matrix__() {
#endif
 glLoadIdentity();
}

void callob_(int *ob) {
  glCallList(*ob);
}

void backfa_(int *flag) {

 if (*flag) {
  glDisable(GL_CULL_FACE);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
 } else {
  glEnable(GL_CULL_FACE);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
 }
}

void depthc_(int *flag) {

 if (*flag == 0) {
//  glEnable(GL_DEPTH_TEST);
 } else {
//  glDisable(GL_DEPTH_TEST);
 }
}

/*
void popnam_() {
 fprintf(stderr, "PopName\n");
 glPopName();
}

void pushna_(GLuint *ob) {
 fprintf(stderr, "PushName %d\n", *ob);
 glPushName(*ob);
}
*/

void fmsave_() {
}
void fmhard_() {
}
void fmrestore_() {
}

void c3i_(int *r, int *g, int *b) { glColor3i(*r, *g, *b); }
void c3f_(float *v) { glColor3fv(v); }
void c4f_(float *v) { glColor4fv(v); }
void n3f_(float *v) {
// fprintf (stderr, "n %f %f %f\n", v[0], v[1], v[2]);
 glNormal3fv(v);
}
void v3f_(float *v) {
// fprintf (stderr, "v %f %f %f\n", v[0], v[1], v[2]);
 glVertex3fv(v);
}

void bgnpoi_() {
 glDisable(GL_LIGHTING);
 glBegin(GL_POINTS);
}

void bgnclo_() {
 glDisable(GL_LIGHTING);
 glBegin(GL_LINE_LOOP); /* closed, continuous line */ 
}
void bgnlin_() {
 glDisable(GL_LIGHTING);
 glBegin(GL_LINE_STRIP); /* continuous line */
}
void bgnseg_() {
 glDisable(GL_LIGHTING);
 glBegin(GL_LINES); /* segments */ 
}

void bgnpol_() { glBegin(GL_POLYGON); }
void bgntri_() { glBegin(GL_TRIANGLES); }
void bgntst_() { glBegin(GL_TRIANGLE_STRIP); }
void bgnqst_() { glBegin(GL_QUAD_STRIP); }

void endpol_() { glEnd(); }
void endclo_() { glEnd(); }
void endtst_() { glEnd(); }
void endqst_() { glEnd(); }
void endtri_() { glEnd(); }
void endlin_() {
 glEnd();
 glEnable(GL_LIGHTING);
}
void endpoi_() {
 glEnable(GL_LIGHTING);
 glEnd();
}
void endany_() {
 glEnd();
}

void delobj_(int *ob) {
// fprintf(stderr, "DeleteList %d;", *ob);
 glDeleteLists(*ob,1);
}

void color_(int index) { glIndexi(index); }

void clear_() {
 glClear( GL_COLOR_BUFFER_BIT ); 
 //glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT ); 
}
void czclea_(float *r, float *g, float *b, float *alpha, float *z) {
 glClearColor(*r,*g,*b,*alpha);
 glClearDepth(*z);
 glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT ); 
}
void hidesurf_(int *flag) {
 if (*flag) {
  glEnable(GL_DEPTH_TEST);
 } else {
  glDisable(GL_DEPTH_TEST);
 }
}

void swapbu_() {
#ifndef QT
 glutSwapBuffers();
#endif
}
void gflush_() { glFlush(); }

void setpat_(int pattern) {
 GLubyte stipple[] = {0x00, 0x00};
 glPolygonStipple(stipple);
}

void loadma_(float *matrix) {
 glLoadMatrixf(matrix);
}
void getmat_(float *matrix) {
}
void multma_(float *matrix) {
 glMultMatrixf(matrix);
}
void scale_(float *x, float *y, float *z) {
 glScalef(*x,*y,*z);
}
void xrot_(float *angle) { glRotatef(*angle, 1., 0., 0.); }
void yrot_(float *angle) { glRotatef(*angle, 0., 1., 0.); }
void zrot_(float *angle) { glRotatef(*angle, 0., 0., 1.); }
void transl_(float *x, float *y, float *z) {
 glTranslatef(*x,*y,*z);
}
void perspe_(float *fovy, float *aspect, float *hither, float *yon, float *eye, float *shear, float *parallax) {
 GLfloat Fov    = *fovy;
 GLfloat Aspect = *aspect;
 GLfloat Near   = *hither;
 GLfloat Far    = *yon;
 GLfloat Zeye   = *eye;
 GLfloat Xeye   = *shear;
 GLfloat LookY  = *parallax;

/*
 glMatrixMode( GL_PROJECTION );
 glLoadIdentity();
*/
 gluPerspective(Fov, Aspect, Near, Far);
 glMatrixMode( GL_MODELVIEW );
 glLoadIdentity();
 gluLookAt(Xeye, 0., Zeye,   0., LookY, 0.,  0., 1., 0.);

}
void ortho_(float *left, float *right, float *bottom, float *top,
  float *near, float *far) {
 glMatrixMode( GL_PROJECTION );
 glLoadIdentity();
 glOrtho(*left, *right, *bottom, *top, *near, *far);
 glMatrixMode( GL_MODELVIEW );
 glLoadIdentity();
}

 
int processHits(GLint hits, int mx, int my, float *xyzw, GLuint *names) {
	int ihit = 0;
	int zhit = 0;
 	int i,j,k,nn;
	unsigned int zmin, zmax, minZ;
	float zcoord;
 
 	/*
 	For each hit in the buffer are allocated 4 bytes:
 	1. Number of hits selected (always one,
 		beacuse when we draw each object
 		we used glLoadName, so we replaced the
 		prevous name in the stack)
 	2. Min Z
 	3. Max Z
 	4. Name of the hit (glLoadName)
 	*/
 
 	//fprintf(stderr, "%d hits\n", hits);
 
 	for (i = 0,j = 0; i < hits; ++i) {
		nn =   (GLuint)names[j++];
 		zmin = (GLuint)names[j++];
		zmax = (GLuint)names[j++];
 		//fprintf(stderr,"Number %d with %d\n"
 		//"Min Z: %u\n" "Max Z: %u\n" "Names on stack:",
 		//i+1,nn, zmin, zmax);
		for (k=0; k<nn; ++k) {
			ihit = (int)names[j];
	/* prefer closest hit, i.e. minimum z value */
			if (zhit == 0 || zmin < minZ) {
			  minZ = zmin;
			  zhit = ihit;
			}
			++j;
			//fprintf(stderr,"%d ", ihit);
		}
 		//fprintf(stderr,"\n");
	}
 
 	//fprintf(stderr,"\n");

  zcoord = minZ;
  g0pick_(&zhit, &mx, &my, xyzw);
  return zhit;
}
void getperspmat_(GLfloat *pmat) {
  glGetFloatv(GL_PROJECTION_MATRIX, pmat);
}
void gettfmmat_(GLfloat *mmat) {
  glGetFloatv(GL_MODELVIEW_MATRIX, mmat);
}

#define BUFSIZE 512
GLuint selectBuf[BUFSIZE];
int g0pickprocess(int mx, int my, float *xyzw) {

  int hitset;
  int hits;
  int ihit;
  int forpick;

  glSelectBuffer(BUFSIZE,selectBuf);
  glRenderMode(GL_SELECT);
  glInitNames();
  glPushName(0);
  glClearDepth(1);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT ); 
  forpick = 1;
  display_(&forpick);
  //glutSwapBuffers();

  // returning to normal rendering mode
  hits = glRenderMode(GL_RENDER);
  if (hits > 0) {
   ihit = processHits(hits,mx,my,xyzw,selectBuf);
   return ihit;
  }
  return 0;
}

void g0worldcoords_(int *mx, int *my, float *tfmmat, float *xyzw) {

 GLdouble pmat[16];
 GLdouble mmat[16];
 GLint viewport[4];
 GLdouble x,y,z;
 GLdouble winx,winy,winz;
 GLfloat wz[PICKWIDTH*PICKWIDTH];
 int  i;
 glGetDoublev(GL_PROJECTION_MATRIX, pmat);

// glGetDoublev(GL_MODELVIEW_MATRIX, mmat);
 for (i=0; i<16; ++i) {
  mmat[i] = tfmmat[i];
 }

 glGetIntegerv(GL_VIEWPORT, viewport);
// glReadPixels (*mx, *my, 1, 1, GL_DEPTH_COMPONENT, GL_DOUBLE, &winz);
// is there a bug in glReadPixels with GL_DOUBLE?
 glReadPixels (*mx - PICKWIDTH/2, *my - PICKWIDTH/2, PICKWIDTH, PICKWIDTH, GL_DEPTH_COMPONENT, GL_FLOAT, wz);
 winx = *mx; winy = *my;
// find closest z in pick box; analogous to processHits
 winz = wz[0];
 for (i=0; i<PICKWIDTH*PICKWIDTH; ++i) {
  if (wz[i] < winz) winz = wz[i];
 }
 gluUnProject(winx,winy,winz,mmat,pmat,viewport,&x,&y,&z);
 xyzw[0] = x;
 xyzw[1] = y;
 xyzw[2] = z;
 xyzw[3] = 1.0;
}
