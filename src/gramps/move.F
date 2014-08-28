C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE MMOVE(ARG)
C
      IMPLICIT NONE
C
      CHARACTER ARG*(*)
      CHARACTER*5 AXIS
C
      LOGICAL OK
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
#include "GETARGS.FOR"
      IF(ID.GT.RWORLD)THEN
        POINT=UPOINT
        NPOINT=LPOINT
        CALL TELLUSER(NOTWORLD,*999)
      END IF
C
      OK=.FALSE.
C
      IF(INDEX(AXIS,'X').NE.0)THEN
        CALL PUTWTFM(ID,WMOVEX)
        OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'Y').NE.0)THEN
        CALL PUTWTFM(ID,WMOVEY)
        OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'Z').NE.0)THEN
        CALL PUTWTFM(ID,WMOVEZ)
        OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'W').NE.0)THEN
        CALL PUTWTFM(ID,WMOVEW)
        OK=.TRUE.
      END IF
C
C       SAME EXIT AS ROTATE
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
