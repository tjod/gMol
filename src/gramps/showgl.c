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
#else
#include <GL/gl.h>
#endif
#include <string.h>
void showgl_ () {

 char tmpline[120];
 const GLubyte *glout;
 int i,n;
 char *blank, *begin;

 glout = glGetString(GL_VERSION);
 strcpy(tmpline, "OpenGL: v");
 i=strlen(tmpline);
 while ( *glout && i<sizeof(tmpline)) { tmpline[i++] = *glout++; }
 tmpline[i] = '\0';
 grout_((char *)tmpline, strlen(tmpline));

 glout = glGetString(GL_VENDOR);
 strcpy(tmpline, "Vendor: ");
 i=strlen(tmpline);
 while ( *glout && i<sizeof(tmpline)) { tmpline[i++] = *glout++; }
 tmpline[i] = '\0';
 grout_(tmpline, strlen(tmpline));

 glout = glGetString(GL_RENDERER);
 strcpy(tmpline, "Renderer: ");
 i=strlen(tmpline);
 while ( *glout && i<sizeof(tmpline)) { tmpline[i++] = *glout++; }
 tmpline[i] = '\0';
 grout_(tmpline, strlen(tmpline));

 glout = glGetString(GL_EXTENSIONS);
 strcpy(tmpline, "Extensions:");
 grout_(tmpline, strlen(tmpline));
 begin = (char *)glout;
 strcpy(tmpline, " ");
 while (blank = strstr(begin," ")) {
  n = blank - begin + 1;
  strncpy(tmpline+1, begin, n);
  grout_(tmpline, n);
  begin = blank+1;
 }

#ifdef linux
 //glutReportErrors();
#endif
}
