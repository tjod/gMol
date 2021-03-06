C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE GROUPINFO(NID,START,FINISH,DEPTH,LEVEL,SIZE)
C
      IMPLICIT NONE
	INTEGER NID, START, FINISH, DEPTH, LEVEL, SIZE
	INTEGER I, K, PUSHPOP
C
#include "UDLIST.FOR"
C
C       THIS IS A GRAMPS UTILITY ROUTINE THAT FINDS OUT
C       ABOUT A GROUP AND REPORTS (RETURNS) WHERE IN THE
C       TREE THE GROUP STARTS, WHERE IT ENDS, HOW DEEP THE
C       GROUP IS (HOW MANY GROUPS THE GROUP CONTAINS) AND
C       ITS LEVEL (HOW MANY GROUPS CONTAIN IT).
C
C       THE WORLDS ARE EASY.  THEY CONTAIN EVERYTHING.
C       DEPTH IS NOT CALCULATED.  IT MUST BE LESS THAN
C       OR EQUAL TO MAXDEPTH, SO JUST RETURN MAXDEPTH.
C
      IF(NID.LE.RWORLD)THEN
        START=1
        FINISH=TREEEND
        LEVEL=0
        DEPTH=MAXDEPTH
        SIZE=UDFREE
        RETURN
      END IF
C
C       INITIALIZE RETURN VALUES TO DEFAULTS
C
      START=0
      FINISH=0
      DEPTH=0
      LEVEL=0
      SIZE=SIZEOBJ(NID)
C
C       GO THROUGH THE WHOLE TREE UNTIL YOU ENCOUNTER THE
C       GROUP IN QUESTION, KEEPING TRACK OF ANY OTHER
C       GROUPS YOU ENCOUNTER ALONG THE WAY.
C
      DO 20 I=1,TREEEND
      K=TREE(I)
      IF(K.GT.GROUPEND)THEN
        IF(TREE(I).EQ.NID)THEN
          START=I
          GOTO 21
        END IF
        IF(TYPOBJ(K).EQ.GROUP)LEVEL=LEVEL+1
      ELSE
        LEVEL=LEVEL-1
      END IF
 20   CONTINUE
C
C       COULDN'T FIND THAT OBJECT
C
      RETURN
C
C       THE OBJECT NEEDN'T BE A GROUP, IN WHICH CASE ITS
C       DEPTH IS ONE (ALREADY DEAFULTED) AND ITS
C       FINISH = ITS START.  ASSUME ITS NOT A GROUP.
C
 21   CONTINUE
      FINISH=START
      IF(TYPOBJ(NID).NE.GROUP)RETURN
C
C       SINCE THIS IS A GROUP FIND OUT THE GROUPS END
C       BY CONTINUING ALONG IN THE TREE UNTIL WE
C       FIND THE GROUP_END CORRESPONDING TO THIS GROUP.
C       IT MAY NOT BE THE NEXT GROUP_END, SINCE THERE
C       MAY BE ANOTHER GROUP_START UNDER THIS GROUP.
C       COUNT THE GROUP_STARTS WITHIN THE GROUP UNDER
C       STUDY TO FIND OUT THE GROUPS DEPTH.
C
C
      SIZE=0
      PUSHPOP=0
      DO 30 I=START,TREEEND
      K=TREE(I)
      IF(K.LE.GROUPEND)THEN
        DEPTH=MAX(DEPTH,PUSHPOP)
        PUSHPOP=PUSHPOP-1
        SIZE=SIZE+1
      ELSE
        IF(TYPOBJ(K).EQ.GROUP)PUSHPOP=PUSHPOP+1
        SIZE=SIZE + SIZEOBJ(NID)
      END IF
      IF(PUSHPOP.EQ.0)THEN
        FINISH=I
        RETURN
      END IF
 30   CONTINUE
C
      RETURN
      END
