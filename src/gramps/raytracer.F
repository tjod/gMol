C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE RAYTRACER(ARG,SWITCHES)
      Implicit None
#include "UDLIST.FOR"
      CHARACTER ARG*(*),SWITCHES*(*)
C
      If( MATCH(SWITCHES,'RAYSHADE') )Then
        Call RAYSHADE(ARG,SWITCHES)
      Else If( MATCH(SWITCHES,'POV') )Then
        Call POVRAY(ARG,SWITCHES)
      Else
        Call RAYT(ARG,SWITCHES)
      End If
      Return
      End
C
      SUBROUTINE RAYT(ARG,SWITCHES)
C
      Implicit None
      CHARACTER ARG*(*),SWITCHES*(*)
#include "ERRORS.FOR"
#include "UDLIST.FOR"
#include "FILES.FOR"
#include "G0COMMON.FOR"
      REAL w,aspect
      Integer OUTSAVE
      Integer PCOLOR
      Character*80 options
      Character*120 syscmd
      Character*10 LINWID
      Character*5 size
      Logical Present
      EXTERNAL TRIM
C
      IF(ARG.EQ.' ' .OR. ARG(1:1).eq.'-')THEN
        OUTFILE='/tmp/plot.ray'
      ELSE
        CALL GETSTR(ARG,POINT,OUTFILE)
        DEFEXT='rayt'
      End If
      ERROR=OPENFILE(OUTFILE,OFILE,.TRUE.,.FALSE.)
      If(ERROR.NE.0)Call Telluser(ERROR, *999)
      If(ARG.NE.' ' .and. point.ne.0)Then
        options=ARG(POINT:)
        If(Index(options, '-r').eq.0)Then
         Write(size, '(i5)')G0XWIND
         options = options(:Trim(options)) // ' -x '// size
         Write(size, '(i5)')G0YWIND
         options = options(:Trim(options)) // ' -y '// size
        End If
      Else
        options='-r 256 -n '
      End If
      CALL SMATCH(SWITCHES,'LINEWIDTH',LINWID,PRESENT)
      IF(PRESENT)THEN
        G0RAYRAD=0.002*A2F(LINWID,ERROR)
      Else
        G0RAYRAD=0.002
      End If
c
      OUTSAVE=OUTTING
      OUTTING=OUTRAYT
      Call g0raysrf0
      Call raythead
      aspect=WLDVAL(WWINDA,LWORLD)
c 0.9 kludge for why?
      w=WLDVAL(WWINDW,LWORLD)*0.95
      Call TIDENT
      if(aspect.lt.w)call uscale(w,w,w,aspect)
      Call MAKETFM(LWORLD)
      Call TREEDISP1(1,TREEEND)
      close (unit=OFILE)
      OUTTING=OUTSAVE
      If( .NOT.MATCH(SWITCHES,'HOLD') )Then
c    call rayshow(fullfile(:TRIM(fullfile)),options(:TRIM(options)))
        syscmd='time rayt '//options(:TRIM(options))//' '
     &      //fullfile(:TRIM(fullfile))//' | display -flip +matte - &'
        Write(0,*)syscmd
        Call syscall(syscmd)
      End If
c
 999  Continue
      Return
      End
c
      Subroutine RAYTHEAD
c
      Implicit None
#include "UDLIST.FOR"
#include "G0COMMON.FOR"
#include "TFMMAT.FOR"
c
      Integer i,j,lid
      Real YON, IA
c
      write(OFILE,'(a)')'/* Output by Gramps */'
      write(OFILE,'(a,g10.3,a,g10.3)')'EYE ',
     & WLDVAL(WWINDS,LWORLD)*WLDVAL(WWINDW,LWORLD),
     & ' 0 ',
     & WLDVAL(WWINDE,LWORLD)*WLDVAL(WWINDW,LWORLD)*WLDVAL(WWINDW,LWORLD)
      write(OFILE,'(a,g10.3,a)')'GAZE ',
     & WLDVAL(WWINDP,LWORLD)/WLDVAL(WWINDW,LWORLD),
     & ' 0 0'
      write(OFILE,'(a)')'UP 0 1 0'
      write(OFILE,'(a,g10.3)')'FOV ',
     &    WLDVAL(WWINDV,LWORLD)
c  Call TIDENT
c  Call MAKETFM(LWORLD)
c  write(OFILE,'(a)')'MATRIX'
c  write(OFILE,'(4f)')((TFMMTX(j,i,CURRENT),i=1,4),j=1,4)
      write(OFILE,'(a,g10.3)')'HITHER ',
     &    WLDVAL(WWINDH,LWORLD)
      YON=WLDVAL(WWINDY,LWORLD)
      write(OFILE,'(a,g10.3)')'YON ',YON
      IA=0.5
      write(OFILE,'(a,f7.2)')'IA ',IA
      write(OFILE,'(a)')'KA 0.30'
      write(OFILE,'(a)')'DEPTHCUE'
      write(OFILE,'(a,3f8.4)')'BACKGROUND ',(G0BKRGB(i),i=1,3)
        DO 10 i=1, NLITES
        LID = GLITE1 + i - 1
        IF( BTEST(KINDOBJ(LID),BLNKBIT) )then
        Continue
        ELSE
        write(OFILE,'(a,a,i1,a)')'/* ','Light',i,' */'
                call g0lits(i, LID)
        END IF
 10     CONTINUE
c
c  Boy did this slow down rendering!! (beacuse of bounding volumes)
c  Use rlebg r g b >bg.rle; rlecomp x.rle over bkg.rle
c  write(OFILE,'(a)')'/* BACKGROUND */'
c  write(OFILE,'(a)') 'polygon'
c  write(OFILE,'(a,3g10.3)') '   point  -500  500',YON*100.
c  write(OFILE,'(a,3g10.3)') '   point   500  500',YON*100.
c  write(OFILE,'(a,3g10.3)') '   point   500 -500',YON*100.
c  write(OFILE,'(a,3g10.3)') '   point  -500 -500',YON*100.
c  write(OFILE,'(a,3g10.3)') '   point  -500  500',YON*100.
c  write(OFILE,'(a,3g10.3)') '   color ', (G0BKRGB(i),i=1,3)
c  write(OFILE,'(a,3g10.3)') '   ka ',1./IA
c  write(OFILE,'(a)') '   kd  0.000     kp  0.0'
c  write(OFILE,'(a)') '   ks  0.000     kn   0.0  kt  0.00 ri   1.00'
c
      Return
      End
C
      SUBROUTINE RAYSHADE(ARG,SWITCHES)
C
      Implicit None
      CHARACTER ARG*(*),SWITCHES*(*)
#include "ERRORS.FOR"
#include "UDLIST.FOR"
#include "FILES.FOR"
#include "G0COMMON.FOR"
      REAL w,aspect
      Integer OUTSAVE
      Integer PCOLOR
      Character*80 options
      Character*120 syscmd
      Character*10 LINWID
      Logical Present
      EXTERNAL TRIM
C
      IF(ARG.EQ.' ' .OR. ARG(1:1).eq.'-')THEN
        OUTFILE='/tmp/plot.ray'
      ELSE
        CALL GETSTR(ARG,POINT,OUTFILE)
        DEFEXT='rays'
      End If
      ERROR=OPENFILE(OUTFILE,OFILE,.TRUE.,.FALSE.)
      If(ERROR.NE.0)Call Telluser(ERROR, *999)
      If(ARG.ne.' ' .and. point.ne.0)Then
        options=ARG(POINT:)
      Else
        options='-R 256 256 -p '
      End If
      CALL SMATCH(SWITCHES,'LINEWIDTH',LINWID,PRESENT)
      IF(PRESENT)THEN
        G0RAYRAD=0.002*A2F(LINWID,ERROR)
      Else
        G0RAYRAD=0.002
      End If
c
      OUTSAVE=OUTTING
      OUTTING=OUTRAYS
      Call g0raysrf0
      Call rayshead
      aspect=WLDVAL(WWINDA,LWORLD)
c 0.9 kludge for why?
      w=WLDVAL(WWINDW,LWORLD)*0.95
      Call TIDENT
      if(aspect.lt.w)call uscale(w,w,w,aspect)
      Call MAKETFM(LWORLD)
      Call TREEDISP1(1,TREEEND)
      close (unit=OFILE)
      OUTTING=OUTSAVE
      If( .NOT.MATCH(SWITCHES,'HOLD') )Then
c    call rayshow(fullfile(:TRIM(fullfile)),options(:TRIM(options)))
        syscmd='time rayshade '//options(:TRIM(options))//' '
     &      //fullfile(:TRIM(fullfile))//' | rlespiff | get4d '
        Write(0,*)syscmd
        Call syscall(syscmd)
      End If
c
 999  Continue
      Return
      End
c
      Subroutine RAYSHEAD
c
      Implicit None
#include "UDLIST.FOR"
#include "G0COMMON.FOR"
#include "TFMMAT.FOR"
c
      Integer i,j,lid
      Real YON, IA
      Real rtot, gtot, btot
c
      write(OFILE,'(a)')'/* Output by Gramps */'
      write(OFILE,'(a,g10.3,a,g10.3)')'eyep ',
     &    WLDVAL(WWINDS,LWORLD)/WLDVAL(WWINDW,LWORLD),
     &    ' 0 ',
     &    WLDVAL(WWINDE,LWORLD)/WLDVAL(WWINDW,LWORLD)
      write(OFILE,'(a,g10.3,a)')'lookp ',
     &    WLDVAL(WWINDP,LWORLD)/WLDVAL(WWINDW,LWORLD),
     &    ' 0 0'
      write(OFILE,'(a)')'up 0 1 0'
      write(OFILE,'(a,g10.3)')'fov ',
     &    WLDVAL(WWINDV,LWORLD)/WLDVAL(WWINDW,LWORLD)
      Call g0lits(0,0)
        DO 10 i=1, NLITES
        LID = GLITE1 + i - 1
        IF( BTEST(KINDOBJ(LID),BLNKBIT) )then
        Continue
        ELSE
        write(OFILE,'(a,a,i1,a)')'/* ','Light',i,' */'
                call g0lits(i, LID)
        END IF
 10     CONTINUE
      Call g0alit
c
      Return
      End
C
      SUBROUTINE POVRAY(ARG,SWITCHES)
C
      Implicit None
      CHARACTER ARG*(*),SWITCHES*(*)
#include "ERRORS.FOR"
#include "UDLIST.FOR"
#include "FILES.FOR"
#include "G0COMMON.FOR"
      Integer OUTSAVE
      Integer PCOLOR
      Character*80 options
      Character*120 syscmd
      Character*10 LINWID
      Logical Present
      EXTERNAL TRIM
C
C   SUBROUTINE TO PLOT SCREEN.
C
      IF(MATCH(SWITCHES,'SHADOW')) Then
      End If
      IF(ARG.EQ.' ' .OR. ARG(1:1).eq.'+')THEN
#ifdef MINGW32
        OUTFILE='$TMP/plot.pov'
#else
        OUTFILE='/tmp/plot.pov'
#endif
      ELSE
        CALL GETSTR(ARG,POINT,OUTFILE)
        DEFEXT='pov'
      End If
      ERROR=OPENFILE(OUTFILE,OFILE,.TRUE.,.FALSE.)
      If(ERROR.NE.0)Then
        write(*,*) outfile, fullfile
      	Call Telluser(ERROR, *999)
      End If
      If(ARG.ne.' ' .and. point.ne.0)Then
        options=ARG(POINT:)
      Else
        options='+H256 +W256 '
      End If
      CALL SMATCH(SWITCHES,'LINEWIDTH',LINWID,PRESENT)
      IF(PRESENT)THEN
        G0RAYRAD=0.002*A2F(LINWID,ERROR)
      Else
        G0RAYRAD=0.002
      End If
c
      OUTSAVE=OUTTING
      OUTTING=OUTPOV
      Call g0raysrf0
      Call povhead
      Call TIDENT
      Call MAKETFM(LWORLD)
      Call TREEDISP1(1,TREEEND)
      close (unit=OFILE)
      OUTTING=OUTSAVE
      If( .NOT.MATCH(SWITCHES,'HOLD') )Then
#ifdef MINGW32
        syscmd = 'start '//fullfile(:TRIM(fullfile))
     &    //' +A '//options(:TRIM(options))

#else
        syscmd=' povray '//options(:TRIM(options))
     &    //' +P +A +I '//fullfile(:TRIM(fullfile))//' &'
C     &     //' +O/tmp/plot.tga; display /tmp/plot.tga &'
#endif
        Write(0,*)syscmd
        Call syscall(syscmd)
      End If
c
 999  Continue
      Return
      End
c
      Subroutine POVHEAD
c
      Implicit None
#include "UDLIST.FOR"
#include "G0COMMON.FOR"
#include "TFMMAT.FOR"
c
      Integer i,j,lid
      Real YON, IA
      Real rtot, gtot, btot
      Real aspect
c
      write(OFILE,'(a)')'/* Output by Gramps */'
      write(OFILE,'(a,g10.3,a,g10.3)')'camera { '
      write(OFILE,'(a,g10.3,a,g10.3,a)')' location <',
     &    WLDVAL(WWINDP,LWORLD)*WLDVAL(WWINDW,LWORLD),
     &    ', 0, ',
     &    WLDVAL(WWINDE,LWORLD)
     &   *WLDVAL(WWINDW,LWORLD)
     &   *WLDVAL(WWINDA,LWORLD),
     &    '>'
C      write(OFILE,'(a,g10.3,a)')' right <',
C     &    -WLDVAL(WWINDA,LWORLD),
C     &    ',0>'
      write(OFILE,'(a)') 'right     -x*image_width/image_height'
      write(OFILE,'(a)')' look_at <0,0,0>'
      aspect=WLDVAL(WWINDA,LWORLD)/WLDVAL(WWINDW,LWORLD)
      write(OFILE,'(a,g8.3,a)')' angle ',
C angle seems ok for square windows, but ...
     &    WLDVAL(WWINDV,LWORLD)
      write(OFILE,'(a)')'}'

      Call g0lits(0,0)
        DO 10 i=1, NLITES
        LID = GLITE1 + i - 1
        IF( BTEST(KINDOBJ(LID),BLNKBIT) )then
        Continue
        ELSE
        write(OFILE,'(a,a,i1,a)')'/* ','Light',i,' */'
                call g0lits(i, LID)
        END IF
 10     CONTINUE
      Call g0alit
c
      Return
      End
