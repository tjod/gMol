C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      INTEGER UPOINT, LPOINT, VPOINT, MPOINT
C
C       PARSE THE ARGS FOR NAME,SPECIFIER,DEV/VAL [,SPECIFIER,DEV/VAL]
C       SPECIFIER IS USUALLY AN AXIS NAME
C
      POINT=1
      UPOINT=POINT
      CALL NAMID(ARG,POINT,ID)
      IF(ERROR.NE.0)CALL TELLUSER(ERROR,*999)
      LPOINT=POINT
C
 10   CONTINUE
      VPOINT=POINT
      CALL GETSTR(ARG,POINT,AXIS)
      CALL UPCASE(AXIS)
      IF(ERROR.NE.0)CALL TELLUSER(AXISERR,*999)
      MPOINT=NPOINT
      IF(POINT.EQ.0)CALL TELLUSER(INCOMP,*999)
      CALL GETFN(ARG,POINT)
      IF(ERROR.NE.0)CALL TELLUSER(ERROR,*999)
C
C       A "C" ACCOMPANYING A SPECIFIER MEANS CONTINUOUS
C       UPDATES OF THE TRANSFORMATION, SO FLAG THE FACT.
C
      IF(INDEX(AXIS,'C').NE.0)THEN
        FUNKY=IOR(FUNKY,FADDINC)
      END IF
      IF( BTEST(FUNKY,BADDINC) )CONT=.TRUE.
C
C       NOW THAT THE ARGS ARE GOT, CHECK OUT ALL POSSIBLE
C       SPECIFIERS AND HAVE PUTTFM INSERT THE INFO INTO
C       THE CORRECT SPOTS IN THE DEVUSE, FNUSE, ETC. ARRAYS
C
