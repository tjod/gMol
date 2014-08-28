C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE GRAMPS_RENAME(ARG)
C
      IMPLICIT NONE
      CHARACTER ARG*(*)
C
	INTEGER VPOINT, NEWID
#include "UDLIST.FOR"
#include "ERRORS.FOR"
      CHARACTER NEWNAME*(NAMMAX)
C
C       THIS ROUTINE MODIFIES THE ARRAY OF NAMES OF OBJ'S
C
C       LOCATE THE OLD NAME, GET THE NEW NAME, CHECK IF
C       THE NEW NAME IS IN USE AND IF NOT RENAME.
C
      POINT=1
      CALL NAMID(ARG,POINT,ID)
      IF(ERROR.NE.0)CALL TELLUSER(ERROR,*999)
      IF(POINT.EQ.0)CALL TELLUSER(NONAME,*999)
      VPOINT=POINT
      CALL NAMEID(ARG,POINT,NEWID,NEWNAME)
      IF(ERROR.EQ.0)ERROR=DUPNAM
      IF(ERROR.NE.OBJERR)THEN
        POINT=VPOINT
        CALL TELLUSER(ERROR,*999)
      END IF
      ERROR=0
C
      NAMOBJ(ID)=NEWNAME
C
 999  RETURN
      END
