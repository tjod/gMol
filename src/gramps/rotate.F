C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE ROTATE(ARG,SWITCHES)
C
      IMPLICIT NONE
      CHARACTER ARG*(*),SWITCHES*(*)
C
      LOGICAL OK
      CHARACTER*7 AXIS
C
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
#include "GETARGS.FOR"
C
      OK=.FALSE.
C
      IF(INDEX(AXIS,'X').NE.0)THEN
         IF(MATCH(SWITCHES,'LAB'))FUNKY=IOR(FUNKY,FLABROT)
         CALL PUTTFM(ID,UROTXX)
         OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'Y').NE.0)THEN
         IF(MATCH(SWITCHES,'LAB'))FUNKY=IOR(FUNKY,FLABROT)
         CALL PUTTFM(ID,UROTYY)
         OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'Z').NE.0)THEN
         IF(MATCH(SWITCHES,'LAB'))FUNKY=IOR(FUNKY,FLABROT)
         CALL PUTTFM(ID,UROTZZ)
         OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'P').NE.0)THEN
         IF(MATCH(SWITCHES,'LAB'))FUNKY=IOR(FUNKY,FLABROT)
         CALL PUTTFM(ID,UAROTX)
         OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'Q').NE.0)THEN
         IF(MATCH(SWITCHES,'LAB'))FUNKY=IOR(FUNKY,FLABROT)
         CALL PUTTFM(ID,UAROTY)
         OK=.TRUE.
      END IF
C
      IF(INDEX(AXIS,'R').NE.0)THEN
         IF(MATCH(SWITCHES,'LAB'))FUNKY=IOR(FUNKY,FLABROT)
         CALL PUTTFM(ID,UAROTZ)
         OK=.TRUE.
      END IF
C
C        IF NO VALID SPECIFIER WAS GIVEN, LET THE USER KNOW
C
      IF(.NOT.OK) THEN
         POINT=VPOINT
         NPOINT=MPOINT
         CALL TELLUSER(AXISERR,*999)
      END IF
C
C        IF THERE ARE MORE ARGS, GO GET THEM.
C
      IF(POINT.NE.0)GOTO 10
C
C        MODIFY THE SUMMARY WORDS IN FNUSE, DEVUSE, ETC.
C
      CALL ORTOTAL(ID)
C
 999      RETURN
C
      END
