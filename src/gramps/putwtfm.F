C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE PUTWTFM(NID,TFMTYP)
C
      IMPLICIT NONE
	INTEGER NID, TFMTYP
C
      REAL DELTA
C
	Real SCALE
      REAL LOWER, UPPER
	Integer PUTFUNK
      LOGICAL ADDLOW,ADDUPP
      EQUIVALENCE (VAL,LOWER),(STEP,UPPER)
      EQUIVALENCE (ADDIT,ADDLOW),(ADDSTEP,ADDUPP)
#include "UDLIST.FOR"
#include "DEFVAL.FOR"
C
C       THIS ROUTINE IS STRICTLY ANALOGOUS TO PUTTFM, EXCEPT
C       THAT IS AFFECTS THE ARRAYS WHICH APPLY ONLY TO THE
C       WORLDS AND THEIR UNIQUE TFMS (PERSPECTIVE AND MOVE)
C
	PUTFUNK=FUNKY
      IF(FUNKY.EQ.FNEGDEV)GOTO 99
        SCALE=1./WUNITS(TFMTYP)
C
      IF(FUNKY.EQ.0)THEN
        IF(.NOT.GOTVAL)GOTO 99
        IF(ADDIT)THEN
          WLDVAL(TFMTYP,NID)=
     &      WLDVAL(TFMTYP,NID)+SCALE*VAL
        ELSE
          WLDVAL(TFMTYP,NID)=SCALE*VAL
        END IF
C       CALL WSHOW(NID,TFMTYP)
        GOTO 99
      END IF
C
      IF(FUNKY.EQ.FADDINC)THEN
        IF(.NOT.GOTVAL)GOTO 99
        IF(ADDIT)THEN
          WLDINC(TFMTYP,NID)=
     &      WLDINC(TFMTYP,NID)+SCALE*VAL
        ELSE
          WLDINC(TFMTYP,NID)=SCALE*VAL
        END IF
        GOTO 99
      END IF
C
      IF( BTEST(FUNKY,BLIMIT) )THEN
        WLDINC(TFMTYP,ID)=STEP
        IF(ADDIT)THEN
          BOUNDW(TFMTYP,ID)=WLDVAL(TFMTYP,ID) + SCALE*VAL
        ELSE
          IF(WLDVAL(TFMTYP,ID).EQ.SCALE*VAL)THEN
            PUTFUNK=0
            WLDINC(TFMTYP,ID)=0
            GOTO 99
          ELSE
            BOUNDW(TFMTYP,ID)=SCALE*VAL
          END IF
        END IF
        IF( BTEST(FUNKY,BSIN) )THEN
          DELTA = BOUNDW(TFMTYP,ID)
          AMPLW(TFMTYP,ID) = ( DELTA - WLDVAL(TFMTYP,ID) )/2
          WLDANG(TFMTYP,ID)=0.0
        END IF
        GOTO 99
      END IF
C
      IF( BTEST(FUNKY,BBOUNDED) )THEN
        IF(ADDLOW)THEN
          LWBOUND(TFMTYP,ID)=WLDVAL(TFMTYP,ID) + SCALE*LOWER
        ELSE
          LWBOUND(TFMTYP,ID)=SCALE*LOWER
        END IF
        IF(ADDUPP)THEN
          UWBOUND(TFMTYP,ID)=WLDVAL(TFMTYP,ID) + SCALE*UPPER
        ELSE
          UWBOUND(TFMTYP,ID)=SCALE*UPPER
        END IF
        GOTO 99
      END IF
C
 99   DEVWLD(TFMTYP,NID)=DEV
      FNWLD(TFMTYP,NID)=PUTFUNK
      DWFACT(TFMTYP,NID)=FACTOR
C
      DCHANGE=.TRUE.
      RETURN
      END
