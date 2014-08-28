C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
      SUBROUTINE G0DONE
C
      IMPLICIT NONE
C
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
      IF(OUTTING.EQ.OUTNIL)THEN
        RETURN
      ELSE IF(OUTTING.EQ.OUTWINDOW)THEN
C        CALL WINCLO
      ELSE IF(OUTTING.EQ.OUTTEK4010)THEN
        CONTINUE
      ELSE
        RETURN
      END IF
C
      RETURN
      END
C
      SUBROUTINE G0INIT(XSIZ, YSIZ, XORI, YORI, BORDER)
C
      IMPLICIT NONE
      INTEGER XSIZ, YSIZ, XORI, YORI
      LOGICAL BORDER
C
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
C     WRITE(*,*)OUTNIL,OUTWINDOW,OUTTEK4010,BADOUTTYPE,OUTTING
      IF(OUTTING.EQ.OUTNIL)THEN
        RETURN
      ELSE IF(OUTTING.EQ.OUTWINDOW)THEN
      ELSE IF(OUTTING.EQ.OUTTEK4010)THEN
        CONTINUE
      ELSE
        CALLTELLUSER1(BADOUTTYPE)
        RETURN
      END IF
C
      Return
      End
C
      SUBROUTINE G0PINIT
C
      IMPLICIT NONE
C
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
#include "DEFVAL.FOR"
      INTEGER I
      Integer genobj
C
      OUTTING=OUTWINDOW
C AGF should be 1 at this point
      TUNITS(AGF)=NEWUNITS
      Call setunits(NEWUNITS)
      Call g0light_init
      Call g0initfog
      Call g0line_width(3.0)
      G0PCNT=0
C
C  These are in UDINIT
C      CALL G0SETSPEC(DINTP)
C      CALL G0SETDIFF(DINTD)
C      CALL G0SETAMBI(DINTA)
C      CALL G0SETEMIS(DINTE)
C      CALL G0SETSHIN(DINTY)
CC  CALL G0SETSTYL(1.)
C      CALL G0DBKCOL(0.,0.,0.)

       CALL G0BLEND(4,5)

C  Polygon representation style
      G0RSTYLE=G0ShowFilled

C Inititialize sphere, etc.
      CALL G0SPHINIT(G0SPHN1, G0SPHR1, G0SPHN2, G0SPHR2)
      CALL G0CYLINIT(G0CYLN1, G0CYLR1, G0CYLN2, G0CYLR2)
C
C Initialize textures
C     G0TEX0 = genobj()
C     Call g0texture_init(G0TEX0)
C
      Call G0DFONT(0)
C
      Return
      End
C
      SUBROUTINE G0WSTORE(XO, YO, XS, YS)
C
      IMPLICIT NONE
      INTEGER XO, YO, XS, YS

      INTEGER WINXO, WINYO, WINXS, WINYS
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
C
#ifndef QT
      IF (OUTTING .EQ. OUTWINDOW) THEN
         CALL getwinpos(WINXO, WINXS, WINYO, WINYS)
      ELSE
#endif
        WINXO = XO
        WINYO = YO
        WINXS = XS
        WINYS = YS
#ifndef QT
      END IF
#endif
C
      G0XORIG=WINXO
      G0YORIG=WINYO
      G0XWIND=WINXS
      G0YWIND=WINYS
      Return
      End
C
      SUBROUTINE G0WSET(XO, YO, XS, YS, TELLWS)
C
      IMPLICIT NONE
      INTEGER  XO, YO, XS, YS, TELLWS
      REAL ASPECT
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
C  Change the window size
C
C Stores in G0XORIG, etc.
      Call G0WSTORE(XO, YO, XS, YS)

      IF(OUTTING.EQ.OUTNIL)THEN
        RETURN
      ELSE IF(OUTTING.EQ.OUTWINDOW)THEN
        ASPECT = FLOAT(XS) / FLOAT(YS)
        IF (STEREO.EQ.LRSTEREO) ASPECT = 0.5 * ASPECT
        WLDVAL(WWINDA,LWORLD) = ASPECT
        WLDVAL(WWINDA,RWORLD) = ASPECT
        If (TELLWS .GT. 0) Then
          call setwinpos(G0XORIG, G0XWIND, G0YORIG, G0YWIND)
        EndIf
C        call viewpo(0,G0XWIND-1, 0,G0YWIND-1)
        DCHANGE = .TRUE.
      ELSE
C    CALLTELLUSER1(BADOUTTYPE)
        RETURN
      END IF
C
      END
C
      SUBROUTINE G0ASPECT
C
      IMPLICIT NONE
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
      REAL ASPECT
C
      IF(     ABS(STEREO).EQ.UDSTEREO)THEN
        ASPECT =( FLOAT(G0XWIND)/FLOAT(G0YWIND) )*2.
      ELSE IF(ABS(STEREO).EQ.ZSTEREO)THEN
c The 140 is empirical 
ceyes    ASPECT =  FLOAT(G0XWIND)/FLOAT(G0YWIND)
        ASPECT =  FLOAT(G0XWIND)/FLOAT(G0YWIND-140)
      ELSE IF(ABS(STEREO).EQ.LRSTEREO)THEN
        ASPECT =( FLOAT(G0XWIND)/FLOAT(G0YWIND) )/2.
      ELSE IF(STEREO.EQ.FSTEREO)THEN
        ASPECT =  FLOAT(G0XWIND)/FLOAT(G0YWIND)
      ELSE IF(STEREO.EQ.RGSTEREO)THEN
        ASPECT =  FLOAT(G0XWIND)/FLOAT(G0YWIND)
      ELSE IF(STEREO.EQ.NOSTEREO)THEN
        ASPECT =  FLOAT(G0XWIND)/FLOAT(G0YWIND)
      END IF
      WLDVAL(WWINDA,LWORLD)=ASPECT*WLDVAL(WWINDW,LWORLD)
      WLDVAL(WWINDA,RWORLD)=ASPECT*WLDVAL(WWINDW,RWORLD)
C
      RETURN
      END
C
      SUBROUTINE G0STEREO(LREYE)
C
      IMPLICIT NONE
      INTEGER LREYE
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
      INTEGER XVIEW, YVIEW
C
C   Set out viewport
C
      IF(OUTTING.EQ.OUTNIL)THEN
        RETURN
      ELSE IF(OUTTING.EQ.OUTWINDOW)THEN
      IF(STEREO.EQ.NOSTEREO)THEN
        XVIEW=G0XWIND-1
        YVIEW=G0YWIND-1
        call viewpo(0,XVIEW, 0,YVIEW)
      ELSE IF(STEREO.EQ.LRSTEREO)THEN
        XVIEW=G0XWIND-1
        YVIEW=G0YWIND-1
        IF(LREYE.EQ.LWORLD)THEN
            call viewpo(0,XVIEW/2, 0,YVIEW)
        ELSE IF(LREYE.EQ.RWORLD)THEN
            call viewpo(XVIEW/2,XVIEW, 0,YVIEW)
        END IF
      ELSE IF(STEREO.EQ.ZSTEREO)THEN
c The 20 is empirical and works only for full height windows
        XVIEW=G0XWIND
        YVIEW=G0YWIND/2
        IF(LREYE.EQ.LWORLD)THEN
            call viewpo(0,XVIEW, YVIEW+20, G0YWIND)
ceyes        call viewpo(0,XVIEW, YVIEW   , G0YWIND)
        ELSE
            call viewpo(0,XVIEW, 0,YVIEW-20)
ceyes        call viewpo(0,XVIEW, 0,YVIEW)
        END IF
      ELSE IF(STEREO.EQ.UDSTEREO)THEN
        XVIEW=G0XWIND-1
        YVIEW=G0YWIND-1
        IF(LREYE.EQ.LWORLD)THEN
            call viewpo(0,XVIEW, YVIEW/2, G0YWIND)
        ELSE
            call viewpo(0,XVIEW, 0,YVIEW/2)
        END IF
      END IF
      ELSE
        CALLTELLUSER1(BADOUTTYPE)
        RETURN
      END IF
C
      END
C
C  Adds processed data to buffers
C
      Subroutine AddBuff_L(ALIST,BUFF,W,NORML,CWISE)
      Real ALIST(9),BUFF(9),W
      Integer NORML
      Logical CWISE
C
      Integer IHUE, ISAT, IVAL

C Coords
      BUFF(1)= ALIST(1)/W
      BUFF(2)= ALIST(2)/W
      BUFF(3)= ALIST(3)/W
      Return

      Entry AddBuff_LN(ALIST,BUFF,W,NORML,CWISE)
C Coords
      BUFF(1)= ALIST(1)/W
      BUFF(2)= ALIST(2)/W
      BUFF(3)= ALIST(3)/W
C Normals
      CALL G0NMLZ( ALIST(4), BUFF(4), NORML )
      Return

      Entry AddBuff_LC(ALIST,BUFF,W,NORML,CWISE)
C Coords
      BUFF(1)= ALIST(1)/W
      BUFF(2)= ALIST(2)/W
      BUFF(3)= ALIST(3)/W
C Colors
C      IHUE=MOD(INT(120+ALIST(4)),360)
C      ISAT=MOD(INT(100*ALIST(5)),101)
C      IVAL=MOD(INT(100*ALIST(6)),101)
C      CALL HSVRGB(IHUE, ISAT, IVAL, BUFF(4), BUFF(5), BUFF(6) )
      BUFF(7)= ALIST(7)
      BUFF(8)= ALIST(8)
      BUFF(9)= ALIST(9)
      Return

      Entry AddBuff_LNC(ALIST,BUFF,W,NORML,CWISE)
C Coords
      BUFF(1)= ALIST(1)/W
      BUFF(2)= ALIST(2)/W
      BUFF(3)= ALIST(3)/W
C Normals
      CALL G0NMLZ( ALIST(4), BUFF(4), NORML )
C Colors
C      IHUE=MOD(INT(120+ALIST(7)),360)
C      ISAT=MOD(INT(100*ALIST(8)),101)
C      IVAL=MOD(INT(100*ALIST(9)),101)
C      CALL HSVRGB(IHUE, ISAT, IVAL, BUFF(7), BUFF(8), BUFF(9))
      BUFF(7)= ALIST(7)
      BUFF(8)= ALIST(8)
      BUFF(9)= ALIST(9)
      Return
C
      End
C
      Subroutine RaysPoly(BUFF, N, IDIM,COLRV,NORML,OFILE)
      Implicit None
      Integer OFILE
      Integer N,IDIM
      Logical COLRV
      Integer NORML
      REAL BUFF(IDIM,N)
      Integer k,j
      Real NML(3)
C
      If (COLRV) Then
        If (NORML.EQ.0) Then
          Call G0SETRGB(BUFF(4,1), BUFF(5,1), BUFF(6,1))
        Else
          Call G0SETRGB(BUFF(7,1), BUFF(8,1), BUFF(9,1))
        End If
      End If
C
      Call g0raysurf(OFILE)
      If(n.eq.3)Then
        write(OFILE,'(a)')'triangle '
      Else
        write(OFILE,'(a)')'poly '
      End If
      If (NORML.EQ.0) Then
        CALL G0NRML(BUFF(1,1), BUFF(1,2), BUFF(1,3), NML )
        Call f3n( NML )
      End If
      do j=1,n
        write(OFILE,'(2(a,3g12.4))') '   point ', (BUFF(k,j),k=1,3)
c Rayshade won't handle normals for polygons, just triangles ???
        If(n.eq.3) Then
          If (NORML.EQ.0) Then
            write(OFILE,'(2(a,3g12.4))')'   normal ', (NML(k),k=1,3)
          Else
            write(OFILE,'(2(a,3g12.4))')'   normal ', (BUFF(k,j),k=4,6)
          End If
        End If
      End Do

      End

      Subroutine RaytPoly(BUFF, N, IDIM,COLRV,NORML,OFILE)
      Implicit None
      Integer OFILE
      Integer N,IDIM
      Logical COLRV
      Integer NORML
      REAL BUFF(IDIM,N)
      Integer k,j
      Real NML(3)
      Logical Told /.FALSE./
      Save Told
#include "ERRORS.FOR"
C
      If(n.eq.3) Then
        write(OFILE,'(a)')'triangle '
      Else
        write(OFILE,'(a)')'polygon '
      End If
      If (NORML.EQ.0) Then
        CALL G0NRML(BUFF(1,1), BUFF(1,2), BUFF(1,3), NML )
        call f3n( NML )
      End If

      do j=1,n
        write(OFILE,'(2(a,3g12.4))') '   point ', (BUFF(k,j),k=1,3)
c Rayt won't handle normals for polygons, just triangles !
        If(n.eq.3) Then
          If (NORML.EQ.0) Then
            write(OFILE,'(2(a,3g12.4))')'   normal ', (NML(k),k=1,3)
          Else
            write(OFILE,'(2(a,3g12.4))')'   normal ', (BUFF(k,j),k=4,6)
          End If
        Else
          If (NORML.NE.0) Then
            If(.NOT.Told) Call Telluser1(NORAYTNORM)
            Told = .TRUE.
          End If
        End If
      End Do

      If (COLRV) Then
        If (NORML.EQ.0) Then
          Call G0SETRGB(BUFF(4,1), BUFF(5,1), BUFF(6,1))
        Else
          Call G0SETRGB(BUFF(7,1), BUFF(8,1), BUFF(9,1))
        End If
      End If
      Call g0raysurf(OFILE)

      End

      Subroutine PovPoly(BUFF, N, IDIM,COLRV,NORML,OFILE)
      Implicit None
      Integer OFILE
      Integer N,IDIM
      Logical COLRV
      Integer NORML
      REAL BUFF(IDIM,N)
      Integer k,j
      Real NML(3)
      Logical Told /.FALSE./
      Save Told
#include "ERRORS.FOR"
C
      If (COLRV) Then
        If (NORML.EQ.0) Then
          Call G0SETRGB(BUFF(4,1), BUFF(5,1), BUFF(6,1))
        Else
          Call G0SETRGB(BUFF(7,1), BUFF(8,1), BUFF(9,1))
        End If
      End If
      Call g0raysurf(OFILE)

      If(n.eq.3) Then
        If (NORML.EQ.0) Then
          write(OFILE,'(a)')'triangle {'
        Else
          write(OFILE,'(a)')'smooth_triangle {'
        End If
      Else
        write(OFILE,'(a,i4,a)')'polygon {',n,','
      End If
C      If (NORML.EQ.0) Then
C        CALL G0NRML(BUFF(1,1), BUFF(1,2), BUFF(1,3), NML )
C        call f3n( NML )
C      End If

      do j=1,n
        If (NORML.EQ.0) Then
          write(OFILE,'(a,3(g12.4,a))')'<',
     &   BUFF(1,j),',',BUFF(2,j),',',BUFF(3,j),'>'
        Else
          write(OFILE,'(a,6(g12.4,a))')'<',
     &   BUFF(1,j),',',BUFF(2,j),',',BUFF(3,j),'>,<',
     &   BUFF(4,j),',',BUFF(5,j),',',BUFF(6,j),'>'
        End If
      End Do

      write(OFILE,'(a)')'}'

      End
C
      Subroutine PSPoly(BUFF, N, IDIM, COLRV, NORML, OFILE)
      Implicit None
      Integer N,IDIM
      REAL BUFF(IDIM,N)
      Logical COLRV
      Integer NORML
      Integer OFILE
      Integer k,j
      Real H,S,BR 
      Real ZAVG
      REAL PHASE
      Parameter (PHASE=360/3)

#include "G0COMMON.FOR"

      write(OFILE,'(a)')'newpath'
      write(OFILE,'(2f15.7,a)') (BUFF(k,1),k=1,2),' polymoveto'
      ZAVG = BUFF(1,1)
      do j=2,n
        write(OFILE,'(2f15.7,a)') (BUFF(k,j),k=1,2),' polylineto'
        ZAVG = ZAVG + (1-BUFF(k,3))/2
      END DO
      ZAVG=ZAVG/n
C Scale color value by z value, but not too much
      BR=G0VALU*ZAVG/2 + 0.5
      If (BR.LT.0) BR=0.0
      If (BR.GT.1) BR=1.0
      H=MOD(INT(360*G0HUE+PHASE),360)/360.
      S=G0SAT

      write(OFILE,'(3f15.7,a)')H,S,BR,' newcolor'
      write(OFILE,'(a)')'polyfill'
      END

      Subroutine G1Poly(BUFF,N,IDIM,RSTYLE,COLRV,NORML)
      Implicit None
      Integer N,IDIM
      REAL BUFF(IDIM,N)
      Integer RSTYLE
      Logical COLRV
      Integer NORML
      Integer k,j
      Real RTMP(3), NML(3)
#include "G0COMMON.FOR"
C
      If (n .eq. 3) Then
        Call G0BGN(RSTYLE,G0Triangle)
      Else
        Call G0BGN(RSTYLE,G0Polygon)
      End If

      IF(NORML.EQ.0)THEN
        CALL G0NRML(BUFF(1,1), BUFF(1,2), BUFF(1,3), NML )
        call f3n( NML )
      End If
      do j=1,n
       If (NORML.NE.0) Then
         call f3n( BUFF(4,j) )
          If (COLRV) Then
C           call c3f( BUFF(7,j) )
C           call g0material(BUFF(7,j), BUFF(8,j), BUFF(9,j),
C     &      1.0, 0.4, 0.4, 0.4, 0.0, 0.3125, -1, g0bfrgb)
            Call G0SETHSV(BUFF(7,j),BUFF(8,j),BUFF(9,j))
            Call G0COLOR(0)
          End If
       Else
         If (COLRV)call c3f( BUFF(4,j) )
       End If
       call v3f( BUFF(1,j) )
c      If(j.eq.1) Call g0cexCoord1f(0.0);
c      If(j.eq.n) Call g0texCoord1f(1.0);
       If(RSTYLE.eq.4)Then
         Do k=1,3
           If (NORML.NE.0) Then
             RTMP(k)=BUFF(k,j)+BUFF(k+3,j)
           Else
             RTMP(k)=BUFF(k,j)+NML(k)
           End If
         End Do
         call v3f( RTMP )
         call v3f( BUFF(1,j) )
       End If
      END DO
      Call G0END(RSTYLE,1)
C
      End
C
      SUBROUTINE G0POLY( ALIST, WBASE, N, NDIM,
     &   NORML, CWISE, COLRV, MESH )
      IMPLICIT NONE
C
      INTEGER N, NDIM
      REAL ALIST(NDIM*N)
      REAL WBASE
      LOGICAL CWISE, MESH, COLRV
      INTEGER J1, J2, J3, I1, I2, I3
      INTEGER NX, NY, NZ
      INTEGER MSHTYP
      Integer NORML
      Integer NINE, THREE
      INTEGER G0MKMSH, G0XSTRIP, G0YSTRIP
      CHARACTER*4 STRING
C
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
      REAL NML(3), RTMP(3)
      REAL W
      INTEGER NVERT
      Integer PNORML
      INTEGER NTRI
      INTEGER IHUE, ISAT, IVAL
      REAL R, G, B
      Real TRIBUFF(9,3)
      INTEGER RSTYLE
      INTEGER I, J, K
      LOGICAL VCOLR
      SAVE NVERT, PNORML, NTRI, VCOLR
C
      IF(N.GT.G0PMAX)CALL TELLUSER(TOOBIG,*999)
      VCOLR=COLRV
C      If (VCOLR) Call g0colormaterial(1)
C
C Fill buffers with processed data, location/normal/color
C
      DO 10 I=1,N
      J=(I-1)*NDIM
      IF(CWISE .AND. .NOT.MESH)THEN
        K=N+1-I
      ELSE
        K=I
      END IF
      IF(NDIM.EQ.4)THEN
        W=ALIST(4+J)
      ELSE IF(NDIM.EQ.8)THEN
        W=ALIST(8+J)
      ELSE
        W=WBASE
      END IF
C
      IF(NORML.NE.0)THEN
        IF(COLRV)THEN
          Call AddBuff_LNC(ALIST(J+1),G0LNCBUF(1,K),W,NORML,CWISE)
        ELSE
          Call AddBuff_LN(ALIST(J+1),G0NBUF(1,K),W,NORML,CWISE)
        END IF
      ELSE
        IF(COLRV)THEN
          Call AddBuff_LC(ALIST(J+1),G0LCBUF(1,K),W,NORML,CWISE)
        ELSE
          Call AddBuff_L(ALIST(J+1),G0DBUF(1,K),W,NORML,CWISE)
        END IF
      END IF
 10   CONTINUE
C
C Save NORML,NVERT for use in G0MESH
C
      PNORML=NORML
      IF(MESH)THEN
        NVERT=N
        RETURN
      END IF
C
C  Now fish the data out of the buffers and make the graphics calls
C
      RSTYLE = G0RSTYLE
      IF(NORML.NE.0)THEN
        If(OUTTING.EQ.OUTWINDOW)Then
          If (COLRV) Then
            Call G1Poly(G0LNCBUF,N,9,RSTYLE,COLRV,NORML)
          Else
            Call G1Poly(G0NBUF,N,6,RSTYLE,COLRV,NORML)
          End If

        Else If(OUTTING.EQ.OUTPOSTSCR)Then
          If (COLRV) Then
           Call PSPoly(G0LNCBUF, N,9, COLRV, NORML, OFILE)
          Else
           Call PSPoly(G0NBUF, N,6, COLRV, NORML, OFILE)
          End If

        Else If(OUTTING.EQ.OUTRAYT)Then
          If(COLRV)Then
            Call RaytPoly(G0LNCBUF,N,9,COLRV,NORML,OFILE)
          Else
            Call RaytPoly(G0NBUF,N,6,COLRV,NORML,OFILE)
          End If

        Else If(OUTTING.EQ.OUTRAYS)Then
          If(COLRV)Then
            Call RaysPoly(G0LNCBUF,N,9,COLRV,NORML,OFILE)
          Else
            Call RaysPoly(G0NBUF,N,6,COLRV,NORML,OFILE)
          End If

        Else If(OUTTING.EQ.OUTPOV)Then
          If(COLRV)Then
            Call PovPoly(G0LNCBUF,N,9,COLRV,NORML,OFILE)
          Else
            Call PovPoly(G0NBUF,N,6,COLRV,NORML,OFILE)
          End If
        End If
C
C NORML .EQ. FALSE
      ELSE
        If(OUTTING.EQ.OUTWINDOW)Then
          If (COLRV) Then
            Call G1Poly(G0LCBUF,N,6,RSTYLE,COLRV,NORML)
          Else
            Call G1Poly(G0DBUF,N,3,RSTYLE,COLRV,NORML)
          End If

        Else If(OUTTING.EQ.OUTPOSTSCR)Then
          If (COLRV) Then
           Call PSPoly(G0LCBUF, N,6, COLRV, NORML, OFILE)
          Else
           Call PSPoly(G0DBUF, N,3, COLRV, NORML, OFILE)
          End If

        Else If(OUTTING.EQ.OUTRAYT)Then
          If(COLRV)Then
            Call RaytPoly(G0LCBUF,N,6,COLRV,NORML,OFILE)
          Else
            Call RaytPoly(G0DBUF,N,3,COLRV,NORML,OFILE)
          End If

        Else If(OUTTING.EQ.OUTRAYS)Then
          If(COLRV)Then
            Call RaysPoly(G0LCBUF,N,6,COLRV,NORML,OFILE)
          Else
            Call RaysPoly(G0DBUF,N,3,COLRV,NORML,OFILE)
          End If

        Else If(OUTTING.EQ.OUTPOV)Then
          If(COLRV)Then
            Call PovPoly(G0LCBUF,N,6,COLRV,NORML,OFILE)
          Else
            Call PovPoly(G0DBUF,N,3,COLRV,NORML,OFILE)
          End If
        End If

      END IF
      NVERT=0
      RETURN
C
      ENTRY G0MESH(ALIST, WBASE, N, NDIM, NORML, CWISE, COLRV, MSHTYP)
C  SUBROUTINE G0POLY( ALIST, WBASE, N, NDIM, NORML, CWISE,
C     &    COLRV, MESH )
C
C      write(*,*)'g0mesh: ndim,n,nvert,pnorml,vcolr',
C     &   ndim,n,nvert,pnorml,vcolr
C PNORML is NORML from last call to g0poly
C VCOLR  is COLRV from last call to g0poly
C NVERT  is set on last call to g0poly
C
      RSTYLE = G0RSTYLE
      IF(NVERT.EQ.0)CALL TELLUSER(GETPIXERR,*999)
      IF(N.GT.G0TMAX)CALL TELLUSER(TOOBIG,*999)
      if(cwise)then
        i1=3
        i2=2
        i3=1
      else
        i1=1
        i2=2
        i3=3
      end if

C Explicit connectivity
      IF(ABS(MSHTYP).EQ.1 .OR. MSHTYP.EQ.0)THEN
        NTRI=N
        J=1
        K=1
        DO 20 I=1,N
          G0TBUF(i1,I)=ALIST(K)-1
          G0TBUF(i2,I)=ALIST(K+1)-1
          G0TBUF(i3,I)=ALIST(K+2)-1
          J=J+3
          K=K+NDIM
 20     CONTINUE
        if (MSHTYP .EQ. 0)RSTYLE = G0ShowLines
C
C The mesh is a consecutive list of triangles
      ELSE IF(ABS(MSHTYP).EQ.2)THEN
        NTRI=ALIST(1)
        IF(NTRI.EQ.0)CALL TELLUSER(GETPIXERR,*999)
C
C Automesh
      ELSE IF(ABS(MSHTYP).EQ.3)THEN
        IF(VCOLR)THEN
          write(*,*)'g0mesh: LocNormCol',nvert
          CALL TELLUSER1(NOTIMPL)
        End If
        NX=ALIST(1)
        NY=ALIST(2)
        NZ=ALIST(3)
C You get rhombus patterns in either direction which are more
C or less apparent depending on which direction is emphasized by
C the geometry of the object (long&skinny or wide&fat).  The choice
C of X or Y strip is equivalent in either case.
C The 'problem' is that the closure happens in a different direction
C depending on whether XSTRIP or YSTRIP is chosen; so closure
C happens by making an extra strip such that there are m+1 strips
C each n long with m<n.  Its more efficient to draw few long strips,
C but the look can be different, especially with FLAT shading.
        If(OUTTING.EQ.OUTWINDOW)Then
          If(NZ.EQ.0 .OR. NZ.EQ.1)Then
            If(NX.lt.NY)Then
              NZ=NZ-2
            Else
              NZ=NZ-4
            End If
          End If
          If(NZ.EQ.-1)Then
            NTRI=G0XSTRIP(NX,NY,1,CWISE,PNORML,G0PBUF,G0DBUF,RSTYLE)
          Else If(NZ.EQ.-2)Then
            NTRI=G0XSTRIP(NX,NY,0,CWISE,PNORML,G0PBUF,G0DBUF,RSTYLE)
          Else If(NZ.EQ.-3)Then
            NTRI=G0YSTRIP(NX,NY,1,CWISE,PNORML,G0PBUF,G0DBUF,RSTYLE)
          Else If(NZ.EQ.-4)Then
            NTRI=G0YSTRIP(NX,NY,0,CWISE,PNORML,G0PBUF,G0DBUF,RSTYLE)
          End If
          Goto 999
C
        Else If(OUTTING.EQ.OUTPOSTSCR .OR. OUTTING.EQ.OUTRAYS .OR.
     &          OUTTING.EQ.OUTRAYT .OR. OUTTING.EQ.OUTPOV)Then
          NTRI=G0MKMSH(NX, NY, NZ, G0TBUF, G0TMAX, i1, i2, i3)
          IF(NTRI.GT.G0TMAX)CALL TELLUSER(TOOBIG,*999)
        End If
      END IF
c
c  Explicitely output triangle connections as given
c   or this was an ordered list of triangles
c  write(*,*)'G0MESH: NTRI, MSHTYP = ',NTRI, MSHTYP

      if (OUTTING.EQ.OUTWINDOW) Call G0BGN(RSTYLE,G0Triangle)
      DO I=1,NTRI
C
C Set up the buffer for this triangle
C ordered list 1,2,3 4,5,6 7,8,9 ...
      IF(ABS(MSHTYP).EQ.2)THEN
         j1=3*(I-1)+1
         j2=j1+1
         j3=j1+2
C explicit list
       Else
         j1=G0TBUF(1,I)+1
         j2=G0TBUF(2,I)+1
         j3=G0TBUF(3,I)+1
       End If
C
C Move the chosen points/normals/colors into a triangle buffer
C Points
C K=1 is x-coord, y is 2, z is 3
       Do k=1,3
         If (PNORML.NE.0) Then
           IF (VCOLR) Then
C Points, normals and colors
             TRIBUFF(k,1)=G0LNCBUF(k,j1)
             TRIBUFF(k,2)=G0LNCBUF(k,j2)
             TRIBUFF(k,3)=G0LNCBUF(k,j3)
           Else
C Points, normals, no colors
             TRIBUFF(k,1)=G0NBUF(k,j1)
             TRIBUFF(k,2)=G0NBUF(k,j2)
             TRIBUFF(k,3)=G0NBUF(k,j3)
           End If

         Else
           IF (VCOLR) Then
C Points, colors, no normals
             TRIBUFF(k,1)=G0LCBUF(k,j1)
             TRIBUFF(k,2)=G0LCBUF(k,j2)
             TRIBUFF(k,3)=G0LCBUF(k,j3)
           Else
C Points only
             TRIBUFF(k,1)=G0DBUF(k,j1)
             TRIBUFF(k,2)=G0DBUF(k,j2)
             TRIBUFF(k,3)=G0DBUF(k,j3)
           End If
         End If
       End Do
C Normals ; may be colors for point only and point/color mode
C         ; may be missing if points only
C K=4 is x-normal, y is 5, z is 6
       Do k=4,6
         If (PNORML.NE.0) Then
           IF (VCOLR) Then
C Normals and Colors
             TRIBUFF(k,1)=G0LNCBUF(k,j1)
             TRIBUFF(k,2)=G0LNCBUF(k,j2)
             TRIBUFF(k,3)=G0LNCBUF(k,j3)
           Else
C Normals, no Colors
             TRIBUFF(k,1)=G0NBUF(k,j1)
             TRIBUFF(k,2)=G0NBUF(k,j2)
             TRIBUFF(k,3)=G0NBUF(k,j3)
           End If

C Colors go in 4,5,6 when no normals
         Else
           IF (VCOLR) Then
             TRIBUFF(k,1)=G0LCBUF(k,j1)
             TRIBUFF(k,2)=G0LCBUF(k,j2)
             TRIBUFF(k,3)=G0LCBUF(k,j3)
           End If
C Points only; no normals or colors
         End If
       End Do
C Colors
C K=7 is red,  green is 8, blue is 9
       Do k=7,9
         If (PNORML.NE.0 .AND. VCOLR) Then
C Normals and colors, otherwise no more data to put into TRIBUFF
           TRIBUFF(k,1)=G0LNCBUF(k,j1)
           TRIBUFF(k,2)=G0LNCBUF(k,j2)
           TRIBUFF(k,3)=G0LNCBUF(k,j3)
         End If
      End Do
C
C      IF(VCOLR)THEN
CC Not true anymore?
C        write(*,*)'g0mesh: LocNormCol',nvert
C        CALL TELLUSER1(NOTIMPL)
C      ELSE

        If(OUTTING.eq.OUTWINDOW)Then
          Call G1Poly(TRIBUFF,3,9,RSTYLE,VCOLR,PNORML)

        Else If(OUTTING.eq.OUTPOSTSCR)Then
          Call PSPoly(TRIBUFF,3,9,VCOLR,PNORML,OFILE)

        Else If(OUTTING.EQ.OUTRAYT)Then
          Call RaytPoly(TRIBUFF,3,9,VCOLR,PNORML,OFILE)

        Else If(OUTTING.EQ.OUTRAYS)Then
          Call RaysPoly(TRIBUFF,3,9,VCOLR,PNORML,OFILE)

        Else If(OUTTING.EQ.OUTPOV)Then
          Call PovPoly(TRIBUFF,3,9,VCOLR,PNORML,OFILE)

        End If

C      END IF

      END DO
      IF (OUTTING.EQ.OUTWINDOW)Call G0END(RSTYLE,G0Triangle)
C
 999  CONTINUE
      RETURN
      END
C
C Routine to facilitate polygon rendering (reptype) style
      Subroutine G0BGN(STYLE, TYPE)
      Implicit None
      Integer STYLE
      Integer TYPE
#include "G0COMMON.FOR"
C
      If(STYLE.eq.G0ShowFilled)Then
        If(TYPE.EQ.G0Polygon)Then
          call bgnpol
        Else If(TYPE.EQ.G0QuadStrip)Then
          call bgnqst
        Else If(TYPE.EQ.G0TriStrip)Then
           call bgntst
        Else If(TYPE.EQ.G0Triangle)Then
          call bgntri
        End If
      Else If(STYLE.EQ.G0ShowNormals)Then
      Else If(STYLE.eq.G0ShowLines) Then
        If(TYPE.EQ.G0QuadStrip .OR. TYPE.EQ.G0Triangle)Then
          call bgnseg
        Else
          call bgnlin
        End If
      Else If(STYLE.eq.G0ShowPoints)Then
        Call bgnpoi
      End If
      Return
c
      Entry G0END(STYLE,TYPE)
      Call endany
      Return
      End
C
      Integer Function G0YSTRIP(NX, NY, NZ, CWISE, PNORML, NBUF, DBUF,
     &     STYLE)
      IMPLICIT NONE
      INTEGER NX, NY, NZ 
      Logical CWISE
      Integer PNORML
      Integer STYLE
      Real NBUF(3,2,NX,NY)
      Real DBUF(3,  NX,NY)
C
#include "G0COMMON.FOR"
C
      integer i,j,k,j1,j2,jend
      Real NML(3), RTMP(3)
c
      If(NZ.eq.0)Then
        jend=NY-2
      Else
        jend=NY-1
      End If
      Do j=0,jend
C   Call G0BGN(STYLE,G0QuadStrip)
      Call G0BGN(STYLE,G0TriStrip)
      Do i=1,NX
      If(CWISE)Then
        j2=MOD(j+1, NY) + 1
        j1=j + 1
      Else
        j1=MOD(j+1, NY) + 1
        j2=j + 1
      End If
      If(PNORML.NE.0)Then
        call f3n( NBUF(1,2,i,j1) )
        call v3f( NBUF(1,1,i,j1) )
        If(STYLE.eq.4)Then
          Do k=1,3
          RTMP(k)=NBUF(k,1,i,j1)+NBUF(k+3,1,i,j1)
          End Do
          call v3f( RTMP )
          call v3f( NBUF(1,1,i,j1) )
        End If
        call f3n( NBUF(1,2,i,j2) )
        call v3f( NBUF(1,1,i,j2) )
        If(STYLE.eq.4)Then
          Do k=1,3
          RTMP(k)=NBUF(k,1,i,j2)+NBUF(k+3,1,i,j2)
          End Do
          call v3f( RTMP )
          call v3f( NBUF(1,1,i,j2) )
        End If
      Else
c n,7,3 ... 1,8,-3 is like n,7,2(not n,7,1) .... 1,8,-3
c In other words, if I compute normals, they always face front.
        if(i.lt.NX)Then
          CALL G0NRML( DBUF(1,i,j1), DBUF(1,i,j2), DBUF(1,i+1,j1), NML )
C         write(15,'(3f8.2,3i3)')
C    &     DBUF(1,i,j1), DBUF(2,i,j1), DBUF(3,i,j1), i,j1,j2
C         write(15,'(3f8.2)')
C    &     NML(1)+DBUF(1,i,j1), NML(2)+DBUF(2,i,j1), NML(3)+DBUF(3,i,j1)
          call f3n( NML )
          If(STYLE.eq.4)Then
            Do k=1,3
              RTMP(k)=DBUF(k,i,j1)+NML(k)
            End Do
            call v3f( DBUF(1,i,j1) )
            call v3f( RTMP )
          End If
        End If
        call v3f( dbuf(1,i,j1) )
        call v3f( dbuf(1,i,j2) )
      End If
      End Do
      Call G0END(STYLE,2)
      End Do
      G0YSTRIP=JEND+1
      Return
      End
C
      Integer Function G0XSTRIP(NX, NY, NZ, CWISE, PNORML, NBUF, DBUF,
     &     STYLE)
      IMPLICIT NONE
      INTEGER NX, NY, NZ 
      Logical CWISE
      Integer PNORML
      Integer STYLE
      Real NBUF(3,2,NX,NY)
      Real DBUF(3,  NX,NY)
C
#include "G0COMMON.FOR"
C
      integer i,j,k
      Integer i1,i2,iend
      Real NML(3), RTMP(3)
c
      If(NZ.eq.0)Then
        iend=NX-2
      Else
        iend=NX-1
      End If
      Do i=0,iend
C   Call G0BGN(STYLE,G0QuadStrip)
      Call G0BGN(STYLE,G0TriStrip)
      Do j=1,NY
      If(CWISE)Then
        i2=MOD(i+1, NX) + 1
        i1=i + 1
      Else
        i1=MOD(i+1, NX) + 1
        i2=i + 1
      End If
      If(PNORML.NE.0)Then
        call f3n( NBUF(1,2,i2,j) )
        call v3f( NBUF(1,1,i2,j) )
        If(STYLE.eq.4)Then
          Do k=1,3
            RTMP(k)=NBUF(k,1,i2,j)+NBUF(k+3,1,i2,j)
          End Do
          call v3f( RTMP )
          call v3f( NBUF(1,1,i2,j) )
        End If
        call f3n( NBUF(1,2,i1,j) )
        call v3f( NBUF(1,1,i1,j) )
        If(STYLE.eq.4)Then
          Do k=1,3
            RTMP(k)=NBUF(k,1,i1,j)+NBUF(k+3,1,i1,j)
          End Do
          call v3f( RTMP )
          call v3f( NBUF(1,1,i1,j) )
        End If
      Else
c n,7,3 ... 1,8,-3 is like n,7,2(not n,7,1) .... 1,8,-3
c In other words, if I compute normals, they always face front.
        if(j.lt.NY)Then
          CALL G0NRML( DBUF(1,i2,j), DBUF(1,i1,j), DBUF(1,i2,j+1), NML )
          call f3n( NML )
          If(STYLE.eq.4)Then
            Do k=1,3
              RTMP(k)=DBUF(k,i2,j)+NML(3)
            End Do
            call v3f( DBUF(1,i2,j) )
            call v3f( RTMP )
          End If
        End If
        call v3f( dbuf(1,i2,j) )
        call v3f( dbuf(1,i1,j) )
      End If
      End Do
      Call G0END(STYLE,2)
      End Do
      G0XSTRIP=IEND+1
      Return
      End
C
      INTEGER FUNCTION G0MKMSH(NX, NY, NZ, G0TBUF, TRIMAX, i1, i2, i3)
C
      IMPLICIT NONE
      INTEGER NX, NY, NZ, TRIMAX, i1, i2, i3
      INTEGER G0TBUF(3,TRIMAX)
C
      integer i,j,k,m
      integer nxy, mxy, mz, mxyz, ntri
C
        IF(NZ.LE.1)THEN
        if(NZ.EQ.1)then
          nxy=NX*NY
          NTRI=(NX-1)*NY*2
        else
          nxy=NX*(NY-1)
          NTRI=(NX-1)*(NY-1)*2
        end if
        NZ=1
      ELSE
        NTRI=(NX-1)*(NY-1)*2*NZ*3
        nxy=NX*(NY-1)
      END IF
      IF(NTRI.GT.TRIMAX)THEN
        G0MKMSH=NTRI
        RETURN
      END IF
C
      M=0
      mxy=NX*NY
      mxyz=MXY*NZ
      do k=1,NZ
      mz=(k-1)*NX*NY
      do j=mz+1,mz+nxy,NX
      do i=j,j+NX-2
      M=M+1
      G0TBUF(i1,M)=MOD(I-1,mxyz)
      G0TBUF(i2,M)=MOD(NX+I,mxyz)
      G0TBUF(i3,M)=MOD(NX+I-1,mxyz)
      M=M+1
      G0TBUF(i1,M)=MOD(I-1,mxyz)
      G0TBUF(i2,M)=MOD(I,mxyz)
      G0TBUF(i3,M)=MOD(NX+I,mxyz)
      if(k.ne.NZ)then
        M=M+1
        G0TBUF(i1,M)=I-1
        G0TBUF(i2,M)=I-1+mxy
        G0TBUF(i3,M)=I-1+mxy+NX
        M=M+1
        G0TBUF(i1,M)=I-1
        G0TBUF(i2,M)=I-1+mxy+NX
        G0TBUF(i3,M)=I-1+NX
        M=M+1
        G0TBUF(i1,M)=I-1
        G0TBUF(i2,M)=I
        G0TBUF(i3,M)=mxy+I
        M=M+1
        G0TBUF(i1,M)=I-1
        G0TBUF(i2,M)=mxy+I
        G0TBUF(i3,M)=I-1+mxy
      end if
      end do
      end do
      end do
C
      IF(NTRI.NE.M)then
        write(*,*)'G0MKMESH: M != NTRI',M,NTRI
        NTRI=M
      end if
      G0MKMSH=M
      RETURN
      END
C
      SUBROUTINE G0NRML( A, B, C, N )
      REAL A(3), B(3), C(3), N(3)
C
      REAL V1(3), V2(3)
      REAL NL
C
C  FOR TRIANGLE WITH POINTS A,B,C, FIND NORMAL TO IT.
C
C  First, let v1=a-c and v2=b-c (vectors)
C
      DO 10 I=1,3
      V1(I)=A(I)-C(I)
      V2(I)=B(I)-C(I)
 10   CONTINUE
C
C  Then take cross v1xv2 and normalize to 1.
C
      NL=0.0
      DO 100 I=1,3
      J=MOD(I,3)+1
      K=MOD(I+1,3)+1
      N(I)=V1(J)*V2(K) - V2(J)*V1(K)
      NL=NL+N(I)**2
 100  CONTINUE
C
      NL=SQRT(NL)
c  write(*,*)'normal length = ', nl
      IF(NL.LT.1.E-8) Then
        write(*,*)'teeny normal length = ', nl
        RETURN
      End If
C
      DO 200 I=1,3
      N(I)=N(I)/NL
 200  CONTINUE
c  write(*,*)'normalized = ',n
C
      RETURN
      END
C
C
      SUBROUTINE G0NMLZ( XYZ, NML, REVERSE)
C
      REAL XYZ(3), NML(3)
      Integer REVERSE
C
C Normalize a vector x,y,z
C
      REAL NMLLEN
C
      If (xyz(1).eq.0 .and. xyz(2).eq.0 .and. xyz(3).eq.0) Then
        write(*,*)'nmlz got zero normal!'
        NMLLEN=1
      Else
        NMLLEN=SQRT(xyz(1)*xyz(1) + xyz(2)*xyz(2) + xyz(3)*xyz(3))
      End If
      NML(1)=XYZ(1)/NMLLEN
      NML(2)=XYZ(2)/NMLLEN
      NML(3)=XYZ(3)/NMLLEN
      If(REVERSE.EQ.-1) Then
        NML(1)=-NML(1)
        NML(2)=-NML(2)
        NML(3)=-NML(3)
      End If
C
      RETURN
      END
C
      SUBROUTINE G0DRAW(X,Y,Z)
      IMPLICIT NONE
      REAL              X,Y,Z
C
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
C   DRAW TO POINT X,Y,Z
C
      IF(OUTTING.EQ.OUTASC)Then
        write(OFILE,*)'L ',X,Y,Z
      Else IF(OUTTING.EQ.OUTWINDOW .OR. OUTTING.EQ.OUTPOSTSCR
     &  .OR. OUTTING.EQ.OUTRAYS .OR. OUTTING.EQ.OUTRAYT
     &  .OR. OUTTING.EQ.OUTPOV)THEN
        G0PTYP=G0LINES
        IF(G0PCNT.GE.G0PMAX)THEN
          CALL G0POUT
        END IF
        G0PCNT=G0PCNT+1
c   write(6,*)'drawing to buffer at ',G0PCNT
        IF(G0PCNT.GT.0)THEN
          G0DBUF(1,G0PCNT)=X
          G0DBUF(2,G0PCNT)=Y
          G0DBUF(3,G0PCNT)=Z
        ELSE
          CALL TELLUSER1(NEVER)
          RETURN
        END IF
      END IF
C
      RETURN
C
      ENTRY      G0MOVE(X,Y,Z)
C
C   MOVE TO POINT X,Y,Z
C
      IF(OUTTING.EQ.OUTASC)Then
        write(OFILE,*)'P ',X,Y,Z
      Else IF(OUTTING.EQ.OUTWINDOW .OR. OUTTING.EQ.OUTPOSTSCR
     &  .OR. OUTTING.EQ.OUTRAYS .OR. OUTTING.EQ.OUTRAYT
     &  .OR. OUTTING.EQ.OUTPOV)THEN
        IF(G0PCNT.GE.2)THEN
          CALL G0POUT
        END IF
        G0PCNT=1
c   write(6,*)'move added to buffer at ',G0PCNT
          G0DBUF(1,G0PCNT)=X
          G0DBUF(2,G0PCNT)=Y
          G0DBUF(3,G0PCNT)=Z
      END IF
C
      RETURN
C
      ENTRY      G0DOT(X,Y,Z)
C
C   DRAW A DOT AT X,Y,Z
C
      IF(OUTTING.EQ.OUTASC)Then
        write(OFILE,*)'P ',X,Y,Z
      Else IF(OUTTING.EQ.OUTWINDOW .OR. OUTTING.EQ.OUTPOSTSCR
     &  .OR. OUTTING.EQ.OUTRAYS .OR. OUTTING.EQ.OUTRAYT
     &  .OR. OUTTING.EQ.OUTPOV)THEN
      G0PTYP=G0DOTS
        IF(G0PCNT.GE.G0PMAX)THEN
          CALL G0POUT
        END IF
        G0PCNT=G0PCNT+1
c   write(6,*)'points added to buffer at ',G0PCNT
        G0DBUF(1,G0PCNT)=X
        G0DBUF(2,G0PCNT)=Y
        G0DBUF(3,G0PCNT)=Z
      END IF
      Return
C
      ENTRY      G0SPOT(X,Y,Z)
C
C   DRAW A 'SPOT' AT X,Y,Z
C
      IF(OUTTING.EQ.OUTASC)Then
        write(OFILE,*)'P ',X,Y,Z
      Else IF(OUTTING.EQ.OUTWINDOW .OR. OUTTING.EQ.OUTPOSTSCR
     &  .OR. OUTTING.EQ.OUTRAYS .OR. OUTTING.EQ.OUTRAYT
     &  .OR. OUTTING.EQ.OUTPOV)THEN
      G0PTYP=G0SPTS
        IF(G0PCNT.GE.G0PMAX)THEN
          CALL G0POUT
        END IF
        G0PCNT=G0PCNT+1
c   write(6,*)'points added to buffer at ',G0PCNT
        G0DBUF(1,G0PCNT)=X
        G0DBUF(2,G0PCNT)=Y
        G0DBUF(3,G0PCNT)=Z
      END IF
C
      RETURN
      END
C
      INTEGER FUNCTION genobj()
      IMPLICIT NONE
#include "G0COMMON.FOR"
      G0ID = G0ID + 1
      genobj = G0ID
      RETURN
      END
C
      SUBROUTINE G0CYL(X,Y,Z,CYLRAD)
      IMPLICIT NONE
C
      REAL X,Y,Z,CYLRAD
      INTEGER CYLN
C
#include "G0COMMON.FOR"
#include "ERRORS.FOR"
#include "UDLIST.FOR"
C
      INTEGER USE3
      REAL AMATRIX(4,4), IMATRIX(4,4)
      REAL COOR1(4), COOR2(4), COOR3(4)
      REAL CLEN
      INTEGER T1, T2, T3
      INTEGER I, L0, L1, L2, J, NX, NY, NZ
      REAL OLDX, OLDY, OLDZ, OLDW
      External SCALE
      EQUIVALENCE (COOR1(1), OLDX), (COOR1(2), OLDY), (COOR1(3), OLDZ),
     &  (COOR1(4), OLDW)
      INTEGER NTP, NTV
      SAVE NTP, NTV
C
      IF(OUTTING.EQ.OUTWINDOW .OR. OUTTING.EQ.OUTPOSTSCR
     &  .OR. OUTTING.EQ.OUTRAYS .OR. OUTTING.EQ.OUTRAYT
     &  .OR. OUTTING.EQ.OUTPOV)THEN
        G0PTYP=G0CYLS
        IF(G0PCNT.LT.1)RETURN
        OLDX=G0DBUF(1,G0PCNT)
        OLDY=G0DBUF(2,G0PCNT)
        OLDZ=G0DBUF(3,G0PCNT)
        OLDW=1.
c    write(*,'(a,7g10.3)')
c     &    'cylinder ',oldx,oldy,oldz,x,y,z,cylrad
        G0PCNT=G0PCNT+1
        G0DBUF(1,G0PCNT)=X
        G0DBUF(2,G0PCNT)=Y
        G0DBUF(3,G0PCNT)=Z
C????    if(ntp.eq.0)return
        COOR2(1)=X
        COOR2(2)=Y
        COOR2(3)=Z
        COOR2(4)=1.

        If(OUTTING.EQ.OUTWINDOW)THen
          USE3=0
          OUTTING=0
          CALL AAXIS(COOR1,COOR2,COOR3,AMATRIX,IMATRIX,USE3)
          OUTTING=OUTWINDOW
          CALL PUSHMA
          CALL TMUL(IMATRIX)
          CLEN=SQRT( (X-OLDX)**2 + (Y-OLDY)**2 + (Z-OLDZ)**2 )
          CALL SCALE(CYLRAD, CYLRAD, CLEN)
          Call callob(G0CYL0)
          CALL POPMAT

        Else If(OUTTING.EQ.OUTPOSTSCR)Then
          write(OFILE,'(5f15.7,a)')OLDX,OLDY,X,Y,CYLRAD,' cylinder'

        Else If(OUTTING.EQ.OUTRAYT)Then
          write(OFILE,'(a)')'cylinder'
          write(OFILE,'(3(a,3g12.4))')
     &      '   point ',oldx,oldy,oldz,'   point ',x,y,z,
     &      ' radius ',cylrad
          Call g0raysurf(OFILE)

        Else If(OUTTING.EQ.OUTRAYS)Then
          Call g0raysurf(OFILE)
          write(OFILE,'(a,g12.4)')' cylinder ', CYLRAD
          write(OFILE,'(6g12.4)')oldx,oldy,oldz,x,y,z

        Else If(OUTTING.EQ.OUTPOV)Then
          Call g0raysurf(OFILE)
          write(OFILE,'(a)')' cylinder { '
          write(OFILE,'(a,3(g12.4,a))')'<',oldx,',',oldy,',',oldz,'>'
          write(OFILE,'(a,3(g12.4,a))')'<',x,',',y,',',z,'>'
          write(OFILE,'(g12.4)') CYLRAD
          write(OFILE,'(a)')'}'
        End If

      Else

        Return
      END IF

      RETURN
      END
C
      SUBROUTINE G0CYLINIT(LON, LORES, HIN, HIRES)
      IMPLICIT NONE
      Integer LON, LORES, HIN, HIRES
#include "G0COMMON.FOR"
C
      G0CYL1=0
      G0CYL2=0
      G0CYL0=0
      CALL G0CYLREDO(LON, LORES, HIN, HIRES)
      RETURN
      END
C
      SUBROUTINE G0CYLREDO(LON, LORES, HIN, HIRES)
      IMPLICIT NONE
      Integer LON, LORES, HIN, HIRES
#include "G0COMMON.FOR"
C
      INTEGER NX, NY, NZ, ntp, i
      Integer NORML
      Logical cwise
      INTEGER genobj
      INTEGER G0XSTRIP, G0YSTRIP
C
      ny=2
      nz=0
      NORML=1
      cwise=.true.
C
      Call g0cylmak(LON, g0pbuf)
      nx=LON+1
      If (G0CYL1 .eq. 0) Then
        G0CYL1=genobj()
      Else
        call delobj(G0CYL1)
      End If
      call makeob(G0CYL1)
      ntp=g0ystrip(nx,ny,nz,cwise,NORML,g0pbuf,g0dbuf,LORES)
      Call g0cylmakbase(LON, g0pbuf)
      Call G0POLY( g0pbuf, 1.0, nx, 3, 0,
     &    .FALSE., .FALSE., .FALSE. )
      Call TRANSL(0., 0., 1.)
      Call XROT(180.)
      Call G0POLY( g0pbuf, 1.0, nx, 3, 0,
     &    .FALSE., .FALSE., .FALSE. )
      call closeo
      G0CYLN1=LON
      G0CYLR1=LORES
C
      Call g0cylmak(HIN, g0pbuf)
      nx=HIN+1
      If (G0CYL2 .eq. 0) Then
        G0CYL2=genobj()
      Else
        call delobj(G0CYL2)
      End If
      call makeob(G0CYL2)
      ntp=g0ystrip(nx,ny,nz,cwise,NORML,g0pbuf,g0dbuf,HIRES)
      Call g0cylmakbase(HIN, g0pbuf)
      Call G0POLY( g0pbuf, 1.0, nx, 3, 0,
     &    .FALSE., .FALSE., .FALSE. )
      Call TRANSL(0., 0., 1.)
      Call XROT(180.)
      Call G0POLY( g0pbuf, 1.0, nx, 3, 0,
     &    .FALSE., .FALSE., .FALSE. )
      call closeo
      G0CYLN2=HIN
      G0CYLR2=HIRES

      If (G0CYL0 .EQ. 0) Then
        G0CYL0=genobj()
        G0RES=1
        CALL G0CYLEDIT(LON, LORES, HIN, HIRES)
      End If
C
      Return
      END
C
      SUBROUTINE G0CYLEDIT(LON, LORES, HIN, HIRES)
      IMPLICIT NONE
      Integer LON, LORES, HIN, HIRES
#include "G0COMMON.FOR"
#include "UDLIST.FOR"

      call delobj(G0CYL0)
      call makeob(G0CYL0)
      If (G0RES .EQ. 0) THEN
        call callob(G0CYL1)
      Else
        call callob(G0CYL2)
      End If
      call closeo
      DCHANGE=.TRUE.
C
      RETURN

      END
c
      SUBROUTINE G0SPHERE(X,Y,Z,SPHRAD)
      IMPLICIT NONE
C
      REAL X,Y,Z,SPHRAD
      INTEGER SPHN
C
#include "G0COMMON.FOR"
#include "ERRORS.FOR"
#include "UDLIST.FOR"
C
      INTEGER USE3
      REAL AMATRIX(4,4), IMATRIX(4,4)
      REAL COOR1(4), COOR2(4), COOR3(4)
      REAL CLEN
      INTEGER T1, T2, T3
      INTEGER I, L0, L1, L2, J, NX, NY, NZ
      REAL OLDX, OLDY, OLDZ, OLDW
      External SCALE
      EQUIVALENCE (COOR1(1), OLDX), (COOR1(2), OLDY), (COOR1(3), OLDZ),
     &  (COOR1(4), OLDW)
      INTEGER NTP, NTV
      SAVE NTP, NTV
C
        IF(OUTTING.EQ.OUTNIL)THEN
          RETURN
        Else IF(OUTTING.EQ.OUTRAYT)Then
                write(OFILE,'(a)')'sphere'
                write(OFILE,'(a,3g12.4,a,3g12.4)')'   center ',x,y,z,
     &          ' radius ',sphrad
                Call g0raysurf(OFILE)
        Else IF(OUTTING.EQ.OUTRAYS)Then
                Call g0raysurf(OFILE)
                write(OFILE,'(a,g12.4)')' sphere ',sphrad
                write(OFILE,'(3g12.4)')x,y,z
        Else IF(OUTTING.EQ.OUTPOV)Then
                Call g0raysurf(OFILE)
                write(OFILE,'(a,3(g12.4,a),g12.4,a)')
     &           'sphere {<', x,',',y,',',z,'>', sphrad,'}'
        Else IF(OUTTING.EQ.OUTPOSTSCR)Then
                write(OFILE,'(3f15.7,a)')X,Y,SPHRAD,' sphere'
        Else IF(OUTTING.EQ.OUTWINDOW)Then
              CALL PUSHMA
              CALL TRANSL(X, Y, Z)
              CALL SCALE(SPHRAD,SPHRAD,SPHRAD)
              Call callob(g0sph0)
              CALL POPMAT
      End If
C
      RETURN
      END
C
      SUBROUTINE G0SPHINIT(LON, LORES, HIN, HIRES)
      IMPLICIT NONE
      Integer LON, LORES, HIN, HIRES
#include "G0COMMON.FOR"
C
      G0SPH1=0
      G0SPH2=0
      G0SPH0=0
      CALL G0SPHREDO(LON, LORES, HIN, HIRES)
      RETURN
      END
C
      SUBROUTINE G0SPHREDO(LON, LORES, HIN, HIRES)
      IMPLICIT NONE
      Integer LON, LORES, HIN, HIRES
#include "G0COMMON.FOR"
#include "ERRORS.FOR"
C
      INTEGER ntri, i
      Integer NORML
      Logical cwise
      INTEGER genobj
      INTEGER g0sphmak
C
      NORML=1
      cwise=.true.
C
      ntri = g0sphmak(LON, g0dbuf,G0PMAX*2)
      If (ntri .lt. 1) Then
        Call Telluser1(MAXTRI)
      Else
        If (G0SPH1 .eq. 0) Then
          G0SPH1=genobj()
        Else
          call delobj(G0SPH1)
        End If
        call makeob(G0SPH1)
        Call G0BGN(LORES,G0Triangle)
        do i = 1,ntri*3
          call f3n( g0dbuf(1,i) )
          call v3f( g0dbuf(1,i) )
        end do
        Call G0END(LORES,G0Triangle)
        call closeo
        G0SPHN1=LON
        G0SPHR1=LORES
      End If
C
      ntri = g0sphmak(HIN, g0dbuf,G0PMAX*2)
      If (ntri .lt. 1) Then
        Call Telluser1(MAXTRI)
      Else
        If (G0SPH2 .eq. 0) Then
          G0SPH2=genobj()
        Else
          call delobj(G0SPH2)
        End If
        call makeob(G0SPH2)
        Call G0BGN(HIRES,G0Triangle)
        do i = 1,ntri*3
          call f3n( g0dbuf(1,i) )
          call v3f( g0dbuf(1,i) )
        end do
        Call G0END(HIRES,G0Triangle)
        call closeo
        G0SPHN2=HIN
        G0SPHR2=HIRES
      End If

      If (G0SPH0 .EQ. 0) Then
        G0SPH0=genobj()
        G0RES=1
        CALL G0SPHEDIT(LON, LORES, HIN, HIRES)
      End If
C
      Return
      END
C
      SUBROUTINE G0SPHEDIT(LON, LORES, HIN, HIRES)
      IMPLICIT NONE
      Integer LON, LORES, HIN, HIRES
#include "G0COMMON.FOR"
#include "UDLIST.FOR"

      call delobj(G0SPH0)
      call makeob(G0SPH0)
      If (G0RES .EQ. 0) THEN
        call callob(G0SPH1)
      Else
        call callob(G0SPH2)
      End If
      call closeo
      DCHANGE=.TRUE.
C
      RETURN

      END
c
      SUBROUTINE G0HIRES(VAL)
      IMPLICIT NONE
#include "G0COMMON.FOR"
      INTEGER VAL
      IF (VAL .EQ. 0) THEN
        IF (G0RES .NE. 0) THEN
          G0RES = 0
          CALL G0CYLEDIT(0, 0, 0, 0)
          CALL G0SPHEDIT(0, 0, 0, 0)
        END IF
      ELSE
        IF (G0RES .NE. 1) THEN
          G0RES = 1
          CALL G0CYLEDIT(0, 0, 0, 0)
          CALL G0SPHEDIT(0, 0, 0, 0)
        END IF
      END IF
      END

      SUBROUTINE G0POUT
C
      IMPLICIT NONE
      INTEGER I,j
C
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
      Real NML(3)
C
C  OUTPUT THE POINT BUFFER
C
      IF(G0PCNT.LT.1)RETURN
      IF(OUTTING.EQ.OUTPOSTSCR)THEN
      If(G0PTYP.EQ.G0LINES)Then
        write(OFILE,'(2f15.7,a)')(G0DBUF(j,1),j=1,2),' moveto'
        Do i=2,G0PCNT
        write(OFILE,'(2f15.7,a)')(G0DBUF(j,i),j=1,2),' lineto'
        End Do
      Else If(G0PTYP.EQ.G0DOTS .or. G0PTYP.EQ.G0SPTS)Then
        Do i=1,G0PCNT
        write(OFILE,'(2f15.7,a)')
     &      (G0DBUF(j,i),j=1,2),' dot'
        End Do
      Else If(G0PTYP.EQ.G0CYLS)Then
C  Else If(G0PTYP.EQ.G0CONES)Then
      End If
      Else If(OUTTING.EQ.OUTRAYT)Then
      If(G0PTYP.EQ.G0LINES)Then
        Do i=1,G0PCNT-1
        write(OFILE,'(a)')'cylinder'
        write(OFILE,'(3(a,3g12.4))')
     &    '   point ',(G0DBUF(j,i),j=1,3),
     &    '   point ',(G0DBUF(j,i+1),j=1,3),
     &    ' radius ',G0RAYRAD
        If(i.eq.1)Call g0raysurf(OFILE)
        End Do
      Else If(G0PTYP.EQ.G0DOTS .or. G0PTYP.EQ.G0SPTS)Then
        Do i=1,G0PCNT
        write(OFILE,'(a)')'dot'
        write(OFILE,'(a,3g12.4)')'    center ',(G0DBUF(j,i),j=1,3)
c Don't know point normals, but if I guess that their components are
c the same as the location, it will be exactly right for a sphere, at least.
        Call G0NMLZ(G0DBUF(1,i),NML,1)
        write(OFILE,'(  a,3g12.4)')'    normal ',NML
        If(i.eq.1)Call g0raysurf(OFILE)
        End Do
      Else If(G0PTYP.EQ.G0CYLS)Then
C  Else If(G0PTYP.EQ.G0CONES)Then
      End If
      Else If(OUTTING.EQ.OUTRAYS)Then
      If(G0PTYP.EQ.G0LINES)Then
        Call g0raysurf(OFILE)
        Do i=1,G0PCNT-1
        write(OFILE,'(a,g12.4)')' cylinder ', G0RAYRAD
        write(OFILE,'(6g12.4)')
     &    (G0DBUF(j,i),j=1,3), (G0DBUF(j,i+1),j=1,3)
        End Do
      Else If(G0PTYP.EQ.G0DOTS .or. G0PTYP.EQ.G0SPTS)Then
        Call g0raysurf(OFILE)
        Do i=1,G0PCNT
        write(OFILE,'(a,g12.4)')' disc ',G0RAYRAD
        write(OFILE,'(3g12.4,a)') (G0DBUF(j,i),j=1,3), ' 0 0 1'
        End Do
      Else If(G0PTYP.EQ.G0CYLS)Then
C  Else If(G0PTYP.EQ.G0CONES)Then
      End If
      Else If(OUTTING.EQ.OUTWINDOW)THEN
c   write(6,*)'outputting buffer at G0PCNT = ',G0PCNT
        IF(G0PTYP.EQ.G0CYLS)THEN
        CONTINUE
C    Else IF(G0PTYP.EQ.G0CONES)THEN
        CONTINUE
        ELSE IF(G0PTYP.EQ.G0LINES)THEN
c   write(6,'(3f10.5)')
c    w    (((G0PBUF(i,j,k),i=1,3),j=1,2),k=1,g0pcnt)
c     write(6,*)'replacing element with line list of length',
c    w          G0PCNT
        call bgnlin
        DO I=1, G0PCNT
c    CALL MOVE( G0PBUF(1,1,I), G0PBUF(2,1,I), G0PBUF(3,1,I) )
c    CALL DRAW( G0PBUF(1,2,I), G0PBUF(2,2,I), G0PBUF(3,2,I) )
        call v3f(G0DBUF(1,i))
        END DO
        call endlin
        ELSE IF(G0PTYP.EQ.G0DOTS)THEN
c   write(6,'(3f10.5)')
c    w    (( G0DBUF(i,l)  ,i=1,3),             l=1,g0pcnt)
c     write(6,*)'replacing element with point list of length',
c    w          G0PCNT
        call bgnpoi
        DO I=1, G0PCNT
        call v3f(G0DBUF(1,i))
        END DO
        call endpoi
        ELSE IF(G0PTYP.EQ.G0SPTS)THEN
        Do i=1,G0PCNT
C    CALL CMOV( G0DBUF(1,I), G0DBUF(2,I), G0DBUF(3,I) )
C    CALL fmprstr('.')
        End Do
        ELSE
          CONTINUE
        END IF
      ELSE
      RETURN
      END IF
C
      G0PCNT=0
      RETURN
      END
C
      Subroutine g0raysurf(rayfile)
      IMPLICIT NONE
      Integer rayfile
      Real ka,kd,kp,kt,ks,r,g,b
      Save ka,kd,kp,kt,ks,r,g,b
      Logical srf0
      Save srf0
C
#include "UDLIST.FOR"
#include "G0COMMON.FOR"
c
c   To avoid writing out unecessary (long parsing) surface descriptions.
      if(srf0 .and. ka.eq.g0ambi .and. kd.eq.g0diff .and. kp.eq.g0shiny
     &     .and. kt.eq.g0alpha .and. ks.eq.g0spec .and.
     &     r.eq.g0red .and. g.eq.g0green .and. b.eq.g0blue)return
      srf0=.true.
c
      If(OUTTING.EQ.OUTRAYS)Then
        write(rayfile,'(a,3g10.3)')' surface current '
        write(rayfile,'(a,3g10.3)')'    body ',G0RED,G0GREEN,G0BLUE
        write(rayfile,'(a,3g10.3)')
     &    '    ambient ',
     &      G0RED*  (1.-G0ALPHA)*G0AMBI*(1.-G0FRONT),
     &      G0GREEN*(1.-G0ALPHA)*G0AMBI*(1.-G0FRONT),
     &      G0BLUE* (1.-G0ALPHA)*G0AMBI*(1.-G0FRONT),
     &    '    diffuse ',
     &      G0RED*  (1.-G0ALPHA)*G0DIFF*(1.-G0FRONT),
     &      G0GREEN*(1.-G0ALPHA)*G0DIFF*(1.-G0FRONT),
     &      G0BLUE* (1.-G0ALPHA)*G0DIFF*(1.-G0FRONT),
     &    '    specular ',
     &      G0RED*  (1.-G0ALPHA)*G0SPEC*(1.-G0FRONT),
     &      G0GREEN*(1.-G0ALPHA)*G0SPEC*(1.-G0FRONT),
     &      G0BLUE* (1.-G0ALPHA)*G0SPEC*(1.-G0FRONT)
C the set shininess factor (G0SHINY) controls both specpow and reflect
c Don't ask me why 32, its a good guess that looks fine
        write(rayfile,'(a,g10.3)')
     &    '    specpow ',32.*G0SHINY,
     &    '    reflect ',    G0SHINY,
     &    '    transp ', G0ALPHA,
     &    '    index ', 1.
        write(rayfile,'(a,a)')' applysurf current'
c
      Else If(OUTTING.EQ.OUTRAYT)Then
c
        write(rayfile,'(a,3g10.3)')'   color ',G0RED,G0GREEN,G0BLUE
        write(rayfile,'(6x,a,g10.3)')
     &    ' ka ', G0AMBI,
     &    ' kd ', G0DIFF,
     &    ' kp ', G0SPEC
        write(rayfile,'(6x,a,g10.3)')
     &    ' ks ', G0SHINY,
     &    ' kn ', 128.*G0SHINY,
     &    ' kt ',  G0ALPHA,
     &    ' ri ', 1.

      Else If(OUTTING.EQ.OUTPOV)Then
        write(rayfile,'(a)')'#default { '
        write(rayfile,'(a,4(a,g10.3),a)')'finish { ',
     &  ' ambient ', G0AMBI,    ' diffuse ', G0DIFF,
     &  ' specular ',G0SPEC*0.5,'roughness ',G0SHINY*0.10,'}'
        write(rayfile,'(a,4(a,g10.3),a)')'pigment{color',
     &  ' red ',G0RED,' green ',G0GREEN,' blue ',G0BLUE,
     &  ' filter ', G0ALPHA, '}'
        write(rayfile,'(a)')'}'

      End If
c
      ka=g0ambi
      kd=g0diff
      kp=g0shiny
      kt=g0alpha
      ks=g0spec
      r=g0red
      g=g0green
      b=g0blue
c
      Return
c
      ENTRY g0raysrf0
c
      srf0=.false.
      return
      End
c
      SUBROUTINE G0ERASE(FLAG)
      IMPLICIT NONE
      Integer FLAG
C
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
      INTEGER TMPCOL
C
C   ERASE THE SCREEN
C
      IF(OUTTING.EQ.OUTNIL)THEN
        RETURN

      ELSE IF(FLAG.EQ.0)Then
        RETURN

      ELSE IF(OUTTING.EQ.OUTWINDOW)THEN
c    call viewpo(0,G0XWIND-1, 0,G0YWIND-1)
        IF(G0RGB)THEN
C      CALL G0RGBCOL(G0BKRGB(1),G0BKRGB(2),G0BKRGB(3), 1.0)
        ELSE
          TMPCOL=G0BKCOL
          CALL COLOR(TMPCOL)
        END IF
        IF(HIDESMODE)THEN
          CALL HIDESURF(1)
          CALL CZCLEA(G0BKRGB(1),G0BKRGB(2),G0BKRGB(3), 1.0, 1.0)
C       IF(HITTING.GT.0 .OR. FLAG.EQ.2)THEN
C       CALL CZCLEA(G0BKCOL, '7FFFFE'X)
C       ELSE
C       CALL CZCLEA(G0BKCOL, '7FFFFF'X)
C       END IF
c      write(*,*)'zclear'
        ELSE
          CALL HIDESURF(0)
          CALL CLEAR
        END IF

      ELSE IF(OUTTING.EQ.OUTTEK4010)THEN
        CONTINUE

      ELSE
        CALLTELLUSER1(BADOUTTYPE)
        RETURN
      END IF
C
      END
C
      SUBROUTINE G0COLOR(IID)
      IMPLICIT NONE
      INTEGER          IID
C
C
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
      REAL             HUE,SAT,VALU
      INTEGER          Z1, Z2
      INTEGER          LOW, HIGH
      REAL             R,G,B
      REAL             ALPHA
      Integer zmask
      INTEGER IHUE, ISAT, IVAL, IALPHA, MID, ILEN
      Logical APLANES
      Real H,S,BR
      REAL PHASE
      Parameter (PHASE=360/3)
      REAL DMAX 
      REAL RMAX, GMAX, BMAX, RMIN, GMIN, BMIN
      REAL MCOL
      REAL ambient, diffuse, specular, emissive, shiny
      INTEGER nbits
C  INTEGER genobj
c  SAVE UNIQ
c  DATA UNIQ/100000/
C
C   SET THE CURRENT HUE, SATURATION, VALUE and ALPHA
C  AND INTENSITIES of SPECULAR, DIFFUSE, AMBIENT, SHINY and EMISSIVE.
C
C HITTING = 1 means hit active, -1 means hit processing now
      If(IID.GT.0)Then
C       If (HITTING.NE.-1) Then
          call huesat(iid,hue,sat,valu)
          Call G0SETHSV(HUE,SAT,VALU)
          call g0inten(iid)
          alpha    = tfmval(uiTRNSP,iid)
          specular = tfmval(uiSPEC,iid)
          diffuse  = tfmval(uiDIFF,iid)
          ambient  = tfmval(uiAMBI,iid)
          shiny    = tfmval(uiSHIN,iid)
          emissive = tfmval(uiEMIS,iid)
C       End If
      Else
        hue=g0hue
        sat=g0sat
        valu=g0valu
        ambient=g0ambi
        diffuse=g0diff
        shiny=g0shiny
        specular=g0spec
        alpha=g0alpha
      End If

      IF(HUE.EQ.0 .AND. SAT.EQ.0 .AND. VALU.EQ.0)RETURN
      Z1=0
      Z2=X'7F0000'
C
      IF(OUTTING.EQ.OUTNIL)THEN
        RETURN

      Else If(OUTTING.EQ.OUTRAYS .OR. OUTTING.EQ.OUTRAYT .OR.
     &        OUTTING.EQ.OUTPOV)THEN
        IHUE=MOD(INT(120+360*HUE),360)
        ISAT=MOD(INT(100*SAT) ,101)
        IVAL=MOD(INT(100*VALU),101)
        CALL HSVRGB(IHUE, ISAT, IVAL, R, G, B)
        G0RED=R
        G0GREEN=G
        G0BLUE=B

      Else IF(OUTTING.EQ.OUTPOSTSCR)THEN
        H=MOD(INT(360*HUE+PHASE),360)/360.
        S=SAT
        BR=VALU
         If(S.EQ.0 .and. BR.EQ.1)Then
          write(OFILE,'(a)')' white'
        Else If(BR.EQ.0)Then
          write(OFILE,'(a)')' black'
        Else
          write(OFILE,'(3f15.7,a)')H,S,BR,' newcolor'
        End If

      ELSE IF(OUTTING.EQ.OUTWINDOW)THEN
C HITTING = 1 means hit active, -1 means hit processing now
C        If (HITTING.EQ.-1) Then
C           Call c3i(0,iid,0)
C        Else 
          IF(G0RGB)THEN
           IHUE=MOD(INT(120+360*HUE), 360)
           ISAT=MOD(INT(100*SAT), 101)
           IVAL=MOD(INT(100*VALU),101)
           CALL HSVRGB(IHUE, ISAT, IVAL, R, G, B)
           emissive = -1.0
C set material seperately
           IF(G0SRCE.LT.0 .OR. G0DEST.LT.0) Then
             if (iid.gt.0)nbits = trpat(alpha);
           Else
             if (iid.gt.0)nbits = trpat(0.0);
           End If
           call g0material(R, G, B, alpha, ambient, diffuse, specular,
     &      emissive, shiny, 0, g0bfrgb)
           if (backface .ne. 0) Then
             call g0material(R, G, B, alpha, ambient, diffuse, specular,
     &       emissive, shiny, backface, g0bfrgb)
           End If
          End If
C        END IF
        return

      ELSE IF(OUTTING.EQ.OUTTEK4010)THEN
        CONTINUE

      ELSE
        CALLTELLUSER1(BADOUTTYPE)
        RETURN
      END IF
C
      Return
      End
C
      Subroutine G0SETHSV(HUE,SAT,VALU)
      IMPLICIT NONE
      REAL             HUE,SAT,VALU
C
#include "G0COMMON.FOR"
      G0HUE=HUE
      G0SAT=SAT
      G0VALU=VALU
      Return
      END
C
      SUBROUTINE G0RGBCOL( R, G, B, VALPHA )
      REAL R,G,B, VALPHA
      real rgba(4)
c
      rgba(1)=r
      rgba(2)=g
      rgba(3)=b
      rgba(4)=valpha
c    write(*,*)'G0RGBCOL: ALPHA = ',valpha
      call c4f( rgba )
c
      RETURN
      END
C
      SUBROUTINE G0SETSPEC(FRACTION)
      IMPLICIT NONE
      REAL FRACTION, STYLE
C
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
      G0SPEC=FRACTION
      RETURN
C
      ENTRY G0SETDIFF(FRACTION)
      G0DIFF=FRACTION
      RETURN
C
      ENTRY G0SETAMBI(FRACTION)
      G0AMBI=FRACTION
      RETURN
C
      ENTRY G0SETEMIS(FRACTION)
      G0EMIS=FRACTION
      RETURN
C
      ENTRY G0SETSHIN(FRACTION)
      G0SHINY=FRACTION
      RETURN
C
      ENTRY G0SETSTYL(STYLE)
      G0RSTYLE=STYLE
      RETURN
C
      END
C
      SUBROUTINE G0BLINK(ONOFF)
      IMPLICIT NONE
      LOGICAL ONOFF
C
#include "G0COMMON.FOR"
C
C   MAKE ALL SUBSEQUENT LINES BLINK
C
      G0BLNK=ONOFF
      END
C
      SUBROUTINE G0TEXT(X,Y,Z,STRING,ILEN,FFONT,XSCALE,YSCALE,CSIZE)
      IMPLICIT NONE
      REAL          X,Y,Z
      CHARACTER*(*) STRING
      CHARACTER*4   FFONT
      INTEGER       XSCALE,YSCALE,CSIZE
C
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
      INTEGER ILEN
      EXTERNAL TRIM
C
      ILEN=TRIM(STRING)
      IF(ILEN.LE.0)RETURN
      IF(OUTTING.EQ.OUTNIL)THEN
        RETURN
      ELSE IF(OUTTING.EQ.OUTWINDOW)THEN
        call g0drawtext(X,Y,Z,STRING(:ILEN),CSIZE)
C    CALL CMOV(X,Y,Z)
C   CALL CHARST(STRING,ILEN)
c  STRING(ILEN+1:ILEN+1)=CHAR(0)
c  write(*,*)'g0subs: ilen = ',ilen
C    CALL fmprstr(STRING(1:ilen)//char(0))
      ELSE IF(OUTTING.EQ.OUTPOSTSCR)THEN
        write(OFILE,'(a)')   '/Times-Roman findfont'
        write(OFILE,'(f15.7,a)')  CSIZE/72., ' scalefont setfont'
        write(OFILE,'(2f15.7,a)') X,Y,' moveto'
        write(OFILE,'(a,a,a)')   '(' , STRING(1:ILEN) , ') show'
      ELSE
        CALLTELLUSER1(BADOUTTYPE)
        RETURN
      END IF
C
      END
C
      SUBROUTINE G0DASH(DASHPAT)
      IMPLICIT NONE
      INTEGER         DASHPAT
C
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
C   SET PATTERN FOR SUBSEQUENT DASHED LINES
C
      G0PATT=DASHPAT
      END
C
      SUBROUTINE G0DBKCOL(R,G,B)
      IMPLICIT NONE
      REAL R,G,B
C
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
C   SET THE BACKGROUND COLOR
C  if colormap mode, use first (R) as index
C
C  IF(G0RGB)THEN
C     G0BKCOL=R + LSHIFT(G,8) + LSHIFT(B,16)
C  ELSE
C    G0BKCOL=R
C  END IF
      G0BKRGB(1)=R
      G0BKRGB(2)=G
      G0BKRGB(3)=B
      END
C
      SUBROUTINE G0DBFCOL(R,G,B)
      IMPLICIT NONE
      REAL R,G,B
C
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
C   SET THE BACK-FACING Polygon COLOR
      G0BFRGB(1)=R
      G0BFRGB(2)=G
      G0BFRGB(3)=B
      END
C
      INTEGER FUNCTION G0IBKCOL()
      IMPLICIT NONE
C
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
      G0IBKCOL=G0BKCOL
      END
C
      SUBROUTINE G0DFONT(IFONT)
      IMPLICIT NONE
      INTEGER IFONT
C
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
      INTEGER i
      INTEGER fmfindfont, fmscalefont
      EXTERNAL TRIM
C
C   SET THE FONT
C
      If(ifont.LT.0 .OR. ifont.GT.6)Then
        ERROR=BADFONT
        Return
      Else
        G0FONT=ifont
      End If
C
      RETURN
      END
C
      Character*(*) Function G0IFONT()
      IMPLICIT NONE
C
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
      Integer i, fmgetfontname
C
      If(OUTTING.EQ.OUTNONE)Then
        G0IFONT=G0FNTNAM
      Else If(OUTTING.EQ.OUTWINDOW)Then
        If(G0FONT.EQ.0)Then
          G0IFONT='Hardware'
        Else
c      i=fmgetfontname(G0FONT, Len(G0IFONT), G0IFONT)
          i=0
          G0IFONT(i+1:)=' '
        End If
      End If
      Return
      End
C
      SUBROUTINE G0DTSIZE(TSIZE)
      IMPLICIT NONE
      INTEGER           TSIZE
C
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
C   SET THE CHARACTER SIZE
C
      G0TSIZE=TSIZE
      END
C
      INTEGER FUNCTION G0ITSIZE()
      IMPLICIT NONE
C
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
      G0ITSIZE=G0FONT
      END
C
      SUBROUTINE G0DXTSCALE(TSCALE)
      IMPLICIT NONE
      INTEGER             TSCALE
C
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
C   SET THE CHARACTER X SCALE
C
      G0XTSCALE=TSCALE
      END
C
      INTEGER FUNCTION G0IXTSCALE()
      IMPLICIT NONE
C
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
      G0IXTSCALE=G0XTSCALE
      END
C
      SUBROUTINE G0DYTSCALE(TSCALE)
      IMPLICIT NONE
      INTEGER             TSCALE
C
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
C   SET THE CHARACTER Y SCALE
C
      G0YTSCALE=TSCALE
      END
C
      INTEGER FUNCTION G0IYTSCALE()
      IMPLICIT NONE
C
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
      G0IYTSCALE=G0YTSCALE
      END
C
      SUBROUTINE G0DCOLORS(I,RGB)
C
      IMPLICIT NONE
C
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
      REAL RGB(3)
      INTEGER IR,IG,IB,I
C
      IR=RGB(1)*2.55
      IG=RGB(2)*2.55
      IB=RGB(3)*2.55
C  CALL MAPCOL(I,IR,IG,IB)
C
      RETURN
      END
C
      SUBROUTINE HSVRGB(IHUE,ISAT,IVAL, R,G,B)
C
      REAL LS,CIRCLE,SIXTY,ONE,ZERO,HUE,VAL,SAT,R,G,B,F,P,Q,T
C
      INTEGER I,IHUE,IVAL,ISAT
C
C     CONVERT HSV TO RGB
C
      DATA COLS/100./, CIRCLE/360./, SIXTY/60./, ONE/1.0/, ZERO/0.0/
C
      HUE = FLOAT(IHUE)
      VAL = FLOAT(IVAL) / COLS
      SAT = FLOAT(ISAT) / COLS
C
C     ACHROMATIC CASE ?
C
      IF (ISAT .NE. 0 ) GOTO 100
      R = VAL
      G = VAL
      B = VAL
      GOTO 200
  100 CONTINUE
C
C     OTHER CASES
C
      IF (HUE.EQ.CIRCLE) HUE = ZERO
      HUE = HUE/SIXTY
      I = INT (HUE)
      F = HUE - FLOAT(I)
      P = VAL * (ONE-SAT)
      Q = VAL * (ONE - (SAT*F) )
      T = VAL * (ONE - (SAT * (ONE - F)))
C
      GOTO  (10,20,30,40,50) ,I
      IF (I.NE.0) GOTO 999
    5 R = VAL
      G = T
      B = P
      GOTO 200
   10 R = Q
      G = VAL
      B = P
      GOTO 200
   20 R = P
      G = VAL
      B = T
      GOTO 200
   30 R = P
      G = Q
      B = VAL
      GOTO 200
   40 R = T
      G = P
      B = VAL
      GOTO 200
   50 R = VAL
      G = P
      B = Q
  200 CONTINUE
C
      RETURN
C
  999 CONTINUE
      WRITE(6,1000)
 1000 FORMAT(1X,'ERROR IN HSV, I NOT 0<=I<=5')
      END
C
      SUBROUTINE G0REDRAW(FORCE)
      Implicit NONE
      INTEGER FORCE
#include "UDLIST.FOR"
C
C Force a redraw: update display, erase(possibly) and show!
      IF (FORCE .NE. 0) DCHANGE = .TRUE.
      if (AUTOCLEAR) THEN
        Call G0REDISPLAY(1)
      Else
        Call G0REDISPLAY(0)
      EndIf
C      Call swapbu
      Return
      End
C
      SUBROUTINE G0REDISPLAY(EFLAG)
      IMPLICIT NONE
      Integer EFLAG
C
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
      IF(OUTTING.NE.OUTWINDOW)RETURN
      If(DCHANGE .OR. MCHANGE .OR. TCHANGE)Then
        IF(SKIPDISP)RETURN
        If(EFLAG.NE.0)CALL G0ERASE(EFLAG)
        CALL DISPLAY(0)
		  Call swapbu
      End If
C Calling Display has dealt with any ?CHANGES, so set them FALSE now
      TCHANGE=.FALSE.
      MCHANGE=.FALSE.
      DCHANGE=.FALSE.
C
      RETURN
      END
C
      SUBROUTINE G0INTEN(IID)
C
      IMPLICIT NONE
      Integer IID
C
      REAL IFRONT,ISIZE
C
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
C     REAL DMAX
C
C  G0FRONT represents how much dimmer than full the front is to be.
C  G0BACK  represents how much brighter than black the back is to be.
C
C  G0SPEC, G0DIFF, G0AMBI, G0SHINY, G0EMIS represent the amount
C  of specular, diffuse, ambient, shiny and emissive intensity to use.
C  G0ALPHA is the transparency.
C
C     DMAX=MAXDCUE*SMOOR-1
      G0ALPHA=tfmval(uiTRNSP,iid)
      G0SPEC =tfmval(uiSPEC,iid)
      G0DIFF =tfmval(uiDIFF,iid)
      G0AMBI =tfmval(uiAMBI,iid)
      G0SHINY=tfmval(uiSHIN,iid)
      G0EMIS =tfmval(uiEMIS,iid)
      IFRONT =wldval(wfog,LWORLD)
      ISIZE  =wldval(wdepth,LWORLD)
C
      G0FRONT = (1. - IFRONT)
      G0BACK  = (IFRONT + ISIZE)
c     write(*,*)'g0inten: ',ifront, isize, g0front, g0back
C
      IF(G0BACK .LT.0)G0BACK =0.
      IF(G0FRONT.LT.0)G0FRONT=0.
      IF(G0FRONT.GT.1.)G0FRONT=1.
      IF(G0BACK .GT.1.)G0BACK =1.
C
      IF( G0BACK.GT.(1.-G0FRONT) )G0BACK=1.-G0FRONT
C
C     write(*,*)'g0inten: ',ifront, isize, g0front, g0back
      RETURN
      END
C
      Subroutine g0delobj(fid)
      Implicit None
      Integer fid
#include "UDLIST.FOR"
C
      Call delobj(objfrid(fid))
      Return
      End
C
      INTEGER FUNCTION G0FILL(DLOC, DSIZE)
C
      IMPLICIT NONE
      REAL        DLOC
      INTEGER     DSIZE
C
      INTEGER           UNIQID
      INTEGER Z1, Z2
      INTEGER genobj
C
#include "G0COMMON.FOR"
#include "UDLIST.FOR"
#include "ERRORS.FOR"
C
C   dumpout will put in the moves and draws.
C   then close the gl object.
C
      if(outting.eq.OUTWINDOW)then

C g0inten is called in closeobj or g0refill, which call me
C        call g0inten(LWORLD)

        uniqid=genobj()
        call makeob(uniqid)
        Z1=0
        Z2=X'7F0000'
C    CALL LSETDE(Z1, Z2)
        call dumpout( DLOC, DSIZE )
C turn off any color material following that may have been activated
        call g0colormaterial(0)
        call closeo
        g0fill=uniqid
      else
        continue
      endif
C
      return
      end
C
      SUBROUTINE G0REFILL( AID )
      IMPLICIT NONE
      INTEGER  AID
      INTEGER G0FILL
C
#include "UDLIST.FOR"
C
      Call g0delobj(AID)
      Call g0inten(AID)
      OBJFRID(AID)=g0fill( dlist(objfrloc(AID)) , objfrsiz(AID) )
      RETURN 
      END
C
      SUBROUTINE G0SHADEM(MODEL)
C
      IMPLICIT NONE
      INTEGER MODEL
C
#include "UDLIST.FOR"
C
      IF(OUTTING.EQ.OUTNIL)THEN
        RETURN
      ELSE IF(OUTTING.EQ.OUTWINDOW)THEN
        call g0shading_model(MODEL)
      END IF
C
      RETURN
      END
C
      SUBROUTINE G0SETRGB(R,G,B)
      IMPLICIT NONE
      REAL R,G,B
#include "G0COMMON.FOR"
      G0RED=R
      G0GREEN=G
      G0BLUE=B
      RETURN
      END

      SUBROUTINE G0SETRGBMODE(MODE)
C
      IMPLICIT NONE
      LOGICAL MODE
C
#include "G0COMMON.FOR"
C
        G0RGB=MODE
        RETURN
        END
C
      SUBROUTINE G0PICK(iobj, mx, my, xyzw)
c
      IMPLICIT NONE
      INTEGER iobj, mx, my
      Real xyzw(4)
C
#include "UDLIST.FOR"
#include "TFMMAT.FOR"
#include "G0COMMON.FOR"
#include "DOLLARGS.FOR"
#include "ERRORS.FOR"
      integer i
      character*100 tmpline
      Integer WASWAIT
      Integer mxsav, mysav, mbsav
      SAVE mxsav, mysav, mbsav
      SAVE WASWAIT
      INTEGER genobj
      EXTERNAL TRIM
C
C      write(*,*) 'hit ',iobj,' at ',mx,my
c record the hit info: name and x,y,z
      if(hitsym.ne.0)then
        ARGLIST(hitsym,AGF)=namobj(iobj)
        If(PICKECHO.GT.0)Then
          WRITE(TMPLINE,'(A,I2,A,A)')'$',HITSYM,'=',namobj(iobj)
          CMDFROM=PICKECHO
          CALL GROUT(TMPLINE)
        End If
        HITSYM=0
      end if
C
C have display get the tfm matrix for this obj into TMPMAT in common
      HITOBJ = iobj
      Call display(0)
C
      Call G0WorldCoords(mx,my,tmpmat,xyzw)
      if(hitcoord.ne.0)then
C        Call G0WorldCoords(mx,my,tmpmat,xyzw)
        UCOORD(1,hitcoord)=xyzw(1)
        UCOORD(2,hitcoord)=xyzw(2)
        UCOORD(3,hitcoord)=xyzw(3)
        UCOORD(4,hitcoord)=xyzw(4)
        If(PICKECHO.GT.0)Then
          WRITE(TMPLINE,'(A,I2,A,4G13.6)')'#',hitcoord,'=',
     &     (xyzw(i),i=1,4)
          CMDFROM=PICKECHO
          CALL GROUT(TMPLINE)
        End If
        HITCOORD=0
      end if
 20   continue
C
      If(WAITING.NE.0)Then
C WAITING tells who was waiting for a pick - output to him from now on
        CMDFROM=WAITING
      End If
      WASWAIT=WAITING
C
C all done.  clear hit flags
      HITTING = 0
      HITOBJ = 0
      WASWAIT = 0
      WAITING = 0
c
c  Now finish up any gin files we were doing
c  agf is in COMMON and is modified by domore
c
c      Do While(agf.ge.2)
      Do While(agf.gt.1 .and. nadvance.eq.0)
        Call DOMORE
      End Do
C If we get to here, WAITING must be zero and agf = 1
C  If(WASWAIT.NE.0)Call GPROMPT
c
      RETURN
C
      ENTRY G0SHOWPICK
C
      write(tmpline,'(4(A,I5))')
     &  'Last pick at x = ',mxsav,' + ',G0XORIG,
     &       '; y = ',mysav,' + ',G0YORIG
      call grout(tmpline)
c
      RETURN
C
      END
C
      SUBROUTINE G0XFORM(X,Y,Z,MATRIX,NEWX,NEWY,NEWZ)
C
      REAL X,Y,Z,MATRIX(4,4),NEWX,NEWY,NEWZ
      REAL OLD(4), NEW(4)
      INTEGER I,J,K

      OLD(1)=X
      OLD(2)=Y
      OLD(3)=Z
      OLD(4)=1.
      DO 200 J=1,4
      NEW(J)=0.0
C
      DO 100 K=1,4
      NEW(J)=NEW(J) + OLD(K)*MATRIX(K,J)
 100  CONTINUE
 200  CONTINUE
C
      NEWX=NEW(1)/NEW(4)
      NEWY=NEW(2)/NEW(4)
      NEWZ=NEW(3)/NEW(4)
      RETURN
      END
C
C  Remember Hither and Yon for picking to compute world z.
C
      Subroutine G0HIYO(HI, YO, EY)
      G0HI=HI
      G0YO=YO
      G0EY=EY
      RETURN
      END
C
      SUBROUTINE G0LITS(ilight, LOBJ)
C
      IMPLICIT NONE
      INTEGER ilight, LOBJ, nlights
C
      REAL LPROPS(14)
C
#include "UDLIST.FOR"
#include "TFMMAT.FOR"
      INTEGER LNUM(NLITES)
      REAL R, G, B, RTOT, GTOT, BTOT
      Real x,y,z
      INTEGER IHUE, ISAT, IVAL
      REAL HUE, SAT, VALU
      SAVE RTOT, GTOT, BTOT, nlights
C
c kludge for rayshade,pov ambient light, initial call
      If(ilight.eq.0)Then
        rtot=0.
        gtot=0.
        btot=0.
        nlights=0
        Return
      End If
c
      call huesat(LOBJ, hue, sat, valu)
      IHUE=MOD(INT(120+360*HUE), 360)
      ISAT=MOD(INT(100*SAT), 101)
      IVAL=MOD(INT(100*VALU),101)
      CALL HSVRGB(IHUE, ISAT, IVAL, R, G, B)
      If(OUTTING.EQ.OUTWINDOW)Then
        call pushma
        call maketfm(LOBJ)
        call lmdef(ilight, R, G, B, TFMVAL(UIAMBI,LOBJ),
     &      TFMVAL(UIDIFF, LOBJ), TFMVAL(UISPEC, LOBJ))
        call lmbind(ilight,1)
        call popmat

      Else If (OUTTING.EQ.OUTRAYT)Then
        Call TIDENT
        Call maketfm(LOBJ)
        x=TFMMTX(3,1,CURRENT)*1000.
        y=TFMMTX(3,2,CURRENT)*1000.
        z=TFMMTX(3,3,CURRENT)*1000.
        write(OFILE,'(a)')'LIGHT'
        write(OFILE,'(a,3f15.7)')'   point ',x,y,z
        write(OFILE,'(a,3f15.7)')'   color ',R,G,B
        write(OFILE,'(a,3f15.7)')'   intensity ',
     &    TFMVAL(UIAMBI,LOBJ) + TFMVAL(UIDIFF,LOBJ)

      Else If (OUTTING.EQ.OUTRAYS)Then
        Call TIDENT
        Call maketfm(LOBJ)
        x=TFMMTX(3,1,CURRENT)
        y=TFMMTX(3,2,CURRENT)
        z=TFMMTX(3,3,CURRENT)
c    write(OFILE,'(a,3g12.4)')'   intensity ',R,G,B
        write(OFILE,'(a,3g12.4)')
     &    ' light ', LPROPS(6),LPROPS(7),LPROPS(8),
     &    '    directional ', x,y,z
c for rayshasde ambient light source, add up all lights ambient intensities
        rtot=rtot+LPROPS(2)
        gtot=gtot+LPROPS(3)
        btot=btot+LPROPS(4)

      Else If (OUTTING.EQ.OUTPOV)Then
        Call TIDENT
        Call maketfm(LOBJ)
        x=TFMMTX(3,1,CURRENT)*1000
        y=TFMMTX(3,2,CURRENT)*1000
        z=TFMMTX(3,3,CURRENT)*1000
c    write(OFILE,'(a,3g12.4)')'   intensity ',R,G,B
        write(OFILE,'(a,3(g12.4,a),a,3(a,g12.4),a,a)')
     &    ' light_source { < ',x,',',y,',',z,'>',
     &    'color ', ' red ', R,' green ',G, ' blue ',B,
     &    ' shadowless}'
        rtot=rtot+R
        gtot=gtot+G
        btot=btot+B
        nlights = nlights + 1

      End If
c
      return
c kludge for rayshade ambient light
      Entry G0ALIT
      If (OUTTING.EQ.OUTRAYS)Then
        write(OFILE,'(a,3f15.7,a)')
     &    ' light ',rtot, gtot, btot, ' ambient'
      Else If (OUTTING.EQ.OUTPOV)Then
        If (nlights > 0) Then
         write(OFILE,'(a,3(g12.4,a),a)')
     &   'global_settings { ambient_light <',
     &   0.4/nlights,',',0.4/nlights,',',0.4/nlights,'>','}'
        End If
      End If
      Return
C
      ENTRY G0NOLIT(ilight)
C
      call lmbind(ilight, 0)
      return
      end
C
      SUBROUTINE G0BLEND(FS, FD)
C
      IMPLICIT NONE
      INTEGER FS, FD
C
#include "UDLIST.FOR"
#include "G0COMMON.FOR"
      G0SRCE=fs
      G0DEST=fd
C
      ENTRY G0SBLEND(fs, fd)
C
      If(OUTTING.EQ.OUTWINDOW)Then
        call g0blendf(G0SRCE,G0DEST)
      End If

      return
      end
C
      SUBROUTINE G0MPASS
        IMPLICIT NONE
#include "UDLIST.FOR"
#include "G0COMMON.FOR"
        INTEGER NREAD, i, j
      integer wred, wgreen, wblue, walpha
      Integer ZEXTREM, zdiff
      Real zrange,lz
c
c  first attempt for multi-pass rendering to get alpha
c  blending (transparency) on non-alpha machines.
c  As of now (4/4/91) this routine reads the front buffer
c  and recreates it scan line at a time and re-writes it.
c  no visible change.  Eventually, render all alpha=0 into
c  one buffer, then each alpha!=0 one by one into other buffer
c  and blend into original buffer.
c
c  Never got around to alpha, let's try depth cueing.
C
      DCHANGE=.TRUE.
      Call g0redisplay(2)
      ZEXTREM='7FFFFF'X
      if(worksize.lt.g0xwind*2)return
        DO i=0,G0YWIND-1
C    CALL READSO(SRCFRO)
c  NREAD=LRECTR(0,i, G0XWIND-1,i, WORK)
C  NREAD=LRECTR(0,i, G0XWIND  ,i, WORK)
C    CALL READSO(SRCZBU)
c  NREAD=LRECTR(0,i, G0XWIND-1,i, WORK(G0XWIND+1))
C  NREAD=LRECTR(0,i, G0XWIND  ,i, WORK(G0XWIND+1))
      do j=1,g0xwind
      if(work(j).ne.0)then
        wred =   ibits(work(j), 0, 8)
        wgreen = ibits(work(j), 8, 8)
        wblue =  ibits(work(j), 16, 8)
        walpha = ibits(work(j), 24, 8)
c    write(*,*)i,j,wred,wgreen,wblue,walpha
        zdiff=ZEXTREM-work(j+G0XWIND)
        if(zdiff.le.1)then
          work(j)=wred + wgreen*256 + wblue*256*256
        else
c      if(lz.ne.zrange)write(*,*)i,j,zrange
          zrange=float(zdiff)/float(ZEXTREM)
          if(zrange.lt.0 .or. zrange.gt.1)write(*,*)i,j,zrange
          lz=(1.-zrange)
          work(j)=int(zrange*wred   + lz*g0bkrgb(1)) +
     &        int(zrange*wgreen + lz*g0bkrgb(2))*256 + 
     &        int(zrange*wblue  + lz*g0bkrgb(3))*256*256
        end if
      end if
      end do
c  work(1)=255
c  work(G0XWIND)=255*256
c  CALL LRECTW(0,i, G0XWIND-1,i, WORK)
C  CALL LRECTW(0,i, G0XWIND  ,i, WORK)
      END DO
C
      return
      END
C
      INTEGER FUNCTION G0DUMP(ORDER)
        IMPLICIT NONE
#include "UDLIST.FOR"
#include "G0COMMON.FOR"
        INTEGER NREAD, i, j
      LOGICAL ORDER
C
      if(worksize.lt.g0xwind .or. worksize.lt.g0ywind)then
        g0dump=-1
        return
      end if
c
c    WRITE(UNIT)G0XWIND, G0YWIND
      i= fcwrite(g0xwind,1)
      i= fcwrite(g0ywind,1)
C    CALL READSO(SRCFRO)
      IF(ORDER)THEN
        i= fcwrite(1,1)
          DO i=G0XWIND-1,0,-1
          NREAD=g0read_rgb_buff(i,0, i,G0YWIND-1, WORK)
        j= fcwrite(WORK, G0YWIND)
        END DO
      ELSE
        i= fcwrite(0,1)
          DO i=0,G0YWIND-1
          NREAD=g0read_rgb_buff(0,i, G0XWIND-1,i, WORK)
c      WRITE(UNIT)(WORK(j), j=1,G0XWIND)
        j= fcwrite(WORK, G0XWIND)
        END DO
      END IF
        G0DUMP=0
        RETURN
        END
C
      SUBROUTINE G0MODES
C
      Implicit None
#include "UDLIST.FOR"
#include "G0COMMON.FOR"
      IF(OUTTING.NE.OUTWINDOW)RETURN
C
C  G0RGB and DBLMODE in COMMON
C
      IF(G0RGB)THEN
c    CALL RGBMOD
      ELSE
c    CALL CMODE
      END IF
C
        IF(DBLMODE)THEN
c      CALL DOUBLE
        ELSE
c      CALL SINGLE
        END IF
C
      DCHANGE=.TRUE.
      RETURN
      END
C
      SUBROUTINE G0FPAUSE(SECONDS)
C
      REAL SECONDS
      INTEGER TICKS
C
C  Should agree with CLK_TCK in include file <limits.h>
      REAL TOCK
      PARAMETER (TOCK=100.)
C
      TICKS=SECONDS*TOCK
c  CALL SGINAP(TICKS)
C
      RETURN
      END
C
      SUBROUTINE G0FULSC(mdevx, mdevy)
        IMPLICIT NONE
      INTEGER mdevx, mdevy
#include "G0COMMON.FOR"
C#include <fdevice.h>
      INTEGER XOSAV, YOSAV, XWSAV, YWSAV, MXSAV, MYSAV
      LOGICAL FULLSC
      SAVE XOSAV, YOSAV, XWSAV, YWSAV, MXSAV, MYSAV
      SAVE FULLSC
      DATA FULLSC/.FALSE./
C
      IF(FULLSC)RETURN
      XOSAV=G0XORIG
      YOSAV=G0YORIG
      XWSAV=G0XWIND
      YWSAV=G0YWIND
      FULLSC=.TRUE.
      mxsav=mdevx
      mysav=mdevy
      if(mxsav.ge.0 .and. mysav.ge.0)then
      end if
      RETURN
C
      ENTRY G0SAVSC
      CALL G0WSET(XOSAV, YOSAV, XWSAV, YWSAV, 1)
      FULLSC=.FALSE.
      if(mxsav.ge.0 .and. mysav.ge.0)then
      end if
      RETURN
      END
c
      Subroutine f3n(dummy)
      Real dummy(3)
      If(dummy(1).eq.0 .and. dummy(2).eq.0 .and. dummy(3).eq.0)Then
        write(*,*)'f3n got zero normal!'
        return
      Else
        call n3f(dummy)
      End If
      Return
      End
