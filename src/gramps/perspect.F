C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE PERSPECT(ARG)
C
      IMPLICIT NONE
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
      CHARACTER ARG*(*)
      CHARACTER*5 AXIS
      LOGICAL OK
C
C       ENTRY POINT FOR CONVENIENCE OF THE NAME
C
      ENTRY PERSP(ARG)
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
      IF(INDEX(AXIS,'S').NE.0)THEN
        CALL PUTWTFM(ID,WWINDS)
        OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'V').NE.0)THEN
        CALL PUTWTFM(ID,WWINDV)
        OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'P').NE.0)THEN
        CALL PUTWTFM(ID,WWINDP)
        OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'A').NE.0)THEN
        CALL PUTWTFM(ID,WWINDA)
        OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'H').NE.0)THEN
        CALL PUTWTFM(ID,WWINDH)
        OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'Y').NE.0)THEN
        CALL PUTWTFM(ID,WWINDY)
        OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'F').NE.0)THEN
        CALL PUTWTFM(ID,WFOG)
        OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'D').NE.0)THEN
        CALL PUTWTFM(ID,WDEPTH)
        OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'E').NE.0)THEN
        CALL PUTWTFM(ID,WWINDE)
        OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'W').NE.0)THEN
        CALL PUTWTFM(ID,WWINDW)
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
