C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      CHARACTER*40 ARGLIST(ARGMAX,AGFMAX)
      COMMON /ARGMS/ARGLIST
      INTEGER NB
      PARAMETER (NB=ARGMAX*AGFMAX)
      SAVE /ARGMS/
