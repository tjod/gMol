C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE TELLUSER(ERROUT,*)
      ENTRY      TELLUSER1(ERROUT)
C
      IMPLICIT NONE
      INTEGER ERROUT
C
#include "UDLIST.FOR"
      EXTERNAL TRIM
      CHARACTER BEEP*1
      INTEGER RMSSTS,RMSSTV,CONDVAL,FNUM,IUNIT
      INTEGER NSHIFT, NARROWS, PRSIZE, NBLANKS
      CHARACTER*120 COMMAND
      CHARACTER*120 TMPLINE
      COMMON /CMDLINE/COMMAND
      SAVE   /CMDLINE/
      CHARACTER*40 BLANKS,ARROWS
      LOGICAL SYNERR
      INTEGER CEND, I
#include "ERRORS.FOR"
#include "ERRTXT.FOR"
      DATA BLANKS/'                                        '/
      DATA ARROWS/'^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^'/
C     DATA BEEP/o'7'/
C  LENGTH OF "GRAMPS>" PROMPT
C      DATA PRSIZE/7/
C
      BEEP=CHAR(7)
      If (CMDFROM .EQ. QT_OUT) Then
        PRSIZE=0
      Else
        PRSIZE=7
      End If
      BLANKS='                                        '
      ARROWS='^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^'
C
C   THIS IS A GRAMPS UTILITY SUBROUTINE USED TO WRITE
C   AN ERROR MESSAGE TO THE TERMINAL.  THE MESSAGE IS
C   ACCOMPANIED BY A BEEP, UNLESS WE'RE FILMING - A
C   BEEP WHILE FILMING WOULD ADVANCE THE CAMERA!
C
C   IF ERROR REPORTING IS TURNED OFF, DON'T WRITE ANYTHING
C
      IF(.NOT.REPORT)GOTO 999
C
C   MAKE SURE THERE ARE NO WEIRD ERROR NUMBERS
C   ERROR NUMBERS ARE NEGATIVE IF THEY DO NOT
C   ARISE FROM SYNTAX ERRORS.  SET THE FLAG SYNERR
C   FOR LATER USE.
C
      IF(ERROUT.LT.0)THEN
        SYNERR=.FALSE.
        ERROR=IABS(ERROUT)
      ELSE
        ERROR=ERROUT
        SYNERR=.TRUE.
      END IF
      IF( (ERROUT.GT.LASTERROR).OR.(ERROUT.EQ.0) )THEN
        ERROR=LASTERROR
        SYNERR=.FALSE.
      END IF
C
C   IF ERROR IS NEVER, GRAMPS HAS SIGNALLED THAT SOMETHING
C   HAS OCCURRED THAT WAS NOT EXPECTED AND IT CANNOT DEAL
C   WITH IT, I.E. THERE'S A BUG IN GRAMPS.  DO THE
C   DIVISION TO FORCE A FORTRAN ERROR AND TRACEBACK AND DIE.
C
      IF(ERROR.EQ.NEVER)THEN
C    CALL abort()
      END IF
C
C   POINT AT THE OFFENDING SPOT WITH ARROWS.
C   ECHO THE COMMAND IF WE'RE DOING A GIN FILE AND THE USER
C   DOES NOT HAVE VERIFY OR ECHO ON.
C   WRITE OUT THE ARROWS IF THIS IS A SYNTAX ERROR
C
      CEND=TRIM(COMMAND)
      NBLANKS=(ARGSTART-1)+(POINT-1)
      NARROWS=NPOINT-POINT
      IF(NARROWS.LE.0)NARROWS=1
C
      IF( (POINT.LE.0).OR.(NPOINT.EQ.0) )THEN
        NBLANKS=CEND
        NARROWS=5
      END IF
C
      NSHIFT=5*(AGF-2)
      IF(NSHIFT.LT.0)NSHIFT=0
C
      IF(DOING)THEN
        IF(.NOT.VERIFY)THEN
          IF(NSHIFT.LE.0)THEN
            CALL GROUT('!'//COMMAND(:CEND))
          ELSE
            CALL GROUT('!'//BLANKS(:NSHIFT)//COMMAND(:CEND))
          END IF
        END IF
      END IF
C
      IF(SYNERR)THEN
        IF( (.NOT.DOING) .AND. (.NOT.ECHO) )NBLANKS=NBLANKS+PRSIZE
        NBLANKS=NBLANKS+NSHIFT
        IF(NBLANKS.LE.0)THEN
          CALL GROUT('^'//ARROWS(:NARROWS))
        ELSE
          CALL GROUT('^'//BLANKS(:NBLANKS)//ARROWS(:NARROWS))
        END IF
      END IF
C
C   PRINT OUT THE MESSAGE WITH A BEEP, UNLESS WE'RE FILMING
C
      IF(FILMPAUSE .OR. CMDFROM .EQ. QT_OUT)THEN
        WRITE(TMPLINE,2000)ERROR,ERRTXT(ERROR)
      ELSE
        WRITE(TMPLINE,2000)ERROR,BEEP,ERRTXT(ERROR)
      END IF
      CALL GROUT(TMPLINE)
C  I= SPEAK(ERROR)
C
C   IF REQUESTED, FIND OUT AND REPORT MORE ABOUT THE ERROR
C
      IF(FULLERR)THEN
C    CALL ERRSNS(FNUM,RMSSTS,RMSSTV,IUNIT,CONDVAL)
        IF(FNUM.NE.0)THEN
         WRITE(TMPLINE,6000)FNUM,RMSSTS,RMSSTV,IUNIT,CONDVAL
         CALL GROUT(TMPLINE)
        END IF
      END IF
 6000 FORMAT('FORTRAN error #',I6,' RMS #',Z8,'/',Z8,
     &  ' UNIT # ',I3,' condition value ',Z8)
C
C
C   AND SET TO NOT COLLECT THIS LINE WHICH CAUSED AN ERROR.
C
 999  SKIPCOLL=.TRUE.
C
C   CLEAR OUT THE ERROR NUMBER SO NO OTHER ERROR IS
C   REPORTED BECAUSE OF THE ERROR JUST REPORTED
C
      ERROR=0
C
C   THERE IS ONLY ONE RETURN POINT, BUT IT MAKES IT
C   NEATER FOR THE CALLING PROGRAM TO GET OUT OF WHAT
C   IT WAS DOING (AND RETURN PRESUMABLY).  LOOK AT
C   A CALL OF TELLUSER (E.G. IN ROTATE) AND YOU'LL SEE
C   WHAT I MEAN.
C
      RETURN 1
C
 2000 FORMAT('?',I2,': ',A,A)
      END
