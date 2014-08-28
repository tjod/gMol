C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      INTEGER FUNCTION SPAN(STRING,FOR)
C
      IMPLICIT NONE
      CHARACTER STRING*(*),FOR*1
      INTEGER ILENGTH, I
C
C   THIS FUNCTION RETURNS THE NUMBER OF CONSECUTIVE
C   "FOR" CHARACTERS THAT THERE ARE IN THE STRING BEFORE
C   IT ENCOUNTERS A CHARACTER WHICH IS NOT THE "FOR"
C   CHARACTER.  IT IS USEFUL FOR SKIPPING OVER BLANKS.
C
      ILENGTH=LEN(STRING)
      DO 10 I=1,ILENGTH
      IF(STRING(I:I).NE.FOR)THEN
        SPAN=I-1
        RETURN
      END IF
 10   CONTINUE
C
      SPAN=ILENGTH
      RETURN
      END
C
C
      INTEGER FUNCTION TRIM(STRING)
C
      IMPLICIT NONE
      CHARACTER STRING*(*)
      INTEGER I
C
      DO 10 I=LEN(STRING),1,-1
      IF(STRING(I:I).NE.' ')THEN
        TRIM=I
        RETURN
      END IF
 10   CONTINUE
C
      TRIM=0
      RETURN
      END
C
      INTEGER FUNCTION RINDEX(STRING, CHAR1)
C
      IMPLICIT NONE
      CHARACTER STRING*(*)
      CHARACTER CHAR1*1
      INTEGER I
C
C Reverse index: finds last occurrence of char1 in string
C
      DO 10 I=LEN(STRING),1,-1
      IF(STRING(I:I).EQ.CHAR1)THEN
        RINDEX=I
        RETURN
      END IF
 10   CONTINUE
C
      RINDEX=0
      RETURN
      END
C
C
      LOGICAL FUNCTION MATCH(ABBREV,FULL)
C
      IMPLICIT NONE
C
      CHARACTER ABBREV*(*),FULL*(*)
      INTEGER SPOT, NEXTSPOT, ENDSPOT, EQSPOT
      COMMON /SLSPOT/ SPOT,EQSPOT,ENDSPOT
      CHARACTER*80 STMP
      INTEGER SLEN
C
      NEXTSPOT=INDEX(ABBREV,'/')
      SPOT=0
      MATCH=.FALSE.
C
 10   IF(NEXTSPOT.NE.0)THEN
        SPOT=SPOT+NEXTSPOT
        NEXTSPOT=INDEX(ABBREV(SPOT+1:),'/')
        IF(NEXTSPOT.EQ.0)THEN
          ENDSPOT=INDEX(ABBREV,' ')
          IF(ENDSPOT.EQ.0)THEN
            ENDSPOT=LEN(ABBREV)
          ELSE
            ENDSPOT=ENDSPOT-1
          END IF
        ELSE
          ENDSPOT=NEXTSPOT+SPOT-1
        END IF
        IF(ENDSPOT.GT.SPOT)THEN
          EQSPOT=INDEX(ABBREV(SPOT:ENDSPOT),'=')
          IF(EQSPOT.EQ.0)THEN
            STMP=ABBREV(SPOT+1:ENDSPOT)
            CALL UPCASE(STMP)
C       SLEN=ENDSPOT-(SPOT+1)+1
            SLEN=ENDSPOT- SPOT
            IF( FULL(1:SLEN).EQ. STMP(1:SLEN) )THEN
              MATCH=.TRUE.
              RETURN
            END IF
          ELSE
            STMP=ABBREV(SPOT+1:SPOT+EQSPOT-2)
            CALL UPCASE(STMP)
C       SLEN=(SPOT+EQSPOT-2)-(SPOT+1)+1
            SLEN=      EQSPOT-2
            IF( FULL(1:SLEN).EQ. STMP(1:SLEN) )THEN
              MATCH=.TRUE.
              RETURN
            END IF
          END IF
        END IF
        GOTO 10
      END IF
C
      SPOT=0
      RETURN
      END
C
C
      SUBROUTINE SMATCH(ABBREV,FULL,VALUE,PRESENT)
C
      IMPLICIT NONE
      CHARACTER ABBREV*(*),FULL*(*),VALUE*(*)
      CHARACTER*1 BLANK
      INTEGER SPOINT
      LOGICAL PRESENT,MATCH
C
      INTEGER SPOT, NEXTSPOT, ENDSPOT, EQSPOT
      COMMON /SLSPOT/ SPOT,EQSPOT,ENDSPOT
      DATA BLANK/' '/
      BLANK=' '
C
      IF(.NOT.MATCH(ABBREV,FULL))THEN
        VALUE=BLANK
        PRESENT=.FALSE.
      ELSE
        PRESENT=.TRUE.
        IF(EQSPOT.EQ.0)THEN
          VALUE=BLANK
        ELSE
C     VALUE=(ABBREV(SPOT+EQSPOT:ENDSPOT))
          SPOINT=1
          CALL GETSTR(ABBREV(SPOT+EQSPOT:ENDSPOT),SPOINT,VALUE)
        END IF
      END IF
C
      RETURN
      END
C
C
      SUBROUTINE LOCASE(STR)
C
      IMPLICIT NONE
C
      CHARACTER STR*(*)
      INTEGER I, TRIM, ACODE
      EXTERNAL TRIM
C
      DO 10 I=1,TRIM(STR)
      ACODE=ICHAR(STR(I:I))
      IF( (ACODE.GE.ICHAR('A')).AND.(ACODE.LE.ICHAR('Z')) )
     &  STR(I:I)=CHAR(ACODE+32)
 10   CONTINUE
C
      RETURN
      END
C
      SUBROUTINE UPCASE(STR)
C
      IMPLICIT NONE
C
      CHARACTER STR*(*)
      INTEGER I, ACODE, TRIM
          EXTERNAL TRIM
C  DATA SMALLA/'a'/,SMALLZ/'z'/
C
      DO 10 I=1,TRIM(STR)
      ACODE=ICHAR(STR(I:I))
      IF( (ACODE.GE.ICHAR('a')).AND.(ACODE.LE.ICHAR('z')) )
     &  STR(I:I)=CHAR(ACODE-32)
 10   CONTINUE
C
      RETURN
      END
C
      LOGICAL FUNCTION SCMP(A, B)
C
      CHARACTER *(*) A, B
      INTEGER TRIM
          EXTERNAL TRIM
      INTEGER CDIFF, i, CLEN
C
C   COMPARE A and B without case sensitivity.
C   Only compare up to shorter of A, B to allow for abbreviations.
C
      SCMP=.TRUE.
      IF(A.EQ.B)RETURN
C
C     CLEN=MIN(TRIM(A), TRIM(B))
      CLEN=TRIM(A)
      IF( A(1:CLEN).EQ.B(1:CLEN) )RETURN
C
      DO 10 i=1, CLEN
      CDIFF=ABS( ICHAR(A(i:i)) - ICHAR(B(i:i)) )
      IF(CDIFF.NE.0 .AND. CDIFF.NE.32)THEN
         SCMP=.FALSE.
         RETURN
      END IF
 10   CONTINUE
C
      RETURN
      END
C
      Integer Function xblank(string)
C
      Implicit None
      Character*(*) string
C
      Integer i,iend
      Integer TMPSIZ
      Parameter (TMPSIZ=120)
      Character*(TMPSIZ) xtmp
      Integer xend
      Integer TRIM
      EXTERNAL TRIM
C
C  Function to remove all blanks from string
C  Returns final length of string
C
      iend=TRIM(string)
      xend=0
      xtmp=' '
      Do i=1,iend
      If(string(i:i).ne.' ')Then
        xend=xend+1
        If(xend.gt.TMPSIZ)Then
          xblank=-TMPSIZ
          Return
        End If
        xtmp(xend:xend)=string(i:i)
      End If
      End Do
C
      string=xtmp
      xblank=xend
      Return
      End
