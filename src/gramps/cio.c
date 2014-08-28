/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

static int fd;

fcopen_(file, fsize, flen)
char *file;
int *fsize;
int flen;
{
file[*fsize]='\0';
fd = open(file, O_CREAT|O_TRUNC|O_RDWR);
return(fd);
}

fcclose_() { close(fd); }

fcwrite_(buffer, bufsize)
int *buffer;
int *bufsize;
{
int nbytes, i;
nbytes = *bufsize * sizeof(int);
i = write(fd, buffer, nbytes);
return(i);
}

fcread_(buffer, bufsize)
int *buffer;
int *bufsize;
{
int nbytes, i;
nbytes = *bufsize * sizeof(int);
i = read(fd, buffer, nbytes);
return(i);
}
