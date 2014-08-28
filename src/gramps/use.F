C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE USE(ARG,SWITCHES)
C
      IMPLICIT NONE
      CHARACTER ARG*(*),SWITCHES*(*)
C
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
      REAL AMAT(4,4),IAMAT(4,4)
      Real DUMMY(4), c1(4), c2(4), c3(4)
      INTEGER COORD1, COORD2, COORD3, MPOINT, I, J
C
C       THIS ROUTINE INSERTS THE PREVIOUSLY PICKED COORDS
C       AND CALCULATES THE ARB AXIS MATRIX WHICH IS INSERTED
C       INTO THE UDL OF AN OBJECT SO THAT IT WILL ROTATE ABOUT
C       THE ARB AXIS.  IT ALSO INSERTS THE INVERSE MATRIX.
C
C       GET THE COORD NAMES (NUMBER) AND THE OBJ TO USE IT ON
C
      CALL GETCOORD(ARG,POINT,COORD1)
      IF(ERROR.NE.0)CALL TELLUSER(ERROR,*999)
      IF(POINT.EQ.0)CALL TELLUSER(INCOMP,*999)
C
      CALL GETCOORD(ARG,POINT,COORD2)
      IF(ERROR.NE.0)CALL TELLUSER(ERROR,*999)
      IF(POINT.EQ.0)CALL TELLUSER(INCOMP,*999)
C
      IF( (MATCH(SWITCHES,'PAXIS')) .OR.
     &    (MATCH(SWITCHES,'QAXIS')) ) THEN
        CALL GETCOORD(ARG,POINT,COORD3)
        IF(ERROR.NE.0)CALL TELLUSER(ERROR,*999)
      Else
	coord3=0
      END IF
c
      IF(POINT.EQ.0)CALL TELLUSER(NONAME,*999)
C
      MPOINT=POINT
      CALL NAMID(ARG,POINT,ID)
      IF (ERROR.NE.0) CALL TELLUSER(ERROR,*999)
C
c	 Allow for negative coord #
	Do i=1,4
	If(coord1.lt.0 .and. ABS(coord1).le.NCOORDS)Then
		c1(i)=    -ucoord(i,ABS(coord1))
	Else If(coord1.gt.0 .and. coord1.le.NCOORDS)Then
		c1(i)=     ucoord(i,ABS(coord1))
	End If
	If(coord2.lt.0 .and. ABS(coord2).le.NCOORDS)Then
		c2(i)=    -ucoord(i,ABS(coord2))
	Else If(coord2.gt.0 .and. coord2.le.NCOORDS)Then
		c2(i)=     ucoord(i,ABS(coord2))
	End If
	If(coord3.lt.0 .and. ABS(coord3).le.NCOORDS)Then
		c3(i)=    -ucoord(i,ABS(coord3))
	Else If(coord3.gt.0 .and. coord3.le.NCOORDS)Then
		c3(i)=     ucoord(i,ABS(coord3))
	End If
	End Do
c	c1(4)=     ucoord(i,ABS(coord1))
c	c2(4)=     ucoord(i,ABS(coord2))
c	c3(4)=     ucoord(i,ABS(coord3))
      IF(MATCH(SWITCHES,'PAXIS'))THEN
        CALL AAXIS(c1,c2,c3,AMAT,IAMAT,2)
      ELSE IF(MATCH(SWITCHES,'QAXIS'))THEN
        CALL AAXIS(c1,c2,c3,AMAT,IAMAT,1)
      ELSE
        CALL AAXIS(c1,c2,dummy,AMAT,IAMAT,0)
      END IF
      IF(ERROR.NE.0)CALL TELLUSER(ERROR,*999)
C
C       THEN WRITE THE CHOSEN MATRICES INTO THE UDL
C
      DO 10 I=1,4
      DO 10 J=1,4
      ARBMAT(I,J,ID)=AMAT(I,J)
      IARBMAT(I,J,ID)=IAMAT(I,J)
 10   CONTINUE
C
C       LET DISPLAY KNOW THAT A CHANGE HAS OCCURRED
C
      DCHANGE=.TRUE.
 999  RETURN
      END
