C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE PLOT(ARG,SWITCHES)
C
      Implicit None
      CHARACTER ARG*(*),SWITCHES*(*)
#include "ERRORS.FOR"
#include "UDLIST.FOR"
#include "FILES.FOR"
#include "G0COMMON.FOR"
      Real aspect
      Integer OUTSAVE
      Integer PCOLOR
      EXTERNAL TRIM
C
C   SUBROUTINE TO PLOT SCREEN.
C
      IF(ARG.EQ.' ')THEN
        OUTFILE='/usr/tmp/plot.ps'
      ELSE
        CALL GETSTR(ARG,POINT,OUTFILE)
        DEFEXT='ps'
      End If
      ERROR=OPENFILE(OUTFILE,OFILE,.TRUE.,.FALSE.)
      If(ERROR.NE.0)Call Telluser(ERROR, *999)
c
      OUTSAVE=OUTTING
      OUTTING=OUTPOSTSCR
      Call pshead(ofile)
      If ( MATCH(SWITCHES,'NOREVERSEBW') )Then
        write(OFILE,'(a)')'/white {0 0 1 sethsbcolor} def'
        write(OFILE,'(a)')'/black {0 0 0 sethsbcolor} def'
      Else
        write(OFILE,'(a)')'/white {0 0 0 sethsbcolor} def'
        write(OFILE,'(a)')'/black {0 0 1 sethsbcolor} def'
      End If
      If( MATCH(SWITCHES,'NOCOLOR') )Then
        write(OFILE,'(a)')'/newcolor {pop pop pop} def'
        PCOLOR=0
      Else
        write(OFILE,'(a)')'/newcolor {sethsbcolor} def'
        If( MATCH(SWITCHES,'TRANS') )Then
          PCOLOR=2
        Else
          PCOLOR=1
        End If
      End If
      If(G0XWIND.le.0 .or. G0YWIND.le.0)Then
        Continue
      Else
        aspect=FLOAT(G0YWIND)/FLOAT(G0XWIND)
        if(aspect.lt.1.)Then
          write(OFILE,'(2f15.7,a)')1., aspect, ' scale'
        Else if(aspect.gt.1.)Then
          write(OFILE,'(2f15.7,a)')1./aspect, 1., ' scale'
        End If
      End If
      If(G0RSTYLE.EQ.2)Then
        write(OFILE,'(a)') '/polyfill {closepath fill} def'
        write(OFILE,'(a)') '/polymoveto {moveto} def'
        write(OFILE,'(a)') '/polylineto {lineto} def'
      Else If(G0RSTYLE.EQ.1)Then
        write(OFILE,'(a)') '/polyfill {closepath stroke} def'
        write(OFILE,'(a)') '/polymoveto {moveto} def'
        write(OFILE,'(a)') '/polylineto {lineto} def'
      Else If(G0RSTYLE.EQ.0)Then
        write(OFILE,'(a)') '/polyfill {} def'
        write(OFILE,'(a)') '/polymoveto {dot} def'
        write(OFILE,'(a)') '/polylineto {dot} def'
      End If
      Call DISPLAY(0)
      write(OFILE,'(a)')'showpage'
      close (unit=OFILE)
      OUTTING=OUTSAVE
      If( .NOT.MATCH(SWITCHES,'HOLD') )
     &    call psplot(outfile(:TRIM(outfile)), PCOLOR)
c
 999  Continue
      Return
      End
c
      Subroutine PSHEAD(OFILE)
c
      Implicit None
      Integer OFILE
c
      write(OFILE,'(a)')'%!'
      write(OFILE,'(a)')'%Author:  Gramps v.2.0'
      write(OFILE,'(a)')'1 72 div 4 div setlinewidth'
      write(OFILE,'(a)')'72 72 scale'
      write(OFILE,'(a)')'8.5 2 div 11 2 div translate'
      write(OFILE,'(a)')'4 4 scale'
      write(OFILE,'(a)') '/sphere {0 360 arc fill} def'
      write(OFILE,'(a)') '/dotsize 0.003 def'
      write(OFILE,'(a)') '/dot {dotsize 0 360 arc fill} def'
      write(OFILE,'(a)') '/cylinder'
     &  // ' {gsave 2 mul setlinewidth moveto lineto stroke grestore}'
     &  // ' def'
c
      Return
      End
