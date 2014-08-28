C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE GROUT(TMPLINE)
C
      IMPLICIT NONE
#include "UDLIST.FOR"
      CHARACTER*(*) TMPLINE
      CHARACTER*200 OUTLINE, TTMP
      CHARACTER*1 FIRST, BLANK
      INTEGER IEND
      EXTERNAL TRIM
      Integer zi, I
      Character*12 ZERO
      DATA ZERO/'0.000000E+00'/
C
      BLANK=' '
      zi=INDEX(TMPLINE,ZERO)
      Do While(zi.ne.0)
        TTMP=TMPLINE(1:zi-1)//' 0.0 '//TMPLINE(zi+12:)
        TMPLINE=TTMP
        zi=INDEX(TMPLINE,ZERO)
      End Do
      IEND=TRIM(TMPLINE)
      FIRST=TMPLINE(1:1)
      IF(FIRST.EQ.'>' .OR. FIRST.EQ. '?' .OR.
     &     FIRST.EQ.'^' .OR. FIRST.EQ. '!')THEN
        OUTLINE=TMPLINE
      ELSE
        OUTLINE=' '//TMPLINE
        IEND=IEND+1
      END IF
      If(REROUTE.EQ.ACTIVE)Then
        Write(RFILE,'(A)')OUTLINE(:IEND)
      End If
      IF(CMDFROM.EQ.STANDARD_OUT)Then
        I= CWRITE( OUTLINE(1:IEND) )
#ifdef QT
      ELSE IF(CMDFROM.EQ.QT_OUT)Then
        I= QWRITE( OUTLINE(1:IEND) )
#endif
#ifdef USE_SOCK
      ELSE IF (CMDFROM.EQ.SOCKET_OUT)Then
        I= SOCKSEND( OUTLINE(1:IEND) )
#endif
      ELSE IF (CMDFROM.EQ.COMMAND_OUT)Then
        Call COMMOUT( OUTLINE(1:IEND) )
      ELSE
        WRITE(0,'("!",A)') OUTLINE(1:IEND)
      END IF
      RETURN
C
      END
C
      Subroutine setfrom(flag)
C
        Implicit None
#include "UDLIST.FOR"
      Integer flag
C
      CMDFROM = flag
      Return
      End
