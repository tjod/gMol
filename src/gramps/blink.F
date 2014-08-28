C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE BLINK(ARG,SWITCHES)
C
      IMPLICIT NONE
C
#include "UDLIST.FOR"
#include "ERRORS.FOR"
      INTEGER SIZE
C
      CHARACTER ARG*(*)
      CHARACTER SWITCHES*(*)
      INTEGER  SPOT, START, FINISH, DEPTH, LEVEL
      INTEGER I,K
C
C       THIS ROUTINE SETS BITS IN THE LINE GENERATOR STATUS WORD 
C       OF AN OBJ'S UDL WHICH AFFECTS THE BLINKING OF AN OBJ.
C
C       GET THE OBJECT NAME.
C
      POINT=1
      CALL NAMID(ARG,POINT,ID)
      IF(ERROR.NE.0)CALL TELLUSER(ERROR,*999)
C
C       ASSUME USER WANTS BLINKING TURNED ON
C       UNLESS THE /OFF SWITCH IS USED.
C
C       IF THE OBJ TO BE BLINKED IS A GROUP, WE MUST
C       MODIFY THE DLIST WORD OF EACH MEMBER OF THE GROUP
C
      IF(TYPOBJ(ID).NE.GROUP)THEN
        SPOT=LOCOBJ(ID)-1+UDBLINK
        IF(MATCH(SWITCHES,'OFF'))THEN
          DLIST(SPOT)=0
        ELSE
          DLIST(SPOT)=1
        END IF
      ELSE
        CALL GROUPINFO(ID,START,FINISH,DEPTH,LEVEL,SIZE)
        IF(START.EQ.0)START=1
        DO 20 I=START,FINISH
        K=TREE(I)
        IF(K.GT.GROUPEND)THEN
          SPOT=LOCOBJ(K)-1+UDBLINK
          IF(MATCH(SWITCHES,'OFF'))THEN
            DLIST(SPOT)=0
          ELSE
            DLIST(SPOT)=1
          END IF
        END IF
 20     CONTINUE
      END IF
C
C       IF ITS A WORLD, DO IT TO IT
C
      IF(ID.LE.RWORLD)THEN
        SPOT=LOCOBJ(ID)-1+UDBLINK
        IF(MATCH(SWITCHES,'OFF'))THEN
          DLIST(SPOT)=0
        ELSE
          DLIST(SPOT)=1
        END IF
      END IF
C
C       LET DISPLAY KNOW A CHANGE IN A UDL HAS OCCURRED
C
      DCHANGE=.TRUE.
 999  RETURN
      END
