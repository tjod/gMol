C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      INTEGER FUNCTION CONTUPD(DO_IT)
C
      IMPLICIT NONE
      INTEGER DO_IT
C
      REAL DEST
      REAL SIN
      REAL PI
      PARAMETER (PI=3.1415927)
C
C#include "DEVICES.FOR"
#include "UDLIST.FOR"
       INTEGER J
C
      IF (.NOT.CONT .AND. DO_IT.EQ.0) Then
C      If nothing needs updating and DO_IT (forcing flag) is off (zero),
C        CONTUPD = 0
C        Return
         GOTO 999
      End If
C
C       SET THE CONT FLAG .FALSE. UNLESS IT TURNS OUT THAT
C       SOME OBJECT STILL HAS ITS BADDINC BIT SET.  YOU NEED TO
C       CHECK EACH OBJECT THIS WAY IN CASE ALL OBJECTS HAVE
C       TURNED OFF THEIR CONTINUOUS TRANSFORMATIONS SINCE YOU
C       LAST CHECKED.  ONCE CONT IS LEFT FALSE, THOUGH, IT CAN
C       ONLY BE SET TRUE BY ONE OF THE TRANSFORMATION SET-UP
C       SUBROUTINES (E.G. ROTATE).
C       THIS WAY, THIS SUBROUTINE WILL ONLY HAVE TO CHECK THROUGH
C       EACH OBJECT ONE TIME TO LEARN THAT NO OBJECT IS CONTINUALLY
C       UPDATING.
C
      CONT=.FALSE.

      DO 30 ID=1,NOBJ
      IF( .NOT.BTEST(FNUSE(SUMMARY,ID),BADDINC) )GOTO 30
      CONT=.TRUE.
C
C       NOW CHECK THROUGH EACH POSSIBLE TRANSFORMATION AND UPDATE
C       THOSE VALUES WHICH NEED TO BE UPDATED.
C
C
C       THERE ARE THREE SPECIAL FUNCTIONS TO CONSIDER AFTER
C       IT HAS BEEN DETERMINED THAT "ADDINC" IS EVEN IN
C       EFFECT FOR THE OBJECTS TRANSFORMATION.
C       1) "LIMIT" WHERE THE INC IS ADDED UP TO A LIMIT
C           OR DESTINATION VALUE.  THIS CAN BE A LINEAR
C           ADDITION OR A SIN FUNCTION ADDITION IN WHICH
C           INC CHANGES SIZE BASED ON A SIN FUNCTION.
C           THIS FUNCTION ALSO USES TFMANG AND AMPL ARRAYS.
C           TFMINC ARRAY HOLDS A STEP NUMBER, WHICH IS
C           DECREMENTED EACH TIME THRU, UNTIL IT REACHES 1.
C       2) "BOUNDED" WHERE THE TRANSFORMATION IS BOUNDED
C           BY A CERTAIN VALUE.  JUST ADD INC BUT CHECK
C           TO SEE IF THE NEW VALUE IS OUTSIDE THE BOUND.
C           IF IT IS, THEN SET THE VALUE=BOUND, UNLESS...
C       3)  UNLESS THE FUNCTION IS "BOUNCED", IN WHICH CASE
C           THE DIRECTION OF ADDITION IS CHANGED SO THAT
C           THE TRANSFORMATION BOUNCES WITHIN BOUNDS WHEN
C           THE BOUND IS REACHED.
C	4)  The function could be wrap-around at the limits; this
C	    is also a BOUNDED function.
C	5)  The function could be OBJECT directed, meaning that
C	    the values come from the data in an object.
C
C       IF THIS IS A NORMAL OR BOUNDED ADDINC THEN TFMINC HOLDS THE
C       INCREMENT SIZE.  IF IT IS "LIMIT" FUNCTION, THEN
C       TFMINC HOLDS THE STEP NUMBER IN THE INTERPOLATION,
C       AND THE INC IS CALCULATED BASED ON OTHER INFORMATION,
C       HELD IN TFMANG AND AMPL ARRAYS.
C
      DO 10 J=1,MTFM
      IF( .NOT.BTEST(FNUSE(J,ID),BADDINC) )GOTO 10
      IF(BTEST(FNUSE(J,ID),BLIMIT) )THEN
        IF(TFMINC(J,ID).LE.1)THEN
          TFMVAL(J,ID)=BOUNDS(J,ID)
          TFMINC(J,ID)=0
          FNUSE(J,ID)=IEOR( FNUSE(J,ID),IOR(FADDINC,FLIMIT) )
          FNUSE(J,ID)=IEOR( FNUSE(J,ID),FSIN )
          CALL ORTOTAL(ID)
        ELSE
          IF( BTEST(FNUSE(J,ID),BSIN) )THEN
            TFMANG(J,ID)=TFMANG(J,ID) +
     &        (PI-TFMANG(J,ID))/TFMINC(J,ID)
            TFMVAL(J,ID)=BOUNDS(J,ID) -
     &        AMPL(J,ID)*( 1+COS(TFMANG(J,ID)) )
          ELSE
            DEST=BOUNDS(J,ID)
            TFMVAL(J,ID)=TFMVAL(J,ID) +
     &        (DEST-TFMVAL(J,ID))/TFMINC(J,ID)
          END IF
          TFMINC(J,ID)=TFMINC(J,ID)-1
        END IF
      ELSE IF( BTEST(FNUSE(J,ID),BBOUNDED) )THEN
          DEST=TFMVAL(J,ID)
          DEST=DEST + TFMINC(J,ID)
          IF(DEST.GT.UBOUND(J,ID))THEN
            IF( BTEST(FNUSE(J,ID),BWRAP) )THEN
              TFMVAL(J,ID)=LBOUND(J,ID) + (DEST-UBOUND(J,ID))
            ELSE
              TFMVAL(J,ID)=UBOUND(J,ID)
              IF( BTEST(FNUSE(J,ID),BBOUNCE) )THEN
                TFMINC(J,ID)=-TFMINC(J,ID)
                FNUSE(J,ID)=IEOR(FNEGDEV,FNUSE(J,ID) )
              END IF
            END IF
          ELSE
            IF(DEST.LT.LBOUND(J,ID))THEN
              IF( BTEST(FNUSE(J,ID),BWRAP) )THEN
                TFMVAL(J,ID)=UBOUND(J,ID) + (DEST-LBOUND(J,ID))
              ELSE
                TFMVAL(J,ID)=LBOUND(J,ID)
                IF( BTEST(FNUSE(J,ID),BBOUNCE) )THEN
                  TFMINC(J,ID)=-TFMINC(J,ID)
                  FNUSE(J,ID)=IEOR(FNEGDEV,FNUSE(J,ID))
                END IF
              END IF
            ELSE
              TFMVAL(J,ID)=DEST
            END IF
          END IF
      ELSE IF( BTEST(FNUSE(J,ID),BOBJ) )THEN
        CALL OBJUPD(J,ID)
      ELSE
        TFMVAL(J,ID)=TFMVAL(J,ID) + TFMINC(J,ID)
      END IF
C
C       NOW LET THIS UPDATING BE INSERTED AS MATRICES INTO THE UDL
C
C      CALL USHOW(ID,J)
C
 10   CONTINUE
C
C       CHECK OUT THE EXTRA WOLRD TRANSFORMATIONS, IF NECESSARY
C
      IF(ID.LE.RWORLD)CALL CONTWUPD
C
 30   CONTINUE
C
C      IF( BTEST(FNUSE(SUMMARY,ID),BOBJ) ) CALL OBJUPD(SUMMARY,ID)
 999  CONTINUE
      If (nadvance.gt.0) nadvance = nadvance - 1
      If (nadvance.gt.0) Then
        CONTUPD = nadvance
      Else
C Advance has expired - we may need to continue inside dofile
        Do While(WAITING .EQ. 0 .AND. AGF.GT.1 .AND. NADVANCE.EQ.0)
          CALL DOMORE
        End Do
        If(PPAUSE)Then
C          CONTUPD=AGF-1
          CONTUPD=0
        Else
          If (CONT) Then
            CONTUPD=-1
          Else
C            CONTUPD=AGF-1
            CONTUPD=0
          End If
        End If
      End If
C
      RETURN
      END
C
C
      SUBROUTINE CONTWUPD
C
      IMPLICIT NONE
C
      REAL DEST
      REAL SIN
      REAL PI
      PARAMETER (PI=3.1415927)
      INTEGER J
C
C#include "DEVICES.FOR"
#include "UDLIST.FOR"
C
C       THIS SUBROUTINE IS STRICTLY ANALOGOUS TO CONTUPD
C       BUT ONLY APPLIES TO THE WOLRDS AND THE THE SPECIAL
C       TRANSFORMATIONS (PERSPECTIVE AND MOVE) WHICH ONLY
C       THE WORLDS CAN UTILIZE.
C
      IF( .NOT.BTEST(FNWLD(WSUMMRY,ID),BADDINC) )RETURN
      DO 20 J=1,WTFM
      IF( .NOT.BTEST(FNWLD(J,ID),BADDINC) )GOTO 20
      IF(BTEST(FNWLD(J,ID),BLIMIT) )THEN
        IF(WLDINC(J,ID).LE.1)THEN
          WLDVAL(J,ID)=BOUNDW(J,ID)
          WLDINC(J,ID)=0
          FNWLD(J,ID)=IEOR( FNWLD(J,ID),IOR(FADDINC,FLIMIT) )
          FNWLD(J,ID)=IEOR( FNWLD(J,ID),FSIN )
          CALL ORTOTAL(ID)
        ELSE
          IF( BTEST(FNWLD(J,ID),BSIN) )THEN
            WLDANG(J,ID)=WLDANG(J,ID) +
     &        (PI-WLDANG(J,ID))/WLDINC(J,ID)
            WLDVAL(J,ID)=BOUNDW(J,ID) -
     &        AMPLW(J,ID)*( 1+COS(WLDANG(J,ID)) )
          ELSE
            DEST=BOUNDW(J,ID)
            WLDVAL(J,ID)=WLDVAL(J,ID) +
     &        (DEST-WLDVAL(J,ID))/WLDINC(J,ID)
          END IF
          WLDINC(J,ID)=WLDINC(J,ID)-1
        END IF
      ELSE IF( BTEST(FNWLD(J,ID),BBOUNDED) )THEN
          DEST=WLDVAL(J,ID)
          DEST=DEST + WLDINC(J,ID)
          IF(DEST.GT.UWBOUND(J,ID))THEN
            IF( BTEST(FNWLD(J,ID),BWRAP) )THEN
              WLDVAL(J,ID)=LWBOUND(J,ID) + (DEST-UWBOUND(J,ID))
            ELSE
              WLDVAL(J,ID)=UWBOUND(J,ID)
              IF( BTEST(FNWLD(J,ID),BBOUNCE) )THEN
                WLDINC(J,ID)=-WLDINC(J,ID)
                FNWLD(J,ID)=IEOR(FNEGDEV,FNWLD(J,ID) )
              END IF
            END IF
          ELSE
            IF(DEST.LT.LWBOUND(J,ID))THEN
              IF( BTEST(FNWLD(J,ID),BWRAP) )THEN
                WLDVAL(J,ID)=UWBOUND(J,ID) + (DEST-LWBOUND(J,ID))
              ELSE
                WLDVAL(J,ID)=LWBOUND(J,ID)
                IF( BTEST(FNWLD(J,ID),BBOUNCE) )THEN
                  WLDINC(J,ID)=-WLDINC(J,ID)
                  FNWLD(J,ID)=IEOR(FNEGDEV,FNWLD(J,ID) )
                END IF
              END IF
            ELSE
              WLDVAL(J,ID)=DEST
            END IF
          END IF
      ELSE
        WLDVAL(J,ID)=WLDVAL(J,ID) + WLDINC(J,ID)
      END IF
C      CALL WSHOW(ID,J)
 20   CONTINUE
C
      RETURN
      END
C
C****************************************************************
C
C (C) Copyright 1989  TJ O'Donnell
C
C****************************************************************
      SUBROUTINE OBJUPD(TFMTYP,IID)
C
      IMPLICIT NONE
C
#include "UDLIST.FOR"
C
        INTEGER TFMTYP, IID
	REAL X,Y,Z, ZBASE, WBASE
	LOGICAL SKIP
	INTEGER NDIM, SPOT, ENDSPOT, DIMSKP, NSKIP
	INTEGER JID, ENDOBJ, N, LDM1, LDM2, CHLEN, W
C
C	TFMINC packs three types of data:
C	1) the ID of the object to be followed
C	2) the current dimensionality of the data
C	3) the number of data points to be skipped per update.
C
	JID=    MOD(INT(TFMINC(TFMTYP,IID)), MOBJ)
C
	DIMSKP= TFMINC(TFMTYP,IID)/MOBJ
	NSKIP=  DIMSKP/10
	NDIM=   MOD(DIMSKP,10)
        IF(NSKIP.LE.0)NSKIP=1
	IF(NDIM.LE.0 .OR. NDIM.GT.4)NDIM=3
	SPOT=   LBOUND(TFMTYP,IID)
	ENDSPOT=UBOUND(TFMTYP,IID)
	WBASE=  DFACT(TFMTYP,IID)
	ZBASE=0
C
	ENDOBJ=ABS(LOCOBJ(JID)) + SIZEOBJ(JID)
	IF(SPOT.LE.0 .OR. SPOT.GE.ENDOBJ)THEN
		SPOT=ABS(LOCOBJ(JID))
		ENDSPOT=0
		WBASE=0
		NDIM=3
	END IF
	IF(SPOT.GE.ENDSPOT)THEN
		SKIP=.TRUE.
		DO WHILE(SKIP)
			N=   DLIST(SPOT)
			LDM1=DLIST(SPOT+1)
			LDM2=DLIST(SPOT+2)
			SPOT=SPOT+3
			IF(N.EQ.0)THEN
				IF(LDM1.EQ.0)THEN
					IF(LDM2.EQ.0)THEN
						LDM2=DLIST(SPOT+3)
						SPOT=SPOT+4
					END IF
					WBASE=LDM2
				ELSE IF(LDM1.EQ.1)THEN
					CONTINUE
				ELSE IF(LDM1.EQ.2)THEN
					CONTINUE
				ELSE IF(LDM1.EQ.3)THEN
					SPOT=SPOT+1
					CHLEN=DLIST(SPOT)
					SPOT=SPOT+CHLEN
				ELSE IF(LDM1.EQ.4)THEN
					CONTINUE
				ELSE IF(LDM1.EQ.5)THEN
					CONTINUE
				ELSE IF(LDM1.EQ.6)THEN
					NDIM=LDM2
				ELSE IF(LDM1.EQ.7)THEN
					CONTINUE
				ELSE IF(LDM1.EQ.8)THEN
					SPOT=SPOT+6
					SPOT=SPOT+1
					CHLEN=DLIST(SPOT)
					SPOT=SPOT+CHLEN
				ELSE
					CONTINUE
				END IF
			ELSE
				IF(LDM1.EQ.0 .OR. LDM1.EQ.2 .OR. LDM1.EQ.4)THEN
					SKIP=.FALSE.
				ELSE IF     (LDM1.EQ.5)THEN
					NDIM=4
					SKIP=.FALSE.
				ELSE IF( (LDM1.EQ.6 .OR. LDM1.EQ.7
     &				.OR. LDM1.EQ.8) .AND.
     &				(ABS(LDM2).EQ.1 .OR. ABS(LDM2).EQ.2) )THEN
					NDIM=NDIM*2
					SKIP=.FALSE.
				ELSE IF( (LDM1.EQ.9 .OR. LDM1.EQ.10) .AND.
     &				(ABS(LDM2).EQ.1 .OR. ABS(LDM2).EQ.2) )THEN
					NDIM=NDIM*3
					SKIP=.FALSE.
				END IF
				IF(LDM1.EQ.8)THEN
					SPOT=SPOT+N*3
				END IF
			END IF
		END DO
		ENDSPOT=SPOT+N*NDIM
		UBOUND(TFMTYP,IID)=ENDSPOT
		IF(WBASE.EQ.0)THEN
			IF(LDM2.LE.3)THEN
				WBASE=1.
			ELSE
				WBASE=32767.
			END IF
		END IF
	END IF
	IF(NDIM.GT.3)THEN
		W=DLIST(SPOT+2)
	ELSE
		W=WBASE
	END IF
	IF(NDIM.GT.2)THEN
		Z=DLIST(SPOT+2)
	ELSE
		Z=ZBASE
	END IF
	X=DLIST(SPOT)/W
	Y=DLIST(SPOT+1)/W
	Z=Z/W
	IF(TFMTYP.EQ.UTRANX  .OR. TFMTYP.EQ.UROTXX .OR.
     &	   TFMTYP.EQ.UATRANX .OR. TFMTYP.EQ.UAROTX .OR.
     &	   TFMTYP.EQ.UASCALX .OR. TFMTYP.EQ.USCALEX .OR.
     &	   TFMTYP.EQ.UCHUE   .OR. TFMTYP.EQ.UITRNSP .OR.
     &	   TFMTYP.EQ.UFRAME ) TFMVAL(TFMTYP,IID)=X
	IF(TFMTYP.EQ.UTRANY  .OR. TFMTYP.EQ.UROTYY .OR.
     &	   TFMTYP.EQ.UATRANY .OR. TFMTYP.EQ.UAROTY .OR.
     &	   TFMTYP.EQ.UASCALY .OR. TFMTYP.EQ.USCALEY .OR.
     &	   TFMTYP.EQ.UCSAT   .OR. TFMTYP.EQ.UIAMBI .OR.
     &	   TFMTYP.EQ.UFRAMI ) TFMVAL(TFMTYP,IID)=Y
	IF(TFMTYP.EQ.UTRANZ  .OR. TFMTYP.EQ.UROTZZ .OR.
     &	   TFMTYP.EQ.UATRANZ .OR. TFMTYP.EQ.UAROTZ .OR.
     &	   TFMTYP.EQ.UASCALZ .OR. TFMTYP.EQ.USCALEZ .OR.
     &	   TFMTYP.EQ.UCVAL   .OR. TFMTYP.EQ.UISPEC)TFMVAL(TFMTYP,IID)=Z
c 	write(*,*) 'objupd: ',namobj(iid), namobj(jid)
c	write(*,*) 'x,y,z:,w ',DLIST(SPOT), DLIST(SPOT+1), DLIST(SPOT+2), W
	SPOT=SPOT+NDIM*NSKIP
	LBOUND(TFMTYP,IID)=SPOT
	UBOUND(TFMTYP,IID)=ENDSPOT
	TFMINC(TFMTYP,IID)=(NSKIP*10 + NDIM)*MOBJ + JID
	DFACT (TFMTYP,IID)=WBASE
C
c  	write(*,*) 'lbound: ',lbound(tfmtyp,iid)
c  	write(*,*) 'ubound: ',ubound(tfmtyp,iid)
c  	write(*,*) 'tfminc: ',tfminc(tfmtyp,iid)
c  	write(*,*) 'dfact : ',dfact (tfmtyp,iid)
c  	write(*,*) 'tfmval: ',tfmval(tfmtyp,iid)
C	
	return
	end
