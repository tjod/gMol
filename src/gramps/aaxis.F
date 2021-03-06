C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE AAXIS(COOR1,COOR2,COOR3,MATRIX,IMATRIX,USE3)
C
      IMPLICIT NONE
C
#include "ERRORS.FOR"
#include "UDLIST.FOR"
      REAL MATRIX(4,4),IMATRIX(4,4)
C
      INTEGER USE3
      REAL PI, ANGSCALE
      PARAMETER (PI=3.1415927)
      PARAMETER (ANGSCALE=1./(2.0*PI))
      REAL DELX,DELY,DELZ,VLENGTH,V,DIRCOSX,DIRCOSY,DIRCOSZ
      REAL DELX3,DELY3,DELZ3,V3LENGTH,DIRCOSX3,DIRCOSY3,DIRCOSZ3
      REAL DEN1,DEN2,DENLEN
      REAL COOR1(4),COOR2(4),COOR3(4)
      REAL XANGLE, YANGLE, ZANGLE, XS1, YS1, ZS1, WS1
      INTEGER OUTSAVE
C
C       THIS ROUTINE CALCULATES THE MATRIX NECESSARY TO
C       ROTATE AND TRANSLATE A SET OF COORDS SO THAT THE
C       ARBITRARY AXIS IS COINCIDENT WITH THE +Z AXIS.
C       IT ALSO CALULATES THE INVERSE MATRIX TO TFM THE
C       SET OF COORDS BACK.  THIS IS THE METHOD FOR
C       ARB AXIS ROTATION USED IN NEWMAN AND SPROUL.
C
C       USE3 SPECIFIES THE (OPTIONAL) Z ROTATION AFTER X AND Y.
C         IF USE3 IS 0, ONLY 2 POINTS WERE GIVEN AND THERE
C           IS NO Z ROTATION.
C         IF USE3 IS 1, COOR3 IS THE POINT WHICH WILL DEFINE
C           THE Q AXIS.
C         IF USE3 IS 2, COOR3 IS THE POINT WHICH WILL DEFINE
C           THE P AXIS.
C
      IF((COOR1(4).EQ.0) .OR. (COOR2(4).EQ.0)) THEN
        ERROR=BADW
        RETURN
      END IF
      DELX=COOR2(1)/COOR2(4) - COOR1(1)/COOR1(4)
      DELY=COOR2(2)/COOR2(4) - COOR1(2)/COOR1(4)
      DELZ=COOR2(3)/COOR2(4) - COOR1(3)/COOR1(4)
      VLENGTH=SQRT(DELX*DELX+DELY*DELY+DELZ*DELZ)
      IF(VLENGTH.EQ.0)THEN
        ERROR=BADAXIS
        RETURN
      END IF
      DIRCOSX=DELX/VLENGTH
      DIRCOSY=DELY/VLENGTH
      DIRCOSZ=DELZ/VLENGTH
      V=SQRT(DIRCOSZ*DIRCOSZ+DIRCOSY*DIRCOSY)
C
C       NEED THIS CHECK BECAUSE OF MATH IMPRECISION.
C       V>1.0 WAS CONFUSING ACOS!
C       IT SHOULD ALWAYS BE THAT -1 < V < 1, BUT...
C
      IF(V.GT.1.0)THEN
        V=1.0
      ELSE IF(V.LT.-1.0)THEN
        V=-1.0
      END IF
C
      IF(V.NE.0)THEN
        XANGLE=ACOS(DIRCOSZ/V)
        YANGLE=ACOS(V)
      ELSE
        YANGLE=PI/2
        XANGLE=0
      END IF
      IF(DELY.LT.0)XANGLE=-XANGLE
      IF(DELX.GT.0)YANGLE=-YANGLE
C
C       CALCULATE Z ROTATION
C
      IF (USE3 .EQ. 0) THEN
        ZANGLE = 0
      ELSE
        IF (COOR3(4).EQ.0) THEN
          ERROR=BADW
          RETURN
        END IF
        DELX3=COOR3(1)/COOR3(4)- COOR1(1)/COOR1(4)
        DELY3=COOR3(2)/COOR3(4)- COOR1(2)/COOR1(4)
        DELZ3=COOR3(3)/COOR3(4)- COOR1(3)/COOR1(4)
        V3LENGTH=SQRT(DELX3*DELX3+DELY3*DELY3+DELZ3*DELZ3)
        IF(V3LENGTH.EQ.0)THEN
          ERROR=BADAXIS
          RETURN
        END IF
C
        DIRCOSX3=DELX3/V3LENGTH
        DIRCOSY3=DELY3/V3LENGTH
        DIRCOSZ3=DELZ3/V3LENGTH
C
C         DEN1/V IS X COORD. OF X,Y ROTATED COOR3
         IF(V.EQ.0)THEN
C JUST HAD Y ROTATION OF +/-90
         DEN1 = DIRCOSZ3*SIN(YANGLE)
         DEN2 = DIRCOSY3
        ELSE
         DEN1 = DIRCOSX3*V*V -
     &          DIRCOSX*(DIRCOSY3*DIRCOSY+DIRCOSZ3*DIRCOSZ)
         DEN2 = DIRCOSY3*DIRCOSZ - DIRCOSZ3*DIRCOSY
        END IF
        DENLEN = SQRT(DEN1*DEN1+DEN2*DEN2)
        IF (DENLEN.EQ.0)THEN
          ERROR=BADAXIS
          RETURN
        END IF
        ZANGLE=ASIN(DEN1/DENLEN)
C
C         DEN2/V IS Y COORD. OF X,Y ROTATED COOR3
C
C       IF (DEN2/V .LT. 0.0) ZANGLE=-(ZANGLE+PI)
        IF (DEN2   .LT. 0.0) ZANGLE=-(ZANGLE+PI)
C
C         ROTATE BACK 90 DEGREES TO GET P AXIS
        IF (USE3 .EQ. 2) ZANGLE=ZANGLE-PI/2
      END IF
C
      XS1=COOR1(1)/COOR1(4)
      YS1=COOR1(2)/COOR1(4)
      ZS1=COOR1(3)/COOR1(4)
      WS1=1.
C
C       IMATRIX IS THE INVERSE MATRIX
C
      OUTSAVE=OUTTING
      OUTTING=OUTNIL
C
      CALL TIDENT
      CALL UTRAN(XS1,YS1,ZS1,WS1)
      CALL UROTX(-XANGLE*ANGSCALE)
      CALL UROTY(-YANGLE*ANGSCALE)
      CALL UROTZ(-ZANGLE*ANGSCALE)
      CALL TGET(IMATRIX)
C
C       MATRIX IS THE ARB AXIS TFM MATRIX
C
      CALL TIDENT
      CALL UROTZ(ZANGLE*ANGSCALE)
      CALL UROTY(YANGLE*ANGSCALE)
      CALL UROTX(XANGLE*ANGSCALE)
      CALL UTRAN(-XS1,-YS1,-ZS1,WS1)
      CALL TGET(MATRIX)
C
      OUTTING=OUTSAVE
C
      RETURN
C
      END
