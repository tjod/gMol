C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE SSCALE(ARG)
C
      IMPLICIT NONE
C
#include "UDLIST.FOR"
#include "ERRORS.FOR"
      CHARACTER ARG*(*)
      CHARACTER*5 AXIS
      LOGICAL OK
C
#include "GETARGS.FOR"
C
      OK=.FALSE.
C
      IF(INDEX(AXIS,'X').NE.0)THEN
        CALL PUTTFM(ID,USCALEX)
        OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'Y').NE.0)THEN
        CALL PUTTFM(ID,USCALEY)
        OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'Z').NE.0)THEN
        CALL PUTTFM(ID,USCALEZ)
        OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'W').NE.0)THEN
        CALL PUTTFM(ID,USCALEW)
        OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'P').NE.0)THEN
        CALL PUTTFM(ID,UASCALX)
        OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'Q').NE.0)THEN
        CALL PUTTFM(ID,UASCALY)
        OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'R').NE.0)THEN
        CALL PUTTFM(ID,UASCALZ)
        OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'S').NE.0)THEN
        CALL PUTTFM(ID,UASCALW)
        OK=.TRUE.
      END IF
C
C       SAME EXIT STUFF HERE AS IN ROTATE
C
      IF(.NOT.OK)THEN
        POINT=VPOINT
        NPOINT=MPOINT
        CALL TELLUSER(AXISERR,*999)
      END IF
C
      IF(POINT.NE.0)  GOTO 10
C
      CALL ORTOTAL(ID)
C
 999  RETURN
      END
