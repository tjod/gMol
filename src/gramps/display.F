C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE DISPLAY(forpick)
C
      IMPLICIT NONE
      Integer forpick
C
#include "UDLIST.FOR"
C#include "DEVICES.FOR"
#include "TFMMAT.FOR"
#include "G0COMMON.FOR"
C
C
C       USSIZE=1 MEANS FILL THE BUFFER (USLIST) STARTING AT 1.
C
      USSIZE=1
C
C       DISPLAY THE MENU.
C
      IF(MENUEND.GT.0)THEN
        IF(.NOT.SKIPDISP)THEN
        call g0set_matrix_mode_projection
        CALL TIDENT
        If (forpick .GT. 0) Then
          Call setpickmatrix
          HITTING = HITNOW
        End If
        call g0set_matrix_mode_view
        CALL TIDENT
C        CALL TPUSH
        IF(WINVRT)CALL USCALE(1.0,1.0,-1.0,1.0)
        CALL MENUDISP
C        CALL TPOP
        END IF
      END IF
C
C       IF THERE ARE NO DISPLAY OBJECTS, RETURN.
C       IF THE SKIPDISP FLAG IS SET, DON'T OUTPUT ANYTHING.
C
      IF(SKIPDISP.OR.TREEEND.LE.0)RETURN
C
      USSIZE=1

      if(outting.eq.OUTWINDOW)CALL G0STEREO(LWORLD)
      call g0set_matrix_mode_projection
      CALL TIDENT
      If (forpick .GT. 0) Then
        Call setpickmatrix
        HITTING = HITNOW
      End If
      CALL MAKEPER(LWORLD)
C      call g0set_matrix_mode_view done in call perspe now
c      call tident
      IF(OUTTING.EQ.OUTWINDOW .AND. forpick.EQ.0)CALL LITDISP

      if (stereo.eq.RGSTEREO) call g0color(LWORLD)
      CALL MAKETFM(LWORLD)
      if(outting.eq.OUTWINDOW)then
        continue
      else
        CALL COORMUL(LWORLD,LWORLD)
        CALL DUMPBUFF
      endif
C
C       Apply textures
C
C       Call g0texture(G0TEX0)
C
C       DISPLAY ALL OBJECTS IN THE TREE.
C
C      CALL TREEDISP(1,TREEEND)
      CALL TREEDISP1(1,TREEEND)
C
C       IF STEREO MODE IS ON, DISPLAY THE RIGHT WORLD
C       AND REPEAT DISPLAY OF EACH OBJECT
C
      IF(STEREO.NE.NOSTEREO)THEN
        USSIZE=1
        if(outting.eq.OUTWINDOW)CALL G0STEREO(RWORLD)
        call g0set_matrix_mode_projection
        CALL TIDENT
        If (forpick .GT. 0) Then
          Call setpickmatrix
          HITTING = HITNOW
        End If
        CALL MAKEPER(RWORLD)
        IF(OUTTING.EQ.OUTWINDOW .AND. forpick.EQ.0)CALL LITDISP
        if (stereo.eq.RGSTEREO) call g0color(RWORLD)
        CALL MAKETFM(RWORLD)
        if(outting.eq.OUTWINDOW)then
          continue
        else
          CALL COORMUL(RWORLD,RWORLD)
          CALL DUMPBUFF
        endif
C       IF(DRAWING)CALL DRAWDISP
C        CALL TREEDISP(1,TREEEND)
        CALL TREEDISP1(1,TREEEND)
      END IF
C
      RETURN
      END
C
C
      SUBROUTINE TREEDISP(FROM,TO)
C
      IMPLICIT NONE
      INTEGER FROM,TO
C
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
C      LOGICAL HIDETMP
C      INTEGER BACKFTMP
C       Display pass for no transparent objs
      G0DPASS=0
      CALL G0SBLEND(4,5)
      CALL TREEDISP1(FROM,TO)
C       Display pass for only transparent objs
      G0DPASS=1
C      HIDETMP=HIDESMODE
C      HIDESMODE=.FALSE.
C       CALL HIDESURF(0)
       CALL BACKFA(1)
C      BACKFTMP=BACKFACE
C      BACKFACE=1
      CALL G0SBLEND(4,6)
      CALL TREEDISP1(FROM,TO)
C      HIDESMODE=HIDETMP
C      BACKFACE=BACKFTMP
      END
C---------------------------------------------------------------
C
      SUBROUTINE TREEDISP1(FROM,TO)
C
      IMPLICIT NONE
      INTEGER FROM, TO
C
#include "UDLIST.FOR"
#include "ERRORS.FOR"
#include "G0COMMON.FOR"
C     INCLUDE ESTBL.FOR
C
      INTEGER TLEVEL, TWATCH, TLEV, I, K, L, NLOOP, J
      CHARACTER*80 TMPLINE
      INTEGER TESTSIZE
C      LOGICAL LOOPDO,DUMMY,LOOPSET
      INTEGER TXPAND, OBJDISP
C
C       THE TREE HOLDS THE STRUCTURE INFO ABOUT THE DISPLAY.
C       IT IS AN ARRAY HOLDING THE ID NUMBERS OF OBJECTS
C       IN THE ORDER IN WHICH THEY ARE TO BE DISPLAYED.
C       THE ID NUMBERS MAY REFER TO SIMPLE OBJECTS, OR
C       TO GROUPS.  IT IS THE GROUP WHICH GIVES THE
C       TREE A NON-LINEAR STRUCTURE.
C       THE END OF A GROUP
C       IS SPECIALLY MARKED IN THE TREE WITH AN ENTRY EQUAL
C       TO THE PARAMETER "GROUPEND" WHICH IS 0.
C       A GROUPEND CAN ALSO BE MARKED BY A NEGATIVE NUMBER
C       IN WHICH CASE THE GROUP WHICH IT ENDS IS BLANKED AND
C       THE GROUPEND POP SHOULD NOT BE DONE.
C
      TLEVEL=1
      TWATCH=0
C
      IF(TCHANGE)THEN
        XTEND=TXPAND(FROM,TO)
        IF(SHOWXTREE)THEN
          CALL GROUT(' ')
          TLEV=0
          DO I=1,XTEND
          K=XTREE(I)
          IF(K.GT.GROUPEND)THEN
            IF( .NOT.BTEST(KINDOBJ(I),BLNKBIT) ) Then
              WRITE(TMPLINE,'(20A)')('   ',L=1,TLEV),NAMOBJ(K)
              CALL GROUT(TMPLINE)
            End If
            IF( TYPOBJ(K).EQ.GROUP .OR. 
     &          ( BTEST(KINDOBJ(K),SYNOBIT) .AND.
     &            MASTSYN(K).NE.K ) )TLEV=TLEV+1
          ELSE
            TLEV=TLEV-1
          END IF
          END DO
          SHOWXTREE=.FALSE.
          TCHANGE=.FALSE.
          WRITE(TMPLINE,'(I5)')TLEV
          CALL GROUT(TMPLINE)
        END IF
      END IF
      IF(XTEND.LE.0)RETURN
C
C      write(*,*) xtend, (xtree(i),i=1,treeend)
      DO 10 J=1,XTEND
      I=XTREE(J)
C
CC       IF THIS IS A LOOPER, SEE IF THE LOOP NEEDS SETTING
C
C      NLOOP=1
C      IF( I.GT.GROUPEND )THEN
C        IF( BTEST(KINDOBJ(I),LOOPBIT) )THEN
C          LOOPING = LOOPSET(J,TLEVEL,TWATCH)
C      ELSE
C          LOOPING=.FALSE.
C        END IF
C      END IF
C
CC       LOOP CONTINUES FROM THIS POINT, IF WE'RE OBJECT LOOPING.
C
 20   CONTINUE
      I=XTREE(J)
      IF(I.LE.GROUPEND)THEN
        IF(I.EQ.GROUPEND)THEN
          TLEVEL=TLEVEL-1
C	  write(*,*) 'pop group'
          CALL TPOP
C          if(outting.eq.OUTWINDOW .AND. HITTING.EQ.HITNOW)call popnam
        END IF
        GOTO 900
      END IF
C
C       IS THE OBJECT IS BLANKED?
C
      IF( BTEST(KINDOBJ(I),BLNKBIT) )GOTO 900
C
C	Transparent object displayed this pass?
C
C      IF( TYPOBJ(I).NE.GROUP) Then
C        If (G0DPASS.EQ.0 .AND. tfmval(UITRNSP,i).ne.0) goto 900
C        If (G0DPASS.EQ.1 .AND. tfmval(UITRNSP,i).eq.0) goto 900
C      END IF
C
      IF(OBJDISP(I).EQ.1)THEN
C       A group
        TLEVEL=TLEVEL + 1
      ELSE
C        IF(.NOT.LOOPING)THEN
C	  write(*,*) 'pop obj'
          CALL TPOP
          if(outting.eq.OUTWINDOW .AND. HITTING.EQ.HITNOW)call popnam
C        end if
      END IF
C
CC       IF WE'VE COME TO THE END OF THE OBJECT BEING LOOPED
CC       THEN RESTART THE LOOP BY CALLING LOOPCHECK FOR A NEW J.
C
 900  CONTINUE
C      IF(LOOPING .AND. TLEVEL.EQ.TWATCH)THEN
C        NLOOP=NLOOP+1
C        IF(LOOPDO(J,TLEVEL,TWATCH))GOTO 20
C      END IF
C
 10   CONTINUE
C
 999  CONTINUE
      RETURN
      END
C
C---------------------------------------------------------------
C
      SUBROUTINE MENUDISP
C
      IMPLICIT NONE
C
#include "UDLIST.FOR"
#include "TFMMAT.FOR"
      REAL  HUE, SAT, VALU
      INTEGER I, K
C
      IF(MENUEND.LE.0)RETURN
C
      DO 10 I=1,MENUEND
      K=MENUTREE(I)
      IF( .NOT.BTEST(KINDOBJ(K),BLNKBIT) )THEN
C        write(*,*) 'push menu'
        CALL TPUSH
        CALL MAKETFM(K)
        call g0color(k)
        if(outting.eq.OUTWINDOW)then
          if (HITTING.EQ.HITNOW) call pushnam(k)
          if (HITOBJ .EQ. k) call gettfmmat(tmpmat)
          call callob (objfrid(idobj(k)))
          if (HITTING.EQ.HITNOW) call popnam
        else
          CALL COORMUL(K,IDOBJ(K))
          CALL DUMPBUFF
        endif
C        write (*,*) 'pop menu'
        CALL TPOP
      END IF
 10   CONTINUE
C
      RETURN
      END
C
C
C
C---------------------------------------------------------------
C
      FUNCTION IDFRAM(I)
C
      IMPLICIT NONE
#include "UDLIST.FOR"
      integer j, jtop, i
C
      If(TFMVAL(UFRAMI,I).LE.0)Then
        jtop=FRMCNT(I)
c    j = (TFMVAL(UFRAME,I)+32768)*jtop/65536 + 1
        j = (TFMVAL(UFRAME,I)+1)*jtop/2 + 1
        if(j.lt.1)j=1
        if(j.gt.jtop)j=jtop
        IDFRAM=IDOBJ(I) + j
      Else
        IDFRAM=IDOBJ(I) + MIN(INT(TFMVAL(UFRAMI,I)),FRMCNT(I))
      End If
C
      RETURN
      END
C
C
C---------------------------------------------------------------
C
C      LOGICAL FUNCTION LOOPSET(J,TLEVEL,TWATCH)
CC
C      IMPLICIT NONE
C      INTEGER J, TLEVEL, TWATCH
C      LOGICAL LOOPDO
C      INTEGER JSAVE, I, K
C      INTEGER NLOOP
C      SAVE NLOOP
C      DATA NLOOP/0/
CC
C#include "UDLIST.FOR"
CC
C      IF(NLOOP.EQ.0)THEN
C        NLOOP=CLOOP+1
C        JSAVE=J
C        TWATCH=TLEVEL
C        LOOPSET=.TRUE.
C      ELSE
C        LOOPSET=.FALSE.
C      END IF
CC
C      RETURN
CC
C      ENTRY LOOPDO(J,TLEVEL,TWATCH)
CC
C      NLOOP=NLOOP-1
C      IF(NLOOP.EQ.0)THEN
C      DO 10 i=1,cloop+1
C      CALL TPOP
C      if(outting.eq.OUTWINDOW .AND. HITTING.EQ.HITNOW)call popnam
C 10   CONTINUE
C        TWATCH=0
C        LOOPDO=.FALSE.
C      ELSE
C        J=JSAVE
C        LOOPDO=.TRUE.
C      END IF
CC
C      RETURN
C      END
C
      INTEGER FUNCTION TXPAND(FROM, TO)
C   
      IMPLICIT NONE
      INTEGER TXJNST
      INTEGER FROM, TO
      LOGICAL SYNLEFT
      INTEGER I, J, CALLEV, K
      SAVE    I, J, CALLEV
#include "UDLIST.FOR"
#include "ERRORS.FOR"
      INTEGER RECURS(2,WORKSIZE/2)
      EQUIVALENCE (WORK, RECURS)
C
      CALLEV=0
      J=0
      I=0
C
      ENTRY TXJNST(FROM, TO)
      CALLEV=CALLEV+1
      IF(CALLEV.GE.WORKSIZE/2)GOTO 999
C
      I=FROM
 10   CONTINUE
      IF(I.LE.TO)THEN
        K=TREE(I)
        IF( .NOT.BTEST(KINDOBJ(K),BLNKBIT) )THEN
          RECURS(1,CALLEV)=I
          RECURS(2,CALLEV)=TO
          CALL TXINST(I,J)
          IF(J.EQ.0)GOTO 998
          I= RECURS(1,CALLEV)
          TO=RECURS(2,CALLEV)
        END IF
        I=I+1
        GOTO 10
      END IF
C
 998  CONTINUE
      CALLEV=CALLEV-1
      TXPAND=J
      RETURN
C
 999  CONTINUE
      ERROR=XTRSIZE
      CALL TELLUSER1(ERROR)
      TXPAND=0
      J=0
      RETURN
C
      END
C
      SUBROUTINE TXINST(I,J)
C   
      IMPLICIT NONE
      INTEGER I, J
      INTEGER XTRSIZE, K, KOBJ, START, FINISH, DEPTH, LEVEL, SIZE
      INTEGER TXJNST, NSHIFT
#include "UDLIST.FOR"
C
      IF(J.GE.XTREESIZE)THEN
        ERROR=XTRSIZE
        CALL TELLUSER1(ERROR)
        J=0
        RETURN
      END IF
C
      K=TREE(I)
      IF( BTEST(KINDOBJ(K),SYNOBIT) .AND. MASTSYN(K).NE.K )THEN 
        J=J+1
        XTREE(J)=K
C
C    J=J+1
        KOBJ=FRMCNT(K)
        CALL GROUPINFO(KOBJ,START,FINISH,DEPTH,LEVEL,SIZE)
        NSHIFT=TXJNST(START, FINISH)
        IF(NSHIFT.EQ.0)RETURN
C
        J=J+1
        XTREE(J)=GROUPEND
      ELSE
        J=J+1
        XTREE(J)=K
      END IF
C
      RETURN
      END
C
      INTEGER FUNCTION OBJDISP(I)
      IMPLICIT NONE
C
#include "UDLIST.FOR"
#include "TFMMAT.FOR"
#include "ERRORS.FOR"
      REAL HUE, SAT, VALU
      INTEGER I, IFR
      External trim
C
C       GOTO THE APPROPRIATE SPOT TO DISPLAY
C       THIS TYPE OF OBJECT.
C
      If(outting.eq.outrayt .or. outting.eq.outrays
     &   .or.  outting.eq.outpov)Then
        write(OFILE,'(a)')'/* '//namobj(i)(:trim(namobj(i)))//' */'
      Else If(outting.eq.outpostscr)Then
        write(OFILE,'(a)')'%grampsObject: '//namobj(i)(:trim(namobj(i)))
      End If
      IF( BTEST(KINDOBJ(I),SYNOBIT) .AND. MASTSYN(I).NE.I )GOTO 400
      GOTO (100,200,300,400,50,50) TYPOBJ(I)+1
 50   CALL TELLUSER(NEVER,*900)
C
C       AN ORDINARY OBJECT
C
 100  CONTINUE
C      write(*,*) 'push obj'
      CALL TPUSH
      CALL MAKETFM(I)
      if (stereo.ne.RGSTEREO)call g0color(i)
      if(outting.eq.OUTWINDOW) Then
        if (HITTING.EQ.HITNOW)call pushnam(i)
        if (HITOBJ .EQ. i) call gettfmmat(tmpmat)
c          call g0refill(idobj(i))
          call callob (objfrid(idobj(i)))
c  dumpout uses no display lists, required for text; could refill before callob too
c          call dumpout (dlist(objfrloc(idobj(i))), objfrsiz(idobj(i)))
      else
        CALL COORMUL(I,IDOBJ(I))
        CALL DUMPBUFF
      endif
      GOTO 900
C
C       A FRAMED PICTURE
C       CALCULATE AND DISPLAY THE CORRECT FRAME(S)
C
 200  CONTINUE
C      write(*,*) 'push obj'
      CALL TPUSH
      CALL MAKETFM(I)
      if (stereo.ne.RGSTEREO)call g0color(i)
      If(outting.eq.OUTWINDOW) Then
       if (HITTING.eq.HITNOW) call pushnam(i)
       if (HITOBJ .EQ. i) call gettfmmat(tmpmat)
      End If
      IF( BTEST(KINDOBJ(I),RETBIT) )THEN
        DO 201 IFR=IDOBJ(I)+1,IDFRAM(I)
        if(outting.eq.OUTWINDOW)then
         call callob (objfrid(ifr))
        else
          CALL COORMUL(I,IFR)
          CALL DUMPBUFF
        endif
 201    CONTINUE
      ELSE
        if(outting.eq.OUTWINDOW)then
         call callob (objfrid(idfram(i)))
        else
          CALL COORMUL(I,IDOBJ(I))
          CALL COORMUL(I,IDFRAM(I))
          CALL DUMPBUFF
        endif
      END IF
      GOTO 900
C
C       A SINGLE FRAME OBJECT
C       DISPLAY THE SINGLE FRAME
C
 300  CONTINUE
C      write(*,*) 'push obj'
      CALL TPUSH
      CALL MAKETFM(I)
      if (stereo.ne.RGSTEREO)call g0color(i)
      if(outting.eq.OUTWINDOW)then
        IF(HITTING.eq.HITNOW)call pushnam(i)
        if (HITOBJ .EQ. i) call gettfmmat(tmpmat)
         call callob (objfrid(idobj(i)+1))
      else
        CALL COORMUL(I,IDOBJ(I))
        CALL COORMUL(I,IDOBJ(I)+1)
        CALL DUMPBUFF
      endif
      GOTO 900
C
C       A GROUP
C
 400  CONTINUE
C      TLEVEL=TLEVEL+1
C      write(*,*) 'push group'
      CALL TPUSH
      CALL MAKETFM(I)
      if (stereo.ne.RGSTEREO)call g0color(i)
      if(outting.eq.OUTWINDOW .AND. HITTING.EQ.HITNOW) Then
       IF (BTEST(KINDOBJ(I),SYNOBIT) .AND. MASTSYN(I).NE.I )
     &     call pushnam(i)
      end if
      OBJDISP=1
      RETURN
C
C       END OF THE USED-TO-BE DO LOOP
C
 900  CONTINUE
      OBJDISP=0
      RETURN
      END
C
      SUBROUTINE LITDISP
C
      IMPLICIT NONE
      INTEGER I, LID
C
#include "UDLIST.FOR"
C
      DO 10 i=1, NLITES
      LID = GLITE1 + i - 1
      IF( BTEST(KINDOBJ(LID),BLNKBIT) )then
        call g0nolit(i, LID)
      ELSE
        call g0lits(i, LID)
      END IF
 10   CONTINUE
C
      return
      end
