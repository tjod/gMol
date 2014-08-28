/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2013 by TJ O'Donnell and
C  Copyright (C) 2005-2013 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
namespace Atom {
  #define NotR 1.5
  // from http://periodictable.com/
    float radius[119] = {NotR,
    1.20,1.40,
    1.82,NotR,NotR,1.70,1.55,1.52,1.47,1.54,
    2.27,1.73,NotR,2.10,1.80,1.80,1.75,1.88,
    2.75,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,1.63,1.40,1.39,1.87,NotR,1.85,1.90,1.85,2.02,
    NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,1.63,1.72,1.58,1.93,2.17,NotR,2.06,1.98,2.16,
    NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,1.75,1.66,1.55,1.96,2.02,NotR,NotR,NotR,NotR,
    NotR,NotR,NotR,NotR,NotR,1.86,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR,NotR
  };


#define GREEN  {0,1,1}
#define BLUE   {120./360,1,1}
#define RED    {240./360,1,1}
#define YELLOW {300./360,1,1}
#define CYAN   {60./360,1,1}
#define PURPLE {180./360,1,1}
#define ORANGE {270./360,1,1}
#define WHITE  {0,0,1}
#define GREY   {0,0,0.7}
#define PINK   {210./360,0.7,1}
#define BROWN   {260./360,1.0,0.9}
#define UNKNOWN {0,0.25,1} // light green
#define NotC   UNKNOWN
    float color[119][3] = {NotC,
    WHITE,NotC,
    NotC,NotC,NotC,GREY,BLUE,RED,CYAN,NotC,
    NotC,NotC,NotC,NotC,ORANGE,YELLOW,GREEN,NotC,
    NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,BROWN,NotC,
    NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,PURPLE,NotC,
    NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,
    NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC,NotC
    };
}
