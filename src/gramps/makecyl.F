C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
	SUBROUTINE G0CYLMAK(NTV,buf)
	Implicit None
	Integer ntv
	Real buf(3,2,ntv+1,2)
	Real cbuf(3,ntv+1)
C
	Integer nx,ny,nz
	Real angle,dangle,cx,cy,cz
	Integer i,j,k
	Real Pi
	PARAMETER (PI=3.14159265358979323846)
c
c	dangle=360./ntv
	dangle=2*PI/ntv
	nx=ntv+1
	ny=2
c
	cz=0.
	do j=1,ny
		angle=0.
		do i=1,nx
		cx=cos(angle)
		cy=sin(angle)
		buf(1,1,i,j)=cx
		buf(2,1,i,j)=cy
		buf(3,1,i,j)=cz
		buf(1,2,i,j)=cx
		buf(2,2,i,j)=cy
		buf(3,2,i,j)=0.
		angle=angle-dangle
		end do
		cz=1.
	end do
c
	Return

	Entry G0CYLMAKBASE(NTV,cbuf)
C
C Just the circular base
C
	dangle=2*PI/ntv
	nx=ntv+1
c
	angle=0.
	do i=1,nx
	  cx=cos(angle)
	  cy=sin(angle)
	  cbuf(1,i)=cx
	  cbuf(2,i)=cy
	  cbuf(3,i)=0.
	  angle=angle-dangle
	end do
c
	Return
	End
