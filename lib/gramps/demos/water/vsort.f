	integer x,y,z
	integer xmin,ymin,zmin
c
	nfr=0
 10	continue
	nfr=nfr+1
	zmin=0
	read(*,*,end=99)n,ldm1,ldm2
	if(n.ne.625)stop 'bad n'
	do i=1,n
	read(*,*)x,y,z
	if(z.lt.zmin)then
		zmin=z
		ymin=y
		xmin=x
	end if
	end do
	write(*,*)'frame ',nfr,'.  min @ ',xmin,ymin,zmin
c
	read(*,*)n,ldm1,ldm2
	do i=1,n
	read(*,'(1x)')
	end do
	read(*,*)n,ldm1,ldm2
	if(n.ne.-1)stop 'bad -1'
c
	goto 10
c
 99	continue
	stop 'done'
	end
