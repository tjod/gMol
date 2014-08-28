C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE TEXTXYZ(ARG,SPOINT,CHLEN,TXTSIZ,XMOVE,YMOVE,ZMOVE,
     &                     WMOVE)
C
      IMPLICIT NONE
      CHARACTER ARG*(*)
	INTEGER SPOINT, CHLEN, TXTSIZ
      REAL XMOVE, YMOVE, ZMOVE, WMOVE
C
#include "ERRORS.FOR"
#include "UDLIST.FOR"
      CHARACTER BLANK*1
      INTEGER  CNUM
	Integer FPOINT
C
      CHARACTER*20 FARG
      REAL CPERLINE(8)
      SAVE CPERLINE
      DATA CPERLINE/322,217,129,92,76,46,28,17/
      DATA BLANK/' '/
      BLANK=' '
C
      XMOVE=0
      YMOVE=0
      ZMOVE=0
      WMOVE=0
C
C       GET USER VALUES IF GIVEN
C
      IF(SPOINT.NE.0)THEN
        CALL GETSTR(ARG,SPOINT,FARG)
        IF(NPOINT.EQ.0)RETURN
        IF( (ERROR.EQ.0).AND.(FARG(1:1).EQ.'#') )THEN
	  FPOINT=1
          CALL GETCOORD(FARG,FPOINT,CNUM)
	  If(ERROR.NE.0)Return
	  If(CNUM.LT.0)Then
		  XMOVE=-UCOORD(1,ABS(CNUM))
		  YMOVE=-UCOORD(2,ABS(CNUM))
		  ZMOVE=-UCOORD(3,ABS(CNUM))
	  Else
		  XMOVE= UCOORD(1,CNUM)
		  YMOVE= UCOORD(2,CNUM)
		  ZMOVE= UCOORD(3,CNUM)
	  End If
	  WMOVE=UCOORD(4,ABS(CNUM))
          RETURN
        END IF
        IF( (ERROR.EQ.0).AND.(FARG.EQ.'*') )THEN
          IF(CHLEN.GT.CPERLINE(TXTSIZ))THEN
            POINT=1
            NPOINT=POINT+CHLEN
            ERROR=NOCEN
            RETURN
          END IF
          XMOVE=-1.*CHLEN/CPERLINE(TXTSIZ)
        ELSE
          ERROR=0
	  XMOVE=A2F(FARG,ERROR)
          IF(ERROR.NE.0)RETURN
        END IF
        IF(SPOINT.NE.0)THEN
          CALL GETSTR(ARG,SPOINT,FARG)
          IF(ERROR.NE.0)RETURN
	  YMOVE=A2F(FARG,ERROR)
          IF(ERROR.NE.0)RETURN
          IF(SPOINT.NE.0)THEN
            CALL GETSTR(ARG,SPOINT,FARG)
            IF(ERROR.NE.0)RETURN
	    ZMOVE=A2F(FARG,ERROR)
            IF(ERROR.NE.0)RETURN
              IF(SPOINT.NE.0)THEN
                CALL GETSTR(ARG,SPOINT,FARG)
                IF(ERROR.NE.0)RETURN
	        WMOVE=A2F(FARG,ERROR)
                IF(ERROR.NE.0)RETURN
              END IF
          END IF
        END IF
      END IF
C
      RETURN
      END
