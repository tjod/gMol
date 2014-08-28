C-------------------------------------------------------------------------C
C                  Copyright (C) 1991-2010 TJ O'Donnell                   C
C-------------------------------------------------------------------------C
      implicit none
c
c      input:
c      nx, ny and nz are # of x, y and z values.
c      x, y and z are the coords of the grid points.
c      vdata is the scalar value at the grid points.
c      isoval is the value to contour at.
c      output:
c      nvert, ntri are the number of vertices and triangles.
c      note: number of normals equals number of vertices
c      vlist, is the list of vertices.
c
      integer nx, ny, nz, nvert, ntri
      integer nx0, ny0, nz0, nxx, nyy, nzz
      real isoval(10)
c
c     integer MAXDIM
c     PARAMETER (MAXDIM=100)
      integer MAXGRID
c     PARAMETER (MAXGRID=(MAXDIM+1)*(MAXDIM+1)*(MAXDIM+1))
      PARAMETER (MAXGRID=200*200*200)
      integer MAXTRI 
      PARAMETER (MAXTRI=50000)
      integer MAXVERT
      PARAMETER (MAXVERT=MAXTRI*3)
      INTEGER MAXTAGS
      PARAMETER (MAXTAGS=50000)
c
      Integer mtags, mtri, mvert
      real vdata(MAXGRID)
      integer tags(MAXGRID)
      real vlist(3,2,MAXVERT)
      real tlist(3,MAXTRI)
C      real parea(0:MAXTAGS)
C      real pvol (0:MAXTAGS)
      integer ntags
c
      character*80 file
      integer glen
      integer ngrid, nvox
      real x0, y0, z0, xx, yy, zz, dx, dy, dz, edge, prad
      real cx, cy, cz, wx, wy, wz
      integer i, j, k, niso
      real scale
      integer outfile
      character*3 xyz
      character*80 title
      real volume, bigvol, invol, outvol
      real area
      character*80 filename
      character*80 switch
      logical ask, volcomp, normcomp, prpvol
      character*10 askfmt
      integer PIX, QIX, NOOUT
      PARAMETER (NOOUT=4)
      PARAMETER (PIX=1)
      PARAMETER (QIX=2)
      integer iunit, punit, ounit
      Real tm(3,3), ti(3,3), cxyz(3), dxyz(3)
      logical mesh
      External Matmul
c
      mesh = .false.
      prpvol=.false.
      volcomp = .true.
      normcomp = .true.
      outfile = 0
      ask=.true.
      askfmt='(1x,a$)'
      iunit = 5
      punit=9
      ounit=11
c interp may use ounit+1 for DEBUG output
      mtags=MAXTAGS
      mtri=MAXTRI
      mvert=MAXVERT
      Call unit(tm)
      Call unit(ti)
c
c      Call readg94(punit,x0,y0,z0,xx,yy,zz,nx,ny,nz,xyz,title)
c
      call getarg(1, file);
      call readgrid(vdata, nx, ny, nz, x0, y0, z0, xx, yy, zz, file)
      write(*,*)'Coordinates from: ',x0, y0, z0
      write(*,*)'              to: ',xx, yy, zz
c
      write(*,'(a,3(''      1,'',i4))')'grid size: ',nx, ny, nz
      Call flush(6)
      nx0=1
      ny0=1
      nz0=1
      nxx=nx
      nyy=ny
      nzz=nz
c
c      call readdata(vdata, tags, nx, ny, nz, ntags, punit)
      dx=(xx-x0)/(nx-1)
      dy=(yy-y0)/(ny-1)
      dz=(zz-z0)/(nz-1)
c
C (New) contour from (same) data - see goto 25 below
 25   Continue
      write(*,askfmt)
     &      'enter number of, then those many contour values: '
      Call flush(6)
      read(iunit,*)niso, (isoval(i),i=1,niso)
      if(niso.le.0) stop 'Zero contours requested'
c
      write(*,askfmt)
     &      'enter output scale factor, '//
     &      'negative to reverse inside and outside: '
      Call flush(6)
      read(iunit,*)scale
      Call flush(6)
      write(*,askfmt)'enter output file name: '
      Call flush(6)
      read(iunit,'(a)')file
      if(index(file,'.qix').eq.0)then
        open(unit=ounit, file=file, status='unknown')
        outfile = PIX
      else
        open(unit=ounit, file=file, status='unknown',
     &       form='unformatted')
        outfile = QIX
      end if
c
c      Read request to contour only part of volume
      If(ask)write(*,askfmt)
     &        'Contour point and widths (0 0 0 0 0 0 for all): '
        Call flush(6)
      Read(iunit,*)cx,cy,cz,wx,wy,wz
c
      If(cx.eq.0 .and. cy.eq.0 .and. cz.eq.0 .and.
     &         wx.eq.0 .and. wy.eq.0 .and. wz.eq.0)Then
        nx0=1
        ny0=1
        nz0=1
        nxx=nx
        nyy=ny
        nzz=nz
      Else
        nx0=1 + (cx-wx-x0)/dx
        nxx=1 + (cx+wx-x0)/dx
        ny0=1 + (cy-wy-y0)/dy
        nyy=1 + (cy+wy-y0)/dy
        nz0=1 + (cz-wz-z0)/dz
        nzz=1 + (cz+wz-z0)/dz
        Write(0,'(a,6i5)')
     &        'Contour grid bounds:',nx0,nxx,ny0,nyy,nz0,nzz
        Call flush(6)
        If(nx0.lt.1 .or. nxx.lt.1 .or. ny0.lt.1 .or. nyy.lt.1 .or.
     &      nz0.lt.1 .or. nzz.lt.1 .or. nx0.gt.nx .or. nxx.gt.nx .or.
     &      ny0.gt.ny .or. nyy.gt.ny .or. nz0.gt.nz .or. nzz.gt.nz .or.
     &      nxx.lt.nx0 .or. nyy.lt.ny0 .or. nzz.lt.nz0)Then
          Write(*,*)'Contour point/width outside range of data'
          Call flush(6)
          If(nxx.gt.nx .or. nxx.lt.1)nxx=nx
          If(nyy.gt.ny .or. nyy.lt.1)nyy=ny
          If(nzz.gt.nz .or. nzz.lt.1)nzz=nz
          If(nx0.lt.1  .or. nx0.gt.nxx)nx0=1
          If(ny0.lt.1  .or. ny0.gt.nyy)ny0=1
          If(nz0.lt.1  .or. nz0.gt.nzz)nz0=1
          Write(0,'(a,6i5)')
     &      'Contour grid bounds:',nx0,nxx,ny0,nyy,nz0,nzz
          Call flush(6)
        End If
c      To draw a box enclosing the coutoured volume, get/name=conbox cube
c      where cube is a unit cube ($gramps_lib), and then....
        Write(0,'(4(a,f10.4))')'translate conbox x,',cx,
     &        ' y,',cy,' z,',cz,' w,',1.
        Call flush(6)
        Write(0,'(4(a,f10.4))')'scale     conbox x,',wx,
     &        ' y,',wy,' z,',wz,' w,',2.
        Call flush(6)
      End If 
c
c      To draw a box enclosing the entire volume, get/name=volbox cube
c      where cube is a unit cube ($gramps_lib), and then....
      cx=(xx+x0)/2.
      cy=(yy+y0)/2.
      cz=(zz+z0)/2.
      wx=(xx-x0)
      wy=(yy-y0)
      wz=(zz-z0)
      Write(0,'(4(a,f10.4))')'translate volbox x,',cx,
     &      ' y,',cy,' z,',cz,' w,',1.
      Call flush(6)
      Write(0,'(4(a,f10.4))')'scale     volbox x,',wx,
     &      ' y,',wy,' z,',wz,' w,',2.
      Call flush(6)
c
      nvox=(nx-1)*(ny-1)*(nz-1)
      bigvol=nvox*dx*dy*dz
c
C     do i=1,niso
C     do j=0,mtags
C     parea(j)=0.0
C     pvol(j) =0.0
C     end do
C
      write(*,*)'contour at ',isoval(i)
      Call flush(6)
      if(outfile .eq. QIX)then
        write(ounit)0,0,abs(scale)
        Call flush(6)
      else if(outfile .eq. PIX)then
        write(ounit,*)'0,0,',abs(scale)
        Call flush(6)
      end if
      call intsect(isoval(i), vdata, nx, ny, nz,
     &      nx0, ny0, nz0, nxx, nyy, nzz, nvert, ntri,
     &      vlist, mvert, x0, y0, z0, dx, dy, dz,
     &  volume, volcomp, tags, ntags)
      if(volcomp)then
        if((scale*isoval(i)).gt.0)then
          invol=volume
          outvol=bigvol-volume
        else
          outvol=volume
          invol=bigvol-volume
        end if
        write(*,'(1x,a,f12.3,a,f12.3)')'volume: inside = ',invol,
     &        ' outside = ',outvol
        Call flush(6)
C        if(prpvol .and. ntags.gt.0)then
C          write(*,*)'   Tag   Partial Volumes'
C          Call flush(6)
C           do j=0,ntags
C           if(pvol(j).ne.0)Then
C            write(*,'(1x,i5,3x,f12.3)')j, pvol(j)
C            Call flush(6)
C          End If
C          end do
C        end if
C      end if
c
C      call vsort(nvert, ntri,  vlist, mvert, tlist)
c
       call interp(vlist, nvert, nx, ny, nz,
     &      vdata, isoval(i), x0, y0, z0, dx, dy, dz, scale, area,
     &      volcomp, normcomp, tags, ntags, ounit, outfile, tm)
      write(*,'(1x,i5,a)')ntri, ' triangles created.'
      Call flush(6)
c
      if(volcomp)then
        write(*,'(1x,a,f12.3)')'total surface area = ', area
        Call flush(6)
C       if(prpvol .and. ntags.gt.0)then
C         write(*,*)'   Tag   Partial Areas'
C         Call flush(6)
C          do j=0,ntags
C          if(parea(j).ne.0)Then
C           write(*,'(1x,i5,3x,f12.3)')j, parea(j)
C           Call flush(6)
C         End If
C         end do
C         write(*,*)
C         Call flush(6)
C       end if
C     end if
c
      if(outfile.eq.QIX)then
        write(ounit)-1.,0.,0.
      else if(outfile.eq.PIX)then
        write(ounit,*)'-1,0,0'
      end if
      end do
c
c      could be more data sets
c      or could be sending data to gramps
c
      close(unit=ounit)
C 
C Signal end of computation, and go back for more
      Write(*,'(a)')'DONE'
      Call flush(6)
      Goto 25
C
 999  continue
      close(unit=punit)
      stop
      end
