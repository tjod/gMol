C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE UDINIT
C
      IMPLICIT NONE
C
#include "UDLIST.FOR"
#include "FILES.FOR"
#include "DEFVAL.FOR"
#include "TFMMAT.FOR"
C#include "DEVICES.FOR"
#include "G0COMMON.FOR"
C
      INTEGER I, J
      CHARACTER*(NAMMAX) LNAME(NLITES)
      DATA LNAME/'Light1','Light2','Light3','Light4',
     & 'Light5','Light6','Light7','Light8'/
C
C       INITIALIZE SYSTEM,
C       DISPLAY LIST POINTER TO FREE CORE
C       AND ASSORTED SYSTEM VARIABLES.
C
C       THIS ROUTINE IS ALSO CALLED BY RESTART.
C
      UDFREE = 1
      USSIZE=1
      NOBJ=0
      NIDS=0
      NFRAMES=0
      TREEEND=0
      XTEND=0
      MENUEND=0
      CONT=.FALSE.
      NADVANCE = 0
      COLLECT=.FALSE.
      COLLFILE=' '
      DEFDIR=' '
      CLOSE (UNIT=CFILE)
      OUTTING=OUTNONE
      OUTFILE=' '
      CLOSE (UNIT=OFILE)
#ifdef QT
      CMDFROM = QT_OUT
#else
      CMDFROM = STANDARD_OUT
#endif
      DRAWING=.FALSE.
      GRNUM=ICHAR('A')-1
      SPILL=UDLIMIT
      MPSERR=0
      AUTOCLEAR=.TRUE.
      HITSYM=0
      HITCOORD=-1
      HITTING=0
      HITOBJ=0
      WAITING=0
      CURSOR=.FALSE.
      DBLMODE=.TRUE.
      CLSPOT=0
      ADSENS=1
      MAXDEPTH=11
      REPORT=.TRUE.
      LOWERCASE=.FALSE.
      SNAPPING=.FALSE.
      XGL=.FALSE.
      SMOLIN=.FALSE.
      call setsmooth(0)
      GFILE=GFILBASE
      AGF=1
      G0SPHN1=2
      G0SPHR1=2
      G0SPHN2=4
      G0SPHR2=2
      G0RES=2
      G0CYLN1=7
      G0CYLR1=2
      G0CYLN2=20
      G0CYLR2=2
      CALL G0DTSIZE(5)
      CALL G0DFONT(0)
      CALL G0DXTSCALE(100)
      CALL G0DYTSCALE(100)
C
      CALL G0SETSPEC(DINTP)
      CALL G0SETDIFF(DINTD)
      CALL G0SETAMBI(DINTA)
      CALL G0SETEMIS(DINTE)
      CALL G0SETSHIN(DINTY)
C
      CALL G0DBKCOL(0.,0.,0.)
      Call G0BLEND(4,5)
C
      CURRENT=1
      BASEZ=0
c      BASEW=32767
C
      DO 5 I=1,NCOORDS
      UCOORD(1,I)=0.
      UCOORD(2,I)=0.
      UCOORD(3,I)=0.
      UCOORD(4,I)=1.
 5    CONTINUE
C
C       CREATE THE LEFT WORLD GROUP.
C       ALLOCATE ONE WORD FOR IT.
C
      CALL OPENOBJ('LWorld',WORLD)
      CALL CLOSEOBJ(WORLD)
      KINDOBJ(LWORLD)=GROUP
C
C       OPEN THE RIGHT WORLD (OBJ #2) TO BE READY FOR STEREO
C       SAME PERSPECTIVE AND ROTATIONS AS LEFT WORLD FOR NOW.
C
      CALL OPENOBJ('RWorld',WORLD)
      CALL CLOSEOBJ(WORLD)
      KINDOBJ(RWORLD)=GROUP
      STEREO=NOSTEREO
C
C   Create LIGHT objects
C
      DO 6 i=1, NLITES
      CALL OPENOBJ(LNAME(i), LIGHT)
      CALL CLOSEOBJ(LIGHT)
      TFMVAL(UCSAT,NOBJ)=0
      TFMVAL(UCVAL,NOBJ)=1.
      CALL BLNKOBJ(NOBJ)
 6    CONTINUE
      CALL DBLNKOBJ(GLITE1)
C
C       SET THE DEFAULT VALUES FOR THE PERSP AND MOVE TFMS.
C
      DO 10 I=LWORLD,RWORLD
      DO 8 J=1,WTFM
      FNWLD(J,I)=0
      WLDVAL(J,I)=0
      WLDINC(J,I)=0
 8    DEVWLD(J,I)=0
      WLDVAL(WMOVEX,I)=DMOVX
      WLDVAL(WMOVEY,I)=DMOVY
      WLDVAL(WMOVEZ,I)=DMOVZ
      WLDVAL(WMOVEW,I)=DMOVW
      WLDVAL(WWINDS,I)=DWINDS
      WLDVAL(WWINDV,I)=DWINDV
      WLDVAL(WWINDP,I)=DWINDP
      WLDVAL(WWINDA,I)=DWINDA
      WLDVAL(WWINDH,I)=DWINDH
      WLDVAL(WWINDY,I)=DWINDY
      WLDVAL(WFOG,I)  =DWFOG
      WLDVAL(WDEPTH,I)=DWDEPTH
      WLDVAL(WWINDE,I)=DWINDE
      WLDVAL(WWINDW,I)=DWINDW
 10   CONTINUE
C
C       CREATE THE CURSORS FOR EVENTUAL DRAWING
C
C     CALL DRAWAIDS
C
C       SAVE THE POINTER TO USER UDL SPACE
C
      UDSTART=UDFREE
C
C       DO THE STARTUP FILES
C
      RETURN
      END

      SUBROUTINE STARTUP
      IMPLICIT NONE
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
      CHARACTER*120 DIR
      INTEGER I

      CALL GETENV( 'gramps_lib', DIR )
      if (DIR(1:1) .eq. ' ') Call Telluser(NOGRLIB, *999)

      POINT=1
      ARGSTART=1
      CALL DOFILE('$gramps_lib/gramps.gin')
      POINT=1
      ARGSTART=1
      CALL DOFILE('$HOME/gramps.gin')
      ERROR=0
C
 999  CONTINUE
      RETURN
      END
