C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE DEASSIGN(ARG,SWITCHES)
C
      IMPLICIT NONE
C
#include "UDLIST.FOR"
#include "ERRORS.FOR"
      INTEGER I, J
C
C       THIS ROUTINE MODIFIES THE DEV ASSIGNMENTS OF EACH
C       OBJECT TO ENTIRELY DEASSIGN A DEVICE.  
C
C       GET THE DEVICE AND THE BIT MASK ASSOCIATED WITH IT
C
      CHARACTER ARG*(*)
      CHARACTER SWITCHES*(*)
C
C       THE /ALL SWITCH DESSIGNS ALL (BUT THE WORLDS') DEVICES
C
      IF(MATCH(SWITCHES,'ALL'))THEN
        IF(NOBJ.LE.RWORLD)GOTO 999
        Do  I=RWORLD+1,NOBJ
          Do J=1,SUMMARY
            IF(DEVUSE(J,I) .NE. 0) Then
              DEV = DEVUSE(J,I)
              DEVUSE(J,I)=0
              call setdevval(dev,0,0)
            End If
          End Do
        End Do
        GOTO 999
      END IF
C
      POINT=1
 5    CALL GETDEV(ARG,POINT,DEV)
      IF(ERROR.NE.0)CALL TELLUSER(ERROR,*999)
C
C       EXAMINE EACH OBJECT.  IF THE OBJECT  IS NOT USING
C       THE DEV IN QUESTION, GO ON TO THE NEXT OBJ.  IF IT
C       IS USING THE DEV, FIND OUT IN WHICH TFM(S) AND REMOVE
C       THE BIT ASSIGNED TO THE DEVICE IN QUESTION.
C
      DO 20 I=1,NOBJ
      IF(IAND(DEV,DEVUSE(SUMMARY,I)).EQ.0)GOTO 20
C
      DEVUSE(SUMMARY,I)=IEOR(DEVUSE(SUMMARY,I),DEV)
      DO  10 J=1,MTFM
      IF( IAND(DEV,DEVUSE(J,I)).NE.0) Then
        DEVUSE(J,I)=0
      End If
 10   CONTINUE
C
C       IF THE WORLDS ARE UNDER SCRUTINY, CHECK THE TFMS UNIQUE
C       TO THE WORLDS (PERSPECTIVE AND MOVE).
C
      IF(I.LE.RWORLD)THEN
        DEVWLD(WSUMMRY,I)=IEOR(DEVWLD(WSUMMRY,I),DEV)
        DO  15 J=1,WTFM
        IF( IAND(DEV,DEVWLD(J,I)).NE.0) DEVWLD(J,I)=0
 15     CONTINUE
      END IF
C
 20   CONTINUE
      call setdevval(dev,0,0);
C
      IF(POINT.NE.0)GOTO 5
 999  RETURN
      END
