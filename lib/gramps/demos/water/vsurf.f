	integer x,y,z
c
	nfr=1
c
 1	continue
	m=0
	read(*,*,end=999)n,ldm1,ldm2
	write(*,100)625,7,4
 10	continue
	if(m.ne.0)read(*,*,end=999)n,ldm1,ldm2
	if(n.lt.0)then
		nfr=nfr+1
		nn=25
		m=(nn-1)*(nn-1)*2
		write(*,100)m,8,1
		do j=1,nn*(nn-1),nn
		do i=j,j+ nn-2
		write(*,100)i, nn+1+i, nn  +i
		write(*,100)i,    1+i, nn+1+i
		end do
		end do
		write(*,100)n,ldm1,ldm2
 		if(nfr.ge.3)stop '2 frame test'
		goto 1
	end if
	m=m+1
	do j=1,n
	read(*,*)x,y,z
	if( (mod(j,2).eq.1) .and. (mod(m,2).eq.1) )write(*,100)x,y,z
	end do
	goto 10
c
c
 100	format(i7,',',i7,',',i7)
 999	continue
	stop 'end of file'
	end
