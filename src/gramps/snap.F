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
      integer i, vpoint, fnamelen
      LOGICAL ORDER
      EXTERNAL TRIM
C
      If(MATCH(SWITCHES,'NOADVANCE')) Then
c  find mpeg process if necessary
        Call close_ffmpeg()
        SNAPPING=.FALSE.
        snaptype = ""
        GOTO 999
      End If
C
      POINT=1
      VPOINT=1
      CALL GETSTR(ARG,POINT,FILE)
      IF(ERROR.NE.0)CALL TELLUSER(ERROR, *999)
      DEFEXT='png'
C        openfile uses global DEFEXT and modifies global FULLFILE, DIR, NAME and REST (extension)
      If(MATCH(SWITCHES,'ADVANCE'))THEN
        ERROR=OPENFILE(file,PFILE,.FALSE.,'PARSE')
        SNAPPING=.TRUE.
        snaptype = REST
        If (snaptype .EQ. 'mpeg' .OR. snaptype .EQ. 'mp4') Then
          SNAPFILE=fullfile
        Else
          SNAPFILE = DIR(1:TRIM(DIR)) // '/'
     &           // NAME(1:TRIM(NAME)) // '%08d.' // REST
        End If
      Else
C        openfile uses global DEFEXT and modifies global FULLFILE and global REST
C        just parse/create fullfile.  no actual open/create
        ERROR=OPENFILE(file,PFILE,.FALSE.,'PARSE')
c       fname = fullfile
        SNAPFILE=fullfile
        snaptype = REST
      End If
      IF(ERROR.NE.0)THEN
        POINT=VPOINT
        CALL TELLUSER(ERROR,*999)
      END IF
C
      If(OUTTING.NE.OUTWINDOW)Call Telluser(NOTDEV,*999)

      If (SNAPPING) Then
        Call savewin(snapfile, TRIM(snapfile), snaptype, FCOUNT)
      Else
        Call savewin(snapfile, TRIM(snapfile), snaptype, -1)
        Call close_ffmpeg()
      End If
 999  CONTINUE
      RETURN
      END
