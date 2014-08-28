      Subroutine intsect(isoval, vdata, nx, ny, nz,
     &      nx0, ny0, nz0, nxx, nyy, nzz, nvert, ntri,
     &  vlist, MAXVERT, x0, y0, z0, dx, dy, dz,
     &  volume, volcomp, tags, ntags)
c
      implicit none
c
c find edge intersections
c
      integer nx, ny, nz
      integer nx0, ny0, nz0, nxx, nyy, nzz
      real isoval
      real vdata(nx, ny, nz)
      integer tags(nx, ny, nz)
      integer nvert, ntri, MAXVERT
      integer vlist(3,2,MAXVERT)
      real x0, y0, z0, dx, dy, dz
      real volume
      integer ntags
C      real pvol(0:ntags)
c
      integer i, j, k, ii, jj, itag
      integer edge, vx, vi
      real tv, tetvol, tvtot, cubvol
      logical volcomp
      integer ounit, junit
c
      integer nntri(254)
      integer ncases
      parameter (ncases=10)
      integer elist(3,ncases,254)
      integer vedge(6,12)
      integer vtex(6,8)
      integer MAXTET
      PARAMETER (MAXTET=8)
      integer tetedge(4,MAXTET,127)
      integer nntet(127)
#include "edges.inc"
#include "tets.inc"
c
      ounit=18
      junit=ounit+1
      nvert=0
      ntri=0
      cubvol=dx*dy*dz
      volume=0.0
c
      do 30 i=nx0, nxx-1
      do 30 j=ny0, nyy-1
      do 30 k=nz0, nzz-1
      vx=0
      if(vdata(i,   j,   k  ).ge.isoval)vx=vx+1
      if(vdata(i+1, j,   k  ).ge.isoval)vx=vx+2
      if(vdata(i+1, j+1, k  ).ge.isoval)vx=vx+4
      if(vdata(i,   j+1, k  ).ge.isoval)vx=vx+8
      if(vdata(i,   j,   k+1).ge.isoval)vx=vx+16
      if(vdata(i+1, j  , k+1).ge.isoval)vx=vx+32
      if(vdata(i+1, j+1, k+1).ge.isoval)vx=vx+64
      if(vdata(i,   j+1, k+1).ge.isoval)vx=vx+128
c
      If (ntags .gt. 0) Then
        itag=tags(i,j,k)
        if(itag.lt.0 .or. itag.gt.ntags)itag=0
      End If
      if(vx.eq.0)then
        continue
      else if(vx.eq.255)then
        volume=volume+cubvol
C       if(ntags.gt.0) pvol(itag)=pvol(itag) + cubvol
      else 
        do 20 jj=1,nntri(vx)
        do 10 ii=1,3
        nvert=nvert+1
        if(nvert.gt.MAXVERT)then
          write(0,*)'**error** too many triangles!'
          nvert=ntri*3
          return
        end if
        edge=elist(ii,jj,vx)
        vlist(1,1,nvert)=i+vedge(1,edge)
        vlist(2,1,nvert)=j+vedge(2,edge)
        vlist(3,1,nvert)=k+vedge(3,edge)
        vlist(1,2,nvert)=i+vedge(4,edge)
        vlist(2,2,nvert)=j+vedge(5,edge)
        vlist(3,2,nvert)=k+vedge(6,edge)
 10        continue
        ntri=ntri+1
 20        continue
        if(vx.gt.127)then
          vi=255-vx
        else
          vi=vx
        end if
        if(volcomp)then
c          write(junit,*)'24,0,3'
c          do edge=1,12
c          write(junit,*)
c     &          x0 + (i+vedge(1,edge)-1)*dx,
c     &          y0 + (j+vedge(2,edge)-1)*dy,
c     &          z0 + (k+vedge(3,edge)-1)*dz
c          write(junit,*)
c     &          x0 + (i+vedge(4,edge)-1)*dx,
c     &          y0 + (j+vedge(5,edge)-1)*dy,
c     &          z0 + (k+vedge(6,edge)-1)*dz
c          end do
          tvtot=0.0
          do 25 jj=1,nntet(vi)
          tv = tetvol(tetedge(1,jj,vi), x0, y0, z0, dx, dy, dz,
     &          vedge, vtex, isoval, vdata, nx, ny, nz, i, j, k)
          if(vx.gt.127)then
            tvtot=tvtot-tv
          else
            tvtot=tvtot+tv
          end if
 25          continue
          if(tvtot.lt.0)tvtot=cubvol+tvtot
c          write(0,*)'partial volume = ',tvtot
          volume=volume+tvtot
C          if(ntags.gt.0) pvol(itag)=pvol(itag) + tvtot
        else
          volume=volume + (cubvol/2.)
C          if(ntags.gt.0) pvol(itag)=pvol(itag) + (cubvol/2.)
        end if
      end if
c
 30      continue
c
#ifdef SHOWTETS
      write(junit,*)'-1,0,0'
      write(ounit,*)'-1,0,0'
#endif
      return
      end
c
       Subroutine interp(vlist, nvert, nx, ny, nz,
     &      vdata, isoval, x0, y0, z0, dx, dy, dz, scale, area,
     &      volcomp, normcomp, tags, ntags, ounit, outfile, tm)
c
      Implicit none
c
c interpolate along edges and compute normals
c
      integer nx, ny, nz
       integer vlist(3,2,*)
      integer nvert
      integer ntri
      integer xtri
      real vdata(nx,ny,nz)
      real isoval
      real x0, y0, z0, dx, dy, dz
      real scale, area
      integer ounit
      logical volcomp, normcomp
      integer outfile
      integer ntags
C     real parea(0:ntags)
      integer tags(nx,ny,nz)
      real tm(3,3)
c
      real dv,dd,du
      real x(3),y(3),z(3)
      real xn(3),yn(3),zn(3)
      real crd(3,3), nml(3,3), drc(3,3), lmn(3,3)
      integer xa,ya,za,xb,yb,zb
      real xna,xnb,yna,ynb,zna,znb
      integer i,j,k, ldm2
      integer itag, jtag(2,3)
      real ta, triarea, qarea, estlen
      integer uarea
      integer PIX, QIX
      PARAMETER (PIX=1)
      PARAMETER (QIX=2)
C      PARAMETER (DB=3)
      External Matmul
c
      if((scale*isoval).gt.0)then
        if(normcomp)then
          ldm2=-2
        else
          ldm2=-3
        end if
      else
        if(normcomp)then
          ldm2=1
        else
          ldm2=3
        end if
      end if
c
      area=0.
      ntri = 0;
      do i=1,nvert,3
      do j=1,3
       xa=vlist(1,1,i+j-1)
       ya=vlist(2,1,i+j-1)
       za=vlist(3,1,i+j-1)
       xb=vlist(1,2,i+j-1)
       yb=vlist(2,2,i+j-1)
       zb=vlist(3,2,i+j-1)
      If (ntags.gt.0)Then
        jtag(1,j)=tags(xa,ya,za)
        jtag(2,j)=tags(xb,yb,zb)
      End If
c      if(jtag(1,j).gt.7000)write(0,*)xa,ya,za,tags(xa,ya,za)
c      if(jtag(2,j).gt.7000)write(0,*)xb,yb,zb,tags(xb,yb,zb)
      dd = vdata(xb, yb, zb) - vdata(xa, ya, za) 
      du = isoval - vdata(xa, ya, za)
      If (dd .ne. 0) Then
        dv = du / dd
      Else
        dv = 0
      End If
      if( xb.eq.xa )then
        x(j)=x0 + (xa - 1) * dx
      else
        x(j)=x0 + (xa - 1 + dv) * dx
      end if
      if( yb.eq.ya )then
        y(j)=y0 + (ya - 1) * dy
      else
        y(j)=y0 + (ya - 1 + dv) * dy
      end if
      if( zb.eq.za )then
        z(j)=z0 + (za - 1) * dz
      else
        z(j)=z0 + (za - 1 + dv) * dz
      end if
c Normals
      if(normcomp)then
        if(xa.eq.1)then
          xna=(vdata(xa+1,ya,za)-vdata(xa  ,ya,za))
        else if(xa.eq.nx)then
          xna=(vdata(xa  ,ya,za)-vdata(xa-1,ya,za))
        else
          xna=(vdata(xa+1,ya,za)-vdata(xa-1,ya,za)) * 0.5
        end if
        if(ya.eq.1)then
          yna=(vdata(xa,ya+1,za)-vdata(xa,ya  ,za))
        else if(ya.eq.ny)then
          yna=(vdata(xa,ya  ,za)-vdata(xa,ya-1,za))
        else
          yna=(vdata(xa,ya+1,za)-vdata(xa,ya-1,za)) * 0.5
        end if
        if(za.eq.1)then
          zna=(vdata(xa,ya,za+1)-vdata(xa,ya,za  ))
        else if(za.eq.nz)then
          zna=(vdata(xa,ya,za  )-vdata(xa,ya,za-1))
        else
          zna=(vdata(xa,ya,za+1)-vdata(xa,ya,za-1)) * 0.5
        end if
        if(xb.eq.1)then
          xnb=(vdata(xb+1,yb,zb)-vdata(xb  ,yb,zb))
        else if(xb.eq.nx)then
          xnb=(vdata(xb  ,yb,zb)-vdata(xb-1,yb,zb))
        else
          xnb=(vdata(xb+1,yb,zb)-vdata(xb-1,yb,zb)) * 0.5
        end if
        if(yb.eq.1)then
          ynb=(vdata(xb,yb+1,zb)-vdata(xb,yb  ,zb))
        else if(yb.eq.ny)then
          ynb=(vdata(xb,yb  ,zb)-vdata(xb,yb-1,zb))
        else
          ynb=(vdata(xb,yb+1,zb)-vdata(xb,yb-1,zb)) * 0.5
        end if
        if(zb.eq.1)then
          znb=(vdata(xb,yb,zb+1)-vdata(xb,yb,zb  ))
        else if(zb.eq.nz)then
          znb=(vdata(xb,yb,zb  )-vdata(xb,yb,zb-1))
        else
          znb=(vdata(xb,yb,zb+1)-vdata(xb,yb,zb-1)) * 0.5
        end if
c
        xn(j)=(xna + (xnb-xna)*dv)/dx
        yn(j)=(yna + (ynb-yna)*dv)/dy
        zn(j)=(zna + (znb-zna)*dv)/dz
        estlen=abs(xn(j))+abs(yn(j))+abs(zn(j))
        xn(j)=xn(j)/estlen
        yn(j)=yn(j)/estlen
        zn(j)=zn(j)/estlen
        if(dv.lt.0 .or. dv.gt.1)then
          write(0,*)'**error** 0<dv<1 violated!',i,dv,xa,ya,za
        end if
      end if
c  store data in linear array to be sent to gramps (possibly).
c      end of (3) triangle points
C Orthogonalize
      crd(1,j)=x(j)
      crd(2,j)=y(j)
      crd(3,j)=z(j)
      Call Matmul(crd(1,j), tm, drc(1,j))
      If(normcomp)Then
        nml(1,j)=xn(j)
        nml(2,j)=yn(j)
        nml(3,j)=zn(j)
        Call Matmul(nml(1,j), tm, lmn(1,j))
      End If
      end do
c
      if(volcomp)then
        ta=triarea(x,y,z)
C        if (ta < 0.0001) then
C          write(0,*)'triangle area = ',ta
C          cycle
C        end if
        area=area+ta
        uarea=0
        do k=1,2
        do j=1,3
        If (ntags.gt.0) itag=jtag(k,j)
        if(itag.lt.0 .or. itag.gt.ntags)itag=0
c divide up area according to tags on the 6 vertices
        if(itag.gt.0)Then
C         parea(itag) = parea(itag) + ta/6.
c some vertices may be un-tagged
        else 
          uarea=uarea+1
        end if
        end do
        end do
c divide up area belonging to untagged vertices among uarea-6 remaining verts
c all vertices may be unowned? when uarea=6
        if(uarea.eq.6)then
C         parea(0) = parea(0) + ta
        else if(uarea.gt.0 .and. uarea.lt.6)then
c          write(0,'(7i5)')uarea, jtag
          qarea = ((ta/6.)*uarea) / (6-uarea)
          do k=1,2
          do j=1,3
          If (ntags.gt.0) itag=jtag(k,j)
          if(itag.lt.0 .or. itag.gt.ntags)itag=0
C         if(ntags.gt.0)parea(itag) = parea(itag) + qarea
          end do
          end do
        end if
      else
        uarea=0
      end if
      if(outfile.eq.QIX)then
        write(ounit)3.,6.,float(ldm2)
        if(normcomp)then
c          write(ounit)(x(j),y(j),z(j),xn(j),yn(j),zn(j),j=1,3)
          write(ounit)((drc(k,j),k=1,3),(lmn(k,j),k=1,3),j=1,3)
        else
c          write(ounit)(x(j),y(j),z(j),j=1,3)
          write(ounit)((drc(k,j),k=1,3),j=1,3)
        end if
      else if(outfile.eq.PIX)then
        write(ounit,'(2(i1,'',''),i2)')3,6,ldm2
        if(normcomp)then
          write(ounit,'((2(g12.5,'',''),g12.5))')
C     &          ( (drc(k,j),k=1,3), (lmn(k,j),k=1,3), j=1,3)
     &          (x(j),y(j),z(j),xn(j),yn(j),zn(j),j=1,3)
        else
          write(ounit,'(2(g12.5,'',''),g12.5)')
     &          ( (drc(k,j),k=1,3), j=1,3)
c     &          (x(j),y(j),z(j),j=1,3)
        end if
      else
c callback addtri to add to db
        ntri = ntri + 1
        if (normcomp) then
          do j=1,3
            xtri = ntri*10 + j
            call addtrinml(itag,xtri,x(j),y(j),z(j),xn(j),yn(j),zn(j))
          end do
        else
          do j=1,3
            xtri = ntri*10 + j
            call addtri(itag,xtri,x(j),y(j),z(j))
          end do
        end if
      end if

c      end of all nvert vertices
      end do
c
      return
      end
c
      real function tetvol(tetedge, x0, y0, z0, dx, dy, dz,
     &  vedge, vtex, isoval, vdata, nx, ny, nz, ii, jj, kk)
c
      implicit none
      integer tetedge(4)
      real x0, y0, z0, dx, dy, dz
      integer vedge(6,12)
      integer vtex(6,8)
      integer nx,ny,nz
      real isoval
      real vdata(nx,ny,nz)
      integer ii,jj,kk
c
      integer j
      real dv,dd,du
      integer edge
      real volume
      integer xa, ya, za, xb, yb, zb
      real x(4), y(4), z(4)
      real v(3,3)
c
      do 10 j=1,4
        edge=tetedge(j)
        if(edge.gt.0)then
          xa=ii+vedge(1,edge)
          ya=jj+vedge(2,edge)
          za=kk+vedge(3,edge)
          xb=ii+vedge(4,edge)
          yb=jj+vedge(5,edge)
          zb=kk+vedge(6,edge)
        else
          xa=ii+vtex(1,-edge)
          ya=jj+vtex(2,-edge)
          za=kk+vtex(3,-edge)
          xb=xa
          yb=ya
          zb=za
        end if
C       dv=( isoval            - vdata(xa, ya, za) ) /
C    &         ( vdata(xb, yb, zb) - vdata(xa, ya, za) )
        dd = vdata(xb, yb, zb) - vdata(xa, ya, za) 
        du = isoval - vdata(xa, ya, za)
        If (dd .ne. 0) Then
          dv = du / dd
        Else
          dv = 0
        End If
        if( xb.eq.xa )then
                x(j)=x0 + (xa - 1) * dx
        else
                x(j)=x0 + (xa - 1 + dv) * dx
        end if
        if( yb.eq.ya )then
                y(j)=y0 + (ya - 1) * dy
        else
                y(j)=y0 + (ya - 1 + dv) * dy
        end if
        if( zb.eq.za )then
                z(j)=z0 + (za - 1) * dz
        else
                z(j)=z0 + (za - 1 + dv) * dz
        end if
 10      continue
c
c      volume is triple product of v1,v2,v3 where vn are
c      vectors between arbitrary "origin" vertex (#1)
c      and remaining 3 tetrahedron vertices.
c
      volume=0.0
      do j=1,3
      v(1,j)=x(j+1)-x(1)
      v(2,j)=y(j+1)-y(1)
      v(3,j)=z(j+1)-z(1)
      end do
      volume = v(1,1) * (v(2,2)*v(3,3) - v(2,3)*v(3,2)) -
     &         v(2,1) * (v(1,2)*v(3,3) - v(1,3)*v(3,2)) +
     &         v(3,1) * (v(1,2)*v(2,3) - v(1,3)*v(2,2))
      tetvol=volume/6.
c
      return
      end
c
      real function triarea(x,y,z)
c
      real x(3), y(3), z(3)
c
      real ax, ay, az, bx, by, bz
      real cpx, cpy, cpz
c
c      area is 1/2 the magnitude of the cross product AxB,
c      where A is vector from  point 2 to point 1 and
c      B is vector from point 3 to point 1
c      of the (3 point-duh) triangle.
c
      ax=x(2)-x(1)
      ay=y(2)-y(1)
      az=z(2)-z(1)
      bx=x(3)-x(1)
      by=y(3)-y(1)
      bz=z(3)-z(1)
c
      cpx=ay*bz-az*by
      cpy=az*bx-ax*bz
      cpz=ax*by-ay*bx
      triarea = 0.5*sqrt(cpx*cpx + cpy*cpy + cpz*cpz)
c
      return
      end
c
      Subroutine Matmul(pqr, t, xyz)
      Implicit None
      Real pqr(3), t(3,3), xyz(3)
C
      Integer i,j
C
      Do i=1,3
      xyz(i)=0.
      Do j=1,3
      xyz(i)=xyz(i) + pqr(j)*t(i,j)
      End Do
      End Do
C
      Return
      End
c
      Subroutine unit(tm)
C
      Implicit None
      Real tm(3,3)
C     
      Integer i,j
C     
        Do i=1,3
        Do j=1,3
        If(i.eq.j)Then
                tm(i,j)=1.0
        Else
                tm(i,j)=0.0
        End If
        End Do
        End Do
C     
      Return
      End
