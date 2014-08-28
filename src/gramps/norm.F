C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE NORM(EMAX)
C
#include "TFMMAT.FOR"
      REAL SMAX,EMAX
C
C       THIS ROUTINE NORMALIZES THE CURRENT TFM MATRIX
C       SO THAT THE MAXIMUM ELEMENT HAS A VALUE EMAX.
C
      SMAX=ABS(TFMMTX(1,1,CURRENT))
      DO 10 I=1,4
      DO 10 J=1,4
      SMAX=MAX( SMAX,ABS(TFMMTX(I,J,CURRENT)) )
 10   CONTINUE
C
      DO 20 I=1,4
      DO 20 J=1,4
      TFMMTX(I,J,CURRENT)=TFMMTX(I,J,CURRENT)/SMAX*EMAX
 20   CONTINUE
C
      RETURN
      END
