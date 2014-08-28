C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      INTEGER STACKSIZE
      PARAMETER (STACKSIZE=15)
      INTEGER CURRENT,BASEX,BASEY,BASEZ,BASEW
      REAL TFMMTX(4,4,STACKSIZE)
      REAL TMPMAT(4,4)
      COMMON /PUSHPOP/CURRENT,BASEX,BASEY,BASEZ,BASEW
      COMMON /STACK/TFMMTX, TMPMAT
      SAVE   /STACK/
      SAVE   /PUSHPOP/
