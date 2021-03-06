C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
	SUBROUTINE ARGSUBS(SUBLINE)
C
	IMPLICIT NONE
C
#include "UDLIST.FOR"
#include "ERRORS.FOR"
#include "DOLLARGS.FOR"
C
	CHARACTER SUBLINE*(*)
	CHARACTER*1 COMMA,BLANK,DOLLAR
	INTEGER SKIP, SUBLEN, SPOT, NLEN, RLEN, ARGNUM, QLEN
	EXTERNAL TRIM
	DATA COMMA/','/,BLANK/' '/,DOLLAR/'$'/
	COMMA=','
	BLANK=' '
	DOLLAR='$'
C
C	 THIS IS A GRAMPS UTILITY ROUTINE.
C	 IT IS USED TO FIND OCCURENCES OF PARAMTER NAMES
C	 (E.G. $1 OR $2) AND REPLACE THESE OCCURRENCES
C	 WITH STRINGS SUPPLIED BY THE SUBLINE WHICH
C	 INITIATED THE DOING OF THE CURRENT .GIN FILE.
C
C	 CHECK FOR THE LACK OF ANY $ARGS.
C
	SKIP=0
	SUBLEN=LEN(SUBLINE)
C
 10   CONTINUE
	IF(SKIP.GE.SUBLEN)GOTO 999
	SPOT=INDEX(SUBLINE(SKIP+1:),DOLLAR)
	IF(SPOT.EQ.0)GOTO 999
	SPOT=SPOT+SKIP
C
C	 NOW THAT THE $ SPOT IS KNOWN, LOOK FOR A
C	 NUMBER AFTER THE $.  THE END OF THE NUMBER IS
C	 DELIMITED BY ANOTHER $ OR BY THE END OF LINE.
C	 NLEN IS THE LENGTH OF THE NUMBER NAMING THE $ARG.
C	 RLEN IS THE LENGTH OF THE $ARG STRING ITSELF, THAT IS
C	 THE LENGTH OF THE STRING TO BE REPLACED.
C
	NLEN=INDEX( SUBLINE(SPOT+1:),DOLLAR )
	IF(NLEN.EQ.0)THEN
	  NLEN=TRIM(SUBLINE)-SPOT
	  RLEN=NLEN+1
	ELSE
	  RLEN=NLEN+1
	  NLEN=NLEN-1
	END IF
C
C	 TRANSLATE INTEGER FROM ASCII TO BINARY
C
C     READ(SUBLINE(SPOT+1:),1000,ERR=998)ARGNUM
	ARGNUM=A2I(SUBLINE(SPOT+1:SPOT+NLEN),ERROR)
	IF(ERROR.NE.0)GOTO 998
	IF( (ARGNUM.LE.0).OR.(ARGNUM.GT.ARGMAX) )GOTO 998
C
C	 REPLACE THE $ARG WITH THE STRING CORREPSONDING
C	 TO THE NUMBER GIVEN AS THE $ARG NAME
C	 QLEN IS THE LENGTH OF THE REPLACING STRING.
C
	QLEN=TRIM(ARGLIST(ARGNUM,AGF))
	CALL REPLACE( SUBLINE,SPOT,RLEN,ARGLIST(ARGNUM,AGF)(1:QLEN) )
	IF(ERROR.NE.0)RETURN
C
C	 AND LOOK FOR ANY MORE $ARGS
C	 SKIP IS UPDATED TO POINT PAST WHERE THE $ARG WAS
C	 JUST REPLACED, TAKING INTO ACCOUNT THE DIFFERENCE
C	 IN LENGTHS BETWEEN THE REPLACER AND REPLACEE.
C
	SKIP=SPOT+QLEN
	GOTO 10
C
C	 AN ERROR IN ARGNUM WAS FOUND.  DON'T DIE, JUST SKIP OVER
C	 THAT $ (DOING NO REPLACEMENT) AND KEEP LOOKING.
C
 998  SKIP=SPOT+1
	GOTO 10
C
C	 NORMAL EXIT POINT
C
 999  ERROR=0
	RETURN
C
 1000 FORMAT(I5)
	END
C
C
	SUBROUTINE REPLACE(STRING,START,RLEN,WITH)
C
	IMPLICIT NONE
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
	INTEGER START, RLEN, WLEN, SLEN
	CHARACTER STRING*(*),WITH*(*)
	CHARACTER*80  TMPSTR
C
C	 THIS IS A GRAMPS UTILITY ROUTINE THAT REPLACES A
C	 SUBSTRING OF A 'STRING' BEGINNING AT 'START' FOR
C	 A LENGTH OF 'RLEN' WITH ANOTHER STRING, 'WITH'.
C
C	 FIRST REARRANGE THE TARGET
C	 STRING SO THAT THE REPLACEMENT STRING WILL FIT IN.
C	 THIS SHIFT COULD EITHER EXPAND OR COMPRESS THE TARGET.
C
	WLEN=LEN(WITH)
	SLEN=LEN(STRING)
	IF(SLEN.GE.START+WLEN)THEN
	  TMPSTR=STRING(START+RLEN:)
	  STRING(START+WLEN:)=TMPSTR
	ELSE
	  ERROR=STRLEN
	  RETURN
	END IF
C
C	 THE REPLACEMENT STRING COULD BE NULL, IN WHICH CASE
C	 THE SHIFTING WAS ALL THAT WAS NECESSARY - RETURN.
C
	IF(WLEN.EQ.0)RETURN
C
C	 OTHERWISE INSERT THE REPLACEMENT STRING IN THE RIGHT SPOT
C	 IF THE TARGET IS BIG ENOUGH!
C
	IF(SLEN.GE.START+WLEN-1)THEN
	  STRING(START:START+WLEN-1)=WITH
	ELSE
	  ERROR=STRLEN
	  RETURN
	END IF
C
	RETURN
	END
