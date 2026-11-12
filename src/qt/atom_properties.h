/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2013 by TJ O'Donnell and
C  Copyright (C) 2005-2013 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
class Atom {
	Atom();
	~Atom();
public:
  // from http://periodictable.com/
    static const float radius[119];
    static const float color[119][3];
};
#define NotR 1.5

// Hue (0-360), Saturation (0-1), Value (0-1)
#define GREEN  {0,1,1}
#define BLUE   {120.,1,1}
#define RED    {240.,1,1}
#define YELLOW {300.,1,1}
#define CYAN   {60.,1,1}
#define PURPLE {180.,1,1}
#define ORANGE {270.,1,1}
#define WHITE  {0.,0,1}
#define GREY   {0.,0,0.7}
#define CARBON {0.,0,0.5}
#define PINK   {210.,0.7,1}
#define BROWN   {260.,1.0,0.9}
#define NOCOLOR {0.,0.25,1} // light green
#define NotC   NOCOLOR
