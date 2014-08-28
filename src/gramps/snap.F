C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE SNAP(ARG, SWITCHES)
C
      IMPLICIT NONE
      CHARACTER ARG*(*), SWITCHES*(*)
#include "UDLIST.FOR"
#include "ERRORS.FOR"
#include "FILES.FOR"
C
      CHARACTER*80 FILE
      CHARACTER*80 fname
      integer i, vpoint
      LOGICAL ORDER
      EXTERNAL TRIM
C
      If(MATCH(SWITCHES,'NOADVANCE'))THEN
        SNAPPING=.FALSE.
        GOTO 999
      End If
C
      POINT=1
      VPOINT=1
      CALL GETSTR(ARG,POINT,FILE)
      IF(ERROR.NE.0)CALL TELLUSER(ERROR, *999)
      DEFEXT='jpg'
C        openfile uses global DEFEXT and modifies global fullfile
      If(MATCH(SWITCHES,'ADVANCE'))THEN
        ERROR=OPENFILE(file,PFILE,.FALSE.,'PARSE')
        SNAPPING=.TRUE.
        SNAPFILE=fullfile
        call mkfname(fname, snapfile, fcount)
        ERROR=OPENFILE(fname,PFILE,.TRUE.,' ')
      Else
        ERROR=OPENFILE(file,PFILE,.TRUE.,' ')
        fname = fullfile
      End If
      IF(ERROR.NE.0)THEN
        POINT=VPOINT
        CALL TELLUSER(ERROR,*999)
      END IF
#ifdef __APPLE__
      CLOSE(UNIT=PFILE)
#elif GFORTRAN
      CLOSE(UNIT=PFILE)
#else
C      CLOSE(UNIT=PFILE, DISPOSE='DELETE')
      CLOSE(UNIT=PFILE)
#endif
C
C      If(SWITCHES.EQ.' ' .OR. MATCH(SWITCHES,'IPASTE'))THEN
      If(OUTTING.NE.OUTWINDOW)Call Telluser(NOTDEV,*999)
      call savewin(fname, trim(fname))
C      Else If(MATCH(SWITCHES,'ADVANCE'))THEN
C       i=TRIM(fullfile)
C       SNAPFILE=fullfile(:i-4)
C       SNAPFILE=fullfile
C      ELSE
C        ORDER=MATCH(SWITCHES,'YX')
C        i=TRIM(fullfile)
C        i= fcopen(fullfile, i)
C        CALL G0DUMP(ORDER)
C        i= fcclose()
C      END IF
C
 999  CONTINUE
      RETURN
      END
