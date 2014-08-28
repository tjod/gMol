C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE RESTART(ARG,SWITCHES)
C
      IMPLICIT NONE
      CHARACTER ARG*(*)
      CHARACTER SWITCHES*(*)
      INTEGER OUTSAVE
      REAL LA, RA
C
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
C       THIS ROUTINE RESTARTS THE PICTURE SYSTEM BY
C       STOPPING IT AND RE-INITIALIZING EVERYTHING.
C       IF AN ARGUMENT IS GIVEN, DON'T RESTART. I ASSUME
C       THE USER MEANT TO RESET AND OBJECT AND MISTYPED
C       RESET (I'VE DONE IT AND WAS FRUSTRATED!)
C
C       THE /DISPLAY SWITCH WILL ONLY REBUILD THE SYSTEM
C       WITHOUT AFFECTING THE UDL STUFF.  CAN BE USED WHEN
C       THE SYSTEM FREAKS OUT, WHICH IT SOMETIMES DOES
C       EVEN WHEN GRAMPS IS STILL HEALTHY AND RUNNING.
C
      IF(.NOT.MATCH(SWITCHES,'DISPLAY'))THEN
        IF(SWITCHES.NE.' ')CALL TELLUSER(BADSWITCH,*999)
        IF(ARG.NE.' ')CALL TELLUSER(NOARGS,*999)
        LA = WLDVAL(WWINDA,LWORLD)
        RA = WLDVAL(WWINDA,RWORLD)
        OUTSAVE=OUTTING
        CALL UDINIT
        Call g0pinit
        Call startup
        OUTTING=OUTSAVE
        WLDVAL(WWINDA,LWORLD) = LA
        WLDVAL(WWINDA,RWORLD) = RA
        RETURN
      END IF
C
 999  RETURN
      END
