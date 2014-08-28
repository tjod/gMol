C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE LOOP(ARG)
C
	IMPLICIT NONE
      CHARACTER ARG*(*)
#include "UDLIST.FOR"
	INTEGER START, FINISH, DEPTH, LVL, SIZE
C
C     WRITTEN  4/20/90 TJ O'DONNELL
C
      IF(ARG.NE.' ')THEN
        POINT=1
        CALL NAMID(ARG,POINT,ID)
        IF(ERROR.NE.0)CALL TELLUSER(ERROR,*999)
        CALL GROUPINFO(ID,START,FINISH,DEPTH,LVL,SIZE)
        IF(ID.LE.RWORLD)START=1
        IF(POINT.NE.0)THEN
          CALL GETVLU(ARG,POINT,CLOOP,ADDIT)
          IF(ERROR.NE.0)CALL TELLUSER(ERROR,*999)
          IF(CLOOP.LE.0)CLOOP=0
        ELSE
          CLOOP=0
        END IF
      ELSE
        CLOOP=0
      END IF
C
      IF(CLOOP.GT.0)KINDOBJ(ID)=IOR(KINDOBJ(ID), LOOPBYTE)
 999  CONTINUE
      RETURN
      END
