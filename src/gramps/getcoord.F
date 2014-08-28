C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE GETCOORD(ARG,SPOINT,NCOORD)
C
      IMPLICIT NONE
C
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
      CHARACTER ARG*(*)
      CHARACTER*5 CNUM
	INTEGER SPOINT,VPOINT, NCOORD, AP
C
C       THIS IS A GRAMPS UTILITY ROUTINE THAT PARSES THE
C       COMMAND LINE FOR A COORD VALUE.  THAT IS A STRING
C       BEGINNING WITH # AND FOLLOWED BY A NUMBER WITHIN
C       THE CORRECT RANGE.
C
      VPOINT=SPOINT
      CALL GETSTR(ARG,SPOINT,CNUM)
      IF(ERROR.NE.0)GOTO 998
      IF(CNUM(1:1).NE.'#')GOTO 997
      AP=2
      CALL GETVLU(CNUM,AP,NCOORD,ADDIT)
      IF(ERROR.NE.0)GOTO 997
C Allow negative coord number to be interpreted by calling routine
      IF( (NCOORD.EQ.0).OR.(ABS(NCOORD).GT.NCOORDS) )GOTO 997
C
C       NORMAL EXIT POINT
C
      ERROR=0
      RETURN
C
C       ERROR EXIT POINT.  RESET ARG POINTER.
C
 997  ERROR=COORDNUM
 998  SPOINT=VPOINT
      RETURN
C
      END
