C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      Subroutine ADVANCE(ARG,SWITCHES)
C
      IMPLICIT NONE
C
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C#include "DEVICES.FOR"
C
      CHARACTER ARG*(*)
      CHARACTER SWITCHES*(*)
      CHARACTER*1 BEEP,BLANK
      CHARACTER*15 ASTEP
      INTEGER APOINT
      Character*255 fname
      LOGICAL NOUPD,NOCLR
      INTEGER ZERO, MINUSONE, NADV, I
      EXTERNAL TRIM
      DATA ZERO/0/,MINUSONE/-1/
      DATA BEEP/''/
      DATA BLANK/' '/
C
      ZERO=0
      MINUSONE=-1
      BEEP=CHAR(7)
      BLANK=' '
C
C   THIS SUBROUTINE ADVANCES THE DISPLAY CYCLE A SPECIFIED
C   NUMBER OF TIMES AND TRIGGERS A CAMERA, IF NECESSARY.
C   FIRST, THERE NEEDS TO BE AN OUTPUT SPECIFIED.
C
C     IF(OUTTING.EQ.0)CALL TELLUSER(NOOUT,*999)
C
C   PICK OUT THE NUMBER TO ADVANCE
C
      POINT=1
      IF(ARG.EQ.BLANK)THEN
        NADV=1
      ELSE
        CALL GETSTR(ARG,POINT,ASTEP)
        IF(ERROR.NE.0)CALL TELLUSER(ERROR,*999)
        If (ASTEP.EQ.BLANK) Then
          NADV=1
        Else
          POINT=1
          IF(ASTEP(1:1) .EQ. "S" .OR. ASTEP(1:1) .EQ. "s") Then
            APOINT=2
          Else
            APOINT=1
          End If
          CALL GETVLU(ASTEP,APOINT,NADV,ADDIT)
          IF(ERROR.NE.0)CALL TELLUSER(ERROR,*999)
        End If
      END IF
      IF(MATCH(SWITCHES,'NOUPDATE'))THEN
        NOUPD=.TRUE.
      ELSE
        NOUPD=.FALSE.
      END IF
      NOCLR = .NOT.AUTOCLEAR
      IF(MATCH(SWITCHES,'NOCLEAR'))THEN
        NOCLR=.TRUE.
      ELSE
        NOCLR=.FALSE.
      END IF
      IF(MATCH(SWITCHES,'CLEAR'))THEN
        NOCLR=.FALSE.
      ELSE
        NOCLR=.TRUE.
      END IF
C
C   A NUMBER OF ZERO OR LESS IS IGNORED
      IF(NADV.LE.0)RETURN
C
C global for contupd
      NADVANCE = NADV
C
C the event loop in gramps.c or timer in qt
C  will call contupd and redisplay when it can.
      if (agf.gt.1)call settimer(1)
      return

C   THIS COMMAND ALLOWS USER CONTROL OF THE CALL OF THE
C   DISPLAY SUBROUTINE.  DO ALL CONT UPDATES, IF NECESSARY
C   BEFORE DISPLAYING.
C
      DO 10 I=1,NADV
C  IF(PENSTOP)CALL PSTOP
      IF(.NOT.SKIPDISP .AND. OUTTING.GT.0)THEN
        IF( CONT.AND.(.NOT.NOUPD) )CALL CONTUPD(1)
        DCHANGE=.TRUE.
        CALL G0REDRAW(1,1)
        If (AGF.GT.1) Then
          call updategl
C if we're doing a file, we gotta go on
        Else
        End If
      ELSE
        IF( CONT.AND.(.NOT.NOUPD) )CALL CONTUPD(1)
        IF(OUTTING.LT.0)THEN
          DCHANGE=.TRUE.
          CALL DISPLAY(0)
        END IF
      END IF
C
      If(SNAPPING)Then
       call mkfname(fname, SNAPFILE, fcount)
       call savewin(fname, TRIM(fname))
      End If
      IF(OUTTING.EQ.OUTQIX)WRITE(OFILE)MINUSONE,ZERO,ZERO
      IF(OUTTING.EQ.OUTPIX)WRITE(OFILE,6)MINUSONE,ZERO,ZERO
      IF(OUTTING.EQ.OUTPOSTSCR)WRITE(OFILE,'(a)')'showpage'
      IF(FPAUSE.GT.0)CALL G0FPAUSE(FPAUSE)
      FCOUNT=FCOUNT+1
 10   Continue
c  IF(OUTTING.GT.0)CALL G0ALPHA
C
 6    FORMAT(I7,I7,I7,I7)
C
 999  RETURN
      END
C
      Subroutine ADVSNAP
C
C Entry point for gramps main idle loop to allow for snapshot
C
      IMPLICIT NONE
C
#include "UDLIST.FOR"
      Character*255 fname
      External TRIM
C
      FCOUNT=FCOUNT+1
      If(SNAPPING)Then
       call mkfname(fname, SNAPFILE, fcount)
       call savewin(fname, TRIM(fname))
      End If
      End

      Subroutine mkfname(fname, snapfile, fcount)
C
      Implicit None
      Character*(*) fname, snapfile
      Integer fcount
      Integer TRIM
      External TRIM
      Integer rindex
      External rindex
      Character*20 ext
      Character*80 file
      Integer dot
C
      dot = rindex(snapfile,".")
      file = snapfile(:dot-1)
      ext = snapfile(dot+1:)
      Write(fname, '(a,i8.8,a)')FILE(:TRIM(FILE)), fcount,
     &   "."//EXT(:TRIM(EXT))

C      If(fcount .lt. 10)Then
C       Write(fname, '(a,i1,a)')FILE(:TRIM(FILE)), fcount,
C     &   "."//EXT(:TRIM(EXT))
C      Else If(fcount .lt. 100)Then
C       Write(fname, '(a,i2,a)')FILE(:TRIM(FILE)), fcount,
C     &   "."//EXT(:TRIM(EXT))
C      Else If(fcount .lt. 1000)Then
C       Write(fname, '(a,i3,a)')FILE(:TRIM(FILE)), fcount,
C     &   "."//EXT(:TRIM(EXT))
C      Else If(fcount .lt. 10000)Then
C       Write(fname, '(a,i4,a)')FILE(:TRIM(FILE)), fcount,
C     &   "."//EXT(:TRIM(EXT))
C      Else If(fcount .lt. 100000)Then
C       Write(fname, '(a,i5,a)')FILE(:TRIM(FILE)), fcount,
C     &   "."//EXT(:TRIM(EXT))
C      Else If(fcount .lt. 1000000)Then
C       Write(fname, '(a,i6,a)')FILE(:TRIM(FILE)), fcount,
C     &   "."//EXT(:TRIM(EXT))
C      Else If(fcount .lt. 1000000)Then
C       Write(fname, '(a,i7,a)')FILE(:TRIM(FILE)), fcount,
C     &   "."//EXT(:TRIM(EXT))
C      Else If(fcount .lt. 10000000)Then
C       Write(fname, '(a,i8,a)')FILE(:TRIM(FILE)), fcount,
C     &   "."//EXT(:TRIM(EXT))
C      End If
C
      Return
      End
