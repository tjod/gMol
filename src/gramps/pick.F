C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE PPICK(ARG,SWITCHES)
C
      IMPLICIT NONE
      CHARACTER ARG*(*)
      CHARACTER SWITCHES*(*)
C
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
      Logical MPICK, PICKCOORD, PICKNAME
C
      IF(MATCH(SWITCHES,'POINT'))THEN
        MPICK=PICKCOORD(ARG)
      ELSE IF(MATCH(SWITCHES,'SYMBOL'))THEN
        IF(AGF.EQ.1)THEN
          MPICK=PICKNAME(ARG)
        ELSE
          CALL TELLUSER(NOTCMD,*999)
        END IF
      ELSE IF(MATCH(SWITCHES,'PARAMETER'))THEN
        IF(AGF.GT.1)THEN
          MPICK=PICKNAME(ARG)
        ELSE
          CALL TELLUSER(NOTGIN,*999)
        END IF
      ELSE
        CALL TELLUSER(BADSWITCH,*999)
      END IF
C
      IF(ERROR.EQ.0)THEN
        IF(MATCH(SWITCHES,'WAIT'))THEN
          IF(NOBJ.GT.RWORLD)WAITING=CMDFROM
        Else IF(MATCH(SWITCHES,'ECHO'))THEN
          IF(NOBJ.GT.RWORLD)PICKECHO=CMDFROM
        END IF
      ELSE
        CALL TELLUSER(ERROR,*999)
      END IF
C
C If MPICK==TRUE user will have to mouse-pick, so figure out how.
C Otherwise, this pick command just assigned a constant value, so
C leave HITTING alone in case earlier mouse-pick active.
      If(MPICK)Then
        HITTING=0
        IF(MATCH(SWITCHES,'RIGHT'))HITTING=HITTING+1
        IF(MATCH(SWITCHES,'MIDDLE'))HITTING=HITTING+2
        IF(MATCH(SWITCHES,'LEFT'))HITTING=HITTING+4
      If(HITTING.EQ.0)HITTING=7
      End If
C
 999  RETURN
      END
C
C
      Logical Function PICKCOORD(ARG)
C
      IMPLICIT NONE
      CHARACTER ARG*(*)
C
#include "UDLIST.FOR"
#include "ERRORS.FOR"
#include "DEFVAL.FOR"
Cinclude "DEVICES.FOR"
C
C
      INTEGER NCOORD, I, SPOT, ARGNUM, SPOINT
      CHARACTER*12 UTMP
      REAL SFACTOR
C
C Returns TRUE if only one arg (user will have to mouse-pick)
      PICKCOORD=.FALSE.
C
C       GET THE NAME (NUMBER) OF THE COORD
C
      SPOINT=POINT
      CALL GETCOORD(ARG,POINT,NCOORD)
      IF(NCOORD.LE.0)Then
        ERROR=COORDNUM
        POINT=SPOINT
      End If
      IF(ERROR.NE.0)GOTO 999
C
C       GET THE  X,Y,Z'S DEFINING THE COORD
C
      If(TUNITS(AGF).EQ.NEWUNITS)Then
        SFACTOR=1.0
      Else
        SFACTOR=32767.0
      End If
      IF (POINT.EQ.0) THEN
        HITCOORD=NCOORD
        CALL SETPICKCURSOR
        DCHANGE=.TRUE.
        PICKCOORD=.TRUE.
      ELSE
        UCOORD(4,NCOORD)=1.
        DO 10 I=1,4
        CALL GETSTR(ARG,POINT,UTMP)
        UCOORD(I,NCOORD)=A2F(UTMP,ERROR)/SFACTOR
        IF (ERROR.NE.0) GOTO 999
        IF (POINT.EQ.0) THEN
          IF ( (I.EQ.1) .OR. (I.EQ.2) ) THEN
            ERROR=INCOMP
            GOTO 999
          ELSE IF ( I.EQ.3 ) THEN
            GOTO 999
          ELSE
            CONTINUE
          END IF
        END IF
 10     CONTINUE
      END IF
C
 999  RETURN
      END
C
C
      Logical Function PICKNAME(ARG)
C
      IMPLICIT NONE
C
#include "UDLIST.FOR"
#include "ERRORS.FOR"
#include "DOLLARGS.FOR"
C#include "DEVICES.FOR"
C
      INTEGER SPOT, ARGNUM
      CHARACTER ARG*(*)
      CHARACTER*1 DOLLAR
      DOLLAR='$'
C
C Returns TRUE if only one arg (user will have to mouse-pick)
C
      PICKNAME=.FALSE.
      SPOT=INDEX(ARG,DOLLAR)
      IF(SPOT.EQ.0)THEN
        POINT=1
        NPOINT=3
        ERROR=BADDOLARG
        GOTO 999
      END IF
      POINT=SPOT+1
      CALL GETVLU(ARG,POINT,ARGNUM,ADDIT)
      IF(ERROR.NE.0)GOTO 999
      IF( (ARGNUM.LE.0).OR.(ARGNUM.GT.ARGMAX) )THEN
        POINT=SPOT
        ERROR=BADDOLARG
        GOTO 999
      END IF
      IF(POINT.NE.0)THEN
        CALL GETSTR(ARG,POINT,ARGLIST(ARGNUM,AGF))
        IF(ERROR.NE.0)GOTO 999
      ELSE
        HITSYM=ARGNUM
      CALL SETPICKCURSOR
      DCHANGE=.TRUE.
      PICKNAME=.TRUE.
      END IF
C
 999  RETURN
      END
C
      SUBROUTINE CHECKWAIT(FLAG)
      Implicit NONE
      Integer FLAG
#include "UDLIST.FOR"
      FLAG = WAITING
      END
