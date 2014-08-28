C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE SFRAME(ARG,SWITCHES)
C
      IMPLICIT NONE
      CHARACTER SWITCHES*(*)
      CHARACTER ARG*(*)
C
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
	INTEGER START, FINISH, DEPTH, LEVEL, SIZE
	INTEGER I, K
      CHARACTER AXIS*2
      LOGICAL OK,RETAIN
C
#include "GETARGS.FOR"
C
      OK=.FALSE.
      IF(MATCH(SWITCHES,'RETAIN'))THEN
        RETAIN=.TRUE.
      ELSE
        RETAIN=.FALSE.
      END IF
C
C       IF THE OBJ IS A GROUP, SET THE FRAMING FOR EACH
C       OBJECT WITHIN THE GROUP
C
      IF(TYPOBJ(ID).EQ.GROUP)
     &    CALL GROUPINFO(ID,START,FINISH,DEPTH,LEVEL,SIZE)
C
      IF(INDEX(AXIS,'N').NE.0)THEN
        IF(TYPOBJ(ID).NE.GROUP)THEN
          IF(TYPOBJ(ID).EQ.FRAME)CALL PUTTFM(ID,UFRAME)
        ELSE
          DO 20 I=START+1,FINISH
          K=TREE(I)
          IF( (K.NE.GROUPEND).AND.(TYPOBJ(K).EQ.FRAME) )
     &        CALL PUTTFM(K,UFRAME)
 20       CONTINUE
        END IF
        OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'I').NE.0)THEN
        IF(TYPOBJ(ID).NE.GROUP)THEN
          IF(TYPOBJ(ID).EQ.FRAME)CALL PUTTFM(ID,UFRAMI)
        ELSE
          DO 30 I=START+1,FINISH
          K=TREE(I)
          IF( (K.NE.GROUPEND).AND.(TYPOBJ(K).EQ.FRAME) )
     &        CALL PUTTFM(K,UFRAMI)
 30       CONTINUE
        END IF
        OK=.TRUE.
      END IF
C
C       .NOT.OK MEANS NO VALID SPECIFIER WAS GIVEN
C
      IF(.NOT.OK)THEN
        POINT=VPOINT
        NPOINT=MPOINT
        CALL TELLUSER(AXISERR,*999)
      END IF
C
C       MORE ARGS MAY FOLLOW
C
      IF(POINT.NE.0)GOTO 10
C
C       MAKE SURE THAT THE SUMMARY WORDS IN DEVUSE AND FNUSE
C       REFLECT CHANGES MADE HERE.  IF THE FRAMING OBJ IS
C       A GROUP, EACH GROUP MEMBER WAS AFFECTED.
C
      IF(TYPOBJ(ID).NE.GROUP)THEN
        CALL ORTOTAL(ID)
        IF( RETAIN )THEN
          KINDOBJ(ID)=IOR(KINDOBJ(ID),RETBYTE)
        ELSE
          KINDOBJ(ID)=IAND(KINDOBJ(ID),NOT(RETBYTE))
        END IF
      ELSE
        DO 40 I=START,FINISH
        K=TREE(I)
        IF(K.NE.GROUPEND)THEN
          IF(TYPOBJ(K).EQ.FRAME)THEN
            CALL ORTOTAL(K)
            IF( RETAIN )THEN
              KINDOBJ(K)=IOR(KINDOBJ(K),RETBYTE)
            ELSE
              KINDOBJ(K)=IAND(KINDOBJ(K),NOT(RETBYTE))
            END IF
          END IF
        END IF
 40     CONTINUE
      END IF
C
 999  RETURN
      END
