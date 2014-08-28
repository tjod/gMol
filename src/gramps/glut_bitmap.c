/* Copyright (c) Mark J. Kilgard, 1994. */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

//#include "glutint.h"
/* Non-Win32 platforms need APIENTRY defined to nothing
   because all the GLUT routines have the APIENTRY prefix
   to make Win32 happy. */
#if defined(_WIN32)
#define APIENTRY __stdcall
#else
#define APIENTRY
#endif

#include "glutbitmap.h"

#if defined(_WIN32)
/* To get around the fact that Microsoft DLLs only allow functions
   to be exported and now data addresses (as Unix DSOs support), the
   GLUT API constants such as GLUT_STROKE_ROMAN have to get passed
   through a case statement to get mapped to the actual data structure
   address. */
void*
__glutFont(void *font)
{
  switch((int)font) {
/*
  case (int)GLUT_STROKE_ROMAN:
    return &glutStrokeRoman;
  case (int)GLUT_STROKE_MONO_ROMAN:
    return &glutStrokeMonoRoman;
*/
  case (int)GLUT_BITMAP_9_BY_15:
    return &glutBitmap9By15;
  case (int)GLUT_BITMAP_8_BY_13:
    return &glutBitmap8By13;
  case (int)GLUT_BITMAP_TIMES_ROMAN_10:
    return &glutBitmapTimesRoman10;
  case (int)GLUT_BITMAP_TIMES_ROMAN_24:
    return &glutBitmapTimesRoman24;
  case (int)GLUT_BITMAP_HELVETICA_10:
    return &glutBitmapHelvetica10;
  case (int)GLUT_BITMAP_HELVETICA_12:
    return &glutBitmapHelvetica12;
  case (int)GLUT_BITMAP_HELVETICA_18:
    return &glutBitmapHelvetica18;
  }
//  __glutFatalError("out of memory.");
  /* NOTREACHED */
}
#endif

//void APIENTRY 
void 
glutBitmapCharacter(GLUTbitmapFont font, int c)
{
  const BitmapCharRec *ch;
  BitmapFontPtr fontinfo;
  GLint swapbytes, lsbfirst, rowlength;
  GLint skiprows, skippixels, alignment;

#if defined(_WIN32)
  fontinfo = (BitmapFontPtr) __glutFont(font);
#else
  fontinfo = (BitmapFontPtr) font;
#endif

  if (c < fontinfo->first ||
    c >= fontinfo->first + fontinfo->num_chars)
    return;
  ch = fontinfo->ch[c - fontinfo->first];
  if (ch) {
    /* Save current modes. */
    glGetIntegerv(GL_UNPACK_SWAP_BYTES, &swapbytes);
    glGetIntegerv(GL_UNPACK_LSB_FIRST, &lsbfirst);
    glGetIntegerv(GL_UNPACK_ROW_LENGTH, &rowlength);
    glGetIntegerv(GL_UNPACK_SKIP_ROWS, &skiprows);
    glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &skippixels);
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
    /* Little endian machines (DEC Alpha for example) could
       benefit from setting GL_UNPACK_LSB_FIRST to GL_TRUE
       instead of GL_FALSE, but this would require changing the
       generated bitmaps too. */
    glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
    glPixelStorei(GL_UNPACK_LSB_FIRST, GL_FALSE);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBitmap(ch->width, ch->height, ch->xorig, ch->yorig,
      ch->advance, 0, ch->bitmap);
    /* Restore saved modes. */
    glPixelStorei(GL_UNPACK_SWAP_BYTES, swapbytes);
    glPixelStorei(GL_UNPACK_LSB_FIRST, lsbfirst);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, rowlength);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, skiprows);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, skippixels);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
  }
}
