C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE TRANSLATE(ARG)
C
      IMPLICIT NONE
C
      CHARACTER ARG*(*)
      CHARACTER*7 AXIS
      LOGICAL OK
C
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
#include "GETARGS.FOR"
C
      OK=.FALSE.
C
      IF(INDEX(AXIS,'X').NE.0)THEN
        CALL PUTTFM(ID,UTRANX)
        OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'Y').NE.0)THEN
        CALL PUTTFM(ID,UTRANY)
        OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'Z').NE.0)THEN
        CALL PUTTFM(ID,UTRANZ)
        OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'W').NE.0)THEN
        CALL PUTTFM(ID,UTRANW)
        OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'P').NE.0)THEN
        CALL PUTTFM(ID,UATRANX)
        OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'Q').NE.0)THEN
        CALL PUTTFM(ID,UATRANY)
        OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'R').NE.0)THEN
        CALL PUTTFM(ID,UATRANZ)
        OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'S').NE.0)THEN
        CALL PUTTFM(ID,UATRANW)
        OK=.TRUE.
      END IF
C
C       SAME EXIT STUFF HERE AS IN ROTATE
C
      IF(.NOT.OK) THEN
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
