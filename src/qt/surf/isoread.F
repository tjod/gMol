      Subroutine readg94(iunit,x0,y0,z0,xx,yy,zz,nx,ny,nz,xyz,title)
      Implicit None
      Integer iunit
      Real x0,y0,z0,xx,yy,zz
      Integer nx,ny,nz
      Character*(*) xyz, title
C
      Character*1 junk
      Integer natoms,i,atnum
      Real dx,dy,dz,djunk
      Real charge,x,y,z
C
      Read(iunit,'(a)')title
C Don't need density type
      Read(iunit,'(a)')junk
      Read(iunit,*)natoms,x0,y0,z0
      Read(iunit,*)nx,dx,djunk,djunk
      Read(iunit,*)ny,djunk,dy,djunk
      Read(iunit,*)nz,djunk,djunk,dz
C Don't need atom coordinates
      Do i=1,iabs(natoms)
      Read(iunit,*)atnum,charge,x,y,z
      Write(*,*)atnum,charge,x,y,z
      End Do
      if (natoms < 0) Read(iunit,'(a)') junk
C
      xx=x0+dx*nx
      yy=y0+dy*ny
      zz=z0+dz*nz
      xyz='xyz'
C
      Return
      End
c
      subroutine readdata(vdata, tags, nx, ny, nz, ntags, iunit)
c
      implicit none
      integer nx, ny, nz, iunit
      real vdata(nx,ny,nz)
      integer tags(nx,ny,nz)
      integer ntags
      integer i,j,k
c
      read(iunit,'(6E13.5)')(((vdata(i,j,k),k=1,nz),j=1,ny),i=1,nx)
      If(ntags.gt.0) Then
        read(iunit,*,end=999)(((tags(i,j,k),k=1,nz),j=1,ny),i=1,nx)
      End If
c
 999  continue
      return
      end
c
