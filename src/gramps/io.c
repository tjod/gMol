/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
//#include <stropts.h>
#include <sys/time.h>
#include <sys/types.h>
//#include <sys/ioctl.h>

#ifdef USE_SOCK
int iopeek(int msec)
{

 fd_set rfds;
 struct timeval tv;
 int retval;

 /* Watch stdin (fd 0) to see when it has input. */
 FD_ZERO(&rfds);
 FD_SET(0, &rfds);
 /* Wait zero seconds. poll */
 tv.tv_sec = 0;
 if (msec < 1) msec = 10;
 tv.tv_usec = msec;

 retval = select(1, &rfds, NULL, NULL, &tv);
 /* Don't rely on the value of tv now! */

 if (retval)
     /* printf("Data is available now.\n"); */
     /* FD_ISSET(0, &rfds) will be true. */
     return(1);
 else
     /* printf("No data within five seconds.\n"); */
     return(0);

}
#endif

void cdone_(){return;}

cinit(file)

int *file;
{

/* printf(">gramps>", 8); */
fflush(stdout);
/*return(STANDARD_OUT = 1)*/
return(1);
}

void cwrite_(line, llen)

char *line;
int llen;
{
if(line[0] == '>') fprintf(stdout, "%.*s",   llen, line);
else		   fprintf(stdout, "%.*s\n", llen, line);
fflush(stdout);
}
