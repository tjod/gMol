C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE FIX(ARG,SWITCHES)
C
      IMPLICIT NONE
C
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
      INTEGER SIZE
      CHARACTER ARG*(*)
      CHARACTER SWITCHES*(*)
      INTEGER START, FINISH, DEPTH, LEVEL, I, K
C
C       THIS ROUTINE CONCATENATES ALL THE TFM MATRICES OF
C       AN OBJ INTO ONE FIX MATRIX.  THE INDIVIDUAL TFM MATRICES
C       ARE RESET TO 
C       UNIT MATRICES FOR FURTHER USE.  THE FIX
C       MATRIX IS INITALLY THE UNIT MATRIX.
C
C       GET THE OBJ NAME AND ID
C
      POINT=1
 1    CALL NAMID(ARG,POINT,ID)
      IF(ERROR.NE.0)CALL TELLUSER(ERROR,*999)
C
c      CALL WRESET(SWITCHES)
      If(WRESET(SWITCHES))CONTINUE
C
      CALL FFIX(ID,SWITCHES)
C
C       AN ENTIRE GROUP CAN BE FIXED IF /GROUP IS SPECIFIED
C
      IF( MATCH(SWITCHES,'GROUP').AND.(TYPOBJ(ID).EQ.GROUP) )THEN
        CALL GROUPINFO(ID,START,FINISH,DEPTH,LEVEL,SIZE)
        DO 30 I=START+1,FINISH
        K=TREE(I)
        IF (K.GT.GROUPEND)CALL FFIX(K,SWITCHES)
 30     CONTINUE
      END IF
C
C       MORE ARGS? - GET THEM
C
      IF(POINT.NE.0)GOTO 1
 999  RETURN
      END
C
C
      SUBROUTINE FFIX(NID,SWITCHES)
C
      IMPLICIT NONE
      INTEGER NID, ESID
C
#include "UDLIST.FOR"
#include "TFMMAT.FOR"
      CHARACTER SWITCHES*(*)
      INTEGER I
      LOGICAL LWORK(WORKSIZE)
      EQUIVALENCE (WORK,LWORK)
C
C       THIS ROUTINE DOES THE MATRIX CONCATENATION OF ALL
C       TFM MATRICES OF AN OBJ.  IT ALSO WRITES THE MATRIX
C       INTO THE UDL IN THE CORRECT SPOT AND RESET THE OTHER
C       TFM MATRICES TO UNIT MATRICES.
C
      CALL MAKEFIX(NID,SWITCHES,FIXMAT(1,1,NID))
      ESID=IDOBJ(NID)
      DO 10 I=UITRNSP,SUMMARY-1,1
        IF(LWORK(I))THEN
          DEVUSE(I,NID)=0
          FNUSE(I,NID)=0
        ENDIF
        LWORK(I)=.FALSE.
 10   CONTINUE
      CALL RRESET(NID,SWITCHES)
C
      RETURN
      END
C
C
      SUBROUTINE MAKEFIX(NID,SWITCHES,MATRIX)
C
      IMPLICIT NONE
C
#include "UDLIST.FOR"
#include "TFMMAT.FOR"
#include "DEFVAL.FOR"
      INTEGER NID
      CHARACTER SWITCHES*(*)
      REAL MATRIX(4,4)
      INTEGER I
      INTEGER ESID
      REAL XVAL, YVAL, ZVAL, W
      LOGICAL LWORK(WORKSIZE)
      EQUIVALENCE (WORK,LWORK)
      Integer OUTSAVE
C
      OUTSAVE=OUTTING
      OUTTING=OUTNIL
      ESID=IDOBJ(NID)
      CALL TIDENT
C
C       PERSPECTIVE MATRIX.
c	IF((NID.EQ.LWORLD.OR.NID.EQ.RWORLD).AND.LWORK(MTFM+WWINDE) )THEN
c		IF(WLDVAL(WWINDW,NID).LT.1)WLDVAL(WWINDW,NID)=1
c		CALL UWINDP(WLDVAL(WWINDS,NID),WLDVAL(WWINFV,NID),
c     &			WLDVAL(WWINDP,NID),WLDVAL(WWINDA,NID),
c     &			WLDVAL(WWINDH,NID),WLDVAL(WWINDY,NID),
c     &			WLDVAL(WWINDE,NID),WLDVAL(WWINDW,NID))
c	END IF
C
C       TRANSLATION STUFF
C
      IF(LWORK(UTRANX))THEN
        XVAL=TFMVAL(UTRANX,NID)
      ELSE
        XVAL=0
      END IF
      IF(LWORK(UTRANY))THEN
        YVAL=TFMVAL(UTRANY,NID)
      ELSE
        YVAL=0
      END IF
      IF(LWORK(UTRANZ))THEN
        ZVAL=TFMVAL(UTRANZ,NID)
      ELSE
        ZVAL=0
      END IF
      W=TFMVAL(UTRANW,NID)
      IF(LWORK(UTRANX).OR.LWORK(UTRANY).OR.LWORK(UTRANZ))
     &  CALL UTRAN(XVAL,YVAL,ZVAL,W)
C
C       ROTATIONS (EACH IN A SEPARATE MATRIX MAKES IT EASIER).
C
      IF(LWORK(UROTXX))CALL UROTX(TFMVAL(UROTXX,NID))
      IF(LWORK(UROTYY))CALL UROTY(TFMVAL(UROTYY,NID))
      IF(LWORK(UROTZZ))CALL UROTZ(TFMVAL(UROTZZ,NID))
C
C       ARBITRARY AXIS STUFF.
C
C     CALL MATCON(TFMMTX(1,1,CURRENT),IARBMAT(1,1,NID) )
      If(IARBMAT(4,4,NID).NE.0)CALL TMUL(IARBMAT(1,1,NID))
C
      IF(LWORK(UATRANX))THEN
        XVAL=TFMVAL(UATRANX,NID)
      ELSE
        XVAL=0
      END IF
      IF(LWORK(UATRANY))THEN
        YVAL=TFMVAL(UATRANY,NID)
      ELSE
        YVAL=0
      END IF
      IF(LWORK(UATRANZ))THEN
        ZVAL=TFMVAL(UATRANZ,NID)
      ELSE
        ZVAL=0
      END IF
      W=TFMVAL(UATRANW,NID)
      IF(LWORK(UATRANX).OR.LWORK(UATRANY).OR.LWORK(UATRANZ))
     &  CALL UTRAN(XVAL,YVAL,ZVAL,W)
C
C       ARB ROTATIONS
C
      IF(LWORK(UAROTX))CALL UROTX(TFMVAL(UAROTX,NID))
      IF(LWORK(UAROTY))CALL UROTY(TFMVAL(UAROTY,NID))
      IF(LWORK(UAROTZ))CALL UROTZ(TFMVAL(UAROTZ,NID))
C
C       ARB SCALES
C
      IF(LWORK(UASCALX))
     &	CALL USCALE(TFMVAL(UASCALX,NID),DASCY,DASCZ,DASCW)
      IF(LWORK(UASCALY))
     &	CALL USCALE(DASCX,TFMVAL(UASCALY,NID),DASCZ,DASCW)
      IF(LWORK(UASCALZ))
     &	CALL USCALE(DASCX,DASCY,TFMVAL(UASCALZ,NID),DASCW)
      IF(LWORK(UASCALW))
     &	CALL USCALE(DASCX,DASCY,DASCZ,TFMVAL(UASCALW,NID))
C
C     CALL MATCON(TFMMTX(1,1,CURRENT),ARBMAT(1,1,NID) )
      If(ARBMAT(4,4,NID).NE.0)CALL TMUL(ARBMAT(1,1,NID))

C       END OF ARBITRARY AXIS STUFF
C
C       CARTESIAN AXIS SCALES
C
      IF(LWORK(USCALEX))
     &	CALL USCALE(TFMVAL(USCALEX,NID),DSCY,DSCZ,DSCW)
      IF(LWORK(USCALEY))
     &	CALL USCALE(DSCX,TFMVAL(USCALEY,NID),DSCZ,DSCW)
      IF(LWORK(USCALEZ))
     &	CALL USCALE(DSCX,DSCY,TFMVAL(USCALEZ,NID),DSCW)
      IF(LWORK(USCALEW))
     &	CALL USCALE(DSCX,DSCY,DSCZ,TFMVAL(USCALEW,NID))
C
C       THE CURRENT FIX MATRIX IS CONCATENATED, TOO.
C       THEN REPLACE THE FIX MATRIX WITH THE CURRENT
C       TFM MATRIX INTO WHICH WE'VE BEEN CONCATENATING.
C
C     CALL MATCON(TFMMTX(1,1,CURRENT),FIXMAT(1,1,NID) )
C
C Fix up 4,4 element, if it has never been used (see openobj)
	If(FIXMAT(4,4,NID).EQ.0.0)FIXMAT(4,4,NID)=1.0
C
      CALL TMUL(FIXMAT(1,1,NID))
      IF(OUTTING.NE.OUTWINDOW)CALL NORM(1.)
      CALL TGET(MATRIX)
C
      OUTTING=OUTSAVE
      RETURN
      END

