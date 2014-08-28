/*
C *************************************************************
C This file contains source code for the gMol computer program
C  Copyright (C) 1981-2010 by TJ O'Donnell and
C  Copyright (C) 2005-2010 gNova, Inc.
C It is unlawful to modify or remove this copyright notice.
C See the accompanying LICENSE file for further information. 
C *************************************************************
*/
/* Routines to support socket i/o for gramps - Fortran callable */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/time.h>
#include <string.h>
#include <netdb.h>
#include <sys/stat.h>

int sock = 0;   /* the socket to accept the connections on */
int fdsock = 0; /* the file descriptor for socket i/o */

#define ENDMSG  -1
#define ENDPROG -2

#define GRAMPS_SOCK 5103

gethostname_(char *host_name, int lenhost) {
 gethostname(host_name, sizeof(host_name));
}

sockclientend()
{
/* clients use fdsock returned from socket call as the fd */
/* and don't use sock at all */
/*if(fdsock != 0){ shutdown(fdsock, 2); close(fdsock); }*/
if(fdsock != 0)close(fdsock);
fdsock = 0;
}
sockclientend_() { sockclientend(); }

int sockclientinit(char *hname, char *service)
{
	char host_name[100];
	struct  sockaddr_in name;
	struct hostent *hp, *gethostbyname();
	struct servent *sp, *getservbyname();

	/*printf("%s %s\n", host_name, service); */
	if(strlen(hname) > 0)
	  strcpy(host_name, hname);
	else
	  gethostname(host_name, sizeof(host_name));

	if ((fdsock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Opening streams socket");
		return(-1);
	}

	name.sin_family = AF_INET;

	/* connect socket using name and port passed in to this routine */
	hp = gethostbyname(host_name);
	if (hp == 0)
	{
		fprintf(stderr,"%s: unknown host.\n",host_name);
		close(fdsock);return(-2);
	}
	bcopy(hp->h_addr, &name.sin_addr, hp->h_length);

	sp = getservbyname( service, (char *)0 );
	if (sp == 0)
		{
/*		fprintf(stderr,"%s: unknown service.\n", service);
		fprintf(stderr,"Trying GRAMPS_SOCK\n"); */
		name.sin_port = htons(GRAMPS_SOCK);
		}
	else
		name.sin_port = sp->s_port;

/*
	printf("%s at %s on port %d\n", service, inet_ntoa(name.sin_addr),
			ntohs(name.sin_port));
*/

	if (connect (fdsock,(struct sockaddr *)&name, sizeof(name)) == -1)
	{
/*		fprintf (stderr, "Can't connect.  Make sure that %s is running on %s\n", service, host_name); */
		close(fdsock);return(-3);
	}

	return(fdsock);
}
int sockclientinit_(char *hname, char *service, int hlen, int slen) {
char host_name[100];
char service_name[100];

if(hlen > 0)strncpy(host_name, hname, hlen);
host_name[hlen]='\0';
if(slen > 0)strncpy(service_name, service, slen);
service_name[slen]='\0';
return(sockclientinit(host_name, service_name));
}


sockserverend(int how)
{
 if(sock   != 0){
 /*
  fprintf(stderr, "socket shutdown %x\n", shutdown(sock, 2));
  fprintf(stderr, "socket closed %x\n", close(sock));
 */
  shutdown(sock, how);
  close(sock);
 }
 if(fdsock != 0)close(fdsock);
 fdsock = sock = 0;
}
sockserverend_() { sockserverend(2); }


int sockserverinit(char *service)
{
	struct sockaddr_in name;
	struct servent *sp, *getservbyname();
	int readret;
	char message[256];

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Opening streams socket");
		return(-1);
	}

	name.sin_family = AF_INET;
	name.sin_addr.s_addr = INADDR_ANY;  /* wildcard local host address. */

	sp = getservbyname( service, (char *)0 );
	if (sp == 0)
		{
		fprintf(stderr,"%s: unknown service.\n", service);
		fprintf(stderr,"Trying GRAMPS_SOCK\n");
		name.sin_port = GRAMPS_SOCK;
		}
	else
		name.sin_port = sp->s_port;

	/*printf("%s port =  %d\n", service, ntohs(sp->s_port));*/


	if (bind (sock, (struct sockaddr *) &name, sizeof(name)) == -1)
	{
/*		perror ("bind failed"); */
		return (-2);
	}

	if (listen (sock, 2) == -1) {
		perror ("listen failed");
		return (-3);
	}
	fprintf (stderr,"Listening for connections on port %d\n",
		ntohs(name.sin_port));

	return(sock);

/*
	fprintf (stderr,"Waiting (select) for acceptance\n");
	while( message_wait(sock, 500000) == 0 ) ;
*/

}
int sockserverinit_(char *service, int slen) { sockserverinit(service); }

int sockaccept()
{
struct sockaddr_in from;
socklen_t fromlen = sizeof (from);
struct hostent *fromhost;
struct servent *fromserv;
char *p;

while ((fdsock = accept (sock, (struct sockaddr *)&from, &fromlen)) == -1) {
	perror ("accept failed....waiting");
	sleep(10);
	return(-1);
}

fprintf(stderr,"Accepted connection from client %s\n",
	       	inet_ntoa(from.sin_addr));
fromhost = gethostbyname((char *)inet_ntoa(from.sin_addr));
while (fromhost != NULL && (p=*fromhost->h_aliases) != NULL) {
 fprintf(stderr," alias <%s>\n", p);
}
fromserv = getservbyport(from.sin_port, "tcp");
if(fromserv != NULL)
	fprintf(stderr,"Accepted connection from service <%s>\n",
	fromserv->s_name);
/*fprintf(stderr,"FileDescriptor = %d\n", fdsock);*/
return(fdsock);

}

message_wait(int fd, int msec)
/* Routine to return 0 if nothing ready to be read on file-descriptor fd
   else it returns -1 for error, 1 for data ready to read.
   Wait upto msec milliseconds before deciding
   there is nothing to read */
{
int i;
fd_set readset;
struct timeval timeout;

/*
timeout.tv_sec  = 1;
timeout.tv_usec = 0;
*/
if (fd < 0) return 0;

timeout.tv_sec  = 0;
/* usec is microseconds */
timeout.tv_usec = msec*1000;
FD_ZERO(&readset);
FD_SET(fd, &readset);

/*
printf("fd = %d\n", fd);
printf("fdsetfd = %d\n", FD_ISSET(fd, &readset) );
*/

//if( i=select(FD_SETSIZE, &readset, (fd_set *)0, (fd_set *)0, &timeout) < 0)
i=select(1, &readset, (fd_set *)0, (fd_set *)0, &timeout);
fprintf(stderr,"select %d\n",i);
if( i < 0) {
	perror("in message_wait");
	return(-1);
}
if (FD_ISSET(fd, &readset))return(1);else return(0);
}

sockcheck(int msec)
{
int i;

/* waits on sock (after listen and before accept)
   or waits on fdsock for some data if acceptance has occurred(fdsock != 0).
   message_wait returns -1 for error, 0 for no data ready, 1 for data ready
 */
 if( sock != 0 && fdsock == 0 )
        {
        i = message_wait(sock, msec);
        if(i==1){ sockaccept(); i = 2; }
        }
 else if( sock != 0 && fdsock != 0 )
        i = message_wait(fdsock, msec);
 else if( sock == 0 && fdsock != 0 )
        i = message_wait(fdsock, msec);
 else
        i = -1; /* error */

return(i);
}

int sockcheck_(int *msec) { return (sockcheck(*msec)); }

int sockget(message, msglen)
char *message;
int msglen;
{
int msgsize;
int i, pad, nbytes;

msgsize = read(fdsock, &nbytes, sizeof(int));
if( msgsize <= 0 ) return(msgsize);
if(nbytes > msglen)
  {
  nbytes = msglen;
  fprintf(stderr, "sockget: message too long\n");
  }
else if(nbytes < 0)
  return(nbytes);
else if(nbytes == 0)
  {
  pad = 0;
  msgsize = msglen;  /* all blanks! */
  }
else
  {
  msgsize = read(fdsock, message, nbytes);
  if(msgsize > 0) pad = msgsize;
  else            pad = 0;
  }
/* pad return string with blanks to keep Fortran happy */
for(i = pad; i < msglen; i++) message[i]=' ';
/* but return trim size, or error from read */
return( msgsize );
}
int sockget_(char *message, int msglen) { return(sockget(message, msglen)); }

sockendprog()
{
int writeret;
int msgend=ENDPROG;

if(fdsock)writeret = write(fdsock, &msgend, sizeof(msgend));
}
sockendprog_() { sockendprog(); }
sockquit_() { sockendprog(); }

sockend()
{
int writeret;
int msgend=ENDMSG;

writeret = write(fdsock, &msgend, sizeof(msgend));
}
sockend_() { sockend(); }

int socksend(char *message)
{
int writeret;
int msglen;
static int noclient=0;

msglen = strlen(message);
if (fdsock == 0) {
	if (noclient == 0) {
          fprintf(stderr, "no socket client for output.\n");
	  noclient++;
	}
        fprintf(stderr, "%s\n", message);
} else {
  writeret = write(fdsock, &msglen, sizeof(int));
  if(msglen > 0)writeret = write(fdsock, message, msglen);
  else          writeret = 0;
}

/* return the nbytes written */
return( writeret );
}
int socksend_(char *message, int msglen) {
char msgtmp[100];

if(msglen > 0)strncpy(msgtmp, message, msglen);
msgtmp[msglen] = '\0';
return(socksend(msgtmp));
}

int trim(char *string)
/* returns number of characters in string before trailing blanks */
/* returns 0 if string is all blank */
{
int i;
for (i=strlen(string)-1; i >= 0; i--)
	if( string[i] != ' ' )return(i+1);

return(0);
}

#define MBUFSIZ 1000

int qixsend(float *data, float nelem)
{
int nbytes;
int nwrite;

nbytes = nelem   * sizeof (float);
nwrite = write(fdsock, &nbytes, sizeof(long) );
if(nwrite != sizeof(long))
  fprintf(stderr,"qixsend: only %d bytes of %d written\n",nwrite,sizeof(long));
if(nbytes>0)
  {
  nwrite = write(fdsock, data, nbytes);
  if(nwrite != nbytes)
    fprintf(stderr, "qixsend: only %d bytes of %d written\n", nwrite, nbytes);
  }
return(0);
}
int qixsend_(float *data, float *nelem) { return(qixsend(data, *nelem)); }

int qixread(char *data, long nelem)
/* float *data; screwed up pointer arithmetic on data+nread */
{
int nbytes;
int nread;
int nleft;
int ntotal;

nread = read(fdsock, &nbytes, (sizeof nbytes) );
if( nbytes <= 0 )return(-1);
if( nelem * sizeof(float) < nbytes )return(0);
nread = 0; ntotal = 0;
for(nleft = nbytes; nleft > 0; nleft -= nread)
  ntotal += (nread = read(fdsock, (data+ntotal), nleft) );
return(ntotal / sizeof(float) );
}
int qixread_(char *data, long *nelem) { return(qixread(data, *nelem)); }
