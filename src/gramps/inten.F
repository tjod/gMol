C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE INTEN(ARG)
C
      IMPLICIT NONE
C
#include "UDLIST.FOR"
#include "ERRORS.FOR"
      INTEGER SIZE
C
      CHARACTER ARG*(*)
	INTEGER START, FINISH, DEPTH, LEVEL, I, K, J
      LOGICAL OK
	Integer NTYPES
	Parameter (NTYPES=6)
      CHARACTER AXIS*(NTYPES), KWORDS*(NTYPES)
	Integer KTYPES(NTYPES)
	Data KWORDS/'TPDAYE'/
	Data KTYPES/UITRNSP, UISPEC, UIDIFF, UIAMBI,
     &		    UISHIN, UIEMIS/
C
#include "GETARGS.FOR"
C
      OK=.FALSE.
C
C       IF ITS A GROUP, EACH MEMBER MUST HAVE ITS INTENSITY
C       SET, SINCE THE INTENSITY IS NOT A MATRIX AND THEREFORE
C       ONLY HAS AN EFFECT FOR AN INDIVIDUAL OBJECT.
C
      IF(TYPOBJ(ID).EQ.GROUP)THEN
        CALL GROUPINFO(ID,START,FINISH,DEPTH,LEVEL,SIZE)
        IF(START.EQ.0)START=1
      END IF
C
      DO i=1,NTYPES
      IF(INDEX(AXIS,KWORDS(i:i)).NE.0)THEN
        IF(TYPOBJ(ID).NE.GROUP)THEN
          CALL PUTTFM(ID,KTYPES(i))
        ELSE
          IF(ID.LE.RWORLD)THEN
            CALL PUTTFM(ID,KTYPES(i))
          END IF
          DO 20 j=START,FINISH
          K=TREE(j)
          IF(K.GT.GROUPEND)THEN
            CALL PUTTFM(K,KTYPES(i))
          END IF
 20       CONTINUE
        END IF
        OK=.TRUE.
      END IF
      End Do
C
C       STANDARD EXIT TESTING AND STUFF (SEE ROTATE FOR DETAILS)
C
      IF( .NOT.OK )THEN
        POINT=VPOINT
        NPOINT=MPOINT
        CALL TELLUSER(AXISERR,*999)
      END IF
C
      IF(POINT.NE.0)GOTO 10
C
C       REMEMBER TO ORTOTAL EACH MEMBER, IF THIS INTEN
C       AFFECTED A GROUP
C
      IF(TYPOBJ(ID).NE.GROUP)THEN
        CALL ORTOTAL(ID)
      ELSE
        IF(ID.LE.RWORLD)CALL ORTOTAL(ID)
        DO 50 I=START,FINISH
        K=TREE(I)
        IF(K.GT.GROUPEND)CALL ORTOTAL(K)
 50     CONTINUE
      END IF
C
 999  RETURN
      END
