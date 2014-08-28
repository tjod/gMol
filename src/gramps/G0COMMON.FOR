C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
C       COMMON AREA FOR G0 VARIABLES, USED BY G0 ROUTINES
C
      INTEGER MAXCOL, MAXDCUE, SAVCOL, SMOOR
C
C       BUFFER FOR MOVES AND DRAWS
C
      INTEGER G0PTYP, G0DOTS, G0LINES, G0CYLS, G0SPTS
      PARAMETER (G0DOTS=1)
      PARAMETER (G0LINES=2)
      PARAMETER (G0CYLS=3)
      PARAMETER (G0SPTS=4)
C
C	For G0BGN
	Integer G0Polygon, G0QuadStrip, G0TriStrip, G0Triangle
	Parameter (G0Polygon=1)
	Parameter (G0QuadStrip=2)
	Parameter (G0Triangle=3)
	Parameter (G0TriStrip=4)
	Integer G0ShowPoints, G0ShowFilled, G0ShowLines, G0ShowNormals
	Parameter (G0ShowPoints=0)
	Parameter (G0ShowLines=1)
	Parameter (G0ShowFilled=2)
	Parameter (G0ShowNormals=4)
	
C
      INTEGER G0PMAX
      PARAMETER (G0PMAX=500000)
C	Each should amount to 6*G0PMAX storage locations
      REAL  G0PBUF  (3,2,G0PMAX)
      REAL  G0NBUF  (6,  G0PMAX)
      REAL  G0LNCBUF(9,  G0PMAX*2/3)
      REAL  G0LCBUF (6,  G0PMAX)
      REAL  G0DBUF  (3,  G0PMAX*2)
      EQUIVALENCE (G0PBUF, G0DBUF),  (G0PBUF, G0NBUF)
      EQUIVALENCE (G0PBUF, G0LCBUF), (G0PBUF, G0LNCBUF)
C
C	Maximum number of transparency patterns/masks
C	Must agree with MAXTPAT in g0csubs.c
      INTEGER MAXTPAT
      PARAMETER (MAXTPAT=8)
C
      INTEGER G0TMAX
      PARAMETER (G0TMAX=G0PMAX*3)
      INTEGER G0TBUF  (3, G0TMAX)
C
      CHARACTER*100 G0FNTNAM
      COMMON /G0CMISC/G0FNTNAM
C
      REAL    G0FRONT, G0BACK, G0HI, G0YO, G0EY
      REAL G0AMBI, G0DIFF, G0SPEC, G0EMIS, G0SHINY
      REAL G0RED, G0GREEN, G0BLUE, G0ALPHA, G0POWER
      REAL G0HUE, G0SAT, G0VALU
      Real G0RAYRAD
      REAL G0BKRGB(3)
      REAL G0BFRGB(3)
      INTEGER G0PCNT, G0RSTYLE, G0FONT, G0BPLA
      INTEGER G0BKCOL,G0XTSCALE,G0YTSCALE,G0TSIZE,G0PATT
      INTEGER G0COLR,G0XWIND,G0YWIND,G0ID, G0XORIG, G0YORIG
      INTEGER G0FR, G0VW, G0ST, G0DPASS, G0SRCE, G0DEST, G0APLANES
      INTEGER G0SPH0, G0SPH1, G0SPH2, G0CYL0, G0CYL1, G0CYL2
      Integer G0RES
	Integer G0CYLN1, G0CYLR1, G0CYLN2, G0CYLR2
	Integer G0SPHN1, G0SPHR1, G0SPHN2, G0SPHR2
	Integer G0TEX0
      LOGICAL G0BLNK,G00INIT,G0OPEN, G0RGB
      COMMON /G0AMISC/ MAXCOL, MAXDCUE, SAVCOL, SMOOR,
     &      G0BKCOL,G0XTSCALE,G0YTSCALE,G0TSIZE,
     &      G0HUE,G0SAT,G0VALU,G0BKRGB,G0COLR,G0PATT,G0XWIND,G0YWIND,
     &      G0XORIG, G0YORIG, G0BLNK,G0ID,G0FRONT, G0BACK, G0BPLA,
     &      G0RGB, G00INIT,G0OPEN, G0FONT, G0SRCE, G0DEST,
     &      G0FR,G0VW,G0ST,G0DPASS,G0PCNT,G0PTYP, G0RSTYLE,
     &      G0AMBI, G0DIFF, G0SPEC, G0EMIS, G0SHINY,
     &      G0SPH0, G0SPH1, G0SPH2, G0CYL0, G0CYL1, G0CYL2,
     &      G0HI, G0YO, G0EY, G0RED, G0GREEN, G0BLUE, G0ALPHA,
     &	    G0POWER, G0RAYRAD, G0APLANES, G0RES,
     &      G0CYLN1, G0CYLR1, G0CYLN2, G0CYLR2,
     &      G0SPHN1, G0SPHR1, G0SPHN2, G0SPHR2,
     &      G0TEX0, G0BFRGB
      COMMON /G0BUFFS/G0PBUF, G0TBUF
      SAVE   /G0AMISC/
      SAVE   /G0BUFFS/
c      SAVE   /G0CMISC/
C++++++++++++++++++++++++++++++++++++++++++++++
	INTEGER g0read_z_buff
	INTEGER g0read_rgb_buff
	INTEGER g0getpicknames
	EXTERNAL g0read_z_buff
	EXTERNAL g0read_rgb_buff
	EXTERNAL g0getpicknames
