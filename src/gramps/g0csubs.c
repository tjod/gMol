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
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "gramps.h"
#include <FTGL/ftgl.h>
FTGLfont *font3D = NULL;
FTGLfont *fontBitmap = NULL;
FTGLfont *font2D = NULL;

#include "ffmpeg_wrapper.h"
FILE *ffmpeg; // popen-ed by savewin; must be closed adter SNAP/NOADVANCE

/* G0 routines that used to be in g0subs.f */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool endsWith(const char *str, const char *suffix) {
    if (!str || !suffix) {
        return false;
    }

    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);

    // If the suffix is longer than the string, it cannot match
    if (suffix_len > str_len) {
        return false;
    }

    // Compare the end of 'str' with 'suffix'
    return strcmp(str + (str_len - suffix_len), suffix) == 0;
}

//#define BAYER_TRANSPARENCY
#define VOID_AND_CLUSTER
#if defined BAYER_TRANSPARENCY

static const uint8_t BAYER_8X8[8][8] = {
    {  0, 32,  8, 40,  2, 34, 10, 42 },
    { 48, 16, 56, 24, 50, 18, 58, 26 },
    { 12, 44,  4, 36, 14, 46,  6, 38 },
    { 60, 28, 52, 20, 62, 30, 54, 22 },
    {  3, 35, 11, 43,  1, 33,  9, 41 },
    { 51, 19, 59, 27, 49, 17, 57, 25 },
    { 15, 47,  7, 39, 13, 45,  5, 37 },
    { 63, 31, 55, 23, 61, 29, 53, 21 }
};

int trpat_(float *alpha) {
    // Map alpha [0.0, 1.0] to 65 discrete states (0 to 64 active pixels per 8x8 tile)

    int level = round(*alpha * 64.0f);
    if (level < 1) {
        glDisable(GL_POLYGON_STIPPLE);
        return 0;
    }

    uint8_t mask1024[128];
    int nbits = 1024 * (*alpha);

    for (int y = 0; y < 8; ++y) {
        // Build one 8-bit row from the Bayer matrix (MSB first)
        uint8_t rowByte = 0;
        for (int x = 0; x < 8; ++x) {
            if (BAYER_8X8[y][x] < level) {
                rowByte |= (1 << (7 - x));
            }
        }

        // Tile 4 times horizontally (4 bytes = 32 bits) and 4 times vertically (32 rows)
        for (int tileY = 0; tileY < 4; ++tileY) {
            int rowOffset = (tileY * 8 + y) * 4;
            mask1024[rowOffset + 0] = rowByte;
            mask1024[rowOffset + 1] = rowByte;
            mask1024[rowOffset + 2] = rowByte;
            mask1024[rowOffset + 3] = rowByte;
        }
    }

    glEnable(GL_POLYGON_STIPPLE);
    glPolygonStipple(mask1024);
    return nbits;
}
#elif defined VOID_AND_CLUSTER
#include "vac_stipple.h"
int trpat_(float *alpha) {
    int nbits = 0;

    if (*alpha <= 0.0f) {
        glDisable(GL_POLYGON_STIPPLE);
        return nbits;
    }

    float transparency = 1.0 - *alpha;
    GLubyte mask[VAC_STIPPLE_BYTES];
    vac_build_stipple_mask(transparency, mask);
    glEnable(GL_POLYGON_STIPPLE);
    glPolygonStipple(mask);
    return nbits;
}

#else

#define MAXTPAT 10
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
 pattern = *transparency * (MAXTPAT + 0.5);
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
#endif

void rayshow_(name, options, namelen, optlen)
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

void psplot_(name, color, namelen)
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

int savewin_(char *snapfile, int *snaplen, char *snaptype, int *fcount, size_t slen)
{
// snaptype if fortran character*4, so padded with blanks to length; see strcmp below
  char filename[256];
  if (*fcount < 0) {
    int flen = *snaplen + 1;
    snprintf(filename, flen, "%s", snapfile);
  } else {
    // snapfile has %08d in it, but needs to be null-terminated
    char snapformat[256];
    int flen = *snaplen + 1; // to add the null
    strncpy(snapformat, snapfile, flen);
    snapformat[flen] = '\0';
    // now replace the %08d with fcount
    sprintf(filename, snapformat, *fcount);
  }

  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  int width  = viewport[2];
  int height = viewport[3];

#if defined openjpeg
#include "openjpeg_wrapper.h"
#elif defined STB_WRITE
#include "stb_wrapper.h"
#endif

  if (0 == strcmp(snaptype, "jpg ") || 0 == strcmp(snaptype, "jpeg")) {
#if defined openjpeg
#include "openjpeg_wrapper.h"
     save_framebuffer_to_jp2(filename, width, height);
#elif defined STB_WRITE
    int quality = 90;
    saveScreenshotJPEG(filename, width, height, quality);
  } else if (0 == strcmp(snaptype, "png ")) {
    saveScreenshotPNG(filename, width, height);
#endif

  } else if (0 == strcmp(snaptype, "mpeg") || 0 == strcmp(snaptype, "mp4 ")) {
     if (ffmpeg == NULL) {
      ffmpeg = open_framebuffer_to_ffmpeg(filename, width, height, 60);
     }
     // ffmpeg is left popen-ed for further calls (here) to save_framebuffer_to_ffmpeg
     // SNAP/NOADVANCE call close_savewin which pcloses ffmpeg
     if (ffmpeg) {
       return save_framebuffer_to_ffmpeg(ffmpeg, width, height);
     }
  }
// unsigned char *rgbbuf;
// FILE *fp;

// glPixelStorei(GL_PACK_ALIGNMENT, 1);
// rgbbuf = (unsigned char *)calloc(3*width*height, sizeof(unsigned char));
// if (NULL == rgbbuf) {
//  fprintf(stderr, "can't allocate rgb buffer\n");
//  return;
// }
// glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, rgbbuf);
// snprintf(filename, *namlen, "%s", name);
// return (NULL);
  return (0);
}

int close_ffmpeg_() {
  // called by snap.F when SNAP/NOADVANCE
  if (ffmpeg) {
    pclose(ffmpeg);
    ffmpeg = NULL;
    return 1;
  } else {
    return 0;
  }
}

void g0texture_(int *iobj) {
    return glCallList(*iobj);
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
void g0texcoord1f_(float *c) {
    return glTexCoord1f(*c);
}

//void g0colormaterial_(int *flag) {
//static int state=-1;
// printf("g0colormaterial flag %d; state %d\n", *flag, state);
// if (*flag == state) return;
// if (*flag == 1) {
//  glEnable(GL_COLOR_MATERIAL);
//  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
//  state = 1;
// } else {
//  glDisable(GL_COLOR_MATERIAL);
//  state = 0;
// }
//}

void g0colormaterial_(int *flag) {
// called when an object is read in (pix file) and contains colors
  if (*flag == 1) {
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
  } else {
    glDisable(GL_COLOR_MATERIAL);
  }
}
void g0light_init_ () {

 /* setup lighting, etc */

 int zero=0;
 glEnable(GL_LIGHTING);
 //glEnable(GL_COLOR_MATERIAL);
 //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
 //g0colormaterial_(&zero);
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

void setsmooth_(int *flag) {
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

void g0normalization_(int *type) {
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

void g0material_ (float *red, float *green, float *blue, float *alpha,
 float *ambient, float *diffuse, float *specular, float *emissive,
 float *shiny, int *backface, float bfcolor[3])  {

 GLfloat tmp[4];
 tmp[3] = 1.0 - *alpha;
 GLfloat ared, agreen, ablue;
 GLfloat aambi, adiff, aspec;
 int aface;

  //printf ("g0material %f %f %f %f %d\n", *red, *green, *blue, *alpha, *backface);
 if (*backface == 1) {
  // bfcolor (g0bfrgb) stores rgb color
  aface = GL_BACK;
  ared = bfcolor[0];
  agreen = bfcolor[1];
  ablue = bfcolor[2];
  // judgement call that colored backface should not be specular
  aambi = 0.5; // *ambient;
  adiff = 0.5; // *diffuse;
  aspec = 0.0; // *specular;
 } else if (*backface == -1) {
// bfcolor (g0bfrgb) stores ambient, diffuse, specular intensities, not rgb color
// can make backface specular, but with same color as frontface
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
 }

 // should affect backfaces ,too, but no :(
 tmp[0] = ared;
 tmp[1] = agreen;
 tmp[2] = ablue;
 glColor4fv(tmp);

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

void g0blendf_(int *source, int *dest) {
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

int initialize_fonts_(char *font_path, int flen) {
  int err1 = 0;
  int err2 = 0;
  int err3 = 0;
  if (font3D)    ftglDestroyFont(font3D);
  if (font2D)    ftglDestroyFont(font2D);
  if (fontBitmap) ftglDestroyFont(fontBitmap);
  font3D     = ftglCreateExtrudeFont(font_path);
  fontBitmap = ftglCreateBitmapFont (font_path);
  font2D     = ftglCreatePolygonFont(font_path);
  if (font3D && fontBitmap && font2D) {
    err3 = ftglGetFontError(font3D);
    err2 = ftglGetFontError(font2D);
    err1 = ftglGetFontError(fontBitmap);
    if ((err1 + err2 + err3) == 0) {
        // i'm using my own display lists, thank you
        ftglSetFontDisplayList(font3D, 0);
        ftglSetFontDisplayList(fontBitmap, 0);
        ftglSetFontDisplayList(font2D, 0);
        return 1;
    }
  }
//  fprintf(stderr, "3D font error: %d\n", err3);
//  fprintf(stderr, "2D font error: %d\n", err2);
//  fprintf(stderr, "Bitmap font error: %d\n",  err1);
  return 0;
}

void draw_3D_text(float *x, float *y, float *z, char *string, int *size, int slen) {
  if (font3D) {
    int res = 72;
    ftglSetFontFaceSize(font3D, *size, res); // Base font size
    ftglSetFontDepth(font3D, 20.0);         // Depth of the 3D extrusion; let user scale later
    glTranslatef(*x, *y, *z);
    //ftglSetFontOutset(font, 0.0f, 1.0f);  // Front/back outset smoothing
    glScalef(0.01f, 0.01f, 0.01f); // this is a reasonable guess to fit screen; user can scale later
    string[slen] = '\0';
    ftglRenderFont(font3D, string, FTGL_RENDER_ALL);
  }
}

void draw_2D_text(float *x, float *y, float *z, char *string, int *size, int slen) {
  if (font2D) {
    int res = 72;
    ftglSetFontFaceSize(font2D, *size, res); // Base font size
    glDisable(GL_LIGHTING);
    glTranslatef(*x, *y, *z);
    glScalef(0.01f, 0.01f, 0.01f); // this is a reasonable guess to fit screen; user can scale later
    string[slen] = '\0';
    ftglRenderFont(font2D, string, FTGL_RENDER_ALL);
    glEnable(GL_LIGHTING);
  }
}


void draw_bitmap_text(float *x, float *y, float *z, char *string, int *size, int slen) {
  if (fontBitmap) {
      int res = 72;
      ftglSetFontFaceSize(fontBitmap, *size, res);
      glDisable(GL_LIGHTING);
      glRasterPos3f(*x, *y, *z);
      string[slen] = '\0';
      ftglRenderFont(fontBitmap, string, FTGL_RENDER_ALL);
      glEnable(GL_LIGHTING);
  }
}


void g0drawtext_(float *x, float *y, float *z, int *size, char *ftype, char *string, int flen, int slen) {
  if ( strncmp(ftype, "3D", 2)  == 0) {
    draw_3D_text(x, y, z, string, size, slen);
  } else if ( strncmp(ftype, "2D", 2)  == 0) {
    draw_2D_text(x, y, z, string, size, slen);
  } else {
   draw_bitmap_text(x, y, z, string, size, slen);
  }
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

 //fprintf(stderr, "viewpo %d %d %d %d\n", *xlo, *ylo, *xhi, *yhi);

#ifdef QT
 GLint viewport[4];
 glGetIntegerv(GL_VIEWPORT, viewport);
 //fprintf(stderr, "GL_VIEWPORT %d,%d,%d,%d\n", viewport[0], viewport[1], viewport[2], viewport[3]);
 int width  = viewport[2] - viewport[0];
 int height = viewport[3] - viewport[1];
#else
 int width  = *xhi - *xlo + 1;
 int height = *yhi - *ylo + 1;
#endif
 glViewport(*xlo, *ylo, width, height);
 //fprintf(stderr, "viewpo %dx%d\n", width, height);
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


void identity_matrix_() {
 glLoadIdentity();
}

void callob_(int *ob) {
// printf ("callob shows GL_COLOR_MATERIAL %d; GL_LIGHTING %d\n",
//        glIsEnabled(GL_COLOR_MATERIAL), glIsEnabled(GL_LIGHTING));
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

void c3i_(int *r, int *g, int *b) {
// fprintf (stderr, "c %d %d %d\n",*r, *g, *b);
 glColor3i(*r, *g, *b);
}
void c3f_(float *v) {
// fprintf (stderr, "c %f %f %f\n", v[0], v[1], v[2]);
 glColor3fv(v);
}
void c4f_(float *v) {
// fprintf (stderr, "c4 %f %f %f %f\n", v[0], v[1], v[2], v[3]);
 glColor4fv(v);
}
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
 //glDisable(GL_LIGHTING);
 glBegin(GL_LINE_LOOP); /* closed, continuous line */ 
}
void bgnlin_() {
 glDisable(GL_LIGHTING);
 glBegin(GL_LINE_STRIP); /* continuous line */
}
void bgnseg_() {
 //glDisable(GL_LIGHTING);
 glBegin(GL_LINES); /* segments */ 
}

void bgnpol_() {
 //glDisable(GL_LIGHTING);
 glBegin(GL_POLYGON);
}
void bgntri_() {
 glBegin(GL_TRIANGLES);
}
void bgntst_() {
 glBegin(GL_TRIANGLE_STRIP);
}
void bgnqst_() {
 glBegin(GL_QUAD_STRIP);
}

void endpol_() {
 glEnd();
 //glEnable(GL_LIGHTING);
}
void endclo_() {
 glEnd();
 //glEnable(GL_LIGHTING);
}
void endtst_() {
 glEnd();
 //glEnable(GL_LIGHTING);
}
void endqst_() {
 glEnd();
 //glEnable(GL_LIGHTING);
}
void endtri_() {
 glEnd();
 //glEnable(GL_LIGHTING);
}
void endlin_() {
 glEnd();
 glEnable(GL_LIGHTING);
}
void endpoi_() {
 glEnd();
 glEnable(GL_LIGHTING);
}
void endany_() {
 glEnd();
 //glEnable(GL_LIGHTING);
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
//void gflush_() { glFlush(); }

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

 
int processHits(GLint hits, int mx, int my, float (*xyzw)[4], GLuint *names) {
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
int g0pickprocess(int mx, int my, float (*xyzw)[4]) {

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
 int  i, j;
 glGetDoublev(GL_PROJECTION_MATRIX, pmat);

// for (i=0; i<16; ++i) {
//   if (!(i%4)) printf ("\n");
//   printf("%f ", pmat[i]);
// }
// printf("\n");

// glGetDoublev(GL_MODELVIEW_MATRIX, mmat);
 for (i=0; i<16; ++i) {
  mmat[i] = tfmmat[i];
//   if (!(i%4)) printf ("\n");
//   printf("%f ", tfmmat[i]);
 }
// printf("\n");

 glGetIntegerv(GL_VIEWPORT, viewport);
// for (i=0; i<4; ++i) {
//  printf("%d ", viewport[i]);
// }
// printf("\n");

// glReadPixels (*mx, *my, 1, 1, GL_DEPTH_COMPONENT, GL_DOUBLE, &winz);
// is there a bug in glReadPixels with GL_DOUBLE?
 glReadPixels (*mx - PICKWIDTH/2, *my - PICKWIDTH/2, PICKWIDTH, PICKWIDTH, GL_DEPTH_COMPONENT, GL_FLOAT, wz);
 winx = *mx; winy = *my;
// printf("%f %f", winx, winy);
// find closest z in pick box; analogous to processHits
 winz = wz[0];
 for (i=0; i<PICKWIDTH*PICKWIDTH; ++i) {
//  printf(" %f", wz[i]);
  if (wz[i] < winz) winz = wz[i];
 }
// printf ("\n %f %f %f\n", winx, winy, winz);
 gluUnProject(winx,winy,winz,mmat,pmat,viewport,&x,&y,&z);
 xyzw[0] = x;
 xyzw[1] = y;
 xyzw[2] = z;
 xyzw[3] = 1.0;
// printf ("%f %f %f\n",  x,y,z);
}
